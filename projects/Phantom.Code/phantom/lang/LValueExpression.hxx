#pragma once

// haunt {

// clang-format off

#include "LValueExpression.h"

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
    PHANTOM_SOURCE("LValueExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(LValueExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Type*, uint)>()["0"]
            .constructor<void(Type*, Type*, uint)>()["0"]
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<Type*() const>("getRValueType", &_::getRValueType)
        
        .protected_()
            .method<void*(ExecutionContext&) const, pure_virtual>("internalEvalAddress", &_::internalEvalAddress)
        
        .protected_()
            .field("m_pRValueType", &_::m_pRValueType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LValueExpression")
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
