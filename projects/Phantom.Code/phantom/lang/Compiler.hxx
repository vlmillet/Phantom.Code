#pragma once

// haunt {

// clang-format off

#include "Compiler.h"

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
    PHANTOM_SOURCE("Compiler")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Compiler)
        {
            using CompiledSources = typedef_< phantom::lang::CompiledSources>;
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
            .staticMethod<::phantom::lang::Compiler *()>("Get", &_::Get)
            .constructor<void()>()
            .method<Debugger*() const>("getDebugger", &_::getDebugger)
            .method<BuildSessionId(ArrayView<String>, Options const&)>("newSession", &_::newSession)
            .method<void(BuildSessionId, StringView)>("buildProject", &_::buildProject)
            .method<void(BuildSessionId, StringView)>("buildProjectAt", &_::buildProjectAt)
            .method<CompiledSource* (BuildSessionId, StringView, ArrayView<String>, Options const&)>("buildSource", &_::buildSource)["ArrayView<String>()"]["Options()"]
            .method<BuildSession const&(BuildSessionId) const>("getBuildSession", &_::getBuildSession)
            .method<Modules(BuildSessionId) const>("getBuildSessionModules", &_::getBuildSessionModules)
            .method<void(StringView)>("cleanupProject", &_::cleanupProject)
            .method<void(StringView)>("cleanBuild", &_::cleanBuild)
            .method<bool() const>("isBuildOutdated", &_::isBuildOutdated)
            .method<void()>("abortBuild", &_::abortBuild)
            .method<void(SourceFile*) const>("outdate", &_::outdate)
            .method<CompiledSources(StringView)>("listProjectCompiledSources", &_::listProjectCompiledSources)
            .method<CompiledSource*(SourceStream*) const>("getCompiledSource", &_::getCompiledSource)
            .method<CompiledSource*(Source*) const>("getCompiledSource", &_::getCompiledSource)
            .method<CodeGenerator*(Module*) const>("getCodeGenerator", &_::getCodeGenerator)
            .method<DebugInformation*(Module*) const>("getDebugInformation", &_::getDebugInformation)
            .method<Language*(StringView) const>("getLanguage", &_::getLanguage)
            .method<Languages const&() const>("getLanguages", &_::getLanguages)
            .method<Language*(StringView) const>("getLanguageFromFileExtension", &_::getLanguageFromFileExtension)
            .method<Language*(Source*) const>("findSourceLanguage", &_::findSourceLanguage)
            .method<Language*(SourceStream*) const>("findSourceStreamLanguage", &_::findSourceStreamLanguage)
            .method<::phantom::lang::Language *() const>("getDefaultLanguage", &_::getDefaultLanguage)
            .method<void(Language*)>("setDefaultLanguage", &_::setDefaultLanguage)
            .method<void(Language*)>("addLanguage", &_::addLanguage)
            .method<void(Language*)>("removeLanguage", &_::removeLanguage)
            .method<void(BuildSessionId)>("restoreBuildSession", &_::restoreBuildSession)
            .method<void(Module*, Options, Message*)>("codeGen", &_::codeGen)["{}"]["nullptr"]
        
        .public_()
            .field("languageAdded", &_::languageAdded)
            .field("languageAboutToBeRemoved", &_::languageAboutToBeRemoved)
            .field("sourceBuilt", &_::sourceBuilt)
            .field("sourceBuildStarted", &_::sourceBuildStarted)
            .field("buildSessionUserHook", &_::buildSessionUserHook)
            .field("buildSessionDone", &_::buildSessionDone)
            .field("buildSessionApplied", &_::buildSessionApplied)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Compiler")
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
