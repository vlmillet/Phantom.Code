#pragma once

#include "Message.h"
#include "fwd.h"

#include <phantom/thread/RecursiveSpinMutex.h>

namespace phantom
{
namespace lang
{
class Compiler;

struct ProjectBuildSession
{
    Message*        message{};
    CompiledSources all;
    CompiledSources succeeded;
    CompiledSources failed;
};

struct PHANTOM_EXPORT_PHANTOM_CODE BuildSession
{
    friend class Compiler;

public:
    bool isSuccessful() const;

    bool addProject(StringView _name);
    bool addProjectPath(StringView _path);
    void addSearchPath(StringView _path);

    void loadSources(StringView a_SourceUniqueName);
    void loadPackages(StringView a_PackageUniqueName);
    void loadSourcesOrPackages(StringView a_UniqueName);

    SmallVector<ProjectBuildSession const*> getBuiltProjectsSessions() const
    {
        SmallVector<ProjectBuildSession const*> bps;
        for (auto& bp : builtProjects)
        {
            bps.push_back(projects.find(bp)->second);
        }
        return bps;
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
