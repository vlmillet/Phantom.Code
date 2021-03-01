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
/// \brief  Represents a new expression (ex: New<MyClass>(...)) .
class PHANTOM_EXPORT_PHANTOM_CODE NewExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a new expression with a given constructor call expression.
    ///
    /// \param  a_pConstructorCallExpression    The constructor call expression.
    /// \param  a_Count                         The count of instance created
    /// \param  a_bPhantom                      Create an equivalent to phantom PHANTOM_NEW()
    /// instead of new
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    NewExpression(Expression* a_pInitializationExpression, Expression* a_pCountExpression = nullptr);

    void initialize()
    {
        Expression::initialize();
        addSubExpression(m_pInitializationExpression);
        m_pInitializationExpression->setTemporary(false);
        if (m_pCountExpression)
        {
            addSubExpression(m_pCountExpression);
        }
    }

    virtual NewExpression* asNewExpression() const { return (NewExpression*)this; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual NewExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the initialization expression associated with this expression (generally an
    /// instance of ConstructorCallExpression or InitializerListExpression).
    ///
    /// \return The initialization expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getInitializationExpression() const { return m_pInitializationExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the count of instance created.
    ///
    /// \return The count of instance created.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getCountExpression() const { return m_pCountExpression; }

protected:
    Expression* m_pInitializationExpression;
    Expression* m_pCountExpression;
};

} // namespace lang
} // namespace phantom
