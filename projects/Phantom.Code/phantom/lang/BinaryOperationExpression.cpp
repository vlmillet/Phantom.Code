// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "BinaryOperationExpression.h"

#include <phantom/lang/Application.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
BinaryOperationExpression::BinaryOperationExpression(Type* a_pValueType, StringView a_strOperator,
                                                     Expression* a_pLeftExpression, Expression* a_pRightExpression)
    : Expression(a_pValueType),
      m_strOperator(a_strOperator),
      m_pLeftOperandExpression(a_pLeftExpression),
      m_pRightOperandExpression(a_pRightExpression)
{
}

BinaryOperationExpression::BinaryOperationExpression(StringView a_strOperator, Expression* a_pLeftExpression,
                                                     Expression* a_pRightExpression)
    : Expression(Application::Get()->getAuto()),
      m_strOperator(a_strOperator),
      m_pLeftOperandExpression(a_pLeftExpression),
      m_pRightOperandExpression(a_pRightExpression)
{
    PHANTOM_ASSERT(a_pLeftExpression->isTemplateDependant() || a_pRightExpression->isTemplateDependant());
}

void BinaryOperationExpression::initialize()
{
    Expression::initialize();
    addSubExpression(m_pLeftOperandExpression);
    addSubExpression(m_pRightOperandExpression);
}

BinaryOperationExpression* BinaryOperationExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<BinaryOperationExpression>(getValueType(), m_strOperator,
                                                    m_pLeftOperandExpression->clone(a_pOwner),
                                                    m_pRightOperandExpression->clone(a_pOwner));
}

} // namespace lang
} // namespace phantom
