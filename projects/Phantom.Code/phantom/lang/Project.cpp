// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "Project.h"

#include "BuildSource.h"
#include "BuildSystem.h"
#include "Solution.h"

#include <fstream>
#include <phantom/lang/Application.h>
#include <phantom/lang/Module.h>
#include <phantom/lang/Package.h>
#include <phantom/lang/Plugin.h>
#include <phantom/lang/SourceFile.h>
#include <phantom/utils/Path.h>
#include <phantom/utils/StringUtil.h>
#include <system_error>

namespace phantom
{
namespace lang
{
Project::~Project()
{
    if (m_pModule)
    {
        if (!m_pModule->isNative())
        {
            if (m_pModule->getOwner())
                Application::Get()->removeModule(m_pModule);
            Application::Get()->deleteModule(m_pModule);
            m_pModule = nullptr;
        }
    }
}

Projects Project::getDependenciesProjects() const
{
    Projects projs;
    for (auto pMod : getDependencies())
    {
        if (Project* pDep = getSolution()->getProjectFromModule(pMod))
            projs.push_back(pDep);
    }
    return projs;
}

phantom::String Project::getSourcePath() const
{
    return Path(m_Path).parentPath().genericString();
}

Package* Project::getPackageForSourceStream(SourceStream* a_pStream) const
{
    PHANTOM_ASSERT(std::find(m_SourceFiles.begin(), m_SourceFiles.end(), a_pStream) != m_SourceFiles.end());
    Path p(Path(a_pStream->getPath()).relative(Path(getPath()).parentPath()));
    if (p.size() == 1)
    {
        return m_pModule->getOrCreatePackage("default");
    }
    String packageName;
    for (size_t i = 0; i < p.size() - 1; ++i)
    {
        if (packageName.empty())
            packageName = p[i];
        else
            packageName += '.' + p[i];
    }
    return m_pModule->getOrCreatePackage(packageName);
}

SourceFile* Project::getSourceFileByPath(StringView a_Path) const
{
    for (SourceFile* pSourceFile : m_SourceFiles)
        if (Path::Equivalent(Path(pSourceFile->getPath()).relative(Path(getPath()).parentPath()),
                             Path(a_Path).relative(Path(getPath()).parentPath())))
            return pSourceFile;
    return nullptr;
}

void Project::addDependency(Module* a_pModule)
{
    PHANTOM_ASSERT(std::find(m_Dependencies.begin(), m_Dependencies.end(), a_pModule) == m_Dependencies.end());
    PHANTOM_ASSERT(m_pSolution->getProjectFromModule(a_pModule) || a_pModule->getPlugin(),
                   "the module is neither a project neither a plugin");
    m_Dependencies.push_back(a_pModule);
    m_pModule->addDependency(a_pModule);
}

bool Project::addDependency(StringView a_Name)
{
    if (std::find_if(m_Dependencies.begin(), m_Dependencies.end(), [&](Module* m) { return m->getName() == a_Name; }) !=
        m_Dependencies.end())
    {
        PHANTOM_LOG(Warning, "'%.*s' : dependency project '%.*s' already declared",
                    PHANTOM_STRING_AS_PRINTF_ARG(m_Path), PHANTOM_STRING_AS_PRINTF_ARG(a_Name));
        return true;
    }
    if (Project* pProject = m_pSolution->getProjectFromName(a_Name))
    {
        if (pProject == this || pProject->hasProjectDependencyCascade(this))
            return false;
        addDependency(pProject->getModule());
        return true;
    }
    if (Plugin* pPlugin = Application::Get()->getPlugin(a_Name))
    {
        pPlugin->load();
        addDependency(pPlugin->getModule());
        return true;
    }
    return false;
}

void Project::removeDependency(Module* a_pModule)
{
    PHANTOM_ASSERT(m_pSolution->getProjectFromModule(a_pModule) || a_pModule->getPlugin());
    auto found = std::find(m_Dependencies.begin(), m_Dependencies.end(), a_pModule);
    PHANTOM_ASSERT(found != m_Dependencies.end());
    m_pModule->removeDependency(a_pModule);
    m_Dependencies.erase(found);
}

bool Project::hasDependency(Module* a_pModule) const
{
    auto found = std::find(m_Dependencies.begin(), m_Dependencies.end(), a_pModule);
    return found != m_Dependencies.end();
}

bool Project::addSourceFile(SourceFile* a_pSourceFile)
{
    PHANTOM_ASSERT(getSourceFileByPath(a_pSourceFile->getPath()) == nullptr, "source file already added");
    m_SourceFiles.push_back(a_pSourceFile);
    return true;
}

SourceFile* Project::addSourceFile(StringView a_RelativePath, StringView a_Code /*= ""*/)
{
    if (getSourceFileByPath(a_RelativePath))
        return nullptr;
    Path projectFullPath(getPath());
    Path sourcePath = a_RelativePath;
    if (!sourcePath.isAbsolute())
        sourcePath = projectFullPath.parentPath().childPath(sourcePath);
    if (!sourcePath.exists())
    {
        std::error_code errcode;
        if (!Path::CreateDirectories(sourcePath.parentPath(), errcode))
            return nullptr;
        std::ofstream os(sourcePath.genericString().c_str());
        if (!os.is_open())
            return nullptr;
        os.write(a_Code.data(), a_Code.size());
    }
    SourceFile* pSourceFile = new_<SourceFile>(sourcePath.genericString());
    if (!addSourceFile(pSourceFile))
    {
        delete_<SourceFile>(pSourceFile);
        return nullptr;
    }
    return pSourceFile;
}

void Project::removeSourceFile(SourceFile* a_pSourceFile)
{
    auto found = std::find(m_SourceFiles.begin(), m_SourceFiles.end(), a_pSourceFile);
    PHANTOM_ASSERT(found != m_SourceFiles.end());
    m_SourceFiles.erase(found);
}

bool Project::isPathExisting() const
{
    Path path = getPath();
    return path.exists() && path.isDirectory();
}

bool Project::hasProjectDependencyCascade(Project* a_pOther) const
{
    if (hasDependency(a_pOther->getModule()))
        return true;
    for (Module* pDep : m_Dependencies)
    {
        if (Project* pDepProj = m_pSolution->getProjectFromModule(pDep))
            if (pDepProj->hasProjectDependencyCascade(a_pOther))
                return true;
    }
    return false;
}

void Project::getBuildSources(BuildSources& _out) const
{
    for (auto source : m_SourceFiles)
    {
        if (auto pCS = BuildSystem::Get()->getCompiledSource(source))
            _out.push_back(pCS);
    }
}

phantom::lang::BuildSources Project::getBuildSources() const
{
    BuildSources sources;
    getBuildSources(sources);
    return sources;
}

phantom::String Project::getPath() const
{
    if (Path::IsAbsolute(m_Path))
    {
        return m_Path;
    }
    return Path(m_pSolution->getPath()).parentPath().childPath(m_Path).genericString();
}

Project::Project(Solution* a_pSolution, StringView a_Path, Module* a_pModule)
    : m_pSolution(a_pSolution), m_Path(a_Path), m_pModule(a_pModule)
{
}

ProjectData Project::getData() const
{
    ProjectData data;
    data.options = m_Options;
    for (SourceFile* pSourceFile : m_SourceFiles)
    {
        data.files.push_back(Path(pSourceFile->getPath()).relative(Path(getPath()).parentPath()).genericString());
    }
    for (Module* pModule : m_Dependencies)
    {
        if (Project* pProject = m_pSolution->getProjectFromModule(pModule))
        {
            data.dependencies.push_back(pProject->getName());
        }
        else
        {
            PHANTOM_ASSERT(pModule->getPlugin());
            data.dependencies.push_back(pModule->getPlugin()->getName());
        }
    }
    return data;
}

int Project::getDependencyLevel() const
{
    int maxLevel = 0;
    for (auto dep : m_Dependencies)
    {
        if (Project* pProject = m_pSolution->getProjectFromModule(dep))
        {
            int level = pProject->getDependencyLevel() + 1;
            if (level > maxLevel)
                maxLevel = level;
        }
    }
    return maxLevel;
}

bool Project::setData(ProjectData _data)
{
    PHANTOM_ASSERT(m_pModule);
    m_Options = _data.options;
    for (auto& file : _data.files)
    {
        Path p(Path(getPath()).parentPath().childPath(file));
        if (getSourceFileByPath(p.genericString()))
        {
            PHANTOM_LOG(Warning, "file '%.*s' already exists in this project, skipping doublon",
                        PHANTOM_STRING_AS_PRINTF_ARG(p.genericString()));
            continue;
        }
        addSourceFile(new_<SourceFile>(p.genericString()));
    }
    for (auto& dep : _data.dependencies)
    {
        if (!addDependency(dep))
            return false;
    }
    return true;
}

void Project::removeFromDisk()
{
    Path::Remove(getPath());
}

} // namespace lang
} // namespace phantom
