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
            .constructor<void(Expression*, Parameter*)>()({"a_pExpandedExp","a_pExpandedParam"})
            .method<Expression*() const>("getExpandedExpression", &_::getExpandedExpression)
            .method<Parameter*() const>("getExpandedParameter", &_::getExpandedParameter)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)({""})
            .method<void(ExecutionContext&) const, virtual_|override_>("flush", &_::flush)({"a_Context"})
            .method<bool() const, virtual_|override_>("hasStorage", &_::hasStorage)
        
        .protected_()
            .field("m_pExpandedExpression", &_::m_pExpandedExpression)
            .field("m_pExpandedParameter", &_::m_pExpandedParameter)
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
