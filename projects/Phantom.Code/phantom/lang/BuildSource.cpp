// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "BuildSource.h"

#include "BuildSystem.h"
#include "CppLite.h"
#include "Label.h"
#include "Language.h"
#include "Message.h"
#include "Parser.h"
#include "Semantic.h"
#include "phantom/lang/Package.h"
#include "phantom/lang/Source.h"
#include "phantom/lang/SourceStream.h"
#include "phantom/utils/Path.h"
#include "phantom/utils/SmallSet.h"

#include <iostream>
#include <phantom/utils/Console.h>
#include <stdarg.h>

namespace phantom
{
namespace lang
{
BuildSource* BuildSource::Get(Source* a_pSource)
{
    return BuildSystem::Get()->getCompiledSource(a_pSource);
}

BuildSource::Session const& BuildSource::EmptyBuild()
{
    static BuildSource::Session s_EmptyBuild;
    return s_EmptyBuild;
}

void BuildSource::addBuildDependency(BuildSource* a_pSource)
{
    PHANTOM_ASSERT(m_bBuildInProgress);

    if (a_pSource != this)
    {
        currentBuild().addDependency(a_pSource);

        // if the dependency belongs to the same session
        if (a_pSource->m_bBuildInProgress)
        {
            // try to trigger parsing of the dependency now before this one continues
            a_pSource->_advanceParse(getParser()->getPass());
        }
    }
}

void BuildSource::addBuildDependency(Source* a_pSource)
{
    if (a_pSource->isNative())
        return;
    if (auto pCS = BuildSystem::Get()->getCompiledSource(a_pSource))
        addBuildDependency(pCS);
}

BuildSource::BuildSource(BuildSystem* a_pCompiler, SourceStream* a_pSourceStream)
    : m_pCompiler(a_pCompiler), m_pSourceStream(a_pSourceStream)
{
    m_InvalidBuild.m_pStatusMessage = new_<Message>(MessageType::Error, Path(a_pSourceStream->getPath()).stem());
}

bool BuildSource::hasError() const
{
    return getLastBuild().hasError();
}

bool BuildSource::Session::hasError() const
{
    return m_pStatusMessage == nullptr || m_pStatusMessage->getMostValuableMessageType() == MessageType::Error;
}

BuildSource::~BuildSource() {}

void BuildSource::_advanceParse(uint a_Pass)
{
    PHANTOM_ASSERT(m_bBuildInProgress);
    while (getParser()->getPass() < a_Pass)
    {
        getParser()->stepParse(getParser()->getPass() + 1);
    }
}

time_t BuildSource::outdate()
{
    if (m_bBuildInProgress)
        return 0;
    time_t time = m_uiLastValidBuildTime;
    m_uiLastValidBuildTime = 0;
    return time;
}

Message* BuildSource::error(const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = getCurrentBuild().m_pStatusMessage->message(MessageType::Error, a_Format, args);
    va_end(args);
    return pMessage;
}

Message* BuildSource::warning(const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = getCurrentBuild().m_pStatusMessage->message(MessageType::Warning, a_Format, args);
    va_end(args);
    return pMessage;
}

Message* BuildSource::error(const CodeRangeLocation& a_Location, const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = getCurrentBuild().m_pStatusMessage->messageD(MessageType::Error, a_Location, a_Format, args);
    // vprintf(a_Format, args);
    va_end(args);
    return pMessage;
}

Message* BuildSource::warning(const CodeRangeLocation& a_Location, const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = getCurrentBuild().m_pStatusMessage->messageD(MessageType::Warning, a_Location, a_Format, args);
    va_end(args);
    return pMessage;
}

BuildSource::Session BuildSource::createSession(Package* a_pPck, Source* a_pSource, Language* a_pLanguage,
                                                Message& a_Message)
{
    Session build;
    a_pSource->setSourceStream(m_pSourceStream);

    PHANTOM_ASSERT(a_pLanguage);
    build.m_pBuildSource = this;
    build.m_pSource = a_pSource;
    build.m_pPackage = a_pPck;
    ::time(&build.m_Time);
    build.m_pLanguage = a_pLanguage;
    build.m_pStatusMessage = new_<Message>(MessageType::Success, a_pSource->getName());
    build.m_pStatusMessage->setData(CodeRangeLocation(a_pSource, CodeRange()));
    a_Message.addChild(build.m_pStatusMessage);
    if (a_pLanguage)
    {
        build.m_pParser = a_pLanguage->createParser(a_pSource, build.m_pStatusMessage);
        build.m_pSemantic = build.m_pParser->getSemantic();
        build.m_PerPassDependencies.resize(build.m_pParser->getPassCount());
    }
    return build;
}

BuildSource::Session BuildSource::createSession(Package* a_pPck, Source* a_pSource, Message& a_Message)
{
    return createSession(a_pPck, a_pSource, m_pCompiler->getDefaultLanguage(), a_Message);
}

int BuildSource::beginBuild(Session a_Build)
{
    PHANTOM_ASSERT(!m_bBuildInProgress);
    m_PreviousBuildIndex = m_CurrentBuildIndex;
    m_bBuildInProgress = true;
    // undo current build
    if (m_CurrentBuildIndex > -1)
    {
        m_BuildStack[m_CurrentBuildIndex].undo();

        // trim undone builds
        while (m_CurrentBuildIndex != (m_BuildStack.size() - 1))
        {
            m_BuildStack.back().clear();
            m_BuildStack.pop_back();
        }
        if (m_LastBuildIndex > m_CurrentBuildIndex)
            m_LastBuildIndex = m_CurrentBuildIndex;
    }
    m_CurrentBuildIndex++;
    a_Build.getPackage()->addSource(a_Build.getSource());
    // push new build
    m_BuildStack.emplace_back(a_Build);
    return m_CurrentBuildIndex;
}

void BuildSource::abortBuild()
{
    currentBuild().m_pStatusMessage->error("build abort");
}

bool BuildSource::endBuild()
{
    PHANTOM_ASSERT(m_bBuildInProgress);
    m_bBuildInProgress = false;
    if (!hasSucceeded())
    {
        m_InvalidBuild.clear();
        m_InvalidBuild = m_BuildStack.back(); // backup last build as invalid build
        m_LastBuildIndex = -1;
        m_BuildStack.back().undo(); // undo current build
        m_BuildStack.pop_back();    // pop current build
        if (!m_BuildStack.empty())  // if any previous build, restore it
            m_BuildStack.back().redo();
        m_CurrentBuildIndex--;
        return false;
    }
    m_LastBuildIndex = m_CurrentBuildIndex;
    return true;
}

void BuildSource::restore(int _buildIndex)
{
    if (m_CurrentBuildIndex == _buildIndex)
        return;
    m_PreviousBuildIndex = m_CurrentBuildIndex;
    int count = _buildIndex - m_CurrentBuildIndex;
    if (m_CurrentBuildIndex != -1)
        m_BuildStack[m_CurrentBuildIndex].undo();
    m_CurrentBuildIndex += count;
    if (m_CurrentBuildIndex != -1)
        m_BuildStack[m_CurrentBuildIndex].redo();
    m_LastBuildIndex = m_CurrentBuildIndex;
}

void BuildSource::stampTime()
{
    time(&m_uiLastValidBuildTime);
}

bool BuildSource::hasSucceeded() const
{
    return getCurrentBuild().hasSucceeded();
}

bool BuildSource::Session::hasSucceeded() const
{
    SmallSet<const Session*> treated;
    return _hasSucceeded(treated);
}

bool BuildSource::Session::_hasSucceeded(SmallSet<const Session*>& treated) const
{
    if (!treated.insert(this).second) // already treated
        return true;
    if (hasError())
        return false;
    if (Source* pSource = getSource())
    {
        for (auto pDep : pSource->getDependencies())
        {
            if (BuildSource* pCS = BuildSystem::Get()->getCompiledSource(pDep))
            {
                if (!pCS->getCurrentBuild()._hasSucceeded(treated))
                    return false;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool BuildSource::_isOutdated(SmallSet<BuildSource const*>& _treated) const
{
    if (!_treated.insert(this).second)
        return false;
    if (m_uiLastValidBuildTime == 0 || m_pSourceStream->getLastChangeTime() > m_uiLastValidBuildTime)
        return true;
    if (Source* pSource = getLastBuild().getSource())
    {
        for (auto pDepSource : pSource->getDependencies())
        {
            if (BuildSource* pDepBuildSource = BuildSystem::Get()->getCompiledSource(pDepSource))
            {
                if (pDepBuildSource->m_uiLastValidBuildTime > m_uiLastValidBuildTime ||
                    pDepBuildSource->_isOutdated(_treated))
                    return true;
            }
        }
    }
    return false;
}

bool BuildSource::isOutdated() const
{
    SmallSet<BuildSource const*> treated;
    return _isOutdated(treated);
}

namespace
{
void _PrintMessage(phantom::lang::Message* _msg) {}
} // namespace

void BuildSource::dumpMessages()
{
    Functor<void(phantom::lang::Message*, int)> printBuildErrors = [&](phantom::lang::Message* pMsg, int lvl) {
        for (phantom::lang::Message* msg : pMsg->getChildren())
        {
            String messageText;
            auto   print = [&](String msg) {
                messageText += msg;
                std::cout << msg.c_str();
            };
            if (lvl == 0)
            {
                std::cout << phantom::console::fg_white;
                phantom::lang::CodeRangeLocation loc;
                if (msg->getData().as(&loc) && loc.source)
                {
                    print(phantom::Path::Absolute(loc.source->getSourceStream()->getPath()).genericString());
                    char buffer[16];
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
                    _itoa(loc.range.begin.line, buffer, 10);
#else
                    itoa(loc.range.begin.line, buffer, 10);
#endif
                    print('(' + String(buffer) + ")");
                    print(": ");
                }
                switch (msg->getType())
                {
                case phantom::MessageType::Error:
                    std::cout << phantom::console::fg_red;
                    print("error: ");
                    break;
                case phantom::MessageType::Warning:
                    std::cout << phantom::console::fg_yellow;
                    print("warning: ");
                    break;
                case phantom::MessageType::Information:
                    std::cout << phantom::console::fg_cyan;
                    print("note: ");
                    break;
                case phantom::MessageType::Success:
                    std::cout << phantom::console::fg_green;
                    print("success: ");
                    break;
                default:
                    break;
                }
            }

            std::cout << phantom::console::fg_gray;
            auto&&     msgTxt = msg->getText();
            StringView txt = StringView(msgTxt.data(), msgTxt.size());
            print(String(txt));
            print("\n");
#if defined(DRYAD_WIN)
            OutputDebugStringA(messageText.c_str());
#endif
            printBuildErrors(msg, lvl + 1);
        }
    };
    printBuildErrors(getLastBuild().getStatusMessage(), 0);
}

void BuildSource::Session::clear()
{
    if (m_pParser)
    {
        m_pParser->getLanguage()->destroyParser(m_pParser);
        m_pParser = nullptr;
    }
    if (m_pSource && m_pSource->getPackage())
    {
        PHANTOM_ASSERT(m_pPackage == m_pSource->getPackage());
        m_pPackage->removeSource(m_pSource);
        m_pPackage->deleteSource(m_pSource);
    }
    m_pSource = nullptr;
    m_pPackage = nullptr;
    m_pSemantic = nullptr;
}

BuildSource::Session::Session() : m_pSemantic(CppLite::Get()->getDefaultSemantic()) {}

void BuildSource::Session::undo()
{
    m_pPackage->removeSource(m_pSource);
}

void BuildSource::Session::redo()
{
    m_pPackage->addSource(m_pSource);
}

void BuildSource::Session::addDependency(BuildSource* a_pDep)
{
    m_PerPassDependencies[getParser()->getPass()].insert(a_pDep);
}

} // namespace lang

} // namespace phantom
