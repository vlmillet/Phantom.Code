#pragma once

// haunt {

// clang-format off

#include "LValueInitializationStatement.h"

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
    PHANTOM_SOURCE("LValueInitializationStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(LValueInitializationStatement)
        {
            this_()
            .inherits<::phantom::lang::Statement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Type*, Expression*)>()({"a_pValueType","a_pInitializationExpression"})
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::LValueInitializationStatement *() const, virtual_>("asLValueInitializationStatement", &_::asLValueInitializationStatement)
            .method<Expression*() const>("getInitializationExpression", &_::getInitializationExpression)
            .method<Evaluable*(Expression*) const, virtual_|override_>("evaluateExpressionLifeTime", &_::evaluateExpressionLifeTime)({"a_pExpression"})
            .method<Type*() const>("getValueType", &_::getValueType)
        
        .protected_()
            .field("m_pValueType", &_::m_pValueType)
            .field("m_pInitializationExpression", &_::m_pInitializationExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LValueInitializationStatement")
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
