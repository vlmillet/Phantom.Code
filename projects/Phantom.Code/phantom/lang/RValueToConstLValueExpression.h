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
/// \brief Base class for scoped r-value expressions (keeping a value alive accross a scope).
/// \code{.cpp}
///     ex: const int& value = rvalue; // keeps the rvalue alive accross the scope
/// \endcode
class PHANTOM_EXPORT_PHANTOM_CODE RValueToConstLValueExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an r-value expression with a referenced expression.
    ///
    /// \param [in,out] a_pReferenceableExpression  If non-null, the referenceable expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    RValueToConstLValueExpression(Expression* a_pRValueExpression);

    void initialize()
    {
        Expression::initialize();
        addSubExpression(m_pRValueExpression);
    }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual RValueToConstLValueExpression* asRValueToConstLValueExpression() const
    {
        return const_cast<RValueToConstLValueExpression*>(this);
    }

    virtual void flush(ExecutionContext& a_Context) const { m_pRValueExpression->flush(a_Context); }

    Expression* getRValueExpression() const { return m_pRValueExpression; }

    virtual RValueToConstLValueExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    Expression* removeRValueToLValueExpression() override { return m_pRValueExpression; }

    Expression* removeRValueStorageExpression() override { return m_pRValueExpression; }

protected:
    Expression* m_pRValueExpression;
};

} // namespace lang
} // namespace phantom
