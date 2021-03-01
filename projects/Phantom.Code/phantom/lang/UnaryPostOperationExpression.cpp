// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "UnaryPostOperationExpression.h"

#include <phantom/lang/Application.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
UnaryPostOperationExpression::UnaryPostOperationExpression(Type* a_pType, StringView a_strOperator,
                                                           Expression* a_pExpression)
    : Expression(a_pType), m_pOperandExpression(a_pExpression), m_strOperator(a_strOperator)
{
}

UnaryPostOperationExpression::UnaryPostOperationExpression(StringView a_strOperator, Expression* a_pExpression)
    : Expression(Application::Get()->getAuto()), m_pOperandExpression(a_pExpression), m_strOperator(a_strOperator)
{
}

void UnaryPostOperationExpression::flush(ExecutionContext& a_Context) const
{
    m_pOperandExpression->flush(a_Context);
}

UnaryPostOperationExpression* UnaryPostOperationExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<UnaryPostOperationExpression>(getValueType(), m_strOperator,
                                                       m_pOperandExpression->clone(a_pOwner));
}

} // namespace lang
} // namespace phantom
