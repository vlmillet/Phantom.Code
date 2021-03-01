#pragma once

// haunt {

// clang-format off

#include "Statement.h"

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
    PHANTOM_SOURCE("Statement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Statement)
        {
            this_()
            .inherits<::phantom::lang::Evaluable>()
        
        .public_()
        
        .public_()
            .constructor<void(uint)>()["0"]
            .method<void(ExecutionContext&) const, pure_virtual>("eval", &_::eval)
            .method<::phantom::lang::Statement *() const, virtual_>("asStatement", &_::asStatement)
            .method<Block*() const>("getBlock", &_::getBlock)
            .method<Block*() const>("getRootBlock", &_::getRootBlock)
            .method<Subroutine*() const>("getSubroutine", &_::getSubroutine)
            .method<Statement*() const>("getNextStatement", &_::getNextStatement)
            .method<Evaluable*(Expression*) const, virtual_|override_>("evaluateExpressionLifeTime", &_::evaluateExpressionLifeTime)
            .method<::phantom::lang::LocalVariableInitializationStatement *() const, virtual_>("asLocalVariableInitializationStatement", &_::asLocalVariableInitializationStatement)
            .method<::phantom::lang::BranchIfNotStatement *() const, virtual_>("asBranchIfNotStatement", &_::asBranchIfNotStatement)
            .method<::phantom::lang::BranchIfStatement *() const, virtual_>("asBranchIfStatement", &_::asBranchIfStatement)
            .method<::phantom::lang::BranchStatement *() const, virtual_>("asBranchStatement", &_::asBranchStatement)
            .method<::phantom::lang::ReturnStatement *() const, virtual_>("asReturnStatement", &_::asReturnStatement)
            .method<::phantom::lang::ExpressionStatement *() const, virtual_>("asExpressionStatement", &_::asExpressionStatement)
            .method<::phantom::lang::FieldInitializationStatement *() const, virtual_>("asFieldInitializationStatement", &_::asFieldInitializationStatement)
            .method<::phantom::lang::BaseConstructorCallStatement *() const, virtual_>("asBaseConstructorCallStatement", &_::asBaseConstructorCallStatement)
            .method<::phantom::lang::Label *() const, virtual_>("asLabel", &_::asLabel)
            .method<Statement*() const>("getNextNonBlockStatement", &_::getNextNonBlockStatement)
        
        .protected_()
            .method<void(Block*), virtual_>("onAttachedToBlock", &_::onAttachedToBlock)
            .method<void(), virtual_>("restore", &_::restore)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Statement")
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
