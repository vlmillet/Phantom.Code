// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "LocalVariableInitializationStatement.h"

#include "Block.h"
#include "CallExpression.h"
#include "Expression.h"
#include "TemporaryObjectDestructionExpression.h"
#include "phantom/lang/Type.h"

#include <phantom/lang/LocalVariable.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
LocalVariableInitializationStatement::LocalVariableInitializationStatement(
LocalVariable* a_pLocalVariable, Expression* a_pInitializationExpression /*= nullptr*/)
    : m_pInitializationExpression(a_pInitializationExpression), m_pLocalVariable(a_pLocalVariable)
{
    PHANTOM_ASSERT(m_pInitializationExpression);
    PHANTOM_ASSERT(m_pLocalVariable);
    PHANTOM_ASSERT(m_pInitializationExpression->isTemplateDependant() ||
                   (m_pLocalVariable->getValueType()->removeAllQualifiers()->isSame(
                    m_pInitializationExpression->getValueType()->removeAllQualifiers()) ||
                    m_pLocalVariable->getValueType()->removeAllQualifiers()->isSame(
                    m_pInitializationExpression->getValueType()->removeRValueReference()->removeAllQualifiers())),
                   "initialization eval type does not match local variable type");
}

void LocalVariableInitializationStatement::initialize()
{
    Statement::initialize();
    addReferencedElement(m_pLocalVariable);
    addSubExpression(m_pInitializationExpression);
    m_pInitializationExpression->setTemporary(false);
    Expression* noRVS = m_pInitializationExpression->removeRValueStorageExpression();
    bool        noRVRTemp = noRVS->isTemporary();
    if (noRVRTemp)
    {
        noRVS->setTemporary(false);

        if (m_pLocalVariable->getValueType()->asRValueReference() ||
            m_pLocalVariable->getValueType()->asConstLValueReference())
        {
            if (ClassType* pCT = m_pLocalVariable->getValueType()->removeReference()->removeConst()->asClassType())
            {
                m_temporaryStorage = true;
            }
        }
    }
}

void LocalVariableInitializationStatement::eval(ExecutionContext& a_Context) const
{
    void* _where = a_Context.addressOf(m_pLocalVariable);
    m_pInitializationExpression->eval(a_Context, _where);
    evalScopedDestructions(a_Context);
}

Evaluable* LocalVariableInitializationStatement::evaluateExpressionLifeTime(Expression* a_pExpression) const
{
    PHANTOM_ASSERT(m_pInitializationExpression == a_pExpression);
    if (m_pLocalVariable->getValueType()->asRValueReference() ||
        m_pLocalVariable->getValueType()->asConstLValueReference()) /// const T& | T&&
    {
        return getEnclosingBlock();
    }
    return const_cast<LocalVariableInitializationStatement*>(this);
}

void LocalVariableInitializationStatement::onAttachedToBlock(Block* a_pBlock)
{
    if (m_temporaryStorage)
    {
        a_pBlock->addScopedDestruction(a_pBlock->New<TemporaryObjectDestructionExpression>(
        m_pInitializationExpression->removeRValueStorageExpression()));
    }
    Statement::onAttachedToBlock(a_pBlock);
}

} // namespace lang
} // namespace phantom
