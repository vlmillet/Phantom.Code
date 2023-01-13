#pragma once

#include "Message.h"
#include "fwd.h"

#include <phantom/thread/RecursiveSpinMutex.h>

namespace phantom
{
namespace lang
{
class BuildSystem;

struct ProjectBuildSession
{
    Message*     message{};
    BuildSources all;
    BuildSources succeeded;
    BuildSources failed;
};

struct PHANTOM_EXPORT_PHANTOM_CODE BuildSession
{
    friend class BuildSystem;

public:
    bool isSuccessful() const;

    String findProjectPath(StringView _projectName) const;
    void   addSearchPath(StringView _path);

    SmallVector<ProjectBuildSession const*> getProjectBuildSessions() const
    {
        SmallVector<ProjectBuildSession const*> bps;
        for (auto& bp : builtProjects)
        {
            bps.push_back(projects.find(bp)->second);
        }
        return bps;
    }
    ProjectBuildSession const* getProjectBuildSession(StringView _projectPath) const
    {
        auto found = projects.find(_projectPath);
        return found == projects.end() ? nullptr : found->second;
    }

private:
    SmallMap<String, ProjectBuildSession*> projects;
    SmallVector<String>                    searchPaths;
    SmallMap<String, SmallVector<String>>  projectsPathFromSourceUniqueName;
    SmallMap<String, SmallVector<String>>  projectsPathFromPackageName;
    Options                                options;
    SmallVector<String>                    builtProjects;
    RecursiveSpinMutex                     mutex;
};
} // namespace lang
} // namespace phantom
