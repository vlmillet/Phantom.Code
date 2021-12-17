#pragma once

// haunt {

// clang-format off

#include "FieldPointerExpression.h"

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
    PHANTOM_SOURCE("FieldPointerExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(FieldPointerExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Expression*, Expression*)>()({"a_pObjectExpression","a_pFieldPointerExpression"})
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::FieldPointerExpression *() const, virtual_>("asFieldPointerExpression", &_::asFieldPointerExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<Expression*() const>("getObjectExpression", &_::getObjectExpression)
            .method<Expression*() const>("getFieldPointerExpression", &_::getFieldPointerExpression)
            .method<::phantom::lang::FieldPointerExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
        
        .protected_()
            .field("m_pObjectExpression", &_::m_pObjectExpression)
            .field("m_pFieldPointerExpression", &_::m_pFieldPointerExpression)
            .field("m_pFieldPointer", &_::m_pFieldPointer)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FieldPointerExpression")
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
