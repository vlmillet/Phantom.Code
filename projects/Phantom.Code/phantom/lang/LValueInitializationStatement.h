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
/// \brief  Base class for l-value initialization statements (variables/data members/etc...).
class PHANTOM_EXPORT_PHANTOM_CODE LValueInitializationStatement : public Statement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an l-value initialization statement with a value type and an
    /// initialization expression.
    ///
    /// \param [in,out] a_pValueType                The value type
    /// \param [in,out] a_pInitializationExpression The initialization expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LValueInitializationStatement(Type* a_pValueType, Expression* a_pInitializationExpression);

    void initialize()
    {
        if (m_pInitializationExpression)
            addSubExpression(m_pInitializationExpression);
    }

    virtual LValueInitializationStatement* asLValueInitializationStatement() const
    {
        return (LValueInitializationStatement*)this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the initialization expression.
    ///
    /// \return The initialization expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getInitializationExpression() const { return m_pInitializationExpression; }

    Evaluable* evaluateExpressionLifeTime(Expression* a_pExpression) const override;

    Type* getValueType() const { return m_pValueType; }

protected:
    Type*       m_pValueType;
    Expression* m_pInitializationExpression;
};

} // namespace lang
} // namespace phantom
