// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "PointerAdjustmentExpression.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
PointerAdjustmentExpression::PointerAdjustmentExpression(Type* a_pOutputRefOrPtrType, Expression* a_pPointerExpression,
                                                         ptrdiff_t a_iOffset)
    : Expression(a_pOutputRefOrPtrType), m_pPointerExpression(a_pPointerExpression), m_iOffset(a_iOffset)
{
    PHANTOM_ASSERT(m_iOffset != 0, "adjustment is not identity and offset must have non zero value");
    PHANTOM_ASSERT(m_pPointerExpression->getValueType()->removeQualifiers()->asPointer() ||
                   m_pPointerExpression->getValueType()->removeQualifiers()->asReference() || isTemplateDependant());
}

void PointerAdjustmentExpression::eval(ExecutionContext& a_Context) const
{
    byte** ppDest = (byte**)a_Context.resultPointer();
    m_pPointerExpression->eval(a_Context, ppDest);
    if (*ppDest)
        *ppDest += m_iOffset;
}

PointerAdjustmentExpression* PointerAdjustmentExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<PointerAdjustmentExpression>(getValueType(), m_pPointerExpression, m_iOffset);
}

} // namespace lang
} // namespace phantom
