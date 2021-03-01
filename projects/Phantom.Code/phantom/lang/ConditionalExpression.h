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
class PHANTOM_EXPORT_PHANTOM_CODE ConditionalExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ConditionalExpression(Expression* a_pTestExpression, Expression* a_pThenExpression, Expression* a_pElseExpression);
    void initialize();

    virtual void eval(ExecutionContext& a_Context) const;

    virtual ConditionalExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    Expression* getConditionExpression() const { return m_pTestExpression; }
    Expression* getThenExpression() const { return m_pThenExpression; }
    Expression* getElseExpression() const { return m_pElseExpression; }

protected:
    Expression* m_pTestExpression;
    Expression* m_pThenExpression;
    Expression* m_pElseExpression;
};

} // namespace lang
} // namespace phantom
