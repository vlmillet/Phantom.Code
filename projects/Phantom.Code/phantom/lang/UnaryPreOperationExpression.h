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
/// \brief  Generic representation of an unary pre-fixed operation expression, mostly used for
/// placeholding in templates.
class PHANTOM_EXPORT_PHANTOM_CODE UnaryPreOperationExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    UnaryPreOperationExpression(Type* a_pType, StringView a_strOperator, Expression* a_pExpression);
    UnaryPreOperationExpression(StringView a_strOperator, Expression* a_pExpression);

    void initialize();

    virtual void flush(ExecutionContext& a_Context) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the operand expression.
    ///
    /// \return The operand expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getOperandExpression() const { return m_pOperandExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the operator name.
    ///
    /// \return The operator name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getOperatorName() const { return m_strOperator; }

    virtual UnaryPreOperationExpression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    void eval(ExecutionContext&) const override { PHANTOM_ASSERT_NO_IMPL(); }

protected:
    String      m_strOperator;
    Expression* m_pOperandExpression;
};

} // namespace lang
} // namespace phantom
