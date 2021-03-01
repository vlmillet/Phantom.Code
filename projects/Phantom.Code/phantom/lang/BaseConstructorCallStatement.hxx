#pragma once

// haunt {

// clang-format off

#include "BaseConstructorCallStatement.h"

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
    PHANTOM_SOURCE("BaseConstructorCallStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(BaseConstructorCallStatement)
        {
            this_()
            .inherits<::phantom::lang::Statement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Class*, Expression*)>()
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::BaseConstructorCallStatement *() const, virtual_>("asBaseConstructorCallStatement", &_::asBaseConstructorCallStatement)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)
            .method<Expression*() const>("getConstructorCallExpression", &_::getConstructorCallExpression)
            .method<Class*() const>("getBaseClass", &_::getBaseClass)
            .method<Class*() const>("getDerivedClass", &_::getDerivedClass)
        
        .protected_()
            .field("m_pConstructorCallExpression", &_::m_pConstructorCallExpression)
            .field("m_pDerivedClass", &_::m_pDerivedClass)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BaseConstructorCallStatement")
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
