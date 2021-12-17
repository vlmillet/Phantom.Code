// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "CallExpression.h"

#include "Semantic.h"

#include <ostream>
#include <phantom/lang/LValueReference.h>
#include <phantom/lang/Method.h>
#include <phantom/lang/Pointer.h>
#include <phantom/lang/RValueReference.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
CallExpression::CallExpression(Subroutine* a_pSubroutine, ExpressionsView a_Arguments)
    : Expression(a_pSubroutine->getReturnType()), m_pSubroutine(a_pSubroutine), m_Arguments(a_Arguments)
{
}

CallExpression::CallExpression(Subroutine* a_pSubroutine, Expression* a_pArgument)
    : CallExpression(a_pSubroutine, Expressions{a_pArgument})
{
}

CallExpression::CallExpression(Subroutine* a_pSubroutine) : CallExpression(a_pSubroutine, Expressions{}) {}

void CallExpression::initialize()
{
    Expression::initialize();

    PHANTOM_ASSERT(m_pSubroutine);

    addDependencyOn(m_pSubroutine);
    addReferencedElement(m_pSubroutine);
    for (auto pArg : m_Arguments)
    {
        addSubExpression(pArg);
    }
    bool bThis = (m_pSubroutine->asMethod() != nullptr);
    PHANTOM_ASSERT(m_Arguments.size() == (m_pSubroutine->getParameters().size() + bThis));
    // m_TempValues.resize(m_Arguments.size(), nullptr);

    PHANTOM_ASSERT(
    !bThis ||
    (m_Arguments.size() &&
     m_Arguments[0]->hasEvalConversionTo(static_cast<Method*>(m_pSubroutine)->getOwnerClassType()->makePointer())));
    for (size_t i = 0; i < m_pSubroutine->getParameters().size(); ++i)
    {
        PHANTOM_ASSERT(m_Arguments[i + bThis]->removeRValueReferenceExpression()->hasEvalConversionTo(
                       m_pSubroutine->getParameterType(i)) ||
                       m_Arguments[i + bThis]->removeRValueReferenceExpression()->hasEvalConversionTo(
                       m_pSubroutine->getParameterType(i)->removeConstLValueReference()) ||
                       m_Arguments[i + bThis]->removeRValueReferenceExpression()->hasEvalConversionTo(
                       m_pSubroutine->getParameterType(i)->removeRValueReference()));
    }
    if (m_pSubroutine->getReturnType()->removeAllQualifiers()->asClass())
    {
        setTemporary(true);
    }
    for (auto pArg : m_Arguments)
    {
        // any argument which is a raw class (passing a class by-value) will be destroyed inside the callee and must not
        // be marked as a temporary
        if (pArg->getValueType()->removeQualifiers()->asClassType())
        {
            pArg->setTemporary(false);
        }
    }
}

void CallExpression::terminate()
{
    Expression::terminate();
}

bool CallExpression::transformArgument(Semantic* a_pSemantic, size_t i, Functor<Expression*(Expression*)> a_Transform)
{
    PHANTOM_ASSERT(i < m_Arguments.size() && m_Arguments[i]);
    Expression* pOld = m_Arguments[i];
    removeSubExpression(pOld);
    Expression* pNew = a_Transform(pOld);
    if (pNew)
    {
        bool  bThis = (m_pSubroutine->asMethod() != nullptr);
        Type* pDestType;
        if (bThis && i == 0)
        {
            pDestType = static_cast<Method*>(m_pSubroutine)->getOwnerClassType()->makePointer();
        }
        else
        {
            pDestType = m_pSubroutine->getParameterType(i - bThis);
        }
        if (pNew->getValueType()->removeRValueReference() != pDestType)
        {
            m_Arguments[i] = a_pSemantic->convert(pNew, pDestType, this);
            if (m_Arguments[i] == nullptr)
            {
                goto cancel;
            }
        }
        else
        {
            m_Arguments[i] = pNew;
        }
        PHANTOM_ASSERT(m_Arguments[0]->removeRValueReferenceExpression()->hasEvalConversionTo(pDestType) ||
                       m_Arguments[i]->removeRValueReferenceExpression()->hasEvalConversionTo(
                       pDestType->removeConstLValueReference()));
        return true;
    }
cancel:
    if (pNew)
        Delete(pNew);
    m_Arguments[i] = pOld;
    addSubExpression(pOld);
    return false;
}

void CallExpression::eval(ExecutionContext& a_Context) const
{
    VoidPtrs addresses;
    evaluateArguments(a_Context, m_Arguments, addresses);
    if (a_Context.resultPointer() != nullptr)
    {
        m_pSubroutine->placementCall(a_Context, addresses.empty() ? nullptr : (addresses.data())); // first call
    }
    else // void or non provided result pointer
    {
        if (m_pSubroutine->getReturnType() != PHANTOM_TYPEOF(void))
        {
            auto  pType = m_pSubroutine->getReturnType()->removeQualifiers();
            void* temp = alloca(pType->getSize());
            a_Context.pushResultPointer(temp);
            m_pSubroutine->placementCall(a_Context, addresses.empty() ? nullptr : (addresses.data()));
            a_Context.popResultPointer();
        }
        else
        {
            m_pSubroutine->call(addresses.empty() ? nullptr : (addresses.data()));
        }
    }
    flush(a_Context);
}

void CallExpression::flush(ExecutionContext& a_Context) const
{
    size_t i = m_Arguments.size();
    while (i--)
    {
        m_Arguments[i]->flush(a_Context);
    }
}

void CallExpression::setFinal()
{
    addFlags(PHANTOM_R_FLAG_MANDATORY);
}

LanguageElement* CallExpression::hatchExpression()
{
    Subroutine* pSubroutine = m_pSubroutine;
    // setOwner(nullptr);
    return pSubroutine;
}

Expression* CallExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    Expressions clonedExpressions;
    for (auto it = m_Arguments.begin(); it != m_Arguments.end(); ++it)
    {
        clonedExpressions.push_back((*it)->clone(a_pOwner));
    }
    CallExpression* exp = a_pOwner->New<CallExpression>(m_pSubroutine, clonedExpressions);
    if (isFinal())
        exp->setFinal();
    if (isImplicit())
        exp->setImplicit();
    return exp;
}

LanguageElement* CallExpression::removeExpression() const
{
    return m_pSubroutine;
}

bool CallExpression::isPersistent() const
{
    return Expression::isPersistent() && m_pSubroutine->isNative();
}

void CallExpression::dump(std::ostream& a_Out, int a_Lvl) const
{
    int l = a_Lvl;
    while (l--)
        a_Out << "  ";
    a_Out << rtti.metaClass->getName() << " (" << getValueType()->getDecoratedName() << ' '
          << m_pSubroutine->getDecoratedName() << ')' << std::endl;
    for (auto subExpr : getSubExpressions())
        subExpr->dump(a_Out, a_Lvl + 1);
}

bool CallExpression::isFinal() const
{
    return testFlags(PHANTOM_R_FLAG_MANDATORY);
}

} // namespace lang
} // namespace phantom
