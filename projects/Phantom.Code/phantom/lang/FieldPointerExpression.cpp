// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "FieldPointerExpression.h"

#include <phantom/lang/FieldPointer.h>
#include <phantom/lang/LValueReference.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
FieldPointer* FieldPointerExpression_extractType(Expression* a_pFieldExpression)
{
    FieldPointer* pFieldPointer = a_pFieldExpression->getValueType()->removeReference()->asFieldPointer();
    if (pFieldPointer == nullptr)
        return nullptr;
    return pFieldPointer;
}

FieldPointerExpression::FieldPointerExpression(Expression* a_pObjectExpression, Expression* a_pFieldExpression)
    : Expression(FieldPointerExpression_extractType(a_pFieldExpression)->getValueType()->makeLValueReference()),
      m_pObjectExpression(a_pObjectExpression),
      m_pFieldPointerExpression(a_pFieldExpression),
      m_pFieldPointer(FieldPointerExpression_extractType(a_pFieldExpression))
{
    ClassType* pClassType = getValueType()->removeReference()->asFieldPointer()->getObjectType();

    PHANTOM_ASSERT(a_pObjectExpression->getValueType()->removeConstReference()->isA(pClassType));

    PHANTOM_ASSERT(m_pObjectExpression);
    PHANTOM_ASSERT(m_pObjectExpression->getValueType() && m_pObjectExpression->hasEffectiveAddress());
}

void FieldPointerExpression::initialize()
{
    Expression::initialize();
    addSubExpression(m_pObjectExpression);
}

void FieldPointerExpression::eval(ExecutionContext& a_Context) const
{
    byte buffer[4 * sizeof(void*)];
    m_pFieldPointerExpression->load(a_Context, &buffer);
    *(void**)a_Context.resultPointer() =
    m_pFieldPointer->getAddress(&buffer, m_pObjectExpression->loadEffectiveAddress(a_Context));
}

FieldPointerExpression* FieldPointerExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<FieldPointerExpression>(m_pObjectExpression, m_pFieldPointerExpression);
}

} // namespace lang
} // namespace phantom
