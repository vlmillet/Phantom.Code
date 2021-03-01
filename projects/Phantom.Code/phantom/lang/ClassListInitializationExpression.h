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
/// \brief  Represents a class list initialization expression (ex: MyClass{{exp0, exp1}, exp0, ...,
/// expN}) .
class PHANTOM_EXPORT_PHANTOM_CODE ClassListInitializationExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a class list initialization expression with a list of
    /// per-member/per-class initialization expression.
    ///
    /// \param  a_pConstructor      The default constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ClassListInitializationExpression(Class* a_pClass, ExpressionsView a_Expressions);

    void initialize();

    virtual ClassListInitializationExpression* asClassListInitializationExpression() const
    {
        return (ClassListInitializationExpression*)this;
    }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual ClassListInitializationExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constructor associated with this expression.
    ///
    /// \return The constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expressions const& getExpressions() const { return m_Expressions; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the class associated with this expression.
    ///
    /// \return The class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Class* getClass() const { return m_pClass; }

protected:
    Class*      m_pClass;
    Expressions m_Expressions;
};

} // namespace lang
} // namespace phantom
