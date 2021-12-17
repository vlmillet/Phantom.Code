#pragma once

// haunt {

// clang-format off

#include "AllocateExpression.h"

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
    PHANTOM_SOURCE("AllocateExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(AllocateExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Type*, bool)>()({"a_pType","a_bOnStack"})["false"]
            .constructor<void(Type*, Expression*, bool)>()({"a_pType","a_pSizeExpression","a_bOnStack"})["false"]
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::AllocateExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<Expression*() const>("getSizeExpression", &_::getSizeExpression)
            .method<Type*() const>("getType", &_::getType)
            .method<bool() const>("isOnStack", &_::isOnStack)
        
        .protected_()
            .field("m_pType", &_::m_pType)
            .field("m_pSizeExpression", &_::m_pSizeExpression)
            .field("m_bStack", &_::m_bStack)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("AllocateExpression")
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
