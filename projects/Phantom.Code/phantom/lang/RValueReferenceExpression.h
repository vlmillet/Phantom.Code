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
/// \brief Creates an RValue reference expression from a class type expression.
/// \code{.cpp}
///     ex: A -> A&&; // keeps the rvalue alive accross the scope
/// \endcode
class PHANTOM_EXPORT_PHANTOM_CODE RValueReferenceExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an r-value expression with a referenced expression.
    ///
    /// \param [in,out] a_pReferenceableExpression  If non-null, the referenceable expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    RValueReferenceExpression(Expression* a_pRValueExpression);

    void initialize()
    {
        Expression::initialize();
        addSubExpression(m_pRValueExpression);
    }

    void eval(ExecutionContext& a_Context) const override;

    void flush(ExecutionContext& a_Context) const override { m_pRValueExpression->flush(a_Context); }

    Expression* getRValueExpression() const { return m_pRValueExpression; }

    RValueReferenceExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    void load(ExecutionContext& a_Context) const override { m_pRValueExpression->eval(a_Context); }

    Expression* load(LanguageElement* a_pOwner) override;

    Expression* loadRValueReferenceExpression() override { return load(nullptr); }

    Expression* removeRValueReferenceExpression() override { return m_pRValueExpression; }

    Expression* removeRValueToLValueExpression() override { return m_pRValueExpression; }

    LanguageElement* hatchExpression() override;

    LanguageElement* removeExpression() const override { return m_pRValueExpression->removeExpression(); }

protected:
    Expression* m_pRValueExpression;
};

} // namespace lang
} // namespace phantom
