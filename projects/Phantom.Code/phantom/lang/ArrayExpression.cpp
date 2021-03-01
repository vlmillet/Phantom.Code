// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "ArrayExpression.h"

#include <phantom/lang/Array.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
ArrayExpression::ArrayExpression(Array* a_pType, ExpressionsView a_Values) : Expression(a_pType), m_Values(a_Values)
{
    PHANTOM_ASSERT(a_pType);
    PHANTOM_ASSERT(a_Values.size() <= a_pType->getItemCount());
    for (Expression* pExp : m_Values)
    {
        PHANTOM_ASSERT(pExp->hasEvalConversionTo(a_pType->getItemType()) ||
                       pExp->removeRValueReferenceExpression()->hasEvalConversionTo(a_pType->getItemType()));
    }
    if (a_pType->getItemType()->asClass())
        setTemporary(true);
    PHANTOM_ASSERT(a_pType->getItemCount() || m_Values.empty());
}

void ArrayExpression::initialize()
{
    Expression::initialize();
    for (Expression*& pExp : m_Values)
    {
        addSubExpression(pExp);
        pExp->setTemporary(false);
    }
}

void ArrayExpression::eval(ExecutionContext& a_Context) const
{
    byte* pDest = (byte*)a_Context.resultPointer();
    if (size_t c = m_Values.size())
    {
        size_t itemSize = getValueType()->getSize() / c;
        for (Expression* pExp : m_Values)
        {
            pExp->eval(a_Context, pDest);
            pDest += itemSize;
        }
    }
    else
    {
        memset(pDest, 0, getValueType()->getSize()); // zero mem empty array
    }
}

ArrayExpression* ArrayExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<ArrayExpression>(static_cast<Array*>(getValueType()), m_Values);
}

} // namespace lang
} // namespace phantom
