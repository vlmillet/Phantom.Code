// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Statement.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents an expression statement.
/// \code{.cpp}
///     expr;
/// \endcode
class PHANTOM_EXPORT_PHANTOM_CODE ExpressionStatement : public Statement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an expression statement with the given expression.
    ///
    /// \param [in,out] a_pExpression   The expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionStatement(Expression* a_pExpression);

    void initialize()
    {
        Statement::initialize();
        addSubExpression(m_pExpression);
    }

    virtual ExpressionStatement* asExpressionStatement() const { return (ExpressionStatement*)this; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext& a_Context) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the expression.
    ///
    /// \return null if it fails, else the expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getExpression() const { return m_pExpression; }

protected:
    Expression* m_pExpression;
};

} // namespace lang
} // namespace phantom
