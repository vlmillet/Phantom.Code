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
class PHANTOM_EXPORT_PHANTOM_CODE InitializerListExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a class list initialization expression with a list of
    /// per-member/per-class initialization expression.
    ///
    /// \param  a_Expressions      The initializers expressions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    InitializerListExpression(InitializerListType* a_pType, ExpressionsView a_Expressions);
    InitializerListExpression(Class* a_pInitializerListClass, ExpressionsView a_Expressions);

    void initialize()
    {
        Expression::initialize();
        for (auto pExp : m_Expressions)
        {
            addSubExpression(pExp);
            pExp->setTemporary(false);
        }
    }
    void terminate() override;

    virtual InitializerListExpression* asInitializerListExpression() const { return (InitializerListExpression*)this; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual InitializerListExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the expressions associated contained in this initializer expression.
    ///
    /// \return The expressions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expressions const& getExpressions() const { return m_Expressions; }

    Type* getContentType() const;

    // InitializerListExpression* finalize(Type* a_pContentType, LanguageElement* a_pContextScope);

private:
    Expressions   m_Expressions;
    mutable byte* m_pEvalBuffer = nullptr;
};

} // namespace lang
} // namespace phantom
