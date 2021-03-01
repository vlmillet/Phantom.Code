// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "DeleteExpression.h"

#include <phantom/typeof>
/* *********************************************** */
namespace phantom
{
namespace lang
{
DeleteExpression::DeleteExpression(Class* a_pClass, Expression* a_pExpression)
    : Expression(PHANTOM_TYPEOF(void)),
      m_pExpression(a_pExpression),
      m_pClass(a_pClass),
      m_pInitClass(a_pClass),
      m_bPhantom(true)
{
    PHANTOM_ASSERT(m_pExpression->isTemplateDependant() ||
                   m_pExpression->getValueType()->removeReference()->removeQualifiers()->asPointer());
    PHANTOM_ASSERT(m_pClass == nullptr || m_pClass->isTemplateDependant() || m_pClass->getDestructor() ||
                   a_pExpression->hasEvalConversionTo(m_pClass->addPointer()));
}

DeleteExpression::DeleteExpression(Expression* a_pExpression, bool a_bPhantom)
    : Expression(PHANTOM_TYPEOF(void)),
      m_pExpression(a_pExpression),
      m_pInitClass(a_bPhantom
                   ? nullptr
                   : a_pExpression->getValueType()->removeReference()->removeQualifiers()->removePointer()->asClass()),
      m_pClass(a_bPhantom
               ? nullptr
               : a_pExpression->getValueType()->removeReference()->removeQualifiers()->removePointer()->asClass()),
      m_bPhantom(a_bPhantom)
{
    PHANTOM_ASSERT(m_pExpression->isTemplateDependant() ||
                   m_pExpression->getValueType()->removeReference()->removeQualifiers()->asPointer());
}

void DeleteExpression::initialize()
{
    Expression::initialize();
    addReferencedElement(m_pClass);
    addSubExpression(m_pExpression);
}

void DeleteExpression::eval(ExecutionContext& a_Context) const
{
    void* pDeletedPointer;
    m_pExpression->load(a_Context, &pDeletedPointer);
    if (m_pClass)
        m_pClass->deleteInstance(pDeletedPointer);
    else
    {
        m_pClass->destroy(pDeletedPointer);
        m_pClass->deallocate(pDeletedPointer);
    }
    flush(a_Context);
}

DeleteExpression* DeleteExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return m_pInitClass ? a_pOwner->New<DeleteExpression>(m_pInitClass, m_pExpression->clone(a_pOwner))
                        : a_pOwner->New<DeleteExpression>(m_pExpression->clone(a_pOwner), m_bPhantom);
}

} // namespace lang
} // namespace phantom
