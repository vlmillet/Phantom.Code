// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "Label.h"

#include "Block.h"
#include "BranchStatement.h"
#include "BuildSource.h"
#include "BuildSystem.h"

#include <phantom/lang/Subroutine.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
Label::Label(int a_iTag /*= 0*/) : m_uiIndex(0xffffffff), m_iTag(a_iTag) {}

Label::Label(StringView a_strLabelName, int a_iTag /*= 0*/)
    : m_LabelName(a_strLabelName), m_uiIndex(0xffffffff), m_iTag(a_iTag)
{
}

void Label::eval(ExecutionContext&) const
{
    /*bypass*/
}

void Label::flush(ExecutionContext&) const {}

void Label::onAttachedToBlock(Block* a_pBlock)
{
    Statement::onAttachedToBlock(a_pBlock);
    PHANTOM_ASSERT(getSubroutine()); // ASSERT_DEBUG
    for (auto pBranch : m_BranchStatements)
    {
        if (pBranch->getOwner())
        {
            pBranch->stripRAIIDestructions();
        }
    }
}

void Label::_registerBranchStatement(BranchStatement* a_pBr)
{
    m_BranchStatements.push_back(a_pBr);
}

} // namespace lang
} // namespace phantom
