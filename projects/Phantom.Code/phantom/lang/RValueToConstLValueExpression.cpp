// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "RValueToConstLValueExpression.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
RValueToConstLValueExpression::RValueToConstLValueExpression(Expression* a_pRValueExpression)
    : Expression(a_pRValueExpression->getValueType()->addConst()->addLValueReference()),
      m_pRValueExpression(a_pRValueExpression)
{
    PHANTOM_ASSERT(m_pRValueExpression);
    PHANTOM_ASSERT(a_pRValueExpression->getValueType()->asReference() == nullptr);
}

RValueToConstLValueExpression* RValueToConstLValueExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<RValueToConstLValueExpression>(m_pRValueExpression);
}

void RValueToConstLValueExpression::eval(ExecutionContext& a_Context) const
{
    *((void**)a_Context.resultPointer()) = m_pRValueExpression->loadEffectiveAddress(a_Context);
}

} // namespace lang
} // namespace phantom
