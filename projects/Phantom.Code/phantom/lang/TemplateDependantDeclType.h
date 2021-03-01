// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "LanguageElementVisitorEx.h"
#include "lang.h"

#include <phantom/lang/Type.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Template dependant C++ decltype.
/// \code{.cpp}
///             decltype(T) // with T a template argument or template dependant element
/// \endcode
class PHANTOM_EXPORT_PHANTOM_CODE TemplateDependantDeclType : public Type
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    TemplateDependantDeclType(Expression* m_pExpression, Type* a_pAutoType = nullptr);

    virtual bool isA(Type*) const { return true; }

    virtual bool isCopyable() const { return true; }

    Type*       getAutoType() const { return m_pAutoType; }
    Expression* getExpression() const { return m_pExpression; }

protected:
    Expression* m_pExpression;
    Type*       m_pAutoType;
};

} // namespace lang
} // namespace phantom
