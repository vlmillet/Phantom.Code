// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "WStringLiteralExpression.h"

#include <phantom/typeof>
/* *********************************************** */
namespace phantom
{
namespace lang
{
WStringLiteralExpression::WStringLiteralExpression(StringView a_Literal, const std::wstring& a_Value)
    : Expression(PHANTOM_TYPEOF(const wchar_t*)), m_Value(a_Value), m_Literal(a_Literal)
{
    m_Buffer = m_Value.c_str();
}

void WStringLiteralExpression::eval(ExecutionContext& a_Context) const
{
    *((const wchar_t**)a_Context.resultPointer()) = m_Buffer;
}

WStringLiteralExpression* WStringLiteralExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<WStringLiteralExpression>(m_Literal, m_Value);
}

} // namespace lang
} // namespace phantom
