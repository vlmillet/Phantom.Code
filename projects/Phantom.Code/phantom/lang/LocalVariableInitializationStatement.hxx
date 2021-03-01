#pragma once

// haunt {

// clang-format off

#include "LocalVariableInitializationStatement.h"

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
    PHANTOM_SOURCE("LocalVariableInitializationStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(LocalVariableInitializationStatement)
        {
            this_()
            .inherits<::phantom::lang::Statement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(LocalVariable*, Expression*)>()
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::LocalVariableInitializationStatement *() const, virtual_>("asLocalVariableInitializationStatement", &_::asLocalVariableInitializationStatement)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)
            .method<LocalVariable*() const>("getLocalVariable", &_::getLocalVariable)
            .method<Expression*() const>("getInitializationExpression", &_::getInitializationExpression)
            .method<Evaluable*(Expression*) const, virtual_|override_>("evaluateExpressionLifeTime", &_::evaluateExpressionLifeTime)
            .method<void(Block*), virtual_|override_>("onAttachedToBlock", &_::onAttachedToBlock)
            .method<bool() const>("isTemporaryContainer", &_::isTemporaryContainer)
        
        .protected_()
            .field("m_pLocalVariable", &_::m_pLocalVariable)
            .field("m_pInitializationExpression", &_::m_pInitializationExpression)
            .field("m_temporaryContainer", &_::m_temporaryContainer)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LocalVariableInitializationStatement")
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
