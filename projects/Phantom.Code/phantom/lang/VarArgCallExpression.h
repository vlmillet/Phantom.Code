// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "CallExpression.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a subroutine call expression.
class PHANTOM_EXPORT_PHANTOM_CODE VarArgCallExpression : public CallExpression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a var arg call expression with a subroutine, an argument list and an
    /// optional special return type (which differs from subroutine return type).
    ///
    /// \param [in] a_pSubroutine The subroutine.
    /// \param [in] a_Arguments         The arguments.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VarArgCallExpression(Subroutine* a_pSubroutine, ExpressionsView a_Arguments);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a var arg call expression with a subroutine and a single argument.
    ///
    /// \param [in] a_pSubroutine   The subroutine.
    /// \param [in] a_pArgument     The argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VarArgCallExpression(Subroutine* a_pSubroutine, Expression* a_pArgument);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a call expression with a subroutine and no argument.
    ///
    /// \param [in] a_pSubroutine   The subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VarArgCallExpression(Subroutine* a_pSubroutine);

    ~VarArgCallExpression();

    virtual void eval(ExecutionContext& a_Context) const override;

private:
    virtual Expression* cloneImpl(LanguageElement* a_pOwner) const override;
};

} // namespace lang
} // namespace phantom
