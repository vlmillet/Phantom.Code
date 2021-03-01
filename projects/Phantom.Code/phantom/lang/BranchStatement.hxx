#pragma once

// haunt {

// clang-format off

#include "BranchStatement.h"

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
    PHANTOM_SOURCE("BranchStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(BranchStatement)
        {
            this_()
            .inherits<::phantom::lang::ControlStatement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .method<::phantom::lang::BranchStatement *() const, virtual_>("asBranchStatement", &_::asBranchStatement)
            .constructor<void(Label*)>()
            .method<Label*() const>("getLabel", &_::getLabel)
        
        .protected_()
            .method<Statement*() const, virtual_|override_>("getTargetStatement", &_::getTargetStatement)
            .method<void(Block*), virtual_|override_>("onAttachedToBlock", &_::onAttachedToBlock)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BranchStatement")
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
