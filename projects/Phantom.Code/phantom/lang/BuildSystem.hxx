#pragma once

// haunt {

// clang-format off

#include "BuildSystem.h"

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
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("BuildSystem")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(BuildSystem)
        {
            using BuildSources = typedef_< phantom::lang::BuildSources>;
            using Languages = typedef_< phantom::lang::Languages>;
            using Modules = typedef_< phantom::lang::Modules>;
            using Options = typedef_< phantom::lang::Options>;
            using SourceVersions = typedef_<_::SourceVersions>;
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
        
        .public_()
            .typedef_<SourceVersions>("SourceVersions")
            /// missing symbol(s) reflection (time_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .typedef_<SourceTimes>("SourceTimes")
        
        .public_()
            .staticMethod<::phantom::lang::BuildSystem *()>("Get", &_::Get)
            .constructor<void()>()
            .method<Debugger*() const>("getDebugger", &_::getDebugger)
            .method<BuildSessionId(ArrayView<String>, Options const&)>("newSession", &_::newSession)({"_projectsSearchPaths","a_Options"})["{}"]["{}"]
            .method<bool(BuildSessionId, StringView)>("buildProject", &_::buildProject)({"a_Id","a_Name"})
            .method<bool(BuildSessionId, StringView)>("buildProjectAndDependents", &_::buildProjectAndDependents)({"a_Id","a_Name"})
            .method<bool(BuildSessionId, StringView)>("buildProjectAt", &_::buildProjectAt)({"a_Id","a_Path"})
            .method<bool(BuildSessionId, StringView)>("buildProjectAndDependentsAt", &_::buildProjectAndDependentsAt)({"a_Id","a_Path"})
            .method<bool(BuildSessionId, Module*)>("buildProject", &_::buildProject)({"a_Id","a_pModule"})
            .method<bool(BuildSessionId, Module*)>("buildProjectAndDependents", &_::buildProjectAndDependents)({"a_Id","a_pModule"})
            .method<bool(BuildSessionId) const>("hasBuildSucceeded", &_::hasBuildSucceeded)({"a_Id"})
            .method<void(BuildSessionId, StringView)>("loadSourcesOrPackages", &_::loadSourcesOrPackages)({"a_Id","a_UniqueName"})
            .method<BuildSource*(BuildSessionId, StringView, ArrayView<String>, Options const&)>("buildSource", &_::buildSource)({"a_Id","a_Path","_projectsSearchPaths","a_Options"})["ArrayView<String>()"]["Options()"]
            .method<BuildSession const&(BuildSessionId) const>("getBuildSession", &_::getBuildSession)({"a_Id"})
            .method<Modules(BuildSessionId) const>("getBuildSessionModules", &_::getBuildSessionModules)({"a_Id"})
            .method<void(StringView)>("cleanupProject", &_::cleanupProject)({"_path"})
            .method<void(StringView)>("cleanBuild", &_::cleanBuild)({"_path"})
            .method<bool() const>("isBuildOutdated", &_::isBuildOutdated)
            .method<void()>("abortBuild", &_::abortBuild)
            .method<void(SourceFile*) const>("outdate", &_::outdate)({"a_pSource"})
            .method<BuildSources(StringView)>("listProjectBuildSourcesAt", &_::listProjectBuildSourcesAt)({"_path"})
            .method<BuildSources(BuildSessionId, StringView)>("listProjectBuildSources", &_::listProjectBuildSources)({"_sessionId","_project"})
            .method<String(BuildSessionId, StringView) const>("findProjectPath", &_::findProjectPath)({"_sessionId","_project"})
            .method<BuildSource*(SourceStream*) const>("getCompiledSource", &_::getCompiledSource)({"a_pSourceStream"})
            .method<BuildSource*(Source*) const>("getCompiledSource", &_::getCompiledSource)({"a_pSource"})
            .method<CodeGenerator*(Module*) const>("getCodeGenerator", &_::getCodeGenerator)({"a_pModule"})
            .method<DebugInformation*(Module*) const>("getDebugInformation", &_::getDebugInformation)({"a_pModule"})
            .method<Language*(StringView) const>("getLanguage", &_::getLanguage)({"a_strLanguageName"})
            .method<Languages const&() const>("getLanguages", &_::getLanguages)
            .method<Language*(StringView) const>("getLanguageFromFileExtension", &_::getLanguageFromFileExtension)({"a_Extension"})
            .method<Language*(Source*) const>("findSourceLanguage", &_::findSourceLanguage)({"a_pSource"})
            .method<Language*(SourceStream*) const>("findSourceStreamLanguage", &_::findSourceStreamLanguage)({"a_pStream"})
            .method<::phantom::lang::Language *() const>("getDefaultLanguage", &_::getDefaultLanguage)
            .method<void(Language*)>("setDefaultLanguage", &_::setDefaultLanguage)({"a_pLanguage"})
            .method<void(Language*)>("addLanguage", &_::addLanguage)({"a_pLanguage"})
            .method<void(Language*)>("removeLanguage", &_::removeLanguage)({"a_pLanguage"})
            .method<void(BuildSessionId)>("restoreBuildSession", &_::restoreBuildSession)({"a_Id"})
            .method<void(Module*, Options, Message*)>("codeGen", &_::codeGen)({"a_pModule","a_Options","a_pMsg"})["{}"]["nullptr"]
        
        .public_()
            .field("languageAdded", &_::languageAdded)
            .field("languageAboutToBeRemoved", &_::languageAboutToBeRemoved)
            .field("sourceBuilt", &_::sourceBuilt)
            .field("sourceBuildStarted", &_::sourceBuildStarted)
            .field("buildSessionUserHook", &_::buildSessionUserHook)
            .field("buildSessionStarting", &_::buildSessionStarting)
            .field("buildSessionSucceeding", &_::buildSessionSucceeding)
            .field("buildSessionDone", &_::buildSessionDone)
            .field("buildSessionFailed", &_::buildSessionFailed)
            .field("buildSessionSucceeded", &_::buildSessionSucceeded)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BuildSystem")
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
