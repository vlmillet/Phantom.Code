// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "lang.h"
#include "phantom/utils/String.h"

namespace phantom
{
namespace lang
{
struct SolutionData
{
    struct ProjectInfo
    {
        String path;
    };
    String                   name;
    Options                  options;
    SmallVector<ProjectInfo> projects;
};

class PHANTOM_EXPORT_PHANTOM_CODE Solution
{
public:
    Solution(StringView _path);
    ~Solution();

    StringView getName() const { return m_Name; }

    SolutionData getData() const;
    bool         setData(SolutionData _data);

    void setName(StringView a_Name) { m_Name = a_Name; }

    StringView getPath() const { return m_Path; }
    String     getFolder() const;

    Project* getProjectFromName(StringView a_Name) const;
    Project* getProjectFromPath(StringView a_Path) const;
    Project* getProjectFromModule(Module* a_pModule) const;

    Project* addProject(StringView a_Path, Module* a_pExtendedNativeModule = nullptr);
    Project* addProject(StringView a_Name, StringView a_Path, Module* a_pNativeModule /*= nullptr*/);
    void     removeProject(StringView a_Path);

    Projects const& getProjects() const { return m_Projects; }

    Projects getProjectsSortedByDependencies() const;

    Options const& getOptions() const { return m_Options; }

    void setOptions(Options const& _options) { m_Options = _options; }

    bool exists() const;

    void removeFromDisk();

private:
    String   m_Name;
    String   m_Path;
    Options  m_Options;
    Projects m_Projects;
};

} // namespace lang
} // namespace phantom
