// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "LValueInitializationStatement.h"

#include "Block.h"
#include "Expression.h"
#include "phantom/lang/Type.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
LValueInitializationStatement::LValueInitializationStatement(Type*       a_pValueType,
                                                             Expression* a_pInitializationExpression)
    : m_pValueType(a_pValueType), m_pInitializationExpression(a_pInitializationExpression)
{
    PHANTOM_ASSERT(m_pValueType->isSame(a_pInitializationExpression->getValueType()));
}

Evaluable* LValueInitializationStatement::evaluateExpressionLifeTime(Expression* a_pExpression) const
{
    PHANTOM_ASSERT(m_pInitializationExpression == a_pExpression);
    if (getValueType()->asRValueReference() || getValueType()->asConstLValueReference()) /// const T& | T&&
    {
        return getEnclosingBlock();
    }
    return const_cast<LValueInitializationStatement*>(this);
}

} // namespace lang
} // namespace phantom
