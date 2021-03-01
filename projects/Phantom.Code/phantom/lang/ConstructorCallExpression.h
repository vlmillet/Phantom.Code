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
/// \brief  Represents a placement construction expression (ex: new (pMyPlace) MyClass(...)) .
class PHANTOM_EXPORT_PHANTOM_CODE ConstructorCallExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a placement construction expression with a default constructor.
    ///
    /// \param  a_pConstructor      The default constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ConstructorCallExpression(Constructor* a_pConstructor);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a placement construction expression with a constructor and call
    /// arguments.
    ///
    /// \param  a_pConstructor      The constructor.
    /// \param  a_Arguments         The arguments.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ConstructorCallExpression(Constructor* a_pConstructor, ExpressionsView a_Arguments);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a placement construction expression with a constructor and a single call
    /// argument.
    ///
    /// \param  a_pConstructor      The constructor.
    /// \param  a_Arguments         The argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ConstructorCallExpression(Constructor* a_pConstructor, Expression* a_pArgument);

    void initialize();

    virtual void eval(ExecutionContext& a_Context) const;

    virtual ConstructorCallExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constructor associated with this expression.
    ///
    /// \return The constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constructor* getConstructor() const { return m_pConstructor; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator pointing to the first argument.
    ///
    /// \return An iterator pointing to the first argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expressions::const_iterator beginArguments() const { return m_Arguments.begin(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator pointing to the end of arguments.
    ///
    /// \return An iterator pointing to the end of arguments.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expressions::const_iterator endArguments() const { return m_Arguments.end(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the arguments.
    ///
    /// \return The arguments.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionsView getArguments() const { return m_Arguments; }

    virtual void flush(ExecutionContext& a_Context) const;

private:
    Constructor* m_pConstructor;
    Expressions  m_Arguments;
};

} // namespace lang
} // namespace phantom
