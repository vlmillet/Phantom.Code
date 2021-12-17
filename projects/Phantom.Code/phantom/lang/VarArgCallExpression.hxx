#pragma once

// haunt {

// clang-format off

#include "VarArgCallExpression.h"

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
    PHANTOM_SOURCE("VarArgCallExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(VarArgCallExpression)
        {
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            this_()
            .inherits<::phantom::lang::CallExpression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Subroutine*, ExpressionsView)>()({"a_pSubroutine","a_Arguments"})
            .constructor<void(Subroutine*, Expression*)>()({"a_pSubroutine","a_pArgument"})
            .constructor<void(Subroutine*)>()({"a_pSubroutine"})
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)({"a_Context"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("VarArgCallExpression")
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
