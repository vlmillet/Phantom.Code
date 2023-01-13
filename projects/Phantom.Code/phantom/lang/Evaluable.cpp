// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "Evaluable.h"

#include "BuildSource.h"
#include "BuildSystem.h"
#include "CppLite.h"
#include "Expression.h"
#include "Interpreter.h"

#include <ostream>
#include <phantom/typeof>
/* *********************************************** */
namespace phantom
{
namespace lang
{
Evaluable::Evaluable(uint a_uiFlags /*= 0*/) : LanguageElement(a_uiFlags), m_pEvalType(PHANTOM_TYPEOF(void)) {}

Evaluable::Evaluable(Type* a_pEvalType, uint a_uiFlags /*= 0*/) : LanguageElement(a_uiFlags), m_pEvalType(a_pEvalType)
{
    PHANTOM_ASSERT(m_pEvalType);
}

Evaluable::~Evaluable()
{
    if (m_pScopedDestructions)
        delete_<Expressions>(m_pScopedDestructions);
}

BuildSource* Evaluable::getCompiledSource() const
{
    Source* pSource = getSource();
    return pSource ? BuildSystem::Get()->getCompiledSource(pSource) : nullptr;
}

Semantic* Evaluable::getSemantic() const
{
    BuildSource* pCSource = getCompiledSource();
    return pCSource ? pCSource->getSemantic() : CppLite::Get()->getDefaultSemantic();
}

Language* Evaluable::getLanguage() const
{
    BuildSource* pCSource = getCompiledSource();
    return pCSource ? pCSource->getLanguage() : nullptr;
}

void Evaluable::addSubExpression(Expression*& a_prExpression)
{
    PHANTOM_ASSERT(a_prExpression);
    if (a_prExpression->getOwner() || a_prExpression->getSource() != getSource())
    {
        a_prExpression = a_prExpression->clone(getSource());
    }
    a_prExpression->setOwner(this);
    m_SubExpressions.push_back(static_cast<Expression*>(a_prExpression));
}

void Evaluable::removeSubExpression(Expression* a_pExp)
{
    a_pExp->setOwner(nullptr);
    m_SubExpressions.erase_unsorted(std::find(m_SubExpressions.begin(), m_SubExpressions.end(), a_pExp));
}

void Evaluable::eval() const
{
    InterpreterContext context;
    // if evaluable is a non-void evaluable, we provide a placeholder buffer to receive the
    // evaluated value
    bool  nonVoid = (m_pEvalType != PHANTOM_TYPEOF(void));
    void* addr = nullptr;
    if (nonVoid)
    {
        context.pushResultPointer(addr = alloca(m_pEvalType->getSize()));
        m_pEvalType->construct(addr);
    }
    eval(context);
    {
        context.releaseTemporaries(const_cast<Evaluable*>(this));
    }
    if (nonVoid)
    {
        m_pEvalType->destroy(addr);
        context.popResultPointer();
    }
}

void Evaluable::eval(void* a_pResultAddress) const
{
    InterpreterContext context;
    PHANTOM_ASSERT(m_pEvalType != PHANTOM_TYPEOF(void), "evaluable must return a value");
    // if expression expects to return a value, we provide a placeholder buffer to receive it
    context.pushResultPointer(a_pResultAddress);
    eval(context);
    {
        context.releaseTemporaries(const_cast<Evaluable*>(this));
    }
    PHANTOM_ASSERT(context.resultPointer() == a_pResultAddress);
    context.popResultPointer();
}

Evaluable* Evaluable::evaluateExpressionLifeTime(Expression*) const
{
    return evaluateLifeTime();
}

void Evaluable::evaluateArguments(ExecutionContext& a_Context, ExpressionsView a_Arguments, VoidPtrs& addresses) const
{
    addresses.resize(a_Arguments.size());
    size_t i = a_Arguments.size();
    while (i--) // evaluate arguments from right to left
    {
        Expression* pArgument = a_Arguments[i];
        addresses[i] = pArgument->loadEffectiveAddress(a_Context);
    }
}

void Evaluable::pushTempDestruction(ExecutionContext& a_Context, Type* a_pType, void* a_pBuffer) const
{
    if (getOwner() && getOwner()->asEvaluable())
    {
        static_cast<Evaluable*>(getOwner())->pushTempDestruction(a_Context, a_pType, a_pBuffer);
    }
    else
    {
        a_Context.pushTempDestruction(const_cast<Evaluable*>(this), a_pType, a_pBuffer);
    }
}

void Evaluable::dump(std::ostream& a_Out, int a_Lvl) const
{
    int l = a_Lvl;
    while (l--)
        a_Out << "  ";
    a_Out << rtti.metaClass->getName() << std::endl;
    for (auto subExpr : m_SubExpressions)
        subExpr->dump(a_Out, a_Lvl + 1);
}

void Evaluable::flush(ExecutionContext&) const {}

void Evaluable::evalScopedDestructions(ExecutionContext& a_Context) const
{
    if (m_pScopedDestructions)
    {
        for (auto it = m_pScopedDestructions->begin(); it != m_pScopedDestructions->end(); ++it)
        {
            (*it)->eval(a_Context);
        }
    }
}

void Evaluable::addScopedDestruction(Expression* a_pExpression)
{
    if (m_pScopedDestructions == nullptr)
    {
        m_pScopedDestructions = new_<Expressions>();
    }
    m_pScopedDestructions->insert(m_pScopedDestructions->begin(), a_pExpression);
    addSubExpression(a_pExpression);
}

bool Evaluable::hasStorageCascade() const
{
    if (hasStorage())
        return true;
    for (auto exp : m_SubExpressions)
    {
        if (exp->hasStorageCascade())
            return true;
    }
    return false;
}

namespace
{
Expressions empty_expr;
}

ExpressionsView Evaluable::getSubExpressions() const
{
    return m_SubExpressions;
}

Expressions const& Evaluable::getScopedDestructions() const
{
    return m_pScopedDestructions ? *m_pScopedDestructions : empty_expr;
}

} // namespace lang
} // namespace phantom
