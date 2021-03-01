#pragma once

// haunt {

// clang-format off

#include "ExpressionStatement.h"

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
    PHANTOM_SOURCE("ExpressionStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ExpressionStatement)
        {
            this_()
            .inherits<::phantom::lang::Statement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Expression*)>()
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::ExpressionStatement *() const, virtual_>("asExpressionStatement", &_::asExpressionStatement)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)
            .method<Expression*() const>("getExpression", &_::getExpression)
        
        .protected_()
            .field("m_pExpression", &_::m_pExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ExpressionStatement")
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
