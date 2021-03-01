// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "ControlStatement.h"
#include "Expression.h"
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a return statement.
/// \code{.cpp}
///     ex: return 0.f / return;
/// \endcode
class PHANTOM_EXPORT_PHANTOM_CODE ReturnStatement : public ControlStatement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

    friend class Block;

public:
    /// \brief  Constructs a void return statement.
    ReturnStatement();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a value return statement.
    ///
    /// \param [in,out] a_pReturnExpression The returned value expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ReturnStatement(Expression* a_pReturnExpression);

    void initialize()
    {
        ControlStatement::initialize();
        if (m_pReturnExpression)
            _setupExpression();
    }

    ReturnStatement* asReturnStatement() const override { return (ReturnStatement*)this; }

    void eval(ExecutionContext& a_Context) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the returned value expression.
    ///
    /// \return The returned value expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getExpression() const { return m_pReturnExpression; }

    Statement* getTargetStatement() const override { return nullptr; }

protected:
    void checkValidity();
    void _setupExpression()
    {
        if (m_pReturnExpression->getOwner())
            m_pReturnExpression = m_pReturnExpression->clone(this);
        m_pReturnExpression->removeRValueReferenceExpression()->setTemporary(false);
        addSubExpression(m_pReturnExpression);
    }

    void onAttachedToBlock(Block* a_pBlock) override;

protected:
    Expression* m_pReturnExpression;
    Expression* m_pConvertedReturnExpression;
    String*     m_pExpressionString;
};

} // namespace lang
} // namespace phantom
