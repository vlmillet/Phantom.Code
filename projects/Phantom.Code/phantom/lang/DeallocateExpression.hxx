#pragma once

// haunt {

// clang-format off

#include "DeallocateExpression.h"

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
    PHANTOM_SOURCE("DeallocateExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(DeallocateExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Type*, Expression*, Expression*)>()({"a_pType","a_pSizeExpression","a_pAddressExpression"})
            .constructor<void(Type*, Expression*)>()({"a_pType","a_pAddressExpression"})
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::DeallocateExpression *() const, virtual_>("asDeallocateExpression", &_::asDeallocateExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::DeallocateExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<Expression*() const>("getAddressExpression", &_::getAddressExpression)
            .method<Expression*() const>("getSizeExpression", &_::getSizeExpression)
            .method<Type*() const>("getType", &_::getType)
        
        .protected_()
            .field("m_pType", &_::m_pType)
            .field("m_pSizeExpression", &_::m_pSizeExpression)
            .field("m_pAddressExpression", &_::m_pAddressExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("DeallocateExpression")
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
