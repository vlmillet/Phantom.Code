// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "BaseConstructorCallStatement.h"

#include "ConstructorCallExpression.h"

#include <phantom/lang/Constructor.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
BaseConstructorCallStatement::BaseConstructorCallStatement(Class*      a_pDerivedClass,
                                                           Expression* a_pConstructorCallExpression)
    : m_pConstructorCallExpression(a_pConstructorCallExpression), m_pDerivedClass(a_pDerivedClass)
{
    PHANTOM_ASSERT(m_pDerivedClass);
    PHANTOM_ASSERT(m_pConstructorCallExpression);
    PHANTOM_ASSERT(m_pConstructorCallExpression->as<ConstructorCallExpression>() != nullptr ||
                   m_pConstructorCallExpression->isTemplateDependant());
}

void BaseConstructorCallStatement::initialize()
{
    Statement::initialize();
    addReferencedElement(m_pDerivedClass);
    addSubExpression(m_pConstructorCallExpression);
    m_pConstructorCallExpression->setTemporary(false);
}

void BaseConstructorCallStatement::eval(ExecutionContext& a_Context) const
{
    byte*                      _this = *(byte**)a_Context.addressOfThis();
    ConstructorCallExpression* pCtorExp = m_pConstructorCallExpression->as<ConstructorCallExpression>();
    PHANTOM_ASSERT(pCtorExp);
    m_pConstructorCallExpression->eval(
    a_Context, _this + m_pDerivedClass->getBaseClassOffset(pCtorExp->getConstructor()->getOwnerClass()));
}

Class* BaseConstructorCallStatement::getBaseClass() const
{
    return m_pConstructorCallExpression->getValueType()->removeReference()->removeConst()->asClass();
}

} // namespace lang
} // namespace phantom
