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
/// \brief  Base class for l-value expressions.
class PHANTOM_EXPORT_PHANTOM_CODE LValueExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an l-value expression with a r-value type and modifiers.
    ///
    /// \param [in,out] a_pRValueType   The r-value type.
    /// \param  a_uiFlags               (optional) the flags.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LValueExpression(Type* a_pRValueType, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an l-value expression with a r-value type, an l-value type and modifiers.
    ///
    /// \param [in,out] a_pLValueType   The r-value type.
    /// \param [in,out] a_pRValueType   The l-value type.
    /// \param  modifiers               (optional) the flags.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LValueExpression(Type* a_pLValueType, Type* a_pRValueType, uint a_uiFlags = 0);

    PHANTOM_DTOR ~LValueExpression(void) {}

    virtual void eval(ExecutionContext& a_Context) const;

    Type* getRValueType() const { return m_pRValueType; }

protected:
    virtual void* internalEvalAddress(ExecutionContext& a_Context) const = 0;

protected:
    Type* m_pRValueType;
};

} // namespace lang
} // namespace phantom
