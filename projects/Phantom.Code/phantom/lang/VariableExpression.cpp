// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "VariableExpression.h"

#include <phantom/lang/Variable.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
VariableExpression::VariableExpression(Variable* a_pVariable)
    : LValueExpression(a_pVariable->getValueType()), m_pVariable(a_pVariable)
{
}

void VariableExpression::initialize()
{
    Expression::initialize();
    addReferencedElement(m_pVariable);
}

void* VariableExpression::internalEvalAddress(ExecutionContext&) const
{
    return m_pVariable->getAddress();
}

VariableExpression* VariableExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<VariableExpression>(m_pVariable);
}

} // namespace lang
} // namespace phantom
