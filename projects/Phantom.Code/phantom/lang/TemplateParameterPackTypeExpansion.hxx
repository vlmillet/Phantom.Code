#pragma once

// haunt {

// clang-format off

#include "TemplateParameterPackTypeExpansion.h"

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
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/friend>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("TemplateParameterPackTypeExpansion")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateParameterPackTypeExpansion)
        {
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Type>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            /// missing symbol(s) reflection (phantom::lang::TemplateParameterPackExpansion) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(TemplateParameterPackExpansion*)>()({"a_pExpansion"})
            /// missing symbol(s) reflection (phantom::lang::TemplateParameterPackExpansion) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<TemplateParameterPackExpansion*() const>("getExpansion", &_::getExpansion)
            .method<PlaceholderType*() const>("getPackArgument", &_::getPackArgument)
        
        .protected_()
            /// missing symbol(s) reflection (phantom::lang::TemplateParameterPackExpansion) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("m_pExpansion", &_::m_pExpansion)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateParameterPackTypeExpansion")
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
