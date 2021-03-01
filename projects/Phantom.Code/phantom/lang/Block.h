// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Statement.h"

#include <phantom/lang/Scope.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class LocalVariable;
class Subroutine;
class ReturnStatement;

/// \brief  Represents a code block.

class PHANTOM_EXPORT_PHANTOM_CODE Block : public Statement, public Scope
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

    friend class Subroutine;
    friend class ClassType;
    friend class ControlStatement;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tags to identify the kind of block (useful for retro-translation).
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    enum ETag
    {
        e_Tag_None,
        e_Tag_If,
        e_Tag_For,
        e_Tag_While,
        e_Tag_Switch,
        e_Tag_Do,
        e_Tag_Foreach,
        e_Tag_Assert,
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a block nested in a parent block with an optional identifier name.
    ///
    /// \param  a_pParentBlock    The parent block.
    /// \param  a_iTag           (optional) Tag to identify the kind of block (can be used to
    /// recognize for, while, switch, ... blocks for example).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Block(int a_iTag = e_Tag_None);

    Block* newBlock(int a_iTag = e_Tag_None)
    {
        auto b = New<Block>(a_iTag);
        addStatement(b);
        return b;
    }

    void initialize() { Statement::initialize(); }

    virtual Scope* asScope() const { return (Block*)this; }
    virtual Block* asBlock() const { return (Block*)this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the tag associated with this block.
    ///
    /// \return The tag.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    int getTag() const { return m_iTag; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a local variable.
    ///
    /// \param  a_pType     The local variable type.
    /// \param  a_strName   The local variable name.
    /// \param  a_pInit     (optional) The init value expression.
    /// \param  a_Modifiers (optional) The local variable modifiers.
    ///
    /// \return null if it fails, else the new local variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariable* addLocalVariable(Type* a_pType, StringView a_strName, Expression* a_pInit = nullptr,
                                    Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a local variable.
    ///
    /// \param a_pVariable The local variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addLocalVariable(LocalVariable* a_pVariable);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a local variable from code.
    ///
    /// \param a_pVariable The local variable statement code.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariable* addLocalVariable(StringView a_strCode, Language* a_pLanguage = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a statement.
    ///
    /// \param a_pStatement   The statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addStatement(Statement* a_pStatement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Inserts a statement after another one.
    ///
    /// \param a_pAfterMe      The statement we want to be after (if null, insert at the end).
    /// \param a_pStatement    The statement to insert.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void insertStatementAfter(Statement* a_pAfterMe, Statement* a_pStatement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Convenient function to add an expression statement.
    ///
    /// \param  a_pExpression   The expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionStatement* addStatement(Expression* a_pExpression);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Convenient function to add an expression statement from an expression code.
    ///
    /// \param  a_pLanguage The language used to parse the expression.
    /// \param  a_strExpression     The expression code.
    ///
    /// \return null if it fails, else the new expression statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionStatement* addStatement(StringView a_strExpression, Language* a_pLanguage);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Convenient function to add an expression statement from an expression code.
    ///
    /// \param  a_strExpression The expression code.
    ///
    /// \return null if it fails, else the new expression statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionStatement* addStatement(StringView a_strExpression);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Convenient function to add a return statement from an expression name.
    ///
    /// \param  a_strExpression The expression name (must be empty if embedding subroutine is void).
    ///
    /// \return null if it fails, else the new return statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ReturnStatement* addReturnStatement(StringView a_strExpression);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Convenient function to add a return statement.
    ///
    /// \param a_pExpression   The returned expression (must be null if embedding subroutine is
    /// void).
    ///
    /// \return null if it fails, else the new return statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ReturnStatement* addReturnStatement(Expression* a_pExpression);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Convenient function to add an empty return statement.
    ///
    /// \return null if it fails (non void member function), else the new return statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ReturnStatement* addReturnStatement();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Create call of the given method.
    ///
    /// \return null if it failed to resolve 'this' or argument expressions, else the generated
    /// expression statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* createCall(Semantic* a_pSemantic, Method* a_pMethod, Expression* a_pThisExpression,
                           ExpressionsView a_Arguments);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Create call of the given method using this block 'this'.
    ///
    /// \return null if it failed to resolve 'this' or argument expressions, else the generated
    /// expression statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* createCall(Semantic* a_pSemantic, Method* a_pMethod, ExpressionsView a_Arguments);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Create call of the given function.
    ///
    /// \return null if it failed to resolve argument expressions, else the generated expression
    /// statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* createCall(Semantic* a_pSemantic, Function* a_pFunction, ExpressionsView a_Arguments);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds call of the given method.
    ///
    /// \return null if it failed to resolve 'this' or argument expressions, else the generated
    /// expression statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionStatement* addCall(Semantic* a_pSemantic, Method* a_pMethod, Expression* a_pThisExpression,
                                 ExpressionsView a_Arguments);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds call of the given method.
    ///
    /// \return null if it failed to resolve 'this' or argument expressions, else the generated
    /// expression statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionStatement* addCall(Semantic* a_pSemantic, Method* a_pMethod, ExpressionsView a_Arguments);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds call of the given function.
    ///
    /// \return null if it failed to resolve argument expressions, else the generated expression
    /// statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionStatement* addCall(Semantic* a_pSemantic, Function* a_pFunction, ExpressionsView a_Arguments);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Prepends the given statement.
    ///
    /// \param  a_pStatement    The statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void prependStatement(Statement* a_pStatement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Prepends the given statement expression.
    ///
    /// \param  a_pStatement    The statement expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void prependStatement(Expression* a_pExpression);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Prepends the given statements.
    ///
    /// \param  a_Statements    The statements.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void prependStatements(const Statements& a_Statements);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query a local variable of this block or its sub blocks recursively.
    ///
    /// \param  a_strName   The requested local variable name.
    ///
    /// \return null if it fails, else the local variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariable* getLocalVariableCascade(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query a local variable with the given name.
    ///
    /// \param  a_strName   The requested local variable name.
    ///
    /// \return null if it fails, else the local variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariable* getLocalVariable(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a local variable at given index position.
    ///
    /// \param  i   Zero-based index of the local variable.
    ///
    /// \return null if it fails, else the local variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariable* getLocalVariable(size_t i) const { return m_LocalVariables[i]; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the total local variable count.
    ///
    /// \return The local variable count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getLocalVariableCount() const { return m_LocalVariables.size(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a block statement at given index position..
    ///
    /// \param  i   Zero-based index of the statement.
    ///
    /// \return null if it fails, else the statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Statement* getBlockStatement(size_t i) const { return m_BlockStatements[i]; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the number of block statements in this block (ignoring raii implicit ones)
    ///
    /// \return the number of block statements in this block.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getBlockStatementCount() const { return m_BlockStatements.size(); }

    Statement* getNextStatementOf(Statement* a_pStatement) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the first non block statement recursively through this block and sub blocks.
    ///
    /// \return null if only blocks found, else the first non block statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Statement* getFirstNonBlockStatementCascade() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the next non block statement recursively through this block and sub blocks.
    ///
    /// \return null if only blocks found, else the first non block statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Statement* getNextNonBlockStatement(Statement* a_pStatement) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the nesting subroutine.
    ///
    /// \return null if it fails, else the subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* getSubroutine() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets all local variables, recursively through all the child blocks.
    ///
    /// \param a_LocalVariables  Store the local variables.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getLocalVariablesDeep(LocalVariables& a_LocalVariables) const;

    /// \todo
    /// void getAccessibleLocalVariables(LocalVariables& out, const CodePosition& position) const;
    /// LocalVariable* getAccessibleLocalVariable(StringView name, const CodePosition& position)
    /// const; bool containsLine(int line) const; Block* findBlockAtCodePosition(const CodePosition&
    /// a_Position) const; virtual void getAccessibleElementsAt(const CodePosition& a_Position,
    /// LanguageElements& a_Elements) const;

    virtual void eval(ExecutionContext& a_Context) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator pointing to the end of local variables.
    ///
    /// \return An iterator.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariables const& getLocalVariables() const { return m_LocalVariables; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the explicit statements lists .
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Statements const& getStatements() const { return m_BlockStatements; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parent block.
    ///
    /// \return null if no parent, else the parent block.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Block* getParentBlock() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Searches for the given local variable initialization statement.
    ///
    /// \param  a_pLocal    The local variable.
    ///
    /// \return null if no initialization statement found, else the given local variable
    ///         initialization statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariableInitializationStatement* findLocalVariableInitializationStatement(LocalVariable* a_pLocal) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get the index of the given sub block inside this one.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getBlockIndex(Block* a_pSubBlock) const;

    void dump(std::ostream& a_Out, int a_Lvl) const override;

    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void getUniqueName(StringBuffer& a_Buf) const override;

protected:
    Label* findLabelWithTagCascade(int a_iTag) const;
    void   fetchControlScopedDestructions(Expressions& out, Block* a_pUntilBlock = nullptr) const;

private:
    void _addStatement(Statement* a_pStatement);
    void onScopeSymbolAdded(Symbol* a_pElement) override { a_pElement->setOwner(this); }
    void onScopeSymbolRemoving(Symbol* a_pElement) override { a_pElement->setOwner(nullptr); }

private:
    int            m_iTag;
    LocalVariables m_LocalVariables;
    Statements     m_BlockStatements;
};

} // namespace lang
} // namespace phantom
