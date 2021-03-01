// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "Solution.h"

#include "Compiler.h"
#include "Project.h"

#include <phantom/lang/Application.h>
#include <phantom/lang/Module.h>
#include <phantom/lang/Plugin.h>
#include <phantom/utils/Path.h>
#include <system_error>

namespace phantom
{
namespace lang
{
Solution::Solution(StringView _path) : m_Path(phantom::Path(_path).absolute().canonical().genericString()) {}

Solution::~Solution()
{
    auto   sorted = getProjectsSortedByDependencies();
    size_t i = sorted.size();
    while (i--)
    {
        delete_<Project>(sorted[i]);
    }
}

String Solution::getFolder() const
{
    return Path(m_Path).parentPath().genericString();
}

Project* Solution::getProjectFromName(StringView a_Name) const
{
    for (auto p : m_Projects)
        if (p->getName() == a_Name)
            return p;
    return nullptr;
}

Project* Solution::getProjectFromPath(StringView a_Path) const
{
    Path path(a_Path);
    if (!path.isAbsolute())
        path = path.absolute();

    for (Project* p : m_Projects)
        if (Path::Equivalent(p->getPath(), path))
            return p;
    return nullptr;
}

Project* Solution::getProjectFromModule(Module* a_pModule) const
{
    for (Project* p : m_Projects)
        if (p->getModule() == a_pModule)
            return p;
    return nullptr;
}

Project* Solution::addProject(StringView a_Name, StringView a_Path, Module* a_pNativeModule /*= nullptr*/)
{
    PHANTOM_ASSERT(!a_Name.empty());
    Project* existingProject;
    if (a_pNativeModule && (existingProject = getProjectFromModule(a_pNativeModule)))
        return existingProject;
    if (existingProject = getProjectFromPath(a_Path))
        return existingProject;
    Path p(a_Path);
    Path solutionFolder(m_Path);
    solutionFolder = solutionFolder.parentPath();
    if (!p.isAbsolute())
    {
        p = p.absolute();
    }
    if (!a_pNativeModule)
    {
        a_pNativeModule = Application::Get()->newModule(a_Name);
        Application::Get()->addModule(a_pNativeModule);
    }
    Project* pProject = new_<Project>(this, p.relative(solutionFolder).genericString(), a_pNativeModule);
    pProject->setName(a_Name);
    m_Projects.push_back(pProject);
    return pProject;
}

Project* Solution::addProject(StringView a_Path, Module* a_pExtendedNativeModule /*= nullptr*/)
{
    return addProject(phantom::Path(a_Path).stem(), a_Path, a_pExtendedNativeModule);
}

void Solution::removeProject(StringView a_Path)
{
    Project* pProject = getProjectFromName(a_Path);
    PHANTOM_ASSERT(pProject);
    if (pProject->getModule()->getPlugin() == nullptr)
    {
        Application::Get()->removeModule(pProject->getModule());
    }
    auto found = std::find(m_Projects.begin(), m_Projects.end(), pProject);
    PHANTOM_ASSERT(found != m_Projects.end());
    m_Projects.erase(found);
    delete_<Project>(pProject);
}

Projects Solution::getProjectsSortedByDependencies() const
{
    auto projectsSortedByDependencies = m_Projects;
    std::sort(projectsSortedByDependencies.begin(), projectsSortedByDependencies.end(),
              [](Project* p0, Project* p1) { return p0->getDependencyLevel() < p1->getDependencyLevel(); });
    return projectsSortedByDependencies;
}

bool Solution::exists() const
{
    return Path::Exists(m_Path) && !Path::IsDirectory(m_Path);
}

void Solution::removeFromDisk()
{
    for (Project* pProj : m_Projects)
    {
        pProj->removeFromDisk();
    }
    Path::Remove(m_Path);
}

SolutionData Solution::getData() const
{
    SolutionData data;
    data.name = m_Name;
    data.options = m_Options;
    for (auto p : m_Projects)
    {
        data.projects.push_back(SolutionData::ProjectInfo{p->getPath()});
    }
    return data;
}

bool Solution::setData(SolutionData _data)
{
    m_Name = _data.name;
    m_Options = _data.options;
    for (SolutionData::ProjectInfo const& inf : _data.projects)
    {
        m_Projects.push_back(new_<Project>(this, inf.path, nullptr));
    }
    return true;
}

} // namespace lang
} // namespace phantom
