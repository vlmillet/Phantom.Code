// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "MethodPointerCallExpression.h"

#include <phantom/lang/MethodPointer.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
MethodPointer* MethodPointerCallExpression_ExtractMethodPointer(Expression* a_pMemberExpression)
{
    Type* pType = a_pMemberExpression->getValueType()->removeReference()->removeConst();
    PHANTOM_ASSERT(pType->asMethodPointer());
    return static_cast<MethodPointer*>(pType);
}

MethodPointerCallExpression::MethodPointerCallExpression(Expression* a_pObjectExpression,
                                                         Expression* a_pMemberExpression, ExpressionsView a_Arguments)
    : Expression(MethodPointerCallExpression_ExtractMethodPointer(a_pMemberExpression)->getReturnType()),
      m_pMethodPointer(MethodPointerCallExpression_ExtractMethodPointer(a_pMemberExpression)),
      m_pObjectExpression(a_pObjectExpression),
      m_pMemberExpression(a_pMemberExpression),
      m_Arguments(a_Arguments)
{
    PHANTOM_ASSERT(m_pObjectExpression);
    ClassType* pClassType = m_pMethodPointer->getObjectType();
    PHANTOM_ASSERT(m_pObjectExpression->getValueType()->removeConstReference()->isA(pClassType));
    PHANTOM_ASSERT(m_pObjectExpression->getValueType() != nullptr && m_pObjectExpression->hasEffectiveAddress());
    PHANTOM_ASSERT(m_pMethodPointer->getParameterTypeCount() == m_Arguments.size());
    for (size_t i = 0; i < m_pMethodPointer->getParameterTypeCount(); ++i)
    {
        PHANTOM_ASSERT(m_Arguments[i]->hasEvalConversionTo(m_pMethodPointer->getParameterType(i)));
    }
    Type* pStorageType = storageType(m_pMethodPointer->getReturnType());
    if (pStorageType)
    {
        m_pReturnStorage = pStorageType->allocate();
    }

#pragma message(PHANTOM_TODO "factorize with CallExpressions && ConstructionCallExpression")
    // ----------------
    if (m_pMethodPointer->getReturnType()->removeAllQualifiers()->asClass())
    {
        setTemporary(true);
    }
    // ----------------
}

void MethodPointerCallExpression::initialize()
{
    Expression::initialize();
    addReferencedElement(m_pMethodPointer);
    addSubExpression(m_pObjectExpression);
    for (auto& pArg : m_Arguments)
    {
        addSubExpression(pArg);
        // any argument which is a raw class (passing a class by-value) will be destroyed inside the callee and must
        // not be marked as a temporary
        if (pArg->getValueType()->removeQualifiers()->asClassType())
        {
            pArg->setTemporary(false);
        }
    }
}

void MethodPointerCallExpression::terminate()
{
    _destroyReturnStorage();
}

void MethodPointerCallExpression::eval(ExecutionContext& a_Context) const
{
    VoidPtrs addresses;
    evaluateArguments(a_Context, m_Arguments, addresses);
    byte methodPointerBuffer[64];
    m_pMemberExpression->load(a_Context, methodPointerBuffer);
    if (m_pReturnStorage)
    {
        m_pMethodPointer->call(methodPointerBuffer, m_pObjectExpression->loadEffectiveAddress(a_Context),
                               addresses.data(), m_pReturnStorage);
        getValueType()->copy(a_Context.resultPointer(),
                             m_pReturnStorage); // then copy the return value to dest
    }
    else // void call
    {
        m_pMethodPointer->call(methodPointerBuffer, m_pObjectExpression->loadEffectiveAddress(a_Context),
                               addresses.data());
    }
}

void MethodPointerCallExpression::flush(ExecutionContext& a_Context) const
{
    for (auto it = m_Arguments.begin(); it != m_Arguments.end(); ++it)
    {
        (*it)->flush(a_Context);
    }
}

MethodPointerCallExpression* MethodPointerCallExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    Expressions clonedExpressions;
    for (auto it = m_Arguments.begin(); it != m_Arguments.end(); ++it)
    {
        clonedExpressions.push_back((*it)->clone(a_pOwner));
    }
    return a_pOwner->New<MethodPointerCallExpression>(m_pObjectExpression, m_pMemberExpression, m_Arguments);
}

void MethodPointerCallExpression::_destroyReturnStorage()
{
    if (m_pReturnStorage)
    {
        Type* pStorageType = storageType(m_pMethodPointer->getReturnType());
        if (pStorageType)
        {
            pStorageType->deallocate(m_pReturnStorage);
        }
        m_pReturnStorage = nullptr;
    }
}

} // namespace lang
} // namespace phantom
