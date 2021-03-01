// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "UnaryPreOperationExpression.h"

#include <phantom/lang/Application.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
UnaryPreOperationExpression::UnaryPreOperationExpression(Type* a_pType, StringView a_strOperator,
                                                         Expression* a_pExpression)
    : Expression(a_pType), m_pOperandExpression(a_pExpression), m_strOperator(a_strOperator)
{
}

UnaryPreOperationExpression::UnaryPreOperationExpression(StringView a_strOperator, Expression* a_pExpression)
    : Expression(Application::Get()->getAuto()), m_pOperandExpression(a_pExpression), m_strOperator(a_strOperator)
{
}

void UnaryPreOperationExpression::initialize()
{
    Expression::initialize();
    addSubExpression(m_pOperandExpression);
}

void UnaryPreOperationExpression::flush(ExecutionContext& a_Context) const
{
    m_pOperandExpression->flush(a_Context);
}

UnaryPreOperationExpression* UnaryPreOperationExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<UnaryPreOperationExpression>(getValueType(), getOperatorName(), m_pOperandExpression);
}

} // namespace lang
} // namespace phantom
