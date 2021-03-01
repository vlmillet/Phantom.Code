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
/// \brief  Represents a temporary object destruction. It's an handy expression for temporaries life
/// time control
class PHANTOM_EXPORT_PHANTOM_CODE TemporaryObjectDestructionExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a temporary object destruction expression.
    ///
    /// \param [in,out] a_pObjectExpression The expression whom storage will be destroyed.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemporaryObjectDestructionExpression(Expression* a_pExpression);

    void initialize()
    {
        Expression::initialize();
        addReferencedElement(m_pExpression);
    }

    virtual TemporaryObjectDestructionExpression* asTemporaryObjectDestructionExpression() const
    {
        return (TemporaryObjectDestructionExpression*)this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the object expression associated with this expression.
    ///
    /// \return The object expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getExpression() const { return m_pExpression; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext& a_Context) const { m_pExpression->flush(a_Context); }

    virtual TemporaryObjectDestructionExpression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    Expression* m_pExpression;
};

} // namespace lang
} // namespace phantom
