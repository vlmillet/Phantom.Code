// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "LanguageElementVisitorEx.h"

#include <phantom/lang/LanguageElement.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class Placeholder;
/// \brief  Generic representation of a binary operation expression, mostly used for placeholding in
/// templates.
class PHANTOM_EXPORT_PHANTOM_CODE TemplateParameterPackExpansion : public LanguageElement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    TemplateParameterPackExpansion(LanguageElement* a_pExpandedElement, Placeholder* a_pPackArgument);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the expanded expression.
    ///
    /// \return The expanded expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* getExpandedElement() const { return m_pExpandedElement; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the pack parameter.
    ///
    /// \return The pack parameter.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Placeholder* getPackArgument() const { return m_pPackArgument; }

protected:
    LanguageElement* m_pExpandedElement{};
    Placeholder*     m_pPackArgument{};
};

} // namespace lang
} // namespace phantom
