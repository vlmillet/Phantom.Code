#pragma once

// haunt {

// clang-format off

#include "BuildSource.h"

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
    PHANTOM_SOURCE("BuildSource")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(BuildSource)
        {
            using Options = typedef_< phantom::lang::Options>;
            using Session = typedef_<_::Session>;
            this_()
        
        .public_()
            .staticMethod<::phantom::lang::BuildSource *(Source*)>("Get", &_::Get)({"a_pSource"})
            .struct_<Session>()
                .constructor<void()>()
                .method<BuildSource*() const>("getBuildSource", &_::Session::getBuildSource)
                .method<Source*() const>("getSource", &_::Session::getSource)
                .method<Package*() const>("getPackage", &_::Session::getPackage)
                .method<Parser*() const>("getParser", &_::Session::getParser)
                .method<Semantic*() const>("getSemantic", &_::Session::getSemantic)
                .method<Language*() const>("getLanguage", &_::Session::getLanguage)
                .method<Message*() const>("getStatusMessage", &_::Session::getStatusMessage)
                .method<bool() const>("isNull", &_::Session::isNull)
                .method<bool() const>("isEmpty", &_::Session::isEmpty)
                .method<void()>("undo", &_::Session::undo)
                .method<void()>("redo", &_::Session::redo)
                .method<void(BuildSource*)>("addDependency", &_::Session::addDependency)({"a_pDep"})
                .method<bool(BuildSource*) const>("hasDependency", &_::Session::hasDependency)({"a_pDep"})
                .method<bool() const>("hasError", &_::Session::hasError)
                .method<bool() const>("hasSucceeded", &_::Session::hasSucceeded)
                /// missing symbol(s) reflection (time_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
                // .method<time_t() const>("getTime", &_::Session::getTime)
            .end()
            .staticMethod<const ::phantom::lang::BuildSource::Session &()>("EmptyBuild", &_::EmptyBuild)
            .constructor<void(BuildSystem*, SourceStream*)>()({"a_pCompiler","a_pSourceStream"})
            .method<Language*() const>("getLanguage", &_::getLanguage)
            .method<SourceStream*() const>("getSourceStream", &_::getSourceStream)
            .method<Source*() const>("getSource", &_::getSource)
            .method<bool() const>("hasEverSucceeded", &_::hasEverSucceeded)
            .method<void(LanguageElement*, Symbol*)>("ensureStrongDependency", &_::ensureStrongDependency)({"",""})
            .method<bool(BuildSource*) const>("hasBuildDependency", &_::hasBuildDependency)({"a_pSource"})
            .method<void(BuildSource*)>("addBuildDependency", &_::addBuildDependency)({"a_pSource"})
            .method<void(Source*)>("addBuildDependency", &_::addBuildDependency)({"a_pSource"})
            .method<SmallVector<SmallSet<BuildSource*> > const&() const>("getBuildDependencies", &_::getBuildDependencies)
        
        .public_()
            .method<bool() const>("hasSucceeded", &_::hasSucceeded)
            /// missing symbol(s) reflection (time_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<time_t()>("outdate", &_::outdate)
            .method<bool() const>("hasError", &_::hasError)
            .method<const ::phantom::lang::BuildSource::Session &(size_t) const>("getBuild", &_::getBuild)({"a_uiIndex"})
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
            .method<Session const&() const>("getCurrentBuild", &_::getCurrentBuild)
            .method<Session const&() const>("getLastBuild", &_::getLastBuild)
            .method<Session const&() const>("getPreviousBuild", &_::getPreviousBuild)
            .method<void()>("dumpMessages", &_::dumpMessages)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BuildSource")
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
