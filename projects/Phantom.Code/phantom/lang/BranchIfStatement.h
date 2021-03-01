// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "BranchStatement.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a "branch if" instruction.
class PHANTOM_EXPORT_PHANTOM_CODE BranchIfStatement : public BranchStatement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor.
    ///
    /// \param [in,out] a_pExpression       If non-null, the test expression.
    /// \param [in,out] a_pLabel   If non-null, the label statement to jump to.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    BranchIfStatement(Expression* a_pExpression, Label* a_pLabel);

    void initialize();

    virtual BranchIfStatement* asBranchIfStatement() const { return (BranchIfStatement*)this; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext& a_Context) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the test expression.
    ///
    /// \return null if it fails, else the expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getExpression() const { return m_pExpression; }

protected:
    Expression* m_pExpression;
};

} // namespace lang
} // namespace phantom
