// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "ControlStatement.h"

#include "Block.h"
#include "Expression.h"

#include <phantom/lang/Subroutine.h>
#include <phantom/typeof>
/* *********************************************** */
namespace phantom
{
namespace lang
{
void ControlStatement::eval(ExecutionContext& a_Context) const
{
    for (auto pSMT : getRAIIDestructions())
    {
        pSMT->eval(a_Context);
    }
    a_Context.setNextStatement(getTargetStatement());
}

void ControlStatement::finish()
{
    static_cast<Block*>(getOwner())->fetchControlScopedDestructions(m_RAIIDestructions, nullptr);
    for (auto pExp : m_RAIIDestructions)
    {
        PHANTOM_ASSERT(pExp->getOwner());
        addReferencedElement(pExp);
    }
}

void ControlStatement::stripRAIIDestructions()
{
    if (m_RAIIDestructions.empty())
        return;
    Statement* pNextStatement = getTargetStatement();
    if (pNextStatement)
    {
        Block* pNextBlock = pNextStatement->getBlock();
        auto   copy = m_RAIIDestructions;
        for (auto pExp : m_RAIIDestructions)
            removeReferencedElement(pExp);
        m_RAIIDestructions.clear();
        for (auto raiiDS : copy)
        {
            if (raiiDS->getEnclosingBlock() == pNextBlock)
            // once we meet a statement at the next block or up in the block hierarchy, we stop keeping
            // them (every statement in and outside next block are not affected by the branching)
            {
                break;
            }
            else if (pNextBlock->hasOwnerCascade(
                     raiiDS->getEnclosingBlock())) // we are moving the instruction pointer inside a block itself inside
                                                   // the RAII block => we skip the RAII calls
            {
                break;
            }
            m_RAIIDestructions.push_back(raiiDS);
            addReferencedElement(raiiDS);
        }
    }
}

} // namespace lang
} // namespace phantom
