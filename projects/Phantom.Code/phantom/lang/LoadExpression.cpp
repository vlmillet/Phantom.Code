// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "LoadExpression.h"

#include "phantom/lang/Array.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
static Type* LoadExpressionEvalType(Expression* a_pLoadedExpression)
{
    Type* pDerefType = a_pLoadedExpression->getValueType()->removeReference();
    if (Array* pDerefArray = pDerefType->asArray()) // we are loading a int[5]& for example, we return a int*
    {
        pDerefType = pDerefArray->removeArray()->addPointer();
    }
    return pDerefType;
}

LoadExpression::LoadExpression(Expression* a_pLoadedExpression)
    : Expression(a_pLoadedExpression->getValueType()->removeReference(), LoadExpressionEvalType(a_pLoadedExpression)),
      m_pLoadedExpression(a_pLoadedExpression)
{
    PHANTOM_ASSERT(m_pLoadedExpression);
    PHANTOM_ASSERT(m_pLoadedExpression->getValueType()->asReference());
    setCodeRange(a_pLoadedExpression->getCodeRange());
}

void LoadExpression::initialize()
{
    Expression::initialize();
    addSubExpression(m_pLoadedExpression);
}

LoadExpression* LoadExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<LoadExpression>(m_pLoadedExpression);
}

void LoadExpression::eval(ExecutionContext& a_Context) const
{
    if (getValueType()->getTypeKind() == TypeKind::Array)
    {
        *(void**)a_Context.resultPointer() = m_pLoadedExpression->loadEffectiveAddress(a_Context);
    }
    getValueType()->copy(a_Context.resultPointer(), m_pLoadedExpression->loadEffectiveAddress(a_Context));
}

void* LoadExpression::evalStorage(ExecutionContext& a_Context) const
{
    if (getValueType()->getTypeKind() == TypeKind::Array)
    {
        return &(m_pArrayAddressingHack = m_pLoadedExpression->loadEffectiveAddress(a_Context));
    }
    return m_pLoadedExpression->loadEffectiveAddress(a_Context);
}

void* LoadExpression::getStorage() const
{
    if (getValueType()->getTypeKind() == TypeKind::Array)
    {
        return &m_pArrayAddressingHack;
    }
    return m_pLoadedExpression->getStorage();
}

} // namespace lang
} // namespace phantom
