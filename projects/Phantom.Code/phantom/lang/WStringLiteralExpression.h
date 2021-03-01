// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Expression.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a wide String literal expression.
/// \code{.cpp}
///     L"I'm a wide String literal"
/// \endcode

class PHANTOM_EXPORT_PHANTOM_CODE WStringLiteralExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a wide String literal expression from the original literal and its wide
    /// String value.
    ///
    /// \param  a_Literal The original literal.
    /// \param  a_Value The wide String value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    WStringLiteralExpression(StringView a_Literal, const std::wstring& a_Value);
    PHANTOM_DTOR ~WStringLiteralExpression(void) {}

    virtual void* evalStorage(ExecutionContext&) const { return (void*)&m_Buffer; }

    virtual void eval(ExecutionContext& a_Context) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the wide String as a const wchar_t* pointer.
    ///
    /// \return The wide String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const wchar_t* getString() const { return m_Buffer; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the original literal String (including the "" and explicit escape characters).
    ///
    /// \return The literal.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getLiteral() const { return m_Literal; }

    virtual bool hasStorage() const { return true; }

    virtual WStringLiteralExpression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    String         m_Literal;
    std::wstring   m_Value;
    const wchar_t* m_Buffer;
};

} // namespace lang
} // namespace phantom
