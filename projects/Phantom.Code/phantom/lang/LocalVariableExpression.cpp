// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "LocalVariableExpression.h"

#include <phantom/lang/LocalVariable.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
LocalVariableExpression::LocalVariableExpression(Type* a_pThisType)
    : LValueExpression(a_pThisType->addPointer()->addConst()), m_pLocalVariable(nullptr)
{
    PHANTOM_ASSERT(a_pThisType->removeQualifiers()->asClassType());
}

LocalVariableExpression::LocalVariableExpression(LocalVariable* a_pLocalVariable)
    : LValueExpression(a_pLocalVariable->getValueType()), m_pLocalVariable(a_pLocalVariable)
{
    PHANTOM_ASSERT(m_pLocalVariable);
}

void LocalVariableExpression::initialize()
{
    LValueExpression::initialize();
    if (m_pLocalVariable)
        addReferencedElement(m_pLocalVariable);
}

void* LocalVariableExpression::internalEvalAddress(ExecutionContext& a_Context) const
{
    PHANTOM_ASSERT(m_pLocalVariable == nullptr || m_pLocalVariable->getEvalFrameOffset() != -1);
    if (m_pLocalVariable == nullptr)
    {
        return a_Context.addressOfThis();
    }
    return a_Context.addressOf(m_pLocalVariable);
}

LocalVariableExpression* LocalVariableExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return m_pLocalVariable ? a_pOwner->New<LocalVariableExpression>(m_pLocalVariable)
                            : a_pOwner->New<LocalVariableExpression>(
                              getValueType()->removeReference()->removeConst()->removePointer()->removeConst());
}

bool LocalVariableExpression::isPersistent() const
{
    return Expression::isPersistent() && m_pLocalVariable->isNative();
}

LanguageElement* LocalVariableExpression::removeExpression() const
{
    return m_pLocalVariable;
}

} // namespace lang
} // namespace phantom
