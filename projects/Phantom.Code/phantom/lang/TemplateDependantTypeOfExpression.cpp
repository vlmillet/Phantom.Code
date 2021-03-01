// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "TemplateDependantTypeOfExpression.h"

#include <phantom/typeof>

namespace phantom
{
namespace lang
{
TemplateDependantTypeOfExpression::TemplateDependantTypeOfExpression(Type* a_pType, Type* a_pMetaTypePtrCast)
    : Expression(a_pMetaTypePtrCast), m_pType(a_pType)
{
    PHANTOM_ASSERT(a_pType->isTemplateDependant());
    setTemplateDependant();
}

void TemplateDependantTypeOfExpression::eval(ExecutionContext&) const
{
    PHANTOM_UNREACHABLE();
}

Expression* TemplateDependantTypeOfExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<TemplateDependantTypeOfExpression>(m_pType, getValueType());
}

} // namespace lang
} // namespace phantom
