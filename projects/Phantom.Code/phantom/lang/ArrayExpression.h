// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Expression.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents an array expression (ex: { 0, 2, 5, anInt } ) .
class PHANTOM_EXPORT_PHANTOM_CODE ArrayExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an array expression with an array type and expressions.
    ///
    /// \param  a_pArray    The array type matching the given values.
    /// \param  a_Values        The values of the new array
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ArrayExpression(Array* a_pArray, ExpressionsView a_Values);

    void initialize();

    virtual ArrayExpression* asArrayExpression() const { return (ArrayExpression*)this; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual ArrayExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the array values.
    ///
    /// \return The input expressions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionsView getValues() const { return m_Values; }

protected:
    Expressions m_Values;
};

} // namespace lang
} // namespace phantom
