// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "LanguageElementVisitorEx.h"
#include "lang.h"

#include <phantom/lang/LanguageElement.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM_CODE StaticAssert : public LanguageElement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    StaticAssert(Expression* a_pExpression, StringView a_Text);
    PHANTOM_DTOR ~StaticAssert(void)
    {
    }

    virtual StaticAssert* asStaticAssert() const
    {
        return (StaticAssert*)this;
    }

    Expression* getExpression() const
    {
        return m_pExpression;
    }
    StringView getText() const
    {
        return m_Text;
    }

protected:
    Expression* m_pExpression = nullptr;
    String      m_Text;
};

} // namespace lang
} // namespace phantom
