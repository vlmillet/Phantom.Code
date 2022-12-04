// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "ConstructorCallExpression.h"

#include "CompiledSource.h"
#include "Semantic.h"

#include <phantom/lang/Constructor.h>
#include <phantom/lang/Module.h>
#include <phantom/lang/VirtualMethodTable.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
ConstructorCallExpression::ConstructorCallExpression(Constructor* a_pConstructor)
    : Expression(a_pConstructor->getOwnerClassType()), m_pConstructor(a_pConstructor)
{
    PHANTOM_ASSERT(!a_pConstructor->testModifiers(Modifier::Deleted));
}

ConstructorCallExpression::ConstructorCallExpression(Constructor* a_pConstructor, ExpressionsView a_Arguments)
    : Expression(a_pConstructor->getOwnerClassType()), m_pConstructor(a_pConstructor), m_Arguments(a_Arguments)
{
    PHANTOM_ASSERT(!a_pConstructor->testModifiers(Modifier::Deleted));
    PHANTOM_ASSERT(a_Arguments.size() <= m_pConstructor->getParameters().size());
    PHANTOM_ASSERT(a_Arguments.size() >= m_pConstructor->getRequiredArgumentCount());
    for (size_t i = 0; i < a_Arguments.size(); ++i)
    {
        PHANTOM_ASSERT(
        a_Arguments[i]->removeRValueReferenceExpression()->hasEvalConversionTo(m_pConstructor->getParameterType(i)) ||
        a_Arguments[i]->removeRValueReferenceExpression()->hasEvalConversionTo(m_pConstructor->getParameterType(i)) ||
        a_Arguments[i]->removeRValueReferenceExpression()->hasEvalConversionTo(
        m_pConstructor->getParameterType(i)->removeConstLValueReference()) ||
        a_Arguments[i]->removeRValueReferenceExpression()->hasEvalConversionTo(
        m_pConstructor->getParameterType(i)->removeRValueReference()));
    }
}

ConstructorCallExpression::ConstructorCallExpression(Constructor* a_pConstructor, Expression* a_pArgument)
    : Expression(a_pConstructor->getOwnerClassType()), m_pConstructor(a_pConstructor)
{
    m_Arguments.push_back(a_pArgument);
    PHANTOM_ASSERT(1 >= m_pConstructor->getRequiredArgumentCount());
    PHANTOM_ASSERT(
    m_pConstructor->getParameters().size() >= 1 &&
    (a_pArgument->hasEvalConversionTo(a_pConstructor->getParameterType(0)) ||
     a_pArgument->hasEvalConversionTo(a_pConstructor->getParameterType(0)->removeConstLValueReference())));
}

void ConstructorCallExpression::initialize()
{
    Expression::initialize();

    addReferencedElement(m_pConstructor);
    addDependencyOn(m_pConstructor);
    PHANTOM_ASSERT(m_Arguments.size() >= m_pConstructor->getSignature()->getRequiredArgumentCount() &&
                   m_Arguments.size() <= m_pConstructor->getParameters().size());

    while (m_Arguments.size() < m_pConstructor->getParameters().size())
    {
        auto p = m_pConstructor->getParameters()[m_Arguments.size()];
        m_Arguments.push_back(
        getSemantic()->convert(p->getDefaultArgumentExpression()->clone(this), p->getValueType(), m_pConstructor));
    }

    size_t i = m_Arguments.size();
    while (i--)
    {
        Expression* pArgument = m_Arguments[i];
        PHANTOM_ASSERT(pArgument);
        addSubExpression(m_Arguments[i]); // add elements in evaluation order
    }
    if (m_pConstructor->getOwner()->asClass()) /// if owner is a class, we need destruction so we
                                               /// mark the expression as temporary
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

void ConstructorCallExpression::eval(ExecutionContext& a_Context) const
{
    VoidPtrs addresses;
    evaluateArguments(a_Context, m_Arguments, addresses);
    PHANTOM_ASSERT(a_Context.resultPointer());

    // if native constructor, extract the native closures
    m_pConstructor->construct(a_Context.resultPointer(), addresses.empty() ? nullptr : addresses.data());
    if (m_pConstructor->isNative())
    {
        for (auto vtable : m_pConstructor->getOwnerClass()->getVirtualMethodTables())
        {
            vtable->extractNativeVTable(a_Context.resultPointer());
        }
    }
    flush(a_Context);
}

ConstructorCallExpression* ConstructorCallExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<ConstructorCallExpression>(m_pConstructor, m_Arguments);
}

void ConstructorCallExpression::flush(ExecutionContext& a_Context) const
{
    size_t i = m_Arguments.size();
    while (i--)
    {
        m_Arguments[i]->flush(a_Context);
    }
}

} // namespace lang
} // namespace phantom
