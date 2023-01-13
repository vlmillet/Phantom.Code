// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "CppLitePrerequisites.h"
#include "phantom/utils/ForwardHeap.h"

#include <map>
#include <phantom/lang/Parser.h>
#include <phantom/lang/Semantic.h>
#include <phantom/utils/Flags.h>
#include <phantom/utils/SmallVector.h>
#include <stack>
#include <vector>
/* *********************************************** */

namespace phantom
{
namespace lang
{
class Semantic;
struct NumericConversion;
struct TokExpression;
struct TokSource;
struct CppLitePassData;
class CppLiteAnalyser;

class PHANTOM_EXPORT_PHANTOM_CODE CppLiteParser : public Parser
{
    friend struct Tok;
    friend class CppLiteAnalyser;
    friend struct CppLitePassData;

public:
    enum EFlag
    {
        e_Flag_None = 0,
        e_Flag_PrintErrors = 0x1,
        e_Flag_StandardLookup = 0x2,
        e_Flag_SymbolMode = 0x4,
        e_Flag_NoCodeLocation = 0x8,
    };

    enum EPass
    {
        e_Pass_Imports = 0,
        e_Pass_Globals,       // register every symbol names at global scope
        e_Pass_Inheritance,   // register base classes and source dependencies
        e_Pass_MemberLocal,   // register global classes and create members while listing strong dependencies on other
                              // sources
        e_Pass_MemberGlobal,  // add members with knowledge of base classes members
        e_Pass_Sizing,        // sizes every type which has not been before
        e_Pass_FieldDefaults, // fields default values
        e_Pass_Implicits,     // add trivial constructors and else
        e_Pass_Blocks,
        e_Pass_Count,
    };

    PHANTOM_DECLARE_FLAGS(Flags, EFlag);

    CppLiteParser(Source* a_pSource, Message* a_pMessage, Flags a_Flags = e_Flag_None);
    ~CppLiteParser();

    Flags getFlags() const { return m_Flags; }

    void setFlags(Flags flags) { m_Flags = flags; }

    bool hasFlag(EFlag flag) const { return (m_Flags & flag) == flag; }

    virtual Symbol* parseSymbolName(StringView a_strSymbol, LanguageElement* a_pScope, Modifiers a_Modifiers /*= 0*/,
                                    uint a_uiFlags /*= 0*/);

    virtual Expression* parseExpression(StringView a_strName, LanguageElement* a_pScope);

    CppLite* getLanguage() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Check if the given symbol name is well-formed.
    ///
    /// \param  a_strSymbol     The symbol name.
    ///
    /// \return true if well-formed, false if ill-formed.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool checkSymbolSyntax(StringView a_strSymbol) const;

    size_t getErrorCount() const { return m_uiErrorCount; }

    HAUNT_OFF CppLiteAnalyser* getAnalyser() const { return m_pAnalyser; }

    // void addDependency(Source* a_pSource);

private:
    int              lex() override;
    int              fetchModules(Strings& _modules) override;
    int              fetchImports(Strings& _imports) override;
    void             begin(BuildSessionId _session) override;
    void             beginParse(BuildSessionId _session) override;
    int              parse(uint pass) override;
    LanguageElement* parseElement(StringView a_strExpression, LanguageElement* a_pScope, Modifiers a_Modifiers = 0,
                                  uint a_uiFlags = 0);
    void             endParse() override;
    void             end() override;

private:
    SmallMap<const LanguageElements*, SmallMap<LanguageElement*, LanguageElement*> > m_Templated;

    LanguageElements m_PrecompilationScopeStack;
    LanguageElements m_Elements;
    /// members

    size_t m_uiErrorCount;
    Flags  m_Flags;

    std::basic_istream<char>* m_pInputStream;
    CppLiteAnalyser*          m_pAnalyser;

private:
    CppLitePassData*    m_pPassData;
    ForwardHeapSequence m_Allocator{65536};
};

} // namespace lang
} // namespace phantom
