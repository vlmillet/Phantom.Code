// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "FieldExpression.h"

#include "phantom/lang/Field.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Type* FieldExpression_replicateType(Type* a_pRef, Type* a_pToReplicate)
{
    Type* pResult = a_pRef->replicate(a_pToReplicate);
    if (pResult->asRValueReference())
    {
        /// named r-value reference become l-value reference (property access names the r-value)
        pResult = pResult->removeReference()->addLValueReference();
    }
    return pResult;
}

FieldExpression::FieldExpression(Expression* a_pObjectExpression, Field* a_pField)
    : LValueExpression(FieldExpression_replicateType(a_pObjectExpression->getValueType(), a_pField->getValueType()),
                       a_pField->getValueType()),
      m_pObjectExpression(a_pObjectExpression),
      m_pField(a_pField)
{
    PHANTOM_ASSERT(m_pField);
    PHANTOM_ASSERT(m_pField->getOffset() != ~size_t(0) || a_pField->isTemplateElement());
    PHANTOM_ASSERT(m_pObjectExpression);
    PHANTOM_ASSERT(m_pObjectExpression->hasEffectiveAddress() ||
                   m_pObjectExpression->getValueType()->isTemplateDependant());
}

void FieldExpression::initialize()
{
    LValueExpression::initialize();
    addReferencedElement(m_pField);
    addDependencyOn(m_pField);
    addSubExpression(m_pObjectExpression);
}

void* FieldExpression::internalEvalAddress(ExecutionContext& a_Context) const
{
    void* pAdd = m_pObjectExpression->loadEffectiveAddress(a_Context);
    PHANTOM_ASSERT(pAdd);
    return m_pField->getAddress(pAdd);
}

FieldExpression* FieldExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<FieldExpression>(m_pObjectExpression, m_pField);
}

LanguageElement* FieldExpression::removeExpression() const
{
    return m_pField;
}

bool FieldExpression::isPersistent() const
{
    return Expression::isPersistent() && m_pField->isNative();
}

} // namespace lang
} // namespace phantom
