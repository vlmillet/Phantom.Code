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
/// \brief  Represents a Data member pointer adjustment for polymorphism.

class PHANTOM_EXPORT_PHANTOM_CODE FieldPointerAdjustmentExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a Data member pointer adjustment from an output pointer type, a pointer
    /// expression and an adjustment offset.
    ///
    /// \param [in,out] a_pOutputPointerType    The output pointer type.
    /// \param [in,out] a_pPointerExpression    The pointer expression.
    /// \param  a_iOffset                       The adjustment offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FieldPointerAdjustmentExpression(Type* a_pOutputPointerType, Expression* a_pPointerExpression, ptrdiff_t a_iOffset);
    ~FieldPointerAdjustmentExpression();

    virtual FieldPointerAdjustmentExpression* asFieldPointerAdjustmentExpression() const
    {
        return (FieldPointerAdjustmentExpression*)this;
    }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext& a_Context) const { m_pPointerExpression->flush(a_Context); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the pointer to adjust expression.
    ///
    /// \return The pointer to adjust expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getPointerExpression() const { return m_pPointerExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the adjustment offset.
    ///
    /// \return The offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ptrdiff_t getOffset() const { return m_iOffset; }

    virtual FieldPointerAdjustmentExpression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    Expression* m_pPointerExpression;
    ptrdiff_t   m_iOffset;
};

} // namespace lang
} // namespace phantom
