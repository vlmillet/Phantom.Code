// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "LValueExpression.h"

#include <phantom/lang/LValueReference.h>
#include <phantom/lang/RValueReference.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
Type* LValueExpressionTypeDeducer(Type* a_pInput)
{
    if (!a_pInput)
        return nullptr;
    LValueReference* pReference = a_pInput->asLValueReference();
    if (pReference)
        return pReference;
    RValueReference* pRValueReference = a_pInput->asRValueReference();
    Type*            pType = nullptr;
    if (pRValueReference)
    {
        pType = pRValueReference->removeRValueReference()
                ->makeLValueReference(); /// Named r-value references are l-values
    }
    else
        pType = a_pInput->makeLValueReference();
    return pType;
}

LValueExpression::LValueExpression(Type* a_pRValueType, uint a_uiFlags /*= 0*/)
    : Expression(LValueExpressionTypeDeducer(a_pRValueType), nullptr, a_uiFlags),
      m_pRValueType(a_pRValueType)
{
}

LValueExpression::LValueExpression(Type* a_pLValueType, Type* a_pRValueType, uint a_uiFlags /*= 0*/)
    : Expression(a_pLValueType, nullptr, a_uiFlags), m_pRValueType(a_pRValueType)
{
}

void LValueExpression::eval(ExecutionContext& a_Context) const
{
    void* pPhysicalAddress = internalEvalAddress(a_Context);
    if (m_pRValueType->asReference())
    {
        /// ex: a local variable with a reference type :
        /// int& variable = v;
        /// have an effectiveAddress of &variable, and not its address on the stack
        /// when you do variable = 5; it goes to *(&variable)
        *(void**)a_Context.resultPointer() = *(void**)pPhysicalAddress;
    }
    else
    {
        *(void**)a_Context.resultPointer() = pPhysicalAddress;
    }
}

} // namespace lang
} // namespace phantom
