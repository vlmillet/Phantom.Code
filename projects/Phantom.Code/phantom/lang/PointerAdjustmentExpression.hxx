#pragma once

// haunt {

// clang-format off

#include "PointerAdjustmentExpression.h"

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
    PHANTOM_SOURCE("PointerAdjustmentExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(PointerAdjustmentExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Type*, Expression*, ptrdiff_t)>()({"a_pOutputRefOrPtrType","a_pPointerExpression","a_iOffset"})
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::PointerAdjustmentExpression *() const, virtual_>("asPointerAdjustmentExpression", &_::asPointerAdjustmentExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)({"a_Context"})
            .method<Expression*() const>("getPointerExpression", &_::getPointerExpression)
            .method<ptrdiff_t() const>("getOffset", &_::getOffset)
            .method<::phantom::lang::PointerAdjustmentExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
        
        .protected_()
            .field("m_pPointerExpression", &_::m_pPointerExpression)
            .field("m_iOffset", &_::m_iOffset)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PointerAdjustmentExpression")
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
