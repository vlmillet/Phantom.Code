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
/// \brief  Represents an access expression to a local variable.
class PHANTOM_EXPORT_PHANTOM_CODE LocalVariableExpression : public LValueExpression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a 'this' expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariableExpression(Type* a_pThisType);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs the expression with a given local variable.
    ///
    /// \param [in,out] a_pLocalVariable    If non-null, the local variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariableExpression(LocalVariable* a_pLocalVariable);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the associated local variable.
    ///
    /// \return The local variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void initialize();

    LocalVariable* getLocalVariable() const { return m_pLocalVariable; }

    virtual LocalVariableExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    virtual bool isPersistent() const override;

    virtual LanguageElement* removeExpression() const override;

protected:
    virtual void* internalEvalAddress(ExecutionContext& a_Context) const;

protected:
    LocalVariable* m_pLocalVariable;
};

} // namespace lang
} // namespace phantom
