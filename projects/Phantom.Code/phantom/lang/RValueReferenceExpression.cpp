// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "RValueReferenceExpression.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
RValueReferenceExpression::RValueReferenceExpression(Expression* a_pRValueExpression)
    : Expression(a_pRValueExpression->getValueType()->addRValueReference()), m_pRValueExpression(a_pRValueExpression)
{
    PHANTOM_ASSERT(m_pRValueExpression);
    PHANTOM_ASSERT(a_pRValueExpression->getValueType()->asReference() == nullptr);
}

RValueReferenceExpression* RValueReferenceExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<RValueReferenceExpression>(m_pRValueExpression);
}

Expression* RValueReferenceExpression::load(LanguageElement*)
{
    Expression* pReturn = m_pRValueExpression;
    removeSubExpression(m_pRValueExpression);
    getOwner()->Delete(this);
    return pReturn;
}

phantom::lang::LanguageElement* RValueReferenceExpression::hatchExpression()
{
    return m_pRValueExpression->hatchExpression();
}

void RValueReferenceExpression::eval(ExecutionContext& a_Context) const
{
    void* res = m_pRValueExpression->loadEffectiveAddress(a_Context);
    if (void* ret = a_Context.resultPointer())
        *((void**)ret) = res;
}

} // namespace lang
} // namespace phantom
