#include "CppLiteGrammar.h"

namespace phantom
{
    namespace lang
    {
        namespace { template<class T> void PHANTOM_LANG_CPPLITEGRAMMAR_DTOR(T* p) { p->~T(); } }
        namespace ast
        {
            #define PHANTOM_LANG_CPPLITEGRAMMAR_NEW(...) new (m_Allocator.allocate(sizeof(__VA_ARGS__))) __VA_ARGS__
            #define PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(...) if(__VA_ARGS__) { PHANTOM_LANG_CPPLITEGRAMMAR_DTOR(__VA_ARGS__); m_Allocator.deallocate(__VA_ARGS__, sizeof(__VA_ARGS__)); }
            static const char* g_TokenPrettyNames[] = 
            {
                "<error>"
                , "::"
                , "{"
                , "}"
                , "("
                , ")"
                , "["
                , "]"
                , "class"
                , "union"
                , "struct"
                , "enum"
                , "template"
                , "typename"
                , "this"
                , "nullptr"
                , "operator"
                , "override"
                , "break"
                , "continue"
                , ";"
                , ":"
                , ","
                , "+="
                , "&="
                , "/="
                , "%="
                , "*="
                , "|="
                , "<<="
                , "-="
                , "^="
                , "<="
                , ">="
                , "->"
                , "<"
                , ">"
                , "=="
                , "!="
                , "!"
                , "?"
                , "*"
                , "/"
                , "++"
                , "--"
                , "+"
                , "-"
                , "~"
                , "."
                , "@@"
                , "&&"
                , "&"
                , "||"
                , "|"
                , "^"
                , "%"
                , "<<"
                , "public"
                , "protected"
                , "private"
                , "="
                , "..."
                , "auto"
                , "unsigned"
                , "signed"
                , "long"
                , "bool"
                , "bcharb"
                , "short"
                , "int"
                , "floating point type"
                , "void"
                , "case"
                , "for"
                , "while"
                , "switch"
                , "default"
                , "do"
                , "if"
                , "else"
                , "return"
                , "goto"
                , "new"
                , "delete"
                , "explicit"
                , "typedef"
                , "using"
                , "reinterpret_cast"
                , "static_cast"
                , "const_cast"
                , "sizeof"
                , "alignof"
                , "alignas"
                , "friend"
                , "typeof"
                , "classof"
                , "enumof"
                , "unionof"
                , "import"
                , "#include"
                , "@module"
                , "@function"
                , "@function_ptr"
                , "@delegate"
                , "assert"
                , "static_assert"
                , "@is_same"
                , "const"
                , "volatile"
                , "decltype"
                , "final"
                , "virtual"
                , "static"
                , "hexadecimal value"
                , "float value"
                , "double value"
                , "unsigned long long value"
                , "unsigned long value"
                , "unsigned int value"
                , "long long value"
                , "long value"
                , "int value"
                , "boolean value"
                , "double quoted string"
                , "quoted character"
                , "reserved phantom keyword"
                , "identifier"
                , "<end-of-file>"
            };
            union RuleUnion
            {
                Source m_Source;
                ModuleDeclaration m_ModuleDeclaration;
                ImportDeclaration m_ImportDeclaration;
                Import m_Import;
                ImportBinding m_ImportBinding;
                ImportBase m_ImportBase;
                Meta m_Meta;
                MetaDecl m_MetaDecl;
                Annotation m_Annotation;
                MetaOrAnnotation m_MetaOrAnnotation;
                SourceDeclaration m_SourceDeclaration;
                Declaration m_Declaration;
                MixinStatementDefinition m_MixinStatementDefinition;
                ForInit m_ForInit;
                ConversionFunction m_ConversionFunction;
                Constructor m_Constructor;
                Destructor m_Destructor;
                ConstructorInitializer m_ConstructorInitializer;
                FunctionEnd m_FunctionEnd;
                Operator m_Operator;
                OperatorEnd m_OperatorEnd;
                FunctionOrVariableEnd m_FunctionOrVariableEnd;
                MethodSpecifier m_MethodSpecifier;
                Specifier m_Specifier;
                FunctionOrVariable m_FunctionOrVariable;
                DeclType m_DeclType;
                TemplateArgument m_TemplateArgument;
                Statement m_Statement;
                MixinStatement m_MixinStatement;
                ReturnStatement m_ReturnStatement;
                GotoStatement m_GotoStatement;
                Declarator m_Declarator;
                AutoDeclarator m_AutoDeclarator;
                TemplateArgumentListSplit m_TemplateArgumentListSplit;
                TemplateArgumentNoAssign m_TemplateArgumentNoAssign;
                TemplateArgumentAssign m_TemplateArgumentAssign;
                TemplateArgumentList m_TemplateArgumentList;
                LocalVariableDecl m_LocalVariableDecl;
                Name m_Name;
                FundamentalTypeBase m_FundamentalTypeBase;
                Expression m_Expression;
                LogicalOrExpressionRight m_LogicalOrExpressionRight;
                LogicalAndExpressionRight m_LogicalAndExpressionRight;
                BinOrExpressionRight m_BinOrExpressionRight;
                XOrExpressionRight m_XOrExpressionRight;
                BinAndExpressionRight m_BinAndExpressionRight;
                EqualityExpressionRight m_EqualityExpressionRight;
                RelationalExpressionRight m_RelationalExpressionRight;
                Type m_Type;
                ShiftExpressionRight m_ShiftExpressionRight;
                AddExpressionRight m_AddExpressionRight;
                MulExpressionRight m_MulExpressionRight;
                SpecialCastExpression m_SpecialCastExpression;
                FunctionPtrExpression m_FunctionPtrExpression;
                CallList m_CallList;
                PostFixEnd m_PostFixEnd;
                ArrayExtentExpression m_ArrayExtentExpression;
                StaticAssert m_StaticAssert;
                StringLiteralChain m_StringLiteralChain;
                TypeExtent m_TypeExtent;
                Enum m_Enum;
                EnumConstant m_EnumConstant;
                Using m_Using;
                Typedef m_Typedef;
                Alias m_Alias;
                TemplateParameterDefault m_TemplateParameterDefault;
                TemplateValueParameter m_TemplateValueParameter;
                TemplateTypeParameter m_TemplateTypeParameter;
                TemplateParameter m_TemplateParameter;
                TemplateSignature m_TemplateSignature;
                Template m_Template;
                FunctionBlock m_FunctionBlock;
                QualifiedName m_QualifiedName;
                QualifiedDotName m_QualifiedDotName;
                QualifiedScopedName m_QualifiedScopedName;
                BaseClass m_BaseClass;
                Class m_Class;
                Union m_Union;
                Method m_Method;
                Parameter m_Parameter;
                AccessSpecifier m_AccessSpecifier;
                FieldOverride m_FieldOverride;
                Symbol m_Symbol;
                Member m_Member;
                IfOrWhileLocalVariableDecl m_IfOrWhileLocalVariableDecl;
                IfOrWhileCondition m_IfOrWhileCondition;
                ForeachSignature m_ForeachSignature;
                BlockDeclaration m_BlockDeclaration;
                TypeofExpression m_TypeofExpression;
                LocalVariableStatement m_LocalVariableStatement;
                LastTemplateArgument m_LastTemplateArgument;
                FundamentalTypeFunctionCast m_FundamentalTypeFunctionCast;
                PrimaryExpression m_PrimaryExpression;
                NotLastTemplateArgument m_NotLastTemplateArgument;
                FunctionPointerType m_FunctionPointerType;
                UnsignedType m_UnsignedType;
                SignedType m_SignedType;
                LongType m_LongType;
                VoidType m_VoidType;
                FundamentalType m_FundamentalType;
                QualifiedType m_QualifiedType;
                ExplicitType m_ExplicitType;
                ExpressionType m_ExpressionType;
                ExplicitTypeOrAuto m_ExplicitTypeOrAuto;
                TypeOrAuto m_TypeOrAuto;
                AutoType m_AutoType;
                BasicType m_BasicType;
                Auto m_Auto;
                BasicTypeOrAuto m_BasicTypeOrAuto;
                IntegralConstant m_IntegralConstant;
                ArrayExtent m_ArrayExtent;
                DerivedExtent m_DerivedExtent;
                ExpressionStatement m_ExpressionStatement;
                SwitchStatement m_SwitchStatement;
                AssignExpression m_AssignExpression;
                ConditionalExpression m_ConditionalExpression;
                Friend m_Friend;
                LogicalOrExpression m_LogicalOrExpression;
                BasicMember m_BasicMember;
                LogicalAndExpression m_LogicalAndExpression;
                ForStatement m_ForStatement;
                BinOrExpression m_BinOrExpression;
                WhileStatement m_WhileStatement;
                XOrExpression m_XOrExpression;
                BlockStatement m_BlockStatement;
                BinAndExpression m_BinAndExpression;
                DoWhileStatement m_DoWhileStatement;
                EqualityExpression m_EqualityExpression;
                StatementOrCase m_StatementOrCase;
                RelationalExpression m_RelationalExpression;
                MemberWithMetaOrAnnotation m_MemberWithMetaOrAnnotation;
                ShiftExpression m_ShiftExpression;
                BasicMemberWithMetaOrAnnotation m_BasicMemberWithMetaOrAnnotation;
                AddExpression m_AddExpression;
                IfStatement m_IfStatement;
                MulExpression m_MulExpression;
                CaseStatement m_CaseStatement;
                ExplicitLocalVariableDecl m_ExplicitLocalVariableDecl;
                UnaryExpression m_UnaryExpression;
                AssignExpressionOrInitializerList m_AssignExpressionOrInitializerList;
                ExpressionOrInitializerList m_ExpressionOrInitializerList;
                InitializerList m_InitializerList;
                DefaultStatement m_DefaultStatement;
                ArrayAccess m_ArrayAccess;
                KeywordExpression m_KeywordExpression;
                NullptrExpression m_NullptrExpression;
                PostFixExpression m_PostFixExpression;
                AutoLocalVariableDecl m_AutoLocalVariableDecl;
                CStyleCastExpression m_CStyleCastExpression;
                TypeTraitBinary m_TypeTraitBinary;
                LabelStatement m_LabelStatement;
                AssertStatement m_AssertStatement;
                BreakStatement m_BreakStatement;
                ContinueStatement m_ContinueStatement;
                SizeofExpression m_SizeofExpression;
                LastTemplateArgumentAssign m_LastTemplateArgumentAssign;
                LastTemplateArgumentNoAssign m_LastTemplateArgumentNoAssign;
                NotLastTemplateArgumentAssign m_NotLastTemplateArgumentAssign;
                NotLastTemplateArgumentNoAssign m_NotLastTemplateArgumentNoAssign;
            };
            struct CppLiteGrammarPrivate
            {
                CppLiteGrammar::Allocator m_Allocator;
                spell::Lexer m_Lexer;
                CppLiteGrammar::ErrorDelegate m_ErrorDelegate;
                phantom::SmallVector<spell::Token, 2048> m_Tokens;
                struct ErrorInfo
                {
                    size_t tokenPointer;
                    spell::Location loc;
                    EError code;
                    const char* text;
                };
                ErrorInfo m_LastError;
                struct Try
                {
                    size_t m_StartTokenPointer;
                    bool m_Success;
                };
                phantom::SmallVector<Try> m_Tries;
                size_t m_TokenPointer;
                size_t m_TryPointer;
                size_t m_LookaheadCounter;
                bool error(spell::Location loc, ast::EError code, const char* text)
                {
                    if (m_TryPointer != -1)
                    {
                        if (m_LastError.tokenPointer <= m_TokenPointer)
                        {m_LastError = ErrorInfo{ m_TokenPointer, loc, code, text };}
                        m_Tries[m_TryPointer].m_Success = false;
                    }
                    else if (m_Tries.size())
                    {
                        throwErrors();
                        m_LastError = ErrorInfo{ 0, spell::Location(), e_ErrorOK, "" };
                    }
                    else m_ErrorDelegate(loc, code, text);
                    return false;
                }
                spell::Location location() const {
                    auto tokptr = m_TokenPointer;
                    spell::Location loc;
                    do
                    {
                        loc = m_Tokens[tokptr].location;
                    } while (tokptr-- && !loc.isValid());
                    return loc;
                }
                bool token(EToken t, size_t tokenPointer)
                {
                    return m_Tokens[tokenPointer].id == t;
                }
                bool token(EToken t)
                {
                    return m_Tokens[m_TokenPointer].id == t;
                }
                bool token(std::initializer_list<EToken> const& ts)
                {
                    for(auto t : ts) if(m_Tokens[m_TokenPointer].id == t) return true; return false;
                }
                void clearLastErrors()
                {
                    if (m_TokenPointer >= m_LastError.tokenPointer)
                    m_LastError = ErrorInfo{ 0, spell::Location(), e_ErrorOK, "" };
                }
                bool lookahead(EToken token)
                {
                    return lookahead({token});
                }
                bool lookahead(const std::initializer_list<EToken>& tokens)
                {
                    size_t index = m_TokenPointer + m_LookaheadCounter;
                    if (index >= m_Tokens.size())
                    {
                         m_Tokens.resize(m_Tokens.size() + 1);
                        assert(index < m_Tokens.size());
                        m_Lexer.next(m_Tokens.back()); // read token at the end of the list
                    }
                    spell::Token& tok = m_Tokens[index];
                    for (auto it = tokens.begin(); it != tokens.end(); ++it)
                    {
                        if (tok.id == *it)
                        {
                            m_LookaheadCounter++;
                            return true;
                        }
                    }
                    return false;
                }
                bool endOfFile() const { return m_Lexer.getStatus() != spell::Lexer::e_Status_Ready; }
                EToken token() { return token(m_TokenPointer); }
                EToken token(size_t tokenPointer) { return (EToken)m_Tokens[tokenPointer].id; }
                template<class T>
                void valueAs(T& buf) { m_Tokens[m_TokenPointer].value.as(buf); }
                template<class T>
                bool read(EToken t, T& outValue)
                {
                    m_LookaheadCounter = 0;
                    if (token() == t)
                    {
                        valueAs(outValue);
                        clearLastErrors(); // we succeeded to read a token since last try errors, they are now obsolete
                        consume();
                        return true;
                    }
                    else
                    {
                        outValue = 0;
                        return m_TryPointer == -1 ? errorExpected(location(), t) : errorUnexpected(location(), token());
                    }
                }
                bool read(EToken t)
                {
                    m_LookaheadCounter = 0;
                    if (token() == t)
                    {
                        clearLastErrors(); // we succeeded to read a token since last try errors, they are now obsolete
                        return consume();
                    }
                    else
                    {
                        return m_TryPointer == -1 ? errorExpected(location(), t) : errorUnexpected(location(), token());
                    }
                }
                bool read(const std::initializer_list<EToken>& tokens)
                {
                    m_LookaheadCounter = 0;
                    spell::Token& tok = m_Tokens[m_TokenPointer];
                    for (auto it = tokens.begin(); it != tokens.end(); ++it)
                    {
                        if (tok.id == *it)
                        {
                            clearLastErrors(); // we succeeded to read a token since last try errors, they are now obsolete
                            return consume();
                        }
                    }
                    return errorUnexpected(location(), token());
                }
                template<class T>
                bool read(const std::initializer_list<EToken>& tokens, T& out)
                {
                    m_LookaheadCounter = 0;
                    spell::Token& tok = m_Tokens[m_TokenPointer];
                    for (auto it = tokens.begin(); it != tokens.end(); ++it)
                    {
                        if (tok.id == *it)
                        {
                            bool ok = tok.value.as(out);
                            assert(ok);
                            clearLastErrors(); // we succeeded to read a token since last try errors, they are now obsolete
                            return consume();
                        }
                    }
                    out = 0;
                    return errorUnexpected(location(), token());
                }
                bool checkTokenContiguous(size_t tokenPointer)
                {
                    spell::Location prevLoc = m_Tokens[tokenPointer - 1].location;
                    spell::Location newLoc = location();
                    if (newLoc.begin.line != prevLoc.end.line
                    || newLoc.begin.column != prevLoc.end.column)
                    {
                         return errorUnexpected(newLoc, token(tokenPointer));
                    }
                    return true;
                }
                bool checkNextTokenContiguous()
                {
                    return checkTokenContiguous(m_TokenPointer);
                }
                bool readContiguous(EToken t)
                {
                    return checkNextTokenContiguous() && read(t);
                }
                template<class T>
                bool readContiguous(EToken t, T& out)
                {
                    return checkNextTokenContiguous() && read(t, out);
                }
                bool readContiguous(const std::initializer_list<EToken>& tokens)
                {
                    return checkNextTokenContiguous() && read(tokens);
                }
                template<class T>
                bool readContiguous(const std::initializer_list<EToken>& tokens, T& out)
                {
                    return checkNextTokenContiguous() && read(tokens, out);
                }
                bool consume()
                {
                    m_LookaheadCounter = 0;
                    m_TokenPointer++;
                    if (m_TokenPointer >= m_Tokens.size())
                    {
                        m_Tokens.resize(m_Tokens.size() + 1);
                        assert(m_TokenPointer == m_Tokens.size()-1);
                        spell::Lexer::EStatus status = m_Lexer.next(m_Tokens.back());
                        if (status == spell::Lexer::e_Status_Error) // read token at the end of the list
                        {
                            return error(m_TokenPointer == 0 ? spell::Location() : m_Tokens[m_TokenPointer - 1].location, e_Error_UnknownToken, "");
                        }
                        else if (status == spell::Lexer::e_Status_Finished)
                        {
                             m_Tokens.back().id = tokenEOF;
                        }
                    }
                    return true;
                }
                
                void beginTry()
                {
                    m_TryPointer++;
                    if (m_Tries.size() <= m_TryPointer)
                    {
                        m_Tries.resize(m_TryPointer + 1);
                        m_Tries[m_TryPointer] = Try{ m_TokenPointer, true };
                    }
                    else { m_Tries[m_TryPointer].m_Success = true; m_Tries[m_TryPointer].m_StartTokenPointer = m_TokenPointer; }
                }
                
                bool endTry()
                {
                    assert(m_TryPointer != -1);
                    if (m_Tries[m_TryPointer].m_Success)
                    {
                        m_Tries.resize(m_TryPointer);
                        m_TryPointer--;
                        return true;
                    }
                    else
                    {
                        m_TokenPointer = m_Tries[m_TryPointer].m_StartTokenPointer; // reset token pointer to when the try began
                        m_TryPointer--;
                        return false;
                    }
                }
                bool errorUnexpected(spell::Location loc, EToken t)
                {
                    return error(loc, (EError)(e_Error_UnexpectedToken), tokenName(t));
                }
                bool errorExpected(spell::Location loc, EToken t)
                {
                    return error(loc, (EError)(e_Error_ExpectedToken), tokenName(t));
                }
                const char* tokenName(EToken t)
                {
                    if (t == -1) return "<unknown-token>"; return g_TokenPrettyNames[t]; 
                }
                void throwErrors()
                {
                    if (m_LastError.code != e_ErrorOK)
                    {
                         m_ErrorDelegate(m_LastError.loc, m_LastError.code, m_LastError.text);
                    }
                }
                void eof()
                {
                    if (m_Tries.size())
                    throwErrors();
                }
                static void defaultErrorDelegate(spell::Location loc, EError code, const char* msg)
                {
                    const char* errorMsg = "";
                    if(code > e_ErrorOK && code < e_Error_UnexpectedToken) errorMsg = "expected token ";
                    else if(code > e_Error_UnexpectedToken && code < e_Error_Count) errorMsg = "unexpected token ";
                    printf("%d|%d -> %d|%d : error %d : %s%s\n", loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, code, errorMsg, msg);
                }
                
                CppLiteGrammarPrivate(CppLiteGrammar::Allocator a_Allocator)
                     : m_LookaheadCounter(0)
                     , m_ErrorDelegate(defaultErrorDelegate)
                     , m_TokenPointer(-1)
                     , m_TryPointer(-1)
                     , m_LastError{ 0, spell::Location(), e_ErrorOK, "" }
                     , m_Allocator(a_Allocator)
                {
                    m_Lexer.skip("\\/\\/.*?[\\r?\\n\\0]");
                    m_Lexer.skip("\\/\\*.*?\\*\\/");
                    m_Lexer.skip("[\\t\\n\\r\\0 ]");
                    m_Lexer.addRule(spell::Lexer::as_string(), "::", token_SCOPE);
                    m_Lexer.addRule('{', token_BLOCK_START);
                    m_Lexer.addRule('}', token_BLOCK_END);
                    m_Lexer.addRule(spell::Lexer::as_string(), '(', token_PAREN_START);
                    m_Lexer.addRule(')', token_PAREN_END);
                    m_Lexer.addRule('[', token_BRACKET_START);
                    m_Lexer.addRule(']', token_BRACKET_END);
                    m_Lexer.addRule(spell::Lexer::as_string(), "class", token_CLASS);
                    m_Lexer.addRule("union", token_UNION);
                    m_Lexer.addRule(spell::Lexer::as_string(), "struct", token_STRUCT);
                    m_Lexer.addRule("enum", token_ENUM);
                    m_Lexer.addRule("template", token_TEMPLATE);
                    m_Lexer.addRule(spell::Lexer::as_string(), "typename", token_TYPENAME);
                    m_Lexer.addRule(spell::Lexer::as_string(), "this", token_THIS);
                    m_Lexer.addRule("nullptr", token_NULLPTR);
                    m_Lexer.addRule("operator", token_OPERATOR);
                    m_Lexer.addRule(spell::Lexer::as_string(), "override", token_OVERRIDE);
                    m_Lexer.addRule(spell::Lexer::as_string(), "break", token_BREAK);
                    m_Lexer.addRule(spell::Lexer::as_string(), "continue", token_CONTINUE);
                    m_Lexer.addRule(';', token_SEMI_COLON);
                    m_Lexer.addRule(':', token_COLON);
                    m_Lexer.addRule(',', token_COMMA);
                    m_Lexer.addRule(spell::Lexer::as_string(), "\\+=", token_ASS_ADD);
                    m_Lexer.addRule(spell::Lexer::as_string(), "&=", token_ASS_AND);
                    m_Lexer.addRule(spell::Lexer::as_string(), "\\/=", token_ASS_DIV);
                    m_Lexer.addRule(spell::Lexer::as_string(), "%=", token_ASS_MOD);
                    m_Lexer.addRule(spell::Lexer::as_string(), "\\*=", token_ASS_MUL);
                    m_Lexer.addRule(spell::Lexer::as_string(), "\\|=", token_ASS_OR);
                    m_Lexer.addRule(spell::Lexer::as_string(), "<<=", token_ASS_SHL);
                    m_Lexer.addRule(spell::Lexer::as_string(), "\\-=", token_ASS_SUB);
                    m_Lexer.addRule(spell::Lexer::as_string(), "\\^=", token_ASS_XOR);
                    m_Lexer.addRule(spell::Lexer::as_string(), "<=", token_LE);
                    m_Lexer.addRule(spell::Lexer::as_string(), ">=", token_GE);
                    m_Lexer.addRule(spell::Lexer::as_string(), "->", token_ARROW);
                    m_Lexer.addRule(spell::Lexer::as_string(), '<', token_LT);
                    m_Lexer.addRule(spell::Lexer::as_string(), '>', token_GT);
                    m_Lexer.addRule(spell::Lexer::as_string(), "==", token_EQ);
                    m_Lexer.addRule(spell::Lexer::as_string(), "!=", token_NE);
                    m_Lexer.addRule(spell::Lexer::as_string(), '!', token_NOT);
                    m_Lexer.addRule('?', token_QUESTION);
                    m_Lexer.addRule(spell::Lexer::as_string(), '*', token_MUL);
                    m_Lexer.addRule(spell::Lexer::as_string(), '/', token_DIV);
                    m_Lexer.addRule(spell::Lexer::as_string(), "\\+\\+", token_INC);
                    m_Lexer.addRule(spell::Lexer::as_string(), "\\-\\-", token_DEC);
                    m_Lexer.addRule(spell::Lexer::as_string(), '+', token_PLUS);
                    m_Lexer.addRule(spell::Lexer::as_string(), '-', token_MINUS);
                    m_Lexer.addRule(spell::Lexer::as_string(), '~', token_COMPL);
                    m_Lexer.addRule(spell::Lexer::as_string(), '.', token_DOT);
                    m_Lexer.addRule(spell::Lexer::as_string(), "@@", token_ATAT);
                    m_Lexer.addRule(spell::Lexer::as_string(), "&&", token_LOG_AND);
                    m_Lexer.addRule(spell::Lexer::as_string(), '&', token_BIN_AND);
                    m_Lexer.addRule(spell::Lexer::as_string(), "\\|\\|", token_LOG_OR);
                    m_Lexer.addRule(spell::Lexer::as_string(), '|', token_BIN_OR);
                    m_Lexer.addRule(spell::Lexer::as_string(), '^', token_XOR);
                    m_Lexer.addRule(spell::Lexer::as_string(), '%', token_MOD);
                    m_Lexer.addRule(spell::Lexer::as_string(), "<<", token_SHL);
                    m_Lexer.addRule(spell::Lexer::as_string(), "public", token_PUBLIC);
                    m_Lexer.addRule(spell::Lexer::as_string(), "protected", token_PROTECTED);
                    m_Lexer.addRule(spell::Lexer::as_string(), "private", token_PRIVATE);
                    m_Lexer.addRule(spell::Lexer::as_string(), '=', token_ASSIGN);
                    m_Lexer.addRule(spell::Lexer::as_string(), "\\.\\.\\.", token_ELLIPSE);
                    m_Lexer.addRule(spell::Lexer::as_string(), "auto", token_AUTO);
                    m_Lexer.addRule(spell::Lexer::as_string(), "unsigned", token_UNSIGNED);
                    m_Lexer.addRule(spell::Lexer::as_string(), "signed", token_SIGNED);
                    m_Lexer.addRule(spell::Lexer::as_string(), "long", token_LONG);
                    m_Lexer.addRule(spell::Lexer::as_string(), "bool", token_BOOL);
                    m_Lexer.addRule(spell::Lexer::as_string(), "\bchar\b", token_CHAR);
                    m_Lexer.addRule(spell::Lexer::as_string(), "short", token_SHORT);
                    m_Lexer.addRule(spell::Lexer::as_string(), "int", token_INT);
                    m_Lexer.addRule(spell::Lexer::as_string(), "float|double", token_FLOATING_POINT_TYPE);
                    m_Lexer.addRule(spell::Lexer::as_string(), "void", token_VOID);
                    m_Lexer.addRule("case", token_CASE);
                    m_Lexer.addRule("for", token_FOR);
                    m_Lexer.addRule("while", token_WHILE);
                    m_Lexer.addRule("switch", token_SWITCH);
                    m_Lexer.addRule(spell::Lexer::as_string(), "default", token_DEFAULT);
                    m_Lexer.addRule("do", token_DO);
                    m_Lexer.addRule("if", token_IF);
                    m_Lexer.addRule("else", token_ELSE);
                    m_Lexer.addRule("return", token_RETURN);
                    m_Lexer.addRule("goto", token_GOTO);
                    m_Lexer.addRule("new", token_NEW);
                    m_Lexer.addRule(spell::Lexer::as_string(), "delete", token_DELETE);
                    m_Lexer.addRule("explicit", token_EXPLICIT);
                    m_Lexer.addRule("typedef", token_TYPEDEF);
                    m_Lexer.addRule("using", token_USING);
                    m_Lexer.addRule(spell::Lexer::as_string(), "reinterpret_cast", token_REINTERPRET_CAST);
                    m_Lexer.addRule(spell::Lexer::as_string(), "static_cast", token_STATIC_CAST);
                    m_Lexer.addRule(spell::Lexer::as_string(), "const_cast", token_CONST_CAST);
                    m_Lexer.addRule(spell::Lexer::as_string(), "sizeof", token_SIZEOF);
                    m_Lexer.addRule(spell::Lexer::as_string(), "alignof", token_ALIGNOF);
                    m_Lexer.addRule("alignas", token_ALIGNAS);
                    m_Lexer.addRule("friend", token_FRIEND);
                    m_Lexer.addRule(spell::Lexer::as_string(), "typeof", token_TYPEOF);
                    m_Lexer.addRule(spell::Lexer::as_string(), "classof", token_CLASSOF);
                    m_Lexer.addRule(spell::Lexer::as_string(), "enumof", token_ENUMOF);
                    m_Lexer.addRule(spell::Lexer::as_string(), "unionof", token_UNIONOF);
                    m_Lexer.addRule("import", token_IMPORT);
                    m_Lexer.addRule("#include", token_INCLUDE);
                    m_Lexer.addRule("@module", token_MODULE);
                    m_Lexer.addRule("@function", token_FUNCTION);
                    m_Lexer.addRule("@function_ptr", token_FUNCTION_PTR);
                    m_Lexer.addRule("@delegate", token_DELEGATE);
                    m_Lexer.addRule("assert", token_ASSERT);
                    m_Lexer.addRule("static_assert", token_STATIC_ASSERT);
                    m_Lexer.addRule(spell::Lexer::as_string(), "@is_same", token_IS_SAME);
                    m_Lexer.addRule(spell::Lexer::as_string(), "const", token_CONST);
                    m_Lexer.addRule(spell::Lexer::as_string(), "volatile", token_VOLATILE);
                    m_Lexer.addRule("decltype", token_DECLTYPE);
                    m_Lexer.addRule(spell::Lexer::as_string(), "final", token_FINAL);
                    m_Lexer.addRule(spell::Lexer::as_string(), "virtual", token_VIRTUAL);
                    m_Lexer.addRule(spell::Lexer::as_string(), "static", token_STATIC);
                    m_Lexer.addRule(spell::Lexer::as_ulonglong(), "0[xX][0-9a-fA-F]+([uU]?[Ll][Ll]?)?", token_HEX_VALUE);
                    m_Lexer.addRule(spell::Lexer::as_float(), "(([0-9]+\\.[0-9]*([eE][-+]?[0-9]+)?)|(\\.[0-9]+([eE][-+]?[0-9]+)?))f", token_FLOAT_VALUE);
                    m_Lexer.addRule(spell::Lexer::as_double(), "(([0-9]+\\.[0-9]*([eE][-+]?[0-9]+)?)|(\\.[0-9]+([eE][-+]?[0-9]+)?))", token_DOUBLE_VALUE);
                    m_Lexer.addRule(spell::Lexer::as_ulonglong(), "[0-9]+[uU][Ll][Ll]", token_ULONGLONG_VALUE);
                    m_Lexer.addRule(spell::Lexer::as_ulong(), "[0-9]+[uU][Ll]", token_ULONG_VALUE);
                    m_Lexer.addRule(spell::Lexer::as_uint(), "[0-9]+[uU]", token_UINT_VALUE);
                    m_Lexer.addRule(spell::Lexer::as_longlong(), "[0-9]+[Ll][Ll]", token_LONGLONG_VALUE);
                    m_Lexer.addRule(spell::Lexer::as_long(), "[0-9]+[Ll]", token_LONG_VALUE);
                    m_Lexer.addRule(spell::Lexer::as_int(), "[0-9]+", token_INT_VALUE);
                    m_Lexer.addRule(spell::Lexer::as_bool(), "true|false", token_BOOL_VALUE);
                    m_Lexer.addPattern("SCHAR", "\\\\(x[0-9a-fA-F][0-9a-fA-F])|(u[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F])|(\\\\[\\\"'0tbrnafv\\\\])|[^\"\\\\'\\r\\n]");
                    m_Lexer.addRule(spell::Lexer::as_string(), "\\\"('|{SCHAR})*?\\\"", token_STRING_LIT);
                    m_Lexer.addRule(spell::Lexer::as_string(), "'(\\\"|{SCHAR})'", token_CHAR_LIT);
                    m_Lexer.addRule(spell::Lexer::as_string(), "_PHNTM_[a-zA-Z_][a-zA-Z0-9_]*", token_RESERVED_PHANTOM_KEYWORD);
                    m_Lexer.addRule(spell::Lexer::as_string(), "[a-zA-Z_][a-zA-Z0-9_]*", token_IDENTIFIER);
                }
                bool readSource(Source*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Source);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::ModuleDeclaration* temp0=nullptr;
                        readModuleDeclaration(temp0);
                        if(endTry())
                        {
                            if(__element__->m_ModuleDeclarations == nullptr)
                            __element__->m_ModuleDeclarations = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::ModuleDeclaration*,4ull>);
                            __element__->m_ModuleDeclarations->push_back(temp0);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp0); 
                            break;
                        }
                    }
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::ImportDeclaration* temp1=nullptr;
                        readImportDeclaration(temp1);
                        if(endTry())
                        {
                            if(__element__->m_ImportDeclarations == nullptr)
                            __element__->m_ImportDeclarations = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::ImportDeclaration*,4ull>);
                            __element__->m_ImportDeclarations->push_back(temp1);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp1); 
                            break;
                        }
                    }
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::SourceDeclaration* temp2=nullptr;
                        readSourceDeclaration(temp2);
                        if(endTry())
                        {
                            if(__element__->m_SourceDeclarations == nullptr)
                            __element__->m_SourceDeclarations = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::SourceDeclaration*,4ull>);
                            __element__->m_SourceDeclarations->push_back(temp2);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp2); 
                            break;
                        }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readModuleDeclaration(ModuleDeclaration*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ModuleDeclaration);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_MODULE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_Names = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<char const*,4ull>);
                    char const* temp3=nullptr;
                    if(!read(token_IDENTIFIER, temp3)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_Names->push_back(temp3);
                    while(token(token_DOT, m_TokenPointer + 0))
                    {
                        consume();
                        if(!read(token_IDENTIFIER, temp3)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_Names->push_back(temp3);
                    }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readImportDeclaration(ImportDeclaration*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ImportDeclaration);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(lookahead(token_IMPORT))
                    {
                        if(!read(token_IMPORT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(token() == token_STATIC)
                        {
                            valueAs(__element__->m_STATIC);
                            consume();
                        }
                        if(token() == token_PUBLIC)
                        {
                            valueAs(__element__->m_PUBLIC);
                            consume();
                        }
                        __element__->m_Imports = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Import*,4ull>);
                        phantom::lang::ast::Import* temp4=nullptr;
                        if(!readImport(temp4)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_Imports->push_back(temp4);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            consume();
                            if(!readImport(temp4)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Imports->push_back(temp4);
                        }
                        if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    else
                    {
                        if(!read(token_INCLUDE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(lookahead(token_LT))
                        {
                            if(!read(token_LT, __element__->m_LT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Names = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                            char const* temp5=nullptr;
                            if(!read(token_IDENTIFIER, temp5)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Names->push_back(temp5);
                            while(token(token_DOT, m_TokenPointer + 0))
                            {
                                consume();
                                if(!read(token_IDENTIFIER, temp5)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                __element__->m_Names->push_back(temp5);
                            }
                            if(!read(token_GT, __element__->m_GT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        else
                        {
                            if(!read(token_STRING_LIT, __element__->m_STRING_LIT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readImport(Import*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Import);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Import** __reduction__ = (Import**)&__element__;
                    bool __reductible__ = true;
                    if(!readImportBase(__element__->m_ImportBase)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(token() == token_COLON)
                    {
                        __reductible__ = false;
                        consume();
                        __element__->m_ImportBindings = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::ImportBinding*,4ull>);
                        phantom::lang::ast::ImportBinding* temp6=nullptr;
                        if(!readImportBinding(temp6)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_ImportBindings->push_back(temp6);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            __reductible__ = false;
                            consume();
                            if(!readImportBinding(temp6)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_ImportBindings->push_back(temp6);
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readImportBinding(ImportBinding*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ImportBinding);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    ImportBinding** __reduction__ = (ImportBinding**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(token() == token_ASSIGN)
                    {
                        __reductible__ = false;
                        valueAs(__element__->m_ASSIGN);
                        consume();
                        if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER0)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readImportBase(ImportBase*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ImportBase);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(lookahead(token_IDENTIFIER) && lookahead(token_ASSIGN))
                    {
                        if(!read(token_IDENTIFIER, __element__->m_Alias)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!read(token_ASSIGN, __element__->m_ASSIGN)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_Names = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                        char const* temp7=nullptr;
                        if(!read(token_IDENTIFIER, temp7)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_Names->push_back(temp7);
                        while(token(token_DOT, m_TokenPointer + 0))
                        {
                            consume();
                            if(!read(token_IDENTIFIER, temp7)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Names->push_back(temp7);
                        }
                    }
                    else
                    {
                        __element__->m_Names = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                        char const* temp8=nullptr;
                        if(!read(token_IDENTIFIER, temp8)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_Names->push_back(temp8);
                        while(token(token_DOT, m_TokenPointer + 0))
                        {
                            consume();
                            if(!read(token_IDENTIFIER, temp8)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Names->push_back(temp8);
                        }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readMeta(Meta*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Meta);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    __element__->m_MetaDecls = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::MetaDecl*,4ull>);
                    phantom::lang::ast::MetaDecl* temp9=nullptr;
                    if(!readMetaDecl(temp9)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_MetaDecls = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::MetaDecl*,4ull>);
                    __element__->m_MetaDecls->push_back(temp9);
                    while(!endOfFile())
                    {
                        beginTry();
                        readMetaDecl(temp9);
                        if(endTry())
                        {
                            __element__->m_MetaDecls->push_back(temp9);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp9); 
                            break;
                        }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readMetaDecl(MetaDecl*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(MetaDecl);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_BRACKET_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_STRING_LIT, __element__->m_Key)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(token() == token_ASSIGN)
                    {
                        valueAs(__element__->m_ASSIGN);
                        consume();
                        if(!readAssignExpression(__element__->m_Value)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(!read(token_BRACKET_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAnnotation(Annotation*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Annotation);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_BRACKET_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_BRACKET_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_BRACKET_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_BRACKET_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readMetaOrAnnotation(MetaOrAnnotation*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(MetaOrAnnotation);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    MetaOrAnnotation** __reduction__ = (MetaOrAnnotation**)&__element__;
                    bool __reductible__ = true;
                    beginTry();
                    readMeta(__element__->m_Meta);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readAnnotation(__element__->m_Annotation)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readSourceDeclaration(SourceDeclaration*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(SourceDeclaration);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    SourceDeclaration** __reduction__ = (SourceDeclaration**)&__element__;
                    bool __reductible__ = true;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::MetaOrAnnotation* temp10=nullptr;
                        readMetaOrAnnotation(temp10);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_MetaOrAnnotations == nullptr)
                            __element__->m_MetaOrAnnotations = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::MetaOrAnnotation*,4ull>);
                            __element__->m_MetaOrAnnotations->push_back(temp10);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp10); 
                            break;
                        }
                    }
                    if(!readDeclaration(__element__->m_Declaration)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readDeclaration(Declaration*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Declaration);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Declaration** __reduction__ = (Declaration**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_CLASS:
                        case token_STRUCT:
                        {
                            if(!readClass(__element__->m_Class)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_ENUM:
                        {
                            if(!readEnum(__element__->m_Enum)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_USING:
                        {
                            beginTry();
                            readAlias(__element__->m_Alias);
                            if(endTry())
                            {
                            }
                            else
                            {
                                if(!readUsing(__element__->m_Using)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            }
                        }
                        break;
                        case token_TYPEDEF:
                        {
                            if(!readTypedef(__element__->m_Typedef)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_TEMPLATE:
                        {
                            if(!readTemplate(__element__->m_Template)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_ATAT:
                        {
                            if(!readMixinStatementDefinition(__element__->m_MixinStatementDefinition)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_STATIC_ASSERT:
                        {
                            if(!readStaticAssert(__element__->m_StaticAssert)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_SEMI_COLON:
                        {
                            if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        default:
                        if(!readFunctionOrVariable(__element__->m_FunctionOrVariable)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readMixinStatementDefinition(MixinStatementDefinition*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(MixinStatementDefinition);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_ATAT, __element__->m_ATAT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    char const* temp11=nullptr;
                    beginTry();
                    read(token_IDENTIFIER, temp11);
                    if(endTry())
                    {
                        __element__->m_IDENTIFIERs = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                        __element__->m_IDENTIFIERs->push_back(temp11);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            consume();
                            char const* temp12=nullptr;
                            if(!read(token_IDENTIFIER, temp12)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_IDENTIFIERs->push_back(temp12);
                        }
                    }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_BLOCK_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::Statement* temp13=nullptr;
                        readStatement(temp13);
                        if(endTry())
                        {
                            if(__element__->m_Statements == nullptr)
                            __element__->m_Statements = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Statement*,4ull>);
                            __element__->m_Statements->push_back(temp13);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp13); 
                            break;
                        }
                    }
                    if(!read(token_BLOCK_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBlockDeclaration(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BlockDeclaration);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Statement** __reduction__ = (Statement**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_CLASS:
                        case token_STRUCT:
                        {
                            if(!readClass(__element__->m_Class)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_ENUM:
                        {
                            if(!readEnum(__element__->m_Enum)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_TYPEDEF:
                        {
                            if(!readTypedef(__element__->m_Typedef)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_USING:
                        {
                            if(!readUsing(__element__->m_Using)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_STATIC_ASSERT:
                        {
                            if(!readStaticAssert(__element__->m_StaticAssert)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        default:
                        if(!readAlias(__element__->m_Alias)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readConversionFunction(ConversionFunction*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ConversionFunction);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_OPERATOR)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    while(!endOfFile())
                    {
                        beginTry();
                        char const* temp14=nullptr;
                        read({token_CONST,token_VOLATILE,token_LOG_AND,token_BIN_AND}, temp14);
                        if(endTry())
                        {
                            if(__element__->m_CONSTs == nullptr)
                            __element__->m_CONSTs = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                            __element__->m_CONSTs->push_back(temp14);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(!readFunctionBlock(__element__->m_FunctionBlock)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readConstructor(Constructor*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Constructor);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(token() == token_EXPLICIT)
                    {
                        consume();
                    }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::TemplateArgumentList* temp15=nullptr;
                    readTemplateArgumentList(temp15);
                    if(endTry())
                    {
                        __element__->m_TemplateArgumentList=temp15;
                    }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    phantom::lang::ast::Parameter* temp16=nullptr;
                    beginTry();
                    readParameter(temp16);
                    if(endTry())
                    {
                        __element__->m_Parameters = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Parameter*,4ull>);
                        __element__->m_Parameters->push_back(temp16);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            consume();
                            phantom::lang::ast::Parameter* temp17=nullptr;
                            if(!readParameter(temp17)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Parameters->push_back(temp17);
                        }
                    }
                    else PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp16);
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(lookahead(token_ASSIGN))
                    {
                        char const* temp18=nullptr;
                        if(!read(token_ASSIGN, temp18)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!read({token_DELETE,token_DEFAULT}, __element__->m_Suffix)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    else
                    {
                        if(token() == token_COLON)
                        {
                            consume();
                            __element__->m_ConstructorInitializers = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::ConstructorInitializer*,4ull>);
                            phantom::lang::ast::ConstructorInitializer* temp19=nullptr;
                            if(!readConstructorInitializer(temp19)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_ConstructorInitializers->push_back(temp19);
                            while(token(token_COMMA, m_TokenPointer + 0))
                            {
                                consume();
                                if(!readConstructorInitializer(temp19)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                __element__->m_ConstructorInitializers->push_back(temp19);
                            }
                        }
                        if(!readFunctionBlock(__element__->m_FunctionBlock)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readDestructor(Destructor*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Destructor);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_COMPL, __element__->m_COMPL)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    while(!endOfFile())
                    {
                        beginTry();
                        char const* temp20=nullptr;
                        read({token_OVERRIDE,token_FINAL}, temp20);
                        if(endTry())
                        {
                            if(__element__->m_OVERRIDEs == nullptr)
                            __element__->m_OVERRIDEs = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                            __element__->m_OVERRIDEs->push_back(temp20);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(!readFunctionBlock(__element__->m_FunctionBlock)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readConstructorInitializer(ConstructorInitializer*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ConstructorInitializer);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    ConstructorInitializer** __reduction__ = (ConstructorInitializer**)&__element__;
                    bool __reductible__ = true;
                    if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    switch(token())
                    {
                        case token_PAREN_START:
                        {
                            if(!readCallList(__element__->m_CallList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_BLOCK_START:
                        {
                            if(!readInitializerList(__element__->m_InitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readFunctionEnd(FunctionEnd*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(FunctionEnd);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    phantom::lang::ast::Parameter* temp21=nullptr;
                    beginTry();
                    readParameter(temp21);
                    if(endTry())
                    {
                        __element__->m_Parameters = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Parameter*,4ull>);
                        __element__->m_Parameters->push_back(temp21);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            consume();
                            phantom::lang::ast::Parameter* temp22=nullptr;
                            if(!readParameter(temp22)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Parameters->push_back(temp22);
                        }
                    }
                    else PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp21);
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    while(!endOfFile())
                    {
                        beginTry();
                        char const* temp23=nullptr;
                        read({token_CONST,token_VOLATILE,token_LOG_AND,token_BIN_AND}, temp23);
                        if(endTry())
                        {
                            if(__element__->m_CONSTs == nullptr)
                            __element__->m_CONSTs = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                            __element__->m_CONSTs->push_back(temp23);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(lookahead({token_BLOCK_START,token_OVERRIDE,token_FINAL}))
                    {
                        while(!endOfFile())
                        {
                            beginTry();
                            char const* temp24=nullptr;
                            read({token_OVERRIDE,token_FINAL}, temp24);
                            if(endTry())
                            {
                                if(__element__->m_OVERRIDEs == nullptr)
                                __element__->m_OVERRIDEs = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                                __element__->m_OVERRIDEs->push_back(temp24);
                            }
                            else
                            {
                                break;
                            }
                        }
                        if(!readFunctionBlock(__element__->m_FunctionBlock)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    else
                    {
                        char const* temp25=nullptr;
                        if(!read(token_ASSIGN, temp25)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!read({token_INT_VALUE,token_DELETE,token_DEFAULT}, __element__->m_Suffix)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(lookahead(token_BLOCK_START))
                        {
                            if(!readFunctionBlock(__element__->m_FunctionBlock)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        else
                        {
                            if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readOperator(Operator*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Operator);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_OPERATOR)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    switch(token())
                    {
                        case token_PAREN_START:
                        {
                            __element__->m_OpParen = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>)(2);
                            if(!read(token_PAREN_START, (*__element__->m_OpParen)[0])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(!readContiguous(token_PAREN_END, (*__element__->m_OpParen)[1])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_BRACKET_START:
                        {
                            __element__->m_OpBracket = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>)(2);
                            if(!read(token_BRACKET_START, (*__element__->m_OpBracket)[0])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(!readContiguous(token_BRACKET_END, (*__element__->m_OpBracket)[1])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_GT:
                        {
                            if(lookahead(token_GT) && lookahead(token_GE))
                            {
                                __element__->m_RightShiftAssign = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>)(2);
                                if(!read(token_GT, (*__element__->m_RightShiftAssign)[0])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                if(!readContiguous(token_GE, (*__element__->m_RightShiftAssign)[1])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            }
                            else
                            {
                                if(lookahead(token_GT) && lookahead(token_GT))
                                {
                                    __element__->m_RightShift = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>)(2);
                                    if(!read(token_GT, (*__element__->m_RightShift)[0])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                    if(!readContiguous(token_GT, (*__element__->m_RightShift)[1])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                }
                                else
                                {
                                    if(!read(token_GT, __element__->m_GreaterThan)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                }
                            }
                        }
                        break;
                        default:
                        if(!read({token_ASSIGN,token_ASS_ADD,token_ASS_AND,token_ASS_DIV,token_ASS_MOD,token_ASS_MUL,token_ASS_OR,token_ASS_SHL,token_ASS_SUB,token_ASS_XOR,token_BIN_OR,token_INC,token_DEC,token_MUL,token_PLUS,token_MINUS,token_DIV,token_EQ,token_NE,token_XOR,token_SHL,token_LT,token_LE,token_GE,token_NOT,token_ARROW}, __element__->m_ASSIGN)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        break;
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readOperatorEnd(OperatorEnd*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(OperatorEnd);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!readOperator(__element__->m_Operator)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readFunctionEnd(__element__->m_FunctionEnd)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readFunctionOrVariableEnd(FunctionOrVariableEnd*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(FunctionOrVariableEnd);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    switch(token())
                    {
                        case token_BLOCK_START:
                        {
                            if(!readInitializerList(__element__->m_InitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_BRACKET_START:
                        {
                            __element__->m_ArrayExtents = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>);
                            phantom::lang::ast::TypeExtent* temp26=nullptr;
                            if(!readArrayExtent(temp26)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_ArrayExtents = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>);
                            __element__->m_ArrayExtents->push_back(temp26);
                            while(!endOfFile())
                            {
                                beginTry();
                                readArrayExtent(temp26);
                                if(endTry())
                                {
                                    __element__->m_ArrayExtents->push_back(temp26);
                                }
                                else
                                {
                                    PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp26); 
                                    break;
                                }
                            }
                            if(token() == token_ASSIGN)
                            {
                                consume();
                                if(!readAssignExpressionOrInitializerList(__element__->m_AssignExpressionOrInitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            }
                            if(token() == token_COMMA)
                            {
                                consume();
                                __element__->m_Declarators = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Declarator*,4ull>);
                                phantom::lang::ast::Declarator* temp27=nullptr;
                                if(!readDeclarator(temp27)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                __element__->m_Declarators->push_back(temp27);
                                while(token(token_COMMA, m_TokenPointer + 0))
                                {
                                    consume();
                                    if(!readDeclarator(temp27)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                    __element__->m_Declarators->push_back(temp27);
                                }
                            }
                            if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_LT:
                        {
                            if(!readTemplateArgumentList(__element__->m_TemplateArgumentList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(!readFunctionEnd(__element__->m_FunctionEnd)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_PAREN_START:
                        {
                            beginTry();
                            readFunctionEnd(__element__->m_FunctionEnd);
                            if(endTry())
                            {
                            }
                            else
                            {
                                if(!readCallList(__element__->m_CallList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                if(token() == token_COMMA)
                                {
                                    consume();
                                    __element__->m_Declarators = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Declarator*,4ull>);
                                    phantom::lang::ast::Declarator* temp28=nullptr;
                                    if(!readDeclarator(temp28)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                    __element__->m_Declarators->push_back(temp28);
                                    while(token(token_COMMA, m_TokenPointer + 0))
                                    {
                                        consume();
                                        if(!readDeclarator(temp28)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                        __element__->m_Declarators->push_back(temp28);
                                    }
                                }
                                if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            }
                        }
                        break;
                        case token_ASSIGN:
                        {
                            char const* temp29=nullptr;
                            if(!read(token_ASSIGN, temp29)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(!readAssignExpressionOrInitializerList(__element__->m_AssignExpressionOrInitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(token() == token_COMMA)
                            {
                                consume();
                                __element__->m_Declarators = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Declarator*,4ull>);
                                phantom::lang::ast::Declarator* temp30=nullptr;
                                if(!readDeclarator(temp30)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                __element__->m_Declarators->push_back(temp30);
                                while(token(token_COMMA, m_TokenPointer + 0))
                                {
                                    consume();
                                    if(!readDeclarator(temp30)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                    __element__->m_Declarators->push_back(temp30);
                                }
                            }
                            if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        default:
                        if(token() == token_COMMA)
                        {
                            consume();
                            __element__->m_Declarators = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Declarator*,4ull>);
                            phantom::lang::ast::Declarator* temp31=nullptr;
                            if(!readDeclarator(temp31)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Declarators->push_back(temp31);
                            while(token(token_COMMA, m_TokenPointer + 0))
                            {
                                consume();
                                if(!readDeclarator(temp31)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                __element__->m_Declarators->push_back(temp31);
                            }
                        }
                        if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        break;
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readMethodSpecifier(MethodSpecifier*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(MethodSpecifier);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_STATIC,token_VIRTUAL}, __element__->m_STATIC)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readSpecifier(Specifier*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Specifier);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Specifier** __reduction__ = (Specifier**)&__element__;
                    bool __reductible__ = true;
                    if(!readMethodSpecifier(__element__->m_MethodSpecifier)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readFunctionOrVariable(FunctionOrVariable*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(FunctionOrVariable);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::Specifier* temp32=nullptr;
                        readSpecifier(temp32);
                        if(endTry())
                        {
                            if(__element__->m_Specifiers == nullptr)
                            __element__->m_Specifiers = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Specifier*,4ull>);
                            __element__->m_Specifiers->push_back(temp32);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp32); 
                            break;
                        }
                    }
                    if(token() == token_ALIGNAS)
                    {
                        consume();
                        if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!read(token_INT_VALUE, __element__->m_AlignAs)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(!readTypeOrAuto(__element__->m_TypeOrAuto)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(lookahead(token_OPERATOR))
                    {
                        if(!readOperatorEnd(__element__->m_OperatorEnd)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    else
                    {
                        if(!readFunctionOrVariableEnd(__element__->m_FunctionOrVariableEnd)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readDeclType(DeclType*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(DeclType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_DECLTYPE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTemplateArgument(TemplateArgument*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TemplateArgument);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readLastTemplateArgumentAssign(LastTemplateArgument*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(LastTemplateArgumentAssign);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!readTemplateArgumentAssign(__element__->m_TemplateArgumentAssign)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    char const* temp33=nullptr;
                    if(!read(token_GT, temp33)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readLastTemplateArgumentNoAssign(LastTemplateArgument*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(LastTemplateArgumentNoAssign);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!readTemplateArgumentNoAssign(__element__->m_TemplateArgumentNoAssign)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    char const* temp34=nullptr;
                    if(!read(token_GT, temp34)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readLastTemplateArgument(TemplateArgument*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(LastTemplateArgument);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    TemplateArgument** __reduction__ = (TemplateArgument**)&__element__;
                    bool __reductible__ = true;
                    beginTry();
                    readLastTemplateArgumentAssign(__element__->m_LastTemplateArgumentAssign);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readLastTemplateArgumentNoAssign(__element__->m_LastTemplateArgumentNoAssign)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readNotLastTemplateArgumentAssign(NotLastTemplateArgument*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(NotLastTemplateArgumentAssign);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!readTemplateArgumentAssign(__element__->m_TemplateArgumentAssign)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_COMMA)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readNotLastTemplateArgumentNoAssign(NotLastTemplateArgument*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(NotLastTemplateArgumentNoAssign);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!readTemplateArgumentNoAssign(__element__->m_TemplateArgumentNoAssign)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_COMMA)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readNotLastTemplateArgument(TemplateArgument*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(NotLastTemplateArgument);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    TemplateArgument** __reduction__ = (TemplateArgument**)&__element__;
                    bool __reductible__ = true;
                    beginTry();
                    readNotLastTemplateArgumentAssign(__element__->m_NotLastTemplateArgumentAssign);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readNotLastTemplateArgumentNoAssign(__element__->m_NotLastTemplateArgumentNoAssign)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTemplateArgumentListSplit(TemplateArgumentListSplit*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TemplateArgumentListSplit);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    TemplateArgumentListSplit** __reduction__ = (TemplateArgumentListSplit**)&__element__;
                    bool __reductible__ = true;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::TemplateArgument* temp35=nullptr;
                        readNotLastTemplateArgument(temp35);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_NotLastTemplateArguments == nullptr)
                            __element__->m_NotLastTemplateArguments = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TemplateArgument*,4ull>);
                            __element__->m_NotLastTemplateArguments->push_back(temp35);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp35); 
                            break;
                        }
                    }
                    if(!readLastTemplateArgument(__element__->m_LastTemplateArgument)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTemplateArgumentNoAssign(TemplateArgumentNoAssign*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TemplateArgumentNoAssign);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    TemplateArgumentNoAssign** __reduction__ = (TemplateArgumentNoAssign**)&__element__;
                    bool __reductible__ = true;
                    if(!readPostFixExpression(__element__->m_PostFixExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTemplateArgumentAssign(TemplateArgumentAssign*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TemplateArgumentAssign);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    TemplateArgumentAssign** __reduction__ = (TemplateArgumentAssign**)&__element__;
                    bool __reductible__ = true;
                    beginTry();
                    read({token_INT_VALUE,token_UINT_VALUE,token_LONG_VALUE,token_ULONG_VALUE,token_LONG_VALUE,token_ULONGLONG_VALUE,token_HEX_VALUE}, __element__->m_INT_VALUE);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTemplateArgumentList(TemplateArgumentList*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TemplateArgumentList);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    char const* temp36=nullptr;
                    if(!read(token_LT, temp36)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(lookahead(token_GT))
                    {
                        if(!read(token_GT, __element__->m_GT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    else
                    {
                        if(!readTemplateArgumentListSplit(__element__->m_TemplateArgumentListSplit)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readFunctionPointerType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(FunctionPointerType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_FUNCTION)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    phantom::lang::ast::Type* temp37=nullptr;
                    beginTry();
                    readType(temp37);
                    if(endTry())
                    {
                        __element__->m_Types = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Type*,4ull>);
                        __element__->m_Types->push_back(temp37);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            consume();
                            phantom::lang::ast::Type* temp38=nullptr;
                            if(!readType(temp38)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Types->push_back(temp38);
                        }
                    }
                    else PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp37);
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readName(Name*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Name);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Name** __reduction__ = (Name**)&__element__;
                    bool __reductible__ = true;
                    if(lookahead(token_OPERATOR))
                    {
                        if(!readOperator(__element__->m_Operator)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    else
                    {
                        if(token() == token_COMPL)
                        {
                            __reductible__ = false;
                            valueAs(__element__->m_COMPL);
                            consume();
                        }
                        if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        beginTry();
                        readTemplateArgumentList(__element__->m_TemplateArgumentList);
                        if(endTry())
                        {
                        }
                        else
                        {
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readFundamentalTypeBase(FundamentalTypeBase*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(FundamentalTypeBase);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_BOOL,token_CHAR,token_SHORT,token_INT,token_LONG,token_FLOATING_POINT_TYPE}, __element__->m_Typename)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readUnsignedType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(UnsignedType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_UNSIGNED, __element__->m_UNSIGNED)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::Type* temp39=nullptr;
                    readType(temp39);
                    if(endTry())
                    {
                        __element__->m_Type=temp39;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readSignedType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(SignedType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_SIGNED, __element__->m_SIGNED)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::Type* temp40=nullptr;
                    readType(temp40);
                    if(endTry())
                    {
                        __element__->m_Type=temp40;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readLongType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(LongType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_LONG, __element__->m_LONG)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::Type* temp41=nullptr;
                    readType(temp41);
                    if(endTry())
                    {
                        __element__->m_Type=temp41;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readVoidType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(VoidType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_VOID, __element__->m_VOID)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readFundamentalType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(FundamentalType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_UNSIGNED:
                        {
                            if(!readUnsignedType(__element__->m_UnsignedType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_UnsignedType;
                        }
                        break;
                        case token_SIGNED:
                        {
                            if(!readSignedType(__element__->m_SignedType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_SignedType;
                        }
                        break;
                        case token_LONG:
                        {
                            if(!readLongType(__element__->m_LongType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_LongType;
                        }
                        break;
                        case token_VOID:
                        {
                            if(!readVoidType(__element__->m_VoidType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_VoidType;
                        }
                        break;
                        default:
                        if(!readFundamentalTypeBase(__element__->m_FundamentalTypeBase)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readQualifiedType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(QualifiedType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    if(!readQualifiedName(__element__->m_QualifiedName)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::Type* temp42=nullptr;
                    readFunctionPointerType(temp42);
                    if(endTry())
                    {
                        __element__->m_FunctionPointerType=temp42;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readExplicitType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ExplicitType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    while(!endOfFile())
                    {
                        beginTry();
                        char const* temp43=nullptr;
                        read({token_CONST,token_VOLATILE}, temp43);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_CONSTs == nullptr)
                            __element__->m_CONSTs = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                            __element__->m_CONSTs->push_back(temp43);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(!readBasicType(__element__->m_BasicType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_BasicType;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::TypeExtent* temp44=nullptr;
                        readTypeExtent(temp44);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_TypeExtents == nullptr)
                            __element__->m_TypeExtents = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>);
                            __element__->m_TypeExtents->push_back(temp44);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp44); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readExpressionType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ExpressionType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    if(!readAssignExpression(__element__->m_AssignExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Type);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    beginTry();
                    readExplicitType(__element__->m_ExplicitType);
                    __reduction__ = &__element__->m_ExplicitType;
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readExpressionType(__element__->m_ExpressionType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_ExpressionType;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readExplicitTypeOrAuto(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ExplicitTypeOrAuto);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    while(!endOfFile())
                    {
                        beginTry();
                        char const* temp45=nullptr;
                        read({token_CONST,token_VOLATILE}, temp45);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_CONSTs == nullptr)
                            __element__->m_CONSTs = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                            __element__->m_CONSTs->push_back(temp45);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(!readBasicTypeOrAuto(__element__->m_BasicTypeOrAuto)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_BasicTypeOrAuto;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::TypeExtent* temp46=nullptr;
                        readTypeExtent(temp46);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_TypeExtents == nullptr)
                            __element__->m_TypeExtents = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>);
                            __element__->m_TypeExtents->push_back(temp46);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp46); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTypeOrAuto(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TypeOrAuto);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    beginTry();
                    readExplicitTypeOrAuto(__element__->m_ExplicitTypeOrAuto);
                    __reduction__ = &__element__->m_ExplicitTypeOrAuto;
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readExpressionType(__element__->m_ExpressionType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_ExpressionType;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAutoType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(AutoType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    while(!endOfFile())
                    {
                        beginTry();
                        char const* temp47=nullptr;
                        read({token_CONST,token_VOLATILE}, temp47);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_CONSTs == nullptr)
                            __element__->m_CONSTs = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                            __element__->m_CONSTs->push_back(temp47);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(!readAuto(__element__->m_Auto)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_Auto;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::TypeExtent* temp48=nullptr;
                        readTypeExtent(temp48);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_TypeExtents == nullptr)
                            __element__->m_TypeExtents = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>);
                            __element__->m_TypeExtents->push_back(temp48);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp48); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBasicType(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BasicType);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_UNSIGNED:
                        case token_SIGNED:
                        case token_BOOL:
                        case token_SHORT:
                        case token_INT:
                        case token_LONG:
                        case token_CHAR:
                        case token_FLOATING_POINT_TYPE:
                        case token_VOID:
                        {
                            if(!readFundamentalType(__element__->m_FundamentalType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_FundamentalType;
                        }
                        break;
                        case token_DECLTYPE:
                        {
                            if(!readDeclType(__element__->m_DeclType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_FUNCTION:
                        {
                            if(!readFunctionPointerType(__element__->m_FunctionPointerType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_FunctionPointerType;
                        }
                        break;
                        default:
                        if(!readQualifiedType(__element__->m_QualifiedType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_QualifiedType;
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAuto(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Auto);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_AUTO, __element__->m_AUTO)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBasicTypeOrAuto(Type*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BasicTypeOrAuto);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Type** __reduction__ = (Type**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_UNSIGNED:
                        case token_SIGNED:
                        case token_BOOL:
                        case token_SHORT:
                        case token_INT:
                        case token_LONG:
                        case token_CHAR:
                        case token_FLOATING_POINT_TYPE:
                        case token_VOID:
                        {
                            if(!readFundamentalType(__element__->m_FundamentalType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_FundamentalType;
                        }
                        break;
                        case token_AUTO:
                        {
                            if(!readAuto(__element__->m_Auto)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_Auto;
                        }
                        break;
                        case token_DECLTYPE:
                        {
                            if(!readDeclType(__element__->m_DeclType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_FUNCTION:
                        {
                            if(!readFunctionPointerType(__element__->m_FunctionPointerType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_FunctionPointerType;
                        }
                        break;
                        default:
                        if(!readQualifiedType(__element__->m_QualifiedType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_QualifiedType;
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readIntegralConstant(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(IntegralConstant);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_INT_VALUE,token_UINT_VALUE,token_LONG_VALUE,token_ULONG_VALUE,token_LONGLONG_VALUE,token_ULONGLONG_VALUE,token_BOOL_VALUE,token_CHAR_LIT,token_HEX_VALUE}, __element__->m_INT_VALUE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readArrayExtentExpression(ArrayExtentExpression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ArrayExtentExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    ArrayExtentExpression** __reduction__ = (ArrayExtentExpression**)&__element__;
                    bool __reductible__ = true;
                    beginTry();
                    readIntegralConstant(__element__->m_IntegralConstant);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readAssignExpression(__element__->m_AssignExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readArrayExtent(TypeExtent*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ArrayExtent);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_BRACKET_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::ArrayExtentExpression* temp49=nullptr;
                    readArrayExtentExpression(temp49);
                    if(endTry())
                    {
                        __element__->m_ArrayExtentExpression=temp49;
                    }
                    if(!read(token_BRACKET_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readDerivedExtent(TypeExtent*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(DerivedExtent);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_BIN_AND,token_LOG_AND,token_MUL,token_CONST,token_VOLATILE}, __element__->m_BIN_AND)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTypeExtent(TypeExtent*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TypeExtent);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    TypeExtent** __reduction__ = (TypeExtent**)&__element__;
                    bool __reductible__ = true;
                    if(lookahead(token_BRACKET_START))
                    {
                        if(!readArrayExtent(__element__->m_ArrayExtent)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_ArrayExtent;
                    }
                    else
                    {
                        if(!readDerivedExtent(__element__->m_DerivedExtent)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_DerivedExtent;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readEnum(Enum*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Enum);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_ENUM)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(token() == token_CLASS)
                    {
                        valueAs(__element__->m_CLASS);
                        consume();
                    }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_BLOCK_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    phantom::lang::ast::EnumConstant* temp50=nullptr;
                    beginTry();
                    readEnumConstant(temp50);
                    if(endTry())
                    {
                        __element__->m_EnumConstants = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::EnumConstant*,4ull>);
                        __element__->m_EnumConstants->push_back(temp50);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            consume();
                            phantom::lang::ast::EnumConstant* temp51=nullptr;
                            if(!readEnumConstant(temp51)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_EnumConstants->push_back(temp51);
                        }
                    }
                    else PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp50);
                    if(!read(token_BLOCK_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readEnumConstant(EnumConstant*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(EnumConstant);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    EnumConstant** __reduction__ = (EnumConstant**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(token() == token_ASSIGN)
                    {
                        __reductible__ = false;
                        consume();
                        if(!readAssignExpression(__element__->m_AssignExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readUsing(Using*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Using);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_USING)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readQualifiedName(__element__->m_QualifiedName)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTypedef(Typedef*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Typedef);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_TYPEDEF)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAlias(Alias*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Alias);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_USING)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_ASSIGN, __element__->m_ASSIGN)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTemplateParameterDefault(TemplateParameterDefault*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TemplateParameterDefault);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    char const* temp52=nullptr;
                    if(!read(token_ASSIGN, temp52)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    readTemplateArgumentAssign(__element__->m_TemplateArgumentAssign);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readTemplateArgumentNoAssign(__element__->m_TemplateArgumentNoAssign)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTemplateValueParameter(TemplateValueParameter*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TemplateValueParameter);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(lookahead(token_IDENTIFIER))
                    {
                        if(!readQualifiedName(__element__->m_QualifiedName)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    else
                    {
                        if(!readFundamentalType(__element__->m_FundamentalType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTemplateTypeParameter(TemplateTypeParameter*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TemplateTypeParameter);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    TemplateTypeParameter** __reduction__ = (TemplateTypeParameter**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_CLASS:
                        case token_TYPENAME:
                        {
                            consume();
                        }
                        break;
                        default:
                        break;
                    }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTemplateParameter(TemplateParameter*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TemplateParameter);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    TemplateParameter** __reduction__ = (TemplateParameter**)&__element__;
                    bool __reductible__ = true;
                    beginTry();
                    readTemplateValueParameter(__element__->m_TemplateValueParameter);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readTemplateTypeParameter(__element__->m_TemplateTypeParameter)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(lookahead(token_ELLIPSE))
                    {
                        if(!read(token_ELLIPSE, __element__->m_ELLIPSE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    else
                    {
                        beginTry();
                        phantom::lang::ast::TemplateParameterDefault* temp53=nullptr;
                        readTemplateParameterDefault(temp53);
                        if(endTry())
                        {
                            __element__->m_TemplateParameterDefault=temp53;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTemplateSignature(TemplateSignature*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TemplateSignature);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_TEMPLATE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    char const* temp54=nullptr;
                    if(!read(token_LT, temp54)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    phantom::lang::ast::TemplateParameter* temp55=nullptr;
                    beginTry();
                    readTemplateParameter(temp55);
                    if(endTry())
                    {
                        __element__->m_TemplateParameters = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TemplateParameter*,4ull>);
                        __element__->m_TemplateParameters->push_back(temp55);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            consume();
                            phantom::lang::ast::TemplateParameter* temp56=nullptr;
                            if(!readTemplateParameter(temp56)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_TemplateParameters->push_back(temp56);
                        }
                    }
                    else PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp55);
                    char const* temp57=nullptr;
                    if(!read(token_GT, temp57)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTemplate(Template*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Template);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Template** __reduction__ = (Template**)&__element__;
                    bool __reductible__ = true;
                    if(!readTemplateSignature(__element__->m_TemplateSignature)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    switch(token())
                    {
                        case token_CLASS:
                        case token_STRUCT:
                        {
                            if(!readClass(__element__->m_Class)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_UNION:
                        {
                            if(!readUnion(__element__->m_Union)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_USING:
                        {
                            if(!readAlias(__element__->m_Alias)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readFunctionBlock(FunctionBlock*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(FunctionBlock);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_BLOCK_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::Statement* temp58=nullptr;
                        readStatement(temp58);
                        if(endTry())
                        {
                            if(__element__->m_Statements == nullptr)
                            __element__->m_Statements = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Statement*,4ull>);
                            __element__->m_Statements->push_back(temp58);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp58); 
                            break;
                        }
                    }
                    if(!read(token_BLOCK_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readQualifiedName(QualifiedName*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(QualifiedName);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    QualifiedName** __reduction__ = (QualifiedName**)&__element__;
                    bool __reductible__ = true;
                    if(token() == token_TYPENAME)
                    {
                        __reductible__ = false;
                        valueAs(__element__->m_TYPENAME);
                        consume();
                    }
                    beginTry();
                    readQualifiedScopedName(__element__->m_Scope);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readQualifiedDotName(__element__->m_Dot)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readQualifiedDotName(QualifiedDotName*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(QualifiedDotName);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(token() == token_DOT)
                    {
                        valueAs(__element__->m_DOT);
                        consume();
                    }
                    __element__->m_Names = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Name*,4ull>);
                    phantom::lang::ast::Name* temp59=nullptr;
                    if(!readName(temp59)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_Names->push_back(temp59);
                    while(token(token_DOT, m_TokenPointer + 0))
                    {
                        consume();
                        if(!readName(temp59)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_Names->push_back(temp59);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readQualifiedScopedName(QualifiedScopedName*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(QualifiedScopedName);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(lookahead(token_SCOPE))
                    {
                        __element__->m_Names = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Name*,4ull>);
                        phantom::lang::ast::Name* temp60=nullptr;
                        if(!readName(temp60)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_Names->push_back(temp60);
                        while(token(token_SCOPE, m_TokenPointer + 0))
                        {
                            consume();
                            if(!readName(temp60)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Names->push_back(temp60);
                        }
                    }
                    else
                    {
                        if(!readName(__element__->m_Name)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!read(token_SCOPE, __element__->m_SCOPE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_Names = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Name*,4ull>);
                        phantom::lang::ast::Name* temp61=nullptr;
                        if(!readName(temp61)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_Names->push_back(temp61);
                        while(token(token_SCOPE, m_TokenPointer + 0))
                        {
                            consume();
                            if(!readName(temp61)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Names->push_back(temp61);
                        }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBaseClass(BaseClass*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BaseClass);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    BaseClass** __reduction__ = (BaseClass**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_PUBLIC:
                        case token_PROTECTED:
                        case token_PRIVATE:
                        {
                            valueAs(__element__->m_PUBLIC);
                            __reductible__ = false;
                            consume();
                        }
                        break;
                        default:
                        break;
                    }
                    if(!readBasicType(__element__->m_BasicType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readClass(Class*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Class);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_CLASS,token_STRUCT}, __element__->m_CLASS)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(token() == token_PAREN_START)
                    {
                        valueAs(__element__->m_PAREN_START);
                        consume();
                        if(!readAssignExpression(__element__->m_MetaClassConstructionExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(token() == token_ALIGNAS)
                    {
                        consume();
                        if(!read(token_PAREN_START, __element__->m_PAREN_START0)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!read(token_INT_VALUE, __element__->m_AlignAs)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::TemplateArgumentList* temp62=nullptr;
                    readTemplateArgumentList(temp62);
                    if(endTry())
                    {
                        __element__->m_TemplateArgumentList=temp62;
                    }
                    if(token() == token_COLON)
                    {
                        consume();
                        __element__->m_BaseClasses = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::BaseClass*,4ull>);
                        phantom::lang::ast::BaseClass* temp63=nullptr;
                        if(!readBaseClass(temp63)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_BaseClasses->push_back(temp63);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            consume();
                            if(!readBaseClass(temp63)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_BaseClasses->push_back(temp63);
                        }
                    }
                    if(!read(token_BLOCK_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::Member* temp64=nullptr;
                        readMember(temp64);
                        if(endTry())
                        {
                            if(__element__->m_Members == nullptr)
                            __element__->m_Members = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Member*,4ull>);
                            __element__->m_Members->push_back(temp64);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp64); 
                            break;
                        }
                    }
                    if(!read(token_BLOCK_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readUnion(Union*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Union);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_UNION)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::TemplateArgumentList* temp65=nullptr;
                    readTemplateArgumentList(temp65);
                    if(endTry())
                    {
                        __element__->m_TemplateArgumentList=temp65;
                    }
                    if(!read(token_BLOCK_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::Member* temp66=nullptr;
                        readBasicMember(temp66);
                        if(endTry())
                        {
                            if(__element__->m_BasicMembers == nullptr)
                            __element__->m_BasicMembers = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Member*,4ull>);
                            __element__->m_BasicMembers->push_back(temp66);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp66); 
                            break;
                        }
                    }
                    if(!read(token_BLOCK_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readMethod(Method*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Method);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::MethodSpecifier* temp67=nullptr;
                        readMethodSpecifier(temp67);
                        if(endTry())
                        {
                            if(__element__->m_MethodSpecifiers == nullptr)
                            __element__->m_MethodSpecifiers = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::MethodSpecifier*,4ull>);
                            __element__->m_MethodSpecifiers->push_back(temp67);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp67); 
                            break;
                        }
                    }
                    if(!readTypeOrAuto(__element__->m_TypeOrAuto)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readFunctionEnd(__element__->m_FunctionEnd)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readParameter(Parameter*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Parameter);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Parameter** __reduction__ = (Parameter**)&__element__;
                    bool __reductible__ = true;
                    if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(token() == token_IDENTIFIER)
                    {
                        __reductible__ = false;
                        valueAs(__element__->m_IDENTIFIER);
                        consume();
                        while(!endOfFile())
                        {
                            beginTry();
                            phantom::lang::ast::TypeExtent* temp68=nullptr;
                            readArrayExtent(temp68);
                            if(endTry())
                            {
                                __reductible__ = false;
                                if(__element__->m_ArrayExtents == nullptr)
                                __element__->m_ArrayExtents = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>);
                                __element__->m_ArrayExtents->push_back(temp68);
                            }
                            else
                            {
                                PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp68); 
                                break;
                            }
                        }
                        if(token() == token_ASSIGN)
                        {
                            __reductible__ = false;
                            valueAs(__element__->m_ASSIGN);
                            consume();
                            if(!readAssignExpressionOrInitializerList(__element__->m_AssignExpressionOrInitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAccessSpecifier(AccessSpecifier*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(AccessSpecifier);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_PUBLIC,token_PROTECTED,token_PRIVATE}, __element__->m_PUBLIC)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readFieldOverride(FieldOverride*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(FieldOverride);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_DEFAULT, __element__->m_DEFAULT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_ASSIGN, __element__->m_ASSIGN)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readAssignExpression(__element__->m_AssignExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readSymbol(Symbol*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Symbol);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Symbol** __reduction__ = (Symbol**)&__element__;
                    bool __reductible__ = true;
                    beginTry();
                    readExplicitType(__element__->m_ExplicitType);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readPostFixExpression(__element__->m_PostFixExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readMember(Member*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Member);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Member** __reduction__ = (Member**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_BRACKET_START:
                        {
                            if(!readMemberWithMetaOrAnnotation(__element__->m_MemberWithMetaOrAnnotation)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_MemberWithMetaOrAnnotation;
                        }
                        break;
                        default:
                        if(!readBasicMember(__element__->m_BasicMember)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_BasicMember;
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readMemberWithMetaOrAnnotation(Member*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(MemberWithMetaOrAnnotation);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    __element__->m_MetaOrAnnotations = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::MetaOrAnnotation*,4ull>);
                    phantom::lang::ast::MetaOrAnnotation* temp69=nullptr;
                    if(!readMetaOrAnnotation(temp69)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_MetaOrAnnotations = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::MetaOrAnnotation*,4ull>);
                    __element__->m_MetaOrAnnotations->push_back(temp69);
                    while(!endOfFile())
                    {
                        beginTry();
                        readMetaOrAnnotation(temp69);
                        if(endTry())
                        {
                            __element__->m_MetaOrAnnotations->push_back(temp69);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp69); 
                            break;
                        }
                    }
                    if(!readMember(__element__->m_Member)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBasicMemberWithMetaOrAnnotation(Member*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BasicMemberWithMetaOrAnnotation);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    __element__->m_MetaOrAnnotations = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::MetaOrAnnotation*,4ull>);
                    phantom::lang::ast::MetaOrAnnotation* temp70=nullptr;
                    if(!readMetaOrAnnotation(temp70)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_MetaOrAnnotations = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::MetaOrAnnotation*,4ull>);
                    __element__->m_MetaOrAnnotations->push_back(temp70);
                    while(!endOfFile())
                    {
                        beginTry();
                        readMetaOrAnnotation(temp70);
                        if(endTry())
                        {
                            __element__->m_MetaOrAnnotations->push_back(temp70);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp70); 
                            break;
                        }
                    }
                    if(!readBasicMember(__element__->m_BasicMember)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readFriend(Member*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Friend);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_FRIEND)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    switch(token())
                    {
                        case token_CLASS:
                        case token_STRUCT:
                        {
                            valueAs(__element__->m_CLASS);
                            consume();
                        }
                        break;
                        default:
                        break;
                    }
                    if(!readQualifiedName(__element__->m_QualifiedName)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBasicMember(Member*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BasicMember);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Member** __reduction__ = (Member**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_PUBLIC:
                        case token_PROTECTED:
                        case token_PRIVATE:
                        {
                            if(!readAccessSpecifier(__element__->m_AccessSpecifier)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_VIRTUAL:
                        {
                            if(!readMethod(__element__->m_Method)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_STATIC:
                        case token_ALIGNAS:
                        {
                            if(!readFunctionOrVariable(__element__->m_FunctionOrVariable)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_OPERATOR:
                        {
                            if(!readConversionFunction(__element__->m_ConversionFunction)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_EXPLICIT:
                        {
                            if(lookahead(token_OPERATOR))
                            {
                                if(!readConversionFunction(__element__->m_ConversionFunction)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            }
                            else
                            {
                                if(!readConstructor(__element__->m_Constructor)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            }
                        }
                        break;
                        case token_COMPL:
                        {
                            if(!readDestructor(__element__->m_Destructor)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_FRIEND:
                        {
                            if(!readFriend(__element__->m_Friend)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_Friend;
                        }
                        break;
                        case token_BRACKET_START:
                        {
                            if(!readBasicMemberWithMetaOrAnnotation(__element__->m_BasicMemberWithMetaOrAnnotation)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_BasicMemberWithMetaOrAnnotation;
                        }
                        break;
                        default:
                        beginTry();
                        readConstructor(__element__->m_Constructor);
                        if(endTry())
                        {
                        }
                        else
                        {
                            if(!readDeclaration(__element__->m_Declaration)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readStatementOrCase(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(StatementOrCase);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Statement** __reduction__ = (Statement**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_CASE:
                        {
                            if(!readCaseStatement(__element__->m_CaseStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_CaseStatement;
                        }
                        break;
                        case token_DEFAULT:
                        {
                            if(!readDefaultStatement(__element__->m_DefaultStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_DefaultStatement;
                        }
                        break;
                        default:
                        if(!readStatement(__element__->m_Statement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_Statement;
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readIfOrWhileLocalVariableDecl(IfOrWhileLocalVariableDecl*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(IfOrWhileLocalVariableDecl);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!readTypeOrAuto(__element__->m_TypeOrAuto)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    char const* temp71=nullptr;
                    if(!read(token_ASSIGN, temp71)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readAssignExpressionOrInitializerList(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readIfOrWhileCondition(IfOrWhileCondition*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(IfOrWhileCondition);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    IfOrWhileCondition** __reduction__ = (IfOrWhileCondition**)&__element__;
                    bool __reductible__ = true;
                    beginTry();
                    readIfOrWhileLocalVariableDecl(__element__->m_IfOrWhileLocalVariableDecl);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readIfStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(IfStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_IF)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readIfOrWhileCondition(__element__->m_Condition)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readStatement(__element__->m_Then)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(token() == token_ELSE)
                    {
                        consume();
                        if(!readStatement(__element__->m_Else)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readCaseStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(CaseStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_CASE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readDefaultStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(DefaultStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    char const* temp72=nullptr;
                    if(!read(token_DEFAULT, temp72)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readLabelStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(LabelStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readSwitchStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(SwitchStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_SWITCH)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_BLOCK_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_StatementOrCases = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Statement*,4ull>);
                    phantom::lang::ast::Statement* temp73=nullptr;
                    if(!readStatementOrCase(temp73)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_StatementOrCases = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Statement*,4ull>);
                    __element__->m_StatementOrCases->push_back(temp73);
                    while(!endOfFile())
                    {
                        beginTry();
                        readStatementOrCase(temp73);
                        if(endTry())
                        {
                            __element__->m_StatementOrCases->push_back(temp73);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp73); 
                            break;
                        }
                    }
                    if(!read(token_BLOCK_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readForeachSignature(ForeachSignature*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ForeachSignature);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!readTypeOrAuto(__element__->m_TypeOrAuto)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readForInit(ForInit*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ForInit);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    ForInit** __reduction__ = (ForInit**)&__element__;
                    bool __reductible__ = true;
                    beginTry();
                    readLocalVariableDecl(__element__->m_LocalVariableDecl);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readForStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ForStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_FOR)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    readForeachSignature(__element__->m_ForeachSignature);
                    if(endTry())
                    {
                    }
                    else
                    {
                        beginTry();
                        phantom::lang::ast::ForInit* temp74=nullptr;
                        readForInit(temp74);
                        if(endTry())
                        {
                            __element__->m_Init=temp74;
                        }
                        if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        beginTry();
                        phantom::lang::ast::Expression* temp75=nullptr;
                        readExpression(temp75);
                        if(endTry())
                        {
                            __element__->m_Test=temp75;
                        }
                        if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        beginTry();
                        phantom::lang::ast::Expression* temp76=nullptr;
                        readExpression(temp76);
                        if(endTry())
                        {
                            __element__->m_Update=temp76;
                        }
                    }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readStatement(__element__->m_Statement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readWhileStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(WhileStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_WHILE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readIfOrWhileCondition(__element__->m_Condition)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readStatement(__element__->m_Statement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBlockStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BlockStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_BLOCK_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::Statement* temp77=nullptr;
                        readStatement(temp77);
                        if(endTry())
                        {
                            if(__element__->m_Statements == nullptr)
                            __element__->m_Statements = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Statement*,4ull>);
                            __element__->m_Statements->push_back(temp77);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp77); 
                            break;
                        }
                    }
                    if(!read(token_BLOCK_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readDoWhileStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(DoWhileStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_DO)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readBlockStatement(__element__->m_BlockStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_WHILE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Statement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Statement** __reduction__ = (Statement**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_IF:
                        {
                            if(!readIfStatement(__element__->m_IfStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_IfStatement;
                        }
                        break;
                        case token_FOR:
                        {
                            if(!readForStatement(__element__->m_ForStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_ForStatement;
                        }
                        break;
                        case token_SWITCH:
                        {
                            if(!readSwitchStatement(__element__->m_SwitchStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_SwitchStatement;
                        }
                        break;
                        case token_WHILE:
                        {
                            if(!readWhileStatement(__element__->m_WhileStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_WhileStatement;
                        }
                        break;
                        case token_ASSERT:
                        {
                            if(!readAssertStatement(__element__->m_AssertStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_AssertStatement;
                        }
                        break;
                        case token_BREAK:
                        {
                            if(!readBreakStatement(__element__->m_BreakStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_BreakStatement;
                        }
                        break;
                        case token_CONTINUE:
                        {
                            if(!readContinueStatement(__element__->m_ContinueStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_ContinueStatement;
                        }
                        break;
                        case token_DO:
                        {
                            if(!readDoWhileStatement(__element__->m_DoWhileStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_DoWhileStatement;
                        }
                        break;
                        case token_BLOCK_START:
                        {
                            if(!readBlockStatement(__element__->m_BlockStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_BlockStatement;
                        }
                        break;
                        case token_RETURN:
                        {
                            if(!readReturnStatement(__element__->m_ReturnStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_GOTO:
                        {
                            if(!readGotoStatement(__element__->m_GotoStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_CLASS:
                        case token_ENUM:
                        case token_STRUCT:
                        case token_USING:
                        case token_TYPEDEF:
                        {
                            if(!readBlockDeclaration(__element__->m_BlockDeclaration)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_BlockDeclaration;
                        }
                        break;
                        case token_ATAT:
                        {
                            if(!readMixinStatement(__element__->m_MixinStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_SEMI_COLON:
                        {
                            if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        default:
                        beginTry();
                        readLabelStatement(__element__->m_LabelStatement);
                        __reduction__ = &__element__->m_LabelStatement;
                        if(endTry())
                        {
                        }
                        else
                        {
                            beginTry();
                            readLocalVariableStatement(__element__->m_LocalVariableStatement);
                            __reduction__ = &__element__->m_LocalVariableStatement;
                            if(endTry())
                            {
                            }
                            else
                            {
                                if(!readExpressionStatement(__element__->m_ExpressionStatement)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                __reduction__ = &__element__->m_ExpressionStatement;
                            }
                        }
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readMixinStatement(MixinStatement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(MixinStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_ATAT, __element__->m_ATAT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    phantom::lang::ast::Expression* temp78=nullptr;
                    beginTry();
                    readExpression(temp78);
                    if(endTry())
                    {
                        __element__->m_Expressions = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Expression*,4ull>);
                        __element__->m_Expressions->push_back(temp78);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            consume();
                            phantom::lang::ast::Expression* temp79=nullptr;
                            if(!readExpression(temp79)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_Expressions->push_back(temp79);
                        }
                    }
                    else PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp78);
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readReturnStatement(ReturnStatement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ReturnStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_RETURN)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::Expression* temp80=nullptr;
                    readExpression(temp80);
                    if(endTry())
                    {
                        __element__->m_pExpression=temp80;
                    }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readGotoStatement(GotoStatement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(GotoStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_GOTO)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readDeclarator(Declarator*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Declarator);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Declarator** __reduction__ = (Declarator**)&__element__;
                    bool __reductible__ = true;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::TypeExtent* temp81=nullptr;
                        readTypeExtent(temp81);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_TypeExtents == nullptr)
                            __element__->m_TypeExtents = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>);
                            __element__->m_TypeExtents->push_back(temp81);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp81); 
                            break;
                        }
                    }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    switch(token())
                    {
                        case token_BRACKET_START:
                        {
                            __reductible__ = false;
                            __element__->m_ArrayExtents = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>);
                            phantom::lang::ast::TypeExtent* temp82=nullptr;
                            if(!readArrayExtent(temp82)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_ArrayExtents = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>);
                            __element__->m_ArrayExtents->push_back(temp82);
                            while(!endOfFile())
                            {
                                beginTry();
                                readArrayExtent(temp82);
                                if(endTry())
                                {
                                    __reductible__ = false;
                                    __element__->m_ArrayExtents->push_back(temp82);
                                }
                                else
                                {
                                    PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp82); 
                                    break;
                                }
                            }
                            char const* temp83=nullptr;
                            if(!read(token_ASSIGN, temp83)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(!readAssignExpressionOrInitializerList(__element__->m_AssignExpressionOrInitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_ASSIGN:
                        {
                            __reductible__ = false;
                            char const* temp84=nullptr;
                            if(!read(token_ASSIGN, temp84)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(!readAssignExpressionOrInitializerList(__element__->m_AssignExpressionOrInitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_PAREN_START:
                        {
                            if(!readCallList(__element__->m_CallList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_BLOCK_START:
                        {
                            if(!readInitializerList(__element__->m_InitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readExplicitLocalVariableDecl(LocalVariableDecl*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ExplicitLocalVariableDecl);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    while(!endOfFile())
                    {
                        beginTry();
                        char const* temp85=nullptr;
                        read({token_CONST,token_VOLATILE}, temp85);
                        if(endTry())
                        {
                            if(__element__->m_CONSTs == nullptr)
                            __element__->m_CONSTs = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                            __element__->m_CONSTs->push_back(temp85);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(!readBasicType(__element__->m_BasicType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_Declarators = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Declarator*,4ull>);
                    phantom::lang::ast::Declarator* temp86=nullptr;
                    if(!readDeclarator(temp86)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_Declarators->push_back(temp86);
                    while(token(token_COMMA, m_TokenPointer + 0))
                    {
                        consume();
                        if(!readDeclarator(temp86)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_Declarators->push_back(temp86);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAutoDeclarator(AutoDeclarator*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(AutoDeclarator);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::TypeExtent* temp87=nullptr;
                        readTypeExtent(temp87);
                        if(endTry())
                        {
                            if(__element__->m_TypeExtents == nullptr)
                            __element__->m_TypeExtents = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>);
                            __element__->m_TypeExtents->push_back(temp87);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp87); 
                            break;
                        }
                    }
                    if(!read(token_IDENTIFIER, __element__->m_IDENTIFIER)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    switch(token())
                    {
                        case token_PAREN_START:
                        {
                            if(!readCallList(__element__->m_CallList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_BLOCK_START:
                        {
                            if(!readInitializerList(__element__->m_InitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        default:
                        while(!endOfFile())
                        {
                            beginTry();
                            phantom::lang::ast::TypeExtent* temp88=nullptr;
                            readArrayExtent(temp88);
                            if(endTry())
                            {
                                if(__element__->m_ArrayExtents == nullptr)
                                __element__->m_ArrayExtents = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>);
                                __element__->m_ArrayExtents->push_back(temp88);
                            }
                            else
                            {
                                PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp88); 
                                break;
                            }
                        }
                        char const* temp89=nullptr;
                        if(!read(token_ASSIGN, temp89)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!readAssignExpressionOrInitializerList(__element__->m_AssignExpressionOrInitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        break;
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAutoLocalVariableDecl(LocalVariableDecl*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(AutoLocalVariableDecl);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    while(!endOfFile())
                    {
                        beginTry();
                        char const* temp90=nullptr;
                        read({token_CONST,token_VOLATILE}, temp90);
                        if(endTry())
                        {
                            if(__element__->m_CONSTs == nullptr)
                            __element__->m_CONSTs = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                            __element__->m_CONSTs->push_back(temp90);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(!readAuto(__element__->m_Auto)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_AutoDeclarators = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::AutoDeclarator*,4ull>);
                    phantom::lang::ast::AutoDeclarator* temp91=nullptr;
                    if(!readAutoDeclarator(temp91)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __element__->m_AutoDeclarators->push_back(temp91);
                    while(token(token_COMMA, m_TokenPointer + 0))
                    {
                        consume();
                        if(!readAutoDeclarator(temp91)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __element__->m_AutoDeclarators->push_back(temp91);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readLocalVariableDecl(LocalVariableDecl*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(LocalVariableDecl);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    LocalVariableDecl** __reduction__ = (LocalVariableDecl**)&__element__;
                    bool __reductible__ = true;
                    if(lookahead(token_AUTO))
                    {
                        if(!readAutoLocalVariableDecl(__element__->m_AutoLocalVariableDecl)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_AutoLocalVariableDecl;
                    }
                    else
                    {
                        if(!readExplicitLocalVariableDecl(__element__->m_ExplicitLocalVariableDecl)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_ExplicitLocalVariableDecl;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readLocalVariableStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(LocalVariableStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!readLocalVariableDecl(__element__->m_LocalVariableDecl)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readExpressionStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ExpressionStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(Expression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readAssignExpression(__element__->m_Expression_AssignExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_Expression_AssignExpression;
                    if(token() == token_COMMA)
                    {
                        __reductible__ = false;
                        consume();
                        if(!readExpression(__element__->m_Expression_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_Expression_Expression;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAssignExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(AssignExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readConditionalExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    if(lookahead(token_GT))
                    {
                        __reductible__ = false;
                        if(lookahead(token_GT))
                        {
                            __reductible__ = false;
                            if(lookahead(token_ASSIGN))
                            {
                                __reductible__ = false;
                                __element__->m_RightShiftAssign = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>)(3);
                                if(!read(token_GT, (*__element__->m_RightShiftAssign)[0])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                if(!readContiguous(token_GT, (*__element__->m_RightShiftAssign)[1])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                if(!readContiguous(token_ASSIGN, (*__element__->m_RightShiftAssign)[2])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                if(!readAssignExpression(__element__->m_RightShiftAssign_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            }
                            else
                            {
                                __reductible__ = false;
                                __element__->m_RightShift = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>)(2);
                                if(!read(token_GT, (*__element__->m_RightShift)[0])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                if(!readContiguous(token_GT, (*__element__->m_RightShift)[1])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                if(!readShiftExpression(__element__->m_RightShift_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            }
                        }
                        else
                        {
                            __reductible__ = false;
                            if(!read(token_GT, __element__->m_GT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(!readAddExpression(__element__->m_Greater_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                    }
                    else
                    {
                        switch(token())
                        {
                            case token_ASSIGN:
                            case token_ASS_ADD:
                            case token_ASS_AND:
                            case token_ASS_DIV:
                            case token_ASS_MOD:
                            case token_ASS_MUL:
                            case token_ASS_OR:
                            case token_ASS_SHL:
                            case token_ASS_SUB:
                            case token_ASS_XOR:
                            {
                                valueAs(__element__->m_Op);
                                __reductible__ = false;
                                consume();
                                __reductible__ = false;
                                if(!readAssignExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                __reduction__ = &__element__->m_RightExpression;
                            }
                            break;
                            default:
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readConditionalExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ConditionalExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readLogicalOrExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    if(token() == token_QUESTION)
                    {
                        __reductible__ = false;
                        consume();
                        if(!readExpression(__element__->m_Then)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!read(token_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!readExpression(__element__->m_Else)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readLogicalOrExpressionRight(LogicalOrExpressionRight*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(LogicalOrExpressionRight);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_LOG_OR, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readLogicalAndExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readLogicalOrExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(LogicalOrExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readLogicalAndExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::LogicalOrExpressionRight* temp92=nullptr;
                        readLogicalOrExpressionRight(temp92);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_LogicalOrExpressionRights == nullptr)
                            __element__->m_LogicalOrExpressionRights = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::LogicalOrExpressionRight*,4ull>);
                            __element__->m_LogicalOrExpressionRights->push_back(temp92);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp92); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readLogicalAndExpressionRight(LogicalAndExpressionRight*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(LogicalAndExpressionRight);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    LogicalAndExpressionRight** __reduction__ = (LogicalAndExpressionRight**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_LOG_AND, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::Expression* temp93=nullptr;
                    readBinOrExpression(temp93);
                    if(endTry())
                    {
                        __element__->m_RightExpression=temp93;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readLogicalAndExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(LogicalAndExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readBinOrExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::LogicalAndExpressionRight* temp94=nullptr;
                        readLogicalAndExpressionRight(temp94);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_LogicalAndExpressionRights == nullptr)
                            __element__->m_LogicalAndExpressionRights = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::LogicalAndExpressionRight*,4ull>);
                            __element__->m_LogicalAndExpressionRights->push_back(temp94);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp94); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBinOrExpressionRight(BinOrExpressionRight*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BinOrExpressionRight);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_BIN_OR, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readXOrExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBinOrExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BinOrExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readXOrExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::BinOrExpressionRight* temp95=nullptr;
                        readBinOrExpressionRight(temp95);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_BinOrExpressionRights == nullptr)
                            __element__->m_BinOrExpressionRights = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::BinOrExpressionRight*,4ull>);
                            __element__->m_BinOrExpressionRights->push_back(temp95);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp95); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readXOrExpressionRight(XOrExpressionRight*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(XOrExpressionRight);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_XOR, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readBinAndExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readXOrExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(XOrExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readBinAndExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::XOrExpressionRight* temp96=nullptr;
                        readXOrExpressionRight(temp96);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_XOrExpressionRights == nullptr)
                            __element__->m_XOrExpressionRights = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::XOrExpressionRight*,4ull>);
                            __element__->m_XOrExpressionRights->push_back(temp96);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp96); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBinAndExpressionRight(BinAndExpressionRight*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BinAndExpressionRight);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    BinAndExpressionRight** __reduction__ = (BinAndExpressionRight**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_BIN_AND, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::Expression* temp97=nullptr;
                    readEqualityExpression(temp97);
                    if(endTry())
                    {
                        __element__->m_RightExpression=temp97;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBinAndExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BinAndExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readEqualityExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::BinAndExpressionRight* temp98=nullptr;
                        readBinAndExpressionRight(temp98);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_BinAndExpressionRights == nullptr)
                            __element__->m_BinAndExpressionRights = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::BinAndExpressionRight*,4ull>);
                            __element__->m_BinAndExpressionRights->push_back(temp98);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp98); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readEqualityExpressionRight(EqualityExpressionRight*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(EqualityExpressionRight);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_EQ,token_NE}, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readRelationalExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readEqualityExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(EqualityExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readRelationalExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::EqualityExpressionRight* temp99=nullptr;
                        readEqualityExpressionRight(temp99);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_EqualityExpressionRights == nullptr)
                            __element__->m_EqualityExpressionRights = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::EqualityExpressionRight*,4ull>);
                            __element__->m_EqualityExpressionRights->push_back(temp99);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp99); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readRelationalExpressionRight(RelationalExpressionRight*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(RelationalExpressionRight);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(lookahead(token_GT) && lookahead(token_GT))
                    {
                        __element__->m_OpMul = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>)(2);
                        if(!read(token_GT, (*__element__->m_OpMul)[0])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!readContiguous(token_GT, (*__element__->m_OpMul)[1])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!readShiftExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    else
                    {
                        if(!read({token_GT,token_GE,token_LE,token_LT,token_EQ,token_NE}, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!readShiftExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readRelationalExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(RelationalExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readShiftExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::RelationalExpressionRight* temp100=nullptr;
                        readRelationalExpressionRight(temp100);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_RelationalExpressionRights == nullptr)
                            __element__->m_RelationalExpressionRights = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::RelationalExpressionRight*,4ull>);
                            __element__->m_RelationalExpressionRights->push_back(temp100);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp100); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readShiftExpressionRight(ShiftExpressionRight*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ShiftExpressionRight);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(lookahead(token_GT))
                    {
                        if(lookahead(token_GE))
                        {
                            __element__->m_OpMul = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>)(2);
                            if(!read(token_GT, (*__element__->m_OpMul)[0])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(!readContiguous(token_GE, (*__element__->m_OpMul)[1])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(!readAddExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        else
                        {
                            if(lookahead(token_GT))
                            {
                                __element__->m_OpMul = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>)(2);
                                if(!read(token_GT, (*__element__->m_OpMul)[0])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                if(!readContiguous(token_GT, (*__element__->m_OpMul)[1])) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                if(!readAddExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            }
                            else
                            {
                                if(!read(token_GT, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                if(!readRelationalExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            }
                        }
                    }
                    else
                    {
                        if(!read(token_SHL, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        if(!readAddExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readShiftExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ShiftExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readAddExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::ShiftExpressionRight* temp101=nullptr;
                        readShiftExpressionRight(temp101);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_ShiftExpressionRights == nullptr)
                            __element__->m_ShiftExpressionRights = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::ShiftExpressionRight*,4ull>);
                            __element__->m_ShiftExpressionRights->push_back(temp101);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp101); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAddExpressionRight(AddExpressionRight*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(AddExpressionRight);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_PLUS,token_MINUS}, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readMulExpression(__element__->m_RightExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAddExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(AddExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readMulExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::AddExpressionRight* temp102=nullptr;
                        readAddExpressionRight(temp102);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_AddExpressionRights == nullptr)
                            __element__->m_AddExpressionRights = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::AddExpressionRight*,4ull>);
                            __element__->m_AddExpressionRights->push_back(temp102);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp102); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readMulExpressionRight(MulExpressionRight*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(MulExpressionRight);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_MUL,token_DIV,token_MOD}, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::Expression* temp103=nullptr;
                    readUnaryExpression(temp103);
                    if(endTry())
                    {
                        __element__->m_RightExpression=temp103;
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readMulExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(MulExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readUnaryExpression(__element__->m_LeftExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_LeftExpression;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::MulExpressionRight* temp104=nullptr;
                        readMulExpressionRight(temp104);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_MulExpressionRights == nullptr)
                            __element__->m_MulExpressionRights = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::MulExpressionRight*,4ull>);
                            __element__->m_MulExpressionRights->push_back(temp104);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp104); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readSpecialCastExpression(SpecialCastExpression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(SpecialCastExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_REINTERPRET_CAST,token_STATIC_CAST,token_CONST_CAST}, __element__->m_CastKind)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    char const* temp105=nullptr;
                    if(!read(token_LT, temp105)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    char const* temp106=nullptr;
                    if(!read(token_GT, temp106)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readFunctionPtrExpression(FunctionPtrExpression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(FunctionPtrExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_FUNCTION_PTR)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readName(__element__->m_Name)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readUnaryExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(UnaryExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_BIN_AND:
                        case token_INC:
                        case token_DEC:
                        case token_MUL:
                        case token_PLUS:
                        case token_MINUS:
                        case token_NOT:
                        case token_COMPL:
                        {
                            __reductible__ = false;
                            if(!read({token_BIN_AND,token_INC,token_DEC,token_MUL,token_PLUS,token_MINUS,token_NOT,token_COMPL}, __element__->m_Op)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(!readUnaryExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        default:
                        if(!readPostFixExpression(__element__->m_PostFixExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_PostFixExpression;
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAssignExpressionOrInitializerList(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(AssignExpressionOrInitializerList);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(lookahead(token_BLOCK_START))
                    {
                        if(!readInitializerList(__element__->m_InitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_InitializerList;
                    }
                    else
                    {
                        if(!readAssignExpression(__element__->m_AssignExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_AssignExpression;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readExpressionOrInitializerList(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ExpressionOrInitializerList);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(lookahead(token_BLOCK_START))
                    {
                        if(!readInitializerList(__element__->m_InitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_InitializerList;
                    }
                    else
                    {
                        if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        __reduction__ = &__element__->m_Expression;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readInitializerList(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(InitializerList);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_BLOCK_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    phantom::lang::ast::Expression* temp107=nullptr;
                    beginTry();
                    readAssignExpressionOrInitializerList(temp107);
                    if(endTry())
                    {
                        __element__->m_AssignExpressionOrInitializerLists = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Expression*,4ull>);
                        __element__->m_AssignExpressionOrInitializerLists->push_back(temp107);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            consume();
                            phantom::lang::ast::Expression* temp108=nullptr;
                            if(!readAssignExpressionOrInitializerList(temp108)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_AssignExpressionOrInitializerLists->push_back(temp108);
                        }
                    }
                    else PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp107);
                    if(!read(token_BLOCK_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readCallList(CallList*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(CallList);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    phantom::lang::ast::Expression* temp109=nullptr;
                    beginTry();
                    readAssignExpressionOrInitializerList(temp109);
                    if(endTry())
                    {
                        __element__->m_AssignExpressionOrInitializerLists = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Expression*,4ull>);
                        __element__->m_AssignExpressionOrInitializerLists->push_back(temp109);
                        while(token(token_COMMA, m_TokenPointer + 0))
                        {
                            consume();
                            phantom::lang::ast::Expression* temp110=nullptr;
                            if(!readAssignExpressionOrInitializerList(temp110)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __element__->m_AssignExpressionOrInitializerLists->push_back(temp110);
                        }
                    }
                    else PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp109);
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readArrayAccess(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ArrayAccess);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_BRACKET_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_BRACKET_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readKeywordExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(KeywordExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_THIS, __element__->m_Keyword)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readNullptrExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(NullptrExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_NULLPTR)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readPostFixExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(PostFixExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    if(!readPrimaryExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    __reduction__ = &__element__->m_Expression;
                    while(!endOfFile())
                    {
                        beginTry();
                        phantom::lang::ast::PostFixEnd* temp111=nullptr;
                        readPostFixEnd(temp111);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_PostFixEnds == nullptr)
                            __element__->m_PostFixEnds = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::PostFixEnd*,4ull>);
                            __element__->m_PostFixEnds->push_back(temp111);
                        }
                        else
                        {
                            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp111); 
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readPostFixEnd(PostFixEnd*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(PostFixEnd);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    PostFixEnd** __reduction__ = (PostFixEnd**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_PAREN_START:
                        {
                            if(!readCallList(__element__->m_CallList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_BRACKET_START:
                        {
                            if(!readArrayAccess(__element__->m_ArrayAccess)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_BLOCK_START:
                        {
                            if(!readInitializerList(__element__->m_InitializerList)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_DOT:
                        case token_ARROW:
                        case token_SCOPE:
                        {
                            __reductible__ = false;
                            if(!read({token_DOT,token_ARROW,token_SCOPE}, __element__->m_DOT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            if(!readName(__element__->m_Name)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        default:
                        if(!read({token_INC,token_DEC}, __element__->m_INC)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readCStyleCastExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(CStyleCastExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    readExpression(__element__->m_Expression);
                    if(endTry())
                    {
                    }
                    else
                    {
                        if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    beginTry();
                    phantom::lang::ast::Expression* temp112=nullptr;
                    readUnaryExpression(temp112);
                    if(endTry())
                    {
                        __element__->m_Casted=temp112;
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTypeTraitBinary(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TypeTraitBinary);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_IS_SAME, __element__->m_IS_SAME)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_COMMA)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readType(__element__->m_Type0)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readStaticAssert(StaticAssert*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(StaticAssert);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_STATIC_ASSERT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readAssignExpression(__element__->m_AssignExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(token() == token_COMMA)
                    {
                        consume();
                        if(!read(token_STRING_LIT, __element__->m_STRING_LIT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readAssertStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(AssertStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_ASSERT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readAssignExpression(__element__->m_AssignExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(token() == token_COMMA)
                    {
                        consume();
                        if(!read(token_STRING_LIT, __element__->m_STRING_LIT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        phantom::lang::ast::Expression* temp113=nullptr;
                        beginTry();
                        readAssignExpression(temp113);
                        if(endTry())
                        {
                            __element__->m_AssignExpressions = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<phantom::lang::ast::Expression*,4ull>);
                            __element__->m_AssignExpressions->push_back(temp113);
                            while(token(token_COMMA, m_TokenPointer + 0))
                            {
                                consume();
                                phantom::lang::ast::Expression* temp114=nullptr;
                                if(!readAssignExpression(temp114)){ PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                                __element__->m_AssignExpressions->push_back(temp114);
                            }
                        }
                        else PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(temp113);
                    }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readBreakStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(BreakStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_BREAK, __element__->m_BREAK)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readContinueStatement(Statement*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ContinueStatement);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read(token_CONTINUE, __element__->m_CONTINUE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_SEMI_COLON)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readSizeofExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(SizeofExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_SIZEOF,token_ALIGNOF}, __element__->m_SIZEOF)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readTypeofExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(TypeofExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!read({token_TYPEOF,token_CLASSOF,token_ENUMOF,token_UNIONOF}, __element__->m_TYPEOF)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readType(__element__->m_Type)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readStringLiteralChain(StringLiteralChain*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(StringLiteralChain);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    StringLiteralChain** __reduction__ = (StringLiteralChain**)&__element__;
                    bool __reductible__ = true;
                    if(!read(token_STRING_LIT, __element__->m_STRING_LIT)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    while(!endOfFile())
                    {
                        beginTry();
                        char const* temp115=nullptr;
                        read(token_STRING_LIT, temp115);
                        if(endTry())
                        {
                            __reductible__ = false;
                            if(__element__->m_STRING_LITs == nullptr)
                            __element__->m_STRING_LITs = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(phantom::SmallVector<spell::any,4ull>);
                            __element__->m_STRING_LITs->push_back(temp115);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readFundamentalTypeFunctionCast(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(FundamentalTypeFunctionCast);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    if(!readFundamentalType(__element__->m_FundamentalType)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_START, __element__->m_PAREN_START)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!readExpression(__element__->m_Expression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    if(!read(token_PAREN_END)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
                bool readPrimaryExpression(Expression*& output)
                {
                    m_LookaheadCounter = 0;
                    auto __element__ = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(PrimaryExpression);
                    auto pos_begin = m_Tokens.back().location.begin;
                    output = __element__;
                    Expression** __reduction__ = (Expression**)&__element__;
                    bool __reductible__ = true;
                    switch(token())
                    {
                        case token_THIS:
                        {
                            if(!readKeywordExpression(__element__->m_KeywordExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_KeywordExpression;
                        }
                        break;
                        case token_SIZEOF:
                        case token_ALIGNOF:
                        {
                            if(!readSizeofExpression(__element__->m_SizeofExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_SizeofExpression;
                        }
                        break;
                        case token_TYPEOF:
                        case token_CLASSOF:
                        case token_ENUMOF:
                        case token_UNIONOF:
                        {
                            if(!readTypeofExpression(__element__->m_TypeofExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_TypeofExpression;
                        }
                        break;
                        case token_NULLPTR:
                        {
                            if(!readNullptrExpression(__element__->m_NullptrExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_NullptrExpression;
                        }
                        break;
                        case token_DOT:
                        case token_SCOPE:
                        case token_IDENTIFIER:
                        case token_OPERATOR:
                        {
                            switch(token())
                            {
                                case token_DOT:
                                case token_SCOPE:
                                {
                                    valueAs(__element__->m_DOT);
                                    __reductible__ = false;
                                    consume();
                                }
                                break;
                                default:
                                break;
                            }
                            if(!readName(__element__->m_Name)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_PAREN_START:
                        {
                            if(!readCStyleCastExpression(__element__->m_CStyleCastExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_CStyleCastExpression;
                        }
                        break;
                        case token_IS_SAME:
                        {
                            if(!readTypeTraitBinary(__element__->m_TypeTraitBinary)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_TypeTraitBinary;
                        }
                        break;
                        case token_STRING_LIT:
                        {
                            if(!readStringLiteralChain(__element__->m_StringLiteralChain)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_REINTERPRET_CAST:
                        case token_STATIC_CAST:
                        case token_CONST_CAST:
                        {
                            if(!readSpecialCastExpression(__element__->m_SpecialCastExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_FUNCTION_PTR:
                        {
                            if(!readFunctionPtrExpression(__element__->m_FunctionPtrExpression)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        }
                        break;
                        case token_UNSIGNED:
                        case token_SIGNED:
                        case token_BOOL:
                        case token_SHORT:
                        case token_INT:
                        case token_LONG:
                        case token_CHAR:
                        case token_FLOATING_POINT_TYPE:
                        case token_VOID:
                        {
                            if(!readFundamentalTypeFunctionCast(__element__->m_FundamentalTypeFunctionCast)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                            __reduction__ = &__element__->m_FundamentalTypeFunctionCast;
                        }
                        break;
                        default:
                        if(!read({token_INT_VALUE,token_UINT_VALUE,token_LONG_VALUE,token_ULONG_VALUE,token_LONGLONG_VALUE,token_ULONGLONG_VALUE,token_BOOL_VALUE,token_CHAR_LIT,token_FLOAT_VALUE,token_DOUBLE_VALUE,token_HEX_VALUE}, __element__->m_INT_VALUE)) { PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__); output = nullptr; return false; }
                        break;
                    }
                    if(__reductible__ && *__reduction__ != __element__)
                    {
                        output = *__reduction__;
                        *__reduction__ = nullptr;
                        PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(__element__);
                    }
                    output->__location.begin = pos_begin;
                    output->__location.end = (m_Tokens.size() > 1) ? m_Tokens[m_Tokens.size()-2].location.end : m_Tokens.back().location.end;
                    return true;
                }
            };
        }
        CppLiteGrammar::CppLiteGrammar(Allocator a_Allocator)
        : m_Allocator(a_Allocator)
        {
            _private = PHANTOM_LANG_CPPLITEGRAMMAR_NEW(ast::CppLiteGrammarPrivate)(a_Allocator);
        }
        CppLiteGrammar::~CppLiteGrammar()
        {
            PHANTOM_LANG_CPPLITEGRAMMAR_DELETE(_private);
        }
        const char* CppLiteGrammar::getTokenPrettyName(ast::EToken tok) const
        { return _private->tokenName(tok); }
        ast::Source* CppLiteGrammar::parseSource(char const* in, size_t size, ErrorDelegate e)
        {
            ast::Source* tmp = nullptr;
            if(e) _private->m_ErrorDelegate = e;
            else _private->m_ErrorDelegate = &ast::CppLiteGrammarPrivate::defaultErrorDelegate;
            if(_private->m_Lexer.begin(in, size) != spell::Lexer::e_Status_Ready) return nullptr;
            _private->consume(); // consumer first token
            if(_private->m_Lexer.getStatus() == spell::Lexer::e_Status_Error) return nullptr;
            _private->readSource(tmp);
            _private->eof();
            return tmp;
        }
        ast::Symbol* CppLiteGrammar::parseSymbol(char const* in, size_t size, ErrorDelegate e)
        {
            ast::Symbol* tmp = nullptr;
            if(e) _private->m_ErrorDelegate = e;
            else _private->m_ErrorDelegate = &ast::CppLiteGrammarPrivate::defaultErrorDelegate;
            if(_private->m_Lexer.begin(in, size) != spell::Lexer::e_Status_Ready) return nullptr;
            _private->consume(); // consumer first token
            if(_private->m_Lexer.getStatus() == spell::Lexer::e_Status_Error) return nullptr;
            _private->readSymbol(tmp);
            _private->eof();
            return tmp;
        }
        ast::Expression* CppLiteGrammar::parseExpression(char const* in, size_t size, ErrorDelegate e)
        {
            ast::Expression* tmp = nullptr;
            if(e) _private->m_ErrorDelegate = e;
            else _private->m_ErrorDelegate = &ast::CppLiteGrammarPrivate::defaultErrorDelegate;
            if(_private->m_Lexer.begin(in, size) != spell::Lexer::e_Status_Ready) return nullptr;
            _private->consume(); // consumer first token
            if(_private->m_Lexer.getStatus() == spell::Lexer::e_Status_Error) return nullptr;
            _private->readExpression(tmp);
            _private->eof();
            return tmp;
        }
    } //phantom
} //lang
