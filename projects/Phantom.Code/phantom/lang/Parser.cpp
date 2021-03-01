// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "Parser.h"

#include "CompiledSource.h"
#include "Language.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
static String emptyOption;

Parser::Parser(Language* a_pLanguage, Source* a_pSource, Message* a_pMessage)
    : m_pLanguage(a_pLanguage),
      m_pSemantic(a_pLanguage->createSemantic(a_pSource, a_pMessage)),
      m_pSource(a_pSource),
      m_pMessage(a_pMessage)
{
}

Parser::~Parser()
{
    m_pLanguage->destroySemantic(m_pSemantic);
}

StringView Parser::getOption(StringView a_Key) const
{
    auto found = m_Options.find(a_Key);
    return found == m_Options.end() ? emptyOption : found->second;
}

int Parser::stepParse(uint a_uiPass)
{
    if (int(m_uiPass) >= int(a_uiPass))
        return 0;
    PHANTOM_ASSERT(m_uiPass + 1 == a_uiPass, "a parse step has been missed");
    ++m_uiPass;
    return parse(m_uiPass);
}

uint Parser::getPassCount() const
{
    return m_pLanguage->getParserPassCount();
}

} // namespace lang
} // namespace phantom
