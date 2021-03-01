// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "StaticAssert.h"

#include "Expression.h"

#include <phantom/typeof>

namespace phantom
{
namespace lang
{
StaticAssert::StaticAssert(Expression* a_pExpression, StringView a_Text)
    : LanguageElement(), m_pExpression(a_pExpression), m_Text(a_Text)
{
    PHANTOM_ASSERT(m_pExpression->isTemplateDependant() ||
                   m_pExpression->isCompileTime() &&
                   m_pExpression->getValueType()->removeReference()->removeQualifiers() == PHANTOM_TYPEOF(bool));
    PHANTOM_ASSERT(m_pExpression->getOwner() == nullptr);
    m_pExpression->setOwner(this);
}

} // namespace lang
} // namespace phantom
