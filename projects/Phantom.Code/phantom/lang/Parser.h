// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "lang.h"

#include <phantom/lang/reflection.h>
#include <phantom/utils/SmallMap.h>
#include <phantom/utils/String.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
struct BuildSession;

/// \brief Base class for parsers which build an Evaluable Syntax Tree from a source code.
class PHANTOM_EXPORT_PHANTOM_CODE Parser : public Object
{
    friend class Source;
    friend class Language;
    friend class Application;

public:
    Parser(Language* a_pLanguage, Source* a_pSource, Message* a_pMessage);
    ~Parser();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Parse the given String to find an existing symbol.
    ///
    /// \param  a_strSymbol        The symbol String to parse.
    /// \param  a_pContextScope The context scope (none by default).
    /// \param  a_Modifiers     The modifiers filtering the search.
    ///
    /// \return null if parsing failed or symbol does not exist, else the existing symbol
    /// represented
    ///         by this String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Symbol* parseSymbolName(StringView a_strSymbol, LanguageElement* a_pContextScope, Modifiers a_Modifiers,
                                    uint a_uiFlags = 0) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Parse the given String to generate an evaluable expression object.
    ///
    /// \param  a_strExpression        The expression String to parse.
    /// \param  a_pContextScope The context scope.
    ///
    /// \return null if parsing failed, else the new expression object represented by this
    /// String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* parseExpression(StringView a_strExpression, LanguageElement* a_pContextScope) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the options controlling the parsing.
    ///
    /// \return The options.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Options& getOptions() const { return m_Options; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the option controlling the parsing.
    ///
    /// \param  a_strKey    The option key.
    /// \param  a_strValue  The option value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setOption(StringView a_strKey, StringView a_strValue) { m_Options[a_strKey] = a_strValue; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the current pass.
    ///
    /// \return The current pass.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    uint getPass() const { return m_uiPass; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the pass count.
    ///
    /// \return The pass count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    uint getPassCount() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the language associated with this parser.
    ///
    /// \return null if it fails, else the language.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Language* getLanguage() const { return m_pLanguage; }

    Message* getMessage() const { return m_pMessage; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an option matching the given key.
    ///
    /// \param  a_Key   The key.
    ///
    /// \return The option.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getOption(StringView a_Key) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the context source this parser is using to build its content.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Source* getSource() const { return m_pSource; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the semantic this parser is using to build its content.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Semantic* getSemantic() const { return m_pSemantic; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Begin parsing a source with multipass system. Reimplement to initialize your parser.
    ///
    /// \param  pass    The source to parse.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void begin(BuildSession&) {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  End parsing a source with multipass system. (begin(Source*) must have been
    /// previously
    ///         called. Reimplement to cleanup your parser
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void end() {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Runs a single parsing pass. Must be reimplemented to perform your own parsing.
    ///
    /// \param  pass The parsing pass to run.
    ///
    /// \return .
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual int parse(uint a_uiPass) = 0;

    int stepParse(uint a_uiPass);

private:
    Language* m_pLanguage = nullptr;
    Semantic* m_pSemantic = nullptr;
    Source*   m_pSource = nullptr;
    Message*  m_pMessage = nullptr;
    uint      m_uiPass = ~uint(0);
    Options   m_Options;
};

} // namespace lang
} // namespace phantom
