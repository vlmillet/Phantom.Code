// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
// #define PHANTOM_USE_BOOST_SPIRIT_CPLUSPLUS_PARSER // uncomment to disable parser
#include "CppLite.h"

#include "BuiltInOperator.h"
#include "CppLiteParser.h"
#include "Expression.h"
#include "phantom/lang/LValueReference.h"
#include "phantom/lang/Package.h"
#include "phantom/lang/Source.h"
#include "phantom/lang/VolatileType.h"
#include "phantom/lang/reflection.h"
// #include "CppLiteTranslator.h"
#include "BuiltInOperators.h"
#include "CallExpression.h"
#include "Message.h"
#include "phantom/detail/core_internal.h"
#include "phantom/lang/Semantic.h"

#include <fstream>
#include <phantom/lang/Application.h>
#include <phantom/utils/Path.h>
#include <phantom/utils/StringUtil.h>
#include <system_error>
/* *********************************************** */

namespace phantom
{
namespace lang
{
extern PHANTOM_EXPORT_PHANTOM Functor<void(Class*)> m_SemanticClassFinalizer;

CppLite::CppLite() : Language("CppLite", ".cpplite", CppLiteParser::e_Pass_Count)
{
    addOption("Safe", "false");
    addOption("ZeroInit", "false");
    addOption("DebugTraceFunction", "false");
    addOption("GlobalMethodOverload", "true");

    m_pPhantomModule = PHANTOM_CLASSOF(Application)->getModule();
    PHANTOM_ASSERT(m_pPhantomModule);
    m_pPhantomCodeModule = PHANTOM_CLASSOF(CppLite)->getModule();
    PHANTOM_ASSERT(m_pPhantomCodeModule);

    m_pDefaultSource = PHANTOM_CLASSOF(CppLite)->getPackage()->getOrCreateSource("default");

    Application::Get()->setCppExpressionParser(Application::CppExpressionParser(this, &CppLite::_internalExpression));
    Application::Get()->setCppExpressionEvaluator(Application::CppExpressionEvaluator(this, &CppLite::_evalExpression));

    for (int i = 0; i < int(Operator::Conversion); ++i)
    {
        m_BuiltInOperators[i] = m_pDefaultSource->New<BuiltInOperator>((Operator)i);
    }
    _installFundamentalBuiltInOperators(true);
    m_pDefaultSemantic = new_<Semantic>(m_pDefaultSource, &m_DefaultMessage);

    m_SemanticClassFinalizer = [=](Class* a_pClass) {
        m_pDefaultSemantic->buildClass(a_pClass, Semantic::e_ClassBuildState_Blocks);
    };
}

CppLite::~CppLite(void)
{
    m_SemanticClassFinalizer = {};
    delete_<Semantic>(m_pDefaultSemantic);
    _installFundamentalBuiltInOperators(false);
    //     for (int i = 0; i < int(Operator::Conversion); ++i)
    //     {
    //         m_pDefaultSource->Delete(m_BuiltInOperators[i]);
    //     }
    Application::Get()->setCppExpressionParser(Application::CppExpressionParser());
    Application::Get()->setCppExpressionEvaluator(Application::CppExpressionEvaluator());
}

void CppLite::Load() {}

void CppLite::_installFundamentalBuiltInOperators(bool a_bAdd)
{
    PHANTOM_ASSERT(Application::Get()->getBuiltInTypes().size());
    builtin_operation_delegate_selector selector;

    Type* pBoolType = PHANTOM_TYPEOF(bool);
    Type* pIntType = PHANTOM_TYPEOF(int);

    /// standard 13.6.2 : promoted integral and arithmetic types
    /// ------------------------
    Types promotedArithmeticTypes;
    Types promotedIntegralTypes;
    Types arithmeticTypes;
    for (Type* pType : Application::Get()->getBuiltInTypes())
    {
        if (pType->asArithmeticType())
        {
            arithmeticTypes.push_back(pType);
        }
        if (pType->isPromotedIntegralType())
        {
            promotedIntegralTypes.push_back(pType);
            promotedArithmeticTypes.push_back(pType);
        }
        else if (pType->asFloatingPointType())
        {
            promotedArithmeticTypes.push_back(pType);
        }
    }

    for (auto it = arithmeticTypes.begin(); it != arithmeticTypes.end(); ++it)
    {
        /// standard 13.6.3
        /// ------------------------
        /// VQ T & operator++(VQ T &);
        /// T operator++(VQ T &, int);
        Type* T = *it;
        Type* Tr = T->makeLValueReference();
        Type* VTr = T->makeVolatile()->makeLValueReference();
        _builtInOperator(Operator::PreIncrement)
        ->editOverload(a_bAdd, Tr, Tr, selector.delegate(Operator::PreIncrement, T));
        _builtInOperator(Operator::PreIncrement)
        ->editOverload(a_bAdd, VTr, VTr, selector.delegate(Operator::PreIncrement, T));
        _builtInOperator(Operator::PostIncrement)
        ->editOverload(a_bAdd, T, Tr, pIntType, selector.delegate(Operator::PostIncrement, T));
        _builtInOperator(Operator::PostIncrement)
        ->editOverload(a_bAdd, T, VTr, pIntType, selector.delegate(Operator::PostIncrement, T));

        if (*it != pBoolType)
        {
            /// standard 13.6.4
            /// ------------------------
            /// VQ T & operator--(VQ T &);
            /// T operator--(VQ T &, int);
            _builtInOperator(Operator::PreDecrement)
            ->editOverload(a_bAdd, Tr, Tr, selector.delegate(Operator::PreDecrement, T));
            _builtInOperator(Operator::PreDecrement)
            ->editOverload(a_bAdd, VTr, VTr, selector.delegate(Operator::PreDecrement, T));
            _builtInOperator(Operator::PostDecrement)
            ->editOverload(a_bAdd, T, Tr, pIntType, selector.delegate(Operator::PostDecrement, T));
            _builtInOperator(Operator::PostDecrement)
            ->editOverload(a_bAdd, T, VTr, pIntType, selector.delegate(Operator::PostDecrement, T));
        }

        Type* L = *it;
        Type* Lr = T->makeLValueReference();
        Type* VLr = T->makeVolatile()->makeLValueReference();

        for (auto pit = promotedArithmeticTypes.begin(); pit != promotedArithmeticTypes.end(); ++pit)
        {
            Type* R = *pit;
            /// standard 13.6.18
            /// ------------------------
            /// VQ L & operator=(VQ L &, R );
            /// VQ L & operator*=(VQ L &, R );
            /// VQ L & operator/=(VQ L &, R );
            /// VQ L & operator+=(VQ L &, R );
            /// VQ L & operator-=(VQ L &, R );

            _builtInOperator(Operator::Assignment)
            ->editOverload(a_bAdd, Lr, Lr, R, selector.delegate(Operator::Assignment, L, R));
            _builtInOperator(Operator::Assignment)
            ->editOverload(a_bAdd, VLr, VLr, R, selector.delegate(Operator::Assignment, L, R));
            _builtInOperator(Operator::AssignmentMultiply)
            ->editOverload(a_bAdd, Lr, Lr, R, selector.delegate(Operator::AssignmentMultiply, L, R));
            _builtInOperator(Operator::AssignmentMultiply)
            ->editOverload(a_bAdd, VLr, VLr, R, selector.delegate(Operator::AssignmentMultiply, L, R));
            _builtInOperator(Operator::AssignmentDivide)
            ->editOverload(a_bAdd, Lr, Lr, R, selector.delegate(Operator::AssignmentDivide, L, R));
            _builtInOperator(Operator::AssignmentDivide)
            ->editOverload(a_bAdd, VLr, VLr, R, selector.delegate(Operator::AssignmentDivide, L, R));
            _builtInOperator(Operator::AssignmentAdd)
            ->editOverload(a_bAdd, Lr, Lr, R, selector.delegate(Operator::AssignmentAdd, L, R));
            _builtInOperator(Operator::AssignmentAdd)
            ->editOverload(a_bAdd, VLr, VLr, R, selector.delegate(Operator::AssignmentAdd, L, R));
            _builtInOperator(Operator::AssignmentSubtract)
            ->editOverload(a_bAdd, Lr, Lr, R, selector.delegate(Operator::AssignmentSubtract, L, R));
            _builtInOperator(Operator::AssignmentSubtract)
            ->editOverload(a_bAdd, VLr, VLr, R, selector.delegate(Operator::AssignmentSubtract, L, R));
        }
    }

    for (auto it = promotedArithmeticTypes.begin(); it != promotedArithmeticTypes.end(); ++it)
    {
        Type* L = *it;

        /// standard 13.6.9
        /// ------------------------
        /// T operator+(T );
        /// T operator-(T );
        _builtInOperator(Operator::Plus)->editOverload(a_bAdd, L, L, selector.delegate(Operator::Plus, L));
        _builtInOperator(Operator::Minus)->editOverload(a_bAdd, L, L, selector.delegate(Operator::Minus, L));

        for (auto it2 = promotedArithmeticTypes.begin(); it2 != promotedArithmeticTypes.end(); ++it2)
        {
            Type* R = *it2;

            /// standard 13.6.12 (L and R are promoted arithmetic types)
            /// ------------------------
            /// LR operator*(L , R );
            /// LR operator/(L , R );
            /// LR operator+(L , R );
            /// LR operator-(L , R );
            /// bool operator<(L , R );
            /// bool operator>(L , R );
            /// bool operator<=(L , R );
            /// bool operator>=(L , R );
            /// bool operator==(L , R );
            /// bool operator!=(L , R );

            Type* LR = (L->getTypeKind() > R->getTypeKind()) ? L : R; /// unary arithmetic conversion (standard 5.x)
            _builtInOperator(Operator::Multiply)
            ->editOverload(a_bAdd, LR, L, R, selector.delegate(Operator::Multiply, L, R));
            _builtInOperator(Operator::Divide)
            ->editOverload(a_bAdd, LR, L, R, selector.delegate(Operator::Divide, L, R));
            _builtInOperator(Operator::Add)->editOverload(a_bAdd, LR, L, R, selector.delegate(Operator::Add, L, R));
            _builtInOperator(Operator::Subtract)
            ->editOverload(a_bAdd, LR, L, R, selector.delegate(Operator::Subtract, L, R));
            _builtInOperator(Operator::Less)
            ->editOverload(a_bAdd, pBoolType, L, R, selector.delegate(Operator::Less, L, R));
            _builtInOperator(Operator::Greater)
            ->editOverload(a_bAdd, pBoolType, L, R, selector.delegate(Operator::Greater, L, R));
            _builtInOperator(Operator::LessEqual)
            ->editOverload(a_bAdd, pBoolType, L, R, selector.delegate(Operator::LessEqual, L, R));
            _builtInOperator(Operator::GreaterEqual)
            ->editOverload(a_bAdd, pBoolType, L, R, selector.delegate(Operator::GreaterEqual, L, R));
            _builtInOperator(Operator::Equal)
            ->editOverload(a_bAdd, pBoolType, L, R, selector.delegate(Operator::Equal, L, R));
            _builtInOperator(Operator::NotEqual)
            ->editOverload(a_bAdd, pBoolType, L, R, selector.delegate(Operator::NotEqual, L, R));
        }
    }

    for (auto it = promotedIntegralTypes.begin(); it != promotedIntegralTypes.end(); ++it)
    {
        Type* L = *it;

        /// standard 13.6.10
        /// ------------------------
        /// T operator~(T );
        _builtInOperator(Operator::Complement)->editOverload(a_bAdd, L, L, selector.delegate(Operator::Complement, L));

        for (auto it2 = promotedIntegralTypes.begin(); it2 != promotedIntegralTypes.end(); ++it2)
        {
            Type* R = *it2;

            if (auto pLE = L->asEnum())
                if (pLE->isScoped())
                    if (R != L) // Left is scoped enum but right is not equal to left
                        continue;

            if (auto pRE = R->asEnum())
                if (pRE->isScoped())
                    continue;

            Type* Lr = L->makeLValueReference();
            Type* VLr = L->makeVolatile()->makeLValueReference();

            Type* LR = (L->getTypeKind() > R->getTypeKind()) ? L : R; /// unary arithmetic conversion (standard 5.x)

            /// standard 13.6.17
            /// ------------------------
            /// LR operator%(L , R );
            /// LR operator&(L , R );
            /// LR operator^(L , R );
            /// LR operator|(L , R );
            /// L operator<<(L , R );
            /// L operator>>(L , R );

            _builtInOperator(Operator::Modulo)
            ->editOverload(a_bAdd, LR, L, R, selector.delegate(Operator::Modulo, L, R));
            _builtInOperator(Operator::BitAnd)
            ->editOverload(a_bAdd, LR, L, R, selector.delegate(Operator::BitAnd, L, R));
            _builtInOperator(Operator::XOr)->editOverload(a_bAdd, LR, L, R, selector.delegate(Operator::XOr, L, R));
            _builtInOperator(Operator::BitOr)->editOverload(a_bAdd, LR, L, R, selector.delegate(Operator::BitOr, L, R));
            _builtInOperator(Operator::ShiftLeft)
            ->editOverload(a_bAdd, L, L, R, selector.delegate(Operator::ShiftLeft, L, R));
            _builtInOperator(Operator::ShiftRight)
            ->editOverload(a_bAdd, L, L, R, selector.delegate(Operator::ShiftRight, L, R));

            /// standard 13.6.22
            /// ------------------------
            /// VQ L & operator%=(VQ L &, R );
            /// VQ L & operator<<=(VQ L &, R );
            /// VQ L & operator>>=(VQ L &, R );
            /// VQ L & operator&=(VQ L &, R );
            /// VQ L & operator^=(VQ L &, R );
            /// VQ L & operator|=(VQ L &, R );
            _builtInOperator(Operator::AssignmentModulo)
            ->editOverload(a_bAdd, Lr, Lr, R, selector.delegate(Operator::AssignmentModulo, L, R));
            _builtInOperator(Operator::AssignmentShiftLeft)
            ->editOverload(a_bAdd, Lr, Lr, R, selector.delegate(Operator::AssignmentShiftLeft, L, R));
            _builtInOperator(Operator::AssignmentShiftRight)
            ->editOverload(a_bAdd, Lr, Lr, R, selector.delegate(Operator::AssignmentShiftRight, L, R));
            _builtInOperator(Operator::AssignmentBitAnd)
            ->editOverload(a_bAdd, Lr, Lr, R, selector.delegate(Operator::AssignmentBitAnd, L, R));
            _builtInOperator(Operator::AssignmentXOr)
            ->editOverload(a_bAdd, Lr, Lr, R, selector.delegate(Operator::AssignmentXOr, L, R));
            _builtInOperator(Operator::AssignmentBitOr)
            ->editOverload(a_bAdd, Lr, Lr, R, selector.delegate(Operator::AssignmentBitOr, L, R));

            _builtInOperator(Operator::AssignmentModulo)
            ->editOverload(a_bAdd, VLr, VLr, R, selector.delegate(Operator::AssignmentModulo, L, R));
            _builtInOperator(Operator::AssignmentShiftLeft)
            ->editOverload(a_bAdd, VLr, VLr, R, selector.delegate(Operator::AssignmentShiftLeft, L, R));
            _builtInOperator(Operator::AssignmentShiftRight)
            ->editOverload(a_bAdd, VLr, VLr, R, selector.delegate(Operator::AssignmentShiftRight, L, R));
            _builtInOperator(Operator::AssignmentBitAnd)
            ->editOverload(a_bAdd, VLr, VLr, R, selector.delegate(Operator::AssignmentBitAnd, L, R));
            _builtInOperator(Operator::AssignmentXOr)
            ->editOverload(a_bAdd, VLr, VLr, R, selector.delegate(Operator::AssignmentXOr, L, R));
            _builtInOperator(Operator::AssignmentBitOr)
            ->editOverload(a_bAdd, VLr, VLr, R, selector.delegate(Operator::AssignmentBitOr, L, R));
        }
    }

    /// standard 13.6.16 (for std::nullptr_t)
    /// ------------------------
    /// bool operator==(T , T );
    /// bool operator!=(T , T );
    _builtInOperator(Operator::Equal)
    ->editOverload(a_bAdd, pBoolType, PHANTOM_TYPEOF(std::nullptr_t), PHANTOM_TYPEOF(std::nullptr_t),
                   &builtin_operation_delegate_Equal<std::nullptr_t, std::nullptr_t>::apply);
    _builtInOperator(Operator::NotEqual)
    ->editOverload(a_bAdd, pBoolType, PHANTOM_TYPEOF(std::nullptr_t), PHANTOM_TYPEOF(std::nullptr_t),
                   &builtin_operation_delegate_NotEqual<std::nullptr_t, std::nullptr_t>::apply);

    /// standard 13.6.23
    /// ------------------------
    /// bool operator!(bool);
    /// bool operator&&(bool, bool);
    /// bool operator||(bool, bool);

    _builtInOperator(Operator::Not)
    ->editOverload(a_bAdd, pBoolType, pBoolType, &builtin_operation_delegate_Not<bool>::apply);
    _builtInOperator(Operator::LogicalAnd)
    ->editOverload(a_bAdd, pBoolType, pBoolType, pBoolType, &builtin_operation_delegate_LogicalAnd<bool, bool>::apply);
    _builtInOperator(Operator::LogicalOr)
    ->editOverload(a_bAdd, pBoolType, pBoolType, pBoolType, &builtin_operation_delegate_LogicalOr<bool, bool>::apply);
}

BuiltInOperator* CppLite::getBuiltInBinaryOperator(StringView a_strName) const
{
    auto op = getBuiltInBinaryOperatorId(a_strName);
    return op == Operator::Unknown ? nullptr : _builtInOperator(op);
}

BuiltInOperator* CppLite::getBuiltInPreUnaryOperator(StringView a_strName) const
{
    auto op = getBuiltInPreUnaryOperatorId(a_strName);
    return op == Operator::Unknown ? nullptr : _builtInOperator(op);
}

BuiltInOperator* CppLite::getBuiltInPostUnaryOperator(StringView a_strName) const
{
    auto op = getBuiltInPostUnaryOperatorId(a_strName);
    return op == Operator::Unknown ? nullptr : _builtInOperator(op);
}

Operator CppLite::getBuiltInBinaryOperatorId(StringView a_strName) const
{
    PHANTOM_ASSERT(phantom::detail::installed());
    static SmallMap<String, Operator> s_OperatorNames;
    if (s_OperatorNames.empty())
    {
        s_OperatorNames["+"] = Operator::Add;
        s_OperatorNames["-"] = Operator::Subtract;
        s_OperatorNames["*"] = Operator::Multiply;
        s_OperatorNames["/"] = Operator::Divide;
        s_OperatorNames["->*"] = Operator::ArrowStar;
        s_OperatorNames["=="] = Operator::Equal;
        s_OperatorNames["!="] = Operator::NotEqual;
        s_OperatorNames[">"] = Operator::Greater;
        s_OperatorNames["<"] = Operator::Less;
        s_OperatorNames[">="] = Operator::GreaterEqual;
        s_OperatorNames["<="] = Operator::LessEqual;
        s_OperatorNames["&&"] = Operator::LogicalAnd;
        s_OperatorNames["||"] = Operator::LogicalOr;
        s_OperatorNames["^"] = Operator::XOr;
        s_OperatorNames["&"] = Operator::BitAnd;
        s_OperatorNames["|"] = Operator::BitOr;
        s_OperatorNames["%"] = Operator::Modulo;
        s_OperatorNames["<<"] = Operator::ShiftLeft;
        s_OperatorNames[">>"] = Operator::ShiftRight;
        s_OperatorNames["="] = Operator::Assignment;
        s_OperatorNames["+="] = Operator::AssignmentAdd;
        s_OperatorNames["-="] = Operator::AssignmentSubtract;
        s_OperatorNames["*="] = Operator::AssignmentMultiply;
        s_OperatorNames["/="] = Operator::AssignmentDivide;
        s_OperatorNames["&="] = Operator::AssignmentBitAnd;
        s_OperatorNames["|="] = Operator::AssignmentBitOr;
        s_OperatorNames["%="] = Operator::AssignmentModulo;
        s_OperatorNames["<<="] = Operator::AssignmentShiftLeft;
        s_OperatorNames[">>="] = Operator::AssignmentShiftRight;
        s_OperatorNames["^="] = Operator::AssignmentXOr;
        s_OperatorNames["[]"] = Operator::Bracket;
    }
    auto found = s_OperatorNames.find(a_strName);
    if (found == s_OperatorNames.end())
        return Operator::Unknown;
    return found->second;
}

Operator CppLite::getBuiltInPreUnaryOperatorId(StringView a_strName) const
{
    PHANTOM_ASSERT(phantom::detail::installed());
    static SmallMap<String, Operator> s_OperatorNames;
    if (s_OperatorNames.empty())
    {
        s_OperatorNames["+"] = Operator::Plus;
        s_OperatorNames["-"] = Operator::Minus;
        s_OperatorNames["++"] = Operator::PreIncrement;
        s_OperatorNames["--"] = Operator::PreDecrement;
        s_OperatorNames["~"] = Operator::Complement;
        s_OperatorNames["!"] = Operator::Not;
        s_OperatorNames["&"] = Operator::Address;
        s_OperatorNames["*"] = Operator::Dereference;
    }
    auto found = s_OperatorNames.find(a_strName);
    if (found == s_OperatorNames.end())
        return Operator::Unknown;
    return found->second;
}

Operator CppLite::getBuiltInPostUnaryOperatorId(StringView a_strName) const
{
    PHANTOM_ASSERT(phantom::detail::installed());
    static SmallMap<String, Operator> s_OperatorNames;
    if (s_OperatorNames.empty())
    {
        s_OperatorNames["++"] = Operator::PostIncrement;
        s_OperatorNames["--"] = Operator::PostDecrement;
        s_OperatorNames["->"] = Operator::Arrow;
    }
    auto found = s_OperatorNames.find(a_strName);
    if (found == s_OperatorNames.end())
        return Operator::Unknown;
    return found->second;
}

bool CppLite::checkSymbolSyntax(StringView a_strSymbol) const
{
    Message msg;
    return CppLiteParser(m_pDefaultSource, &msg,
                         CppLiteParser::Flags(CppLiteParser::e_Flag_StandardLookup) | CppLiteParser::e_Flag_SymbolMode)
    .checkSymbolSyntax(a_strSymbol);
}

bool CppLite::isIdentifier(StringView a_strName) const
{
    if (a_strName.empty())
        return false;
    for (auto it = a_strName.begin(); it != a_strName.end(); ++it)
    {
        char c = *it;
        if (!((((c) >= '0') && ((c) <= '9')) || ((((c) | 0x20) >= 'a') && (((c) | 0x20) <= 'z')) || (c) == '_'))
            return false;
    }
    return true;
}

Symbol* CppLite::findSymbol(StringView a_strSymbol, LanguageElement* a_pContextScope /*= nullptr*/,
                            Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    Message msg;
    return CppLiteParser(_selectBestSource(a_pContextScope), &msg,
                         CppLiteParser::Flags(CppLiteParser::e_Flag_StandardLookup) | CppLiteParser::e_Flag_SymbolMode)
    .parseSymbolName(a_strSymbol, a_pContextScope, a_Modifiers, a_uiFlags);
}

void CppLite::findSymbols(StringView a_strSymbolsName, Symbols& a_Symbols,
                          LanguageElement* a_pContextScope /*= nullptr*/)
{
    phantom::lang::Accesses accesses;
    getDefaultSemantic()->unqualifiedSymbolsLookup(a_strSymbolsName, a_Symbols, &accesses, a_pContextScope);
}

Symbol* CppLite::parseSymbol(StringView a_strSymbol, LanguageElement* a_pContextScope /*= nullptr*/,
                             Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    Message msg;
    return CppLiteParser(_selectBestSource(a_pContextScope), &msg,
                         CppLiteParser::Flags(CppLiteParser::e_Flag_StandardLookup) | CppLiteParser::e_Flag_SymbolMode |
                         CppLiteParser::e_Flag_NoCodeLocation)
    .parseSymbolName(a_strSymbol, a_pContextScope, a_Modifiers, a_uiFlags);
}

Source* CppLite::_selectBestSource(LanguageElement* a_pScope)
{
    if (!a_pScope || a_pScope->getModule() == m_pPhantomModule)
        return m_pDefaultSource;
    auto& modules = Application::Get()->getModules(); // ASSERT_DEBUG

    // module has been loaded before Phantom.Code, we need to use Phantom.Code default source to avoid having dead meta
    // types on module release
    if (std::find(modules.begin(), modules.end(), a_pScope->getModule()) <
        std::find(modules.begin(), modules.end(), m_pPhantomCodeModule))
        return m_pDefaultSource;
    return a_pScope->getSource();
}

Expression* CppLite::_internalExpression(StringView a_strExpression, LanguageElement* a_pContextScope /*= nullptr*/)
{
    return expression(a_strExpression, a_pContextScope);
}

Variant CppLite::_evalExpression(Expression* _exp)
{
    return _exp->get();
}

Expression* CppLite::expression(StringView a_strExpression, Message* a_pMessage,
                                LanguageElement* a_pContextScope /*= nullptr*/)
{
    return parseExpression(a_strExpression, a_pMessage, a_pContextScope);
}

Expression* CppLite::expression(StringView a_strExpression, LanguageElement* a_pContextScope /*= nullptr*/)
{
    Message msg;
    return expression(a_strExpression, &msg, a_pContextScope);
}

Expression* CppLite::parseExpression(StringView a_strExpression, Message* a_pMessage,
                                     LanguageElement* a_pContextScope /*= nullptr*/)
{
    if (a_pContextScope == nullptr || a_pContextScope->asNamespace() || a_pContextScope->getSource()->isNative())
    {
        return CppLiteParser(_selectBestSource(a_pContextScope), a_pMessage, CppLiteParser::e_Flag_StandardLookup)
        .parseExpression(a_strExpression, a_pContextScope);
    }
    else
    {
        Source* pSource = a_pContextScope->getSource();
        PHANTOM_ASSERT(pSource && !pSource->isNative());
        return CppLiteParser(pSource, a_pMessage).parseExpression(a_strExpression, a_pContextScope);
    }
}

Expression* CppLite::parseExpression(StringView a_strExpression, LanguageElement* a_pContextScope /*= nullptr*/)
{
    Message msg;
    return parseExpression(a_strExpression, &msg, a_pContextScope);
}

Parser* CppLite::createParser(Source* a_pSource, Message* a_pMessage) const
{
    return new_<CppLiteParser>(a_pSource, a_pMessage);
}

void CppLite::destroyParser(Parser* a_pParser) const
{
    delete_<CppLiteParser>(static_cast<CppLiteParser*>(a_pParser));
}

Semantic* CppLite::createSemantic(Source* a_pSource, Message* a_pMessage) const
{
    return new_<Semantic>(a_pSource, a_pMessage);
}

void CppLite::destroySemantic(Semantic* a_pSemantic) const
{
    delete_<Semantic>(a_pSemantic);
}

extern StaticGlobal<CppLite> g_PHNTM_LNG_CppLite;

CppLite* CppLite::Get()
{
    return g_PHNTM_LNG_CppLite;
}

} // namespace lang
} // namespace phantom
