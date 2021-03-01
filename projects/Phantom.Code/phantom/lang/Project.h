// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "lang.h"
#include "phantom/private"
#include "phantom/utils/SmallMap.h"
#include "phantom/utils/SmallVector.h"
#include "phantom/utils/String.h"

#include <phantom/utils/SmallSet.h>

namespace phantom
{
namespace lang
{
class Solution;
PHANTOM_FORWARD_DECL_NAMESPACE;

struct ProjectData
{
    Strings files;
    Strings dependencies;
    Options options;
};

class PHANTOM_EXPORT_PHANTOM_CODE Project
{
    friend class Solution;

    PHANTOM_PRIVATE_ACCESS;

public:
    Project(Solution* a_pSolution, StringView a_Path, Module* a_pModule);
    ~Project();

    bool        setData(ProjectData _data);
    ProjectData getData() const;

    Solution* getSolution() const { return m_pSolution; }

    int getDependencyLevel() const;

    StringView         getSolutionRelativePath() const { return m_Path; }
    String             getPath() const;
    StringView         getName() const { return m_Name; }
    void               setName(StringView a_Name) { m_Name = a_Name; }
    StringView         getOutputDir() const { return m_OutputDir; }
    Options const&     getOptions() const { return m_Options; }
    void               setOptions(Options const& _options) { m_Options = _options; }
    SourceFiles const& getSourceFiles() const { return m_SourceFiles; }
    Modules const&     getDependencies() const { return m_Dependencies; }
    Projects           getDependenciesProjects() const;
    Module*            getModule() const { return m_pModule; }
    String             getSourcePath() const;

    Package* getPackageForSourceStream(SourceStream* a_pStream) const;

    SourceFile* getSourceFileByPath(StringView a_Path) const;

    bool addDependency(StringView a_ModuleName);
    void addDependency(Module* a_pModule);
    void removeDependency(Module* a_pModule);
    bool hasDependency(Module* a_pModule) const;

    SourceFile* addSourceFile(StringView a_RelativePath, StringView a_Code = "");
    bool        addSourceFile(SourceFile* a_pSourceFile);
    void        removeSourceFile(SourceFile* a_pSourceFile);

    bool isPathExisting() const;

    bool hasProjectDependencyCascade(Project* a_pOther) const;

    CompiledSources getCompiledSources() const;
    void            getCompiledSources(CompiledSources& _out) const;

private:
    void removeFromDisk();

private:
    Solution*   m_pSolution;
    String      m_Name;
    String      m_OutputDir;
    String      m_Path;
    Options     m_Options;
    SourceFiles m_SourceFiles;
    Modules     m_Dependencies;
    Module*     m_pModule;
    Message*    m_pMessage = nullptr;
};

} // namespace lang
} // namespace phantom
