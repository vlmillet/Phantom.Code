// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "ConstantExpression.h"

#include <phantom/lang/Constant.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
ConstantExpression::ConstantExpression(Constant* a_pConstant)
    : Expression(a_pConstant->getValueType()), m_pConstant(a_pConstant), m_pTempValue(nullptr)
{
    PHANTOM_ASSERT(m_pConstant);
}

void ConstantExpression::initialize()
{
    Expression::initialize();
    addReferencedElement(m_pConstant);
    if (!(isTemplateDependant()))
    {
        m_pTempValue = getValueType()->allocate();
        m_pConstant->getValue(m_pTempValue);
    }
}

void ConstantExpression::terminate()
{
    if (m_pTempValue)
    {
        getValueType()->deallocate(m_pTempValue);
        m_pTempValue = nullptr;
    }
    Expression::terminate();
}

void ConstantExpression::setValue(void const*) const
{
    PHANTOM_ASSERT_FORBIDDEN_CALL();
}

void ConstantExpression::eval(ExecutionContext& a_Context) const
{
    if (a_Context.resultPointer())
    {
        m_pConstant->getValue(a_Context.resultPointer());
    }
}

void* ConstantExpression::evalStorage(ExecutionContext&) const
{
    return m_pTempValue;
}

LanguageElement* ConstantExpression::hatchExpression()
{
    Constant* pConstant = m_pConstant;
    getSource()->Delete(this);
    return pConstant;
}

ConstantExpression* ConstantExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    // TODO : add clone function to Constant
    Constant* pConstant = m_pConstant;
    bool      bOwnsConstant = pConstant && pConstant->getOwner() == this;
    if (bOwnsConstant)
    {
        pConstant = pConstant->clone(a_pOwner);
    }
    return a_pOwner->New<ConstantExpression>(pConstant);
}

bool ConstantExpression::isPersistent() const
{
    return Expression::isPersistent() && (m_pConstant->getOwner() == this || m_pConstant->isNative());
}

LanguageElement* ConstantExpression::removeExpression() const
{
    return m_pConstant;
}

} // namespace lang
} // namespace phantom
