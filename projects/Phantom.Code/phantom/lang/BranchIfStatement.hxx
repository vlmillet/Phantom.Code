#pragma once

// haunt {

// clang-format off

#include "BranchIfStatement.h"

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
    PHANTOM_SOURCE("BranchIfStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(BranchIfStatement)
        {
            this_()
            .inherits<::phantom::lang::BranchStatement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Expression*, Label*)>()
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::BranchIfStatement *() const, virtual_>("asBranchIfStatement", &_::asBranchIfStatement)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)
            .method<Expression*() const>("getExpression", &_::getExpression)
        
        .protected_()
            .field("m_pExpression", &_::m_pExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BranchIfStatement")
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
