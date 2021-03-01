// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "BranchStatement.h"

#include "Expression.h"
#include "Label.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
void BranchStatement::onAttachedToBlock(Block* a_pBlock)
{
    finish();
    Statement::onAttachedToBlock(a_pBlock);
}

BranchStatement::BranchStatement(Label* a_pLabel)
{
    PHANTOM_ASSERT(a_pLabel);
    m_pLabel = a_pLabel;
    m_pLabel->_registerBranchStatement(this);
}

Statement* BranchStatement::getTargetStatement() const
{
    return m_pLabel;
}

} // namespace lang
} // namespace phantom
