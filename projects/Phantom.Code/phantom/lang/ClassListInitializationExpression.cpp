// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "ClassListInitializationExpression.h"

#include <phantom/lang/Field.h>

namespace phantom
{
namespace lang
{
ClassListInitializationExpression::ClassListInitializationExpression(Class* a_pClass, ExpressionsView a_Expressions)
    : Expression(a_pClass, a_pClass), m_Expressions(a_Expressions), m_pClass(a_pClass)
{
    PHANTOM_ASSERT(m_pClass->isListInitializable());
    PHANTOM_ASSERT((m_pClass->getFields().size() + m_pClass->getBaseClasses().size()) == a_Expressions.size());
    size_t c = 0;
    for (Class* pBaseClass : m_pClass->getBaseClasses())
    {
        Expression* pExp = m_Expressions[c];
        PHANTOM_ASSERT(pExp);
        PHANTOM_ASSERT(pExp->getEvalType()->isSame(pBaseClass));
        ++c;
    }
    for (auto pField : m_pClass->getFields())
    {
        Type*       pFieldType = pField->getValueType();
        Expression* pExp = m_Expressions[c];
        PHANTOM_ASSERT((pExp == nullptr && pFieldType->asClassType() == nullptr) ||
                       (pExp &&
                        (((pFieldType->asReference() && pExp->hasEvalConversionTo(pFieldType)) ||
                          pExp->hasEvalConversionTo(pFieldType)))));
        ++c;
    }
}

void ClassListInitializationExpression::initialize()
{
    Expression::initialize();
    for (auto& pE : m_Expressions)
    {
        if (pE)
        {
            addSubExpression(pE);
            pE->setTemporary(false);
        }
    }
    addDependencyOn(m_pClass);
}

void ClassListInitializationExpression::eval(ExecutionContext& a_Context) const
{
    byte* pResult = (byte*)a_Context.resultPointer();
    PHANTOM_ASSERT(pResult);
    memset(pResult, 0, m_pClass->getSize()); // zero init first
    size_t c = 0;
    for (auto baseClass : m_pClass->getBaseClasses())
    {
        if (Expression* pExp = m_Expressions[c++])
            pExp->eval(a_Context, baseClass + baseClass.offset);
    }
    for (auto pField : m_pClass->getFields())
    {
        if (Expression* pExp = m_Expressions[c++])
            pExp->eval(a_Context, pResult + pField->getOffset());
    }
}

ClassListInitializationExpression* ClassListInitializationExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<ClassListInitializationExpression>(m_pClass, m_Expressions);
}

} // namespace lang
} // namespace phantom
