#include "BuildSession.h"

#include "BuildSystem.h"
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

String BuildSession::findProjectPath(StringView _projectName) const
{
    for (auto sp : searchPaths)
    {
        auto p = Path(sp).childPath(_projectName);
        if (p.exists() && p.isDirectory())
        {
            return p.genericString();
        }
    }
    return {};
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
                    if (BuildSystem::Get()->getLanguageFromFileExtension(relSrcPath.extension()))
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

} // namespace lang
} // namespace phantom
