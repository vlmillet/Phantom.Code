// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "FieldInitializationStatement.h"

#include "Block.h"
#include "Expression.h"
#include "TemporaryObjectDestructionExpression.h"
#include "phantom/lang/Field.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
FieldInitializationStatement::FieldInitializationStatement(Field* a_pField, Expression* a_pRightExpression,
                                                           size_t a_uiArrayIndex)
    : m_pField(a_pField),
      m_pInitializationExpression(a_pRightExpression),
      m_uiArrayIndex(a_uiArrayIndex),
      m_uiOffset(a_pField->getOffset() + a_uiArrayIndex * a_pField->getValueType()->getSize())
{
    PHANTOM_ASSERT(m_pInitializationExpression);
    PHANTOM_ASSERT(m_pField);
}

void FieldInitializationStatement::initialize()
{
    Statement::initialize();
    addReferencedElement(m_pField);
    addSubExpression(m_pInitializationExpression);
    m_pInitializationExpression->setTemporary(false);
    m_pInitializationExpression->removeRValueStorageExpression()->setTemporary(false);

    PHANTOM_ASSERT(isTemplateDependant() || m_uiArrayIndex == 0 ||
                   m_pField->getValueType()->removeQualifiers()->asArray());
    PHANTOM_ASSERT(isTemplateDependant() ||
                   m_pInitializationExpression->getValueType()->removeAllQualifiers()->isSame(
                   m_pField->getValueType()->removeAllQualifiers()));
}

void FieldInitializationStatement::eval(ExecutionContext& a_Context) const
{
    byte* _this = *(byte**)a_Context.addressOfThis();
    m_pInitializationExpression->eval(a_Context, _this + m_uiOffset);
    evalScopedDestructions(a_Context);
}

} // namespace lang
} // namespace phantom
