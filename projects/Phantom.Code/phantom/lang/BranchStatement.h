// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "ControlStatement.h"
/* *********************************************** */

namespace phantom
{
namespace lang
{
class Label;
/// \brief  Represents a "branch" instruction.
class PHANTOM_EXPORT_PHANTOM_CODE BranchStatement : public ControlStatement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

    friend class Label;

public:
    virtual BranchStatement* asBranchStatement() const { return (BranchStatement*)this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor.
    ///
    /// \param [in,out] a_pLabel   If non-null, the label statement to jump to.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    BranchStatement(Label* a_pLabel);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the label statement.
    ///
    /// \return null if it fails, else the label statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Label* getLabel() const { return m_pLabel; }

protected:
    Statement* getTargetStatement() const override;
    void       onAttachedToBlock(Block* a_pBlock) override;

private:
    Label* m_pLabel;
};

} // namespace lang
} // namespace phantom
