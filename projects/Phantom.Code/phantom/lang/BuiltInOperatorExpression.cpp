// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "BuiltInOperatorExpression.h"

#include <ostream>
/* *********************************************** */
namespace phantom
{
namespace lang
{
BuiltInOperatorExpression::BuiltInOperatorExpression(BuiltInOperator* a_pOperator, Type* a_pResultType,
                                                     Expression* const*      a_InputArguments,
                                                     Expression* const*      a_EvalArguments,
                                                     BuiltInOperatorDelegate a_Delegate)
    : Expression(a_pResultType), m_pOperator(a_pOperator), m_Delegate(a_Delegate)
{
    PHANTOM_ASSERT(a_pOperator);
    memcpy(m_EvalArguments, a_EvalArguments, a_pOperator->getOperandCount() * sizeof(Expression*));
    memcpy(m_InputArguments, a_InputArguments, a_pOperator->getOperandCount() * sizeof(Expression*));
    for (size_t i = 0; i < a_pOperator->getOperandCount(); ++i)
    {
        PHANTOM_ASSERT(a_pOperator->getId() == Operator::Address ||
                       m_InputArguments[i]->getValueType()->removeReference()->removeQualifiers()->asClassType() ==
                       nullptr);
    }
    Operator op = a_pOperator->getId();
    m_bRequestFlush = a_InputArguments[0]->getValueType()->asReference() != nullptr &&
    (op >= Operator::Assignment && op <= Operator::AssignmentXOr) ||
    (op >= Operator::PreDecrement && op <= Operator::PostIncrement);
    PHANTOM_ASSERT(!m_Delegate.empty() || isTemplateDependant());
}

BuiltInOperatorExpression::BuiltInOperatorExpression(BuiltInOperator* a_pOperator, Type* a_pResultType,
                                                     Expression* const* a_InputArguments)
    : Expression(a_pResultType), m_pOperator(a_pOperator)
{
    PHANTOM_ASSERT(m_pOperator);
    PHANTOM_ASSERT(a_pResultType->isTemplateDependant());
    memcpy(m_InputArguments, a_InputArguments, a_pOperator->getOperandCount() * sizeof(Expression*));
}

void BuiltInOperatorExpression::initialize()
{
    Expression::initialize();
    addReferencedElement(m_pOperator);
    for (size_t i = 0; i < m_pOperator->getOperandCount(); ++i)
    {
        if (m_EvalArguments[i])
            addSubExpression(m_EvalArguments[i]);
    }
}

void BuiltInOperatorExpression::eval(ExecutionContext& a_Context) const
{
    void*  addresses[2];
    size_t i = m_pOperator->getOperandCount();
    switch (m_pOperator->getId())
    {
    case Operator::LogicalOr:
    {
        bool op0;
        m_EvalArguments[0]->load(a_Context, &op0);
        if (op0)
        {
            if (void* pResult = a_Context.resultPointer())
                *(bool*)pResult = true;
            break;
        }
        m_EvalArguments[1]->eval(a_Context);
    }
    break;
    case Operator::LogicalAnd:
    {
        bool op0;
        m_EvalArguments[0]->load(a_Context, &op0);
        if (!op0)
        {
            if (void* pResult = a_Context.resultPointer())
                *(bool*)pResult = false;
            break;
        }
        m_EvalArguments[1]->eval(a_Context);
    }
    break;
    case Operator::Address:
    {
        void* address = m_EvalArguments[0]->loadEffectiveAddress(a_Context);
        if (void* resultAddr = a_Context.resultPointer())
        {
            *(void**)resultAddr = address;
        }
    }
    break;

    default:
    {
        while (i--) // evaluate arguments from right to left
        {
            Expression* pArgument = m_EvalArguments[i];
            addresses[i] = pArgument->loadEffectiveAddress(a_Context);
        }
        if (a_Context.resultPointer())
        {
            m_Delegate(addresses, a_Context.resultPointer());
        }
        else
        {
            char buffer[16];
            m_Delegate(addresses, buffer);
        }
    }
    }
    if (m_bRequestFlush)
        m_EvalArguments[0]->flush(a_Context);
}

static Expression* FindClonedInputArgument(Expression* a_pCloned, Expression* a_pClone, Expression* a_pSearched)
{
    if (a_pCloned == a_pSearched)
        return a_pClone;
    for (auto p : a_pCloned->getElements())
    {
        if (Expression* pExp = p->asExpression()) // TODO : add getSubExpression[Count]() and use it
        {
            if (pExp = FindClonedInputArgument(pExp, pExp, a_pSearched))
                return pExp;
        }
    }
    return nullptr;
}

BuiltInOperatorExpression* BuiltInOperatorExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<BuiltInOperatorExpression>(m_pOperator, getValueType(), m_InputArguments, m_EvalArguments,
                                                    m_Delegate);
}

void BuiltInOperatorExpression::flush(ExecutionContext& a_Context) const
{
    size_t i = m_pOperator->getOperandCount();
    while (i--)
        m_EvalArguments[i]->flush(a_Context);
}

void* BuiltInOperatorExpression::evalStorage(ExecutionContext& a_Context) const
{
    Expression::eval(a_Context, m_Buffer);
    return m_Buffer;
}

Expression* BuiltInOperatorExpression::dereference(LanguageElement* a_pOwner)
{
    if (m_pOperator->getId() == Operator::Address && m_EvalArguments[0] == m_InputArguments[0])
    {
        m_InputArguments[0]->setOwner(nullptr); // attach to source (and let it die on source end)
        getSource()->Delete(this);
        return m_InputArguments[0];
    }
    return Expression::dereference(a_pOwner);
}

Expression* BuiltInOperatorExpression::address(LanguageElement* a_pOwner)
{
    if (m_pOperator->getId() == Operator::Dereference && m_EvalArguments[0] == m_InputArguments[0])
    {
        m_InputArguments[0]->setOwner(nullptr); // attach to source (and let it die on source end)
        getSource()->Delete(this);
        return m_InputArguments[0];
    }
    return Expression::address(a_pOwner);
}

LanguageElement* BuiltInOperatorExpression::removeExpression() const
{
    return m_InputArguments[0]->removeExpression();
}

void BuiltInOperatorExpression::dump(std::ostream& a_Out, int a_Lvl) const
{
    int l = a_Lvl;
    while (l--)
        a_Out << "  ";
    a_Out << rtti.metaClass->getName() << " (" << getValueType()->getDecoratedName() << ' ' << m_pOperator->getName()
          << ')' << std::endl;
    for (auto subExpr : getSubExpressions())
        subExpr->dump(a_Out, a_Lvl + 1);
}

phantom::lang::Evaluable* BuiltInOperatorExpression::evaluateExpressionLifeTime(Expression* a_pExpression) const
{
    // Logical boolean operands are exceptions in the case 'keeping temporary objects life time to outer-most
    // expressions', as they represent more a branch than a value
    // This is why we consider them always as "outer-most expressions"
    if (m_pOperator->getId() == Operator::LogicalAnd || m_pOperator->getId() == Operator::LogicalOr)
    {
        return a_pExpression;
    }
    return Expression::evaluateExpressionLifeTime(a_pExpression);
}

} // namespace lang
} // namespace phantom
