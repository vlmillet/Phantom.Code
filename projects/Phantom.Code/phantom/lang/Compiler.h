// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "BuildSession.h"

#include <phantom/utils/Signal.h>
#include <phantom/utils/SmallMap.h>

namespace phantom
{
namespace lang
{
class Translator;
class Module;
class PHANTOM_EXPORT_PHANTOM_CODE Compiler
{
    friend class CompiledSource;
    friend class Language;
    friend struct BuildSession;

public:
    typedef SmallMap<Source*, uint>   SourceVersions;
    typedef SmallMap<Source*, time_t> SourceTimes;

public:
    static Compiler* Get();

    Compiler();
    ~Compiler();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Returns the debugger associated with the application for source debugging.
    ///
    /// \return a debugger.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Debugger* getDebugger() const { return m_pDebugger; }

    BuildSessionId newSession(ArrayView<String> _projectsSearchPaths = {}, Options const& a_Options = {});

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Build a specific project from its name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void buildProject(BuildSessionId a_Id, StringView a_Path);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Build a specific project from its path.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void buildProjectAt(BuildSessionId a_Id, StringView a_Path);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Build a specific project.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CompiledSource* buildSource(BuildSessionId a_Id, StringView a_Path,
                                ArrayView<String> _projectsSearchPaths = ArrayView<String>(),
                                Options const&    a_Options = Options());

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get successful status of a build sesssion.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    BuildSession const& getBuildSession(BuildSessionId a_Id) const
    {
        PHANTOM_ASSERT(a_Id != -1 && a_Id < int(m_BuildSessions.size()));
        return *m_BuildSessions[a_Id];
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get successful status of a build sesssion.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Modules getBuildSessionModules(BuildSessionId a_Id) const
    {
        Modules modules;
        auto&   ses = m_BuildSessions[a_Id];
        for (auto& proj_ : ses->projects)
        {
            modules.push_back(m_CompiledModules.find(proj_.first)->second);
        }
        return modules;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Release every resource hold for the given project (build cache among others).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void cleanupProject(StringView _path);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Clean every source.
    ///
    /// \param  a_pSourceOldTimes   (optional) [in,out] If non-null, will receive the old build
    /// times of the sources (to restore them in case of smart undo)
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void cleanBuild(StringView _path);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Check if build is outdated (if any source with source file is outdated).
    ///
    /// \return true if build outdated, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isBuildOutdated() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Abort any running build.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void abortBuild();

    void outdate(SourceFile* a_pSource) const;

    CompiledSources listProjectCompiledSources(StringView _path);

    CompiledSource* getCompiledSource(SourceStream* a_pSourceStream) const;

    CompiledSource* getCompiledSource(Source* a_pSource) const;

    CodeGenerator* getCodeGenerator(Module* a_pModule) const;

    DebugInformation* getDebugInformation(Module* a_pModule) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find the language matching the given name.
    ///
    /// \param  a_strLanguageName The language name.
    ///
    /// \return null if it fails, else the language matching the given name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Language* getLanguage(StringView a_strLanguageName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get all the languages registered in this application.
    ///
    /// \return a list of languages.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Languages const& getLanguages() const { return m_Languages; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find the language matching the given source file extension.
    ///
    /// \param  a_Extension The extension.
    ///
    /// \return null if it fails, else the language matching the source file extension.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Language* getLanguageFromFileExtension(StringView a_Extension) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find the language matching the given source.
    ///
    /// \param  a_Extension The extension.
    ///
    /// \return null if it fails, else the language matching the source file extension.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Language* findSourceLanguage(Source* a_pSource) const;

    Language* findSourceStreamLanguage(SourceStream* a_pStream) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default language associated with this application. (C++ is the default one
    /// if not changed)
    ///
    /// \return null if it fails, else the default language.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline Language* getDefaultLanguage() const { return m_pDefaultLanguage; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default language associated with this application. (C++ is the default one)
    ///
    /// \param [in,out] a_pLanguage If non-null, the value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void setDefaultLanguage(Language* a_pLanguage) { m_pDefaultLanguage = a_pLanguage; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a language to the compiler.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addLanguage(Language* a_pLanguage);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a language from the compiler.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeLanguage(Language* a_pLanguage);

    void restoreBuildSession(BuildSessionId a_Id);

    void codeGen(Module* a_pModule, Options a_Options = {}, Message* a_pMsg = nullptr);

public:
    phantom::Signal<void(Language*)>                  languageAdded;
    phantom::Signal<void(Language*)>                  languageAboutToBeRemoved;
    phantom::Signal<void(CompiledSource*)>            sourceBuilt;
    phantom::Signal<void(const CompiledSources&)>     sourceBuildStarted;
    phantom::Signal<void(CompiledSource*, bool&)>     buildSessionUserHook;
    phantom::Signal<void(const ProjectBuildSession&)> buildSessionDone;
    phantom::Signal<void(const ProjectBuildSession&)> buildSessionApplied;

private:
    void _buildProject(BuildSession& a_BuildSession, ProjectBuildSession& _sessionInfo, StringView _path,
                       Options const& a_Options);
    void _buildSourceFiles(BuildSession& a_BuildSession, ProjectBuildSession& a_ProjBuildSession,
                           ArrayView<SourceFile*> a_SourceFiles, StringView _path, Options const& a_Options);
    void _finalizeBuild(StringView a_Path, Module* a_pModule, Options const& a_Options, ProjectBuildSession& toParse,
                        ArrayView<SourceFile*> a_Files, bool a_bRestoreSources = false);
    void _onModuleAdded(Module* a_pModule);
    void _onModuleDiscarded(Module* a_pModule);
    void _onModuleAboutToBeRemoved(Module* a_pModule);
    void _setCodeGenerator(Module* a_pModule, CodeGenerator* a_pCodeGenerator);
    void _createCodeGenerator(Module* a_pModule);
    void _destroyCodeGenerator(Module* a_pModule);
    CodeGenerator*  _createCodeGenerator();
    void            _destroyCodeGenerator(CodeGenerator* a_pCodeGenerator);
    void            _outdateModule(Source* a_pSource);
    CompiledSource* _getOrCreateCompiledSource(SourceStream* a_pSourceStream);
    void            _deleteCompiledSource(SourceStream* a_pSourceStream);
    void            _undoSession(BuildSession& _session);
    void            _redoSession(BuildSession& _session);
    SourceFile*     _getOrCreateSourceFile(StringView _path);
    Module*         _getOrCreateModule(StringView _path);
    void            _listSourceFiles(StringView _path, SmallVector<SourceFile*>& _sourceFiles);

private:
    Class* m_pCodeGeneratorClass = nullptr;
    struct ModuleData
    {
        CodeGenerator*    codeGenerator = nullptr;
        DebugInformation* debugInfo = nullptr;
    };
    SmallMap<String, SourceStream*, 64>          m_UrlToSourceStream;
    SmallMap<SourceStream*, CompiledSource*, 64> m_CompiledSources;
    SmallMap<String, Module*, 64>                m_CompiledModules;
    SmallMap<Module*, ModuleData, 64>            m_ModuleData;
    SourceVersions                               m_ManualPreviousVersions;
    Strings                                      m_ManuallyLoadedModuleNames;
    CompiledSources                              m_ManualSources;
    Language*                                    m_pDefaultLanguage = nullptr;
    Languages                                    m_Languages;
    Debugger*                                    m_pDebugger = nullptr;
    SmallVector<BuildSession*>                   m_BuildSessions;
    Modules                                      m_BuildingModules;
    int                                          m_CurrentSessionId = -1;
    int                                          m_BuildingSessionId = -1;
    bool                                         m_bBuildRunning = false;
    bool                                         m_bBuildAborted = false;
};
} // namespace lang
} // namespace phantom
