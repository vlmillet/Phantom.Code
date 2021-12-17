// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "LValueExpression.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a global variable access expression.
class PHANTOM_EXPORT_PHANTOM_CODE VariableExpression : public LValueExpression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a variable expression with given variable and optional adopted child
    /// expression.
    ///
    /// \param  a_pVariable The variable.
    /// \param  a_pAdoptedChildExpression  (optional) The adopted child expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VariableExpression(Variable* a_pVariable);

    void initialize();

    virtual VariableExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the variable associated with this variable expression.
    ///
    /// \return The variable associated with this variable expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Variable* getVariable() const { return m_pVariable; }

    LanguageElement* removeExpression() const override;

protected:
    virtual void* internalEvalAddress(ExecutionContext& a_Context) const;

protected:
    Variable* m_pVariable;
};

} // namespace lang
} // namespace phantom
