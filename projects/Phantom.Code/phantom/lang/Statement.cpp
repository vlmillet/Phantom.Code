// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Statement.h"

#include "Block.h"
#include "Expression.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
Statement::Statement(uint a_uiFlags) : Evaluable(a_uiFlags) {}

Statement::~Statement() {}

void Statement::eval(ExecutionContext& a_Context) const
{
    evalScopedDestructions(a_Context);
}

Block* Statement::getBlock() const
{
    if (Block* pBlock = getOwner()->asBlock())
        return pBlock;
    return nullptr;
}

Block* Statement::getRootBlock() const
{
    Block* pBlock = getBlock();
    return pBlock ? pBlock->getRootBlock() : asBlock();
}

Subroutine* Statement::getSubroutine() const
{
    Block* pBlock = getBlock();
    return pBlock ? pBlock->getSubroutine() : nullptr;
}

Statement* Statement::getNextStatement() const
{
    Block* pBlock = getBlock();
    if (pBlock == nullptr)
        return nullptr;
    return pBlock->getNextStatementOf((Statement*)this);
}

Evaluable* Statement::evaluateExpressionLifeTime(Expression* a_pExpression) const
{
    // this is the outer most expression
    return a_pExpression;
}

Statement* Statement::getNextNonBlockStatement() const // todo virtual in Block
{
    if (Block* pThisBlock = asBlock())
    {
        if (Statement* pNext = pThisBlock->getFirstNonBlockStatementCascade())
        {
            return pNext;
        }
    }
    Block* pBlock = getBlock();
    return pBlock ? pBlock->getNextNonBlockStatement((Statement*)this) : nullptr;
}

void Statement::onAttachedToBlock(Block* a_pBlock)
{
    for (auto pExp : getSubExpressions())
    {
        pExp->onAttachedToBlock(a_pBlock);
    }
}

} // namespace lang
} // namespace phantom
