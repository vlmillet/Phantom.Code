#pragma once

// haunt {

// clang-format off

#include "BuildSession.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("BuildSession")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(ProjectBuildSession)
        {
            using CompiledSources = typedef_< phantom::lang::CompiledSources>;
            this_()
            .field("message", &_::message)
            .field("all", &_::all)
            .field("succeeded", &_::succeeded)
            .field("failed", &_::failed)
            ;
        }
        PHANTOM_STRUCT(BuildSession)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
        
        .public_()
            .method<bool() const>("isSuccessful", &_::isSuccessful)
            .method<bool(StringView)>("addProject", &_::addProject)({"_name"})
            .method<String(StringView) const>("findProjectPath", &_::findProjectPath)({"_projectName"})
            .method<bool(StringView)>("addProjectPath", &_::addProjectPath)({"_path"})
            .method<void(StringView)>("addSearchPath", &_::addSearchPath)({"_path"})
            .method<void(StringView)>("loadSources", &_::loadSources)({"a_SourceUniqueName"})
            .method<void(StringView)>("loadPackages", &_::loadPackages)({"a_PackageUniqueName"})
            .method<void(StringView)>("loadSourcesOrPackages", &_::loadSourcesOrPackages)({"a_UniqueName"})
            .method<SmallVector<ProjectBuildSession const*>() const>("getBuiltProjectsSessions", &_::getBuiltProjectsSessions)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BuildSession")
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
