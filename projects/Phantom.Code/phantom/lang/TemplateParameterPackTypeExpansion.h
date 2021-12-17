// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "LanguageElementVisitorEx.h"

#include <phantom/lang/Type.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
class TemplateParameterPackExpansion;
/// \brief  Generic representation of a binary operation expression, mostly used for placeholding in
/// templates.
class PHANTOM_EXPORT_PHANTOM_CODE TemplateParameterPackTypeExpansion : public Type
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    TemplateParameterPackTypeExpansion(TemplateParameterPackExpansion* a_pExpansion);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the non type-explicit expansion.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateParameterPackExpansion* getExpansion() const { return m_pExpansion; }

    PlaceholderType* getPackArgument() const;

protected:
    TemplateParameterPackExpansion* m_pExpansion{};
};

} // namespace lang
} // namespace phantom
