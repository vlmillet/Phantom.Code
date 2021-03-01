// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "ReturnStatement.h"

#include "Block.h"
#include "Expression.h"

#include <phantom/lang/Subroutine.h>
#include <phantom/typeof>
/* *********************************************** */
namespace phantom
{
namespace lang
{
ReturnStatement::ReturnStatement()
    : m_pReturnExpression(nullptr), m_pConvertedReturnExpression(nullptr), m_pExpressionString(nullptr)
{
}

ReturnStatement::ReturnStatement(Expression* a_pExpression)
    : m_pReturnExpression(a_pExpression), m_pConvertedReturnExpression(nullptr), m_pExpressionString(nullptr)
{
}

void ReturnStatement::eval(ExecutionContext& a_Context) const
{
    if (m_pReturnExpression)
    {
        m_pReturnExpression->eval(a_Context, a_Context.getReturnAddress());
    }
    ControlStatement::eval(a_Context);
}

void ReturnStatement::checkValidity()
{
    PHANTOM_ASSERT(getSubroutine());
    PHANTOM_ASSERT(getSubroutine()->getReturnType() != PHANTOM_TYPEOF(void) || m_pReturnExpression == nullptr ||
                   m_pReturnExpression->getValueType() == PHANTOM_TYPEOF(void));
    PHANTOM_ASSERT(getSubroutine()->getReturnType() == PHANTOM_TYPEOF(void) || m_pReturnExpression);
    Type* pType = m_pReturnExpression->getValueType()->removeRValueReference();
    PHANTOM_ASSERT((getSubroutine()->isRVOCandidate() && pType == PHANTOM_TYPEOF(void)) ||
                   pType->removeConst()->isSame(getSubroutine()->getReturnType()->removeConst()));
}

void ReturnStatement::onAttachedToBlock(Block* a_pBlock)
{
    ControlStatement::onAttachedToBlock(a_pBlock);
#if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
    if (m_pReturnExpression)
    {
        checkValidity();
    }
#endif
    finish();
}

} // namespace lang
} // namespace phantom
