// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "InitializerListExpression.h"

#include "CppLite.h"
#include "OwnerGuard.h"
#include "Semantic.h"

#include <phantom/lang/InitializerListType.h>
#include <phantom/lang/Namespace.h>
#include <phantom/lang/Template.h>
#include <phantom/lang/TemplateSignature.h>
#include <phantom/lang/TemplateSpecialization.h>

namespace phantom
{
namespace lang
{
InitializerListExpression::InitializerListExpression(InitializerListType* a_pType, ExpressionsView a_Expressions)
    : Expression(a_pType), m_Expressions(a_Expressions), m_pEvalBuffer(nullptr)
{
}

InitializerListExpression::InitializerListExpression(Class* a_pInitializerListClass, ExpressionsView a_Expressions)
    : Expression(a_pInitializerListClass, a_pInitializerListClass), m_Expressions(a_Expressions)
{
    TemplateSpecialization* pTemplateSpecialization = a_pInitializerListClass->getTemplateSpecialization();
    PHANTOM_ASSERT(pTemplateSpecialization != nullptr &&
                   pTemplateSpecialization->getTemplate()->getQualifiedName() == "std::initializer_list");
    for (auto pExp : m_Expressions)
    {
        PHANTOM_ASSERT(
        pExp->getValueType()->isSame(static_cast<Type*>(pTemplateSpecialization->getArgument(0))->removeQualifiers()) ||
        pExp->getValueType()->removeRValueReference()->isSame(
        static_cast<Type*>(pTemplateSpecialization->getArgument(0))->removeQualifiers()));
    }
    setTemporary(getContentType()->asClass());
}

void InitializerListExpression::terminate()
{
    if (m_pEvalBuffer)
    {
        Type* pType = getContentType();

        byte*  pWriteBuffer = m_pEvalBuffer;
        size_t increment = pType->getSize();
        for (size_t i = 0; i < m_Expressions.size(); ++i)
        {
            pType->destroy(pWriteBuffer);
            pWriteBuffer += increment;
        }
        phantom::deallocate(m_pEvalBuffer);
    }
    Expression::terminate();
}

InitializerListExpression* InitializerListExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    if (Class* pClass = getValueType()->removeRValueReference()->asClass())
        return a_pOwner->New<InitializerListExpression>(pClass, m_Expressions);
    return a_pOwner->New<InitializerListExpression>(getValueType()->asInitializerListType(), m_Expressions);
}

Type* InitializerListExpression::getContentType() const
{
    return getValueType()->getTemplateSpecialization()
    ? static_cast<Type*>(getValueType()->getTemplateSpecialization()->getArgument(0))
    : nullptr;
}
//
// InitializerListExpression* InitializerListExpression::finalize(Type* a_pContentType, LanguageElement*
// a_pContextScope)
// {
//     PHANTOM_ASSERT(getValueType()->asInitializerListType());
//     PHANTOM_ASSERT(getOwner() == nullptr);
//     Template* pTemplate =
//     lang::Namespace::Global()->getOrCreateNamespace("std")->getTemplate("std::initializer_list"); LanguageElements
//     arguments; arguments.push_back(a_pContentType); LanguageElement* pResolvedTemplate =
//     CppLite::Get()->getDefaultSemantic()->instantiateTemplate(pTemplate, arguments, a_pContextScope);
//     OwnersGuard<Expressions> convExpressions;
//     for (auto expr : m_Expressions)
//     {
//         OwnerGuard<Expression> clone = expr->clone(getOwner());
//         Expression*            convExpr =
//         clone->convert(a_pContentType, CastKind::Implicit, UserDefinedFunctions::ImplicitsOnly, a_pContextScope);
//         if (convExpr == nullptr)
//             return nullptr;
//         convExpressions->push_back(convExpr);
//     }
//     PHANTOM_ASSERT(pResolvedTemplate && pResolvedTemplate->asClass());
//     Delete(this);
//     return getOwner()->New<InitializerListExpression>(pResolvedTemplate->asClass(), *convExpressions);
// }

void InitializerListExpression::eval(ExecutionContext& a_Context) const
{
    PHANTOM_ASSERT(getValueType()->asClass(), "non finalized initializer list expression cannot be evaluated");
    PHANTOM_ASSERT(getValueType()->getSize() == 2 * sizeof(void*), "wrong std::initializer_list class layout");

    // buffer interpreted as both pointers (begin/end)
    void** pBuffer = (void**)a_Context.resultPointer();

    if (m_Expressions.size())
    {
        Type* pContentType = getContentType();
        if (m_pEvalBuffer == nullptr)
        {
            size_t bufferSize = pContentType->getSize() * m_Expressions.size();
            if (bufferSize)
            {
                m_pEvalBuffer = (byte*)phantom::allocate(bufferSize, pContentType->getAlignment());
            }
        }

        byte*  pWriteBuffer = m_pEvalBuffer;
        size_t increment = getContentType()->getSize();
        // first evaluates to the internal buffer
        for (size_t i = 0; i < m_Expressions.size(); ++i)
        {
            m_Expressions[i]->eval(a_Context, pWriteBuffer);
            pWriteBuffer += increment;
        }
        pBuffer[0] = m_pEvalBuffer;
        pBuffer[1] = pWriteBuffer;
    }
    else // empty initializer list
    {
        pBuffer[0] = nullptr;
        pBuffer[1] = nullptr;
    }
}

} // namespace lang
} // namespace phantom
