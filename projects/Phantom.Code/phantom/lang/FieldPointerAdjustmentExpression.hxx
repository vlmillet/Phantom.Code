#pragma once

// haunt {

// clang-format off

#include "FieldPointerAdjustmentExpression.h"

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
    PHANTOM_SOURCE("FieldPointerAdjustmentExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(FieldPointerAdjustmentExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Type*, Expression*, ptrdiff_t)>()
            .method<::phantom::lang::FieldPointerAdjustmentExpression *() const, virtual_>("asFieldPointerAdjustmentExpression", &_::asFieldPointerAdjustmentExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)
            .method<Expression*() const>("getPointerExpression", &_::getPointerExpression)
            .method<ptrdiff_t() const>("getOffset", &_::getOffset)
            .method<::phantom::lang::FieldPointerAdjustmentExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
        
        .protected_()
            .field("m_pPointerExpression", &_::m_pPointerExpression)
            .field("m_iOffset", &_::m_iOffset)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FieldPointerAdjustmentExpression")
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
