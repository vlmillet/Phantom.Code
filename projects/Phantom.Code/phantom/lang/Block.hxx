#pragma once

// haunt {

// clang-format off

#include "Block.h"

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
#include <phantom/enum>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Block")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Block)
        {
            using ETag = typedef_<_::ETag>;
            using Expressions = typedef_< phantom::lang::Expressions>;
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            using LocalVariables = typedef_< phantom::lang::LocalVariables>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using Statements = typedef_< phantom::lang::Statements>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Statement, ::phantom::lang::Scope>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .enum_<ETag>().values({
                {"e_Tag_None",_::e_Tag_None},
                {"e_Tag_If",_::e_Tag_If},
                {"e_Tag_For",_::e_Tag_For},
                {"e_Tag_While",_::e_Tag_While},
                {"e_Tag_Switch",_::e_Tag_Switch},
                {"e_Tag_Do",_::e_Tag_Do},
                {"e_Tag_Foreach",_::e_Tag_Foreach},
                {"e_Tag_Assert",_::e_Tag_Assert}})
            .end()
            .constructor<void(int)>()["e_Tag_None"]
            .method<Block*(int)>("newBlock", &_::newBlock)["e_Tag_None"]
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::Scope *() const, virtual_>("asScope", &_::asScope)
            .method<::phantom::lang::Block *() const, virtual_>("asBlock", &_::asBlock)
            .method<int() const>("getTag", &_::getTag)
            .method<LocalVariable*(Type*, StringView, Expression*, Modifiers, uint)>("addLocalVariable", &_::addLocalVariable)["nullptr"]["0"]["0"]
            .method<void(LocalVariable*)>("addLocalVariable", &_::addLocalVariable)
            .method<LocalVariable*(StringView, Language*)>("addLocalVariable", &_::addLocalVariable)["nullptr"]
            .method<void(Statement*)>("addStatement", &_::addStatement)
            .method<void(Statement*, Statement*)>("insertStatementAfter", &_::insertStatementAfter)
            .method<ExpressionStatement*(Expression*)>("addStatement", &_::addStatement)
            .method<ExpressionStatement*(StringView, Language*)>("addStatement", &_::addStatement)
            .method<ExpressionStatement*(StringView)>("addStatement", &_::addStatement)
            .method<ReturnStatement*(StringView)>("addReturnStatement", &_::addReturnStatement)
            .method<ReturnStatement*(Expression*)>("addReturnStatement", &_::addReturnStatement)
            .method<ReturnStatement*()>("addReturnStatement", &_::addReturnStatement)
            .method<Expression*(Semantic*, Method*, Expression*, ExpressionsView)>("createCall", &_::createCall)
            .method<Expression*(Semantic*, Method*, ExpressionsView)>("createCall", &_::createCall)
            .method<Expression*(Semantic*, Function*, ExpressionsView)>("createCall", &_::createCall)
            .method<ExpressionStatement*(Semantic*, Method*, Expression*, ExpressionsView)>("addCall", &_::addCall)
            .method<ExpressionStatement*(Semantic*, Method*, ExpressionsView)>("addCall", &_::addCall)
            .method<ExpressionStatement*(Semantic*, Function*, ExpressionsView)>("addCall", &_::addCall)
            .method<void(Statement*)>("prependStatement", &_::prependStatement)
            .method<void(Expression*)>("prependStatement", &_::prependStatement)
            .method<void(const Statements&)>("prependStatements", &_::prependStatements)
            .method<LocalVariable*(StringView) const>("getLocalVariableCascade", &_::getLocalVariableCascade)
            .method<LocalVariable*(StringView) const>("getLocalVariable", &_::getLocalVariable)
            .method<LocalVariable*(size_t) const>("getLocalVariable", &_::getLocalVariable)
            .method<size_t() const>("getLocalVariableCount", &_::getLocalVariableCount)
            .method<Statement*(size_t) const>("getBlockStatement", &_::getBlockStatement)
            .method<size_t() const>("getBlockStatementCount", &_::getBlockStatementCount)
            .method<Statement*(Statement*) const>("getNextStatementOf", &_::getNextStatementOf)
            .method<Statement*() const>("getFirstNonBlockStatementCascade", &_::getFirstNonBlockStatementCascade)
            .method<Statement*(Statement*) const>("getNextNonBlockStatement", &_::getNextNonBlockStatement)
            .method<Subroutine*() const>("getSubroutine", &_::getSubroutine)
            .method<void(LocalVariables&) const>("getLocalVariablesDeep", &_::getLocalVariablesDeep)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<LocalVariables const&() const>("getLocalVariables", &_::getLocalVariables)
            .method<Statements const&() const>("getStatements", &_::getStatements)
            .method<Block*() const>("getParentBlock", &_::getParentBlock)
            .method<LocalVariableInitializationStatement*(LocalVariable*) const>("findLocalVariableInitializationStatement", &_::findLocalVariableInitializationStatement)
            .method<size_t(Block*) const>("getBlockIndex", &_::getBlockIndex)
            /// missing symbol(s) reflection (std::ostream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::ostream &, int) const, virtual_|override_>("dump", &_::dump)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
        
        .protected_()
            .method<Label*(int) const>("findLabelWithTagCascade", &_::findLabelWithTagCascade)
            .method<void(Expressions&, Block*) const>("fetchControlScopedDestructions", &_::fetchControlScopedDestructions)["nullptr"]
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Block")
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
