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
class PHANTOM_EXPORT_PHANTOM_CODE CommaExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    CommaExpression(Expression* a_pLeftExpression, Expression* a_pRightExpression);

    void initialize();

    Expression* getLeftExpression() const { return m_pLeftExpression; }
    Expression* getRightExpression() const { return m_pRightExpression; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual CommaExpression* asCommaExpression() const { return (CommaExpression*)this; }

    virtual CommaExpression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    Expression* m_pLeftExpression = nullptr;
    Expression* m_pRightExpression = nullptr;
};

} // namespace lang
} // namespace phantom
