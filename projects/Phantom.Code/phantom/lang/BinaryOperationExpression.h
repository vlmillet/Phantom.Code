// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Expression.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Generic representation of a binary operation expression, mostly used for placeholding in
/// templates.
class PHANTOM_EXPORT_PHANTOM_CODE BinaryOperationExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    BinaryOperationExpression(StringView a_strOperator, Expression* a_pLeftExpression, Expression* a_pRightExpression);
    BinaryOperationExpression(Type* a_pValueType, StringView a_strOperator, Expression* a_pLeftExpression,
                              Expression* a_pRightExpression);
    void initialize();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the operator name.
    ///
    /// \return The operator name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getOperatorName() const { return m_strOperator; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the left operand expression.
    ///
    /// \return Tthe left operand expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getLeftOperandExpression() const { return m_pLeftOperandExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the right operand expression.
    ///
    /// \return Tthe right operand expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getRightOperandExpression() const { return m_pRightOperandExpression; }

    virtual void eval(ExecutionContext&) const { PHANTOM_ASSERT(false, "Invalid binary operation evaluation"); }

    virtual void flush(ExecutionContext& a_Context) const
    {
        m_pLeftOperandExpression->flush(a_Context);
        m_pRightOperandExpression->flush(a_Context);
    }

    virtual bool hasStorage() const { return false; }

    virtual BinaryOperationExpression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    String      m_strOperator;
    Expression* m_pLeftOperandExpression;
    Expression* m_pRightOperandExpression;
};

} // namespace lang
} // namespace phantom
