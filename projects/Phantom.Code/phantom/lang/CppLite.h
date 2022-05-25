// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "CppLitePrerequisites.h"
#include "Message.h"

#include <phantom/lang/Language.h>
#include <phantom/lang/LanguageElement.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
struct NumericConversion;
class CppLiteParser;

HAUNT_METADATA("name", "CppLite Language")
class PHANTOM_EXPORT_PHANTOM_CODE CppLite : public Language
{
    friend class Semantic;

public:
    HAUNT_MODIFIERS(PHANTOM_R_SINGLETON);

public:
    CppLite(void);
    ~CppLite(void);

    static void Load();

    enum EFlag
    {
        e_Flag_CppCompatible = 0x1,
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Check if the given symbol name is well-formed.
    ///
    /// \param  a_strSymbol     The symbol name.
    ///
    /// \return true if well-formed, false if ill-formed.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool checkSymbolSyntax(StringView a_strSymbol) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if given name is a C++ identifier.
    ///
    /// \param  a_strName   The name.
    ///
    /// \return true if C++ identifier, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isIdentifier(StringView a_strName) const;

    virtual Symbol* findSymbol(StringView a_strSymbol, LanguageElement* a_pContextScope = nullptr,
                               Modifiers a_Modifiers = 0, uint a_uiFlags = 0) override;

    virtual void findSymbols(StringView a_strSymbolsName, Symbols& a_Symbols,
                             LanguageElement* a_pContextScope = nullptr) override;

    virtual Symbol* parseSymbol(StringView a_strSymbol, LanguageElement* a_pContextScope = nullptr,
                                Modifiers a_Modifiers = 0, uint a_uiFlags = 0) override;

    virtual Expression* expression(StringView a_strExpression, LanguageElement* a_pContextScope = nullptr) override;

    Expression* expression(StringView a_strExpression, Message* a_pMessage, LanguageElement* a_pContextScope = nullptr);

    virtual Expression* parseExpression(StringView       a_strExpression,
                                        LanguageElement* a_pContextScope = nullptr) override;

    Expression* parseExpression(StringView a_strExpression, Message* a_pMessage,
                                LanguageElement* a_pContextScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a default semantic object to perform some semantic requests (useful for trivial
    /// manual builds)
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Semantic* getDefaultSemantic() const { return m_pDefaultSemantic; }

    Operator         getBuiltInBinaryOperatorId(StringView a_strName) const;
    Operator         getBuiltInPreUnaryOperatorId(StringView a_strName) const;
    Operator         getBuiltInPostUnaryOperatorId(StringView a_strName) const;
    BuiltInOperator* getBuiltInBinaryOperator(StringView a_strName) const;
    BuiltInOperator* getBuiltInPreUnaryOperator(StringView a_strName) const;
    BuiltInOperator* getBuiltInPostUnaryOperator(StringView a_strName) const;

    BuiltInOperator* getBuiltInOperator(Operator a_Op) const { return m_BuiltInOperators[int(a_Op)]; }

    Source* getDefaultSource() const { return m_pDefaultSource; }

private:
    void              _installFundamentalBuiltInOperators(bool a_bAdd);
    BuiltInOperator*& _builtInOperator(Operator a_Op) { return m_BuiltInOperators[int(a_Op)]; }
    BuiltInOperator*  _builtInOperator(Operator a_Op) const { return m_BuiltInOperators[int(a_Op)]; }
    Expression*       _internalExpression(StringView a_strExpression, LanguageElement* a_pContextScope);
    Variant           _evalExpression(Expression* _exp);
    Source*           _selectBestSource(LanguageElement* a_pScope);

protected:
    Parser*   createParser(Source* a_pSource, Message* a_pMessage) const override;
    void      destroyParser(Parser* a_pParser) const override;
    Semantic* createSemantic(Source* a_pSource, Message* a_pMessage) const override;
    void      destroySemantic(Semantic* a_pSemantic) const override;

private:
    BuiltInOperator* m_BuiltInOperators[int(Operator::COUNT)];
    Module*          m_pPhantomModule = nullptr;
    Module*          m_pPhantomCodeModule = nullptr;
    Semantic*        m_pDefaultSemantic = nullptr;
    Message          m_DefaultMessage;
    Source*          m_pDefaultSource;
    struct ExpressionCacheData
    {
        SmallMap<LanguageElement*, Expression*> expressions;
    };
    std::unordered_map<String, ExpressionCacheData> m_ExpressionCache;

public:
    static CppLite* Get();
};

} // namespace lang
} // namespace phantom
