// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "LanguageElementVisitorEx.h"

#include <phantom/lang/Expression.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
class TemplateParameterPackExpansion;
/// \brief  Generic representation of a binary operation expression, mostly used for placeholding in
/// templates.
class PHANTOM_EXPORT_PHANTOM_CODE TemplateParameterPackExpressionExpansion : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    TemplateParameterPackExpressionExpansion(TemplateParameterPackExpansion* a_pExpansion);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the non type-explicit expansion.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateParameterPackExpansion* getExpansion() const { return m_pExpansion; }

    Placeholder* getPackArgument() const;

    void eval(ExecutionContext&) const override { PHANTOM_ASSERT_FORBIDDEN_CALL(); }

private:
    Expression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    TemplateParameterPackExpansion* m_pExpansion{};
};

} // namespace lang
} // namespace phantom
