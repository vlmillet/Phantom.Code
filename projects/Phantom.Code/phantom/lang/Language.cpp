// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Language.h"

#include <fstream>
#include <phantom/lang/Application.h>
#include <phantom/lang/Module.h>
#include <phantom/lang/Package.h>
#include <phantom/lang/Plugin.h>
#ifndef __DOXYGEN__
#    include <phantom/utils/StringUtil.h>
#endif
#include "Compiler.h"
#include "Expression.h"
#include "conversions.h"
#include "phantom/lang/SourceStream.h"

#include <phantom/utils/Path.h>
#include <stdarg.h>
#include <system_error>
/* *********************************************** */
namespace phantom
{
namespace lang
{
static Languages s_Languages;
static Language* s_pDefaultLanguage;

Language::Language(StringView a_strName, StringView a_strExt, uint a_uiParserPassCount)
    : m_strName(a_strName), m_SourceFileExtension(a_strExt), m_uiParserPassCount(a_uiParserPassCount)
{
    PHANTOM_ASSERT(a_uiParserPassCount > 0);
}

Language::~Language(void) {}

Language* Language::GetDefault()
{
    return s_pDefaultLanguage;
}

Language* Language::GetLanguage(StringView a_Name)
{
    for (auto pLang : s_Languages)
        if (pLang->getName() == a_Name)
            return pLang;
    return nullptr;
}

Languages const& Language::GetLanguages()
{
    return s_Languages;
}

Language* Language::GetFromFileExtension(StringView a_Extension)
{
    for (auto it = s_Languages.begin(); it != s_Languages.end(); ++it)
    {
        Language* pLanguage = (*it);
        if (pLanguage->getSourceFileExtension() == a_Extension)
            return pLanguage;
    }
    return nullptr;
}

bool Conversion::isIdentity() const
{
    return input->removeQualifiers()->isSame(output->removeQualifiers());
}

bool Conversion::isCanonical() const
{
    return isIdentity();
}

Expression* Language::expressionf(const char* a_Format, LanguageElement* a_pContextScope, ...)
{
    va_list args;
    va_start(args, a_Format);
    char buffer[4096];
    int  r = vsnprintf(buffer, 4096, a_Format, args);
    va_end(args);
    if (r < 0)
        return nullptr;
    return expression(buffer, a_pContextScope);
}

Expression* Language::expressionf(const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    char buffer[4096];
    int  r = vsnprintf(buffer, 4096, a_Format, args);
    va_end(args);
    if (r < 0)
        return nullptr;
    return expression(buffer, nullptr);
}

void Language::release(Expression* a_pExpression)
{
    if (a_pExpression)
        Application::Get()->Delete(a_pExpression);
}

Expression* Language::parseExpression(StringView a_strExpression, LanguageElement* a_pContextScope /*= nullptr*/)
{
    return expression(a_strExpression, a_pContextScope);
}

Symbol* Language::parseSymbol(StringView a_strSymbol, LanguageElement* a_pContextScope /*= nullptr*/,
                              Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    return findSymbol(a_strSymbol, a_pContextScope, a_Modifiers, a_uiFlags);
}

Type* Language::findType(StringView a_strType, LanguageElement* a_pContextScope /*= nullptr*/,
                         Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    Symbol* pSymbol = findSymbol(a_strType, a_pContextScope, a_Modifiers, a_uiFlags);
    return (pSymbol) ? pSymbol->toType() : nullptr;
}

Class* Language::findClass(StringView a_strClass, LanguageElement* a_pContextScope /*= nullptr*/,
                           Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    Type* pClass = findType(a_strClass, a_pContextScope, a_Modifiers, a_uiFlags);
    return (pClass) ? pClass->asClass() : nullptr;
}

Function* Language::findFunction(StringView a_strFunction, LanguageElement* a_pContextScope /*= nullptr*/,
                                 Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    Symbol* pFunction = findSymbol(a_strFunction, a_pContextScope, a_Modifiers, a_uiFlags);
    return (pFunction) ? pFunction->asFunction() : nullptr;
}

bool Language::setOption(StringView a_OptionName, Variant const& a_Value)
{
    auto found = m_Options.find(a_OptionName);
    if (found == m_Options.end())
        return false;
    if (!found->second.type()->isSame(a_Value.type()))
        return false;
    found->second = a_Value;
    return true;
}

void Language::addOption(StringView a_OptionName, Variant const& a_DefaultValue)
{
    PHANTOM_ASSERT(m_Options.find(a_OptionName) == m_Options.end());
    m_Options[a_OptionName] = a_DefaultValue;
}

Variant const& Language::getOption(StringView a_OptionName) const
{
    static Variant null;
    auto           found = m_Options.find(a_OptionName);
    if (found == m_Options.end())
        return null;
    return found->second;
}

void Language::SetDefault(Language* a_pLanguage)
{
    s_pDefaultLanguage = a_pLanguage;
}

int ConversionResults::compare(const ConversionResults& other) const
{
    PHANTOM_ASSERT(other.size() == size());
    bool thisWin = true;
    bool otherWin = true;
    for (size_t i = 0; i < content.size(); ++i)
    {
        Conversion* L = content[i];
        Conversion* R = other.content[i];
        int         result;
        if (R == 0 || L == 0)
        {
            result = R == 0 ? L == 0 ? 0 : 1 : L == 0 ? -1 : 0;
        }
        else
        {
            result = L->compare(*R);
            PHANTOM_ASSERT(R->compare(*L) == -result); /// compare() coherence test
        }
        if (result > 0 && otherWin)
            otherWin = false;
        else if (result < 0 && thisWin)
            thisWin = false;
    }
    return thisWin - otherWin;
}

} // namespace lang
} // namespace phantom
