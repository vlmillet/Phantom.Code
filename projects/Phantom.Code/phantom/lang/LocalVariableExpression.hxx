#pragma once

// haunt {

// clang-format off

#include "LocalVariableExpression.h"

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
    PHANTOM_SOURCE("LocalVariableExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(LocalVariableExpression)
        {
            this_()
            .inherits<::phantom::lang::LValueExpression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Type*)>()({"a_pThisType"})
            .constructor<void(LocalVariable*)>()({"a_pLocalVariable"})
            .method<void()>("initialize", &_::initialize)
            .method<LocalVariable*() const>("getLocalVariable", &_::getLocalVariable)
            .method<::phantom::lang::LocalVariableExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<bool() const, virtual_|override_>("isPersistent", &_::isPersistent)
            .method<::phantom::lang::LanguageElement *() const, virtual_|override_>("removeExpression", &_::removeExpression)
        
        .protected_()
            .method<void*(ExecutionContext&) const, virtual_>("internalEvalAddress", &_::internalEvalAddress)({"a_Context"})
        
        .protected_()
            .field("m_pLocalVariable", &_::m_pLocalVariable)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LocalVariableExpression")
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
