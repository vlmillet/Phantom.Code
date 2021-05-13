#pragma once

// haunt {

// clang-format off

#include "NewExpression.h"

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
    PHANTOM_SOURCE("NewExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(NewExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Expression*, Expression*)>()({"a_pInitializationExpression","a_pCountExpression"})["nullptr"]
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::NewExpression *() const, virtual_>("asNewExpression", &_::asNewExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::NewExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<Expression*() const>("getInitializationExpression", &_::getInitializationExpression)
            .method<Expression*() const>("getCountExpression", &_::getCountExpression)
        
        .protected_()
            .field("m_pInitializationExpression", &_::m_pInitializationExpression)
            .field("m_pCountExpression", &_::m_pCountExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("NewExpression")
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
