// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "DeallocateExpression.h"

#include <phantom/lang/Pointer.h>
#include <phantom/typeof>
/* *********************************************** */
namespace phantom
{
namespace lang
{
DeallocateExpression::DeallocateExpression(Type* a_pType, Expression* a_pAddressExpression)
    : Expression(PHANTOM_TYPEOF(void)),
      m_pSizeExpression(nullptr),
      m_pType(a_pType),
      m_pAddressExpression(a_pAddressExpression)
{
    PHANTOM_ASSERT(m_pType);
    PHANTOM_ASSERT(m_pAddressExpression);
    PHANTOM_ASSERT(m_pAddressExpression->isTemplateDependant() ||
                   m_pAddressExpression->getValueType()->removeAllQualifiers()->removeReference()->asPointer());
    PHANTOM_ASSERT(m_pAddressExpression->isTemplateDependant() ||
                   m_pAddressExpression->getValueType()
                   ->removeAllQualifiers()
                   ->removeReference()
                   ->asPointer()
                   ->getPointeeType()
                   ->isSame(m_pType->removeAllQualifiers()));
    PHANTOM_ASSERT(m_pType->asReference() == nullptr);
}

DeallocateExpression::DeallocateExpression(Type* a_pType, Expression* a_pSizeExpression,
                                           Expression* a_pAddressExpression)
    : Expression(PHANTOM_TYPEOF(void)),
      m_pSizeExpression(a_pSizeExpression),
      m_pType(a_pType),
      m_pAddressExpression(a_pAddressExpression)
{
    PHANTOM_ASSERT(m_pSizeExpression);
    PHANTOM_ASSERT(m_pType);
    PHANTOM_ASSERT(m_pType->asReference() == nullptr);
    PHANTOM_ASSERT(m_pAddressExpression);
    PHANTOM_ASSERT(m_pAddressExpression->isTemplateDependant() ||
                   m_pAddressExpression->getValueType()->removeAllQualifiers()->removeReference()->asPointer());
    PHANTOM_ASSERT(m_pAddressExpression->isTemplateDependant() ||
                   m_pAddressExpression->getValueType()
                   ->removeAllQualifiers()
                   ->removeReference()
                   ->asPointer()
                   ->getPointeeType()
                   ->isSame(m_pType->removeAllQualifiers()));
    PHANTOM_ASSERT(m_pSizeExpression->isTemplateDependant() ||
                   m_pSizeExpression->getValueType() == PHANTOM_TYPEOF(size_t));
}

void DeallocateExpression::initialize()
{
    Expression::initialize();
    if (m_pSizeExpression)
        addSubExpression(m_pSizeExpression);
    addSubExpression(m_pAddressExpression);
}

void DeallocateExpression::eval(ExecutionContext& a_Context) const
{
    void* pAddress = nullptr;
    m_pAddressExpression->load(a_Context, &pAddress);
    size_t count = 1;
    if (m_pSizeExpression)

        m_pSizeExpression->load(a_Context, &count);
    PHANTOM_ASSERT(count);
    m_pType->deallocate(pAddress, count);
    flush(a_Context);
}

DeallocateExpression* DeallocateExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<DeallocateExpression>(m_pType, m_pSizeExpression, m_pAddressExpression);
}

} // namespace lang
} // namespace phantom
