// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Expression.h"
#include "Statement.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a Data member initialization statement
///         \code{.cpp}
///                     MyClass(int valueA) : a(valueA) {} // Data member 'a' initialized with
///                     'valueA'
///         \endcode

class PHANTOM_EXPORT_PHANTOM_CODE FieldInitializationStatement : public Statement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor.
    ///
    /// \param [in,out] a_pObjectExpression The object expression (equivalent to *this).
    /// \param [in,out] a_pField       The Data member.
    /// \param [in,out] a_pValueExpression  The value expression.
    /// \param [in,out] a_uiArrayIndex  (optional) The optional array index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FieldInitializationStatement(Field* a_pField, Expression* a_pValueExpression, size_t a_uiArrayIndex = 0);

    void initialize();

    virtual FieldInitializationStatement* asFieldInitializationStatement() const
    {
        return (FieldInitializationStatement*)this;
    }

    void eval(ExecutionContext& a_Context) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member associated with that statement.
    ///
    /// \return The Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* getField() const { return m_pField; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the initialization expression.
    ///
    /// \return The initialization expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getInitializationExpression() const { return m_pInitializationExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the array index for array offsetting.
    ///
    /// \return The array index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getArrayIndex() const { return m_uiArrayIndex; }

protected:
    Field*      m_pField;
    Expression* m_pInitializationExpression;
    size_t      m_uiArrayIndex;

private:
    size_t m_uiOffset;
};

} // namespace lang
} // namespace phantom
