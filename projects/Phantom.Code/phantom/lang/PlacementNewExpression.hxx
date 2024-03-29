#pragma once

// haunt {

// clang-format off

#include "PlacementNewExpression.h"

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
    PHANTOM_SOURCE("PlacementNewExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(PlacementNewExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Expression*, Expression*)>()({"a_pPlaceExpresion","a_pConstructionExpression"})
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::PlacementNewExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<Expression*() const>("getPlaceExpression", &_::getPlaceExpression)
            .method<Expression*() const>("getConstructionExpression", &_::getConstructionExpression)
        
        .protected_()
            .field("m_pPlaceExpression", &_::m_pPlaceExpression)
            .field("m_pConstructionExpression", &_::m_pConstructionExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PlacementNewExpression")
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
