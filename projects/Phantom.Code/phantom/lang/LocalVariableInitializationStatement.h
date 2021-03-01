// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Expression.h"
#include "Statement.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a local variable initialization statement.
/// \code{.cpp}
///     int i = 5;
/// \endcode
class PHANTOM_EXPORT_PHANTOM_CODE LocalVariableInitializationStatement : public Statement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs the initialization statement with a local variable and an initialization
    ///         expression.
    ///
    /// \param  a_pLocalVariable            The local variable.
    /// \param  a_pInitializationExpression The initialization expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariableInitializationStatement(LocalVariable* a_pLocalVariable, Expression* a_pInitializationExpression);

    void initialize();

    virtual LocalVariableInitializationStatement* asLocalVariableInitializationStatement() const
    {
        return (LocalVariableInitializationStatement*)this;
    }

    void eval(ExecutionContext& a_Context) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the associated local variable.
    ///
    /// \return The local variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariable* getLocalVariable() const { return m_pLocalVariable; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the initialization expression.
    ///
    /// \return The initialization expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getInitializationExpression() const { return m_pInitializationExpression; }

    Evaluable* evaluateExpressionLifeTime(Expression* a_pExpression) const override;

    void onAttachedToBlock(Block* a_pBlock) override;

    bool isTemporaryContainer() const { return m_temporaryContainer; }

protected:
    LocalVariable* m_pLocalVariable;
    Expression*    m_pInitializationExpression;
    bool           m_temporaryContainer = false;
};

} // namespace lang
} // namespace phantom
