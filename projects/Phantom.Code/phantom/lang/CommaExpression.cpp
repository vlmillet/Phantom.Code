// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "CommaExpression.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
CommaExpression::CommaExpression(Expression* a_pLeftExpression, Expression* a_pRightExpression)
    : Expression(a_pRightExpression ? a_pRightExpression->getValueType() : nullptr),
      m_pLeftExpression(a_pLeftExpression),
      m_pRightExpression(a_pRightExpression)
{
    PHANTOM_ASSERT(m_pLeftExpression);
    PHANTOM_ASSERT(m_pRightExpression);
}

void CommaExpression::initialize()
{
    Expression::initialize();
    addSubExpression(m_pLeftExpression);
    addSubExpression(m_pRightExpression);
}

void CommaExpression::eval(ExecutionContext& a_Context) const
{
    m_pLeftExpression->eval(a_Context, nullptr);
    m_pRightExpression->eval(a_Context);
}

CommaExpression* CommaExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<CommaExpression>(m_pLeftExpression, m_pRightExpression);
}

} // namespace lang
} // namespace phantom
