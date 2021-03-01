// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "PropertyExpression.h"

#include <phantom/lang/Property.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
static Type* PropertyExpression_replicateType(Type* a_pRef, Type* a_pToReplicate)
{
    Type* pResult = a_pRef->replicate(a_pToReplicate);
    if (pResult->asLValueReference() == nullptr)
    {
        /// named r-value reference become l-value reference (property access names the r-value)
        pResult = pResult->removeReference()->addLValueReference();
    }
    return pResult;
}

PropertyExpression::PropertyExpression(Expression* a_pObjectExpression, Property* a_pProperty)
    : Expression(PropertyExpression_replicateType(a_pObjectExpression->getValueType(), a_pProperty->getValueType())),
      m_pObjectExpression(a_pObjectExpression),
      m_pProperty(a_pProperty)
{
    PHANTOM_ASSERT(a_pProperty);
    if (a_pObjectExpression->getValueType()->removeReference()->removeConst() != a_pProperty->getOwner())
    {
        Class* pLeftClass = a_pObjectExpression->getValueType()->removeReference()->removeConst()->asClass();
        PHANTOM_ASSERT(pLeftClass);
        Class* pOwnerClass = a_pProperty->getOwner()->asClass();
        PHANTOM_ASSERT(pOwnerClass);
        PHANTOM_ASSERT(pLeftClass->getBaseClassOffsetCascade(pOwnerClass) == 0);
    }

    m_pBuffer = getValueType()->removeReference()->allocate();
    getValueType()->removeReference()->construct(m_pBuffer);
}

void PropertyExpression::initialize()
{
    Expression::initialize();
    addReferencedElement(m_pProperty);
    addSubExpression(m_pObjectExpression);
    addDependencyOn(m_pProperty);
}

void PropertyExpression::terminate()
{
    Type* pValueType = getValueType();
    PHANTOM_ASSERT(pValueType);
    if (m_pBuffer)
    {
        pValueType->removeReference()->destroy(m_pBuffer);
        pValueType->removeReference()->deallocate(m_pBuffer);
    }
    Expression::terminate();
}

void PropertyExpression::eval(ExecutionContext& a_Context) const
{
    if (getValueType()->asReference())
    {
        m_pProperty->getValue(m_pObjectExpression->loadEffectiveAddress(a_Context), m_pBuffer);
        *(void**)a_Context.resultPointer() = m_pBuffer;
    }
    else
    {
        m_pProperty->getValue(m_pObjectExpression->loadEffectiveAddress(a_Context), a_Context.resultPointer());
    }
}

void PropertyExpression::flush(ExecutionContext& a_Context) const
{
    void* pAddress = m_pObjectExpression->loadEffectiveAddress(a_Context);
    m_pProperty->setValue(pAddress, m_pBuffer);
    m_pObjectExpression->flush(a_Context);
}

PropertyExpression* PropertyExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<PropertyExpression>(m_pObjectExpression, m_pProperty);
}

LanguageElement* PropertyExpression::removeExpression() const
{
    return m_pProperty;
}

bool PropertyExpression::isPersistent() const
{
    return Expression::isPersistent() && m_pProperty->isNative();
}

} // namespace lang
} // namespace phantom
