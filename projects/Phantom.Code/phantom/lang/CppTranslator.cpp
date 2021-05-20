// license [
// This file is part of the Phantom project. Copyright 2015-2016 Vivien Millet.
// Distributed under the Lesser GNU Lesser General Public License, Version 3.0. (See
// the LICENSE file which comes along with this source code or a copy at
// http://www.wiwila.com/tools/phantom/license/)
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"

#include "CppTranslator.h"

#include "BaseConstructorCallStatement.h"
#include "ClassListInitializationExpression.h"
#include "ClassTypeListInitializationExpression.h"
#include "ConditionalExpression.h"
#include "CppLite.h"
#include "Expression.h"
#include "FieldInitializationStatement.h"
#include "StaticAssert.h"
#include "TemplateDependantDeclType.h"
#include "TemplateDependantTypeOfExpression.h"
#include "phantom/lang/Module.h"
#include "phantom/lang/TemplateDependantTemplateInstance.h"
#include "phantom/lang/TemplateDependantType.h"
#include "phantom/lang/reflection.h"
#include "phantom/utils/Path.h"

#include <map>
#include <ostream>
#include <phantom/lang/Alias.h>
#include <phantom/lang/AllocateExpression.h>
#include <phantom/lang/AnonymousStruct.h>
#include <phantom/lang/AnonymousUnion.h>
#include <phantom/lang/Application.h>
#include <phantom/lang/ArrayExpression.h>
#include <phantom/lang/BinaryOperationExpression.h>
#include <phantom/lang/Block.h>
#include <phantom/lang/BranchIfNotStatement.h>
#include <phantom/lang/BranchIfStatement.h>
#include <phantom/lang/BuiltInConversionExpression.h>
#include <phantom/lang/BuiltInOperatorExpression.h>
#include <phantom/lang/CallExpression.h>
#include <phantom/lang/ConstVolatileType.h>
#include <phantom/lang/ConstantExpression.h>
#include <phantom/lang/Constructor.h>
#include <phantom/lang/ConstructorCallExpression.h>
#include <phantom/lang/DeallocateExpression.h>
#include <phantom/lang/DeleteExpression.h>
#include <phantom/lang/ExpressionStatement.h>
#include <phantom/lang/Field.h>
#include <phantom/lang/FieldExpression.h>
#include <phantom/lang/FieldPointer.h>
#include <phantom/lang/FieldPointerAdjustmentExpression.h>
#include <phantom/lang/FieldPointerExpression.h>
#include <phantom/lang/Function.h>
#include <phantom/lang/FunctionPointer.h>
#include <phantom/lang/FunctionType.h>
#include <phantom/lang/FundamentalsT.h>
#include <phantom/lang/IdentityExpression.h>
#include <phantom/lang/Import.h>
#include <phantom/lang/InitializerListExpression.h>
#include <phantom/lang/LValueInitializationStatement.h>
#include <phantom/lang/LValueReference.h>
#include <phantom/lang/Label.h>
#include <phantom/lang/LoadExpression.h>
#include <phantom/lang/LocalVariableExpression.h>
#include <phantom/lang/LocalVariableInitializationStatement.h>
#include <phantom/lang/MapClass.h>
#include <phantom/lang/MemberAnonymousStruct.h>
#include <phantom/lang/MemberAnonymousUnion.h>
#include <phantom/lang/MethodPointer.h>
#include <phantom/lang/MethodPointerCallExpression.h>
#include <phantom/lang/Namespace.h>
#include <phantom/lang/NewExpression.h>
#include <phantom/lang/Package.h>
#include <phantom/lang/Parameter.h>
#include <phantom/lang/PlaceholderClass.h>
#include <phantom/lang/PlaceholderClassType.h>
#include <phantom/lang/PlaceholderConstant.h>
#include <phantom/lang/PlaceholderTemplate.h>
#include <phantom/lang/PlaceholderType.h>
#include <phantom/lang/PlacementNewExpression.h>
#include <phantom/lang/Pointer.h>
#include <phantom/lang/PointerAdjustmentExpression.h>
#include <phantom/lang/Property.h>
#include <phantom/lang/PropertyExpression.h>
#include <phantom/lang/RValueReference.h>
#include <phantom/lang/RValueReferenceExpression.h>
#include <phantom/lang/RValueToConstLValueExpression.h>
#include <phantom/lang/ReturnStatement.h>
#include <phantom/lang/SequentialContainerClass.h>
#include <phantom/lang/SetClass.h>
#include <phantom/lang/Signature.h>
#include <phantom/lang/Source.h>
#include <phantom/lang/SourceStream.h>
#include <phantom/lang/StringLiteralExpression.h>
#include <phantom/lang/Structure.h>
#include <phantom/lang/StructureExpression.h>
#include <phantom/lang/TemplateDependantArray.h>
#include <phantom/lang/TemplateDependantElement.h>
#include <phantom/lang/TemplateDependantExpression.h>
#include <phantom/lang/TemplateParameter.h>
#include <phantom/lang/TemplateSignature.h>
#include <phantom/lang/TemplateSpecialization.h>
#include <phantom/lang/UnaryPostOperationExpression.h>
#include <phantom/lang/UnaryPreOperationExpression.h>
#include <phantom/lang/Union.h>
#include <phantom/lang/Variable.h>
#include <phantom/lang/VariableExpression.h>
#include <phantom/lang/VirtualMethodTable.h>
/* *********************************************** */

#pragma warning(disable : 4800)
#pragma warning(disable : 4244)

namespace phantom
{
namespace lang
{
namespace
{
struct OperatorInfo
{
    Operator op;
    int      precedence = 0;
    bool     leftToRight = false;
};
SmallVector<OperatorInfo> precedence = {
{Operator::Plus, 0, false},
{Operator::Minus, 3, false},
{Operator::Add, 6, true},
{Operator::Subtract, 6, true},
{Operator::Multiply, 5, true},
{Operator::Divide, 5, true},
{Operator::Address, 3, false},
{Operator::Dereference, 3, false},
{Operator::Arrow, 2, true},
{Operator::ArrowStar, 4, true},
{Operator::PreDecrement, 3, false},
{Operator::PreIncrement, 3, false},
{Operator::PostDecrement, 2, true},
{Operator::PostIncrement, 2, true},
{Operator::Equal, 10, true},
{Operator::NotEqual, 10, true},
{Operator::Greater, 9, true},
{Operator::Less, 9, true},
{Operator::GreaterEqual, 9, true},
{Operator::LessEqual, 9, true},
{Operator::LogicalAnd, 14, true},
{Operator::LogicalOr, 15, true},
{Operator::XOr, 12, true},
{Operator::Not, 3, false},
{Operator::BitAnd, 11, true},
{Operator::BitOr, 13, true},
{Operator::Complement, 3, false},
{Operator::Modulo, 5, true},
{Operator::Comma, 17, true},
{Operator::ShiftLeft, 7, true},
{Operator::ShiftRight, 7, true},
{Operator::Assignment, 16, false},
{Operator::AssignmentAdd, 16, false},
{Operator::AssignmentSubtract, 16, false},
{Operator::AssignmentMultiply, 16, false},
{Operator::AssignmentDivide, 16, false},
{Operator::AssignmentBitAnd, 16, false},
{Operator::AssignmentBitOr, 16, false},
{Operator::AssignmentModulo, 16, false},
{Operator::AssignmentShiftLeft, 16, false},
{Operator::AssignmentShiftRight, 16, false},
{Operator::AssignmentXOr, 16, false},
{Operator::Bracket, 2, true},
{Operator::Parenthesis, 2, true},
{Operator::Conversion, 2},
{Operator::COUNT, 1, true},
};

int priorityComputer(LanguageElement* a_pScope, ClassType* _class, std::map<LanguageElement*, int>& classPrios)
{
    int& prio = classPrios[_class];
    if (prio == 0)
    {
        prio = 1;
        if (auto spec = _class->getTemplateSpecialization())
        {
            for (auto arg : spec->getArguments())
            {
                if (auto argCT = arg->asClassType())
                {
                    if (argCT->hasOwnerCascade(a_pScope))
                    {
                        prio = std::max(prio, priorityComputer(a_pScope, argCT, classPrios) + 1);
                    }
                }
            }
        }
        if (auto class_ = _class->asClass())
        {
            for (auto& bc : class_->getBaseClasses())
            {
                if (bc.baseClass->hasOwnerCascade(a_pScope))
                {
                    prio = std::max(prio, priorityComputer(a_pScope, bc.baseClass, classPrios) + 1);
                }
            }
        }
        for (auto field : _class->getFields())
        {
            if (auto fCT = field->getValueType()->removeQualifiers()->asClassType())
            {
                if (fCT->hasOwnerCascade(a_pScope))
                {
                    prio = std::max(prio, priorityComputer(a_pScope, fCT, classPrios) + 1);
                }
                else if (auto spec = fCT->getTemplateSpecialization())
                {
                    for (auto arg : spec->getArguments())
                    {
                        if (auto argCT = arg->asClassType())
                        {
                            if (argCT->hasOwnerCascade(a_pScope))
                            {
                                prio = std::max(prio, priorityComputer(a_pScope, argCT, classPrios) + 1);
                            }
                        }
                    }
                }
            }
        }
        for (auto type : _class->getTypes())
        {
            if (auto subclass = type->asClassType())
            {
                prio = std::max(prio, priorityComputer(a_pScope, subclass, classPrios) + 1);
            }
        }
        if (auto upClass = _class->getOwner()->asClassType())
        {
            if (upClass != a_pScope)
                prio = std::max(prio, priorityComputer(a_pScope, upClass, classPrios));
        }
    }
    return prio;
}

SmallSet<StringView> keywordSet{"class",
                                "union",
                                "struct",
                                "enum",
                                "template",
                                "typename",
                                "this",
                                "nullptr",
                                "operator",
                                "override",
                                "break",
                                "continue",
                                ";",
                                ":",
                                ",",
                                "+=",
                                "&=",
                                "/=",
                                "%=",
                                "*=",
                                "|=",
                                "<<=",
                                "-=",
                                "^=",
                                "<=",
                                ">=",
                                "->",
                                "<",
                                ">",
                                "==",
                                "!=",
                                "!",
                                "?",
                                "*",
                                "/",
                                "++",
                                "--",
                                "+",
                                "-",
                                "~",
                                ".",
                                "@@",
                                "&&",
                                "&",
                                "||",
                                "|",
                                "^",
                                "%",
                                "<<",
                                "public",
                                "protected",
                                "private",
                                "=",
                                "...",
                                "auto",
                                "unsigned",
                                "signed",
                                "long",
                                "bool",
                                "char",
                                "short",
                                "int",
                                "float|double",
                                "void",
                                "case",
                                "for",
                                "while",
                                "switch",
                                "default",
                                "do",
                                "if",
                                "else",
                                "return",
                                "goto",
                                "new",
                                "delete",
                                "explicit",
                                "typedef",
                                "using",
                                "reinterpret_cast",
                                "static_cast",
                                "const_cast",
                                "sizeof",
                                "alignof",
                                "alignas",
                                "friend",
                                "throw",
                                "try",
                                "catch",
                                "classof",
                                "enumof",
                                "unionof",
                                "import",
                                "#include",
                                "@module",
                                "@function",
                                "@function_ptr",
                                "@delegate",
                                "assert",
                                "static_assert",
                                "@is_same",
                                "const",
                                "volatile",
                                "decltype",
                                "final",
                                "virtual",
                                "static"};
bool                 IsKeyword(StringView _id)
{
    return (keywordSet.find(_id) != keywordSet.end());
}
} // namespace

CppTranslator::CppTranslator() : m_cIndentChar(' '), m_iIndentCharCount(4) {}

CppTranslator::~CppTranslator(void) {}

void CppTranslator::translate(Source* a_pSource, Output& a_Out)
{
    m_pSource = a_pSource;
    translate(0, a_Out.h);
    translate(1, a_Out.cpp);

    String fwd;
    _formatForwards(m_ForwardsH, fwd);
    a_Out.h = fwd + a_Out.h;
    fwd.clear();
    _formatForwards(m_ForwardsCPP, fwd);
    a_Out.cpp = fwd + a_Out.cpp;

    StringUtil::ReplaceAll(a_Out.h, "\n", "\r\n");
    StringUtil::ReplaceAll(a_Out.cpp, "\n", "\r\n");

    for (auto s : m_IncludesH)
    {
        a_Out.hIncludes.push_back(s.second);
    }
    for (auto s : m_IncludesCPP)
    {
        a_Out.cppIncludes.push_back(s.second);
    }
    m_IncludesH.clear();
    m_IncludesCPP.clear();
    m_pSource = nullptr;
}

int CppTranslator::translate(uint a_uiPass, String& a_Out)
{
    m_uiFile = a_uiPass;
    m_bEndif = false;
    m_IndentationLevel = 0;
    m_pTranslation = &a_Out;
    if (a_uiPass == 0)
        translate(m_pSource, TranslationType::Declaration);
    else
        translate(m_pSource, TranslationType::Definition);
    PHANTOM_ASSERT(m_IndentationLevel == 0);
    m_IndentationLevel = 0;
    return 0;
}

StringView CppTranslator::getOption(StringView a_Key) const
{
    auto found = m_Options.find(a_Key);
    return found == m_Options.end() ? StringView() : found->second;
}

bool CppTranslator::addAlias(StringView a_Original, StringView a_Alias)
{
    if (auto o = Application::Get()->findCppSymbol(a_Original))
        if (auto a = Application::Get()->findCppSymbol(a_Alias))
            return m_Aliases[o] = a;
    return false;
}

void CppTranslator::addAlias(Symbol* a_pOriginal, Symbol* a_pAlias)
{
    m_Aliases[a_pOriginal] = a_pAlias;
}

Symbol* CppTranslator::getAlias(Symbol* a_pOriginal) const
{
    auto found = m_Aliases.find(a_pOriginal);
    if (found != m_Aliases.end())
        return found->second;
    return nullptr;
}

void CppTranslator::translateLiteral(const String& a_Value, String& translation, int options)
{
    translation += "\"";
    for (auto it = a_Value.begin(); it != a_Value.end(); ++it)
    {
        switch (*it)
        {
        case '"':
            translation += "\\\"";
            break;
        case '\\':
            translation += "\\\\";
            break;
        case '\n':
            translation += "\\n";
            break;
        case '\r':
            translation += "\\r";
            break;
        case '\t':
            translation += "\\t";
            break;
        case '\0':
            translation += "\\0";
            break;
        default:
            translation += *it;
        }
    }
    translation += "\"";
}

#if o_HAS_BUILT_IN_WCHAR_T
#    define case_e_wchar_t case e_wchar_t:
#else
#    define case_e_wchar_t
#endif

#if o_HAS_BUILT_IN_CHAR16_T
#    define case_e_char16_t case e_char16_t:
#else
#    define case_e_char16_t
#endif

#if o_HAS_BUILT_IN_CHAR32_T
#    define case_e_char32_t case e_char32_t:
#else
#    define case_e_char32_t
#endif

void CppTranslator::addForceIncludeTemplateArgument(StringView _templateQName, size_t _argIdx)
{
    m_ForceIncludeTemplateArguments[_argIdx].insert(_templateQName);
}

void CppTranslator::visit(Alias* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration)
    {
        if (a_pInput->getAliasedSymbol()->asType() && a_pInput->getName().size())
        {
            append("typedef ");
            if (a_pInput->getAliasedSymbol()->getName() == a_pInput->getName())
            {
                append(a_pInput->getAliasedSymbol()->getQualifiedDecoratedName());
                append(' ');
                append(a_pInput->getName());
            }
            else
            {
                m_noAlias++;
                translateTyped(static_cast<Type*>(a_pInput->getAliasedSymbol()), a_pInput->getName());
                m_noAlias--;
            }
            append(';');
        }
        else if (a_pInput->getAliasedSymbol()->asMethod())
        {
            append("using ");
            translate(static_cast<Symbol*>(a_pInput->getAliasedSymbol()->asMethod()->getOwner()));
            append("::");
            append(a_pInput->getAliasedSymbol()->getName());
            append(';');
        }
        else if (a_pInput->getAliasedSymbol()->asNamespace())
        {
            append("namespace ");
            a_pInput->getName();
            append(" = ");
            translate(a_pInput->getAliasedSymbol());
            append(';');
        }
        else if (a_pInput->getName().empty() &&
                 (a_pInput->getAliasedSymbol()->asSource() || a_pInput->getAliasedSymbol()->asPackage())) // import
        {
            append("HAUNT_IMPORT(\"" + a_pInput->getAliasedSymbol()->getUniqueName() + "\");");
        }
        else
        {
            beginDecl();
            appendError("alias cannot be translated to C++ equivalent typedef or using");
            append(';');
            endDecl();
        }
        return;
    }
    if (a_Data.id == TranslationType::Name)
        visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(AllocateExpression* a_pInput, VisitorData a_Data)
{
    append("o_allocate(");
    translate(a_pInput->getType(), TranslationType::Name);
    append(", ");
    a_pInput->getSizeExpression()->visit(this, a_Data);
    append(")");
}
void CppTranslator::visit(AnonymousSection* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(AnonymousStruct* a_pInput, VisitorData a_Data)
{
    visit(static_cast<AnonymousSection*>(a_pInput), a_Data);
}
void CppTranslator::visit(AnonymousUnion* a_pInput, VisitorData a_Data)
{
    visit(static_cast<AnonymousSection*>(a_pInput), a_Data);
}
void CppTranslator::visit(Application* a_pInput, VisitorData a_Data) {}
void CppTranslator::visit(ArrayExpression* a_pInput, VisitorData a_Data)
{
    append("{ ");
    for (auto pExp : a_pInput->getValues())
    {
        if (pExp != a_pInput->getValues()[0])
            append(", ");
        pExp->visit(this, a_Data);
    }
    append(" }");
}
void CppTranslator::visit(Array* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        a_pInput->getItemType()->visit(this, a_Data);
        append('[');
        append(phantom::lexical_cast<String>(a_pInput->getItemCount()));
        append(']');
        return;
    }
    else if (a_Data.id == TranslationType::Typed)
    {
        if (a_pInput->getItemType()->asArray())
        {
            a_pInput->getItemType()->visit(this, a_Data);
        }
        else
        {
            translate(a_pInput->getItemType());
            append(' ');
            append(*(String*)a_Data.in[0]);
        }
        append('[');
        if (a_pInput->getItemCount())
            append(lexical_cast<String>(a_pInput->getItemCount()));
        append(']');
        return;
    }
    visit(static_cast<ExtendedType*>(a_pInput), a_Data);
}
void CppTranslator::visit(BaseConstructorCallStatement* a_pInput, VisitorData a_Data) {}

void CppTranslator::visit(Block* a_pInput, VisitorData a_Data)
{
    switch (a_pInput->getTag())
    {
    case Block::e_Tag_Assert:
    {
        PHANTOM_ASSERT(a_pInput->getStatements().size());
        if (auto pBI = Object::Cast<BranchIfStatement>(a_pInput->getStatements().front()))
        {
            StringView assertMacro = getOption("assert-macro");
            if (assertMacro.empty())
                assertMacro = "assert";
            append(assertMacro);
            append('(');
            pBI->getExpression()->visit(this, a_Data);
            append(')');
            append(';');
        }
        else
        {
            PHANTOM_ASSERT(false);
        }
    }
    break;
    case Block::e_Tag_If:
    {
        beginIf();
        auto                                  it = a_pInput->getStatements().begin();
        LocalVariableInitializationStatement* locVarInit = nullptr;
        if (a_pInput->getLocalVariableCount())
        {
            locVarInit = a_pInput->findLocalVariableInitializationStatement(a_pInput->getLocalVariable(0));
            if (locVarInit)
            {
                translate(locVarInit);
                ++it;
            }
            else
            {
                appendError("expression");
            }
        }
        for (; it != a_pInput->getStatements().end(); ++it)
        {
            if ((*it)->asBlock())
                break;
            BranchIfNotStatement* pIfNot = (*it)->asBranchIfNotStatement();
            if (pIfNot)
            {
                if (locVarInit == nullptr)
                    pIfNot->getExpression()->visit(this, a_Data);
            }
            else
            {
                appendError("expression");
            }
        }
        endIf();

        Statements statements(it, a_pInput->getStatements().end());
        if (statements.size() == 5     // block+else+block+end => {} else {}
            || statements.size() == 2) // block+end => {}
        {
            if (statements[0]->asBlock())
            {
                indent();
                statements[0]->visit(this, a_Data);
            }
            else
            {
                beginDecl();
                appendError("missing then block in if statement");
                append(';');
                endDecl();
            }
            Label* pLabel = statements.size() == 5 ? statements[2]->asLabel() : statements[1]->asLabel();
            if (pLabel)
            {
                if (pLabel->getTag() == Label::e_Tag_Else && statements.size() == 5)
                {
                    Block* pBlock = statements[3]->asBlock();
                    if (pBlock)
                    {
                        if (pBlock->getTag() == Block::e_Tag_If)
                        {
                            indent();
                            beginElseIf();
                        }
                        else
                        {
                            newLine();
                            beginDecl();
                            append("else");
                            endDecl();
                            indent();
                        }
                        pBlock->visit(this, a_Data);
                    }
                    else
                    {
                        beginDecl();
                        appendError("missing else block in if statement");
                        append(';');
                        endDecl();
                    }
                }
                else if (pLabel->getTag() == Label::e_Tag_Else && statements.size() == 2)
                {
                }
                else
                {
                    beginDecl();
                    appendError("invalid first tagged label in if statement");
                    append(';');
                    endDecl();
                }
            }
            else
            {
                beginDecl();
                appendError("missing else/endif tagged label in if statement");
                append(';');
                endDecl();
            }
        }
        else
        {
            beginDecl();
            appendError("invalid if statement formatting");
            append(';');
            endDecl();
        }
    }
    break;
    case Block::e_Tag_Foreach:
    {
        beginFor();
        Expression* pContainerExp = nullptr;
        Block*      pUserBlock = nullptr;
        for (auto pS : a_pInput->getStatements())
        {
            if (auto lvis = Object::Cast<LocalVariableInitializationStatement>(pS))
            {
                if (lvis->getLocalVariable()->getName() == "$c")
                {
                    pContainerExp = lvis->getInitializationExpression();
                }
            }
            else if ((pUserBlock = Object::Cast<Block>(pS)))
            {
                PHANTOM_ASSERT(pContainerExp);
                PHANTOM_ASSERT(pUserBlock->getLocalVariableCount() >= 1);
                translateTyped(pUserBlock->getLocalVariables().front()->getValueType(),
                               pUserBlock->getLocalVariables().front()->getName());
                append(" : ");
                pContainerExp->visit(this, a_Data);
                endFor();
                PHANTOM_ASSERT(pUserBlock->getStatements().size() == 2);
                pUserBlock->getStatements()[1]->visit(this, a_Data);
            }
        }
    }
    break;

    case Block::e_Tag_For:
    {
        beginFor();
        LocalVariable* pLocal = a_pInput->getLocalVariableCount() ? a_pInput->getLocalVariable(0) : nullptr;
        enum state
        {
            e_init,
            e_branch_if_not,
            e_label_start,
            e_block,
            e_label_continue,
            e_update,
            e_branch_if,
            e_break,
        };
        state  s = e_init;
        Block* pContentBlock = nullptr;
        for (Statement* pStatement : a_pInput->getStatements())
        {
            switch (s)
            {
            case e_init:
            {
                LocalVariableInitializationStatement* pLVI = pStatement->asLocalVariableInitializationStatement();
                if (pLVI)
                {
                    if (pLVI->getLocalVariable() == pLocal)
                    {
                        pLVI->visit(this, a_Data);
                        s = e_branch_if_not;
                    }
                    else
                    {
                        appendError(
                        "invalid for statement formatting in initialization section : local variables match failure");
                    }
                    break;
                }
                else if (pStatement->asExpressionStatement())
                {
                    static_cast<ExpressionStatement*>(pStatement)->getExpression()->visit(this, a_Data);
                    append(';');
                    s = e_branch_if_not;
                    break;
                }
                else
                {
                    append(';');
                }
            }
            case e_branch_if_not:
            {
                if (pStatement->asBranchIfNotStatement())
                {
                    static_cast<BranchIfNotStatement*>(pStatement)->getExpression()->visit(this, a_Data);
                    s = e_label_start;
                    append(';');
                    break;
                }
                else
                    append(';');
            }
            case e_label_start:
            {
                if (pStatement->asLabel() == nullptr)
                {
                    appendError("invalid for statement formatting, missing branch to start");
                    return;
                }
                s = e_block;
            }
            break;
            case e_block:
            {
                pContentBlock = pStatement->asBlock();
                s = e_label_continue;
            }
            break;
            case e_label_continue:
            {
                if (pStatement->asLabel() == nullptr)
                {
                    appendError("invalid for statement formatting, missing continue label");
                    return;
                }
                s = e_update;
            }
            break;
            case e_update:
            {
                if (pStatement->asExpressionStatement())
                {
                    pStatement->asExpressionStatement()->getExpression()->visit(this, a_Data);
                    s = e_branch_if;
                    break;
                }
            }
            case e_branch_if:
            {
                if (pStatement->asBranchIfStatement() == nullptr)
                {
                    appendError("invalid for statement formatting, missing second conditional branch");
                    return;
                }
                endFor();
                s = e_break;
            }
            break;
            case e_break:
            {
                if (pStatement->asLabel() == nullptr)
                {
                    appendError("invalid for statement formatting, missing break label");
                    return;
                }
                if (pContentBlock)
                {
                    pContentBlock->visit(this, a_Data);
                }
                else
                {
                    append(';');
                }
            }
            break;
            }
        }
    }
    break;

    case Block::e_Tag_While:
    {
        beginWhile();
        enum state
        {
            e_local_var,
            e_branch_if_not,
            e_block,
            e_branch_continue,
            e_label_break,
        };
        state                                 s = e_local_var;
        LocalVariableInitializationStatement* locVarInit = nullptr;
        for (Statement* pStatement : a_pInput->getStatements())
        {
            switch (s)
            {
            case e_local_var:
            {
                s = e_branch_if_not;
                if ((locVarInit = Object::Cast<LocalVariableInitializationStatement>(pStatement)))
                {
                    translate(locVarInit);
                    break;
                }
            }
            case e_branch_if_not:
            {
                PHANTOM_ASSERT(pStatement->asBranchIfNotStatement());
                if (locVarInit == nullptr)
                    static_cast<BranchIfNotStatement*>(pStatement)->getExpression()->visit(this, a_Data);
                endWhile();
                s = e_block;
            }
            break;
            case e_block:
            {
                Block* pBlock = pStatement->asBlock();
                if (pBlock == nullptr)
                    appendError("invalid while statement formatting, missing while block");
                if (pBlock->getLocalVariables().empty() && pBlock->getStatements().empty())
                    append(";");
                else
                    pStatement->visit(this, a_Data);
                s = e_branch_continue;
            }
            break;
            case e_branch_continue:
            {
                if (pStatement->asBranchStatement() == nullptr)
                {
                    appendError("invalid while statement formatting, missing continue branch");
                    return;
                }
                s = e_label_break;
            }
            break;
            case e_label_break:
            {
                if (pStatement->asLabel() == nullptr)
                {
                    appendError("invalid while statement formatting, missing break label");
                    return;
                }
                else if (static_cast<Label*>(pStatement)->getLabelName() != "break")
                {
                    appendError(
                    "invalid while statement formatting, missing break label (found label without 'break' name)");
                }
                s = e_label_break;
            }
            break;
            }
        }
    }
    break;

    case Block::e_Tag_Do:
    {
        beginDo();
        enum state
        {
            e_label_start,
            e_block,
            e_label_continue,
            e_branch_if,
            e_label_break,
        };
        state  s = e_label_start;
        Block* pContentBlock = nullptr;
        for (Statement* pStatement : a_pInput->getStatements())
        {
            switch (s)
            {
            case e_label_start:
            {
                if (pStatement->asLabel() == nullptr)
                {
                    appendError("invalid do while statement formatting, missing branch to start");
                    return;
                }
                s = e_block;
            }
            break;
            case e_block:
            {
                pContentBlock = pStatement->asBlock();
                if (pContentBlock)
                {
                    pContentBlock->visit(this, a_Data);
                }
                else
                {
                    appendError("missing do while statement block");
                    return;
                }
                s = e_label_continue;
            }
            break;
            case e_label_continue:
            {
                if (pStatement->asLabel() == nullptr)
                {
                    appendError("invalid do while statement formatting, missing continue label");
                    return;
                }
                s = e_branch_if;
            }
            break;
            case e_branch_if:
            {
                if (pStatement->asBranchIfStatement() == nullptr)
                {
                    appendError("invalid do while statement formatting, missing second conditional branch");
                    return;
                }
                beginDecl();
                append("while(");
                static_cast<BranchIfStatement*>(pStatement)->getExpression()->visit(this, a_Data);
                append(");");
                endDecl();
                s = e_label_break;
            }
            break;
            case e_label_break:
            {
                if (pStatement->asLabel() == nullptr)
                {
                    appendError("invalid do while statement formatting, missing break label");
                    return;
                }
            }
            break;
            }
        }
    }
    break;

    case Block::e_Tag_Switch:
    {
        enum state
        {
            e_branchs,
            e_case_label,
            e_block,
            e_label_break,
        };
        state                         s = e_branchs;
        SmallVector<BranchStatement*> branchs;
        SmallVector<Block*>           blocks;
        Block*                        pContentBlock = nullptr;
        int                           branchIndex = 0;
        bool                          hasDefault = false;
        Expressions                   constantExpressions;
        bool                          switchWritten = false;
        for (Statement* pStatement : a_pInput->getStatements())
        {
            switch (s)
            {
            case e_branchs:
            {
                if (auto pB = pStatement->asBranchStatement())
                {
                    if (!switchWritten)
                    {
                        if (auto pBI = pB->asBranchIfStatement())
                        {
                            append("switch(");
                            Object::Cast<BuiltInOperatorExpression>(pBI->getExpression())
                            ->getInputArgument(0)
                            ->visit(this, a_Data);
                            append(')');
                            newLine();
                            indent();
                            beginBlock();
                        }
                        switchWritten = true;
                    }
                    branchs.push_back(pB);
                    break;
                }
            }
            case e_case_label:
                PHANTOM_ASSERT(pStatement->asLabel());
                s = e_block;
                break;
            case e_block:
                PHANTOM_ASSERT(pStatement->asBlock());
                blocks.push_back(static_cast<Block*>(pStatement));
                s = e_case_label;
                break;
            }
        }
        PHANTOM_ASSERT(blocks.size() == branchs.size());

        if (switchWritten)
        {
            for (size_t i = 0; i < blocks.size(); ++i)
            {
                beginDecl();
                if (auto pBIf = branchs[i]->asBranchIfStatement())
                {
                    append("case ");
                    Object::Cast<BuiltInOperatorExpression>(pBIf->getExpression())
                    ->getInputArgument(1)
                    ->visit(this, a_Data);
                    append(":");
                }
                else
                {
                    append("default:");
                }
                // shrink block
                blocks[i]->visit(this, a_Data);
                endDecl();
            }
            endBlock();
        }
    }
    break;

    default:
        if (a_pInput->getStatements().size() == 1 && a_pInput->getLocalVariables().empty() &&
            a_pInput->getStatements().front()->asBlock() && a_pInput->getStatements().front()->asBlock()->getTag() == 0)
        {
            a_pInput->getStatements().front()->visit(this, a_Data);
        }
        else
        {
            beginBlock();
            for (auto* pLoc : a_pInput->getLocalVariables())
            {
                if (pLoc->asParameter())
                    continue;

                Type* pType = pLoc->getValueType();

                addForwardIfRequired(pType);
                addIncludeIfRequired(pType);

                LocalVariableInitializationStatement* pInit = a_pInput->findLocalVariableInitializationStatement(pLoc);
                if (pInit == nullptr && !(pLoc->isThis()))
                {
                    if (pType->asClassType() || pType->asEnum())
                    {
                        addInclude(pType);
                    }
                    beginDecl();
                    translateTyped(pType, pLoc->getName());
                    append(';');
                    endDecl();
                }
            }
            for (auto* pStatement : a_pInput->getStatements())
            {
                beginDecl();
                pStatement->visit(this, a_Data);
                endDecl();
            }
            endBlock();
        }
        break;
    }
}
void CppTranslator::visit(BranchIfNotStatement* a_pInput, VisitorData a_Data)
{
    if (a_pInput->getLabel()->getTag() == 0)
    {
        append("if(!(");
        a_pInput->getExpression()->visit(this, a_Data);
        append(")) goto ");
        append(a_pInput->getLabel()->getLabelName());
        append(';');
    }
    visit(static_cast<BranchStatement*>(a_pInput), a_Data);
}
void CppTranslator::visit(BranchIfStatement* a_pInput, VisitorData a_Data)
{
    if (a_pInput->getLabel()->getTag() == 0)
    {
        append("if(");
        a_pInput->getExpression()->visit(this, a_Data);
        append(") goto ");
        append(a_pInput->getLabel()->getLabelName());
        append(';');
    }
}
void CppTranslator::visit(BranchStatement* a_pInput, VisitorData a_Data)
{
    if (a_pInput->getLabel()->getTag() == 0)
    {
        append("goto ");
        append(a_pInput->getLabel()->getLabelName());
        append(';');
    }
    else if (a_pInput->getLabel()->getTag() == Label::e_Tag_Break)
        append("break;");
    else if (a_pInput->getLabel()->getTag() == Label::e_Tag_Continue)
        append("continue;");
}
void CppTranslator::visit(BuiltInConversionExpression* a_pInput, VisitorData a_Data)
{
    if (a_pInput->getConversionType() == CastKind::Implicit)
    {
        a_pInput->getInputExpression()->visit(this, a_Data);
    }
    else
    {
        switch (a_pInput->getConversionType())
        {
        case CastKind::Const:
            append("const_cast<");
            break;
        case CastKind::Static:
            append("static_cast<");
            break;
        case CastKind::Reinterpret:
            if (auto pCst = Object::Cast<ConstantExpression>(a_pInput->getInputExpression()))
            {
                if (pCst->getValueType() == PHANTOM_TYPEOF(size_t))
                {
                    if (a_pInput->getValueType() == PHANTOM_TYPEOF(Type*))
                    {
                        addInclude(PHANTOM_TYPEOF(Type));
                        append("PHANTOM_TYPEOF(");
                        Type* pType = *reinterpret_cast<Type* const*>(pCst->getConstant()->getAddress());
                        translate(pType);
                        append(')');
                        return;
                    }
                    else if (a_pInput->getValueType() == PHANTOM_TYPEOF(Class*))
                    {
                        addInclude(PHANTOM_TYPEOF(Class));
                        append("PHANTOM_CLASSOF(");
                        Type* pType = *reinterpret_cast<Type* const*>(pCst->getConstant()->getAddress());
                        translate(pType);
                        append(')');
                        return;
                    }
                    else if (a_pInput->getValueType() == PHANTOM_TYPEOF(Union*))
                    {
                        addInclude(PHANTOM_TYPEOF(Union));
                        append("PHANTOM_UNIONOF(");
                        Type* pType = *reinterpret_cast<Type* const*>(pCst->getConstant()->getAddress());
                        translate(pType);
                        append(')');
                        return;
                    }
                    else if (a_pInput->getValueType() == PHANTOM_TYPEOF(Enum*))
                    {
                        addInclude(PHANTOM_TYPEOF(Enum));
                        append("PHANTOM_ENUMOF(");
                        Type* pType = *reinterpret_cast<Type* const*>(pCst->getConstant()->getAddress());
                        translate(pType);
                        append(')');
                        return;
                    }
                }
            }

            append("reinterpret_cast<");
            break;
        case CastKind::Explicit:
            append("(");
            translate(a_pInput->getValueType());
            append(")(");
            a_pInput->getInputExpression()->visit(this, a_Data);
            append(")");
            return;
        }
        translate(a_pInput->getValueType());
        append(">(");
        a_pInput->getInputExpression()->visit(this, a_Data);
        append(")");
    }
}
void CppTranslator::visit(BuiltInOperator* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(BuiltInOperatorExpression* a_pInput, VisitorData a_Data)
{
    String name = a_pInput->getOperator()->getName();
    auto   op = a_pInput->getOperator()->getId();
    bool   paren = requiresParenthesis(op);
    switch (a_pInput->getOperator()->getOperatorType())
    {
    case OperatorKind::Binary:
        if (a_pInput->getOperator()->getId() == Operator::Bracket)
        {
            beginParen(paren);
            appendExpression(a_pInput->getInputArgument(0), a_pInput, a_Data);
            endParen(paren);
            append('[');
            appendExpression(a_pInput->getInputArgument(1), a_pInput, a_Data);
            append(']');
        }
        else
        {
            Type* pT0 = a_pInput->getInputArgument(0)->getValueType();
            Type* pT1 = a_pInput->getInputArgument(1)->getValueType();
            Type* pTe0 = a_pInput->getEvalArgument(0)->getValueType();
            Type* pTe1 = a_pInput->getEvalArgument(1)->getValueType();
            if (pT0 != pTe0)
            {
                if (pT0->asPointer() && pTe0->asPointer())
                {
                    addIncludeIfRequired(pT0->getUnderlyingType());
                    addIncludeIfRequired(pTe0->getUnderlyingType());
                }
            }
            if (pT1 != pTe1)
            {
                if (pT1->asPointer() && pTe1->asPointer())
                {
                    addIncludeIfRequired(pT1->getUnderlyingType());
                    addIncludeIfRequired(pTe1->getUnderlyingType());
                }
            }
            beginParen(paren);
            m_OperatorStack.emplace_back(op, true);
            appendExpression(a_pInput->getInputArgument(0), a_pInput, a_Data);
            m_OperatorStack.pop_back();
            append(' ');
            append(name);
            append(' ');
            m_OperatorStack.emplace_back(op, false);
            appendExpression(a_pInput->getInputArgument(1), a_pInput, a_Data);
            m_OperatorStack.pop_back();
            endParen(paren);
        }
        break;
    case OperatorKind::UnaryPrefixed:
        beginParen(paren);
        append(name);
        m_OperatorStack.emplace_back(op, false);
        appendExpression(a_pInput->getInputArgument(0), a_pInput, a_Data);
        m_OperatorStack.pop_back();
        endParen(paren);
        break;
    case OperatorKind::UnaryPostfixed:
        beginParen(paren);
        m_OperatorStack.emplace_back(op, true);
        appendExpression(a_pInput->getInputArgument(0), a_pInput, a_Data);
        m_OperatorStack.pop_back();
        append(name);
        endParen(paren);
        break;
    case OperatorKind::Ternary:
        beginParen(paren);
        appendExpression(a_pInput->getInputArgument(0), a_pInput, a_Data);
        append("?");
        beginParen(true);
        appendExpression(a_pInput->getInputArgument(1), a_pInput, a_Data);
        endParen(true);
        append(":");
        beginParen(true);
        appendExpression(a_pInput->getInputArgument(2), a_pInput, a_Data);
        endParen(true);
        endParen(paren);
        break;
    default:
        PHANTOM_ASSERT(false);
    }
}

void CppTranslator::visit(CallExpression* a_pInput, VisitorData a_Data)
{
    Subroutine* pSubroutine = a_pInput->getSubroutine();
    PHANTOM_ASSERT(pSubroutine);

    Method*     pMethod = pSubroutine->asMethod();
    Expression* this_ = a_pInput->getArguments().size() ? a_pInput->getArguments().front() : nullptr;
    if (pMethod)
    {
        this_ = this_->clone(this_->getSource())->dereference(this_->getSource());
        addInclude(this_->getValueType()->removeReference()->removeAllQualifiers());
    }
    else
    {
        addInclude(pSubroutine);
    }

    if (a_pInput->isImplicit())
    {
        if (a_pInput->getArguments().size() == 1)
        {
            // conversion function
            this_->visit(this, a_Data);
        }
        else
        {
            PHANTOM_ASSERT(false);
        }
        this_->getSource()->Delete(this_);
        return;
    }

    const Expressions& arguments = a_pInput->getArguments();
    String             subroutineName = pSubroutine->getName();
    bool               needForceFinal = a_pInput->isFinal();
    PHANTOM_ASSERT(subroutineName.size());
    if (subroutineName.compare(0, 8, "operator") == 0 && !needForceFinal)
    {
        char c = subroutineName[8];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'))
        {
            String op = subroutineName.substr(8);
            if (op == "[]" || op == "()")
            {
                BuiltInOperatorExpression* pOpExp = arguments[0]->as<BuiltInOperatorExpression>();
                if (pOpExp && pOpExp->getOperator()->getId() == Operator::Address) /// remove self canceling '&*' case
                {
                    appendExpression(pOpExp->getInputArgument(0), a_pInput, a_Data);
                }
                else
                {
                    appendExpression(this_, a_pInput, a_Data);
                }
                append(op[0]); // ( or [
                if (arguments.size() > 1)
                {
                    for (size_t i = 1; i < arguments.size(); ++i)
                    {
                        if (i != 1)
                            append(',');
                        arguments[i]->visit(this, a_Data);
                    }
                }
                append(op[1]); // ) or ]
            }
            else if (op[0] == ' ') /// conversion function
            {
                auto ce = Object::Cast<CallExpression>(this_->removeRValueReferenceExpression());
                if (ce && ce->getSubroutine()->isNative() &&
                    ce->getSubroutine()->getReturnType()->getQualifiedName() == "phantom::Generic::Return")
                {
                    this_->visit(this, a_Data);
                }
                else
                {
                    bool ccast = (pSubroutine->getReturnType()->asClassType() == nullptr &&
                                  pSubroutine->getReturnType()->asEnum() == nullptr);
                    bool paren = ccast && requiresParenthesis(Operator::Not); // same precedent as cstyle cast
                    beginParen(paren);
                    if (ccast)
                        append('(');
                    translate(pSubroutine->getReturnType());
                    if (ccast)
                        append(')');
                    if (!ccast)
                        append('(');
                    if (ccast)
                        m_OperatorStack.emplace_back(Operator::Not, false);
                    this_->visit(this, a_Data);
                    if (ccast)
                        m_OperatorStack.pop_back();
                    if (!ccast)
                        append(')');
                    endParen(paren);
                }
            }
            else
            {
                if (arguments.size() == 2) // binary or post unary
                {
                    BuiltInOperator* pOp = CppLite::Get()->getBuiltInBinaryOperator(op);
                    if (pOp) // binary operator
                    {
                        bool paren = requiresParenthesis(pOp->getId()); // same precedent as cstyle cast
                        beginParen(paren);
                        m_OperatorStack.emplace_back(pOp->getId(), true);
                        appendExpression(this_, a_pInput, a_Data);
                        m_OperatorStack.pop_back();
                        append(' ');
                        append(op);
                        append(' ');
                        m_OperatorStack.emplace_back(pOp->getId(), false);
                        appendExpression(arguments[1], a_pInput, a_Data);
                        m_OperatorStack.pop_back();
                        endParen(paren);
                    }
                    else // post unary
                    {
                        pOp = CppLite::Get()->getBuiltInPostUnaryOperator(op);
                        PHANTOM_ASSERT(pOp);
                        bool paren = requiresParenthesis(pOp->getId()); // same precedent as cstyle cast
                        beginParen(paren);
                        m_OperatorStack.emplace_back(pOp->getId(), true);
                        appendExpression(this_, a_pInput, a_Data);
                        m_OperatorStack.pop_back();
                        append(op);
                        endParen(paren);
                    }
                }
                else // pre unary
                {
                    if (op != "->")
                    {
                        auto opId = CppLite::Get()->getBuiltInPreUnaryOperatorId(op);
                        bool paren = requiresParenthesis(opId); // same precedent as cstyle cast
                        beginParen(paren);
                        append(op);
                        m_OperatorStack.emplace_back(opId, false);
                        appendExpression(this_, a_pInput, a_Data);
                        m_OperatorStack.pop_back();
                        endParen(paren);
                    }
                    else
                    {
                        appendExpression(this_, a_pInput, a_Data);
                    }
                }
                // Create operator expression
            }
            return;
        }
    }

    int argIdx = 0;
    if (pSubroutine->asMethod() && pSubroutine->asConstructor() == nullptr)
    {
        ++argIdx;
        // check if LHS is implicit 'this' expression
        bool        skipLHS = false;
        Expression* dotExp = nullptr;

        Expression* argNoIdentity = arguments[0]->removeImplicitIdentity();
        if (needForceFinal)
        {
            if (auto biop = Object::Cast<BuiltInOperatorExpression>(argNoIdentity))
            {
                if (biop->getOperator()->getId() == Operator::Address)
                {
                    argNoIdentity = biop->getInputArgument(0)->removeIdentity();
                    if (auto le = Object::Cast<LoadExpression>(argNoIdentity))
                    {
                        if (auto lve = Object::Cast<LocalVariableExpression>(le->getLoadedExpression()))
                        {
                            skipLHS =
                            lve->getLocalVariable() == nullptr || lve->getLocalVariable()->getName() == "this";
                        }
                    }
                    else if (auto pa = Object::Cast<PointerAdjustmentExpression>(argNoIdentity))
                    {
                        if (auto biop2 = Object::Cast<BuiltInOperatorExpression>(pa->getPointerExpression()))
                        {
                            if (biop2->getOperator()->getId() == Operator::Dereference)
                            {
                                if (auto le2 = Object::Cast<LoadExpression>(biop2->getInputArgument(0)))
                                {
                                    if (auto lve = Object::Cast<LocalVariableExpression>(le2->getLoadedExpression()))
                                    {
                                        skipLHS = lve->getLocalVariable() == nullptr ||
                                        lve->getLocalVariable()->getName() == "this";
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (auto le = Object::Cast<LoadExpression>(argNoIdentity))
        {
            if (auto lve = Object::Cast<LocalVariableExpression>(le->getLoadedExpression()))
            {
                auto lv = lve->getLocalVariable();
                skipLHS = (lv == nullptr || lv->getName() == "this");
            }
        }
        if (!skipLHS)
        {
            if (auto bie = Object::Cast<BuiltInOperatorExpression>(argNoIdentity))
            {
                if (bie->getOperator()->getId() == Operator::Address)
                {
                    dotExp = bie->getEvalArgument(0);
                }
            }
        }
        if (!skipLHS)
        {
            if (dotExp)
            {
                m_OperatorStack.emplace_back(Operator::Arrow, true);
                appendExpression(dotExp, a_pInput, a_Data);
                m_OperatorStack.pop_back();
                append(".");
            }
            else
            {
                m_OperatorStack.emplace_back(Operator::Arrow, true);
                appendExpression(arguments[0], a_pInput, a_Data);
                m_OperatorStack.pop_back();
                append("->");
            }
        }
        if (needForceFinal)
        {
            translate(pSubroutine->getOwner());
            append("::");
        }
        append(subroutineName);
    }
    else
    {
        StringBuffer buf;
        pSubroutine->getRelativeName(getContextScope(), buf);
        append(buf);
    }

    // translate "generic" calls

    if (pSubroutine->isNative())
    {
        int c = 0;
        for (auto pParam : pSubroutine->getParameters())
        {
            Type* pType = pParam->getValueType();
            if (pType->getQualifiedName() == "phantom::Generic::Param")
            {
                Type* pArgType;
                if (auto pCtor = Object::Cast<ConstructorCallExpression>(arguments[argIdx]->removeImplicitIdentity()))
                    if (auto pConv = Object::Cast<BuiltInConversionExpression>(pCtor->getArguments().front()))
                        if (auto pCst = Object::Cast<ConstantExpression>(pConv->getInputExpression()))
                        {
                            if (c++)
                                append(", ");
                            else
                                append('<');
                            pCst->load(&pArgType);
                            addIncludeIfRequired(pArgType);
                            addForwardIfRequired(pArgType);
                            translate(pArgType);
                            argIdx++;
                        }
            }
        }
        if (c)
            append('>');
    }

    append('(');
    for (auto i = argIdx; i < arguments.size(); ++i)
    {
        if (i != argIdx)
            append(", ");
        arguments[i]->visit(this, a_Data);
    }
    append(')');

    if (pMethod)
        this_->getSource()->Delete(this_);
}

void CppTranslator::visit(Class* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration)
    {
        _appendHauntMarkers(a_pInput);
        pushContextScope(a_pInput);
        if ((a_pInput->getTemplateSpecialization() == nullptr || a_pInput->getTemplateSpecialization()->isFull()))
        {
            String exp = getOption("export-macro");
            append("class " + (exp.size() ? (exp + ' ') : "") + a_pInput->getName());
        }
        else
        {
            append("class " + a_pInput->getName());
        }
        if (auto pSpec = a_pInput->getTemplateSpecialization())
        {
            if (!pSpec->isEmpty())
            {
                append('<');
                int c = 0;
                for (auto pArg : pSpec->getArguments())
                {
                    if (c++)
                        append(", ");
                    translate(pArg);
                }
                append('>');
            }
        }
        auto& baseClasses = a_pInput->getBaseClasses();
        for (auto it = baseClasses.begin(); it != baseClasses.end(); ++it)
        {
            if (it == baseClasses.begin())
                append(" :");
            else
                append(',');
            switch (it->access)
            {
            case Access::Public:
                append(" public ");
                break;
            case Access::Protected:
                append(" protected ");
                break;
            case Access::Private:
                append(" private ");
                break;
            }
            translate(it->baseClass);
            addInclude(m_IncludesH, it->baseClass);
        }
        newLine();
        indent();
        beginBlock();
        visit(static_cast<ClassType*>(a_pInput), a_Data);
        endBlock("};");
        popContextScope();
        return;
    }
    else
    {
        visit(static_cast<ClassType*>(a_pInput), a_Data);
    }
}

void CppTranslator::visit(ClassListInitializationExpression* a_pInput, VisitorData a_Data)
{
    translate(a_pInput->getValueType());
    append("{");
    int c = 0;
    for (auto exp : a_pInput->getExpressions())
    {
        if (exp)
        {
            if (c++)
                append(", ");
            exp->visit(this, a_Data);
        }
    }
    append("}");
}
void CppTranslator::visit(ClassTypeListInitializationExpression* a_pInput, VisitorData a_Data)
{
    translate(a_pInput->getValueType());
    append("{");
    int c = 0;
    for (auto exp : a_pInput->getExpressions())
    {
        if (exp)
        {
            if (c++)
                append(", ");
            exp->visit(this, a_Data);
        }
    }
    append("}");
}

#define print_public_privates_protecteds(d)                                                                            \
    {                                                                                                                  \
        if (publics.size())                                                                                            \
        {                                                                                                              \
            label(public_label);                                                                                       \
            for (auto it = publics.begin(); it != publics.end(); ++it)                                                 \
            {                                                                                                          \
                beginDecl();                                                                                           \
                (*it)->visit(this, d);                                                                                 \
                endDecl();                                                                                             \
            }                                                                                                          \
            append('\n');                                                                                              \
            publics.clear();                                                                                           \
        }                                                                                                              \
                                                                                                                       \
        if (protecteds.size())                                                                                         \
        {                                                                                                              \
            label(protected_label);                                                                                    \
            for (auto it = protecteds.begin(); it != protecteds.end(); ++it)                                           \
            {                                                                                                          \
                beginDecl();                                                                                           \
                (*it)->visit(this, d);                                                                                 \
                endDecl();                                                                                             \
            }                                                                                                          \
            append('\n');                                                                                              \
            protecteds.clear();                                                                                        \
        }                                                                                                              \
                                                                                                                       \
        if (privates.size())                                                                                           \
        {                                                                                                              \
            label(private_label);                                                                                      \
            for (auto it = privates.begin(); it != privates.end(); ++it)                                               \
            {                                                                                                          \
                beginDecl();                                                                                           \
                (*it)->visit(this, d);                                                                                 \
                endDecl();                                                                                             \
            }                                                                                                          \
            append('\n');                                                                                              \
            privates.clear();                                                                                          \
        }                                                                                                              \
    }

void CppTranslator::visit(ClassType* a_pInput, VisitorData a_Data)
{
    bool        bIsClassTemplate = a_pInput->getTemplateSpecialization() != nullptr;
    VisitorData templateData = a_Data;
    if (a_Data.id == TranslationType::Declaration)
    {
        beginDecl();
        append("PHANTOM_PRIVATE_ACCESS;");
        endDecl();

        for (auto pFriend : a_pInput->getFriends())
        {
            String str;
            if (auto pSpec = pFriend->getTemplateSpecialization())
            {
                _formatTemplateSign(a_pInput, pSpec->getTemplate()->getTemplateSignature(), str);
                str += ' ';
            }
            str += "friend ";
            if (pFriend->asClass())
            {
                str += "class ";
            }
            else if (pFriend->asUnion())
            {
                str += "union ";
            }
            else if (auto pE = pFriend->asEnum())
            {
                str += "enum ";
                if (pE->isScoped())
                    str += "class ";
            }
            StringBuffer buf;
            pFriend->getRelativeName(a_pInput, buf);
            str += StringView(buf);
            str += ';';
            beginDecl();
            append(str);
            endDecl();
        }

        const char* public_label = "public";
        const char* protected_label = "protected";
        const char* private_label = "private";

        Symbols publics;
        Symbols protecteds;
        Symbols privates;

        for (auto pAlias : a_pInput->getAliases())
        {
            if (pAlias->isPublic())
                publics.push_back(pAlias);
            else if (pAlias->isProtected())
                protecteds.push_back(pAlias);
            else
                privates.push_back(pAlias);
        }

        print_public_privates_protecteds(a_Data);

        for (auto pTS : a_pInput->getTemplateSpecializations())
        {
            if (pTS->getTemplateSignature() == nullptr)
                continue; // if the template specialization is an instantiation
            if (pTS->isPublic())
                publics.push_back(pTS);
            else if (pTS->isProtected())
                protecteds.push_back(pTS);
            else
                privates.push_back(pTS);
        }

        print_public_privates_protecteds(a_Data);

        std::map<LanguageElement*, int>        classesPrios;
        std::map<int, SmallVector<ClassType*>> orderedTypes;

        for (auto pElem : a_pInput->getTypes())
        {
            if ((pElem)->asEnum())
            {
                if ((pElem)->isPublic())
                    publics.push_back(pElem);
                else if ((pElem)->isProtected())
                    protecteds.push_back(pElem);
                else
                    privates.push_back(pElem);
            }
            if (auto cl = (pElem)->asClassType())
            {
                orderedTypes[priorityComputer(a_pInput, cl, classesPrios)].push_back(cl);
            }
        }

        for (auto& prio_type : orderedTypes)
        {
            for (auto type : prio_type.second)
            {
                if (type->isPublic())
                    publics.push_back(type);
                else if (type->isProtected())
                    protecteds.push_back(type);
                else
                    privates.push_back(type);
            }
        }

        print_public_privates_protecteds(a_Data);

        for (auto pElem : a_pInput->getFunctions())
        {
            if ((pElem)->isPublic())
                publics.push_back(pElem);
            else if ((pElem)->isProtected())
                protecteds.push_back(pElem);
            else
                privates.push_back(pElem);
        }

        if (bIsClassTemplate)
        {
            print_public_privates_protecteds(templateData);
        }
        else
        {
            print_public_privates_protecteds(a_Data);
        }

        for (auto pElem : a_pInput->getConstructors())
        {
            if ((pElem)->testFlags(PHANTOM_R_FLAG_IMPLICIT))
                continue;
            if ((pElem)->isPublic())
                publics.push_back(pElem);
            else if ((pElem)->isProtected())
                protecteds.push_back(pElem);
            else
                privates.push_back(pElem);
        }

        if (bIsClassTemplate)
        {
            print_public_privates_protecteds(templateData);
        }
        else
        {
            print_public_privates_protecteds(a_Data);
        }

        for (auto pElem : a_pInput->getMethods())
        {
            if ((pElem)->asSignal() || (pElem)->testFlags(PHANTOM_R_FLAG_IMPLICIT))
                continue;
            if ((pElem)->isPublic())
                publics.push_back(pElem);
            else if ((pElem)->isProtected())
                protecteds.push_back(pElem);
            else
                privates.push_back(pElem);
        }

        if (bIsClassTemplate)
        {
            print_public_privates_protecteds(templateData);
        }
        else
        {
            print_public_privates_protecteds(a_Data);
        }

        for (auto pElem : a_pInput->getMethods())
        {
            if ((pElem)->asSignal() == nullptr || (pElem)->testFlags(PHANTOM_R_FLAG_IMPLICIT))
                continue;
            if ((pElem)->isPublic())
                publics.push_back(pElem);
            else if ((pElem)->isProtected())
                protecteds.push_back(pElem);
            else
                privates.push_back(pElem);
        }

        print_public_privates_protecteds(a_Data);

        for (auto pElem : a_pInput->getVariables())
        {
            if ((pElem)->isPublic())
                publics.push_back(pElem);
            else if ((pElem)->isProtected())
                protecteds.push_back(pElem);
            else
                privates.push_back(pElem);
        }

        print_public_privates_protecteds(a_Data);

        /// Data members are the only ones that we keep in same order to avoid class size changes due to
        /// alignment difference

        Access access = Access::Undefined;
        for (Field* pField : a_pInput->getFields())
        {
            Access thisAccess = pField->getAccess();
            if (access != thisAccess)
            {
                access = thisAccess;
                label((thisAccess == Access::Private)
                      ? private_label
                      : (thisAccess == Access::Protected) ? protected_label : public_label);
            }
            if (pField->getMemberAnonymousSection()) // data member belongs to an anonymous struct/union
            {
                if (pField->getMemberAnonymousSection()->getOwner() == a_pInput) // root section
                {
                    if (m_MemberAnonymousSections.find(pField->getMemberAnonymousSection()) ==
                        m_MemberAnonymousSections.end())
                    {
                        m_MemberAnonymousSections.insert(pField->getMemberAnonymousSection());
                        pField->getMemberAnonymousSection()->visit(this, a_Data);
                    }
                }
            }
            else
                pField->visit(this, a_Data);
        }
        m_MemberAnonymousSections.clear();
    }
    else if (a_Data.id == TranslationType::Definition)
    {
        if (!a_pInput->isTemplateElement())
        {
            for (auto pElem : a_pInput->getVariables())
            {
                beginDecl();
                (pElem)->visit(this, a_Data);
                endDecl();
            }
            for (auto pElem : a_pInput->getFunctions())
            {
                beginDecl();
                (pElem)->visit(this, a_Data);
                endDecl();
            }
            for (auto pElem : a_pInput->getConstructors())
            {
                if ((pElem)->testFlags(PHANTOM_R_FLAG_IMPLICIT) || pElem->testModifiers(Modifier::Defaulted) ||
                    pElem->testModifiers(Modifier::Deleted))
                    continue;
                beginDecl();
                (pElem)->visit(this, a_Data);
                endDecl();
            }
            for (auto pElem : a_pInput->getMethods())
            {
                if ((pElem)->testFlags(PHANTOM_R_FLAG_IMPLICIT) || pElem->testModifiers(Modifier::Defaulted) ||
                    pElem->testModifiers(Modifier::Deleted))
                    continue;
                beginDecl();
                (pElem)->visit(this, a_Data);
                endDecl();
            }
            for (auto pElem : a_pInput->getTypes())
            {
                if ((pElem)->testFlags(PHANTOM_R_FLAG_IMPLICIT) || pElem->asEnum())
                    continue;
                beginDecl();
                (pElem)->visit(this, a_Data);
                endDecl();
            }
            for (auto pElem : a_pInput->getTemplateSpecializations())
            {
                if ((pElem)->testFlags(PHANTOM_R_FLAG_IMPLICIT))
                    continue;
                if (!(pElem)->isFull())
                    continue;
                beginDecl();
                (pElem)->visit(this, a_Data);
                endDecl();
            }
        }
    }
    else
        visit(static_cast<Type*>(a_pInput), a_Data);
}
void CppTranslator::visit(ConditionalExpression* a_pInput, VisitorData a_Data)
{
    bool paren =
    requiresParenthesis(Operator::Assignment); // we don't have ternary yet but assignment has same precedence
    beginParen(paren);
    a_pInput->getConditionExpression()->visit(this, a_Data);
    append("? (");
    a_pInput->getThenExpression()->visit(this, a_Data);
    append(") : (");
    a_pInput->getElseExpression()->visit(this, a_Data);
    append(")");
    endParen(paren);
}
void CppTranslator::visit(Constant* a_pInput, VisitorData a_Data)
{
    if (a_pInput->getName().empty())
    {
        StringBuffer buffer;
        if (a_pInput->getValueType() == PHANTOM_TYPEOF(const char*))
        {
            phantom::String str((*(const char**)a_pInput->getAddress()));
            StringConverter<String>::toLiteral(nullptr, buffer, &str);
        }
        else
            a_pInput->toLiteral(buffer);
        m_pTranslation->append(buffer.data(), buffer.size());
    }
    else
    {
        visit(static_cast<Symbol*>(a_pInput), a_Data);
    }
}
void CppTranslator::visit(ConstantExpression* a_pInput, VisitorData a_Data)
{
    addInclude(a_pInput->getConstant());
    addIncludeIfRequired(a_pInput->getConstant()->getValueType());
    translate(a_pInput->getConstant());
}
void CppTranslator::visit(Constructor* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration || a_Data.id == TranslationType::Definition)
    {
        if (a_Data.id == TranslationType::Declaration)
            _appendHauntMarkers(a_pInput);

        StringBuffer buf;
        a_pInput->getRelativeName(getContextScope(), buf);
        append(buf);
        _appendParameterList(a_pInput, a_Data);
        _appendCtorInit(a_pInput, a_Data);
        _appendFuncBlock(a_pInput, a_Data);
    }
    else
        visit(static_cast<Subroutine*>(a_pInput), a_Data);
}
void CppTranslator::visit(ConstructorCallExpression* a_pInput, VisitorData a_Data)
{
    addInclude(a_pInput->getConstructor());
    if (a_pInput->isImplicit())
    {
        if (a_pInput->getArguments().size() == 1)
            a_pInput->getArguments().front()->visit(this, a_Data);
        else
        {
            PHANTOM_ASSERT(false);
        }
        return;
    }
    translate(a_pInput->getConstructor()->getOwner());
    append('(');
    for (auto it = a_pInput->beginArguments(); it != a_pInput->endArguments(); ++it)
    {
        if (it != a_pInput->beginArguments())
        {
            append(", ");
        }
        (*it)->visit(this, a_Data);
    }
    append(')');
}
void CppTranslator::visit(ConstType* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        a_pInput->getConstedType()->visit(this, a_Data);
        append(" const");
        return;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void CppTranslator::visit(ConstVolatileType* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        a_pInput->getConstVolatiledType()->visit(this, a_Data);
        append(" const volatile");
        return;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void CppTranslator::visit(ContainerClass* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Class*>(a_pInput), a_Data);
}
void CppTranslator::visit(Field* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration || a_Data.id == TranslationType::Definition)
    {
        _appendHauntMarkers(a_pInput);
        addForwardIfRequired(a_pInput->getValueType());
        addIncludeIfRequired(a_pInput->getValueType());
        beginDecl();

        if (a_pInput->getAlignment() != a_pInput->getValueType()->getAlignment())
        {
            append("alignas(" + lexical_cast<String>(a_pInput->getAlignment()) + ") ");
        }

        if (a_pInput->isMutable())
        {
            append("mutable ");
        }

        if (a_pInput->isTransient())
        {
            append("/// @transient ");
            endDecl();
            beginDecl();
        }
        else if (a_pInput->getFilterMask() != PHANTOM_R_FILTER_FIELD)
        {
            char buffer[9];
            sprintf(buffer, "%x", a_pInput->getFilterMask());
            append("/// @mask 0x");
            endDecl();
            beginDecl();
            append(buffer);
        }
        translateTyped(a_pInput->getValueType(), a_pInput->getName());
        if (a_pInput->getDefaultExpression())
        {
            append(" = ");
            a_pInput->getDefaultExpression()->visit(this, a_Data);
        }
        append(';');
        endDecl();
        return;
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(FieldExpression* a_pInput, VisitorData a_Data)
{
    addInclude(a_pInput->getObjectExpression()->getValueType()->removeConstReference());

    Expression* pObjNoIdentity = a_pInput->getObjectExpression()->removeImplicitIdentity();

    // check if LHS is implicit 'this' expression
    bool skipLHS = false;
    if (pObjNoIdentity->getMetaClass() == PHANTOM_CLASSOF(BuiltInOperatorExpression))
    {
        auto bie = static_cast<BuiltInOperatorExpression*>(a_pInput->getObjectExpression());
        if (bie->getOperator()->getId() == Operator::Dereference)
        {
            if (bie->getEvalArgument(0)->getMetaClass() == PHANTOM_CLASSOF(LoadExpression))
            {
                auto le = static_cast<LoadExpression*>(bie->getEvalArgument(0));
                if (le->getLoadedExpression()->getMetaClass() == PHANTOM_CLASSOF(LocalVariableExpression))
                {
                    auto lve = static_cast<LocalVariableExpression*>(le->getLoadedExpression());
                    skipLHS = lve->getLocalVariable() == nullptr || lve->getLocalVariable()->getName() == "this";
                }
            }
        }
    }
    if (!skipLHS)
    {
        m_OperatorStack.emplace_back(Operator::Arrow, true);
        appendExpression(a_pInput->getObjectExpression(), a_pInput, a_Data);
        m_OperatorStack.pop_back();
        append(".");
    }
    append(a_pInput->getField()->getName());
}
void CppTranslator::visit(FieldInitializationStatement* a_pInput, VisitorData a_Data) {}
void CppTranslator::visit(FieldPointerExpression* a_pInput, VisitorData a_Data)
{
    addInclude(a_pInput->getObjectExpression()->getValueType()->removeConstReference());
    append('(');
    a_pInput->getObjectExpression()->visit(this, a_Data);
    append(").*(");
    a_pInput->getFieldPointerExpression()->visit(this, a_Data);
    append(')');
}
void CppTranslator::visit(FieldPointer* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        a_pInput->getValueType()->visit(this, a_Data);
        append(" (");
        a_pInput->getObjectType()->visit(this, a_Data);
        append("::*)");
    }
    else if (a_Data.id == TranslationType::Typed)
    {
        a_pInput->getValueType()->visit(this, a_Data);
        append(" (");
        a_pInput->getObjectType()->visit(this, a_Data);
        append("::*");
        append(*(const String*)(a_Data.in[0]));
        append(')');
    }
}
void CppTranslator::visit(Pointer* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        a_pInput->getPointeeType()->visit(this, a_Data);
        append('*');
        return;
    }
    visit(static_cast<ExtendedType*>(a_pInput), a_Data);
}
void CppTranslator::visit(DeallocateExpression* a_pInput, VisitorData a_Data)
{
    append("o_deallocate(");
    translate(a_pInput->getType(), TranslationType::Name);
    append(", ");
    a_pInput->getSizeExpression()->visit(this, a_Data);
    append(") ");
    a_pInput->getAddressExpression()->visit(this, a_Data);
}
void CppTranslator::visit(DeleteExpression* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Definition)
    {
        if (a_pInput->getClass())
        {
            append("o_delete(");
            translateTypeName(a_pInput->getClass());
            append(')');
            append(' ');
            a_pInput->getExpression()->visit(this, a_Data);
        }
        else
        {
            append("o_dynamic_delete (");
            a_pInput->getExpression()->visit(this, a_Data);
            append(')');
        }
        return;
    }
}
void CppTranslator::visit(Ellipsis* a_pInput, VisitorData a_Data)
{ /*visit(static_cast<LanguageElement*>(a_pInput), a_Data);*/
}
void CppTranslator::visit(Enum* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration || a_Data.id == TranslationType::Definition)
    {
        if (a_Data.id == TranslationType::Definition)
        {
            if (a_pInput->getOwner()->asSource())
                return;
        }
        append("enum");
        if (a_pInput->getName().size())
        {
            if (a_pInput->isScoped())
                append(" class");
            append(' ');
            append(a_pInput->getName());
        }
        newLine();
        indent();
        beginBlock();
        auto& consts = a_pInput->getConstants();
        for (auto it = consts.begin(); it != consts.end(); ++it)
        {
            beginDecl();
            if (it != consts.begin())
                append(", ");

            StringView name = (*it)->getName();
            bool       keyword = IsKeyword(name);
            if (keyword)
            {
                append(name + m_NoKeywordChars);
            }
            else
            {
                append(name);
            }
            append(" = ");
            byte value[128];
            (*it)->getValue(value);
            StringBuffer buf;
            a_pInput->getUnderlyingIntType()->valueToString(buf, value);
            m_pTranslation->append(buf.data(), buf.size());
            if (keyword)
                append(" ///< @name " + name);
            endDecl();
        }
        endBlock("};");
    }
    else
        visit(static_cast<Type*>(a_pInput), a_Data);
}
void CppTranslator::visit(Evaluable* a_pInput, VisitorData a_Data)
{
    visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}
void CppTranslator::visit(Expression* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Evaluable*>(a_pInput), a_Data);
}
void CppTranslator::visit(ExpressionStatement* a_pInput, VisitorData a_Data)
{
    a_pInput->getExpression()->visit(this, a_Data);
    append(";");
}
void CppTranslator::visit(Function* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration || a_Data.id == TranslationType::Definition)
    {
        if (a_pInput->getOwner()->asClassType() == nullptr)
        {
            String exp = getOption("export-macro");
            if (exp.size())
            {
                append(exp + ' ');
            }
        }
        else
        {
            if (a_Data.id == TranslationType::Declaration)
            {
                append("static ");
            }
        }
        visit(static_cast<Subroutine*>(a_pInput), a_Data);
        if (a_Data.id == TranslationType::Declaration && !a_pInput->isTemplateElement() ||
            a_pInput->getBlock() == nullptr)
        {
            append(";");
        }
    }
    else
        visit(static_cast<Subroutine*>(a_pInput), a_Data);
}
void CppTranslator::visit(FunctionPointer* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        a_pInput->getFunctionType()->getReturnType()->visit(this, a_Data);
        append("(*)");
        append('(');
        for (auto it = a_pInput->getFunctionType()->beginParameterTypes();
             it != a_pInput->getFunctionType()->endParameterTypes(); ++it)
        {
            if (it != a_pInput->getFunctionType()->beginParameterTypes())
                append(',');
            (*it)->visit(this, a_Data);
        }
        append(')');
    }
    else if (a_Data.id == TranslationType::Typed)
    {
        a_pInput->getFunctionType()->getReturnType()->visit(this, a_Data);
        append("(*");
        append(*(String*)a_Data.in[0]);
        append(')');
        append('(');
        for (auto it = a_pInput->getFunctionType()->beginParameterTypes();
             it != a_pInput->getFunctionType()->endParameterTypes(); ++it)
        {
            if (it != a_pInput->getFunctionType()->beginParameterTypes())
                append(',');
            (*it)->visit(this, a_Data);
        }
        append(')');
    }
    else
        visit(static_cast<Type*>(a_pInput), a_Data);
}
void CppTranslator::visit(FunctionType* a_pInput, VisitorData a_Data)
{
    bool defOrDecl = (a_Data.id == TranslationType::Declaration || a_Data.id == TranslationType::Definition);
    auto pRType = a_pInput->getReturnType();
    pRType->visit(this, a_Data);
    if (defOrDecl)
    {
        addForwardIfRequired(pRType);
        addIncludeIfRequired(pRType);
    }
    append('(');
    int c = 0;
    for (auto pPType : a_pInput->getParameterTypes())
    {
        if (defOrDecl)
        {
            addForwardIfRequired(pPType);
            addIncludeIfRequired(pPType);
        }
        if (c++)
            append(',');
        pPType->visit(this, a_Data);
    }
    append(')');
}
void CppTranslator::visit(IdentityExpression* a_pInput, VisitorData a_Data)
{
    if (a_pInput->getValueType() == PHANTOM_TYPEOF(void))
    {
        append("(void)(");
        a_pInput->getExpression()->visit(this, a_Data);
        append(')');
        return;
    }

    if (a_pInput->getConversionKind() == CastKind::Implicit)
    {
        if (auto ptr = a_pInput->getExpression()->getValueType()->removeQualifiers()->asPointer())
        {
            if (auto pCl = ptr->getPointeeType()->asClass())
            {
                addIncludeIfRequired(pCl);
            }
        }
    }
    if (a_pInput->getConversionKind() == CastKind::Reinterpret)
    {
        if (auto pCall = Object::Cast<CallExpression>(a_pInput->getExpression()))
        {
            if (pCall->getSubroutine()->isNative())
            {
                if (pCall->getSubroutine()->getName().find("operator ") == 0)
                    if (pCall->getSubroutine()->getOwner()->getQualifiedName() == "phantom::Generic::Return")
                    {
                        return a_pInput->getExpression()->visit(this, a_Data);
                    }
            }
        }
        addForwardIfRequired(a_pInput->getValueType());
        append("reinterpret_cast<");
        translateTypeName(a_pInput->getValueType());
        append(">(");
        a_pInput->getExpression()->visit(this, a_Data);
        append(')');
    }
    else if (a_pInput->getConversionKind() == CastKind::Explicit)
    {
        append("(");
        translateTypeName(a_pInput->getValueType());
        append(")(");
        a_pInput->getExpression()->visit(this, a_Data);
        append(')');
    }
    else if (a_pInput->getConversionKind() == CastKind::Static)
    {
        addInclude(a_pInput->getValueType()->removePointer()->removeQualifiers());
        append("static_cast<");
        translateTypeName(a_pInput->getValueType());
        append(">(");
        a_pInput->getExpression()->visit(this, a_Data);
        append(')');
    }
    else if (a_pInput->getConversionKind() == CastKind::Const)
    {
        append("const_cast<");
        translateTypeName(a_pInput->getValueType());
        append(">(");
        a_pInput->getExpression()->visit(this, a_Data);
        append(')');
    }
    else
    {
        a_pInput->getExpression()->visit(this, a_Data);
    }
}
void CppTranslator::visit(InitializerListExpression* a_pInput, VisitorData a_Data)
{
    append("{");
    int c = 0;
    for (auto pExp : a_pInput->getExpressions())
    {
        if (c++)
            append(',');
        pExp->visit(this, a_Data);
    }
    append("}");
}
void CppTranslator::visit(Label* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        if (a_pInput->getTag() == 0)
        {
            append(a_pInput->getLabelName());
        }
    }
    else
    {
        if (a_pInput->getTag() == 0)
        {
            append(a_pInput->getLabelName());
            append(':');
        }
    }
}
void CppTranslator::visit(LanguageElement* a_pInput, VisitorData a_Data)
{
    PHANTOM_ASSERT(false, "missing translation for given element");
}
void CppTranslator::visit(LoadExpression* a_pInput, VisitorData a_Data)
{
    a_pInput->getLoadedExpression()->visit(this, a_Data);
}
void CppTranslator::visit(LocalVariable* a_pInput, VisitorData a_Data) {}
void CppTranslator::visit(LocalVariableExpression* a_pInput, VisitorData a_Data)
{
    if (auto l = a_pInput->getLocalVariable())
        append(l->getName());
    else
        append("this");
}
void CppTranslator::visit(LocalVariableInitializationStatement* a_pInput, VisitorData a_Data)
{
    auto transType = a_Data.id;
    a_Data.id = TranslationType::Definition;
    auto localVarType = a_pInput->getLocalVariable()->getValueType();

    if (auto initExp = a_pInput->getInitializationExpression())
    {
        auto noImplicitInitExp = initExp->removeImplicitIdentity();
        bool preferAuto = noImplicitInitExp == initExp && initExp->getValueType()->getTypeKind() != TypeKind::NullPtr;
        ConstructorCallExpression* pCtorCall = a_pInput->getInitializationExpression()->as<ConstructorCallExpression>();
        if (!pCtorCall)
        {
            if (CallExpression* pCall = a_pInput->getInitializationExpression()->as<CallExpression>())
            {
                preferAuto = !pCall->isImplicit();
            }
        }
        else
        {
            preferAuto = false;
        }
        auto autoType = Application::Get()->getAuto();
        if (localVarType->asPointer())
            autoType = autoType->addPointer();
        else if (localVarType->asLValueReference())
            autoType = autoType->addLValueReference();
        else if (localVarType->asRValueReference())
            autoType = autoType->addRValueReference();
        translateTyped(preferAuto ? autoType : localVarType, a_pInput->getLocalVariable()->getName());
        if (pCtorCall)
        {
            if (pCtorCall->getArguments().size() == 1)
            {
                if (auto rtocl = pCtorCall->getArguments().front()->as<RValueToConstLValueExpression>())
                {
                    if (pCtorCall->getArguments().front()->getValueType()->isSame(
                        a_pInput->getLocalVariable()->getValueType()->addConstLValueReference()))
                    {
                        if (auto convFuncExp = rtocl->getRValueExpression()->as<CallExpression>())
                        {
                            if (convFuncExp->getSubroutine()->getName().startsWith("operator "))
                            {
                                append("(");
                                convFuncExp->getArguments()
                                .front()
                                ->dereference(convFuncExp->getArguments().front()->getSource())
                                ->visit(this, a_Data);
                                append(");");
                                return;
                            }
                        }
                    }
                }
            }
            if (pCtorCall->getArguments().size())
            {
                append("(");
                for (auto it = pCtorCall->beginArguments(); it != pCtorCall->endArguments(); ++it)
                {
                    if (it != pCtorCall->beginArguments())
                        append(',');
                    (*it)->visit(this, a_Data);
                }
                append(")");
            }
        }
        else
        {
            append(" = ");
            a_pInput->getInitializationExpression()->visit(this, a_Data);
        }
    }
    else
    {
        translateTyped(localVarType, a_pInput->getLocalVariable()->getName());
    }
    if (transType != TranslationType::Name)
        append(";");
}
void CppTranslator::visit(LValueExpression* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void CppTranslator::visit(LValueInitializationStatement* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Statement*>(a_pInput), a_Data);
}
void CppTranslator::visit(LValueReference* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        a_pInput->getReferencedType()->visit(this, a_Data);
        append('&');
        return;
    }
    visit(static_cast<Reference*>(a_pInput), a_Data);
}
void CppTranslator::visit(MemberAnonymousSection* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(MemberAnonymousStruct* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration)
    {
        append("struct");
        newLine();
        indent();
        beginBlock();
        for (auto pDE : a_pInput->getDataElements())
        {
            pDE->asSymbol()->visit(this, a_Data);
        }
        endBlock();
    }
    else
        visit(static_cast<MemberAnonymousSection*>(a_pInput), a_Data);
}
void CppTranslator::visit(MemberAnonymousUnion* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration)
    {
        append("union");
        newLine();
        indent();
        beginBlock();
        for (auto pDE : a_pInput->getDataElements())
        {
            pDE->asSymbol()->visit(this, a_Data);
        }
        endBlock();
        endDecl();
    }
    else
        visit(static_cast<MemberAnonymousSection*>(a_pInput), a_Data);
}
void CppTranslator::visit(Method* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration)
    {
        _appendHauntMarkers(a_pInput);

        Methods funcs;
        a_pInput->getOriginalOverriddenMethods(funcs);
        bool overrides = !funcs.empty() && (funcs.size() > 1 || funcs.front() != a_pInput);
        if (!overrides && a_pInput->isVirtual())
            append("virtual ");
        if (a_pInput->getName().front() != '~' &&
            a_pInput->getName().find("operator ") != 0) // not a dtor, neither a conversion method
        {
            addForwardIfRequired(a_pInput->getReturnType());
            addIncludeIfRequired(a_pInput->getReturnType());
            translateTypeName(a_pInput->getReturnType());
            append(' ');
        }
        StringBuffer buf;
        a_pInput->getRelativeName(getContextScope(), buf);
        append(buf);
        _appendParameterList(a_pInput, a_Data);
        if (overrides)
            append(" override");
        if (a_pInput->isPureVirtual())
        {
            append(" = 0");
        }
        _appendFuncBlock(a_pInput, a_Data);
    }
    else
        visit(static_cast<Subroutine*>(a_pInput), a_Data);
}
void CppTranslator::visit(MethodPointerCallExpression* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Definition)
    {
        addInclude(a_pInput->getObjectExpression()->getValueType()->removeConstReference());
        append("((");
        a_pInput->getObjectExpression()->visit(this, a_Data);
        append(").*(");
        a_pInput->getMemberExpression()->visit(this, a_Data);
        append("))(");
        for (auto pArg : a_pInput->getArguments())
        {
            if (pArg != a_pInput->getArguments().front())
            {
                append(',');
            }
            pArg->visit(this, a_Data);
        }
        append(')');
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void CppTranslator::visit(MethodPointer* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        a_pInput->getFunctionType()->getReturnType()->visit(this, a_Data);
        append('(');
        a_pInput->getObjectType()->visit(this, a_Data);
        append("::*)");
        append("(");
        for (auto pType : a_pInput->getParameterTypes())
        {
            if (pType != a_pInput->getParameterTypes().front())
            {
                append(',');
            }
            pType->visit(this, a_Data);
        }
        append(')');
    }
    else if (a_Data.id == TranslationType::Typed)
    {
        a_pInput->getFunctionType()->getReturnType()->visit(this, a_Data);
        append('(');
        a_pInput->getObjectType()->visit(this, a_Data);
        append("::*");
        append(*(String*)a_Data.in[0]);
        append("(");
        for (auto pType : a_pInput->getParameterTypes())
        {
            if (pType != a_pInput->getParameterTypes().front())
            {
                append(',');
            }
            pType->visit(this, a_Data);
        }
        append(')');
    }
    visit(static_cast<MemberPointer*>(a_pInput), a_Data);
}
void CppTranslator::visit(MemberPointer* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void CppTranslator::visit(Module* a_pInput, VisitorData a_Data)
{
    PHANTOM_ASSERT(false);
}

void CppTranslator::visit(Symbol* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        if (auto type = a_pInput->asType())
        {
            auto typeNaked = type->removeEverything();
            auto typeNakedAlias = getAlias(typeNaked);

            if (typeNakedAlias)
            {
                if (auto asType = typeNakedAlias->asType())
                    return visit(type->replicate(asType), a_Data);

                if (auto asAlias = typeNakedAlias->asAlias())
                {
                    if (auto arg = asAlias->getAliasedSymbol()->asType())
                    {
                        Types replicationTypes;
                        auto  currTypeLvl = type;
                        while (currTypeLvl)
                        {
                            if (arg == currTypeLvl)
                            {
                                StringBuffer aliasedName;
                                asAlias->getRelativeName(getContextScope(), aliasedName);

                                size_t c = replicationTypes.size();
                                while (c--)
                                {
                                    currTypeLvl = replicationTypes[c];
                                    if (currTypeLvl->asConstType())
                                    {
                                        if (currTypeLvl->asVolatileType())
                                        {
                                            aliasedName += " const volatile";
                                        }
                                        else
                                        {
                                            aliasedName += " const";
                                        }
                                    }
                                    else if (currTypeLvl->asVolatileType())
                                    {
                                        if (currTypeLvl->asConstType())
                                        {
                                            aliasedName += " const volatile";
                                        }
                                        else
                                        {
                                            aliasedName += " volatile";
                                        }
                                    }
                                    else
                                    {
                                        switch (currTypeLvl->getTypeKind())
                                        {
                                        case TypeKind::Array:
                                            aliasedName += '[';
                                            aliasedName +=
                                            lexical_cast<String>(static_cast<Array*>(currTypeLvl)->getItemCount());
                                            aliasedName += ']';
                                            break;
                                        case TypeKind::Pointer:
                                            aliasedName += '*';
                                            break;
                                        case TypeKind::LValueReference:
                                            aliasedName += '&';
                                            break;
                                        case TypeKind::RValueReference:
                                            aliasedName += "&&";
                                            break;
                                        default:
                                            break;
                                        }
                                    }
                                }
                                append(aliasedName);
                                return;
                            }
                            replicationTypes.push_back(currTypeLvl);
                            currTypeLvl = currTypeLvl->getUnderlyingType();
                        }
                    }
                }
            }

            if (auto typeOwnr = typeNaked->getOwner())
                if (auto pTpl = typeOwnr->asTemplateSpecialization())
                {
                    TemplateSpecialization* replacingSpec = nullptr;
                    auto                    currentScope = getContextScope();
                    Template*               template_ = pTpl->getTemplate();
                    auto                    alias = getAlias(template_);
                    if (alias)
                    {
                        if (auto aliasTpl = alias->asTemplate())
                        {
                            template_ = aliasTpl;

                            for (auto aliasSpec : aliasTpl->getTemplateSpecializations())
                            {
                                if (aliasSpec->isFull())
                                {
                                    if (auto tpled = aliasSpec->getTemplated())
                                        if (auto tpledAsAlias = tpled->asAlias())
                                        {
                                            if (tpledAsAlias->getAliasedSymbol()->isSame(type))
                                            {
                                                replacingSpec = aliasSpec;
                                                break;
                                            }
                                        }
                                }
                            }
                        }
                    }

                    Symbol* nakedSym = alias;
                    if (!nakedSym)
                        nakedSym = typeNaked;

                    StringBuffer original;
                    if (m_ForceQualifiedName != 0)
                        original = type->getQualifiedDecoratedName();
                    else
                        type->getRelativeDecoratedName(currentScope, original);

                    StringView suffix = original;
                    size_t     p = suffix.find_last_of('>');
                    suffix = suffix.substr(p + 1);

                    StringBuffer relName;
                    if (m_ForceQualifiedName != 0)
                        nakedSym->getQualifiedName(relName);
                    else
                        nakedSym->getRelativeName(currentScope, relName);
                    append(relName);
                    append("<");

                    if (replacingSpec)
                        pTpl = replacingSpec;

                    size_t c = 0;
                    for (auto arg : pTpl->getArguments())
                    {
                        if ((replacingSpec || !alias) && pTpl->getDefaultArgument(c))
                            break;
                        if (c)
                            append(", ");
                        if (auto argType = arg->asType())
                        {
                            translate(argType, TranslationType::Name);
                        }
                        else
                        {
                            if (m_ForceQualifiedName != 0)
                                append(arg->getQualifiedDecoratedName());
                            else
                                append(arg->getRelativeDecoratedName(currentScope));
                        }
                        ++c;
                    }
                    append("> ");
                    append(suffix);
                    return;
                }
        }

        PHANTOM_ASSERT(getContextScope());
        if (m_ForceQualifiedName != 0)
            append(a_pInput->getQualifiedDecoratedName());
        else
            append(a_pInput->getRelativeDecoratedName(getContextScope()));
        if (a_pInput->getName() != "nullptr" && IsKeyword(a_pInput->getName()))
        {
            append(m_NoKeywordChars);
        }
        return;
    }
    visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}
void CppTranslator::visit(Namespace* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(NewExpression* a_pInput, VisitorData a_Data)
{
    ConstructorCallExpression* pExp = a_pInput->getInitializationExpression()->as<ConstructorCallExpression>();
    if (pExp)
    {
        append("new ");
        translateTypeName(pExp->getConstructor()->getOwner()->asClass());
        append('(');
        for (auto it = pExp->beginArguments(); it != pExp->endArguments(); ++it)
        {
            if (it != pExp->beginArguments())
                append(',');
            (*it)->visit(this, a_Data);
        }
        append(')');
    }
    else if (InitializerListExpression* pExp = a_pInput->getInitializationExpression()->as<InitializerListExpression>())
    {
        append("new ");
        translateTypeName(pExp->getValueType()->asClass());
        append('{');
        auto& exps = pExp->getExpressions();
        for (auto it = exps.begin(); it != exps.end(); ++it)
        {
            if (it != exps.begin())
                append(',');
            (*it)->visit(this, a_Data);
        }
        append('}');
    }
    else
    {
        TemplateDependantExpression* pExpT = a_pInput->getInitializationExpression()->as<TemplateDependantExpression>();
        if (pExpT)
        {
            TemplateDependantElement* pElem = static_cast<TemplateDependantElement*>(pExpT->getOwner());
            append("new ");
            pElem->getLeft()->asSymbol()->getQualifiedDecoratedName();
            append('(');
            for (auto it = pElem->getFunctionArguments()->begin(); it != pElem->getFunctionArguments()->end(); ++it)
            {
                if (it != pElem->getFunctionArguments()->begin())
                    append(',');
                (*it)->visit(this, a_Data);
            }
            append(')');
        }
    }
}
void CppTranslator::visit(Package* a_pInput, VisitorData a_Data)
{
    PHANTOM_ASSERT(false);
}
void CppTranslator::visit(PackageFolder* a_pInput, VisitorData a_Data)
{
    PHANTOM_ASSERT(false);
}
void CppTranslator::visit(Parameter* a_pInput, VisitorData a_Data)
{
    addForwardIfRequired(a_pInput->getValueType());
    addIncludeIfRequired(a_pInput->getValueType());

    if (a_Data.id == TranslationType::Declaration || a_Data.id == TranslationType::Definition)
    {
        translateTyped(a_pInput->getValueType(), a_pInput->getName());
        if (a_pInput->getDefaultArgumentExpression())
        {
            if (a_Data.id == TranslationType::Definition)
            {
                append(" = /*");
                translate(a_pInput->getDefaultArgumentExpression());
                append("*/");
            }
            else
            {
                append(" = ");
                translate(a_pInput->getDefaultArgumentExpression());
            }
        }
    }
    else
    {
        translateTypeName(a_pInput->getValueType());
    }
}
void CppTranslator::visit(Placeholder* a_pInput, VisitorData a_Data) {}
void CppTranslator::visit(PlaceholderClass* a_pInput, VisitorData a_Data)
{
    a_pInput->getOwner()->visit(this, a_Data);
}
void CppTranslator::visit(PlaceholderClassType* a_pInput, VisitorData a_Data)
{
    a_pInput->getOwner()->visit(this, a_Data);
}
void CppTranslator::visit(PlaceholderConstant* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration)
    {
        translateTypeName(a_pInput->getValueType());
        append(' ');
    }
    append(a_pInput->getName());
}
void CppTranslator::visit(PlaceholderTemplate* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration)
    {
        a_pInput->getTemplateSignature()->visit(this, a_Data);
        append(" class ");
    }
    append(a_pInput->getName());
}
void CppTranslator::visit(PlaceholderType* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration)
    {
        append("class ");
        append(a_pInput->getName());
    }
    else if (a_Data.id == TranslationType::Name)
        append(a_pInput->getName());
    else
        visit(static_cast<Type*>(a_pInput), a_Data);
}
void CppTranslator::visit(PlacementNewExpression* a_pInput, VisitorData a_Data)
{
    addInclude(a_pInput->getConstructionExpression()->getEvalType());
    append("new (");
    a_pInput->getPlaceExpression()->visit(this, a_Data);
    append(')');
    a_pInput->getConstructionExpression()->visit(this, a_Data);
}
void CppTranslator::visit(PrimitiveType* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        append(a_pInput->getName());
    }
    else
        visit(static_cast<Type*>(a_pInput), a_Data);
}
void CppTranslator::visit(Property* a_pInput, VisitorData a_Data)
{
    //     if (a_Data.id == TranslationType::Declaration)
    //     {
    //         beginDecl();
    //         append("/// @property @mask 0x" + lexical_cast<String>((void*)a_pInput->getFilterMask()));
    //         endDecl();
    //     }

    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(PropertyExpression* a_pInput, VisitorData a_Data)
{
    addInclude(a_pInput->getObjectExpression()->getValueType()->removeConstReference());
    append("(");
    a_pInput->getObjectExpression()->visit(this, a_Data);
    append(").");
    append(a_pInput->getProperty()->getGet()->getName());
    append("()");
}
void CppTranslator::visit(Reference* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ExtendedType*>(a_pInput), a_Data);
}
void CppTranslator::visit(ReturnStatement* a_pInput, VisitorData a_Data)
{
    append("return");
    if (a_pInput->getExpression())
    {
        append(' ');
        a_pInput->getExpression()->visit(this, a_Data);
    }
    append(';');
}
void CppTranslator::visit(RValueReference* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        a_pInput->getReferencedType()->visit(this, a_Data);
        append("&&");
        return;
    }
    visit(static_cast<Reference*>(a_pInput), a_Data);
}
void CppTranslator::visit(RValueToConstLValueExpression* a_pInput, VisitorData a_Data)
{
    a_pInput->getRValueExpression()->visit(this, a_Data);
}
void CppTranslator::visit(RValueReferenceExpression* a_pInput, VisitorData a_Data)
{
    a_pInput->getRValueExpression()->visit(this, a_Data);
}
void CppTranslator::visit(Scope* a_pInput, VisitorData a_Data) {}
void CppTranslator::visit(Signature* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        translate(a_pInput->getReturnType());
    }
    append("(");
    int c = 0;
    for (auto p : a_pInput->getParameters())
    {
        if (c++)
            append(", ");
        p->visit(this, a_Data);
    }
    append(")");
}

void CppTranslator::visit(PointerAdjustmentExpression* a_pInput, VisitorData a_Data)
{
    if (a_pInput->isImplicit())
    {
        addIncludeIfRequired(a_pInput->getPointerExpression()->getValueType()->removeAddress()->removeQualifiers());
        return a_pInput->getPointerExpression()->visit(this, a_Data);
    }
    addIncludeIfRequired(a_pInput->getValueType()->removeAddress()->removeQualifiers());
    append("static_cast<");
    translateTypeName(a_pInput->getValueType());
    append(">(");
    a_pInput->getPointerExpression()->visit(this, a_Data);
    append(')');
}

void CppTranslator::appendIncludes(Source* a_pInput, Sources const& a_IncludedSources)
{
    StringViews projectPaths;
    String      projectPathsStr = getOption("project-paths");
    StringUtil::Split(projectPaths, projectPathsStr, ";");

    for (auto& pIncSource : a_IncludedSources)
    {
        if (pIncSource == a_pInput)
            continue;

        phantom::Path sourcePath = pIncSource->getSourceStream()->getPath();

        bool projectFound = false;
        for (auto& pp : projectPaths)
        {
            phantom::Path rootPath = pp;
            if (rootPath.hasChildPath(sourcePath))
            {
                append("#include <" + sourcePath.relative(rootPath).removeExtension().genericString() + ".h>\n");
                projectFound = true;
                break;
            }
        }

        if (!projectFound && pIncSource->isNative())
        {
            phantom::Path moduleDeclCppPath = pIncSource->getModule()->getDeclarationCppFullName();
            auto          rootPath = moduleDeclCppPath.parentPath();
            if (moduleDeclCppPath.parentPath().hasChildPath(sourcePath))
            {
                append("#include <" + sourcePath.relative(rootPath).removeExtension().genericString() + ".h>\n");
                projectFound = true;
                break;
            }
        }
    }
}

void CppTranslator::visit(Source* a_pInput, VisitorData a_Data)
{
    Namespace* pNamespace = a_pInput->getPackage()->getCounterpartNamespace();
    pushContextScope(pNamespace);
    beginDecl();
    appendNamespaceOpening(pNamespace);
    endDecl();
    beginDecl();
    append("PHANTOM_FORWARD_DECL_NAMESPACE;");
    endDecl();

    std::map<LanguageElement*, int> classPrios;

    SmallMap<int, LanguageElements> sortedElements;

    for (auto pElem : a_pInput->getElements())
    {
        if (pElem->testFlags(PHANTOM_R_FLAG_IMPLICIT))
            continue;
        if (auto pSpec = pElem->asTemplateSpecialization())
        {
            if (auto c = pSpec->getTemplated()->asClassType())
                sortedElements[priorityComputer(a_pInput, c, classPrios) + 1].push_back(pElem);
            else
                sortedElements[2].push_back(pElem);
        }
        else if (auto pAlias = pElem->asAlias())
        {
            sortedElements[2].push_back(pElem);
        }
        else if (auto pType = pElem->asType())
        {
            if (pType->asEnum())
                sortedElements[1].push_back(pElem);
            else if (auto pClass = pType->asClassType())
                sortedElements[priorityComputer(a_pInput, pClass, classPrios) + 1].push_back(pElem);
            else
                sortedElements[1].push_back(pElem);
        }
        else if (auto pFunc = pElem->asFunction())
        {
            sortedElements[2].push_back(pElem);
        }
    }

    for (auto& prio_elements : sortedElements)
        for (auto pElem : prio_elements.second)
        {
            if (auto pSpec = pElem->asTemplateSpecialization())
            {
                beginDecl();
                appendNamespaceClosing(pNamespace);
                endDecl();

                if (auto pNS = pSpec->getTemplate()->getNamespace())
                {
                    pushContextScope(pNS);
                    beginDecl();
                    appendNamespaceOpening(pNS);
                    endDecl();

                    beginDecl();
                    pSpec->visit(this, a_Data);
                    endDecl();

                    beginDecl();
                    appendNamespaceClosing(pNS);
                    endDecl();
                    popContextScope();
                }

                beginDecl();
                appendNamespaceOpening(pNamespace);
                endDecl();
            }
            else if (auto pAlias = pElem->asAlias())
            {
                beginDecl();
                pAlias->visit(this, a_Data);
                endDecl();
            }

            else if (auto pType = pElem->asType())
            {
                if (pType->asClassType() || pType->asEnum())
                {
                    beginDecl();
                    pType->visit(this, a_Data);
                    endDecl();
                }
            }
            else if (auto pFunc = pElem->asFunction())
            {
                beginDecl();
                pFunc->visit(this, a_Data);
                endDecl();
            }

            newLine();
        }

    beginDecl();
    appendNamespaceClosing(pNamespace);
    endDecl();

    popContextScope();
}
void CppTranslator::visit(Statement* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Evaluable*>(a_pInput), a_Data);
}
void CppTranslator::visit(StaticAssert* a_pInput, VisitorData a_Data)
{
    append("static_assert((");
    a_pInput->getExpression()->visit(this, a_Data);
    append("), ");
    append(a_pInput->getText());
    append(");");
}
void CppTranslator::visit(StringLiteralExpression* a_pInput, VisitorData a_Data)
{
    append(a_pInput->getLiteral());
}
void CppTranslator::visit(Structure* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration)
    {
        pushContextScope(a_pInput);
        if (a_pInput->getOwner()->asSource())
        {
            String exp = getOption("export-macro");
            append("struct " + (exp.size() ? (exp + ' ') : "") + a_pInput->getName());
        }
        else
        {
            append("struct " + a_pInput->getName());
        }
        newLine();
        indent();
        beginBlock();
        visit(static_cast<ClassType*>(a_pInput), a_Data);
        endBlock("};");
        popContextScope();
    }
    visit(static_cast<ClassType*>(a_pInput), a_Data);
}
void CppTranslator::visit(StructureExpression* a_pInput, VisitorData a_Data)
{
    append("{ ");
    for (auto pExp : a_pInput->getValues())
    {
        if (pExp != a_pInput->getValues()[0])
            append(", ");
        pExp->visit(this, a_Data);
    }
    append(" }");
}

void CppTranslator::_appendParameterList(Subroutine* a_pInput, VisitorData a_Data)
{
    append("(");
    int c = 0;
    for (auto pParam : a_pInput->getSignature()->getParameters())
    {
        if (c++)
            append(", ");
        addForwardIfRequired(pParam->getValueType());
        addIncludeIfRequired(pParam->getValueType());
        translateTyped(pParam->getValueType(), pParam->getName());
        if (auto pExp = pParam->getDefaultArgumentExpression())
        {
            if (a_Data.id == TranslationType::Definition)
                append("/*");
            append(" = ");
            pExp->visit(this, a_Data);
            if (a_Data.id == TranslationType::Definition)
                append("*/");
        }
    }
    append(")");
    if (a_pInput->testModifiers(Modifier::Defaulted))
        append(" = default");
    else if (a_pInput->testModifiers(Modifier::Deleted))
        append(" = delete");
    appendSignatureQuals(a_pInput->getSignature());
}

void CppTranslator::_appendFuncBlock(Subroutine* a_pInput, VisitorData a_Data)
{
    if ((a_pInput->isTemplateElement() || a_Data.id == TranslationType::Definition) && a_pInput->getBlock() &&
        !a_pInput->testModifiers(Modifier::Defaulted) &&
        !a_pInput->testModifiers(Modifier::Deleted)) // template or inline
    {
        newLine();
        indent();
        a_pInput->getBlock()->visit(this, a_Data);
    }
    else
    {
        append(";");
    }
}

void CppTranslator::visit(Subroutine* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration)
    {
        _appendHauntMarkers(a_pInput);
        if (a_pInput->getName().front() != '~' &&
            a_pInput->getName().find("operator ") != 0) // not a dtor, neither a conversion method
        {
            addForwardIfRequired(a_pInput->getReturnType());
            addIncludeIfRequired(a_pInput->getReturnType());
            translateTypeName(a_pInput->getReturnType());
            append(' ');
        }
        StringBuffer buf;
        a_pInput->getRelativeName(getContextScope(), buf);
        append(buf);
        _appendParameterList(a_pInput, a_Data);
        _appendFuncBlock(a_pInput, a_Data);
    }
    else if (a_Data.id == TranslationType::Definition)
    {
        if (a_pInput->getBlock())
        {
            if (a_pInput->getName().front() != '~' &&
                a_pInput->getName().find("operator ") != 0) // not a dtor, neither a conversion method
            {
                translateTypeName(a_pInput->getReturnType());
                append(' ');
            }
            StringBuffer buf;
            a_pInput->getRelativeName(getContextScope(), buf);
            append(buf);
            _appendParameterList(a_pInput, a_Data);
            _appendFuncBlock(a_pInput, a_Data);
        }
    }
    else
        visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(BinaryOperationExpression* a_pInput, VisitorData a_Data)
{
    auto op = CppLite::Get()->getBuiltInBinaryOperatorId(a_pInput->getOperatorName());
    bool paren = requiresParenthesis(op);
    beginParen(paren);
    m_OperatorStack.emplace_back(op, true);
    a_pInput->getLeftOperandExpression()->visit(this, a_Data);
    m_OperatorStack.pop_back();
    append(a_pInput->getOperatorName());
    m_OperatorStack.emplace_back(op, false);
    a_pInput->getRightOperandExpression()->visit(this, a_Data);
    m_OperatorStack.pop_back();
    endParen(paren);
}
void CppTranslator::visit(UnaryPostOperationExpression* a_pInput, VisitorData a_Data)
{
    auto op = CppLite::Get()->getBuiltInPostUnaryOperatorId(a_pInput->getOperatorName());
    bool paren = requiresParenthesis(op);
    beginParen(paren);
    m_OperatorStack.emplace_back(op, true);
    a_pInput->getOperandExpression()->visit(this, a_Data);
    m_OperatorStack.pop_back();
    append(a_pInput->getOperatorName());
    endParen(paren);
}
void CppTranslator::visit(UnaryPreOperationExpression* a_pInput, VisitorData a_Data)
{
    if (auto upoe = Object::Cast<UnaryPostOperationExpression>(a_pInput->getOperandExpression()))
    {
        if (upoe->getOperatorName() == "->")
        {
            return a_pInput->getOperandExpression()->visit(this, a_Data);
        }
    }
    auto op = CppLite::Get()->getBuiltInPreUnaryOperatorId(a_pInput->getOperatorName());
    bool paren = requiresParenthesis(op);
    beginParen(paren);
    append(a_pInput->getOperatorName());
    m_OperatorStack.emplace_back(op, false);
    a_pInput->getOperandExpression()->visit(this, a_Data);
    m_OperatorStack.pop_back();
    endParen(paren);
}
void CppTranslator::visit(Template* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(TemplateDependantArray* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Array*>(a_pInput), a_Data);
}
void CppTranslator::visit(TemplateDependantDeclType* a_pInput, VisitorData a_Data)
{
    append("decltype(");
    a_pInput->getExpression()->visit(this, a_Data);
    append(")");
}
void CppTranslator::visit(TemplateDependantElement* a_pInput, VisitorData a_Data)
{
    bool paren = a_pInput->getLeft() && requiresParenthesis(Operator::Arrow);
    beginParen(paren);
    if (LanguageElement* l = a_pInput->getLeft())
    {
        if (auto e = Object::Cast<Expression>(l))
        {
            UnaryPostOperationExpression* unpost = nullptr;
            auto                          unpre = Object::Cast<UnaryPreOperationExpression>(e);
            auto                          biop = Object::Cast<BuiltInOperatorExpression>(e);
            if (unpre && unpre->getOperatorName() == "*" &&
                (unpost = Object::Cast<UnaryPostOperationExpression>(unpre->getOperandExpression())) &&
                unpost->getOperatorName() == "->")
            {
                m_OperatorStack.emplace_back(Operator::Arrow, true);
                l->visit(this, a_Data);
                m_OperatorStack.pop_back();
            }
            else
            {
                m_OperatorStack.emplace_back(Operator::Arrow, true);
                l->visit(this, a_Data);
                m_OperatorStack.pop_back();
                append(".");
            }
        }
        else
        {
            m_OperatorStack.emplace_back(Operator::COUNT, true);
            l->visit(this, a_Data);
            m_OperatorStack.pop_back();
            append("::");
        }
    }
    append(a_pInput->getName());
    if (a_pInput->getTemplateArguments())
    {
        int c = 0;
        append("<");
        for (auto pArg : *a_pInput->getTemplateArguments())
        {
            if (c++)
                append(", ");
            pArg->visit(this, a_Data);
        }
        append(">");
    }
    if (a_pInput->getFunctionArguments())
    {
        append("(");
        int c = 0;
        for (auto pArg : *a_pInput->getFunctionArguments())
        {
            if (c++)
                append(", ");
            pArg->visit(this, a_Data);
        }
        append(")");
    }
    endParen(paren);
}
void CppTranslator::visit(TemplateDependantExpression* a_pInput, VisitorData a_Data)
{
    a_pInput->getTemplateDependantElement()->visit(this, a_Data);
}
void CppTranslator::visit(TemplateDependantTemplateInstance* a_pInput, VisitorData a_Data)
{
    if (LanguageElement* l = a_pInput->getTemplate())
    {
        l->visit(this, a_Data);
    }
    append("<");
    int c = 0;
    for (auto pArg : a_pInput->getArguments())
    {
        if (c++)
            append(", ");
        pArg->visit(this, a_Data);
    }
    append(">");
}
void CppTranslator::visit(TemplateDependantTypeOfExpression* a_pInput, VisitorData a_Data)
{
    if (auto c = a_pInput->getValueType()->removePointer() == PHANTOM_CLASSOF(Class))
    {
        addInclude(PHANTOM_TYPEOF(Class));
        append("PHANTOM_CLASSOF(");
        translateTypeName(a_pInput->getType());
        append(")");
    }
    else
    {
        addInclude(PHANTOM_TYPEOF(Type));
        append("PHANTOM_TYPEOF(");
        translateTypeName(a_pInput->getType());
        append(")");
    }
}
void CppTranslator::visit(TemplateDependantType* a_pInput, VisitorData a_Data)
{
    a_pInput->getTemplateDependantElement()->visit(this, a_Data);
}
void CppTranslator::visit(TemplateParameter* a_pInput, VisitorData a_Data)
{
    a_pInput->getPlaceholder()->asSymbol()->visit(this, a_Data);
}
void CppTranslator::visit(TemplateSignature* a_pInput, VisitorData a_Data)
{
    append("template<");
    auto& tps = a_pInput->getTemplateParameters();
    for (auto it = tps.begin(); it != tps.end(); ++it)
    {
        if (it != tps.begin())
            append(',');
        (*it)->visit(this, a_Data);
    }
    append('>');
}
void CppTranslator::visit(TemplateSpecialization* a_pInput, VisitorData a_Data)
{
    if (a_pInput->getTemplated())
    {
        if (a_Data.id == TranslationType::Declaration)
        {
            a_pInput->getTemplateSignature()->visit(this, a_Data);
            newLine();
            indent();
        }
        a_pInput->getTemplated()->visit(this, a_Data);
        return;
    }
}
void CppTranslator::visit(Type* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Typed)
    {
        auto& identifier = *(const String*)(a_Data.in[0]);
        if (a_pInput->getName() == identifier)
        {
            m_ForceQualifiedName++;
            translateTypeName(a_pInput);
            m_ForceQualifiedName--;
        }
        else
            translateTypeName(a_pInput);
        append(' ');
        append(*(const String*)(a_Data.in[0]));
    }
    else
        visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(Union* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Declaration)
    {
        append("union ");
        a_pInput->getName();
        newLine();
        indent();
        beginBlock();
        visit(static_cast<ClassType*>(a_pInput), a_Data);
        endBlock("};");
    }
}
void CppTranslator::visit(ValueMember* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(Variable* a_pInput, VisitorData a_Data)
{
    if (a_pInput->getValueType()->asClassType() || a_pInput->getValueType()->asEnum())
    {
        addInclude(a_pInput->getValueType());
    }
    if (a_Data.id == TranslationType::Declaration)
    {
        beginDecl();
        append("static ");
        translateTyped(a_pInput->getValueType(), a_pInput->getName());
        append(';');
        endDecl();
    }
    else if (a_Data.id == TranslationType::Definition)
    {
        if (a_pInput->getOwner()->asClassType())
        {
            /// Static data member
            beginDecl();
            String* pushed = m_pTranslation;
            String  temp;
            m_pTranslation = &temp;
            translate(a_pInput);
            m_pTranslation = pushed;
            translateTyped(a_pInput->getValueType(), temp);
            append(';');
            endDecl();
        }
        else
        {
            /// Global variable
        }
    }
    else
        visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void CppTranslator::visit(VariableExpression* a_pInput, VisitorData a_Data)
{
    addInclude(a_pInput->getVariable());
    translate(a_pInput->getVariable());
}
void CppTranslator::visit(VirtualMethodTable* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}

void CppTranslator::visit(VolatileType* a_pInput, VisitorData a_Data)
{
    if (a_Data.id == TranslationType::Name)
    {
        a_pInput->getVolatiledType()->visit(this, a_Data);
        append(" volatile");
    }
}
//
// void CppTranslator::visit(FundamentalTypeT<void>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("void");
// }
// void CppTranslator::visit(FundamentalTypeT<std::nullptr_t>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("std::nullptr_t");
// }
// void CppTranslator::visit(IntegralTypeT<bool>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("bool");
// }
// void CppTranslator::visit(IntegralTypeT<char>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("char");
// }
// void CppTranslator::visit(IntegralTypeT<uchar>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("unsigned char");
// }
// void CppTranslator::visit(IntegralTypeT<schar>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("signed char");
// }
// void CppTranslator::visit(IntegralTypeT<short>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("short");
// }
// void CppTranslator::visit(IntegralTypeT<ushort>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("unsigned short");
// }
// void CppTranslator::visit(IntegralTypeT<int>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("int");
// }
// void CppTranslator::visit(IntegralTypeT<uint>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("unsigned int");
// }
// void CppTranslator::visit(IntegralTypeT<long>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("long");
// }
// void CppTranslator::visit(IntegralTypeT<ulong>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("unsigned long");
// }
// void CppTranslator::visit(IntegralTypeT<longlong>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("long long");
// }
// void CppTranslator::visit(IntegralTypeT<ulonglong>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("unsigned long long");
// }
// void CppTranslator::visit(FloatingPointTypeT<float>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("float");
// }
// void CppTranslator::visit(FloatingPointTypeT<double>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("double");
// }
// void CppTranslator::visit(FloatingPointTypeT<longdouble>* a_pInput, VisitorData a_Data)
// {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("long double");
// }
//
// PHANTOM_IF_WCHAR_T(void CppTranslator::visit(IntegralTypeT<wchar_t>* a_pInput, VisitorData a_Data) {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("wchar_t");
// });
// PHANTOM_IF_CHAR16_T(void CppTranslator::visit(IntegralTypeT<char16_t>* a_pInput, VisitorData a_Data) {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("char16_t");
// });
// PHANTOM_IF_CHAR32_T(void CppTranslator::visit(IntegralTypeT<char32_t>* a_pInput, VisitorData a_Data) {
//     if (a_Data.id == TranslationType::Name || a_Data.id == TranslationType::Typed)
//         append("char32_t");
// });

void CppTranslator::translate(LanguageElement* a_pInput, int a_TranslationType)
{
    if (auto sym = a_pInput->asSymbol())
    {
        auto alias = getAlias(sym);
        if (alias)
            return translate(alias, a_TranslationType);
    }
    VisitorData data;
    data.id = a_TranslationType;
    a_pInput->visit(this, data);
}

void CppTranslator::translateTyped(Type* a_pInput, const String& a_Identifier)
{
    VisitorData data;
    data.id = TranslationType::Typed;
    void const* in[1] = {&a_Identifier};
    data.in = in;
    a_pInput->visit(this, data);
}

Alias* CppTranslator::_findAliasOf(Module* a_pModule, Type* a_pType)
{
    Sources sources;
    a_pModule->getSources(sources);
    for (auto source : sources)
    {
        for (auto al : source->getAliases())
        {
            if (al->getAliasedSymbol() == a_pType)
            {
                StringView alias = al->getName();
                if (alias.size())
                {
                    return al;
                }
            }
        }
    }
    for (auto dep : a_pModule->getDependencies())
    {
        if (auto al = _findAliasOf(dep, a_pType))
            return al;
    }
    return nullptr;
}

phantom::lang::Alias* CppTranslator::_findAliasOf(Type* a_pType)
{
    auto found = m_AliasCache.find(a_pType);
    if (found == m_AliasCache.end())
    {
        return m_AliasCache[a_pType] = _findAliasOf(m_pSource->getModule(), a_pType);
    }
    return found->second;
}

void CppTranslator::translateTypeName(Type* a_pType)
{
    if (!m_noAlias)
    {
        if (a_pType->getTemplateSpecialization())
        {
            if (auto alias = _findAliasOf(a_pType))
            {
                return translate(alias, TranslationType::Name);
            }
        }
    }
    translate(a_pType, TranslationType::Name);
}

void CppTranslator::_addInclude(SmallMap<String, Source*>& a_Includes, Source* a_pSource)
{
    if (a_pSource != m_pSource)
        a_Includes[a_pSource->getUniqueName()] = a_pSource;
}

void CppTranslator::addInclude(SmallMap<String, Source*>& a_Includes, Symbol* a_pSymbol)
{
    if (auto pTCP = Object::Cast<TemplateDependantClassPromotion>(a_pSymbol))
    {
        addInclude(a_Includes, pTCP->getBase());
    }
    else if (auto pTDTI = Object::Cast<TemplateDependantTemplateInstance>(a_pSymbol))
    {
        _addInclude(a_Includes, pTDTI->getTemplate()->getSource());
    }
    else if (auto pSpec = a_pSymbol->getTemplateSpecialization())
    {
        if (auto type = a_pSymbol->asType())
        {
            if (auto alias = _findAliasOf(type))
            {
                return addInclude(a_Includes, alias);
            }
        }

        if (!pSpec->isNative() && pSpec->getTemplate()->isNative()) // extended
        {
            _addInclude(a_Includes, pSpec->getTemplate()->getSource());
        }
        else if (auto pInst = pSpec->getInstantiationSpecialization())
        {
            _addInclude(a_Includes, pInst->getSource());
        }
        else if (pSpec->getSource()->getName() == "default")
            _addInclude(a_Includes, pSpec->getTemplate()->getSource());
        else
            _addInclude(a_Includes, pSpec->getSource());

        size_t argIdx = 0;
        for (auto* pArg : pSpec->getArguments())
        {
            if (auto pType = pArg->asType())
            {
                pType = pType->removeQualifiers();
                addForwardIfRequired(pType);
                if (pType->asClassType() || pType->asEnum())
                {
                    if (m_ForceIncludeTemplateArguments[argIdx].find(a_pSymbol->getQualifiedName()) !=
                        m_ForceIncludeTemplateArguments[argIdx].end())
                    {
                        addInclude(pType);
                    }
                }
            }
            argIdx++;
        }
    }
    else
    {
        _addInclude(a_Includes, a_pSymbol->getSource());
    }
}

void CppTranslator::addInclude(Symbol* a_pSymbol)
{
    if (m_uiFile == TranslationType::Declaration)
    {
        addInclude(m_IncludesH, a_pSymbol);
    }
    else if (m_uiFile == TranslationType::Definition)
    {
        addInclude(m_IncludesCPP, a_pSymbol);
    }
}

void CppTranslator::addForwardH(Type* a_pType)
{
    if (a_pType->getTemplateSpecialization())
    {
        if (auto alias = _findAliasOf(a_pType))
        {
            return addInclude(m_IncludesH, alias);
        }
    }
    PHANTOM_ASSERT(a_pType->asEnum() == nullptr);
    m_ForwardsH[a_pType->getQualifiedName()] = a_pType;
}

void CppTranslator::addForwardCPP(Type* a_pType)
{
    if (a_pType->getTemplateSpecialization())
    {
        if (auto alias = _findAliasOf(a_pType))
        {
            return addInclude(m_IncludesCPP, alias);
        }
    }
    PHANTOM_ASSERT(a_pType->asEnum() == nullptr);
    m_ForwardsCPP[a_pType->getQualifiedName()] = a_pType;
}

void CppTranslator::addForward(Type* a_pType)
{
    a_pType = a_pType->removeQualifiers();
    if (a_pType->getOwner() && a_pType->getOwner()->asClassType())
    {
        addInclude(static_cast<Symbol*>(a_pType->getOwner()));
    }
    else
    {
        if (auto pSpec = a_pType->getTemplateSpecialization())
        {
            for (auto pArg : pSpec->getArguments())
            {
                if (auto pType = pArg->asType())
                {
                    if (pType->asClassType())
                        addForward(pType);
                    else
                        addForwardIfRequired(pType);
                }
            }
        }
        if (m_uiFile == TranslationType::Declaration)
        {
            addForwardH(a_pType);
        }
        else if (m_uiFile == TranslationType::Definition)
        {
            addForwardCPP(a_pType);
        }
    }
}

void CppTranslator::appendNamespaceList(Namespace* a_pNamespace)
{
    String list;
    while (a_pNamespace && a_pNamespace->getName().size())
    {
        list = list.empty() ? a_pNamespace->getName() : (a_pNamespace->getName() + ',' + list);
        a_pNamespace = a_pNamespace->getParentNamespace();
    }
    append(list);
}

void CppTranslator::appendNamespaceScope(Namespace* a_pNamespace)
{
    String list;
    while (a_pNamespace && a_pNamespace->getName().size())
    {
        list = list.empty() ? a_pNamespace->getName() : (String(a_pNamespace->getName()) + "::" + list);
        a_pNamespace = a_pNamespace->getParentNamespace();
    }
    append(list);
}

void CppTranslator::appendNamespaceOpening(Namespace* a_pNamespace)
{
    String list;
    while (a_pNamespace && a_pNamespace->getName().size())
    {
        list = "namespace " + a_pNamespace->getName() + " {\n" + list;
        a_pNamespace = a_pNamespace->getParentNamespace();
    }
    append(list);
}

void CppTranslator::translateSymbol(Symbol* a_pSymbol, String& a_strSymbol, LanguageElement* a_pContextScope)
{
    VisitorData data;
    data.id = TranslationType::Name;
    m_pTranslation = &a_strSymbol;
    m_IndentationLevel = 0;
    if (a_pContextScope == nullptr)
        a_pContextScope = Namespace::Global();
    pushContextScope(a_pContextScope);
    a_pSymbol->visit(this, data);
    popContextScope();
    m_pTranslation = nullptr;
}

void CppTranslator::translateExpression(Expression* a_pExpression, String& a_strExpresson,
                                        LanguageElement* a_pContextScope)
{
    // setFlags((EFlags)(e_Flag_NoSourceMode | getFlags()));
    VisitorData data;
    data.id = TranslationType::Definition;
    m_pTranslation = &a_strExpresson;
    m_IndentationLevel = 0;
    if (a_pContextScope == nullptr)
        a_pContextScope = Namespace::Global();
    pushContextScope(a_pContextScope);
    a_pExpression->visit(this, data);
    popContextScope();
    m_pTranslation = nullptr;
}

void CppTranslator::appendExpression(Expression* a_pExpression, Expression*, VisitorData a_Data)
{
    //     Expression* noIdentity = a_pExpression->removeImplicitIdentity();
    //
    //     CallExpression* callExp = nullptr;
    //     if (noIdentity->as<LValueExpression>() || noIdentity->as<ConstantExpression>() ||
    //         (callExp = noIdentity->as<CallExpression>()) || noIdentity->as<ConstructorCallExpression>())
    //     {
    //         if (callExp && callExp->getSubroutine()->getName().find("operator") == 0)
    //         {
    //             if (callExp->getArguments().size() >= 2)
    //             {
    //                 append('(');
    //                 a_pExpression->visit(this, a_Data);
    //                 append(")");
    //                 return;
    //             }
    //         }
    //         a_pExpression->visit(this, a_Data);
    //     }
    //     else
    //     {
    //         append('(');
    a_pExpression->visit(this, a_Data);
    //         append(")");
    //     }
}

bool CppTranslator::requiresParenthesis(Operation _embedding, Operator _sub) const
{
    auto& l = precedence[int(_embedding.op)];
    auto& r = precedence[int(_sub)];
    if (l.precedence < r.precedence)
    {
        return true;
    }
    else if (l.precedence > r.precedence)
        return false;
    else if (_embedding.lhs)
        return !l.leftToRight;
    else
        return l.leftToRight;
}

void CppTranslator::addForwardIfRequired(Type* a_pType)
{
    a_pType = a_pType->removeQualifiers();
    if (auto pSpec = a_pType->getTemplateSpecialization())
    {
        addForward(a_pType);
        for (auto pArg : pSpec->getArguments())
        {
            if (auto pType = pArg->asType())
            {
                addForwardIfRequired(pType);
            }
        }
    }
    if (auto pFT = a_pType->asFunctionType())
    {
        addForwardIfRequired(pFT->getReturnType());
        for (auto pPT : pFT->getParameterTypes())
        {
            addForwardIfRequired(pPT);
        }
    }
    else
        while (a_pType->asAddressType() || a_pType->asArray())
        {
            Type* pUT = a_pType->getUnderlyingType()->removeQualifiers();
            if (pUT->asClassType())
            {
                addForward(pUT);
                break;
            }
            a_pType = pUT;
        }
}

void CppTranslator::appendNamespaceClosing(Namespace* a_pNamespace)
{
    String list;
    while (a_pNamespace && a_pNamespace->getName().size())
    {
        list += "} // namespace " + a_pNamespace->getName() + "\n";
        a_pNamespace = a_pNamespace->getParentNamespace();
    }
    append(list);
}

void CppTranslator::appendNestingClassList(ClassType* a_pClassType)
{
    String list;
    while (a_pClassType)
    {
        list = list.empty() ? a_pClassType->getName() : list + ',' + a_pClassType->getName();
        a_pClassType = a_pClassType->getOwner() ? a_pClassType->getOwner()->asClassType() : nullptr;
    }
    append(list);
}

void CppTranslator::_formatTemplateSign(LanguageElement* a_pScope, TemplateSignature* a_pSign, String& a_Str)
{
    a_Str += "template<";
    int c = 0;
    for (auto pTP : a_pSign->getTemplateParameters())
    {
        if (c++)
            a_Str += ",";
        if (pTP->getPlaceholder()->asPlaceholderType())
        {
            a_Str += "class";
        }
        else if (auto pCst = pTP->getPlaceholder()->asPlaceholderConstant())
        {
            StringBuffer buf;
            pCst->getValueType()->getRelativeDecoratedName(a_pScope, buf);
            a_Str += StringView(buf);
        }
    }
    a_Str += "> ";
}

void CppTranslator::_formatForwards(SmallMap<String, Type*>& a_Forwards, String& a_Str)
{
    struct NamespaceSorter
    {
        bool operator()(Namespace* ns0, Namespace* ns1) const
        {
            return ns0->getQualifiedName() < ns1->getQualifiedName();
        }
    };
    SmallMap<Namespace*, Types, 16, NamespaceSorter> perNS;
    for (auto type : a_Forwards)
    {
        if (type.second->getNamespace())
        {
            perNS[type.second->getNamespace()].push_back(type.second);
        }
    }
    for (auto& ns_types : perNS)
    {
        Namespace* ns = ns_types.first;
        Strings    nsNames;
        while (ns != Namespace::Global())
        {
            nsNames.push_back(ns->getName());
            ns = ns->getParentNamespace();
        }
        size_t i = nsNames.size();
        while (i--)
        {
            a_Str += "namespace " + nsNames[i] + " { ";
        }
        a_Str += '\n';

        for (auto pType : ns_types.second)
        {
            if (auto pSpec = pType->getTemplateSpecialization())
            {
                _formatTemplateSign(ns_types.first, pSpec->getTemplate()->getTemplateSignature(), a_Str);
            }
            if (auto pClass = pType->asClass())
            {
                if (pClass->getDefaultAccess() == Access::Public)
                    a_Str += "struct ";
                else
                    a_Str += "class ";
            }
            else if (auto pE = pType->asEnum())
            {
                a_Str += "enum ";
                if (pE->isScoped())
                    a_Str += "class ";
            }
            else if (pType->asUnion())
            {
                a_Str += "union ";
            }
            a_Str += pType->getName();
            a_Str += ";\n";
        }

        for (auto nsName : nsNames)
        {
            a_Str += "}";
        }
        a_Str += '\n';
    }
}

void CppTranslator::appendSignatureQuals(Signature* a_pSignature)
{
    if (a_pSignature->testModifiers(PHANTOM_R_CONST))
    {
        append(" const");
    }
    if (a_pSignature->testModifiers(PHANTOM_R_VOLATILE))
    {
        append(" volatile");
    }
    if (a_pSignature->testModifiers(PHANTOM_R_LVALUEREF) || a_pSignature->testModifiers(PHANTOM_R_RVALUEREF))
    {
        append('&');
        if (a_pSignature->testModifiers(PHANTOM_R_RVALUEREF))
        {
            append('&');
        }
    }
}

void CppTranslator::addIncludeIfRequired(Type* a_pType)
{
    a_pType = a_pType->removeQualifiers();
    if (a_pType->asClassType() || a_pType->asEnum())
        addInclude(a_pType);
}

void CppTranslator::_appendCtorInit(Constructor* a_pInput, VisitorData a_Data)
{
    if (a_Data.id != TranslationType::Definition && !a_pInput->isTemplateElement())
        return;

    if (a_pInput->getBlock() && !a_pInput->testModifiers(Modifier::Defaulted) &&
        !a_pInput->testModifiers(Modifier::Deleted)) // template or inline
    {
        int c = 0;
        m_IndentationLevel++;
        for (auto pStatement : a_pInput->getBlock()->getStatements())
        {
            if (auto pBC = Object::Cast<BaseConstructorCallStatement>(pStatement))
            {
                if (!pBC->isImplicit())
                {
                    if (c == 0)
                        newLine();
                    beginDecl();
                    if (c++)
                        append(", ");
                    else
                        append(": ");
                    pBC->getConstructorCallExpression()->visit(this, a_Data);
                    endDecl();
                }
            }
            else if (auto pFI = Object::Cast<FieldInitializationStatement>(pStatement))
            {
                if (!pFI->isImplicit())
                {
                    if (c == 0)
                        newLine();
                    beginDecl();
                    if (c++)
                        append(", ");
                    else
                        append(": ");
                    append(pFI->getField()->getName());
                    append("(");
                    if (auto pCtorCall = pFI->getInitializationExpression()->as<ConstructorCallExpression>())
                    {
                        int d = 0;
                        for (auto pArg : pCtorCall->getArguments())
                        {
                            if (d++)
                                append(',');
                            pArg->visit(this, a_Data);
                        }
                    }
                    else
                    {
                        pFI->getInitializationExpression()->visit(this, a_Data);
                    }
                    append(")");
                    endDecl();
                }
            }
        }
        m_IndentationLevel--;
    }
}

void CppTranslator::_appendHauntMarkers(Symbol* a_pInput)
{
    if (a_pInput->getMetaDatas().size())
    {
        if (Method* pMeth = a_pInput->asMethod())
        {
            if (Property* pProp = pMeth->getProperty())
            {
                append("/// @property");
                endDecl();
                beginDecl();
                _appendHauntMarkers(pProp);
            }
        }
        append("/// @metadata[");
        int c = 0;
        for (auto& md : a_pInput->getMetaDatas())
        {
            if (c++)
                append(",");
            append("[");
            StringBuffer key;
            String       str(md.first.getString());
            StringConverter<String>::toLiteral(nullptr, key, &str);
            append(StringView(key));
            if (md.second.type())
            {
                key.clear();
                if (md.second.type()->isSame(PHANTOM_TYPEOF(String)))
                {
                    append(",");
                    StringConverter<String>::toLiteral(nullptr, key, (String*)md.second.data());
                }
                else if (md.second.type()->asFundamentalType() || md.second.type()->asEnum())
                {
                    append(",");
                    md.second.type()->valueToLiteral(key, md.second.data());
                }
                else
                {
                    PHANTOM_LOG(Warning, "unable to translate metadata");
                }
                append(StringView(key));
            }
            append("]");
        }
        append("]");
        endDecl();
        beginDecl();
    }
}

} // namespace lang
} // namespace phantom
