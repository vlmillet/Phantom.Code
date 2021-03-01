// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Statement.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a C style label.

class PHANTOM_EXPORT_PHANTOM_CODE Label : public Statement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

    friend class Subroutine;
    friend class BranchStatement;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tags to identify the kind of label (useful for retro-translation).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ETag
    {
        e_Tag_None,
        e_Tag_Else,
        e_Tag_Endif,
        e_Tag_Break,
        e_Tag_Continue,
        e_Tag_Case,
        e_Tag_Default,
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an anonymous label with optional tag.
    ///
    /// \param  a_iTag  (optional) A tag to identify the kind of label.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Label(int a_iTag = e_Tag_None);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a named label with optional tag.
    ///
    /// \param  a_strLabelName  Name of the label.
    /// \param  a_iTag          (optional) A tag to identify the kind of label.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Label(StringView a_strLabelName, int a_iTag = e_Tag_None);

    /// \brief  Destructor.
    ~Label() = default;

    Label* asLabel() const override { return (Label*)this; }

    void eval(ExecutionContext&) const override;

    void flush(ExecutionContext&) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the label's name.
    ///
    /// \return The label's name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getLabelName() const { return m_LabelName; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the label's name.
    ///
    /// \param  a_strName   The label's name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setLabelName(StringView a_strName) { m_LabelName = a_strName; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the tag given to this label.
    ///
    /// \return The tag.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    int getTag() const { return m_iTag; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets a tag to this label.
    ///
    /// \return The tag.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setTag(int a_iTag) { m_iTag = a_iTag; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the index of this label inside its enclosing block.
    ///
    /// \return The index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getIndex() const { return m_uiIndex; }

protected:
    void onAttachedToBlock(Block* a_pBlock) override;
    void _registerBranchStatement(BranchStatement* a_pBr);

protected:
    String                        m_LabelName;
    size_t                        m_uiIndex;
    SmallVector<BranchStatement*> m_BranchStatements;
    int                           m_iTag;
};

} // namespace lang
} // namespace phantom
