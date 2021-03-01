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
/// \brief  Represents a member function pointer 'this' adjustment expression.

class PHANTOM_EXPORT_PHANTOM_CODE MethodPointerAdjustmentExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    MethodPointerAdjustmentExpression(MethodPointer* a_pOutputPointerType, Expression* a_pPointerExpression,
                                      ptrdiff_t a_iOffset);
    ~MethodPointerAdjustmentExpression();

    virtual MethodPointerAdjustmentExpression* asMethodPointerAdjustmentExpression() const
    {
        return (MethodPointerAdjustmentExpression*)this;
    }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext& a_Context) const { m_pPointerExpression->flush(a_Context); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the member function pointer expression.
    ///
    /// \return The member function pointer expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getPointerExpression() const { return m_pPointerExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the adjustment offset.
    ///
    /// \return The adjustment offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ptrdiff_t getOffset() const { return m_iOffset; }

    virtual MethodPointerAdjustmentExpression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    Expression* m_pPointerExpression;
    ptrdiff_t   m_iOffset;
};

} // namespace lang
} // namespace phantom
