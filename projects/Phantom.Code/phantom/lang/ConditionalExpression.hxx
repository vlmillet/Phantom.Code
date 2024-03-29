#pragma once

// haunt {

// clang-format off

#include "ConditionalExpression.h"

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
    PHANTOM_SOURCE("ConditionalExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ConditionalExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Expression*, Expression*, Expression*)>()({"a_pTestExpression","a_pThenExpression","a_pElseExpression"})
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::ConditionalExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<Expression*() const>("getConditionExpression", &_::getConditionExpression)
            .method<Expression*() const>("getThenExpression", &_::getThenExpression)
            .method<Expression*() const>("getElseExpression", &_::getElseExpression)
        
        .protected_()
            .field("m_pTestExpression", &_::m_pTestExpression)
            .field("m_pThenExpression", &_::m_pThenExpression)
            .field("m_pElseExpression", &_::m_pElseExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ConditionalExpression")
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
