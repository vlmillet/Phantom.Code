// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "Compiler.h"

#include "CodeGenerator.h"
#include "CompiledSource.h"
#include "DebugInformation.h"
#include "Language.h"
#include "Message.h"
#include "Parser.h"
#include "Semantic.h"
#include "Solution.h"
#include "phantom/lang/Module.h"
#include "phantom/lang/SourceFile.h"
#include "phantom/lang/SourceStream.h"
#include "phantom/thread/SpinMutex.h"
#include "phantom/utils/Path.h"
#include "phantom/utils/Placement.h"
#include "phantom/utils/SmallMap.h"
#include "phantom/utils/SmallSet.h"

#include <phantom/lang/Application.h>
#include <phantom/lang/Module.h>
#include <phantom/lang/Package.h>
#include <phantom/lang/Plugin.h>
#include <phantom/lang/Source.h>
#include <thread>

namespace
{
auto getNow = []() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
    .count();
};

}

namespace phantom
{
namespace lang
{
extern StaticGlobal<Compiler> g_PHNTM_LNG_Compiler;

Compiler* Compiler::Get()
{
    PHANTOM_ASSERT(!!g_PHNTM_LNG_Compiler,
                   "Compiler instance not present, you probably forgot to create a phantom::lang::Main instance in "
                   "your 'main' application entry point");
    return &*g_PHNTM_LNG_Compiler;
}

Compiler::Compiler()
{
    PHANTOM_CONNECT(Application::Get(), moduleAdded, this, _onModuleAdded);
}

Compiler::~Compiler()
{
    PHANTOM_DISCONNECT(Application::Get(), moduleAdded, this, _onModuleAdded);
}

phantom::lang::CompiledSource* Compiler::_getOrCreateCompiledSource(SourceStream* a_pSourceStream)
{
    auto& pCS = m_CompiledSources[a_pSourceStream];
    if (pCS)
        return pCS;
    return pCS = new_<CompiledSource>(this, a_pSourceStream);
}

void Compiler::_deleteCompiledSource(SourceStream* a_pSourceStream)
{
    auto found = m_CompiledSources.find(a_pSourceStream);
    if (found != m_CompiledSources.end())
    {
        delete_<CompiledSource>(found->second);
        m_CompiledSources.erase(found);
    }
}

void Compiler::_undoSession(BuildSession& _session)
{
    for (auto& proj_session : _session.projects)
    {
        for (auto succeeded : proj_session.second->succeeded)
        {
            succeeded->m_PreviousBuildIndex = succeeded->m_CurrentBuildIndex--; // rewind build after
        }
        PHANTOM_EMIT buildSessionDone(*proj_session.second);
        for (auto succeeded : proj_session.second->succeeded)
        {
            succeeded->restoreCurrentSources(); // swap sources inside package
        }
        PHANTOM_EMIT buildSessionApplied(*proj_session.second);
    }
}

void Compiler::_redoSession(BuildSession& _session)
{
    for (auto& proj_session : _session.projects)
    {
        for (auto succeeded : proj_session.second->succeeded)
        {
            succeeded->m_PreviousBuildIndex = succeeded->m_CurrentBuildIndex++; // advance build first
        }
        PHANTOM_EMIT buildSessionDone(*proj_session.second);
        for (auto succeeded : proj_session.second->succeeded)
        {
            succeeded->restoreCurrentSources(); // swap sources inside package
        }
        PHANTOM_EMIT buildSessionApplied(*proj_session.second);
    }
}

SourceFile* Compiler::_getOrCreateSourceFile(StringView _path)
{
    auto found = m_UrlToSourceStream.find("file:///" + _path);
    if (found == m_UrlToSourceStream.end())
    {
        return static_cast<SourceFile*>(m_UrlToSourceStream["file:///" + _path] = new_<SourceFile>(_path));
    }
    PHANTOM_ASSERT(found->second->asFile());
    return static_cast<SourceFile*>(found->second);
}

Module* Compiler::_getOrCreateModule(StringView _path)
{
    String moduleName = Path(_path).filename();
    auto   found = m_CompiledModules.find(_path);
    if (found == m_CompiledModules.end())
    {
        if (auto pMod = Application::Get()->getModule(moduleName))
            return pMod;
        else
        {
            pMod = m_CompiledModules[_path] = Application::Get()->newModule(moduleName);
            Application::Get()->addModule(pMod);
            return pMod;
        }
    }
    return found->second;
}

void Compiler::_listSourceFiles(StringView _path, SmallVector<SourceFile*>& _sourceFiles)
{
    DirectoryEntries dirEntries;
    Path::ListDirectoryEntries(_path, dirEntries);

    for (DirectoryEntry const& dirEntry : dirEntries)
    {
        if (dirEntry.isRegularFile())
        {
            if (Language* pLang = getLanguageFromFileExtension(dirEntry.path().extension()))
            {
                SourceFile* pSF = _getOrCreateSourceFile(dirEntry.path().absolute().canonical().genericString());
                _sourceFiles.push_back(pSF);
            }
        }
        else if (dirEntry.isDirectory() && dirEntry.path().back().front() != '.')
        {
            _listSourceFiles(dirEntry.path().genericString(), _sourceFiles);
        }
    }
}

void Compiler::cleanupProject(StringView _path)
{
    SmallVector<SourceFile*> sfs;
    _listSourceFiles(_path, sfs);
    for (auto pSourceF : sfs)
    {
        _deleteCompiledSource(pSourceF);
    }
}

void Compiler::cleanBuild(StringView _path)
{
    SmallVector<SourceFile*> sfs;
    _listSourceFiles(_path, sfs);
    for (auto pSourceF : sfs)
        getCompiledSource(pSourceF)->outdate();
}

BuildSessionId Compiler::newSession(ArrayView<String> _projectsSearchPaths, Options const& a_Options)
{
    m_BuildSessions.resize(m_CurrentSessionId + 1); // ensure we erase previous build sessions
    BuildSessionId sessionid = ++m_CurrentSessionId;
    m_BuildSessions.resize(m_CurrentSessionId + 1);
    BuildSession*& session = m_BuildSessions.back();
    session = new_<BuildSession>();
    session->options = a_Options;
    for (auto& searchP : _projectsSearchPaths)
    {
        session->addSearchPath(searchP);
    }
    return sessionid;
}

void Compiler::buildProject(BuildSessionId a_Id, StringView a_Name)
{
    m_BuildSessions[a_Id]->addProject(a_Name);
}

void Compiler::buildProjectAt(BuildSessionId a_Id, StringView a_Path)
{
    m_BuildSessions[a_Id]->addProjectPath(a_Path);
}

CompiledSource* Compiler::buildSource(BuildSessionId a_Id, StringView a_Path, ArrayView<String> _projectsSearchPaths,
                                      Options const& a_Options)
{
    String                path = Path(a_Path).absolute().canonical().genericString();
    ProjectBuildSession*& projSession = m_BuildSessions[a_Id]->projects[path];
    projSession = new_<ProjectBuildSession>();
    projSession->message = new_<Message>();
    projSession->message->setText(Path(a_Path).filename());
    m_BuildSessions[a_Id]->builtProjects.push_back(path);
    for (auto p : _projectsSearchPaths)
        m_BuildSessions[a_Id]->addSearchPath(p);
    auto sourceFile = _getOrCreateSourceFile(a_Path);

    Options mergedOptions = m_BuildSessions[a_Id]->options;
    for (auto& opt : a_Options)
        mergedOptions.insert(opt);
    _buildSourceFiles(*m_BuildSessions[a_Id], *projSession, sourceFile, a_Path, mergedOptions);
    return projSession->all.front();
}

void Compiler::_buildProject(BuildSession& a_BuildSession, ProjectBuildSession& a_ProjBuildSession, StringView _path,
                             Options const& a_Options)
{
    SmallVector<SourceFile*> sourceFiles;
    if (Path::Exists(_path) && Path::IsDirectory(_path))
    {
        _listSourceFiles(_path, sourceFiles);
    }
    _buildSourceFiles(a_BuildSession, a_ProjBuildSession, sourceFiles, _path, a_Options);
}

void Compiler::_buildSourceFiles(BuildSession& a_BuildSession, ProjectBuildSession& a_ProjBuildSession,
                                 ArrayView<SourceFile*> a_SourceFiles, StringView _path, Options const& a_Options)
{
    PHANTOM_ASSERT(a_ProjBuildSession.message);
    auto& sessionSources = a_ProjBuildSession.all;
    SmallMap<uint, SmallMap<Module*, CompiledSources, 8, ModuleDependencySorter>> sourcePasses;
    Path    projectPath(Path(_path).absolute().canonical());
    Module* pMod = _getOrCreateModule(projectPath.genericString());
    PHANTOM_ASSERT(pMod);

    if (pMod->isNative())
    {
        PHANTOM_LOG(Error, "module '%.*s' is already loaded as native plugin, build skipped",
                    PHANTOM_STRING_AS_PRINTF_ARG(pMod->getName()));
        return;
    }

    if (!m_BuildingModules.empty())
        if (!m_BuildingModules.back()->hasDependency(pMod))
            m_BuildingModules.back()->addDependency(pMod);

    m_BuildingModules.push_back(pMod);
    auto onScopeExit = phantom::makeScopeExit([&]() { m_BuildingModules.pop_back(); });

    // == Collect outdated sources ==

    for (auto pSF : a_SourceFiles)
    {
        CompiledSource* pCSource = _getOrCreateCompiledSource(pSF);
        if (pCSource->isOutdated())
        {
            sessionSources.push_back(pCSource);
        }
    }

    // == If no outdated sources => we are finished ==

    if (sessionSources.empty())
    {
        return;
    }

    // == If outdated sources => let's start a build ==

    PHANTOM_EMIT sourceBuildStarted(sessionSources);

    CompiledSources sourcesParsed;

    // being build
    for (CompiledSource* pCompSource : sessionSources)
    {
        SourceStream* pStream = pCompSource->getSourceStream();

        Path sourcePath = pStream->getPath();
        Path sourcePathRel = sourcePath.relative(projectPath);

        Path packagePathRel = sourcePathRel.parentPath();

        Package* pK = pMod->getOrCreatePackage(packagePathRel.empty() ? "default" : packagePathRel.customString("."));
        pCompSource->beginBuild(pK, pK->newSource(sourcePath.stem()), findSourceStreamLanguage(pStream),
                                *a_ProjBuildSession.message);
        pCompSource->getParser()->begin(a_BuildSession);
    }

#define PHANTOM_CODE_TRACE_ENABLED 1
#if PHANTOM_CODE_TRACE_ENABLED
#    define PHANTOM_CODE_TRACE(...) printf(__VA_ARGS__);
#    define PHANTOM_CODE_TRACE_LN(...)                                                                                 \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n")
#else
#    define PHANTOM_CODE_TRACE(...)
#    define PHANTOM_CODE_TRACE_LN(...)
#endif

    PHANTOM_CODE_TRACE_LN("PROJECT %.*s", PHANTOM_STRING_AS_PRINTF_ARG(Path(_path).stem()));
    PHANTOM_CODE_TRACE("grammar pass on %zu sources ... ", sessionSources.size());

    uint currentPass = 0;

    auto now = getNow();

#define PHANTOM_CODE_USE_PARALLEL_PARSING_ENABLED 1

    if (PHANTOM_CODE_USE_PARALLEL_PARSING_ENABLED && sessionSources.size() > 1)
    {
        SpinMutex mutex;

        SmallVector<std::thread> threads;
        threads.resize(sessionSources.size());

        // parallel lexing
        size_t tidx = 0;
        for (CompiledSource* pSource : sessionSources)
        {
            threads[tidx++] = std::thread([pSource, &mutex, &sourcesParsed, currentPass]() {
                if (pSource->getParser()->stepParse(currentPass) !=
                    0) /// lexing/parsing + collect imports must happen here
                {
                    PHANTOM_WARNING(pSource->getCurrentBuild().hasError(),
                                    "parser returned error but source has no error registered");
                    return;
                }
                auto lock = mutex.autoLock();
                sourcesParsed.push_back(pSource);
            });
        }
        for (auto& th : threads)
            th.join();
    }
    else
    {
        for (CompiledSource* pSource : sessionSources)
        {
            if (pSource->getParser()->stepParse(currentPass) != 0) /// lexing/parsing + collect imports must happen here
            {
                PHANTOM_WARNING(pSource->getCurrentBuild().hasError(),
                                "parser returned error but source has no error registered");
                continue;
            }
            sourcesParsed.push_back(pSource);
        }
    }

    // advance parsing pass
    currentPass++;

    PHANTOM_CODE_TRACE_LN("%zu ms", (getNow() - now));

    // synchronous (for now) semantic analysis
    for (CompiledSource* pSource : sourcesParsed)
    {
        for (uint i = 1; i < pSource->getParser()->getLanguage()->getParserPassCount(); ++i)
        {
            sourcePasses[pSource->getParser()->getLanguage()->getParserPassPriority(i)]
                        [pSource->getSource()->getModule()]
                        .push_back(pSource); /// other passes are for analysis
        }
    }

    struct DependencySorter
    {
        DependencySorter(uint _currentPass) : m_currentPass(_currentPass) {}
        int ComputeDependencyLevel(uint _pass, CompiledSource* a_pSource) const
        {
            int level = 0;
            for (auto pSource : a_pSource->getBuildDependencies()[_pass])
            {
                level = std::max(ComputeDependencyLevel(_pass, pSource) + 1, level);
            }
            return level;
        }
        bool operator()(CompiledSource* a_pS0, CompiledSource* a_pS1) const
        {
            auto pass = m_currentPass + 1;
            while (pass--)
            {
                int l0 = ComputeDependencyLevel(pass, a_pS0);
                int l1 = ComputeDependencyLevel(pass, a_pS1);
                if (l0 < l1)
                    return true;
                else if (l0 > l1)
                    return false;
            }
            return a_pS0 < a_pS1;
        }

        uint m_currentPass = 0;
    };

    PHANTOM_CODE_TRACE("semantic pass on %zu sources ... ", sourcesParsed.size());
    now = getNow();

    for (auto& priority_map : sourcePasses)
    {
        auto& map = priority_map.second;
        for (auto& module_sources : map)
        {
            auto& outdatedModuleSources = module_sources.second;
            std::sort(outdatedModuleSources.begin(), outdatedModuleSources.end(), DependencySorter(currentPass));
            for (auto it = outdatedModuleSources.begin(); it != outdatedModuleSources.end();)
            {
                CompiledSource* pSource = (*it);
                if (pSource->getParser()->stepParse(currentPass) != 0)
                {
                    it = outdatedModuleSources.erase(it);
                }
                else
                    ++it;
            }
        }
        currentPass++;
    }
    PHANTOM_CODE_TRACE_LN("%zu ms", (getNow() - now));

    size_t i = sessionSources.size();
    while (i--)
    {
        CompiledSource* pSource = sessionSources[i];
        pSource->getParser()->end();
    }

    PHANTOM_CODE_TRACE("asm codegen pass on %zu sources ... ", sourcesParsed.size());
    now = getNow();
    _finalizeBuild(projectPath.genericString(), pMod, a_Options, a_ProjBuildSession, a_SourceFiles);
    PHANTOM_CODE_TRACE_LN("%zu ms", (getNow() - now));
}

void Compiler::outdate(SourceFile* a_pSourceStream) const
{
    getCompiledSource(a_pSourceStream)->outdate();
}

CompiledSources Compiler::listProjectCompiledSourcesAt(StringView _path)
{
    CompiledSources          sources;
    SmallVector<SourceFile*> files;
    _listSourceFiles(_path, files);
    for (auto file : files)
    {
        if (auto pCS = _getOrCreateCompiledSource(file))
        {
            sources.push_back(pCS);
        }
    }
    return sources;
}

CompiledSources Compiler::listProjectCompiledSources(BuildSessionId _sessionId, StringView _project)
{
    return listProjectCompiledSourcesAt(findProjectPath(_sessionId, _project));
}

phantom::String Compiler::findProjectPath(BuildSessionId _sessionId, StringView _project) const
{
    return m_BuildSessions[_sessionId]->findProjectPath(_project);
}

CompiledSource* Compiler::getCompiledSource(SourceStream* a_pSourceStream) const
{
    auto found = m_CompiledSources.find(a_pSourceStream);
    return found == m_CompiledSources.end() ? nullptr : found->second;
}

CompiledSource* Compiler::getCompiledSource(Source* a_pSource) const
{
    return getCompiledSource(a_pSource->getSourceStream());
}

CodeGenerator* Compiler::getCodeGenerator(Module* a_pModule) const
{
    auto found = m_ModuleData.find(a_pModule);
    return found == m_ModuleData.end() ? nullptr : found->second.codeGenerator;
}

DebugInformation* Compiler::getDebugInformation(Module* a_pModule) const
{
    auto found = m_ModuleData.find(a_pModule);
    return found == m_ModuleData.end() ? nullptr : found->second.debugInfo;
}

Language* Compiler::getLanguage(StringView a_strLanguageName) const
{
    for (auto pLang : m_Languages)
    {
        if (pLang->getName() == a_strLanguageName)
            return pLang;
    }
    return nullptr;
}

Language* Compiler::getLanguageFromFileExtension(StringView a_Extension) const
{
    for (Language* pLanguage : m_Languages)
    {
        if (pLanguage->getSourceFileExtension() == a_Extension)
            return pLanguage;
    }
    return nullptr;
}

Language* Compiler::findSourceStreamLanguage(SourceStream* a_pStream) const
{
    if (auto pLang = getLanguageFromFileExtension(phantom::Path(a_pStream->getUrl()).extension()))
    {
        return pLang;
    }
    return m_pDefaultLanguage;
}

Language* Compiler::findSourceLanguage(Source* a_pSource) const
{
    if (SourceStream* pStream = a_pSource->getSourceStream())
        return findSourceStreamLanguage(pStream);
    return nullptr;
}

void Compiler::abortBuild()
{
    m_bBuildAborted = true;
}

void Compiler::_finalizeBuild(StringView a_Path, Module* a_pModule, Options const& a_Options,
                              ProjectBuildSession& a_BuildSession, ArrayView<SourceFile*> a_Files,
                              bool a_bRestoreSources)
{
    CompiledSources& buildSessionAll = a_BuildSession.all;
    CompiledSources& buildSessionSuccess = a_BuildSession.succeeded;
    CompiledSources& buildSessionFailures = a_BuildSession.failed;

    // first : check success
    for (CompiledSource* pSource : buildSessionAll)
    {
        // update error from validity
        if (!(pSource->hasSucceeded()))
        {
            // if source failed, remove it from build session
            buildSessionFailures.push_back(pSource);
            pSource->currentBuild().getStatusMessage()->setType(MessageType::Error);
        }
        else
        {
            // User validation
            pSource->currentBuild().getStatusMessage()->setType(MessageType::Success);
            bool         bSucceeded = true;
            PHANTOM_EMIT buildSessionUserHook(pSource, bSucceeded);
            if (!bSucceeded)
            {
                buildSessionFailures.push_back(pSource);
                pSource->error(
                CodeRangeLocation{pSource->getSource(), CodeRange{CodePosition(1, 1), CodePosition(1, 1)}},
                "build aborted");
            }
            else
            {
                buildSessionSuccess.push_back(pSource);
                if (!a_bRestoreSources)
                    pSource->stampTime();
            }
        }
    }

    // end build for sources which failed
    for (auto pFail : buildSessionFailures)
        pFail->endBuild();

    // at this point, we know what succeeded and failed semanticly speaking

    // Until we have a per-source codegen, we need to look if we have a compilation success for every source file in
    // the project :

    for (auto pSF : a_Files)
    {
        if (getCompiledSource(pSF)->getCurrentBuild().isNull())
        {
            for (auto pSucc : buildSessionSuccess)
                pSucc->endBuild();
            // we don't have a valid semantic AST construction for this source, we cannot codegen the module
            return;
        }
    }

    CompiledSources projectSources = listProjectCompiledSourcesAt(a_Path);

    /// COMPILATION (if no compiler class available, load 'Phantom.JIT' module to have the one we
    /// provide)
    CodeGenerator* pCodeGen = getCodeGenerator(a_pModule);
    if (m_pCodeGeneratorClass)
    {
        // if (pCodeGen->m_bOutdated)
        {
            pCodeGen->setOptions(a_Options);

            int retryCount = 4;

        retry_code_gen:
            pCodeGen->begin();

            uint passCount = pCodeGen->getPassCount();
            for (uint pass = 0; pass < passCount; ++pass)
            {
                CodeGenerator::Error err;
                if ((err = pCodeGen->compile(pass)) != CodeGenerator::Error::OK)
                {
                    if (err == CodeGenerator::Error::MemoryNotReady)
                    {
                        if (--retryCount)
                        {
                            pCodeGen->abort();
                            goto retry_code_gen;
                        }
                    }
                    // Code generation failed
                    for (CompiledSource* pSource : projectSources)
                    {
                        pSource->error(CodeRangeLocation(),
                                       err == CodeGenerator::Error::MemoryNotReady
                                       ? "code generation error : memory not available"
                                       : "unresolved symbol, see log");
                        pSource->abortBuild(); //  but remember last valid build if any and defer result to
                                               //  the next compilation which will succeed0
                    }
                    pCodeGen->abort();
                    buildSessionFailures.insert(buildSessionFailures.end(), buildSessionAll.begin(),
                                                buildSessionAll.end());
                    buildSessionAll.clear();
                    buildSessionSuccess.clear();
                    break;
                }
            }

            if (!buildSessionAll.empty())
                pCodeGen->end();
            pCodeGen->m_bOutdated = false;
        }
    }
    else
    {
        static bool logDisplayed = false;
        if (!logDisplayed)
        {
            logDisplayed = true;
            PHANTOM_LOG(Error,
                        "No CodeGenerator derived class found to instantiate compilers ; you can "
                        "use the 'Phantom.JIT' plugin we've implemented based on LLVM");
        }
    }

    // nothing succeeded during this session, just return
    if (buildSessionSuccess.empty())
        return;

    // reattach previous build for pre-build notification purpose (prepare destroying data requires classes to be in
    // their original state)
    for (auto pSource : buildSessionSuccess)
        pSource->cancelCurrentSources();

    PHANTOM_EMIT buildSessionDone(a_BuildSession);

    // undo previous build and reinstall new build for post-build notifications
    for (auto pSource : buildSessionSuccess)
        pSource->restoreCurrentSources();

    for (auto pSource : buildSessionSuccess)
    {
        PHANTOM_VERIFY(pSource->endBuild());
        PHANTOM_EMIT sourceBuilt(pSource);
    }

    PHANTOM_EMIT buildSessionApplied(a_BuildSession);
}

void Compiler::_onModuleAdded(Module* a_pModule)
{
    if (m_pCodeGeneratorClass == nullptr)
    {
        Classes classes;
        a_pModule->findClasses(classes, PHANTOM_CLASSOF(CodeGenerator), true);
        for (Class* pClass : classes)
        {
            m_pCodeGeneratorClass = pClass;
            for (auto pModule : Application::Get()->getModules())
            {
                if (pModule->isInstalled())
                {
                    PHANTOM_ASSERT(getCodeGenerator(pModule) == nullptr);
                    _createCodeGenerator(pModule);
                }
            }
        }
    }
    else
    {
        _createCodeGenerator(a_pModule);
    }
}

void Compiler::_onModuleDiscarded(Module*) {}

// TODO : rename "nativeModuleAboutToBeRemoved"

void Compiler::_onModuleAboutToBeRemoved(Module* a_pModule)
{
    if (m_pCodeGeneratorClass &&
        m_pCodeGeneratorClass->getModule() == a_pModule) // CodeGenerator class is going to be removed
    {
        for (auto pModule : Application::Get()->getModules())
        {
            PHANTOM_ASSERT(getCodeGenerator(pModule) &&
                           getCodeGenerator(pModule)->rtti.metaClass == m_pCodeGeneratorClass);
            _destroyCodeGenerator(pModule);
        }
        m_pCodeGeneratorClass = nullptr;
    }
    if (m_pCodeGeneratorClass)
    {
        _destroyCodeGenerator(a_pModule);
    }
}

bool Compiler::isBuildOutdated() const
{
    Sources sources;
    Application::Get()->getSources(sources);
    for (Source* pSrc : sources)
    {
        if (getCompiledSource(pSrc)->isOutdated())
            return true;
    }
    return false;
}

CodeGenerator* Compiler::_createCodeGenerator()
{
    if (m_pCodeGeneratorClass)
    {
        void*          instance = m_pCodeGeneratorClass->newInstance();
        CodeGenerator* pCodeGen = m_pCodeGeneratorClass->cast<CodeGenerator>(instance);
        pCodeGen->rtti.instance = instance;
        pCodeGen->rtti.metaClass = m_pCodeGeneratorClass;
        return pCodeGen;
    }
    return nullptr;
}

void Compiler::_destroyCodeGenerator(CodeGenerator* a_pCodeGenerator)
{
    PHANTOM_ASSERT(m_pCodeGeneratorClass);
    m_pCodeGeneratorClass->deleteInstance(
    PHANTOM_CLASSOF(CodeGenerator)->cast(m_pCodeGeneratorClass, a_pCodeGenerator));
}

void Compiler::_outdateModule(Source* a_pSource)
{
    if (CodeGenerator* pCG = Compiler::Get()->getCodeGenerator(a_pSource->getModule()))
        pCG->outdate();
}

void Compiler::_setCodeGenerator(Module* a_pModule, CodeGenerator* a_pCodeGenerator)
{
    ModuleData& moduleData = m_ModuleData[a_pModule];
    if (moduleData.codeGenerator)
    {
        if (moduleData.debugInfo)
        {
            moduleData.codeGenerator->destroyDebugInformation(moduleData.debugInfo);
            moduleData.debugInfo = nullptr;
        }
        moduleData.codeGenerator->m_pModule = nullptr;
    }
    moduleData.codeGenerator = a_pCodeGenerator;
    if (moduleData.codeGenerator)
    {
        moduleData.codeGenerator->m_pModule = a_pModule;
        moduleData.debugInfo = moduleData.codeGenerator->createDebugInformation();
        moduleData.debugInfo->m_pModule = a_pModule;
    }
}

void Compiler::_createCodeGenerator(Module* a_pModule)
{
    PHANTOM_ASSERT(getCodeGenerator(a_pModule) == nullptr);
    _setCodeGenerator(a_pModule, _createCodeGenerator());
    m_ModuleData[a_pModule].codeGenerator = _createCodeGenerator();
    m_ModuleData[a_pModule].codeGenerator->m_pModule = a_pModule;
}

void Compiler::_destroyCodeGenerator(Module* a_pModule)
{
    CodeGenerator*& pCodeGenerator = m_ModuleData[a_pModule].codeGenerator;
    _setCodeGenerator(a_pModule, nullptr);
    PHANTOM_ASSERT(pCodeGenerator);
    pCodeGenerator->m_pModule = nullptr;
    pCodeGenerator = nullptr;
    _destroyCodeGenerator(pCodeGenerator);
}

void Compiler::addLanguage(Language* a_pLanguage)
{
    m_Languages.push_back(a_pLanguage);
    PHANTOM_EMIT languageAdded(a_pLanguage);
}

void Compiler::removeLanguage(Language* a_pLanguage)
{
    PHANTOM_EMIT languageAboutToBeRemoved(a_pLanguage);
    for (auto it = m_CompiledSources.begin(); it != m_CompiledSources.end();)
    {
        if (it->second->getLanguage() == a_pLanguage)
            it = m_CompiledSources.erase(it);
        else
            ++it;
    }
    m_Languages.erase(std::find(m_Languages.begin(), m_Languages.end(), a_pLanguage));
}

void Compiler::restoreBuildSession(BuildSessionId a_Id)
{
    if (m_CurrentSessionId == a_Id)
        return;
    PHANTOM_ASSERT(a_Id < int(m_BuildSessions.size()));
    PHANTOM_ASSERT(a_Id >= -1);
    while (m_CurrentSessionId > a_Id)
        _undoSession(*m_BuildSessions[m_CurrentSessionId--]);

    while (m_CurrentSessionId < a_Id)
        _redoSession(*m_BuildSessions[++m_CurrentSessionId]);
}

void Compiler::codeGen(Module* a_pModule, Options a_Options /*= {}*/, Message* a_pMsg /*= nullptr*/)
{
    /// COMPILATION (if no compiler class available, load 'Phantom.JIT' module to have the one we
    /// provide)
    CodeGenerator* pCodeGen = getCodeGenerator(a_pModule);
    PHANTOM_ASSERT(pCodeGen,
                   "module has not been registered ('Application::addModule' method) and thus has no code generator");
    if (m_pCodeGeneratorClass)
    {
        // if (pCodeGen->m_bOutdated)
        {
            pCodeGen->setOptions(a_Options);

            int retryCount = 4;

        retry_code_gen:
            Sources srcs;
            a_pModule->getSources(srcs);
            pCodeGen->begin();

            bool aborted = false;
            uint passCount = pCodeGen->getPassCount();
            for (uint pass = 0; pass < passCount; ++pass)
            {
                CodeGenerator::Error err;
                if ((err = pCodeGen->compile(pass)) != CodeGenerator::Error::OK)
                {
                    if (err == CodeGenerator::Error::MemoryNotReady)
                    {
                        if (--retryCount)
                        {
                            pCodeGen->abort();
                            goto retry_code_gen;
                        }
                    }
                    // Code generation failed
                    if (a_pMsg)
                        a_pMsg->errorD(CodeRangeLocation(), "code generation error : memory not available");
                    pCodeGen->abort();
                    aborted = true;
                    break;
                }
            }

            if (!aborted)
                pCodeGen->end();
            pCodeGen->m_bOutdated = false;
        }
    }
    else
    {
        static bool logDisplayed = false;
        if (!logDisplayed)
        {
            logDisplayed = true;
            PHANTOM_LOG(Error,
                        "No CodeGenerator derived class found to instantiate compilers ; you can "
                        "use the 'Phantom.JIT' plugin we've implemented based on LLVM");
        }
    }
}

struct SourceInfos
{
    String name;
    String url;
    String languageClass;
};
typedef SmallMap<String, SmallVector<SourceInfos>> PackageSourceMap;

} // namespace lang

} // namespace phantom
