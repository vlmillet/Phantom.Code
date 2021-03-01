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
/// \brief  Template dependant expression.
/// \code{.cpp}
///             ex : std::is_abstract<t_Ty>::value
/// \endcode
class PHANTOM_EXPORT_PHANTOM_CODE TemplateDependantExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    TemplateDependantExpression(TemplateDependantElement* a_pElement);

    virtual void eval(ExecutionContext&) const { PHANTOM_ASSERT_NO_IMPL(); }

    virtual TemplateDependantExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    TemplateDependantElement* getTemplateDependantElement() const { return m_pElement; }

protected:
    TemplateDependantElement* m_pElement;
};

} // namespace lang
} // namespace phantom
