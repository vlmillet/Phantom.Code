#pragma once

// haunt {

// clang-format off

#include "TemplateParameterPackExpressionExpansion.h"

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
    PHANTOM_SOURCE("TemplateParameterPackExpressionExpansion")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateParameterPackExpressionExpansion)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(TemplateParameterPackExpansion*)>()({"a_pExpansion"})
            .method<TemplateParameterPackExpansion*() const>("getExpansion", &_::getExpansion)
            .method<Placeholder*() const>("getPackArgument", &_::getPackArgument)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)({""})
        
        .protected_()
            .field("m_pExpansion", &_::m_pExpansion)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateParameterPackExpressionExpansion")
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
