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
/// \brief  Represents a String literal expression.
/// \code{.cpp}
///     "I'm a String literal"
/// \endcode

class PHANTOM_EXPORT_PHANTOM_CODE StringLiteralExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a String literal expression from the original literal and its String
    /// value.
    ///
    /// \param  a_Literal The original literal.
    /// \param  a_Value The String value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringLiteralExpression(StringView a_Literal, StringView a_Value);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a String literal expression from the original literal only, using C++
    /// String literal format.
    ///
    /// \param  a_Literal The original literal.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringLiteralExpression(StringView a_Literal);

    PHANTOM_DTOR ~StringLiteralExpression(void) {}

    void* evalStorage(ExecutionContext& a_Context) const override;

    void eval(ExecutionContext& a_Context) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the String as a const char* pointer.
    ///
    /// \return The String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const char* const* getAddress() const { return &m_Buffer; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the String as a const char* pointer.
    ///
    /// \return The String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const char* getString() const { return m_Buffer; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the original literal String (including the "" and explicit escape characters).
    ///
    /// \return The literal.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getLiteral() const { return m_Literal; }

    bool hasStorage() const override { return true; }

    StringLiteralExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    bool isCompileTime() const override { return true; }

protected:
    String      m_Literal;
    String      m_Value;
    const char* m_Buffer;
};

} // namespace lang
} // namespace phantom
