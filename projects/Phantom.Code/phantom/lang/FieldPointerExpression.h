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
/// \brief  Represents a Data member pointer expression.
/// \code{.cpp}
///            object->*data_member_pointer
/// \endcode
class PHANTOM_EXPORT_PHANTOM_CODE FieldPointerExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a Data member pointer expression from an
    ///         object expression and a Data member expression.
    ///
    /// \param [in,out] a_pFieldPointer    The Data member pointer type.
    /// \param [in,out] a_pObjectExpression         The object expression.
    /// \param [in,out] a_pFieldPointerExpression     The Data member pointer expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FieldPointerExpression(Expression* a_pObjectExpression, Expression* a_pFieldPointerExpression);
    void initialize();

    virtual FieldPointerExpression* asFieldPointerExpression() const { return (FieldPointerExpression*)this; }

    virtual void eval(ExecutionContext& a_Context) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the object expression.
    ///
    /// \return The object expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getObjectExpression() const { return m_pObjectExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member expression.
    ///
    /// \return The Data member pointer expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getFieldPointerExpression() const { return m_pFieldPointerExpression; }

    virtual FieldPointerExpression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    Expression*   m_pObjectExpression;
    Expression*   m_pFieldPointerExpression;
    FieldPointer* m_pFieldPointer;
};

} // namespace lang
} // namespace phantom
