// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "FieldPointerAdjustmentExpression.h"

#include <phantom/lang/FieldPointer.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
FieldPointerAdjustmentExpression::FieldPointerAdjustmentExpression(Type*       a_pOutputPointerType,
                                                                   Expression* a_pPointerExpression,
                                                                   ptrdiff_t   a_iOffset)
    : Expression(a_pOutputPointerType), m_pPointerExpression(a_pPointerExpression), m_iOffset(a_iOffset)
{
    PHANTOM_ASSERT(m_pPointerExpression);
    PHANTOM_ASSERT(m_iOffset != 0, "adjustment is not identity and offset must have non zero value");
}

FieldPointerAdjustmentExpression::~FieldPointerAdjustmentExpression() {}

void FieldPointerAdjustmentExpression::eval(ExecutionContext& a_Context) const
{
    ptrdiff_t* ppDest = (ptrdiff_t*)a_Context.resultPointer();
    m_pPointerExpression->eval(a_Context, ppDest);
    *ppDest += m_iOffset;
}

FieldPointerAdjustmentExpression* FieldPointerAdjustmentExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<FieldPointerAdjustmentExpression>(static_cast<FieldPointer*>(getValueType()),
                                                           m_pPointerExpression, m_iOffset);
}

} // namespace lang
} // namespace phantom
