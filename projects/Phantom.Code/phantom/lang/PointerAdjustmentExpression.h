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
/// \brief  Represents a pointer adjustment expression.
/// \code {.cpp}
///     ex: ((char*)0x12345678) + 5
/// \endcode
class PHANTOM_EXPORT_PHANTOM_CODE PointerAdjustmentExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a pointer adjustment expression from an output address type (in our
    /// example above it's char*), a pointer expresion and a constant offset.
    ///
    /// \param [in,out] a_pOutputAddressType    The output address type.
    /// \param [in,out] a_pPointerExpression    The pointer expression.
    /// \param  a_iOffset                       The offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PointerAdjustmentExpression(Type* a_pOutputRefOrPtrType, Expression* a_pPointerExpression, ptrdiff_t a_iOffset);
    void initialize()
    {
        Expression::initialize();
        addSubExpression(m_pPointerExpression);
    }

    virtual PointerAdjustmentExpression* asPointerAdjustmentExpression() const
    {
        return (PointerAdjustmentExpression*)this;
    }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext& a_Context) const { m_pPointerExpression->flush(a_Context); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the pointer expression.
    ///
    /// \return null The pointer expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getPointerExpression() const { return m_pPointerExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the offset.
    ///
    /// \return The offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ptrdiff_t getOffset() const { return m_iOffset; }

    virtual PointerAdjustmentExpression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    Expression* m_pPointerExpression;
    ptrdiff_t   m_iOffset;
};

} // namespace lang
} // namespace phantom
