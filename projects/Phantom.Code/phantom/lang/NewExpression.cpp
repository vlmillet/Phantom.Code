// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "NewExpression.h"

#include <phantom/typeof>
/* *********************************************** */
namespace phantom
{
namespace lang
{
NewExpression::NewExpression(Expression* a_pInitializationExpression, Expression* a_pCountExpression)
    : Expression(a_pInitializationExpression->getValueType()->addPointer()),
      m_pInitializationExpression(a_pInitializationExpression),
      m_pCountExpression(a_pCountExpression)
{
    PHANTOM_ASSERT(m_pCountExpression == nullptr ||
                   m_pCountExpression->getValueType()->removeAllQualifiers()->removeReference() ==
                   PHANTOM_TYPEOF(size_t));
}

void NewExpression::eval(ExecutionContext& a_Context) const
{
    Type*  pType = m_pInitializationExpression->getEvalType();
    size_t count = 1;
    byte*  pMemory;
    if (m_pCountExpression)
    {
        m_pCountExpression->load(a_Context, &count);
        PHANTOM_ASSERT(count);
        pMemory = (byte*)pType->allocate(count);
    }
    else
    {
        pMemory = (byte*)pType->allocate();
    }
    if (a_Context.resultPointer())
    {
        *((void**)a_Context.resultPointer()) = pMemory;
    }
    for (size_t i = 0; i < count; ++i)
    {
        m_pInitializationExpression->eval(a_Context, pMemory);
        flush(a_Context);
        pMemory += pType->getSize();
    }
    flush(a_Context);
}

NewExpression* NewExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<NewExpression>(m_pInitializationExpression, m_pCountExpression);
}

} // namespace lang
} // namespace phantom
