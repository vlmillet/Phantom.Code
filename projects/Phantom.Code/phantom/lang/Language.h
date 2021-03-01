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
#include <phantom/utils/Variant.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a generic programming language.

class PHANTOM_EXPORT_PHANTOM_CODE Language : public Object
{
    friend class Compiler;
    friend class CompiledSource;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a language with an associated source file extension, parser pass count
    /// and translator pass count.
    ///
    /// \param  a_strExt                 The source file extension.
    /// \param  a_uiParserPassCount      The number of pass needed to perform a complete parsing.
    /// translation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Language(StringView a_strName, StringView a_strExt, uint a_uiParserPassCount);
    ~Language(void);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default language associated with this application. (C++ is the default one
    /// if not changed)
    ///
    /// \return null if it fails, else the default language.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static Language* GetDefault();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default language associated with this application. (C++ is the default one)
    ///
    /// \param [in,out] a_pLanguage If non-null, the value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static void SetDefault(Language* a_pLanguage);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find the language matching the given name.
    ///
    /// \param  a_strLanguageName The language name.
    ///
    /// \return null if it fails, else the language matching the given name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static Language* GetLanguage(StringView a_Name);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get all the languages registered in this application.
    ///
    /// \return a list of languages.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static Languages const& GetLanguages();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get a language from its file extension.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static Language* GetFromFileExtension(StringView a_Extension);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the name of this language.
    ///
    /// \return The name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getName() const { return m_strName; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Check if the given symbol name is well-formed.
    ///
    /// \param  a_strSymbol     The symbol name.
    ///
    /// \return true if well-formed, false if ill-formed.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool checkSymbolSyntax(StringView a_strSymbol) const = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Parse the given String to find an existing symbol, skipping incomplete or invalid
    /// symbols.
    ///
    /// \param  a_strSymbol     The symbol String to parse.
    /// \param  a_pContextScope (optional) The context scope (none by default).
    /// \param  a_Modifiers     (optional) The modifiers filtering the search (none by default).
    /// \param  a_uiFlags       (optional) The flags filtering the search (none by default).
    ///
    /// \return null if parsing failed or symbol does not exist, else the existing symbol described
    ///         by this String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Symbol* findSymbol(StringView a_strSymbol, LanguageElement* a_pContextScope = nullptr,
                               Modifiers a_Modifiers = 0, uint a_uiFlags = 0) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Parse the given String to find existing symbols from a base name, skipping
    /// incomplete or invalid symbols.
    ///
    /// \param  a_strSymbolsName The symbol base name to parse.
    /// \param  a_Symbols        The result symbols .
    /// \param  a_pContextScope (optional) The context scope (none by default).
    /// \param  a_Modifiers     (optional) The modifiers filtering the search (none by default).
    /// \param  a_uiFlags       (optional) The flags filtering the search (none by default).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void findSymbols(StringView a_strSymbolsName, Symbols& a_Symbols,
                             LanguageElement* a_pContextScope = nullptr) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Same as 'findSymbol' except that incomplete symbols are returned also (if not
    /// reimplemented it falls back to 'findSymbol').
    ///
    /// \param  a_strSymbol     The symbol String to parse.
    /// \param  a_pContextScope (optional) The context scope (none by default).
    /// \param  a_Modifiers     (optional) The modifiers filtering the search (none by default).
    /// \param  a_uiFlags       (optional) The flags filtering the search (none by default).
    ///
    /// \return null if parsing failed or symbol does not exist, else the existing symbol described
    ///         by this String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Symbol* parseSymbol(StringView a_strSymbol, LanguageElement* a_pContextScope = nullptr,
                                Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Parse the given String to find an existing type.
    ///
    /// \param  a_strSymbol     The type String to parse.
    /// \param  a_pContextScope (optional) The context scope (none by default).
    /// \param  a_Modifiers     (optional) The modifiers filtering the search (none by default).
    /// \param  a_uiFlags       (optional) The flags filtering the search (none by default).
    ///
    /// \return null if parsing failed or type does not exist, else the existing type described
    ///         by this String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* findType(StringView a_strType, LanguageElement* a_pContextScope = nullptr, Modifiers a_Modifiers = 0,
                           uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Parse the given String to find an existing class.
    ///
    /// \param  a_strSymbol     The class String to parse.
    /// \param  a_pContextScope (optional) The context scope (none by default).
    /// \param  a_Modifiers     (optional) The modifiers filtering the search (none by default).
    /// \param  a_uiFlags       (optional) The flags filtering the search (none by default).
    ///
    /// \return null if parsing failed or class does not exist, else the existing class described
    ///         by this String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Class* findClass(StringView a_strClass, LanguageElement* a_pContextScope = nullptr,
                             Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Parse the given String to find an existing function.
    ///
    /// \param  a_strSymbol     The function String to parse.
    /// \param  a_pContextScope (optional) The context scope (none by default).
    /// \param  a_Modifiers     (optional) The modifiers filtering the search (none by default).
    ///
    /// \return null if parsing failed or function does not exist, else the existing function
    /// described
    ///         by this String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Function* findFunction(StringView a_strFunction, LanguageElement* a_pContextScope = nullptr,
                                   Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Same as 'expression' but can return invalid expressions (if not reimplemented, falls
    /// back to 'expression').
    ///
    /// \param  a_strExpression The expression String to parse.
    /// \param  a_pContextScope (optional) The context scope (none by default).
    ///
    /// \return null or invalid expression if parsing failed, else the new expression
    /// object described by this String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* parseExpression(StringView a_strExpression, LanguageElement* a_pContextScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Parse the given String to generate an evaluable expression object.
    ///
    /// \param  a_strExpression The expression String to parse.
    /// \param  a_pContextScope (optional) The context scope (none by default).
    ///
    /// \return null if parsing failed, else the new expression object described by this
    /// String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* expression(StringView a_strExpression, LanguageElement* a_pContextScope = nullptr) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Parse the given formated C String to generate an evaluable expression object in the
    /// given context scope.
    ///
    /// \param  a_Format The formated C String to parse.
    /// \param  a_pContextScope (optional) The context scope (none by default).
    ///
    /// \return null if parsing failed, else the new expression object described by this
    /// String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* expressionf(const char* a_Format, LanguageElement* a_pContextScope, ...);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Release memory for the given expression previously generated with expression() or
    /// expressionf().
    ///
    /// \param  a_pExpression The expression to release.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void release(Expression* a_pExpression);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Parse the given formated C String to generate an evaluable expression object.
    ///
    /// \param  a_Format The formated C String to parse.
    ///
    /// \return null if parsing failed, else the new expression object described by this
    /// String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* expressionf(const char* a_Format, ...);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the source file extension associated with this language.
    ///
    /// \return The source file extension.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getSourceFileExtension() const { return m_SourceFileExtension; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parsers' pass count.
    ///
    /// \return The parsers' pass count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    uint getParserPassCount() const { return m_uiParserPassCount; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the given parser's pass priority (useful to interlap different languages
    /// parsings). By default returns : pass_value x 1000
    ///
    /// \param  a_uiPass    The pass.
    ///
    /// \return The pass priority.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual uint getParserPassPriority(uint a_uiPass) const { return a_uiPass * 1000; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  set a specific option of this language (must be added by the reimplemented class
    /// using addOption().
    ///
    /// \param  a_OptionName    The option name.
    /// \param  a_Value         The option value.
    ///
    /// \return true if the option exists and given variant matches the expected type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool setOption(StringView a_OptionName, Variant const& a_Value);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  get specific option of this language (must be added by the reimplemented class using
    /// addOption().
    ///
    /// \param  a_OptionName    The option name.
    ///
    /// \return a variant containing the current option value, or a null variant if option does not
    /// exist.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Variant const& getOption(StringView a_OptionName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates a source parser used to parse and generate language elements tree.
    ///
    /// \return The new parser.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Parser* createParser(Source* a_pSource, Message* a_pMessage) const = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Destroys a parser previously created with this language.
    ///
    /// \param  a_pParser   The parser to destroy.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void destroyParser(Parser* a_pParser) const = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates a semantic object used to perform semantic analysis on source E.S.T.
    ///
    /// \return The new parser.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Semantic* createSemantic(Source* a_pSource, Message* a_pMessage) const = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Destroys a semantic object previously created with this language.
    ///
    /// \param  a_pSemantic   The semantic to destroy.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void destroySemantic(Semantic* a_pSemantic) const = 0;

protected:
    void addOption(StringView a_OptionName, Variant const& a_DefaultValue);

private:
    String                    m_strName;
    String                    m_SourceFileExtension;
    SmallMap<String, Variant> m_Options;
    uint                      m_uiParserPassCount;
};

} // namespace lang
} // namespace phantom
