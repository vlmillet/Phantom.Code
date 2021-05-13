#pragma once

// haunt {

// clang-format off

#include "PropertyExpression.h"

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
#include <phantom/friend>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("PropertyExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(PropertyExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Expression*, Property*)>()({"a_pObjectExpression","a_pProperty"})
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<Expression*() const>("getObjectExpression", &_::getObjectExpression)
            .method<Property*() const>("getProperty", &_::getProperty)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)({"a_Context"})
            .method<bool() const, virtual_>("isPersistent", &_::isPersistent)
            .method<::phantom::lang::PropertyExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<::phantom::lang::LanguageElement *() const, virtual_|override_>("removeExpression", &_::removeExpression)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)({"a_Context"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PropertyExpression")
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
