#pragma once

// haunt {

// clang-format off

#include "VariableExpression.h"

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
    PHANTOM_SOURCE("VariableExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(VariableExpression)
        {
            this_()
            .inherits<::phantom::lang::LValueExpression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Variable*)>()({"a_pVariable"})
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::VariableExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<Variable*() const>("getVariable", &_::getVariable)
        
        .protected_()
            .method<void*(ExecutionContext&) const, virtual_>("internalEvalAddress", &_::internalEvalAddress)({"a_Context"})
        
        .protected_()
            .field("m_pVariable", &_::m_pVariable)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("VariableExpression")
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
