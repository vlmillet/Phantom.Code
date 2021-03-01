// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
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

class PHANTOM_EXPORT_PHANTOM_CODE BaseConstructorCallStatement : public Statement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor.
    ///
    /// \param  a_pDerivedClass                 The derived class.
    /// \param  a_pConstructorCallExpression    The constructor call expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    BaseConstructorCallStatement(Class* a_pDerivedClass, Expression* a_pConstructorCallExpression);

    void initialize();

    virtual BaseConstructorCallStatement* asBaseConstructorCallStatement() const
    {
        return (BaseConstructorCallStatement*)this;
    }

    void eval(ExecutionContext& a_Context) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constructor call expression associated with that statement.
    ///
    /// \return The constructor call expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getConstructorCallExpression() const { return m_pConstructorCallExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constructed element associated with that statement, if any set.
    ///
    /// \return The constructed element or null.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Class* getBaseClass() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constructed element associated with that statement, if any set.
    ///
    /// \return The constructed element or null.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Class* getDerivedClass() const { return m_pDerivedClass; }

protected:
    Expression* m_pConstructorCallExpression;
    Class*      m_pDerivedClass;
};

} // namespace lang
} // namespace phantom
