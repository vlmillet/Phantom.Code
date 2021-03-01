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
/// \brief  Represents a class type list initialization expression (ex: MyClass{exp0, ..., expN}) .
class PHANTOM_EXPORT_PHANTOM_CODE ClassTypeListInitializationExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a class list initialization expression with a list of
    /// per-member/per-class initialization expression.
    ///
    /// \param  a_pConstructor      The default constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ClassTypeListInitializationExpression(ClassType* a_pClassType, ExpressionsView a_Expressions);

    void initialize();

    virtual ClassTypeListInitializationExpression* asClassTypeListInitializationExpression() const
    {
        return (ClassTypeListInitializationExpression*)this;
    }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual ClassTypeListInitializationExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constructor associated with this expression.
    ///
    /// \return The constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expressions const& getExpressions() const { return m_Expressions; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the class type associated with this expression.
    ///
    /// \return The class type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ClassType* getClassType() const { return m_pClassType; }

protected:
    ClassType*  m_pClassType;
    Expressions m_Expressions;
};

} // namespace lang
} // namespace phantom
