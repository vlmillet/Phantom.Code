// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "IdentityExpression.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
IdentityExpression::IdentityExpression(Type* a_pValueType, Expression* a_pExpression)
    : Expression(a_pValueType,
                 a_pExpression->getEvalType()->isSame(a_pExpression->getValueType()) ? a_pValueType
                                                                                     : a_pExpression->getEvalType()),
      m_pExpression(a_pExpression)
{
    setImplicit(); // implicit by default
    PHANTOM_ASSERT(m_pExpression);
    setCodeRange(a_pExpression->getCodeRange());
    PHANTOM_ASSERT(a_pValueType->isVoid() ||
                   (m_pExpression->getValueType()->asReference() == nullptr) ==
                   (getValueType()->asReference() == nullptr) ||
                   m_pExpression->getValueType()->getAddressLevel() == getValueType()->getAddressLevel());
}

IdentityExpression* IdentityExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    auto pClone = a_pOwner->New<IdentityExpression>(getValueType(), m_pExpression);
    pClone->m_CastKind = m_CastKind;
    return pClone;
}

} // namespace lang
} // namespace phantom
