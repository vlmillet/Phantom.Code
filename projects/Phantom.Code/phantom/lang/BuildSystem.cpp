// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "BuildSystem.h"

#include "BuildSource.h"
#include "CodeGenerator.h"
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

#pragma optimize("", off)

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
extern StaticGlobal<BuildSystem> g_PHNTM_LNG_Compiler;

BuildSystem* BuildSystem::Get()
{
    PHANTOM_ASSERT(!!g_PHNTM_LNG_Compiler,
                   "Compiler instance not present, you probably forgot to create a phantom::lang::Main instance in "
                   "your 'main' application entry point");
    return &*g_PHNTM_LNG_Compiler;
}

BuildSystem::BuildSystem()
{
    PHANTOM_CONNECT(Application::Get(), moduleAdded, this, _onModuleAdded);
}

BuildSystem::~BuildSystem()
{
    PHANTOM_DISCONNECT(Application::Get(), moduleAdded, this, _onModuleAdded);
}

phantom::lang::BuildSource* BuildSystem::_getOrCreateCompiledSource(SourceStream* a_pSourceStream)
{
    auto& pCS = m_BuildSources[a_pSourceStream];
    if (pCS)
        return pCS;
    return pCS = new_<BuildSource>(this, a_pSourceStream);
}

void BuildSystem::_deleteCompiledSource(SourceStream* a_pSourceStream)
{
    auto found = m_BuildSources.find(a_pSourceStream);
    if (found != m_BuildSources.end())
    {
        delete_<BuildSource>(found->second);
        m_BuildSources.erase(found);
    }
}

void BuildSystem::_undoSession(BuildSession& _session)
{
    for (auto& proj_session : _session.projects)
    {
        for (auto succeeded : proj_session.second->succeeded)
        {
            succeeded->m_PreviousBuildIndex = succeeded->m_CurrentBuildIndex--; // rewind build after
        }
        PHANTOM_EMIT buildSessionStarting(*proj_session.second);
        for (auto succeeded : proj_session.second->succeeded)
        {
            succeeded->restoreCurrentSources(); // swap sources inside package
        }
        PHANTOM_EMIT buildSessionSucceeded(*proj_session.second);
        PHANTOM_EMIT buildSessionDone(*proj_session.second);
    }
}

void BuildSystem::_redoSession(BuildSession& _session)
{
    for (auto& proj_session : _session.projects)
    {
        for (auto succeeded : proj_session.second->succeeded)
        {
            succeeded->m_PreviousBuildIndex = succeeded->m_CurrentBuildIndex++; // advance build first
        }
        PHANTOM_EMIT buildSessionStarting(*proj_session.second);
        for (auto succeeded : proj_session.second->succeeded)
        {
            succeeded->restoreCurrentSources(); // swap sources inside package
        }
        PHANTOM_EMIT buildSessionSucceeded(*proj_session.second);
        PHANTOM_EMIT buildSessionDone(*proj_session.second);
    }
}

SourceFile* BuildSystem::_getOrCreateSourceFile(StringView _path)
{
    auto found = m_UrlToSourceStream.find("file:///" + _path);
    if (found == m_UrlToSourceStream.end())
    {
        return static_cast<SourceFile*>(m_UrlToSourceStream["file:///" + _path] = new_<SourceFile>(_path));
    }
    PHANTOM_ASSERT(found->second->asFile());
    return static_cast<SourceFile*>(found->second);
}

Module* BuildSystem::_getOrCreateModule(StringView _path)
{
    Path   p(_path);
    String moduleName = p.filename();
    auto   found = m_CompiledModules.find(p.absolute().genericString());
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

void BuildSystem::_listSourceFiles(StringView _path, SmallVector<SourceFile*>& _sourceFiles)
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

void BuildSystem::_loadSources(BuildSessionId a_Id, StringView a_SourceUniqueName)
{
    auto& session = m_BuildSessions[a_Id];
    auto  lock = session->mutex.autoLock();
    auto  found = session->projectsPathFromSourceUniqueName.find(a_SourceUniqueName);
    if (found != session->projectsPathFromSourceUniqueName.end())
    {
        for (auto& projPath : found->second)
            buildProjectAt(a_Id, projPath);
    }
}

void BuildSystem::_loadPackages(BuildSessionId a_Id, StringView a_PackageUniqueName)
{
    auto& session = m_BuildSessions[a_Id];
    auto  lock = session->mutex.autoLock();
    auto  found = session->projectsPathFromPackageName.find(a_PackageUniqueName);
    if (found != session->projectsPathFromPackageName.end())
    {
        for (auto& projPath : found->second)
            buildProjectAt(a_Id, projPath);
    }
}

void BuildSystem::cleanupProject(StringView _path)
{
    SmallVector<SourceFile*> sfs;
    _listSourceFiles(_path, sfs);
    for (auto pSourceF : sfs)
    {
        _deleteCompiledSource(pSourceF);
    }
}

void BuildSystem::cleanBuild(StringView _path)
{
    SmallVector<SourceFile*> sfs;
    _listSourceFiles(_path, sfs);
    for (auto pSourceF : sfs)
        getCompiledSource(pSourceF)->outdate();
}

BuildSessionId BuildSystem::newSession(ArrayView<String> _projectsSearchPaths, Options const& a_Options)
{
    m_BuildSessions.resize(m_CurrentSessionId +
                           1); // ensure we erase previously undone build sessions (like an undo stack)
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

bool BuildSystem::buildProject(BuildSessionId a_Id, StringView a_Name)
{
    auto path = m_BuildSessions[a_Id]->findProjectPath(a_Name);
    return !path.empty() && buildProjectAt(a_Id, path);
}

bool BuildSystem::buildProject(BuildSessionId a_Id, Module* a_pModule)
{
    return buildProject(a_Id, a_pModule->getName());
}

bool BuildSystem::buildProjectAndDependents(BuildSessionId a_Id, StringView a_Name)
{
    auto path = m_BuildSessions[a_Id]->findProjectPath(a_Name);
    return !path.empty() && buildProjectAndDependentsAt(a_Id, path);
}

bool BuildSystem::buildProjectAndDependents(BuildSessionId a_Id, Module* a_pModule)
{
    return buildProjectAndDependents(a_Id, a_pModule->getName());
}

bool BuildSystem::buildProjectAt(BuildSessionId a_Id, StringView a_Path)
{
    auto& session = *m_BuildSessions[a_Id];
    auto  lock = session.mutex.autoLock();
    Path  p(a_Path);
    if (!p.exists())
        return false;
    String path = p.absolute().canonical().genericString();
    auto&  projSess = session.projects[path];
    if (projSess == nullptr)
    {
        projSess = new_<ProjectBuildSession>();
        if (a_Id > 0) // not the first build session ever
        {
            auto prevProjSession = m_BuildSessions[a_Id - 1]->getProjectBuildSession(path);
            if (prevProjSession) // if previous build session has a matching session for the current project
            {
                if (prevProjSession && !prevProjSession->failed.empty()) // and that project session has failed source
                {
                    // we transfer the succeeded sources to the current project session to ensure we keep track of them
                    // until a build fully succeeds
                    projSess->succeeded = prevProjSession->succeeded;
                }
            }
        }
        projSess->message = new_<Message>();
        projSess->message->setText(Path(a_Path).filename());
        _buildProject(a_Id, *projSess, path, session.options);
        session.builtProjects.push_back(path);
    }
    return projSess->failed.empty();
}

void BuildSystem_CollectLeafModules(Module* _module, SmallVector<Module*>& _dependents)
{
    if (_module->getDependents().empty())
        _dependents.push_back(_module);
    else
        for (auto dep : _module->getDependents())
        {
            BuildSystem_CollectLeafModules(dep, _dependents);
        }
}

bool BuildSystem::buildProjectAndDependentsAt(BuildSessionId a_Id, StringView a_Path)
{
    Module* pMod = Application::Get()->getModule(Path(a_Path).filename());
    if (pMod == nullptr) // not built yet (so dependencies either)
        return buildProjectAt(a_Id, a_Path);

    SmallVector<Module*> dependents;
    BuildSystem_CollectLeafModules(pMod, dependents);

    if (dependents.size() == 1 && dependents.front() == pMod)
        return buildProjectAt(a_Id, a_Path);

    bool success = true;
    for (auto pDep : dependents)
    {
        success = buildProjectAndDependents(a_Id, pDep) || success;
    }
    return success;
}

bool BuildSystem::hasBuildSucceeded(BuildSessionId a_Id) const
{
    return m_BuildSessions[a_Id]->isSuccessful();
}

void BuildSystem::loadSourcesOrPackages(BuildSessionId a_Id, StringView a_UniqueName)
{
    _loadSources(a_Id, a_UniqueName);
    _loadPackages(a_Id, a_UniqueName);
}

BuildSource* BuildSystem::buildSource(BuildSessionId a_Id, StringView a_Path, ArrayView<String> _projectsSearchPaths,
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
    _buildSourceFiles(a_Id, *projSession, sourceFile, a_Path, mergedOptions);
    return projSession->all.front();
}

void BuildSystem::_buildProject(BuildSessionId a_BuildSessionId, ProjectBuildSession& a_ProjBuildSession,
                                StringView _path, Options const& a_Options)
{
    SmallVector<SourceFile*> sourceFiles;
    if (Path::Exists(_path) && Path::IsDirectory(_path))
    {
        _listSourceFiles(_path, sourceFiles);
    }
    _buildSourceFiles(a_BuildSessionId, a_ProjBuildSession, sourceFiles, _path, a_Options);
}

#define PHANTOM_CODE_COMPILER_TRACE_ENABLED 1
#if PHANTOM_CODE_COMPILER_TRACE_ENABLED
#    define PHANTOM_CODE_COMPILER_TRACE(...) printf(__VA_ARGS__);
#    define PHANTOM_CODE_COMPILER_TRACE_LN(...)                                                                        \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n")
#else
#    define PHANTOM_CODE_COMPILER_TRACE(...)
#    define PHANTOM_CODE_COMPILER_TRACE_LN(...)
#endif

void BuildSystem::_buildSourceFiles(BuildSessionId a_BuildSessionId, ProjectBuildSession& a_ProjBuildSession,
                                    ArrayView<SourceFile*> a_SourceFiles, StringView _path, Options const& a_Options)
{
    PHANTOM_ASSERT(a_ProjBuildSession.message);
    auto& sessionSources = a_ProjBuildSession.all;
    // auto& sessionSourcesAlreadySucceeded = a_ProjBuildSession.succeeded;
    SmallMap<uint, BuildSources> sourcePasses;
    Path                         projectPath(Path(_path).absolute().canonical());
    Module*                      pMod = _getOrCreateModule(projectPath.genericString());
    PHANTOM_ASSERT(pMod);

    if (pMod->isNative())
    {
        PHANTOM_LOG(Error, "module '%.*s' is already loaded as native plugin, build skipped",
                    PHANTOM_STRING_AS_PRINTF_ARG(pMod->getName()));
        return;
    }

    for (auto pDep : pMod->getDependencies())
    {
        if (pDep->isNative())
            continue;

        // TODO : solve projects search paths here so that we can directly call !buildProject
        auto path = findProjectPath(a_BuildSessionId, pDep->getName());
        if (!path.empty() && !buildProject(a_BuildSessionId, pDep))
        {
            return;
        }
    }

    if (!m_BuildingModules.empty())
        if (!m_BuildingModules.back()->hasDependency(pMod))
            m_BuildingModules.back()->addDependency(pMod);

    m_BuildingModules.push_back(pMod);

    auto onScopeExit = phantom::makeScopeExit([&]() { m_BuildingModules.pop_back(); });

    // == Collect outdated sources ==

    for (auto pSF : a_SourceFiles)
    {
        BuildSource* pBuildSource = _getOrCreateCompiledSource(pSF);
        if (pBuildSource->isOutdated())
        {
            //             if (pCSource->getCurrentBuild().isEmpty())
            //                 hasBuildEverSucceeded = false;
            PHANTOM_CODE_COMPILER_TRACE_LN("source file to compile : %.*s",
                                           PHANTOM_STRING_AS_PRINTF_ARG(pBuildSource->getSourceStream()->getPath()));
            sessionSources.push_back(pBuildSource);
        }
    }

    // == If no outdated sources => we are finished ==

    if (sessionSources.empty())
    {
        return;
    }

    // == If outdated sources => let's start a build ==

    PHANTOM_EMIT sourceBuildStarted(sessionSources);

    SmallVector<BuildSource::Session const*> sourceParsedSessions;

    SmallVector<BuildSource::Session> sourceSessions;

    // begin the parser
    for (auto* pCompSource : sessionSources)
    {
        SourceStream* pStream = pCompSource->getSourceStream();

        Path sourcePath = pStream->getPath();
        Path sourcePathRel = sourcePath.relative(projectPath);

        Path packagePathRel = sourcePathRel.parentPath();

        Package* pK = pMod->getOrCreatePackage(packagePathRel.empty() ? "default" : packagePathRel.customString("."));

        sourceSessions.push_back(pCompSource->createSession(
        pK, pK->newSource(sourcePath.stem()), findSourceStreamLanguage(pStream), *a_ProjBuildSession.message));

        sourceSessions.back().getParser()->begin(a_BuildSessionId);
    }

    PHANTOM_CODE_COMPILER_TRACE_LN("PROJECT %.*s", PHANTOM_STRING_AS_PRINTF_ARG(Path(_path).stem()));
    PHANTOM_CODE_COMPILER_TRACE("grammar pass on %zu sources ... ", sessionSources.size());

    auto now = getNow();

    // ===============================================================================
    // LEXING

#define PHANTOM_CODE_USE_PARALLEL_PARSING_ENABLED 1

    if (PHANTOM_CODE_USE_PARALLEL_PARSING_ENABLED && sessionSources.size() > 1)
    {
        SpinMutex mutex;

        SmallVector<std::thread> threads;
        threads.resize(sessionSources.size());

        // parallel lexing
        size_t tidx = 0;
        for (auto const& sourceSession : sourceSessions)
        {
            threads[tidx++] = std::thread([&sourceSession, &mutex, &sourceParsedSessions]() {
                /// lexing/parsing must happen here
                if (sourceSession.getParser()->lex() != 0)
                {
                    return;
                }
                auto lock = mutex.autoLock();
                sourceParsedSessions.push_back(&sourceSession);
            });
        }
        for (auto& th : threads)
            th.join();
    }
    else
    {
        for (auto const& sourceSession : sourceSessions)
        {
            if (sourceSession.getParser()->lex() != 0) /// lexing/parsing must happen here
            {
                continue;
            }
            sourceParsedSessions.push_back(&sourceSession);
        }
    }

    // ===============================================================================
    // MODULE FETCHING

    for (auto* pSourceSession : sourceParsedSessions)
    {
        Strings moduleNames;
        if (pSourceSession->getParser()->fetchModules(moduleNames) != 0) /// fetching modules
        {
            continue;
        }

        for (auto moduleName : moduleNames)
        {
            bool    depFailedToLoadOrBuild = false;
            Module* pDepMod = Application::Get()->getModule(moduleName);
            if (!pDepMod)
            {
                if (pSourceSession->getParser()->getOption("prefer-module-kind") == "native")
                {
                    if (Plugin* pPlugin = Application::Get()->getPlugin(moduleName))
                    {
                        if (pPlugin->load())
                        {
                            pDepMod = pPlugin->getModule();
                            PHANTOM_ASSERT(pDepMod);
                        }
                    }
                    if (!pDepMod)
                    {
                        if (!buildProject(a_BuildSessionId, moduleName))
                        {
                            a_ProjBuildSession.message->error(
                            "module '%.*s' : not found, either because project no found or project build failed",
                            PHANTOM_STRING_AS_PRINTF_ARG(moduleName));
                        }
                    }
                }
                else
                {
                    buildProject(a_BuildSessionId, moduleName);
                    if (!pDepMod || !pDepMod->isNative())
                    {
                        if (!hasBuildSucceeded(a_BuildSessionId))
                        {
                            a_ProjBuildSession.message->error("module : failed to build project '%.*s'",
                                                              PHANTOM_STRING_AS_PRINTF_ARG(moduleName));
                        }
                    }
                    pDepMod = Application::Get()->getModule(moduleName);
                    if (!pDepMod)
                    {
                        if (Plugin* pPlugin = Application::Get()->getPlugin(moduleName))
                        {
                            if (pPlugin->load())
                            {
                                pDepMod = pPlugin->getModule();
                                PHANTOM_ASSERT(pDepMod);
                            }
                        }
                        if (!pDepMod)
                        {
                            a_ProjBuildSession.message->error(
                            "module '%.*s' : not found, either because project no found or project build failed",
                            PHANTOM_STRING_AS_PRINTF_ARG(moduleName));
                            depFailedToLoadOrBuild = true;
                        }
                    }
                }
            }
            else
            {
                if (!pDepMod->isNative())
                {
                    BuildSystem::Get()->buildProject(a_BuildSessionId, moduleName);
                    if (!hasBuildSucceeded(a_BuildSessionId))
                    {
                        a_ProjBuildSession.message->error("module : failed to build project '%.*s'",
                                                          PHANTOM_STRING_AS_PRINTF_ARG(moduleName));
                    }
                }
            }
            if (!depFailedToLoadOrBuild)
                if (!pMod->hasDependency(pDepMod))
                    pMod->addDependency(pDepMod);
        }
    }

    // ===============================================================================
    // IMPORT FETCHING

    for (auto* pSourceSession : sourceParsedSessions)
    {
        Strings imports;
        if (pSourceSession->getParser()->fetchImports(imports) != 0) /// fetching modules
        {
            continue;
        }

        // for each import, deduce module dependency
        for (auto import : imports)
        {
            loadSourcesOrPackages(a_BuildSessionId, import);
        }
    }
    // ========================================================================
    // SEMANTIC

    // We only notify of build start from here (so that builds of different modules won't interlace, as module fetching
    // happened above)

    PHANTOM_EMIT buildSessionStarting(a_ProjBuildSession);

    // being build
    for (auto& sourceSession : sourceSessions)
    {
        sourceSession.getParser()->beginParse(a_BuildSessionId);
    }

    uint currentPass = 0;

    for (auto& sourceSession : sourceSessions)
    {
        sourceSession.getBuildSource()->beginBuild(sourceSession);
    }

    PHANTOM_CODE_COMPILER_TRACE_LN("%zu ms", (getNow() - now));

    // synchronous (for now) semantic analysis
    for (auto* pSourceSession : sourceParsedSessions)
    {
        for (uint i = 0; i < pSourceSession->getParser()->getLanguage()->getParserPassCount(); ++i)
        {
            sourcePasses[pSourceSession->getParser()->getLanguage()->getParserPassPriority(i)].push_back(
            pSourceSession->getBuildSource()); /// other passes are for analysis
        }
    }

    struct DependencySorter
    {
        DependencySorter(uint _currentPass, BuildSources& _sources) : m_currentPass(_currentPass), m_sources(_sources)
        {
        }

        int ComputeImportSortScore(Source* a_pSource) const
        {
            int score = 0;
            for (auto* cs : m_sources)
            {
                if (cs->getSource() == a_pSource)
                    continue;
                score += cs->getSource()->hasImport(a_pSource);
            }
            return score;
        }

        bool HasImported(Source* a_pSource, Symbol* a_pSymbol, SmallSet<const Source*>& treated) const
        {
            if (treated.insert(a_pSource).second)
            {
                for (Source::Import const& import : a_pSource->getImports())
                {
                    if (import.symbol == a_pSymbol)
                        return true;
                    if (Source* importSource = import.symbol->asSource())
                        if (HasImported(importSource, a_pSymbol, treated))
                            return true;
                }
            }
            return false;
        }

        bool HasImported(Source* a_pSource, Symbol* a_pSymbol) const
        {
            SmallSet<const Source*> treated;
            return HasImported(a_pSource, a_pSymbol, treated);
        }

        int ComputeDependencyLevel(uint _pass, BuildSource* a_pSource) const
        {
            int level = 0;
            for (auto pSource : a_pSource->getBuildDependencies()[_pass])
            {
                level = std::max(ComputeDependencyLevel(_pass, pSource) + 1, level);
            }
            return level;
        }
        bool operator()(BuildSource* a_pS0, BuildSource* a_pS1) const
        {
#pragma message(PHANTOM_TODO "optimize HasImported by caching all hierarchy inside Source instead of using recursion")
            bool s0Importss1 = HasImported(a_pS0->getSource(), a_pS1->getSource());
            bool s1Importss0 = HasImported(a_pS1->getSource(), a_pS0->getSource());

            if (s0Importss1 != s1Importss0)
            {
                if (s0Importss1)
                    return false;
                if (s1Importss0)
                    return true;
            }

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

            int importScore0 = ComputeImportSortScore(a_pS0->getSource());
            int importScore1 = ComputeImportSortScore(a_pS1->getSource());
            if (importScore0 == importScore1)
                return a_pS0 < a_pS1;
            return importScore0 > importScore1;
        }

        uint          m_currentPass = 0;
        BuildSources& m_sources;
    };

    PHANTOM_CODE_COMPILER_TRACE("semantic pass on %zu sources ... ", sourceParsedSessions.size());
    now = getNow();

    for (auto& priority_sources : sourcePasses)
    {
        auto& outdatedModuleSources = priority_sources.second;

        auto copy_outdatedModuleSources = outdatedModuleSources;
        std::sort(outdatedModuleSources.begin(), outdatedModuleSources.end(),
                  DependencySorter(currentPass, copy_outdatedModuleSources));

        //             printf("PASS : %d\n", priority_map.first);
        //             for (auto src : outdatedModuleSources)
        //             {
        //                 printf("%.*s\n", PHANTOM_STRING_AS_PRINTF_ARG(src->getSource()->getName()));
        //                 printf("");
        //             }
        //             printf("---------------------------\n");

        for (auto it = outdatedModuleSources.begin(); it != outdatedModuleSources.end();)
        {
            BuildSource* pSource = (*it);
            if (pSource->getParser()->stepParse(currentPass) != 0)
            {
                it = outdatedModuleSources.erase(it);
            }
            else
                ++it;
        }
        currentPass++;
    }
    PHANTOM_CODE_COMPILER_TRACE_LN("%zu ms", (getNow() - now));

    size_t i = sessionSources.size();
    while (i--)
    {
        BuildSource* pSource = sessionSources[i];
        pSource->getParser()->end();
    }

    PHANTOM_CODE_COMPILER_TRACE("asm codegen pass on %zu sources ... ", sourceParsedSessions.size());
    now = getNow();
    _finalizeBuild(projectPath.genericString(), pMod, a_Options, a_ProjBuildSession, a_SourceFiles);
    PHANTOM_CODE_COMPILER_TRACE_LN("%zu ms", (getNow() - now));
}

void BuildSystem::outdate(SourceFile* a_pSourceStream) const
{
    getCompiledSource(a_pSourceStream)->outdate();
}

BuildSources BuildSystem::listProjectBuildSourcesAt(StringView _path)
{
    BuildSources             sources;
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

BuildSources BuildSystem::listProjectBuildSources(BuildSessionId _sessionId, StringView _project)
{
    return listProjectBuildSourcesAt(findProjectPath(_sessionId, _project));
}

phantom::String BuildSystem::findProjectPath(BuildSessionId _sessionId, StringView _project) const
{
    return m_BuildSessions[_sessionId]->findProjectPath(_project);
}

BuildSource* BuildSystem::getCompiledSource(SourceStream* a_pSourceStream) const
{
    auto found = m_BuildSources.find(a_pSourceStream);
    return found == m_BuildSources.end() ? nullptr : found->second;
}

BuildSource* BuildSystem::getCompiledSource(Source* a_pSource) const
{
    return getCompiledSource(a_pSource->getSourceStream());
}

CodeGenerator* BuildSystem::getCodeGenerator(Module* a_pModule) const
{
    auto found = m_ModuleData.find(a_pModule);
    return found == m_ModuleData.end() ? nullptr : found->second.codeGenerator;
}

DebugInformation* BuildSystem::getDebugInformation(Module* a_pModule) const
{
    auto found = m_ModuleData.find(a_pModule);
    return found == m_ModuleData.end() ? nullptr : found->second.debugInfo;
}

Language* BuildSystem::getLanguage(StringView a_strLanguageName) const
{
    for (auto pLang : m_Languages)
    {
        if (pLang->getName() == a_strLanguageName)
            return pLang;
    }
    return nullptr;
}

Language* BuildSystem::getLanguageFromFileExtension(StringView a_Extension) const
{
    for (Language* pLanguage : m_Languages)
    {
        if (pLanguage->getSourceFileExtension() == a_Extension)
            return pLanguage;
    }
    return nullptr;
}

Language* BuildSystem::findSourceStreamLanguage(SourceStream* a_pStream) const
{
    if (auto pLang = getLanguageFromFileExtension(phantom::Path(a_pStream->getUrl()).extension()))
    {
        return pLang;
    }
    return m_pDefaultLanguage;
}

Language* BuildSystem::findSourceLanguage(Source* a_pSource) const
{
    if (SourceStream* pStream = a_pSource->getSourceStream())
        return findSourceStreamLanguage(pStream);
    return nullptr;
}

void BuildSystem::abortBuild()
{
    m_bBuildAborted = true;
}

void BuildSystem::_finalizeBuild(StringView a_Path, Module* a_pModule, Options const& a_Options,
                                 ProjectBuildSession& a_BuildSession, ArrayView<SourceFile*> a_Files,
                                 bool a_bRestoreSources)
{
    BuildSources& buildSessionAll = a_BuildSession.all;
    BuildSources& buildSessionSuccess = a_BuildSession.succeeded;
    BuildSources& buildSessionFailures = a_BuildSession.failed;

    // first : check success
    for (BuildSource* pSource : buildSessionAll)
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

    // at this point, we know what succeeded and failed semanticly speaking

    // Until we have a per-source codegen, we need to look if we have a compilation success for every source file in
    // the project, if any failure happened, codegen will be impossible

    if (!buildSessionFailures.empty())
    {
        // end build for sources which failed
        for (auto pFail : buildSessionFailures)
            pFail->endBuild();

        for (auto pSucc : buildSessionSuccess)
            if (std::find(buildSessionAll.begin(), buildSessionAll.end(), pSucc) != buildSessionAll.end())
                pSucc->endBuild();
        // we don't have a valid semantic AST construction for this source, we cannot codegen the module
        PHANTOM_EMIT buildSessionFailed(a_BuildSession);
        PHANTOM_EMIT buildSessionDone(a_BuildSession);
        return;
    }

    // on the first success build for this module, we install it (right now, its just an information, not really
    // triggers anything, but that allows users of modules to know if they are allowed to use them)
    if (!a_pModule->isInstalled())
        a_pModule->setInstalled();

    BuildSources projectSources = listProjectBuildSourcesAt(a_Path);

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
                    for (BuildSource* pSource : projectSources)
                    {
                        pSource->error(CodeRangeLocation(),
                                       err == CodeGenerator::Error::MemoryNotReady
                                       ? "code generation error : memory not available"
                                       : "unresolved symbol, see log");
                        pSource->abortBuild(); //  but remember last valid build if any and defer result to
                                               //  the next compilation which will succeed0
                    }
                    pCodeGen->abort();
                    for (auto pSource : buildSessionAll)
                        PHANTOM_VERIFY(pSource->endBuild());
                    buildSessionFailures.insert(buildSessionFailures.end(), projectSources.begin(),
                                                projectSources.end());
                    buildSessionAll.clear();
                    buildSessionSuccess.clear();
                    PHANTOM_EMIT buildSessionFailed(a_BuildSession);
                    PHANTOM_EMIT buildSessionDone(a_BuildSession);
                    return;
                }
            }
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

    // reattach previous build for pre-build notification purpose (prepare destroying data requires classes to be in
    // their original state)
    for (auto pSource : buildSessionSuccess)
        pSource->cancelCurrentSources();

    PHANTOM_EMIT buildSessionSucceeding(a_BuildSession);

    // undo previous build and reinstall new build for post-build notifications
    for (auto pSource : buildSessionSuccess)
        pSource->restoreCurrentSources();

    for (auto pSource : buildSessionAll)
    {
        // means the source was built in the current session and not succeeded in a previous one
        PHANTOM_VERIFY(pSource->endBuild());
        PHANTOM_EMIT sourceBuilt(pSource);
    }
    PHANTOM_EMIT buildSessionSucceeded(a_BuildSession);
    PHANTOM_EMIT buildSessionDone(a_BuildSession);
}

void BuildSystem::_onModuleAdded(Module* a_pModule)
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

void BuildSystem::_onModuleDiscarded(Module*) {}

// TODO : rename "nativeModuleAboutToBeRemoved"

void BuildSystem::_onModuleAboutToBeRemoved(Module* a_pModule)
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

bool BuildSystem::isBuildOutdated() const
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

CodeGenerator* BuildSystem::_createCodeGenerator()
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

void BuildSystem::_destroyCodeGenerator(CodeGenerator* a_pCodeGenerator)
{
    PHANTOM_ASSERT(m_pCodeGeneratorClass);
    m_pCodeGeneratorClass->deleteInstance(
    PHANTOM_CLASSOF(CodeGenerator)->cast(m_pCodeGeneratorClass, a_pCodeGenerator));
}

void BuildSystem::_outdateModule(Source* a_pSource)
{
    if (CodeGenerator* pCG = BuildSystem::Get()->getCodeGenerator(a_pSource->getModule()))
        pCG->outdate();
}

void BuildSystem::_setCodeGenerator(Module* a_pModule, CodeGenerator* a_pCodeGenerator)
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

void BuildSystem::_createCodeGenerator(Module* a_pModule)
{
    PHANTOM_ASSERT(getCodeGenerator(a_pModule) == nullptr);
    _setCodeGenerator(a_pModule, _createCodeGenerator());
    m_ModuleData[a_pModule].codeGenerator = _createCodeGenerator();
    m_ModuleData[a_pModule].codeGenerator->m_pModule = a_pModule;
}

void BuildSystem::_destroyCodeGenerator(Module* a_pModule)
{
    CodeGenerator*& pCodeGenerator = m_ModuleData[a_pModule].codeGenerator;
    _setCodeGenerator(a_pModule, nullptr);
    PHANTOM_ASSERT(pCodeGenerator);
    pCodeGenerator->m_pModule = nullptr;
    pCodeGenerator = nullptr;
    _destroyCodeGenerator(pCodeGenerator);
}

void BuildSystem::addLanguage(Language* a_pLanguage)
{
    m_Languages.push_back(a_pLanguage);
    PHANTOM_EMIT languageAdded(a_pLanguage);
}

void BuildSystem::removeLanguage(Language* a_pLanguage)
{
    PHANTOM_EMIT languageAboutToBeRemoved(a_pLanguage);
    for (auto it = m_BuildSources.begin(); it != m_BuildSources.end();)
    {
        if (it->second->getLanguage() == a_pLanguage)
            it = m_BuildSources.erase(it);
        else
            ++it;
    }
    m_Languages.erase(std::find(m_Languages.begin(), m_Languages.end(), a_pLanguage));
}

void BuildSystem::restoreBuildSession(BuildSessionId a_Id)
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

void BuildSystem::codeGen(Module* a_pModule, Options a_Options /*= {}*/, Message* a_pMsg /*= nullptr*/)
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
