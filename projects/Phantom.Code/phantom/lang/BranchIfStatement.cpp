// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "BranchIfStatement.h"

#include "Expression.h"

#include <phantom/typeof>
/* *********************************************** */
namespace phantom
{
namespace lang
{
BranchIfStatement::BranchIfStatement(Expression* a_pExpression, Label* a_pLabel)
    : BranchStatement(a_pLabel), m_pExpression(a_pExpression)
{
    PHANTOM_ASSERT(m_pExpression->getValueType() == PHANTOM_TYPEOF(bool) || m_pExpression->isTemplateDependant());
}

void BranchIfStatement::initialize()
{
    BranchStatement::initialize();
    addSubExpression(m_pExpression);
}

void BranchIfStatement::eval(ExecutionContext& a_Context) const
{
    bool value;
    m_pExpression->eval(a_Context, &value);
    evalScopedDestructions(a_Context);
    if (value)
    {
        ControlStatement::eval(a_Context);
    }
}

void BranchIfStatement::flush(ExecutionContext& a_Context) const
{
    m_pExpression->flush(a_Context);
}

} // namespace lang
} // namespace phantom
