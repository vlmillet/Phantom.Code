// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "ClassTypeListInitializationExpression.h"

#include <phantom/lang/Field.h>

namespace phantom
{
namespace lang
{
ClassTypeListInitializationExpression::ClassTypeListInitializationExpression(ClassType*      a_pClassType,
                                                                             ExpressionsView a_Expressions)
    : Expression(a_pClassType, a_pClassType), m_pClassType(a_pClassType), m_Expressions(a_Expressions)
{
    PHANTOM_ASSERT(a_pClassType->isListInitializable());
    PHANTOM_ASSERT(a_pClassType->getFields().size() == a_Expressions.size());
    for (size_t i = 0; i < m_Expressions.size(); ++i)
    {
        Field*      pField = a_pClassType->getField(i);
        Type*       pFieldType = pField->getValueType();
        Expression* pExp = m_Expressions[i];
        PHANTOM_ASSERT((pExp == nullptr && pFieldType->asClassType() == nullptr) ||
                       (pExp &&
                        ((pFieldType->asReference() && pExp->hasEvalConversionTo(pFieldType)) ||
                         pExp->hasEvalConversionTo(pFieldType))));
    }
}

void ClassTypeListInitializationExpression::initialize()
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
    addDependencyOn(m_pClassType);
}

void ClassTypeListInitializationExpression::eval(ExecutionContext& a_Context) const
{
    byte* pResult = (byte*)a_Context.resultPointer();
    PHANTOM_ASSERT(pResult);
    memset(pResult, 0, m_pClassType->getSize()); // zero init first
    for (size_t i = 0; i < m_Expressions.size(); ++i)
    {
        Field* pField = m_pClassType->getField(i);
        if (Expression* pExp = m_Expressions[i])
            pExp->eval(a_Context, pResult + pField->getOffset());
    }
}

ClassTypeListInitializationExpression* ClassTypeListInitializationExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<ClassTypeListInitializationExpression>(m_pClassType, m_Expressions);
}

} // namespace lang
} // namespace phantom
