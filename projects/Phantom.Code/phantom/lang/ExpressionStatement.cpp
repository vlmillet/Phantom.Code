// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "ExpressionStatement.h"

#include "Expression.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
ExpressionStatement::ExpressionStatement(Expression* a_pExpression) : m_pExpression(a_pExpression)
{
    PHANTOM_ASSERT(m_pExpression);
}

void ExpressionStatement::eval(ExecutionContext& a_Context) const
{
    a_Context.pushResultPointer(nullptr);
    m_pExpression->eval(a_Context);
    a_Context.popResultPointer();
    Statement::eval(a_Context);
}

void ExpressionStatement::flush(ExecutionContext& a_Context) const
{
    m_pExpression->flush(a_Context);
}

} // namespace lang
} // namespace phantom
