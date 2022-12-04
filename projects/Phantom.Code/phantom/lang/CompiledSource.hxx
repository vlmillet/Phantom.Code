#pragma once

// haunt {

// clang-format off

#include "CompiledSource.h"

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
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallSet.hxx>
#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("CompiledSource")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(CompiledSource)
        {
            using Build = typedef_<_::Build>;
            using Options = typedef_< phantom::lang::Options>;
            this_()
        
        .public_()
            .staticMethod<::phantom::lang::CompiledSource *(Source*)>("Get", &_::Get)({"a_pSource"})
            .struct_<Build>()
                .constructor<void()>()
                .method<Source*() const>("getSource", &_::Build::getSource)
                .method<Parser*() const>("getParser", &_::Build::getParser)
                .method<Semantic*() const>("getSemantic", &_::Build::getSemantic)
                .method<Language*() const>("getLanguage", &_::Build::getLanguage)
                .method<Message*() const>("getStatusMessage", &_::Build::getStatusMessage)
                .method<bool() const>("isNull", &_::Build::isNull)
                .method<void()>("undo", &_::Build::undo)
                .method<void()>("redo", &_::Build::redo)
                .method<void(CompiledSource*)>("addDependency", &_::Build::addDependency)({"a_pDep"})
                .method<bool(CompiledSource*) const>("hasDependency", &_::Build::hasDependency)({"a_pDep"})
                .method<bool() const>("hasError", &_::Build::hasError)
                .method<bool() const>("hasSucceeded", &_::Build::hasSucceeded)
                /// missing symbol(s) reflection (time_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
                // .method<time_t() const>("getTime", &_::Build::getTime)
            .end()
            .staticMethod<const ::phantom::lang::CompiledSource::Build &()>("EmptyBuild", &_::EmptyBuild)
            .constructor<void(Compiler*, SourceStream*)>()({"a_pCompiler","a_pSourceStream"})
            .method<Language*() const>("getLanguage", &_::getLanguage)
            .method<SourceStream*() const>("getSourceStream", &_::getSourceStream)
            .method<Source*() const>("getSource", &_::getSource)
            .method<void(LanguageElement*, Symbol*)>("ensureStrongDependency", &_::ensureStrongDependency)({"",""})
            .method<bool(CompiledSource*) const>("hasBuildDependency", &_::hasBuildDependency)({"a_pSource"})
            .method<void(CompiledSource*)>("addBuildDependency", &_::addBuildDependency)({"a_pSource"})
            .method<void(Source*)>("addBuildDependency", &_::addBuildDependency)({"a_pSource"})
            .method<SmallVector<SmallSet<CompiledSource*> > const&() const>("getBuildDependencies", &_::getBuildDependencies)
        
        .public_()
            .method<bool() const>("hasSucceeded", &_::hasSucceeded)
            /// missing symbol(s) reflection (time_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<time_t()>("outdate", &_::outdate)
            .method<bool() const>("hasError", &_::hasError)
            .method<const ::phantom::lang::CompiledSource::Build &(size_t) const>("getBuild", &_::getBuild)({"a_uiIndex"})
            .method<size_t() const>("getBuildCount", &_::getBuildCount)
            /// missing symbol(s) reflection (time_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<time_t() const>("getLastValidChangeTime", &_::getLastValidChangeTime)
            /// missing symbol(s) reflection (time_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(time_t)>("setLastValidChangeTime", &_::setLastValidChangeTime)({"a_Time"})
            .method<Message*() const>("getStatusMessage", &_::getStatusMessage)
            .method<int() const>("getPriority", &_::getPriority)
            .method<void(int)>("setPriority", &_::setPriority)({"a_iPriority"})
            .method<bool() const>("isOutdated", &_::isOutdated)
            .method<Parser*() const>("getParser", &_::getParser)
            .method<Semantic*() const>("getSemantic", &_::getSemantic)
            .method<void(Options const&)>("setOptions", &_::setOptions)({"a_Options"})
            .method<Options const&() const>("getOptions", &_::getOptions)
            .method<Build const&() const>("getCurrentBuild", &_::getCurrentBuild)
            .method<Build const&() const>("getLastBuild", &_::getLastBuild)
            .method<Build const&() const>("getPreviousBuild", &_::getPreviousBuild)
            .method<void()>("dumpMessages", &_::dumpMessages)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CompiledSource")
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
