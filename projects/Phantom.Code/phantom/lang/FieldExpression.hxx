#pragma once

// haunt {

// clang-format off

#include "FieldExpression.h"

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
    PHANTOM_SOURCE("FieldExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(FieldExpression)
        {
            this_()
            .inherits<::phantom::lang::LValueExpression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Expression*, Field*)>()({"a_pObjectExpression","a_pField"})
            .method<void()>("initialize", &_::initialize)
            .method<Expression*() const>("getObjectExpression", &_::getObjectExpression)
            .method<Field*() const>("getField", &_::getField)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)({"a_Context"})
            .method<::phantom::lang::LanguageElement *() const, virtual_|override_>("removeExpression", &_::removeExpression)
            .method<bool() const, virtual_>("isPersistent", &_::isPersistent)
            .method<::phantom::lang::FieldExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<void*(ExecutionContext&) const, virtual_>("internalEvalAddress", &_::internalEvalAddress)({"a_Context"})
        
        .protected_()
            .field("m_pObjectExpression", &_::m_pObjectExpression)
            .field("m_pField", &_::m_pField)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FieldExpression")
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
