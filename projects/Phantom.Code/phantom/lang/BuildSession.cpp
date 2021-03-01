#include "BuildSession.h"

#include "Compiler.h"
#include "phantom/lang/Application.h"
#include "phantom/lang/Package.h"
#include "phantom/utils/Path.h"

namespace phantom
{
namespace lang
{
bool BuildSession::isSuccessful() const
{
    for (auto& p : projects)
    {
        if (!p.second->failed.empty())
            return false;
    }
    return true;
}

bool BuildSession::addProject(StringView _name)
{
    for (auto sp : searchPaths)
    {
        auto p = Path(sp).childPath(_name);
        if (p.exists() && p.isDirectory())
        {
            return addProjectPath(p.genericString());
        }
    }
    return false;
}

bool BuildSession::addProjectPath(StringView a_Path)
{
    auto lock = mutex.autoLock();
    Path p(a_Path);
    if (!p.exists())
        return false;
    String path = p.absolute().canonical().genericString();
    auto   found = projects.find(path);
    if (found == projects.end())
    {
        auto& projSess = projects[path];
        projSess = new_<ProjectBuildSession>();
        projSess->message = new_<Message>();
        projSess->message->setText(Path(a_Path).filename());
        Compiler::Get()->_buildProject(*this, *projSess, path, options);
        builtProjects.push_back(path);
        return projSess->failed.empty();
    }
    return found->second->failed.empty();
}

void BuildSession::addSearchPath(StringView _path)
{
    searchPaths.push_back(Path::Absolute(_path).canonical().genericString());
    auto             lock = mutex.autoLock();
    DirectoryEntries projectEntries;
    Path::ListDirectoryEntries(_path, projectEntries);
    for (DirectoryEntry& projectEntry : projectEntries)
    {
        if (projectEntry.path().back().front() == '.' || !Package::IsValidName(projectEntry.path().stem()))
            continue;
        if (projectEntry.isDirectory())
        {
            DirectoryEntries sourceEntries;
            Path::ListDirectoryEntries(projectEntry.path(), sourceEntries, true);
            for (DirectoryEntry& sourceEntry : sourceEntries)
            {
                if (sourceEntry.path().back().front() == '.' || !Package::IsValidName(sourceEntry.path().stem()))
                    continue;
                if (sourceEntry.isRegularFile())
                {
                    Path relSrcPath = sourceEntry.path().relative(projectEntry.path());
                    if (Compiler::Get()->getLanguageFromFileExtension(relSrcPath.extension()))
                    {
                        relSrcPath.removeExtension();
                    }
                    projectsPathFromSourceUniqueName[relSrcPath.customString(".")].push_back(
                    projectEntry.path().absolute().canonical().genericString());
                    projectsPathFromPackageName[relSrcPath.parentPath().customString(".")].push_back(
                    projectEntry.path().absolute().canonical().genericString());
                }
            }
        }
    }
}

void BuildSession::loadSources(StringView a_SourceUniqueName)
{
    auto lock = mutex.autoLock();
    auto found = projectsPathFromSourceUniqueName.find(a_SourceUniqueName);
    if (found != projectsPathFromSourceUniqueName.end())
    {
        for (auto& projPath : found->second)
            addProjectPath(projPath);
    }
}

void BuildSession::loadPackages(StringView a_PackageUniqueName)
{
    auto lock = mutex.autoLock();
    auto found = projectsPathFromPackageName.find(a_PackageUniqueName);
    if (found != projectsPathFromPackageName.end())
    {
        for (auto& projPath : found->second)
            addProjectPath(projPath);
    }
}

void BuildSession::loadSourcesOrPackages(StringView a_UniqueName)
{
    loadSources(a_UniqueName);
    loadPackages(a_UniqueName);
}

} // namespace lang
} // namespace phantom
