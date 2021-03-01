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
/// \brief  Represents a property access expression.
class PHANTOM_EXPORT_PHANTOM_CODE PropertyExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a property expression from a left-hand-side expression and a property.
    ///
    /// \param a_pObjectExpression   The object expression.
    /// \param a_pProperty          The property.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PropertyExpression(Expression* a_pObjectExpression, Property* a_pProperty);

    void initialize();
    void terminate() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the object expression.
    ///
    /// \return The object expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getObjectExpression() const { return m_pObjectExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the property associated with this expression.
    ///
    /// \return The property.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Property* getProperty() const { return m_pProperty; }

    virtual void flush(ExecutionContext& a_Context) const;

    virtual bool isPersistent() const;

    virtual PropertyExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    virtual LanguageElement* removeExpression() const override;

    virtual void eval(ExecutionContext& a_Context) const override;

private:
    Expression* m_pObjectExpression;
    Property*   m_pProperty;
    void*       m_pBuffer;
};

} // namespace lang
} // namespace phantom
