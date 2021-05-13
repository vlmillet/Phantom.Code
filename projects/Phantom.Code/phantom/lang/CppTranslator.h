// license [
// This file is part of the Phantom project. Copyright 2015-2016 Vivien Millet.
// Distributed under the Lesser GNU Lesser General Public License, Version 3.0. (See
// the LICENSE file which comes along with this source code or a copy at
// http://www.wiwila.com/tools/phantom/license/)
// ]

#pragma once

/* ****************** Includes ******************* */
#include "LanguageElementVisitorEx.h"
#include "lang.h"
#include "phantom/lang/TypeIndex.h"

#include <phantom/utils/Flags.h>
#include <phantom/utils/SmallSet.h>
#include <phantom/utils/SmallVector.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
class TemplateSignature;
class PHANTOM_EXPORT_PHANTOM_CODE CppTranslator : public LanguageElementVisitorEx
{
public:
    CppTranslator();
    ~CppTranslator(void);

    struct Output
    {
        String  h;
        String  cpp;
        Sources hIncludes;
        Sources cppIncludes;
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the options controlling the translation.
    ///
    /// \return The options.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Options& getOptions() const { return m_Options; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the options controlling the translation.
    ///
    /// \param  a_Options    The options.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setOptions(const Options& a_Options) { m_Options = a_Options; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the option controlling the translation.
    ///
    /// \param  a_strKey    The option key.
    /// \param  a_strValue  The option value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setOption(StringView a_strKey, StringView a_strValue) { m_Options[a_strKey] = a_strValue; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an option matching the given key.
    ///
    /// \param  a_Key   The key.
    ///
    /// \return The option.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getOption(StringView a_Key) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds alias to replace complex symbols by simpler ones (as typedefs are not memorised by reflection).
    ///
    /// \param  a_pOriginal    The symbol to replace.
    /// \param  a_pAlias  The replacing symbol.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addAlias(Symbol* a_pOriginal, Symbol* a_pAlias);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds alias to replace complex symbols by simpler ones (as typedefs are not memorised by reflection).
    ///
    /// \param  a_pOriginal    The symbol to replace.
    /// \param  a_pAlias  The replacing symbol.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool addAlias(StringView a_Original, StringView a_Alias);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an option matching the given key.
    ///
    /// \param  a_Key   The key.
    ///
    /// \return The option.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Symbol* getAlias(Symbol* a_pOriginal) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the source being translated with multipass.
    ///
    /// \return null if no multipass translation is currently running, else the source being translated.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Source* getSource() const { return m_pSource; }

    void translate(Source* a_pSource, Output& a_Out);

    virtual void translateSymbol(Symbol* a_pSymbol, String& a_strSymbol, LanguageElement* a_pContextScope);

    virtual void translateExpression(Expression* a_pExpression, String& a_strExpresson,
                                     LanguageElement* a_pContextScope);

    /// \brief  Translation types.
    struct TranslationType
    {
        enum
        {
            Declaration,
            Definition,
            Name,
            Typed,
        };
    };

    struct TranslationOption
    {
        enum
        {
            Declaration = 0x1,
            Definition = 0x2,
            FixedSizeIndexes = 0x4,
        };
    };

    enum Flag
    {
        None = 0,
        PrintErrors = 0x1,
        NoSourceMode = 0x2,
    };

    PHANTOM_DECLARE_FLAGS(Flags, Flag);

    Flags getFlags() const { return m_Flags; }

    void setFlags(Flags flags) { m_Flags = flags; }

    bool hasFlag(Flag flag) const { return (m_Flags & flag) == flag; }

    void addForceIncludeTemplateArgument(StringView _templateQName, size_t _argIdx);

    /// ------------------------------- VISIT -----------------------------------

    virtual void visit(Alias* a_pInput, VisitorData a_Data);
    virtual void visit(AllocateExpression* a_pInput, VisitorData a_Data);
    virtual void visit(AnonymousSection* a_pInput, VisitorData a_Data);
    virtual void visit(AnonymousStruct* a_pInput, VisitorData a_Data);
    virtual void visit(AnonymousUnion* a_pInput, VisitorData a_Data);
    virtual void visit(Application* a_pInput, VisitorData a_Data);
    virtual void visit(ArrayExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Array* a_pInput, VisitorData a_Data);
    virtual void visit(BaseConstructorCallStatement* a_pInput, VisitorData a_Data);
    virtual void visit(Block* a_pInput, VisitorData a_Data);
    virtual void visit(BranchIfNotStatement* a_pInput, VisitorData a_Data);
    virtual void visit(BranchIfStatement* a_pInput, VisitorData a_Data);
    virtual void visit(BranchStatement* a_pInput, VisitorData a_Data);
    virtual void visit(BuiltInConversionExpression* a_pInput, VisitorData a_Data);
    virtual void visit(BuiltInOperator* a_pInput, VisitorData a_Data);
    virtual void visit(BuiltInOperatorExpression* a_pInput, VisitorData a_Data);
    virtual void visit(CallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Class* a_pInput, VisitorData a_Data);
    virtual void visit(ClassListInitializationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ClassTypeListInitializationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ClassType* a_pInput, VisitorData a_Data);
    virtual void visit(ConditionalExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Constant* a_pInput, VisitorData a_Data);
    virtual void visit(ConstantExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Constructor* a_pInput, VisitorData a_Data);
    virtual void visit(ConstructorCallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ConstType* a_pInput, VisitorData a_Data);
    virtual void visit(ConstVolatileType* a_pInput, VisitorData a_Data);
    virtual void visit(ContainerClass* a_pInput, VisitorData a_Data);
    virtual void visit(Field* a_pInput, VisitorData a_Data);
    virtual void visit(FieldExpression* a_pInput, VisitorData a_Data);
    virtual void visit(FieldInitializationStatement* a_pInput, VisitorData a_Data);
    virtual void visit(FieldPointerExpression* a_pInput, VisitorData a_Data);
    virtual void visit(FieldPointer* a_pInput, VisitorData a_Data);
    virtual void visit(Pointer* a_pInput, VisitorData a_Data);
    virtual void visit(DeallocateExpression* a_pInput, VisitorData a_Data);
    virtual void visit(DeleteExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Ellipsis* a_pInput, VisitorData a_Data);
    virtual void visit(Enum* a_pInput, VisitorData a_Data);
    virtual void visit(Evaluable* a_pInput, VisitorData a_Data);
    virtual void visit(Expression* a_pInput, VisitorData a_Data);
    virtual void visit(ExpressionStatement* a_pInput, VisitorData a_Data);
    virtual void visit(Function* a_pInput, VisitorData a_Data);
    virtual void visit(FunctionPointer* a_pInput, VisitorData a_Data);
    virtual void visit(FunctionType* a_pInput, VisitorData a_Data);
    virtual void visit(IdentityExpression* a_pInput, VisitorData a_Data);
    virtual void visit(InitializerListExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Label* a_pInput, VisitorData a_Data);
    virtual void visit(LanguageElement* a_pInput, VisitorData a_Data);
    virtual void visit(LoadExpression* a_pInput, VisitorData a_Data);
    virtual void visit(LocalVariable* a_pInput, VisitorData a_Data);
    virtual void visit(LocalVariableExpression* a_pInput, VisitorData a_Data);
    virtual void visit(LocalVariableInitializationStatement* a_pInput, VisitorData a_Data);
    virtual void visit(LValueExpression* a_pInput, VisitorData a_Data);
    virtual void visit(LValueInitializationStatement* a_pInput, VisitorData a_Data);
    virtual void visit(LValueReference* a_pInput, VisitorData a_Data);
    virtual void visit(MemberAnonymousSection* a_pInput, VisitorData a_Data);
    virtual void visit(MemberAnonymousStruct* a_pInput, VisitorData a_Data);
    virtual void visit(MemberAnonymousUnion* a_pInput, VisitorData a_Data);
    virtual void visit(Method* a_pInput, VisitorData a_Data);
    virtual void visit(MethodPointerCallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(MethodPointer* a_pInput, VisitorData a_Data);
    virtual void visit(MemberPointer* a_pInput, VisitorData a_Data);
    virtual void visit(Module* a_pInput, VisitorData a_Data);
    virtual void visit(Symbol* a_pInput, VisitorData a_Data);
    virtual void visit(Namespace* a_pInput, VisitorData a_Data);
    virtual void visit(NewExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Package* a_pInput, VisitorData a_Data);
    virtual void visit(PackageFolder* a_pInput, VisitorData a_Data);
    virtual void visit(Parameter* a_pInput, VisitorData a_Data);
    virtual void visit(Placeholder* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderClass* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderClassType* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderConstant* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderTemplate* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderType* a_pInput, VisitorData a_Data);
    virtual void visit(PlacementNewExpression* a_pInput, VisitorData a_Data);
    virtual void visit(PrimitiveType* a_pInput, VisitorData a_Data);
    virtual void visit(Property* a_pInput, VisitorData a_Data);
    virtual void visit(PropertyExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Reference* a_pInput, VisitorData a_Data);
    virtual void visit(ReturnStatement* a_pInput, VisitorData a_Data);
    virtual void visit(RValueReference* a_pInput, VisitorData a_Data);
    virtual void visit(RValueToConstLValueExpression* a_pInput, VisitorData a_Data);
    virtual void visit(RValueReferenceExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Scope* a_pInput, VisitorData a_Data);
    virtual void visit(Signature* a_pInput, VisitorData a_Data);
    virtual void visit(Source* a_pInput, VisitorData a_Data);
    virtual void visit(Statement* a_pInput, VisitorData a_Data);
    virtual void visit(StaticAssert* a_pInput, VisitorData a_Data);
    virtual void visit(StringLiteralExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Structure* a_pInput, VisitorData a_Data);
    virtual void visit(StructureExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Subroutine* a_pInput, VisitorData a_Data);
    virtual void visit(BinaryOperationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(UnaryPostOperationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(UnaryPreOperationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Template* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantArray* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantDeclType* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantElement* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantExpression* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantTemplateInstance* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantType* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantTypeOfExpression* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateParameter* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateSignature* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateSpecialization* a_pInput, VisitorData a_Data);
    virtual void visit(Type* a_pInput, VisitorData a_Data);
    // virtual void visit(TypeTraitExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Union* a_pInput, VisitorData a_Data);
    virtual void visit(ValueMember* a_pInput, VisitorData a_Data);
    // virtual void visit(VarArgCallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Variable* a_pInput, VisitorData a_Data);
    virtual void visit(VariableExpression* a_pInput, VisitorData a_Data);
    virtual void visit(VirtualMethodTable* a_pInput, VisitorData a_Data);
    virtual void visit(VolatileType* a_pInput, VisitorData a_Data);
    virtual void visit(VirtualMethodTableSetupStatement* a_pInput, VisitorData a_Data) {}
    virtual void visit(PointerAdjustmentExpression* a_pInput, VisitorData a_Data);

    //     virtual void visit(FundamentalTypeT<void>* a_pInput, VisitorData a_Data);
    //     virtual void visit(FundamentalTypeT<std::nullptr_t>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<bool>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<char>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<uchar>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<schar>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<short>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<ushort>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<int>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<uint>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<long>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<ulong>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<longlong>* a_pInput, VisitorData a_Data);
    //     virtual void visit(IntegralTypeT<ulonglong>* a_pInput, VisitorData a_Data);
    //     virtual void visit(FloatingPointTypeT<float>* a_pInput, VisitorData a_Data);
    //     virtual void visit(FloatingPointTypeT<double>* a_pInput, VisitorData a_Data);
    //     virtual void visit(FloatingPointTypeT<longdouble>* a_pInput, VisitorData a_Data);
    //
    //     PHANTOM_IF_WCHAR_T(virtual void visit(IntegralTypeT<wchar_t>* a_pInput, VisitorData a_Data););
    //     PHANTOM_IF_CHAR16_T(virtual void visit(IntegralTypeT<char16_t>* a_pInput, VisitorData a_Data););
    //     PHANTOM_IF_CHAR32_T(virtual void visit(IntegralTypeT<char32_t>* a_pInput, VisitorData a_Data););

    void appendIncludes(Source* a_pInput, Sources const& a_IncludedSources);

protected:
    int translate(uint a_uiPass, String& a_Out);

    void translateLiteral(const String& a_Value, String& translation, int options = 0);
    void translate(LanguageElement* a_pLanguageElement, int a_TranslationType = TranslationType::Name);
    void translateTypeName(Type* a_pType);
    void _addInclude(SmallMap<String, Source*>& a_Includes, Source* a_pSource);
    void translateTyped(Type* a_pType, const String& a_Identifier);

    inline void newLine() { *m_pTranslation += '\n'; }
    inline void indent()
    {
        if (m_IndentationLevel)
            m_pTranslation->resize(m_pTranslation->size() + (m_IndentationLevel * m_iIndentCharCount), m_cIndentChar);
    }
    inline void append(StringView a_strString) { *m_pTranslation += a_strString; }
    inline void append(const char* a_strString) { *m_pTranslation += a_strString; }
    inline void append(char c) { *m_pTranslation += c; }
    inline void add(const String& a_strString)
    {
        indent();
        *m_pTranslation += a_strString;
    }
    inline void add(const char* a_strString)
    {
        indent();
        *m_pTranslation += a_strString;
    }
    inline void add(char* c)
    {
        indent();
        *m_pTranslation += c;
    }
    inline void beginDecl() { indent(); }
    inline void beginBlock(char c = '{')
    {
        append(c);
        newLine();
        m_IndentationLevel++;
    }
    inline void endBlock(char c = '}')
    {
        m_IndentationLevel--;
        indent();
        append(c);
    }
    inline void endBlock(const char* c)
    {
        PHANTOM_ASSERT(m_IndentationLevel);
        m_IndentationLevel--;
        indent();
        append(c);
        newLine();
    }
    inline void endDecl() { append('\n'); }
    inline void label(const String& l)
    {
        m_IndentationLevel--;
        beginDecl();
        append(l + ':');
        endDecl();
        m_IndentationLevel++;
    }
    inline void             pushContextScope(LanguageElement* a_pScope) { m_ContextScopes.push_back(a_pScope); }
    inline LanguageElement* getContextScope() const { return m_ContextScopes.back(); }
    inline void             popContextScope() { m_ContextScopes.pop_back(); }
    void                    addInclude(Symbol* a_pSymbol);
    void                    addInclude(SmallMap<String, Source*>& a_Includes, Symbol* a_pSymbol);
    void                    addForwardH(Type* a_pType);
    void                    addForwardCPP(Type* a_pType);
    void                    addForward(Type* a_pType);
    void                    appendNamespaceList(Namespace* a_pNamespace);
    void                    appendNamespaceScope(Namespace* a_pNamespace);
    void                    appendNamespaceOpening(Namespace* a_pNamespace);
    void                    appendNamespaceClosing(Namespace* a_pNamespace);

    void appendNestingClassList(ClassType* a_pClassType);

    void appendError(const String& a_Error)
    {
        append('@');
        append(a_Error);
        append('@');
        PHANTOM_ASSERT(false);
    }

    void beginElseIf()
    {
        m_bEndif = true;
        append("else ");
    }

    void beginIf()
    {
        if (m_bEndif)
        {
            m_bEndif = false;
        }
        append("if(");
    }

    void endIf()
    {
        append(")");
        newLine();
    }

    void beginFor() { append("for("); }

    void endFor() { append(")"); }

    void beginWhile() { append("while("); }

    void endWhile() { append(")"); }

    void beginDo() { append("do"); }

    void beginParen(bool _a)
    {
        if (_a)
            append('(');
    }
    void endParen(bool _a)
    {
        if (_a)
            append(')');
    }

    void appendExpression(Expression* a_pExpression, Expression*, VisitorData a_Data);

    struct Operation
    {
        Operation() = default;
        Operation(Operator op, bool lhs) : op(op), lhs(lhs) {}
        Operator op;
        bool     lhs = false;
    };
    bool requiresParenthesis(Operation _left, Operator _right) const;
    bool requiresParenthesis(Operator _right) const
    {
        return !m_OperatorStack.empty() && requiresParenthesis(m_OperatorStack.back(), _right);
    }

protected:
    int                                m_uiFile = 0;
    Source*                            m_pSource = nullptr;
    Options                            m_Options;
    String*                            m_pTranslation;
    int                                m_IndentationLevel;
    LanguageElements                   m_ContextScopes;
    SmallSet<MemberAnonymousSection*>  m_MemberAnonymousSections;
    SmallMap<String, Source*>          m_IncludesH;
    SmallMap<String, Source*>          m_IncludesCPP;
    SmallMap<String, Type*>            m_ForwardsH;
    SmallMap<String, Type*>            m_ForwardsCPP;
    SmallMap<Symbol*, Symbol*>         m_Aliases;
    Flags                              m_Flags;
    char                               m_cIndentChar;
    int                                m_iIndentCharCount;
    SmallMap<size_t, SmallSet<String>> m_ForceIncludeTemplateArguments;
    bool                               m_bEndif;
    int                                m_noAlias = 0;
    int                                m_ForceQualifiedName = 0;
    StringView                         m_NoKeywordChars = "$";

    SmallVector<Operation> m_OperatorStack;

private:
    void addForwardIfRequired(Type* a_pType);
    void _formatForwards(SmallMap<String, Type*>& a_ForwardsH, String& a_Str);
    void _formatTemplateSign(LanguageElement* a_pScope, TemplateSignature* a_pSign, String& a_Str);
    void appendSignatureQuals(Signature*);
    void addIncludeIfRequired(Type* a_pType);
    void _appendParameterList(Subroutine* a_pInput, VisitorData a_Data);
    void _appendFuncBlock(Subroutine* a_pInput, VisitorData a_Data);
    void _appendCtorInit(Constructor* a_pInput, VisitorData a_Data);
    void _appendHauntMarkers(Symbol* a_pInput);
};

} // namespace lang
} // namespace phantom
