// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "TypeTraitExpression.h"

#include <phantom/typeof>

namespace phantom
{
namespace lang
{
TypeTraitExpression::TypeTraitExpression(ETypeTrait a_eTypeTrait, Types const& a_Types)
    : Expression(PHANTOM_TYPEOF(bool)), m_eTypeTrait(a_eTypeTrait), m_Types(a_Types)
{
    for (auto type : m_Types)
    {
        if (type->isTemplateDependant())
        {
            setTemplateDependant();
            break;
        }
    }
}

bool TypeTraitExpression::Resolve(ETypeTrait a_eTypeTrait, Types const& a_Types)
{
    switch (a_eTypeTrait)
    {
    case e_TypeTait_Undefined:
        break;
    case e_TypeTait_IsSame:
        PHANTOM_ASSERT(a_Types.size() == 2);
        return a_Types[0]->isSame(a_Types[1]);
        break;
    default:
        break;
    }
    return false;
}

void TypeTraitExpression::eval(ExecutionContext& a_Context) const
{
    if (a_Context.resultPointer() == nullptr)
        return;
    *(bool*)a_Context.resultPointer() = Resolve(m_eTypeTrait, m_Types);
}

Expression* TypeTraitExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<TypeTraitExpression>(m_eTypeTrait, m_Types);
}

} // namespace lang
} // namespace phantom
