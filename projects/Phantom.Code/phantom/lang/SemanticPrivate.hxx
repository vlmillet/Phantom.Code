#pragma once

// haunt {

// clang-format off

#include "SemanticPrivate.h"

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
#include <phantom/function>
#include <phantom/variable>
#include <phantom/method>
#include <phantom/constructor>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("SemanticPrivate")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateSubstitution)
        {
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            this_()
        .public_()
            .constructor<void()>()
            .constructor<void(TemplateSpecialization*)>()
            .method<void(Placeholder*, LanguageElement*)>("insert", &_::insert)
            .method<LanguageElement*(size_t) const>("getArgument", &_::getArgument)
            .method<LanguageElements const&() const>("getArguments", &_::getArguments)
            .method<LanguageElement*(Placeholder*) const>("getArgument", &_::getArgument)
            .method<void(TemplateSpecialization*)>("setInstantiation", &_::setInstantiation)
            .method<TemplateSpecialization*() const>("getInstantiation", &_::getInstantiation)
            .method<size_t() const>("size", &_::size)
            ;
        }
        PHANTOM_REGISTER(Variables) { this_().variable("_canBe", &_canBe); }
        PHANTOM_REGISTER(Variables) { this_().variable("_or", &_or); }
        /// missing symbol(s) reflection (va_list) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Functions) { this_().function<String(const char*, va_list)>("stringf", stringf);}
        PHANTOM_REGISTER(Functions) { this_().function<StringView(Access)>("getAccessString", getAccessString);}
        PHANTOM_CLASS(SemanticPrivate)
        {
            this_()
        .public_()
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SemanticPrivate")
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
