#pragma once

// haunt {

// clang-format off

#include "BranchIfNotStatement.h"

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
    PHANTOM_SOURCE("BranchIfNotStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(BranchIfNotStatement)
        {
            this_()
            .inherits<::phantom::lang::BranchStatement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Expression*, Label*)>()({"a_pExpression","a_pLabel"})
            .method<void()>("initialize", &_::initialize)
            .method<BranchIfNotStatement*() const, virtual_|override_>("asBranchIfNotStatement", &_::asBranchIfNotStatement)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)({"a_Context"})
            .method<void(ExecutionContext&) const, virtual_|override_>("flush", &_::flush)({"a_Context"})
            .method<Expression*() const>("getExpression", &_::getExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BranchIfNotStatement")
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
