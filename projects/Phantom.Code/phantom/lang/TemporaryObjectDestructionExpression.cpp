// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "TemporaryObjectDestructionExpression.h"

#include <phantom/typeof>
/* *********************************************** */
namespace phantom
{
namespace lang
{
TemporaryObjectDestructionExpression::TemporaryObjectDestructionExpression(Expression* a_pExpression)
    : Expression(PHANTOM_TYPEOF(void)), m_pExpression(a_pExpression)
{
}

TemporaryObjectDestructionExpression* TemporaryObjectDestructionExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<TemporaryObjectDestructionExpression>(m_pExpression);
}

void TemporaryObjectDestructionExpression::eval(ExecutionContext& a_Context) const
{
    a_Context.releaseTemporaries(m_pExpression);
}

} // namespace lang
} // namespace phantom
