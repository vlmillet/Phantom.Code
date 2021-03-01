// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "ConditionalExpression.h"

#include <phantom/typeof>
/* *********************************************** */
namespace phantom
{
namespace lang
{
ConditionalExpression::ConditionalExpression(Expression* a_pTestExpression, Expression* a_pThenExpression,
                                             Expression* a_pElseExpression)
    : Expression(a_pThenExpression->getValueType()),
      m_pTestExpression(a_pTestExpression),
      m_pThenExpression(a_pThenExpression),
      m_pElseExpression(a_pElseExpression)
{
    PHANTOM_ASSERT(m_pTestExpression->isTemplateDependant() ||
                   m_pTestExpression->getValueType()->removeQualifiers() == PHANTOM_TYPEOF(bool));
    PHANTOM_ASSERT(m_pElseExpression->isTemplateDependant() || m_pThenExpression->isTemplateDependant() ||
                   m_pElseExpression->getValueType()->removeQualifiers()->isSame(
                   m_pThenExpression->getValueType()->removeQualifiers()));
}

void ConditionalExpression::initialize()
{
    Expression::initialize();
    addSubExpression(m_pTestExpression);
    addSubExpression(m_pThenExpression);
    addSubExpression(m_pElseExpression);
}

void ConditionalExpression::eval(ExecutionContext& a_Context) const
{
    bool result = false;
    m_pTestExpression->load(a_Context, &result);
    if (result)
    {
        m_pThenExpression->eval(a_Context);
    }
    else
    {
        m_pElseExpression->eval(a_Context);
    }
}

ConditionalExpression* ConditionalExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<ConditionalExpression>(m_pTestExpression, m_pThenExpression, m_pElseExpression);
}

} // namespace lang
} // namespace phantom
