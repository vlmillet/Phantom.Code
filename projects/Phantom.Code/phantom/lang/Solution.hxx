#pragma once

// haunt {

// clang-format off

#include "Solution.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Solution")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(SolutionData)
        {
            using Options = typedef_< phantom::lang::Options>;
            using ProjectInfo = typedef_<_::ProjectInfo>;
            using String = typedef_< phantom::String>;
            this_()
            .struct_<ProjectInfo>()
                .field("path", &_::ProjectInfo::path)
            .end()
            .field("name", &_::name)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("options", &_::options)
            .field("projects", &_::projects)
            ;
        }
        PHANTOM_CLASS(Solution)
        {
            using Options = typedef_< phantom::lang::Options>;
            using Projects = typedef_< phantom::lang::Projects>;
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
        .public_()
            .constructor<void(StringView)>()
            .method<StringView() const>("getName", &_::getName)
            .method<SolutionData() const>("getData", &_::getData)
            .method<bool(SolutionData)>("setData", &_::setData)
            .method<void(StringView)>("setName", &_::setName)
            .method<StringView() const>("getPath", &_::getPath)
            .method<String() const>("getFolder", &_::getFolder)
            .method<Project*(StringView) const>("getProjectFromName", &_::getProjectFromName)
            .method<Project*(StringView) const>("getProjectFromPath", &_::getProjectFromPath)
            .method<Project*(Module*) const>("getProjectFromModule", &_::getProjectFromModule)
            .method<Project*(StringView, Module*)>("addProject", &_::addProject)["nullptr"]
            .method<Project*(StringView, StringView, Module*)>("addProject", &_::addProject)
            .method<void(StringView)>("removeProject", &_::removeProject)
            .method<Projects const&() const>("getProjects", &_::getProjects)
            .method<Projects() const>("getProjectsSortedByDependencies", &_::getProjectsSortedByDependencies)
            .method<Options const&() const>("getOptions", &_::getOptions)
            .method<void(Options const&)>("setOptions", &_::setOptions)
            .method<bool() const>("exists", &_::exists)
            .method<void()>("removeFromDisk", &_::removeFromDisk)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Solution")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
