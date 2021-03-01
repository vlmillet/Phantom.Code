// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "AllocateExpression.h"

#include <phantom/typeof>
/* *********************************************** */
namespace phantom
{
namespace lang
{
AllocateExpression::AllocateExpression(Type* a_pType, Expression* a_pSizeExpression, bool a_bStack)
    : Expression(a_pType->addPointer()), m_pType(a_pType), m_pSizeExpression(a_pSizeExpression), m_bStack(a_bStack)
{
    PHANTOM_ASSERT(m_pType);
    PHANTOM_ASSERT(m_pSizeExpression);
    PHANTOM_ASSERT(m_pType->asReference() == nullptr);
    PHANTOM_ASSERT(m_pSizeExpression->getValueType() == PHANTOM_TYPEOF(size_t));
}

AllocateExpression::AllocateExpression(Type* a_pType, bool a_bStack)
    : Expression(a_pType->addPointer()), m_pType(a_pType), m_pSizeExpression(nullptr), m_bStack(a_bStack)
{
    PHANTOM_ASSERT(m_pType);
    PHANTOM_ASSERT(m_pType->asReference() == nullptr);
}

void AllocateExpression::initialize()
{
    Expression::initialize();
    if (m_pSizeExpression)
        addSubExpression(m_pSizeExpression);
}

void AllocateExpression::eval(ExecutionContext& a_Context) const
{
    size_t count = 1;
    if (m_pSizeExpression)
    {
        m_pSizeExpression->load(a_Context, &count);
    }
    PHANTOM_ASSERT(count);
    *(void**)a_Context.resultPointer() = m_pType->allocate(count);
}

AllocateExpression* AllocateExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<AllocateExpression>(m_pType, m_pSizeExpression, m_bStack);
}

} // namespace lang
} // namespace phantom
