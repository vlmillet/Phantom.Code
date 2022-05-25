// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "CppLiteParser.h"

#include "CppLite.h"
#pragma warning(push, 0)
#include "CppLiteGrammar.h"
#pragma warning(pop)

// Fundamental expressions
#include <fstream>
#include <phantom/lang/Alias.h>
#include <phantom/lang/AllocateExpression.h>
#include <phantom/lang/Application.h>
#include <phantom/lang/ArrayExpression.h>
#include <phantom/lang/BaseConstructorCallStatement.h>
#include <phantom/lang/BinaryOperationExpression.h>
#include <phantom/lang/Block.h>
#include <phantom/lang/BranchIfNotStatement.h>
#include <phantom/lang/BranchIfStatement.h>
#include <phantom/lang/BranchStatement.h>
#include <phantom/lang/BuiltInConversionExpression.h>
#include <phantom/lang/BuiltInOperator.h>
#include <phantom/lang/BuiltInOperatorExpression.h>
#include <phantom/lang/CallExpression.h>
#include <phantom/lang/CommaExpression.h>
#include <phantom/lang/ConditionalExpression.h>
#include <phantom/lang/Constant.h>
#include <phantom/lang/ConstantExpression.h>
#include <phantom/lang/Constructor.h>
#include <phantom/lang/ConstructorCallExpression.h>
#include <phantom/lang/CppLite.h>
#include <phantom/lang/DeallocateExpression.h>
#include <phantom/lang/DeleteExpression.h>
#include <phantom/lang/Destructor.h>
#include <phantom/lang/Enum.h>
#include <phantom/lang/Expression.h>
#include <phantom/lang/ExpressionStatement.h>
#include <phantom/lang/Field.h>
#include <phantom/lang/FieldExpression.h>
#include <phantom/lang/FieldInitializationStatement.h>
#include <phantom/lang/FieldPointer.h>
#include <phantom/lang/FieldPointerAdjustmentExpression.h>
#include <phantom/lang/FieldPointerExpression.h>
#include <phantom/lang/Function.h>
#include <phantom/lang/FunctionPointer.h>
#include <phantom/lang/FundamentalsT.h>
#include <phantom/lang/Import.h>
#include <phantom/lang/InitializerListExpression.h>
#include <phantom/lang/Label.h>
#include <phantom/lang/LoadExpression.h>
#include <phantom/lang/LocalVariable.h>
#include <phantom/lang/LocalVariableExpression.h>
#include <phantom/lang/LocalVariableInitializationStatement.h>
#include <phantom/lang/Message.h>
#include <phantom/lang/MethodPointer.h>
#include <phantom/lang/MethodPointerAdjustmentExpression.h>
#include <phantom/lang/MethodPointerCallExpression.h>
#include <phantom/lang/Module.h>
#include <phantom/lang/Namespace.h>
#include <phantom/lang/NewExpression.h>
#include <phantom/lang/OwnerGuard.h>
#include <phantom/lang/Package.h>
#include <phantom/lang/PackageFolder.h>
#include <phantom/lang/Parameter.h>
#include <phantom/lang/ParameterPackExpressionExpansion.h>
#include <phantom/lang/Placeholder.h>
#include <phantom/lang/PlaceholderConstant.h>
#include <phantom/lang/PlaceholderType.h>
#include <phantom/lang/PlacementNewExpression.h>
#include <phantom/lang/Pointer.h>
#include <phantom/lang/PointerAdjustmentExpression.h>
#include <phantom/lang/Property.h>
#include <phantom/lang/PropertyExpression.h>
#include <phantom/lang/RValueReferenceExpression.h>
#include <phantom/lang/Reference.h>
#include <phantom/lang/ReturnStatement.h>
#include <phantom/lang/Semantic.h>
#include <phantom/lang/Signature.h>
#include <phantom/lang/Source.h>
#include <phantom/lang/SourceFile.h>
#include <phantom/lang/Statement.h>
#include <phantom/lang/StaticAssert.h>
#include <phantom/lang/StringLiteralExpression.h>
#include <phantom/lang/SubroutinePointerExpression.h>
#include <phantom/lang/SymbolReferenceExpression.h>
#include <phantom/lang/Template.h>
#include <phantom/lang/TemplateDependantArray.h>
#include <phantom/lang/TemplateDependantDeclType.h>
#include <phantom/lang/TemplateDependantElement.h>
#include <phantom/lang/TemplateDependantExpression.h>
#include <phantom/lang/TemplateDependantTypeOfExpression.h>
#include <phantom/lang/TemplateParameter.h>
#include <phantom/lang/TemplateSignature.h>
#include <phantom/lang/TemplateSpecialization.h>
#include <phantom/lang/TypeTraitExpression.h>
#include <phantom/lang/UnaryPostOperationExpression.h>
#include <phantom/lang/UnaryPreOperationExpression.h>
#include <phantom/lang/Union.h>
#include <phantom/lang/VariableExpression.h>
#include <phantom/utils/StringUtil.h>
#include <set>
#include <sstream>
#define CPPLIGHT_ENABLED_DEBUG_LEAK_CHECKS 0
#define CPPLIGHT_ENABLED_DEBUG_LEAK_HISTORY 0
#if CPPLIGHT_ENABLED_DEBUG_LEAK_HISTORY
#    include "phantom/utils/History.h"
#endif
#include "phantom/lang/Variable.h"
#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    include <windows.h>
#endif
#include "phantom/lang/CompiledSource.h"
#include "phantom/lang/Compiler.h"
#include "phantom/lang/Plugin.h"

#include <phantom/function>
#include <phantom/typeof>
#include <phantom/utils/Generic.h>
/* *********************************************** */
#include "ClassListInitializationExpression.h"
#include "CppLiteMemory.h"
#include "TemplateParameterPackExpansion.h"
#include "TemplateParameterPackTypeExpansion.h"
#include "phantom/lang/TemplateDependantTemplateInstance.h"
#include "phantom/utils/random.h"

#include <phantom/lang/FunctionType.h>
#define DUMP_CODE_RANGES 0

#define CPPLITEPARSER_ASSERT PHANTOM_ASSERT_DEBUG

namespace phantom
{
namespace lang
{
bool CppLiteAssert(const char* e, const char* file, int line)
{
    return phantom::assertion(e, file, line, e);
}

void DebugBreak()
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    ::DebugBreak();
#else
    __builtin_trap();
#endif
}

PHANTOM_FUNCTION(bool, CppLiteAssert, (const char*, const char* file, int));
PHANTOM_FUNCTION(void, DebugBreak, ());

namespace
{
// Function to check if x is power of 2
auto isPowerOfTwo(int n) -> bool
{
    if (n == 0)
        return false;

    return (ceil(log2(n)) == floor(log2(n)));
}
} // namespace

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
static inline uint32_t clz(uint32_t value)
{
    DWORD leading_zero = 0;

    if (_BitScanReverse(&leading_zero, value))
    {
        return 31 - leading_zero;
    }
    else
    {
        // Same remarks as above
        return 32;
    }
}
#else
#    define clz(value) __builtin_clz(value)

#endif

// Stops at any null characters.
int decode_code_point(char** s)
{
    int k = **s ? clz(~(**s << 24)) : 0; // Count # of leading 1 bits.
    int mask = (1 << (8 - k)) - 1;       // All 1's with k leading 0's.
    int value = **s & mask;
    for (++(*s), --k; k > 0 && **s; --k, ++(*s))
    { // Note that k = #total bytes, or 0.
        value <<= 6;
        value += (**s & 0x3F);
    }
    return value;
}

// Assumes that code is <= 0x10FFFF.
// Ensures that nothing will be written at or beyond end.
void encode_code_point(char** s, char* end, int code)
{
    char val[4];
    int  lead_byte_max = 0x7F;
    int  val_index = 0;
    while (code > lead_byte_max)
    {
        val[val_index++] = (code & 0x3F) | 0x80;
        code >>= 6;
        lead_byte_max >>= (val_index == 1 ? 2 : 1);
    }
    val[val_index++] = (code & lead_byte_max) | (~lead_byte_max << 1);
    while (val_index-- && *s < end)
    {
        **s = val[val_index];
        (*s)++;
    }
}

// Returns 0 if no split was needed.
int split_into_surrogates(int code, int* surr1, int* surr2)
{
    if (code <= 0xFFFF)
        return 0;
    *surr2 = 0xDC00 | (code & 0x3FF);       // Save the low 10 bits.
    code >>= 10;                            // Drop the low 10 bits.
    *surr1 = 0xD800 | (code & 0x03F);       // Save the next 6 bits.
    *surr1 |= ((code & 0x7C0) - 0x40) << 6; // Insert the last 5 bits less 1.
    return 1;
}

// Expects to be used in a loop and see all code points in *code. Start *old at 0;
// this function updates *old for you - don't change it. Returns 0 when *code is
// the 1st of a surrogate pair; otherwise use *code as the final code point.
int join_from_surrogates(int* old, int* code)
{
    if (*old)
        *code = (((*old & 0x3FF) + 0x40) << 10) + (*code & 0x3FF);
    *old = ((*code & 0xD800) == 0xD800 ? *code : 0);
    return !(*old);
}

using phantom::int8;
using phantom::int16;
using phantom::int32;
using phantom::int64;
using phantom::uint8;
using phantom::uint16;
using phantom::uint32;
using phantom::uint64;

static String OperatorName(ast::Operator* a_pOperator)
{
    String n;
    if (a_pOperator->m_GreaterThan.hasValue())
        n = "operator>";
    else if (a_pOperator->m_OpBracket)
        n = "operator[]";
    else if (a_pOperator->m_OpParen)
        n = "operator()";
    else if (a_pOperator->m_RightShift)
        n = "operator>>";
    else if (a_pOperator->m_RightShiftAssign)
        n = "operator>>=";
    else
    {
        n = "operator";
        n += a_pOperator->m_ASSIGN.as_string();
    }
    return n;
}

static String NameToString(ast::Name* a_pName)
{
    if (a_pName->m_Operator)
        return OperatorName(a_pName->m_Operator);
    else
        return (a_pName->m_COMPL.hasValue() ? (String("~") + a_pName->m_IDENTIFIER.as_string())
                                            : String(a_pName->m_IDENTIFIER.as_string()));
}

CppLiteParser::CppLiteParser(Source* a_pSource, Message* a_pMessage, Flags a_Flags)
    : Parser(CppLite::Get(), a_pSource, a_pMessage), m_uiErrorCount(0), m_Flags(a_Flags), m_pPassData(nullptr)
{
    getSemantic()->setStandardLookup((a_Flags & e_Flag_StandardLookup) == e_Flag_StandardLookup);
}

CppLite* CppLiteParser::getLanguage() const
{
    return static_cast<CppLite*>(Parser::getLanguage());
}

Symbol* CppLiteParser::parseSymbolName(StringView a_strSymbol, LanguageElement* a_pScope, Modifiers a_Modifiers /*= 0*/,
                                       uint a_uiFlags /*= 0*/)
{
    return Application::Get()->findCppSymbol(a_strSymbol, a_pScope);
}

LanguageElement* CppLiteParser::parseElement(StringView a_strElem, LanguageElement* a_pScope,
                                             Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    return Application::Get()->findCppSymbol(a_strElem, a_pScope);
    // 	if (a_pScope == nullptr)
    //         a_pScope = Namespace::Global();
    //     LanguageElement* pElement = nullptr;
    //     {
    //         //             String exp = '$'+a_strExpression+'$';
    //         //             soul::CppLiteAnalyser driverExpression(exp);
    //         //             m_pAnalyser = &driverExpression;
    //         //             driverExpression.m_pParser = this;
    //         //             int result = driverExpression.parse();
    //         //             if(result != 0)
    //         //             {
    //         //                 m_pAnalyser = nullptr;
    //         //                 return nullptr;
    //         //             }
    //         //             astExpr = driverExpression.expression();
    //         //             CPPLITEPARSER_ASSERT(astExpr);
    //         //             pElement = parse(astExpr, a_pScope, a_Modifiers, a_uiFlags);
    //         //             m_pAnalyser = nullptr;
    //     }
    //     return pElement;
}

bool CppLiteParser::checkSymbolSyntax(StringView a_strSymbol) const
{
    //         String exp = '$' + a_strSymbol + '$';
    //         soul::CppLiteAnalyser driverExpression(exp);
    //         return driverExpression.parse() == 0;
    return false;
}
//
// struct CppLiteSourceByPasserGuard
// {
//     CppLiteSourceByPasserGuard(Semantic* a_pSemantic) : m_pSemantic(a_pSemantic)
//     {
//         m_pRemembered = m_pSemantic->getSource();
//         if (m_pRemembered)
//         {
//             m_pSemantic->setSource(nullptr);
//         }
//     }
//     ~CppLiteSourceByPasserGuard()
//     {
//         if (m_pRemembered)
//             m_pSemantic->setSource(m_pRemembered);
//     }
//     Source*   m_pRemembered;
//     Semantic* m_pSemantic;
// };

static bool CheckShouldWeContinueParsing(Symbol* a_pSymbol)
{
    if (a_pSymbol == nullptr)
        return false;
    return true;
}
static bool CheckShouldWeContinueParsing(Subroutine* a_pSubroutine)
{
    if (a_pSubroutine == nullptr)
        return false;
    return true;
}
/// IN DATA
#define CppLiteGetSource() (m_Data.getSource())
#define CppLiteGetLanguage() (m_Data.m_pParser->getLanguage())
#define CppLiteGetParser() (m_Data.m_pParser)
#define CppLiteHasFeature(feature) (m_Data.m_pParser->getLanguage()->getOption(feature) == "true")
#define CppLiteGetSemantic() (m_Data.m_pSemantic)
// #define CppLiteSourceByPasser() CppLiteSourceByPasserGuard __soul_source_bypasser(CppLiteGetSemantic());
#define CppLiteEndNamespaceLookup() CppLiteGetSemantic()->setSource(CppLiteGetParser()->getSource());
#define CppLiteGetScope() (m_Data.getScope())
#define NewInScope getSource()->New
#define NewConstant(...) Constant::Create(getSource(), __VA_ARGS__)
#define CppLiteGetScopeAs(type) (m_Data.getScope()->as##type())
#define CppLiteCheckShouldWeContinueParsing(s)                                                                         \
    if (!CheckShouldWeContinueParsing(s))                                                                              \
    return true
#define CppLiteDefaultReturnValue(value) bool soul_default_return_value = value

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define CppLiteConvNoCodeRange(...)                                                                                \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(CppLiteConvNoCodeRange_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))

#else
#    define CppLiteConvNoCodeRange(...)                                                                                \
        PHANTOM_PP_CAT(CppLiteConvNoCodeRange_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
//#   define o_findT(...) PHANTOM_PP_CAT(o_findT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#endif

#define CppLiteConv(...) CppLiteConvNoCodeRange(__VA_ARGS__)
#define CppLiteConvNoCodeRange_2(exp, type) CppLiteGetSemantic()->convert(exp, type, CppLiteGetScope())
#define CppLiteConvNoCodeRange_3(exp, type, scope) CppLiteGetSemantic()->convert(exp, type, scope)
#define CppLiteConvNoCodeRange_4(exp, type, convType, userConvFunc)                                                    \
    CppLiteConvNoCodeRange_5(exp, type, convType, userConvFunc, CppLiteGetScope())
#define CppLiteConvNoCodeRange_5(exp, type, convType, userConvFunc, scope)                                             \
    CppLiteGetSemantic()->convert(exp, type, convType, userConvFunc, scope)
#define CppLiteInitNE(...) CppLiteInitNENoCodeRange(__VA_ARGS__)
#define CppLiteInitNENoCodeRange(...) CppLiteGetSemantic()->initializeIfNotEqual(__VA_ARGS__)

#define CppLiteSetScope(elem) CppLiteSetScope_guard __CppLiteSetScope_guard(m_Data, elem)
#define CppLitePushScope(elem) m_Data.pushScope(elem)
#define CppLitePopScope() m_Data.popScope()
#define CppLiteGetTemplateArguments() m_Data.getTemplateArguments()
#define CppLiteGetFunctionArguments() m_Data.getFunctionArguments()
#define CppLiteGetInitializationType() m_Data.getInitializationType()
#define CppLiteGetTemplateSignature() m_Data.getTemplateSignature();
#define CppLiteGetAccess() m_Data.getAccess()
#define CppLiteSaveMask()                                                                                              \
    (((CppLiteGetAccess() != Access::Private) * PHANTOM_R_FILTER_FIELD) |                                              \
     ((CppLiteGetAccess() == Access::Public) * PHANTOM_R_FILTER_PROPERTY))

/// OUT DATA
#define CppLiteMap(elem) m_Data.mapAndReturnElement(input, elem)
#define CppLiteMapAndReturn(elem)                                                                                      \
    CppLiteMap(elem);                                                                                                  \
    return true
#define CppLiteHasMappedElement(type) m_Data.hasElement(input)
#define CppLiteGetElementAs(type) m_Data.findElement<type>(input, &LanguageElement::as##type)
#define CppLiteAddMetas(symbol, metas)                                                                                 \
    if (metas)                                                                                                         \
    m_Data.addMetas(symbol, metas)

#if PHANTOM_DEBUG_LEVEL != PHANTOM_DEBUG_LEVEL_FULL
#    define CppLitePrintErrorMsg(...)
#    define CppLitePrintWarningMsg(...)
#    define CppLitePrintDebugMsg(...)
#else
#    define CppLitePrintErrorMsg(...)                                                                                  \
        if (CppLiteGetParser()->getSource())                                                                           \
            printf("%s(l:%d | c:%d): error: ", CppLiteGetParser()->getSource()->getUniqueName().data(),                \
                   input->location().begin.line, input->location().begin.column);                                      \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n")
#    define CppLitePrintWarningMsg(...)                                                                                \
        if (CppLiteGetParser()->getSource())                                                                           \
            printf("%s(l:%d | c:%d): warning: ", CppLiteGetParser()->getSource()->getUniqueName().data(),              \
                   input->location().begin.line, input->location().begin.column);                                      \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n")
#    define CppLitePrintDebugMsg(...) fprintf(stderr, __VA_ARGS__)
#endif

#define CppLiteErrorMsg(...) CppLiteErrorMsgCodeRangeLocation(CppLiteCodeRangeLocation(input->location()), __VA_ARGS__);

#define CppLiteErrorMsgCodeRangeLocation(codeRangeLocation, ...)                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        CppLitePrintErrorMsg(__VA_ARGS__);                                                                             \
        if (CppLiteGetParser()->getMessage())                                                                          \
            CppLiteGetParser()->getMessage()->errorD(codeRangeLocation, __VA_ARGS__);                                  \
    } while (0)

#define CppLiteWarningMsg(...)                                                                                         \
    CppLiteWarningMsgCodeRangeLocation(CppLiteCodeRangeLocation(input->location()), __VA_ARGS__);

#define CppLiteWarningMsgCodeRangeLocation(codeRangeLocation, ...)                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        CppLitePrintWarningMsg(__VA_ARGS__);                                                                           \
        if (CppLiteGetParser()->getMessage())                                                                          \
            CppLiteGetParser()->getMessage()->warningD(codeRangeLocation, __VA_ARGS__);                                \
    } while (0)

/// ERRORS
#define CppLiteGuard(exp) CppLiteElementGuard __guard##exp(exp, m_Data);
#define CppLiteGuardScoped(exp) CppLiteElementGuardScoped __guard##exp(exp);
#define CppLitesGuardScoped(exp) CppLiteElementsGuardScoped __guard##exp(exp);
#define CppLiteUnguard(exp) __guard##exp.canceled = true;
#define CppLitesGuard(exps) auto __guard##exps = CppLiteCreateGuard(exps);
#define CppLitesUnguard(exps) __guard##exps.canceled = true;
#define CppLiteErrorReturnIf(cond, ...)                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        if (cond)                                                                                                      \
        {                                                                                                              \
            CppLiteErrorReturn(__VA_ARGS__);                                                                           \
        }                                                                                                              \
    } while (0)
#define CppLiteErrorIf(cond, ...)                                                                                      \
    if (cond)                                                                                                          \
    {                                                                                                                  \
        CppLiteErrorMsg(__VA_ARGS__);                                                                                  \
    }
namespace
{
void CppLiteErrorReturnIfNameUsed_ToBreak()
{
    printf("");
}
} // namespace

#define CppLiteErrorReturnIfNameUsed(scope, name)                                                                      \
    if ((scope)->hasSymbol((const char*)name))                                                                         \
    {                                                                                                                  \
        /*CppLiteErrorReturnIfNameUsed_ToBreak(); */                                                                   \
        CppLiteErrorReturn("'%s' : a symbol with this name already exists in this class", (const char*)(name));        \
    }

#define CppLiteBlockErrorIf(cond, ...)                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (cond)                                                                                                      \
        {                                                                                                              \
            CppLiteErrorMsg(__VA_ARGS__);                                                                              \
            _CppLiteBlockErrorIf_code
#define _CppLiteBlockErrorIf_code(...)                                                                                 \
    __VA_ARGS__                                                                                                        \
    }                                                                                                                  \
    }                                                                                                                  \
    while (0)
#define CppLiteErrorContinueIf(cond, ...)                                                                              \
    if (cond)                                                                                                          \
    {                                                                                                                  \
        CppLiteErrorMsg(__VA_ARGS__);                                                                                  \
        continue;                                                                                                      \
    }
#define CppLiteError(...) CppLiteErrorMsg(__VA_ARGS__)
#define CppLiteWarningIf(cond, ...)                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        if (cond)                                                                                                      \
        {                                                                                                              \
            CppLiteWarningMsg(__VA_ARGS__);                                                                            \
        }                                                                                                              \
    } while (0)
#define CppLiteSubError(...)                                                                                           \
    CppLiteGetParser()                                                                                                 \
    ->getMessage()                                                                                                     \
    ->getChild(CppLiteGetParser()->getMessage()->getChildCount() - 1)                                                  \
    ->error(__VA_ARGS__)
#define CppLiteErrorReturn(...)                                                                                        \
    CppLiteErrorMsg(__VA_ARGS__);                                                                                      \
    return soul_default_return_value

#define CppLiteVisitType(member, ...) resolveType(member, ##__VA_ARGS__)
#define CppLiteAddTypeDependency(                                                                                      \
member) // now handled directly in CallExpression / ConstructorCallExpression / FieldExpression
// if (!addTypeDep(input, member))
// return soul_default_return_value
#define CppLiteVisitElement(member, ...) resolveElement(member, ##__VA_ARGS__)
#define CppLiteVisitElements(members, ...) resolveElements(members, ##__VA_ARGS__)
#define CppLiteVisitSymbol(member, ...) resolveSymbol(member, ##__VA_ARGS__)
#define CppLiteVisitParameter(member, ...) resolveParameter(member, ##__VA_ARGS__)
#define CppLiteVisitExpression(member, ...) resolveExpression(member, ##__VA_ARGS__)
#define CppLiteOrphan(element)                                                                                         \
    {                                                                                                                  \
        CPPLITEPARSER_ASSERT((element)->getOwner() == nullptr);                                                        \
    }

#define CppLiteRuleToString(rule) CppLiteNamer::Name(m_Data, rule)

String CppLiteSubroutineName(Subroutine* pSubroutine)
{
    if (Method* pMethod = pSubroutine->asMethod())
    {
        if (pMethod->getProperty())
        {
            if (pMethod->getParameters().size())
            {
                return String(pMethod->getProperty()->getName()) + " @set";
            }
            else
                return String(pMethod->getProperty()->getName()) + " @get";
        }
    }
    return pSubroutine->getName();
}

#define CppLiteElementToString(elem) m_Data.elementToString(elem)

#define CppLiteConstantExpression(any) m_Data.createExpression(any)

Type* CppLiteScopeType(LanguageElement* a_pElement)
{
    CPPLITEPARSER_ASSERT(a_pElement);
    return a_pElement->asType() ? static_cast<Type*>(a_pElement)
                                : a_pElement->asExpression()
    ? static_cast<Expression*>(a_pElement)->getValueType()->removeAllQualifiers()->removeReference()
    : nullptr;
}
struct CppLitePassData;
struct CppLiteElementGuard
{
    template<class T>
    CppLiteElementGuard(T*& a_pElem, CppLitePassData& a_Data)
        : m_pElem((LanguageElement*&)a_pElem), canceled(false), m_Data(a_Data)
    {
    }
    ~CppLiteElementGuard();
    LanguageElement*& m_pElem;
    CppLitePassData&  m_Data;
    bool              canceled;
};
struct CppLiteElementGuardScoped
{
    template<typename T>
    CppLiteElementGuardScoped(T*& a_pElem) : m_pElem((LanguageElement*&)a_pElem), canceled(false)
    {
    }
    ~CppLiteElementGuardScoped()
    {
        //         if (!canceled && m_pElem)
        //         {
        //             if (m_pElem->getOwner() == nullptr)
        //                 Delete(m_pElem);
        //             else
        //             {
        //                 if (std::find(m_pElem->getOwner()->getElements().begin(),
        //                 m_pElem->getOwner()->getElements().end(),
        //                               m_pElem) == m_pElem->getOwner()->getElements().end())
        //                 {
        //                     m_pElem->getOwner()->removeScopedElement(m_pElem);
        //                     Delete(m_pElem);
        //                 }
        //             }
        //         }
    }
    LanguageElement*& m_pElem;
    bool              canceled;
};
struct CppLiteElementsGuardScoped
{
    template<typename T>
    CppLiteElementsGuardScoped(T& a_Elems) : m_Elems((LanguageElements&)a_Elems), canceled(false)
    {
    }
    ~CppLiteElementsGuardScoped()
    {
        //         for (auto m_pElem : m_Elems)
        //         {
        //             if (!canceled && m_pElem)
        //             {
        //                 if (m_pElem->getOwner() == nullptr)
        //                     Delete(m_pElem);
        //                 else
        //                 {
        //                     auto& elems = m_pElem->getOwner()->getElements();
        //                     if (std::find(elems.begin(), elems.end(), m_pElem) == elems.end())
        //                     {
        //                         m_pElem->getOwner()->removeScopedElement(m_pElem);
        //                         Delete(m_pElem);
        //                     }
        //                 }
        //             }
        //         }
    }
    LanguageElements& m_Elems;
    bool              canceled;
};
template<class T>
struct CppLiteElementsGuard
{
    CppLiteElementsGuard(T& a_Elems) : m_elems(a_Elems), canceled(false) {}
    ~CppLiteElementsGuard()
    {
        //         for (auto pElem : m_elems)
        //             if (!canceled && pElem && pElem->getOwner() == nullptr)
        //                 Delete(pElem);
    }
    T&   m_elems;
    bool canceled;
};

template<class T>
static CppLiteElementsGuard<T> CppLiteCreateGuard(T& vec)
{
    return CppLiteElementsGuard<T>(vec);
}

struct CppLitePassData
{
    CppLitePassData(CppLiteParser* a_pParser, BuildSession* a_pBuildSession, CppLiteGrammar::Allocator a_Allocator)
        : m_pSource(a_pParser->getSource()),
          m_pParser(a_pParser),
          m_pSemantic(a_pParser->getSemantic()),
          m_CppLiteGrammar(a_Allocator),
          m_pBuildSession(a_pBuildSession)
    {
        m_pSemantic->setNameFormatDelegate(
        Semantic::NameFormatDelegate(this, &CppLitePassData::semanticNameFormatDelegate));
        pushScope(m_pSource);
        pushTemplateArguments(nullptr);
        pushFunctionArguments(nullptr);
        pushFunctionElements(nullptr);
        pushInitializationType(nullptr);
        pushTemplateSignature(nullptr);
        pushLeftExpression(nullptr);
        pushImplicitThisExpression(nullptr);
        pushReturnAddress(&m_pFallBackReturnElement);

        m_pBreakFunc = CppLite::Get()->findFunction("phantom::lang::DebugBreak()");
        CPPLITEPARSER_ASSERT(m_pBreakFunc);

        m_pAssertHandlerFunc =
        CppLite::Get()->findFunction("phantom::lang::CppLiteAssert(const char*, const char*, int)");
        CPPLITEPARSER_ASSERT(m_pAssertHandlerFunc);
    }
    ~CppLitePassData()
    {
        CPPLITEPARSER_ASSERT(getTemplateArguments() == nullptr);
        CPPLITEPARSER_ASSERT(getFunctionArguments() == nullptr);
        CPPLITEPARSER_ASSERT(getFunctionElements() == nullptr);
        CPPLITEPARSER_ASSERT(getInitializationType() == nullptr);
        CPPLITEPARSER_ASSERT(getTemplateSignature() == nullptr);
        CPPLITEPARSER_ASSERT(getReturnedElement() == m_pFallBackReturnElement);
        popReturnAddress();
        popScope();
        popTemplateArguments();
        popFunctionArguments();
        popFunctionElements();
        popInitializationType();
        popTemplateSignature();
        popLeftExpression();
        popImplicitThisExpression();
        //
        //         for (auto pElem : m_Orphans)
        //         {
        //             CPPLITEPARSER_ASSERT(pElem->getOwner() == nullptr);
        //             Delete(pElem);
        //         }

        // delete orphan elements which could last here after errors happened

#if CPPLIGHT_ENABLED_DEBUG_LEAK_CHECKS
        std::set<LanguageElement*> elements;
        while (m_DebugCheckLeaks.size())
        {
            CPPLITEPARSER_ASSERT(findRtti(m_DebugCheckLeaks.back(), PHANTOM_CLASSOF(LanguageElement)).isNull() ||
                                 m_DebugCheckLeaks.back()->getOwner());
            m_DebugCheckLeaks.pop_back();
#    if CPPLIGHT_ENABLED_DEBUG_LEAK_HISTORY
            m_DebugCheckLeaksHistory.pop();
#    endif
        }
#endif
    }
#define CppLiteMapCodeRange(elem, ...) CppLiteSetCodeRange(elem, m_Data.CppLiteCodeRange(__VA_ARGS__))

    Source*     getSource() const { return m_pSource; }
    Semantic*   getSemantic() const { return m_pSemantic; }
    Expression* toExpression(LanguageElement* a_pElem, Expression* a_pLHS = nullptr) const
    {
        return m_pSemantic->toExpression(a_pElem, a_pLHS);
    }

    inline CodeRange CppLiteCodeRange(ast::_BaseRule* rule) { return CppLiteCodeRange(rule->location()); }
    inline CodeRange CppLiteCodeRange(ast::_BaseRule* begin, ast::_BaseRule* end)
    {
        return CppLiteCodeRange(begin->location(), end->location());
    }
    inline CodeRange CppLiteCodeRange(const spell::Location& loc)
    {
        return loc.isValid()
        ? CodeRange{CodePosition(loc.begin.line, loc.begin.column), CodePosition(loc.end.line, loc.end.column)}
        : CodeRange();
    }
    inline CodeRange CppLiteCodeRange(const spell::Location& begin, const spell::Location& end)
    {
        return begin.isValid() && end.isValid()
        ? CodeRange{CodePosition(begin.begin.line, begin.begin.column), CodePosition(end.end.line, end.end.column)}
        : CodeRange();
    }
    inline CodeRangeLocation CppLiteCodeRangeLocation(const spell::Location& loc)
    {
        return CodeRangeLocation(getSource()->getSource(), CppLiteCodeRange(loc));
    }
    void semanticErrorDelegate(CodeRangeLocation const& location, const char* text)
    {
        CppLitePrintDebugMsg("%s : %d | %d : %s\n", location.source->getName().data(), location.range.begin.line,
                             location.range.begin.column, text);
        if (m_pParser->getMessage())
            m_pParser->getMessage()->errorD(location, text);
    }

    void semanticNameFormatDelegate(LanguageElement* a_pElement, phantom::StringBuffer& a_Buf)
    {
        while (a_pElement->asSymbol() == nullptr)
        {
            a_pElement = a_pElement->getOwner();
        }
        a_pElement->getQualifiedDecoratedName(a_Buf);
    }

    String getSourceText(spell::Location loc)
    {
        if (loc.begin.column == -1 || loc.begin.line == -1 || loc.end.column == -1 || loc.end.line == -1)
            return String();
        SourceStream* pSourceStream = getSource()->getSource()->getSourceStream();
        if (pSourceStream == nullptr)
            return String();
        auto* in = pSourceStream->createInputStream();
        char* buf = (char*)malloc(4096);
        for (int i = 0; i < loc.begin.line - 1; ++i)
        {
            in->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        in->getline(buf, 4096);
        String text = buf;
        text = text.substr(loc.begin.column - 1);
        for (int i = 0; i < (loc.end.line - loc.begin.line - 1); ++i)
        {
            text += '\n';
            in->getline(buf, 4096);
            text += buf;
        }
        if (loc.end.line != loc.begin.line)
        {
            text += '\n';
            String lastLine;
            in->getline(buf, 4096);
            lastLine += buf;
            text += lastLine.substr(0, loc.end.column - 1);
        }
        else
        {
            text = text.substr(0, loc.end.column - loc.begin.column);
        }
        free(buf);
        pSourceStream->destroyInputStream(in);
        return text;
    }

    String getSourceText(ast::_BaseRule* rule) { return getSourceText(rule->location()); }
    void   errorDelegate(spell::Location a_loc, ast::EError a_eError, const char* msg)
    {
        if (m_pParser->getMessage())
        {
            CodeRangeLocation loc(getSource(),
                                  CodeRange{CodePosition(a_loc.begin.line, a_loc.begin.column),
                                            CodePosition(a_loc.end.line, a_loc.end.column)});
            switch (a_eError)
            {
            case ast::e_Error_ExpectedToken:
                CppLitePrintDebugMsg("%s : %d | %d : syntax : expected : '%s'\n", getSource()->getName().data(),
                                     a_loc.begin.line, a_loc.begin.column, msg);
                m_pParser->getMessage()->errorD(loc, "syntax : expected : '%s'", msg);
                break;
            case ast::e_Error_UnexpectedToken:
                CppLitePrintDebugMsg("%s : %d | %d : syntax : unexpected : '%s'\n", getSource()->getName().data(),
                                     a_loc.begin.line, a_loc.begin.column, msg);
                m_pParser->getMessage()->errorD(loc, "syntax : unexpected '%s'", msg);
                break;
            case ast::e_Error_UnknownToken:
                CppLitePrintDebugMsg("%s : %d | %d : syntax : bad token : '%s'\n", getSource()->getName().data(),
                                     a_loc.begin.line, a_loc.begin.column, msg);
                m_pParser->getMessage()->errorD(loc, "syntax : bad token", msg);
                break;
            case ast::e_Error_CannotReadInput:
                CppLitePrintDebugMsg("%s : cannot read input\n", getSource()->getName().data());
                m_pParser->getMessage()->errorD(loc, "cannot read input", msg);
                break;
            }
        }
    }

    void pushScope(LanguageElement* a_pElement) { m_ScopeStack.push_back(a_pElement); }
    void popScope() { m_ScopeStack.pop_back(); }

    void        pushLeftExpression(Expression* a_pElement) { m_LeftExpressionStack.push_back(a_pElement); }
    void        popLeftExpression() { m_LeftExpressionStack.pop_back(); }
    Expression* getLeftExpression() const { return m_LeftExpressionStack.back(); }

    void pushImplicitThisExpression(Expression* a_pElement) { m_ImplicitThisExpressionStack.push_back(a_pElement); }
    void popImplicitThisExpression() { m_ImplicitThisExpressionStack.pop_back(); }
    Expression* getImplicitThisExpression() const { return m_ImplicitThisExpressionStack.back(); }

    void         pushPackArgument(Placeholder* a_pParam) { m_PackArguments.push_back(a_pParam); }
    Placeholder* popPackArgument()
    {
        if (m_PackArguments.empty())
            return nullptr;
        auto pArg = m_PackArguments.back();
        m_PackArguments.pop_back();
        return pArg;
    }

    void       pushPackExpansionParameter(Parameter* a_pParam) { m_PackExpansionParameters.push_back(a_pParam); }
    Parameter* popPackExpansionParameter()
    {
        if (m_PackExpansionParameters.empty())
            return nullptr;
        auto pParam = m_PackExpansionParameters.back();
        m_PackExpansionParameters.pop_back();
        return pParam;
    }

    template<class T>
    T* mapElement(ast::_BaseRule* a_pRule, T* a_pElement)
    {
        if (a_pElement)
        {
            if (a_pElement->getOwner() == nullptr)
                m_DebugCheckLeaks.push_back(a_pElement);
#if CPPLIGHT_ENABLED_DEBUG_LEAK_HISTORY
            m_DebugCheckLeaksHistory.record("");
#endif
            m_RuleToElement[a_pRule].push_back(a_pElement);
        }
        return a_pElement;
    }
    template<class T, size_t N>
    void mapElements(ast::_BaseRule* a_pRule, phantom::SmallVector<T, N>& a_Elements)
    {
        for (auto pElement : a_Elements)
        {
            if (pElement->getOwner() == nullptr)
            {
                m_DebugCheckLeaks.push_back(pElement);
#if CPPLIGHT_ENABLED_DEBUG_LEAK_HISTORY
                m_DebugCheckLeaksHistory.record("");
#endif
            }
            m_RuleToElement[a_pRule].push_back(pElement);
        }
    }
    void clearMappedElement(LanguageElement* a_pElement)
    {
        for (auto it = m_RuleToElement.begin(); it != m_RuleToElement.end(); ++it)
        {
            auto found = std::find(it->second.begin(), it->second.end(), a_pElement);
            if (found != it->second.end())
            {
                it->second.erase(found);
                if (it->second.empty())
                    m_RuleToElement.erase(it);
                return;
            }
        }
    }
    template<class T>
    T* mapAndReturnElement(ast::_BaseRule* a_pRule, T* a_pElement)
    {
        mapElement(a_pRule, a_pElement);
        returnElement(a_pElement);
        if (a_pElement && !(a_pElement->getCodePosition().isValid()) &&
            !m_pParser->hasFlag(CppLiteParser::e_Flag_NoCodeLocation))
            a_pElement->setCodeRange(CppLiteCodeRange(a_pRule->location()));
        return a_pElement;
    }

    const LanguageElements& findElements(ast::_BaseRule* a_pRule)
    {
        static thread_local LanguageElements s_LanguageElements;
        auto                                 found = m_RuleToElement.find(a_pRule);
        return found == m_RuleToElement.end() ? s_LanguageElements : found->second;
    }

    LanguageElement* findElement(ast::_BaseRule* a_pRule)
    {
        auto found = m_RuleToElement.find(a_pRule);
        return found == m_RuleToElement.end() ? nullptr : found->second.back();
    }

    bool hasElement(ast::_BaseRule* a_pRule) const { return m_RuleToElement.find(a_pRule) != m_RuleToElement.end(); }

    template<class T>
    T* findElement(ast::_BaseRule* a_pRule, T* (LanguageElement::*asX)() const)
    {
        auto found = m_RuleToElement.find(a_pRule);
        return found == m_RuleToElement.end() ? (T*)nullptr
                                              : found->second.back() ? (found->second.back()->*asX)() : nullptr;
    }

    void pushReturnAddress(LanguageElement** a_ppElement) { m_ReturnStack.push_back(a_ppElement); }

    void popReturnAddress() { m_ReturnStack.pop_back(); }

    void pushTemplateArguments(LanguageElements* a_pElements) { m_TemplateArguments.push_back(a_pElements); }

    void pushFunctionArguments(Expressions* a_pExpressions) { m_FunctionArguments.push_back(a_pExpressions); }

    void pushFunctionElements(LanguageElements* a_pElements) { m_FunctionElements.push_back(a_pElements); }

    void pushInitializationType(Type* a_pType)
    {
        CPPLITEPARSER_ASSERT(m_InitializationTypes.size() < 256);
        m_InitializationTypes.push_back(a_pType);
    }

    void popInitializationType() { m_InitializationTypes.pop_back(); }

    void popTemplateArguments() { m_TemplateArguments.pop_back(); }

    void popFunctionArguments() { m_FunctionArguments.pop_back(); }

    void popFunctionElements() { m_FunctionElements.pop_back(); }

    void pushTemplateSignature(TemplateSignature* a_pTS) { m_TemplateSignatures.push_back(a_pTS); }

    void popTemplateSignature() { m_TemplateSignatures.pop_back(); }

    String elementToString(LanguageElement* a_pElement)
    {
        if (Symbol* pSymbol = a_pElement->asSymbol())
            return elementToString(pSymbol);
        return "<unnamed>";
    }

    String elementToString(Symbol* a_pSymbol) { return a_pSymbol->getUniqueName(); }

    LanguageElement* elementToScope(LanguageElement* a_pElement)
    {
        if (Expression* pExp = a_pElement->asExpression())
        {
            return pExp->getValueType()->removeReference()->removeQualifiers();
        }
        return a_pElement;
    }

    size_t readCharLit(char* str, const char* cstr, size_t& offset)
    {
        if (cstr[offset++] == '\\')
        {
            switch (cstr[offset++])
            {
            case 'x':
            {
                char ch;
                if (sscanf(&cstr[2], "%2x", (unsigned int*)&ch) != 1)
                    return 0;
                offset += 2;
                str[0] = ch;
                return 1;
            }

            case 'u':
            {
                int codePoint = 0;
                if (sscanf(&cstr[2], "%4x", &codePoint) != 1)
                    return 0;
                char buffer[4];
                memset(buffer, 0, sizeof(buffer));
                char* pEnd = buffer;
                encode_code_point(&pEnd, pEnd + 4, codePoint);

                char* pIt = buffer;

                int i = 0;
                for (; pIt < pEnd; ++pIt)
                {
                    str[i++] = *pIt;
                }
                offset += 4; // '\\' 'u' 'N''N''N''N'
                return i;
            }
            break;

            case 'U':
            {
                int codePoint = 0;
                if (sscanf(&cstr[2], "%8x", &codePoint) != 1)
                    return 0;
                char buffer[8];
                memset(buffer, 0, sizeof(buffer));
                char* pEnd = buffer;
                encode_code_point(&pEnd, pEnd + 8, codePoint);

                char* pIt = buffer;

                int i = 0;
                for (; pIt < pEnd; ++pIt)
                {
                    str[i++] = *pIt;
                }
                offset += 8;
                return i;
            }
            break;

            case '0':
                str[0] = '\0';
                break;

            case 'n':
                str[0] = '\n';
                break;

            case 't':
                str[0] = '\t';
                break;

            case 'r':
                str[0] = '\r';
                break;

            case 'b':
                str[0] = '\b';
                break;

            case 'a':
                str[0] = '\a';
                break;

            case 'v':
                str[0] = '\v';
                break;

            case 'f':
                str[0] = '\f';
                break;

            case '\\':
                str[0] = '\\';
                break;

            default:
                str[0] = cstr[1];
                break;
            }
        }
        else
        {
            str[0] = cstr[0];
        }
        return 1;
    }

    String unquote(const char* cstr)
    {
        String      value = cstr;
        const char* lit = value.data();
        size_t      len = value.size();
        CPPLITEPARSER_ASSERT(lit[0] == '"');
        CPPLITEPARSER_ASSERT(lit[len - 1] == '"');
        char*  str = (char*)(lit + 1);
        size_t size = 0;
        for (size_t i = 0; i < (len - 2);)
        {
            size_t offset = 0;
            size_t count = readCharLit(str + size, lit + i + 1, offset);
            CPPLITEPARSER_ASSERT(count != 0, "invalid String formatting");
            i += offset;
            size += count;
        }
        return String(str, size);
    }

    Expression* createExpression(spell::any any)
    {
        auto pSrc = getSource();
        switch (any.type())
        {
        case spell::any::e_undefined:
            return nullptr;
        case spell::any::e_cstring:
        {
            const char* cstr = any.as_string();
            if (cstr[0] == '\'') // char literal
            {
                char   ch[4];
                size_t offset = 0;
                size_t s = readCharLit(ch, cstr + 1, offset);
                switch (s)
                {
                case 1:
                    return NewInScope<ConstantExpression>(NewConstant(ch[0]));
#if PHANTOM_HAS_BUILT_IN_CHAR16_T
                case 2:
                    return NewInScope<ConstantExpression>(NewConstant(*(char16_t*)&ch));
#elif PHANTOM_HAS_BUILT_IN_WCHAR_T
                case 2:
                    return NewInScope<ConstantExpression>(NewConstant(*(wchar_t*)&ch));
#else
                case 2:
                    return NewInScope<ConstantExpression>(NewConstant(*(short*)&ch));
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR32_T
                case 4:
                    return NewInScope<ConstantExpression>(NewConstant(*(char32_t*)&ch));
#else
                case 4:
                    return NewInScope<ConstantExpression>(NewConstant(*(int*)&ch));
#endif
                default:
                    CPPLITEPARSER_ASSERT(false);
                }
                return nullptr;
            }
            else // double quoted String
            {
                return NewInScope<StringLiteralExpression>(cstr, unquote(cstr));
            }
        }
        break;

        case spell::any::e_char:
            return NewInScope<ConstantExpression>(NewConstant(any.as_char()));
        case spell::any::e_uchar:
            return NewInScope<ConstantExpression>(NewConstant(any.as_uchar()));
        case spell::any::e_int:
            return NewInScope<ConstantExpression>(NewConstant(any.as_int()));
        case spell::any::e_uint:
            return NewInScope<ConstantExpression>(NewConstant(any.as_uint()));
        case spell::any::e_short:
            return NewInScope<ConstantExpression>(NewConstant(any.as_short()));
        case spell::any::e_ushort:
            return NewInScope<ConstantExpression>(NewConstant(any.as_ushort()));
        case spell::any::e_long:
            return NewInScope<ConstantExpression>(NewConstant(any.as_long()));
        case spell::any::e_ulong:
            return NewInScope<ConstantExpression>(NewConstant(any.as_ulong()));
        case spell::any::e_longlong:
            return NewInScope<ConstantExpression>(NewConstant(any.as_longlong()));
        case spell::any::e_ulonglong:
            return NewInScope<ConstantExpression>(NewConstant(any.as_ulonglong()));
        case spell::any::e_double:
            return NewInScope<ConstantExpression>(NewConstant(any.as_double()));
        case spell::any::e_float:
            return NewInScope<ConstantExpression>(NewConstant(any.as_float()));
        case spell::any::e_bool:
            return NewInScope<ConstantExpression>(NewConstant(any.as_bool()));
        default:
            CPPLITEPARSER_ASSERT(false);
            return nullptr;
        }
    }

    Label* getBreakLabel() { return this->m_BreakLabels.empty() ? nullptr : this->m_BreakLabels.top(); }

    Label* getContinueLabel() { return m_ContinueLabels.empty() ? nullptr : this->m_ContinueLabels.top(); }

    Label* pushBreakLabel()
    {
        Label* pLabel = NewInScope<Label>("break", Label::e_Tag_Break);
        this->m_BreakLabels.push(pLabel);
        return pLabel;
    }

    Label* pushContinueLabel()
    {
        Label* pLabel = NewInScope<Label>("continue", Label::e_Tag_Continue);
        this->m_ContinueLabels.push(pLabel);
        return pLabel;
    }

    void popBreakLabel(Label* a_pLabel)
    {
        CPPLITEPARSER_ASSERT(a_pLabel == getBreakLabel());
        this->m_BreakLabels.pop();
    }

    void popContinueLabel(Label* a_pLabel)
    {
        CPPLITEPARSER_ASSERT(a_pLabel == getContinueLabel());
        this->m_ContinueLabels.pop();
    }

    Label* findOrCreateLabel(Subroutine* pSubroutine, StringView name)
    {
        Label*& rpLabel = this->m_LabelMap[pSubroutine][name];
        if (rpLabel == nullptr)
            return (rpLabel = NewInScope<Label>(name));
        else
            return rpLabel;
    }

    void returnElement(LanguageElement* a_pElement)
    {
        CPPLITEPARSER_ASSERT(m_ReturnStack.size());
        *m_ReturnStack.back() = a_pElement;
    }

    LanguageElement* getReturnedElement() const { return *m_ReturnStack.back(); }

    LanguageElement*   getScope() const { return m_ScopeStack.back(); }
    LanguageElements*  getTemplateArguments() const { return m_TemplateArguments.back(); }
    Expressions*       getFunctionArguments() const { return m_FunctionArguments.back(); }
    LanguageElements*  getFunctionElements() const { return m_FunctionElements.back(); }
    Type*              getInitializationType() const { return m_InitializationTypes.back(); }
    Type*&             getInitializationType() { return m_InitializationTypes.back(); }
    TemplateSignature* getTemplateSignature() const { return m_TemplateSignatures.back(); }

    Access getAccess() const
    {
        LanguageElement* pScope = getScope();
        if (pScope->asClassType())
        {
            return static_cast<ClassType*>(pScope)->getCurrentAccess();
        }
        return Access::Undefined;
    }

    ast::MixinStatementDefinition* getMixinStatementDefinition(const char* a_IDENTIFIER);

    BuildSession*    m_pBuildSession{};
    Source*          m_pSource;
    ast::Source*     m_pAstSource;
    ast::Expression* m_pAstExpression;
    CppLiteParser*   m_pParser;
    Semantic*        m_pSemantic;
    CppLiteGrammar   m_CppLiteGrammar;
    LanguageElement* m_pFallBackReturnElement;
    Function*        m_pAssertHandlerFunc;
    Function*        m_pBreakFunc;

    std::stack<Label*>                               m_BreakLabels;
    std::stack<Label*>                               m_ContinueLabels;
    std::map<Subroutine*, std::map<String, Label*>>  m_LabelMap;
    std::map<String, ast::MixinStatementDefinition*> m_MixinStatementDefinitions;

    // out
    phantom::SmallVector<LanguageElement**> m_ReturnStack;

    // in
    phantom::SmallVector<LanguageElement*>   m_ScopeStack;
    phantom::SmallVector<Expression*>        m_LeftExpressionStack;
    phantom::SmallVector<Expression*>        m_ImplicitThisExpressionStack;
    phantom::SmallVector<Placeholder*>       m_PackArguments;
    phantom::SmallVector<Parameter*>         m_PackExpansionParameters;
    phantom::SmallVector<LanguageElements*>  m_TemplateArguments;
    phantom::SmallVector<LanguageElements*>  m_FunctionElements;
    phantom::SmallVector<TemplateSignature*> m_TemplateSignatures;
    phantom::SmallVector<Type*, 256>         m_InitializationTypes;
    phantom::SmallVector<Expressions*>       m_FunctionArguments;

    std::map<ast::_BaseRule*, LanguageElements> m_RuleToElement;
    phantom::SmallVector<LanguageElement*>      m_DebugCheckLeaks;
#if CPPLIGHT_ENABLED_DEBUG_LEAK_HISTORY
    phantom::History m_DebugCheckLeaksHistory;
#endif
};

struct BreakLabelGuard
{
    CppLitePassData& d;
    Label*           l;
    Label*           get() const { return l; }
    BreakLabelGuard(CppLitePassData& d) : d(d) { l = d.pushBreakLabel(); }
    ~BreakLabelGuard() { d.popBreakLabel(l); }
};

struct ContinueLabelGuard
{
    CppLitePassData& d;
    Label*           l;
    Label*           get() const { return l; }
    ContinueLabelGuard(CppLitePassData& d) : d(d) { l = d.pushContinueLabel(); }
    ~ContinueLabelGuard() { d.popContinueLabel(l); }
};

CppLiteElementGuard::~CppLiteElementGuard()
{
    //     if (!canceled && m_pElem && m_pElem->getOwner() == nullptr)
    //     {
    //         m_Data.clearMappedElement(m_pElem);
    //         m_Data.m_Orphans.push_back(m_pElem);
    //     }
}

ast::MixinStatementDefinition* CppLitePassData::getMixinStatementDefinition(const char* a_IDENTIFIER)
{
    auto& defs = m_MixinStatementDefinitions;
    auto  found = defs.find(a_IDENTIFIER);
    if (found != defs.end())
        return found->second;
    for (auto import : getSource()->getImports())
    {
        ArrayView<Source*> sources;
        Source*            pAsSource = import.symbol->asSource();
        if (pAsSource)
        {
            sources = ArrayView<Source*>(&pAsSource, 1);
        }
        else if (Package* pck = import.symbol->asPackage())
        {
            sources = pck->getSources();
        }
        for (auto pSource : sources)
        {
            auto pCompiled = Compiler::Get()->getCompiledSource(pSource);
            if (pCompiled->getLanguage()->getName() == "CppLite")
            {
                auto& defs =
                static_cast<CppLiteParser*>(pCompiled->getParser())->m_pPassData->m_MixinStatementDefinitions;
                found = defs.find(a_IDENTIFIER);
                if (found != defs.end())
                    return found->second;
            }
        }
    }
    return nullptr;
}

struct CppLiteSetScope_guard
{
    CppLiteSetScope_guard(CppLitePassData& a_Data, LanguageElement* a_pElement) : m_Data(a_Data)
    {
        m_Data.pushScope(a_pElement);
    }
    ~CppLiteSetScope_guard() { m_Data.popScope(); }
    CppLitePassData& m_Data;
};

template<class T>
struct CppLitePass : public ast::visitor::Recursive<T>
{
    typedef ast::visitor::Recursive<T> base_type;
    CppLitePass(CppLitePassData& a_Data) : m_Data(a_Data) {}
    CppLitePassData& m_Data;
    bool             soul_default_return_value = true;

    inline LanguageElement* getScope() const { return m_Data.getScope(); }
    inline Source*          getSource() const { return m_Data.getSource(); }

    template<class T, class... Args>
    T* New(Args&&... a_Args)
    {
        return m_Data.getSource()->New<T>(std::forward<Args>(a_Args)...);
    }

    void Delete(LanguageElement* a_pElem) { m_Data.getSource()->Delete(a_pElem); }

    template<class T>
    Constant* createConstant(T _val)
    {
        return Constant::Create<T>(getSource(), _val);
    }

    LocalVariable* createLocalVariable(Block* a_pBlock, Type* a_pType, StringView a_Name)
    {
        auto pLoc = a_pBlock->New<LocalVariable>(a_pType, a_Name);
        if (!a_pType->isNative())
            if (auto pClassType = a_pType->asClassType())
                if (!pClassType->isTemplateDependant())
                    m_Data.m_pSemantic->buildClass(pClassType, Semantic::e_ClassBuildState_Sized);
        return pLoc;
    }

    bool resolveOverrideAndFinal(ast::_BaseRule* input, const char* name,
                                 phantom::SmallVector<spell::any, 4ull> const& _kws, Modifiers& modifiers)
    {
        CppLiteDefaultReturnValue(false);
        for (auto& qu : _kws)
        {
            if (qu.as_string()[0] == 'o')
            {
                CppLiteErrorReturnIf((modifiers & PHANTOM_R_OVERRIDE) == PHANTOM_R_OVERRIDE,
                                     "'%s' : 'override' already used", name);
                CppLiteErrorReturnIf((modifiers & PHANTOM_R_STATIC) == PHANTOM_R_STATIC,
                                     "'%s' : 'static' is not compatible with 'override'", name);
                //             CppLiteErrorReturnIf((modifiers & PHANTOM_R_VIRTUAL) == 0,
                //                               "'%s' : 'override' method must be 'virtual'", name);
                modifiers |= PHANTOM_R_OVERRIDE;
            }
            else
            {
                CppLiteErrorReturnIf((modifiers & PHANTOM_R_FINAL) == PHANTOM_R_FINAL, "'%s' : 'final' already used",
                                     name);
                CppLiteErrorReturnIf((modifiers & PHANTOM_R_STATIC) == PHANTOM_R_STATIC,
                                     "'%s' : 'static' is not compatible with 'final'", name);
                //             CppLiteErrorReturnIf((modifiers & PHANTOM_R_VIRTUAL) == 0,
                //                               "'%s' : 'override' method must be 'virtual'", name);
                modifiers |= PHANTOM_R_FINAL;
            }
        }
        return true;
    }

    inline void CppLiteSetCodeRange(LanguageElement* a_pElem, const CodeRange& a_Range)
    {
        if (!m_Data.m_pParser->hasFlag(CppLiteParser::e_Flag_NoCodeLocation))
            a_pElem->setCodeRange(a_Range);
    }

    inline CodeRange CppLiteCodeRange(const spell::Location& loc)
    {
        return CodeRange{CodePosition(loc.begin.line, loc.begin.column), CodePosition(loc.end.line, loc.end.column)};
    }
    inline CodeRange CppLiteCodeRangeNoEnd(const spell::Location& loc)
    {
        return CodeRange{CodePosition(loc.begin.line, loc.begin.column),
                         CodePosition(loc.begin.line, loc.begin.column)};
    }
    inline CodeRangeLocation CppLiteCodeRangeLocation(const spell::Location& loc)
    {
        return CodeRangeLocation(m_Data.getSource(), CppLiteCodeRange(loc));
    }

    Expression* toExpression(LanguageElement* a_pElem, Expression* a_pLHS = nullptr) const
    {
        return m_Data.toExpression(a_pElem, a_pLHS);
    }

    bool resolveMetaDecl(ast::MetaDecl* input, Symbol* a_pSymbol)
    {
        Expression* pExp = nullptr;
        if (input->m_Value)
        {
            pExp = CppLiteVisitExpression(input->m_Value);
            if (!pExp)
                return true;
            CppLiteErrorReturnIf(pExp->getValueType()->isVoid(), "[[,...]] : invalid void meta data value type");
            CppLiteErrorReturnIf(!pExp->getValueType()->isCopyable(),
                                 "[\"\"=...] : invalid non-copyable meta data value type");
            CppLiteErrorReturnIf(!pExp->isCompileTime(),
                                 "[\"\"=...]: invalid non-compile-time meta data value expression");
        }
        a_pSymbol->setMetaData(m_Data.unquote(input->m_Key), pExp ? pExp->get() : Variant{});
        return true;
    }

    bool resolveMetaOrAnnotations(ast::MetaOrAnnotations* metaOrAnnotations, Symbol* a_pSymbol)
    {
        for (ast::MetaOrAnnotation* pMetaOrAnn : *(metaOrAnnotations))
        {
            if (pMetaOrAnn->m_Meta)
                resolveMeta(pMetaOrAnn->m_Meta, a_pSymbol);
            else
                resolveAnnotation(pMetaOrAnn->m_Annotation, a_pSymbol);
        }
        return true;
    }
    bool resolveMeta(ast::Meta* input, Symbol* a_pSymbol)
    {
        for (auto pDecl : *input->m_MetaDecls)
        {
            resolveMetaDecl(pDecl, a_pSymbol);
        }
        return true;
    }
    bool resolveAnnotation(ast::Annotation* input, Symbol* a_pSymbol)
    {
        CppLiteDefaultReturnValue(false);

        Expressions expressions;
        if (strcmp(input->m_IDENTIFIER, "property") == 0)
        {
            Class* pClass = CppLiteGetScopeAs(Class);
            CppLiteErrorReturnIf(pClass == nullptr, "property : annotated member must be a method");
            Method* pMethod = a_pSymbol->asMethod();
            CppLiteErrorReturnIf(pMethod == nullptr, "property : annotated member must be a method");
            StringView methodName = pMethod->getName();
            StringView propertyName = methodName;
            //          bool setter = false;
            //          if (propertyName.size() > 3 && (::isupper(propertyName[3]) || propertyName[3] == '_'))
            //          {
            //              if (propertyName.find("set") == 0)
            //              {
            //                  setter = true;
            //              }
            //          }
            Type* pValueType = nullptr;
            bool  isSetter = false;
            if (pMethod->getReturnType()->isVoid())
            {
                CppLiteErrorReturnIf(pMethod->getParameters().size() != 1,
                                     "property : expected one parameter for setter");
                CppLiteErrorReturnIf(pMethod->isConst(), "property : illegal const qualifier for setter");
                pValueType = pMethod->getParameters().front()->getValueType();
                CppLiteErrorReturnIf(pValueType->asReference() && !pValueType->asConstLValueReference(),
                                     "property : non-const references are not allowed");
                CppLiteErrorReturnIf(!pValueType->removeConstLValueReference()->isCopyable(),
                                     "property : non-copyable value type");
                isSetter = true;
            }
            else
            {
                CppLiteErrorReturnIf(!pMethod->isConst(), "property : const qualifier expected for getter");
                CppLiteErrorReturnIf(!pMethod->getParameters().empty(), "property : expected no parameter for getter");
                pValueType = pMethod->getReturnType();
                CppLiteErrorReturnIf(pValueType->asReference() && !pValueType->asConstLValueReference(),
                                     "property : non-const references are not allowed");
                CppLiteErrorReturnIf(!pValueType->removeConstLValueReference()->isCopyable(),
                                     "property : non-copyable type");
            }
            Property* pProperty = CppLiteGetScopeAs(Class)->getProperty(propertyName);
            if (pProperty == nullptr)
            {
                pProperty = pClass->New<Property>(pValueType, propertyName);
                pClass->addProperty(pProperty);
            }
            else
            {
                CppLiteErrorReturnIf(pProperty->getValueType()->removeConstLValueReference() !=
                                     pValueType->removeConstLValueReference(),
                                     "property : value type incompatible with previous declaration");
            }

            if (isSetter)
            {
                CppLiteErrorReturnIf(pProperty->getSet() != nullptr, "property : setter already defined");
                pProperty->setSet(pMethod);
            }
            else
            {
                CppLiteErrorReturnIf(pProperty->getGet() != nullptr, "property : getter already defined");
                pProperty->setGet(pMethod);
            }

            pProperty->setAccess(CppLiteGetAccess());
        }
        else if (strcmp(input->m_IDENTIFIER, "transient") == 0)
        {
            if (Field* pField = a_pSymbol->asField())
            {
                CppLiteErrorReturnIf(pField->testModifiers(Modifier::Transient),
                                     "transient : already defined for field '%.*s'",
                                     PHANTOM_STRING_AS_PRINTF_ARG(pField->getName()));
                pField->addModifiers(Modifier::Transient);
            }
            else if (Method* pMethod = a_pSymbol->asMethod())
            {
                if (Property* pProp = pMethod->getProperty())
                {
                    CppLiteErrorReturnIf(pProp->testModifiers(Modifier::Transient),
                                         "transient : already defined for property '%.*s'",
                                         PHANTOM_STRING_AS_PRINTF_ARG(pProp->getName()));
                    pProp->addModifiers(Modifier::Transient);
                }
            }
            else if (Property* pProp = a_pSymbol->asProperty())
            {
                CppLiteErrorReturnIf(pProp->testModifiers(Modifier::Transient),
                                     "transient : already defined for property '%.*s'",
                                     PHANTOM_STRING_AS_PRINTF_ARG(pProp->getName()));
                pProp->addModifiers(Modifier::Transient);
            }
        }
        else if (strcmp(input->m_IDENTIFIER, "mask") == 0)
        {
        }
        //         if (input->m_AssignExpressions)
        //         {
        //             for (auto pAExp : *input->m_AssignExpressions)
        //             {
        //                 Expression* pExp = CppLiteVisitExpression(pAExp, m_Data.getSource());
        //                 if (pExp == nullptr)
        //                     return true;
        //                 expressions.push_back(pExp);
        //             }
        //             Type* pType = CppLiteVisitType(input->m_Type, m_Data.getSource());
        //             CppLiteErrorReturnIf(pType == nullptr, "extension : undefined type");
        //             Class* pClass = pType->asClass();
        //             CppLiteErrorReturnIf(pClass == nullptr || pClass->isA(PHANTOM_CLASSOF(SymbolExtension)),
        //                                  "type must be a SymbolExtension derived class");
        //             // pClass->getConstructor pType->allocate()
        //         }
        return true;
    }

    /// public: protected: private:
    bool traverseAccessSpecifier(ast::AccessSpecifier* input)
    {
        ClassType* pClassType = CppLiteGetScopeAs(ClassType);
        if (pClassType == nullptr)
            return true;
        switch (input->m_PUBLIC[2])
        {
        case 'b':
            pClassType->setCurrentAccess(Access::Public);
            break;
        case 'o':
            pClassType->setCurrentAccess(Access::Protected);
            break;
        case 'i':
            pClassType->setCurrentAccess(Access::Private);
            break;
        default:
            CPPLITEPARSER_ASSERT(false);
        }
        return true;
    }

    /// @transient | @filter(0x1)
    //     bool resolveMemberSpecifier(ast::MemberSpecifier* input, Symbol* a_pSymbol, bool& a_bSaveApplied)
    //     {
    //         CppLiteDefaultReturnValue(false);
    //         if (a_pSymbol)
    //         {
    //             if (input->m_FilterSpecifier)
    //             {
    //                 if (input->m_FilterSpecifier->m_Expression)
    //                 {
    //                     CppLiteErrorReturnIf(a_bSaveApplied, "@save : already specified");
    //                     a_bSaveApplied = true;
    //                     Expression* pExpression = CppLiteVisitExpression(input->m_FilterSpecifier->m_Expression);
    //                     CppLiteErrorReturnIf(
    //                     pExpression == nullptr || !(pExpression->isCompileTime()) ||
    //                     !(pExpression->getEvalType()->removeReference()->removeQualifiers()->asIntegralType()),
    //                     "@filter : value must be a compile-time integral constant, "
    //                     "preferrably an hexadecimal mask or an enum for clarity");
    //                     pExpression = CppLiteConv(pExpression, PHANTOM_TYPEOF(size_t));
    //                     CPPLITEPARSER_ASSERT(pExpression);
    //                     uint serializationMask = 0;
    //                     pExpression->load(&serializationMask);
    //                     CPPLITEPARSER_ASSERT(a_pSymbol->asValueMember());
    //                     ValueMember* pMember = static_cast<ValueMember*>(a_pSymbol);
    //                     pMember->setFilterMask(serializationMask);
    //                 }
    //             }
    //             else
    //             {
    //                 // versionning
    //             }
    //         }
    //         return true;
    //     }

    bool resolveCallList(ast::CallList* a_pList, Expressions& a_Expressions, LanguageElement* a_pScope)
    {
        m_Data.pushInitializationType(nullptr);
        if (a_pList->m_AssignExpressionOrInitializerLists)
        {
            for (auto expr : *a_pList->m_AssignExpressionOrInitializerLists)
            {
                Expression* pExpr = CppLiteVisitExpression(expr, a_pScope);
                if (pExpr == nullptr)
                {
                    m_Data.popInitializationType();
                    return false;
                }
                a_Expressions.push_back(pExpr);
            }
        }
        m_Data.popInitializationType();
        return true;
    }

    bool resolveCallList(ast::CallList* a_pList, Expressions& a_Expressions)
    {
        return resolveCallList(a_pList, a_Expressions, m_Data.getScope());
    }

    Expression* resolveGenericCall(ast::_BaseRule* input, LanguageElement* a_pLHS, const char* a_Identifier,
                                   ast::TemplateArgumentList* a_pTemplateArgs, ast::CallList* a_pCallList)
    {
        CppLiteDefaultReturnValue(false);
        if (!a_pCallList)
            return nullptr;

        Expressions args;
        CppLitesGuard(args);

        LanguageElements templateArgs;
        CppLitesGuard(templateArgs);

        if (!resolveTemplateArguments(a_pTemplateArgs, templateArgs))
            return nullptr;

        if (!resolveCallList(a_pCallList, args))
            return nullptr;

        return CppLiteGetSemantic()->solveGenericCall(a_pLHS, a_Identifier, templateArgs, MakeArrayView(args),
                                                      CppLiteGetScope(), CppLiteGetInitializationType());
    }

    bool resolveTemplateArguments(phantom::SmallVector<ast::TemplateArgument*> const& a_TArgs,
                                  LanguageElements&                                   a_Arguments)
    {
        for (auto pTA : a_TArgs)
        {
            LanguageElement* pArg = CppLiteVisitElement(pTA);
            if (pArg == nullptr)
                return false;
            a_Arguments.push_back(pArg);
        }
        return true;
    }

    bool resolveTemplateArguments(phantom::SmallVector<ast::TemplateArgument*> const& a_TArgs,
                                  LanguageElement* a_pScope, LanguageElements& a_Arguments)
    {
        m_Data.pushScope(a_pScope);
        bool result = resolveTemplateArguments(a_TArgs, a_Arguments);
        m_Data.popScope();
        return result;
    }

    bool resolveTemplateArguments(ast::TemplateArgumentList* a_pList, LanguageElements& a_Arguments)
    {
        SmallVector<ast::TemplateArgument*> templateArgs;
        if (a_pList->m_TemplateArgumentListSplit->m_NotLastTemplateArguments)
        {
            for (ast::TemplateArgument* arg : *a_pList->m_TemplateArgumentListSplit->m_NotLastTemplateArguments)
            {
                templateArgs.push_back(arg);
            }
        }
        auto last = a_pList->m_TemplateArgumentListSplit->m_LastTemplateArgument;
        templateArgs.push_back(last);
        return resolveTemplateArguments(templateArgs, a_Arguments);
    }

    bool resolveTemplateArguments(ast::TemplateArgumentList* a_pList, LanguageElement* a_pScope,
                                  LanguageElements& a_Arguments)
    {
        m_Data.pushScope(a_pScope);
        bool result = resolveTemplateArguments(a_pList, a_Arguments);
        m_Data.popScope();
        return result;
    }

    template<class TypeOrAuto>
    bool resolveFunctionEnd(TypeOrAuto* pTypeOrAuto, const char* name, soul::ast::FunctionEnd* input,
                            Modifiers& modifiers, Subroutine*& a_pSubroutine, bool a_bOperator)
    {
        auto* pScope = CppLiteGetScope();
        CPPLITEPARSER_ASSERT(pScope);

        Parameters              params;
        ClassType*              pClassType = nullptr;
        Type*                   pType = nullptr;
        TemplateSpecialization* pTSpec = nullptr;
        {
            if (auto ts = m_Data.getTemplateSignature())
            {
                m_Data.pushScope(ts);
            }

            auto scopeExit = phantom::makeScopeExit([&]() {
                if (auto ps = m_Data.getTemplateSignature())
                    m_Data.popScope();
            });

            auto pScopeElem = pScope->asLanguageElement();

            if ((pTSpec = pScopeElem->asTemplateSpecialization()))
                pClassType = pTSpec->getNamingScope()->asClassType();
            else
                pClassType = pScopeElem->asClassType();

            if (pTypeOrAuto)
            {
                pType = CppLiteVisitType(pTypeOrAuto);
                if (pType == nullptr)
                    return false;
            }

            CppLiteDefaultReturnValue(false);

            // signature
            if (!resolveParameters(input, input->m_Parameters, params))
                return false;
        }

        Signature* pSignature = New<Signature>(pType, params);

        if (a_bOperator)
        {
            BuiltInOperator* pBinOp = CppLite::Get()->getBuiltInBinaryOperator(name + 8);
            BuiltInOperator* pPreOp = CppLite::Get()->getBuiltInPreUnaryOperator(name + 8);
            BuiltInOperator* pPostOp = CppLite::Get()->getBuiltInPostUnaryOperator(name + 8);
            CppLiteErrorReturnIf(pBinOp && pPreOp == nullptr && pSignature->getParameterCount() != 1,
                                 "'%s' : binary operator expects a single parameter, %zu have been declared", name,
                                 pSignature->getParameterCount());
            CppLiteErrorReturnIf(pPreOp && pBinOp == nullptr && pSignature->getParameterCount() != 0,
                                 "'%s' : prefix operator does not expect any parameter, %zu have been declared", name,
                                 pSignature->getParameterCount());
            CppLiteErrorReturnIf(
            pPostOp && (pPostOp->getId() == Operator::PostDecrement || pPostOp->getId() == Operator::PostIncrement) &&
            pSignature->getParameterCount() != 1,
            "'%s' : postfix operator expects one parameter, %zud have been declared", name,
            pSignature->getParameterCount());
            CppLiteErrorReturnIf(pPostOp && pBinOp == nullptr && pSignature->getParameterCount() != 0,
                                 "'%s' : postfix operator does not expect any parameter, %zu have been declared", name,
                                 pSignature->getParameterCount());
            CppLiteErrorReturnIf(name[8] == '=' && pClassType == nullptr,
                                 "'%s' : illegal declaration of assignment operator outside class/structure scope",
                                 name);
            CppLiteErrorReturnIf(
            name[8] == '=' && pSignature->getParameterCount() == 1 &&
            (pSignature->getParameterType(0)->isSame(pClassType->addConst()->addLValueReference()) ||
             pSignature->getParameterType(0)->isSame(pClassType)) &&
            pClassType->getCopyAssignmentOperator(),
            "'%s' : copy assignment operator already declared", name);
            CppLiteErrorReturnIf(name[8] == '=' && pSignature->getParameterCount() == 1 &&
                                 pSignature->getParameterType(0)->isSame(pClassType->addRValueReference()) &&
                                 pClassType->getMoveAssignmentOperator(),
                                 "'%s' : move assignment operator already declared", name);
        }

        Modifiers refQualifiers = 0;
        // function
        uint flags = PHANTOM_R_NONE;
        // abstract ?
        if (input->m_Suffix.hasValue())
        {
            const char* defaultOrDelete = nullptr;
            if (input->m_Suffix.type() == spell::any::e_int)
            {
                CppLiteErrorReturnIf(a_bOperator, "'%s' : illegal non-zero value after '='", name);
                CppLiteErrorReturnIf(input->m_Suffix.as_int() != 0, "'%s' : ilegal non-zero value after '='", name);
                CPPLITEPARSER_ASSERT((modifiers & Modifier::PureVirtual) != Modifier::PureVirtual);
                CppLiteErrorReturnIf((modifiers & Modifier::Virtual) == 0,
                                     "'%s' : pure virtual method must be 'virtual'", name);
                modifiers |= Modifier::PureVirtual;
            }
            else if (strcmp(input->m_Suffix.as_string(), defaultOrDelete = "delete") == 0)
            {
                modifiers |= Modifier::Deleted;
            }
            else if (strcmp(input->m_Suffix.as_string(), defaultOrDelete = "default") == 0)
            {
                modifiers |= Modifier::Defaulted;
            }
            else
            {
                CPPLITEPARSER_ASSERT(false);
            }
            if (modifiers & (Modifier::Deleted | Modifier::Defaulted))
            {
                CppLiteErrorReturnIf(!(a_bOperator) || name[8] != '=' || pClassType == nullptr,
                                     "'%s' : '%s' can only be used on default/copy/move constructors "
                                     "or assignment operators",
                                     name, defaultOrDelete);
                CppLiteErrorReturnIf(
                !(pSignature->getReturnType()->isTemplateDependant()) &&
                !(pSignature->getReturnType()->isSame(pClassType->addLValueReference())),
                "'%s' : illegal assignment operator return type, expects '%.*s'", name,
                PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType->addLValueReference())));
                CppLiteErrorReturnIf(
                pSignature->getParameterCount() != 1 ||
                (!(pSignature->getParameterType(0)->isTemplateDependant()) &&
                 !(pSignature->getParameterType(0)->isSame(pClassType->addConst()->addLValueReference())) &&
                 !(pSignature->getParameterType(0)->isSame(pClassType->addRValueReference()))),
                "'%s' : '%s' invalid copy/move assignment operator signature, expects '%.*s' or "
                "'%.*s'",
                name, defaultOrDelete,
                PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType->addConst()->addLValueReference())),
                PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType->addRValueReference())));
            }
        }
        // override ?

        else if (input->m_OVERRIDEs && !input->m_OVERRIDEs->empty())
        {
            resolveOverrideAndFinal(input, name, *input->m_OVERRIDEs, modifiers);
        }
        // ref qualifiers
        if (input->m_CONSTs && input->m_CONSTs->size())
        {
            CppLiteErrorReturnIf(modifiers & PHANTOM_R_STATIC,
                                 "'%s' : 'static' is not compatible with '&/&&/const/volatile' ref-qualifiers", name);
            for (auto val : *input->m_CONSTs)
            {
                const char* cstr = val.as_string();
                switch (cstr[0])
                {
                case '&':
                {
                    if (cstr[1] == '&')
                    {
                        CppLiteErrorReturnIf((refQualifiers & PHANTOM_R_RVALUEREF) != 0,
                                             "'%s' : rvalue ref-qualifier && already declared", name);
                        refQualifiers |= PHANTOM_R_RVALUEREF;
                    }
                    else
                    {
                        CppLiteErrorReturnIf((refQualifiers & PHANTOM_R_LVALUEREF) != 0,
                                             "'%s' : lvalue ref-qualifier & already declared", name);
                        refQualifiers |= PHANTOM_R_LVALUEREF;
                    }
                }
                break;
                case 'c':
                {
                    CppLiteErrorReturnIf((refQualifiers & PHANTOM_R_CONST) != 0,
                                         "'%s' : const ref-qualifier already declared", name);
                    refQualifiers |= PHANTOM_R_CONST;
                }
                break;
                case 'v':
                {
                    CppLiteErrorReturnIf((refQualifiers & PHANTOM_R_VOLATILE) != 0,
                                         "'%s' : volatile ref-qualifier already declared", name);
                    refQualifiers |= PHANTOM_R_VOLATILE;
                }
                break;
                default:
                    CPPLITEPARSER_ASSERT(false);
                    break;
                }
            }
        }
        pSignature->addModifiers(refQualifiers);
        modifiers |= refQualifiers;
        Types types = pSignature->getParameterTypes();

        Subroutines subroutines;

        MetaDatas metas;
        a_pSubroutine = nullptr;
        if ((modifiers & PHANTOM_R_STATIC) != 0)
        {
            Function* pFunction = NewInScope<Function>(name, pSignature, modifiers, flags);
            a_pSubroutine = pFunction;
        }
        else
        {
            Method* pMethod = NewInScope<Method>(name, pSignature, modifiers, flags);
            pMethod->setAccess(CppLiteGetAccess());
            CppLiteErrorReturnIf(pClassType == nullptr, "'%s' : method can only be declared at class type scope", name);
            if ((modifiers & PHANTOM_R_VIRTUAL) != 0)
            {
                CppLiteErrorReturnIf(pClassType->asClass() == nullptr,
                                     "'%s' : virtual method can only be declared at class scope", name);
            }
            a_pSubroutine = pMethod;
            pMethod->createThis(pClassType);
        }
        if (pTSpec)
            pTSpec->setTemplated(a_pSubroutine);
        // CppLiteAddMetas(pFunction);
        for (auto& m : metas)
        {
            a_pSubroutine->setMetaData(m.first, m.second);
        }
        return true;
    }

    template<class Declarator>
    bool resolveDeclarator(Type*& a_pType, Declarator* input, Expression*& a_pInitExpression, bool types, bool inits)
    {
        CppLiteDefaultReturnValue(false);
        const char* name = input->m_IDENTIFIER;
        if (types)
        {
            if (input->m_TypeExtents)
            {
                for (auto pExt : *input->m_TypeExtents)
                {
                    m_Data.pushInitializationType(a_pType);
                    a_pType = CppLiteVisitType(pExt);
                    m_Data.popInitializationType();
                    CppLiteErrorReturnIf(a_pType == nullptr, "'%s' : invalid type qualification, extent or derivation",
                                         name);
                }
            }
            if (input->m_ArrayExtents)
            {
                for (size_t arrExtIdx = 0; arrExtIdx < input->m_ArrayExtents->size(); ++arrExtIdx)
                {
                    auto pExt = (*input->m_ArrayExtents)[arrExtIdx];
                    m_Data.pushInitializationType(a_pType);
                    a_pType = CppLiteVisitType(pExt);
                    m_Data.popInitializationType();
                    CppLiteErrorReturnIf(a_pType == nullptr, "'%s' : invalid array extent", name);
                    CPPLITEPARSER_ASSERT(a_pType->getTypeKind() == TypeKind::Array);
                    Array* pArray = static_cast<Array*>(a_pType);
                    if (Object::Cast<TemplateDependantArray>(pArray) == nullptr)
                    {
                        if (pArray->getItemCount() == 0) // Something[]
                        {
                            size_t initCount = 0;
                            if (input->m_ArrayExtents->size() > 1 && (arrExtIdx == input->m_ArrayExtents->size() - 1))
                            {
                                a_pType = pArray->getItemType()->makePointer();
                            }
                            else if (auto pInitList = static_cast<ast::InitializerList*>(input->m_InitializerList))
                            {
                                initCount = (pInitList->m_AssignExpressionOrInitializerLists == nullptr)
                                ? 0
                                : pInitList->m_AssignExpressionOrInitializerLists->size();
                            }
                            else if (auto assEOrInit = input->m_AssignExpressionOrInitializerList)
                            {
                                if (assEOrInit->type() == ast::e_RuleType_InitializerList)
                                {
                                    if (auto pInitList =
                                        static_cast<ast::InitializerList*>(input->m_AssignExpressionOrInitializerList))
                                    {
                                        initCount = (pInitList->m_AssignExpressionOrInitializerLists == nullptr)
                                        ? 0
                                        : pInitList->m_AssignExpressionOrInitializerLists->size();
                                    }
                                }
                            }
                            if (initCount == 0)
                                a_pType = pArray->getItemType()->makePointer();
                            else
                                a_pType = pArray->getItemType()->makeArray(initCount);
                        }
                    }
                }
            }
        }
        if (inits)
        {
            if (input->m_InitializerList)
            {
                ast::InitializerList* pInitList = static_cast<ast::InitializerList*>(input->m_InitializerList);

                if (Array* pArray = a_pType->asArray())
                {
                    size_t itemCount = pArray->getItemCount();
                    size_t initCount = (pInitList->m_AssignExpressionOrInitializerLists == nullptr)
                    ? 0
                    : pInitList->m_AssignExpressionOrInitializerLists->size();
                    if (itemCount == 0)
                    {
                        CppLiteErrorReturnIf(initCount == 0, "'%s' : illegal zero sized array", name);
                        itemCount = initCount;
                        a_pType = pArray = pArray->getItemType()->makeArray(itemCount);
                    }
                    CppLiteErrorReturnIf(itemCount < initCount,
                                         "'%s' : too many initializers in array initialization expression", name);
                    Expressions exps;
                    CppLitesGuard(exps);
                    Class* pItemClass = pArray->getItemType()->asClass();
                    if (itemCount > initCount)
                    {
                        if (pItemClass)
                        {
                            auto pCtor = pItemClass->getDefaultConstructor();
                            CppLiteErrorReturnIf(pCtor == nullptr,
                                                 "'%s' : no default constructor available for "
                                                 "array item initialization",
                                                 name);
                            CppLiteErrorReturnIf(
                            CppLiteGetSemantic()->isSymbolAccessible(pCtor, CppLiteGetScope()),
                            "'%s' : default constructor not accessible in '%.*s'", name,
                            PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(CppLiteGetScope())));
                        }
                    }

                    for (size_t i = 0; i < itemCount; ++i)
                    {
                        if (i < initCount)
                        {
                            Expression* pExp =
                            CppLiteVisitExpression((*pInitList->m_AssignExpressionOrInitializerLists)[i]);
                            if (pExp == nullptr)
                                return true;
                            Expression* pConv = CppLiteInitNE(pExp, pArray->getItemType(), CppLiteGetScope());
                            if (pConv == nullptr)
                                return true;
                            exps.push_back(pConv);
                        }
                        else if (pItemClass)
                        {
                            exps.push_back(
                            CppLiteGetScope()->New<ConstructorCallExpression>(pItemClass->getDefaultConstructor()));
                        }
                    }
                    CppLitesUnguard(exps);
                    CppLiteMapAndReturn(NewInScope<ArrayExpression>(pArray, exps));
                }

                // empty expressions
                else if (pInitList->m_AssignExpressionOrInitializerLists == nullptr ||
                         pInitList->m_AssignExpressionOrInitializerLists->empty())
                {
                    if (a_pType->isTemplateDependant())
                    {
                        a_pInitExpression =
                        NewInScope<TemplateDependantExpression>(NewInScope<TemplateDependantElement>(
                        nullptr, a_pType->getName(), phantom::NullOpt, phantom::ArrayView<LanguageElement*>()));
                        return true;
                    }
                    else if (std::is_same<Declarator, ast::AutoDeclarator>::value)
                    {
                        // auto
                        CppLiteErrorMsg("'%s' : in a direct-list-initialization context the type "
                                        "for 'auto' can "
                                        "only be deduced from a single initializer expression",
                                        name);
                    }
                    else
                    {
                        // non-auto
                        ClassType* pClassType = a_pType->asClassType();
                        if (pClassType)
                        {
                            CppLiteErrorReturnIf(pClassType->isAbstract(),
                                                 "'%.*s' : class is abstract and cannot be instanciated",
                                                 PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType)));
                            CppLiteErrorReturnIf(pClassType->getDefaultConstructor() == nullptr,
                                                 "'%s' : no default constructor available for type '%.*s'", name,
                                                 PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType)));
                            a_pInitExpression =
                            NewInScope<ConstructorCallExpression>(pClassType->getDefaultConstructor());
                            return true;
                        }
                        else
                        {
                            CppLiteErrorReturnIf(a_pType->asReference(),
                                                 "'%s' : no default constructor available for type '%.*s'", name,
                                                 PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType)));
                        }
                    }
                }
                // multiple expressions
                else if (pInitList->m_AssignExpressionOrInitializerLists->size() != 1)
                {
                    // auto
                    if (std::is_same<Declarator, ast::AutoDeclarator>::value)
                    {
                        CppLiteErrorMsg("'%s' : in a direct-list-initialization context the type "
                                        "for 'auto' can "
                                        "only be deduced from a single initializer expression",
                                        name);
                    }
                    // non auto
                    else
                    {
                        m_Data.pushInitializationType(a_pType);
                        a_pInitExpression = CppLiteVisitExpression(pInitList);
                        m_Data.popInitializationType();
                        return a_pInitExpression != nullptr;
                    }
                }
                // single expression
                else
                {
                    // auto
                    if (std::is_same<Declarator, ast::AutoDeclarator>::value)
                    {
                        a_pInitExpression =
                        CppLiteVisitExpression(pInitList->m_AssignExpressionOrInitializerLists->back());
                        return a_pInitExpression != nullptr;
                    }
                    // non auto
                    else
                    {
                        m_Data.pushInitializationType(a_pType);
                        a_pInitExpression = CppLiteVisitExpression(pInitList);
                        m_Data.popInitializationType();
                        return a_pInitExpression != nullptr;
                    }
                }
            }
            else if (input->m_CallList)
            {
                auto* pCallList = input->m_CallList;

                // empty expressions
                if (pCallList->m_AssignExpressionOrInitializerLists == nullptr ||
                    pCallList->m_AssignExpressionOrInitializerLists->empty())
                {
                    if (a_pType->isTemplateDependant())
                    {
                        a_pInitExpression =
                        NewInScope<TemplateDependantExpression>(NewInScope<TemplateDependantElement>(
                        nullptr, a_pType->getName(), phantom::NullOpt, phantom::ArrayView<LanguageElement*>()));
                        return true;
                    }
                    else if (std::is_same<Declarator, ast::AutoDeclarator>::value)
                    {
                        // auto
                        CppLiteErrorMsg("'%s' : in an auto declaration context, an expression in the "
                                        "construction-list is expected to deduce the underlying type",
                                        name);
                        return false;
                    }
                    else
                    {
                        // non-auto
                        ClassType* pClassType = a_pType->asClassType();
                        if (pClassType)
                        {
                            CppLiteErrorReturnIf(pClassType->isAbstract(),
                                                 "'%s' : class is abstract and cannot be instanciated",
                                                 PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType)));
                            CppLiteErrorReturnIf(pClassType->getDefaultConstructor() == nullptr,
                                                 "'%s' : no default constructor available for type '%.*s'", name,
                                                 PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType)));
                            a_pInitExpression =
                            NewInScope<ConstructorCallExpression>(pClassType->getDefaultConstructor());
                            return true;
                        }
                        else
                        {
                            CppLiteErrorReturnIf(a_pType->asReference(), "'%s' : references must be initialized", name);
                            return true;
                        }
                    }
                }
                // multiple expressions
                else if (pCallList->m_AssignExpressionOrInitializerLists->size() > 1)
                {
                    if (std::is_same<Declarator, ast::AutoDeclarator>::value)
                    {
                        // auto
                        CppLiteErrorMsg("'%s' : in an auto declaration context, only a single expression in "
                                        "the "
                                        "construction-list is expected to deduce the underlying type",
                                        name);
                        return false;
                    }

                    // non auto
                    else
                    {
                        ClassType* pClassType = a_pType->asClassType();
                        if (pClassType)
                        {
                            Expressions expressions;
                            CppLitesGuard(expressions);
                            if (!resolveCallList(pCallList, expressions))
                                return false;
                            LanguageElement* pElement = CppLiteGetSemantic()->qualifiedLookup(
                            pClassType, pClassType->getName(), phantom::NullOpt, MakeArrayView(expressions),
                            CppLiteGetScope(), nullptr, 0);
                            if (pElement == nullptr)
                                return true;
                            //                             CppLiteErrorReturnIf(pElement == nullptr,
                            //                                                  "'%s' : no constructor available taking
                            //                                                  the given arguments",
                            //                                                  pClassType->getName().data());
                            CPPLITEPARSER_ASSERT(pElement->asExpression());
                            a_pInitExpression = static_cast<Expression*>(pElement);
                            return true;
                        }
                        else
                        {
                            CppLiteErrorMsg("'%s' : unexpected multiple expressions in value initialization", name);
                            return false;
                        }
                        return a_pInitExpression != nullptr;
                    }
                }
                // single expression
                else
                {
                    if (std::is_same<Declarator, ast::AutoDeclarator>::value)
                    {
                        a_pInitExpression =
                        CppLiteVisitExpression(pCallList->m_AssignExpressionOrInitializerLists->back());
                        return a_pInitExpression != nullptr;
                    }
                    else
                    {
                        Expression* pInitExpression =
                        CppLiteVisitExpression(pCallList->m_AssignExpressionOrInitializerLists->back());
                        if (pInitExpression == nullptr)
                            return true;
                        CppLiteGuard(pInitExpression);
                        Expression* pConv =
                        CppLiteInitNE(pInitExpression, a_pType, UserDefinedFunctions::All, CppLiteGetScope());
                        if (pConv == nullptr)
                            return true;
                        a_pInitExpression = pConv;
                        return a_pInitExpression != nullptr;
                    }
                }
            }
            else if (input->m_AssignExpressionOrInitializerList)
            {
                a_pInitExpression = CppLiteVisitExpression(input->m_AssignExpressionOrInitializerList);
                return a_pInitExpression != nullptr;
            }
            else
            {
                if (a_pType->isTemplateDependant())
                {
                    if (auto pTdti = Object::Cast<TemplateDependantTemplateInstance>(a_pType))
                    {
                        a_pInitExpression =
                        NewInScope<TemplateDependantExpression>(NewInScope<TemplateDependantElement>(
                        nullptr, a_pType->getName(), MakeArrayView(pTdti->getArguments()),
                        phantom::ArrayView<LanguageElement*>()));
                        return true;
                    }
                    else
                    {
                        a_pInitExpression =
                        NewInScope<TemplateDependantExpression>(NewInScope<TemplateDependantElement>(
                        nullptr, a_pType->getName(), phantom::NullOpt, phantom::ArrayView<LanguageElement*>()));
                        return true;
                    }
                }
                else if (std::is_same<Declarator, ast::AutoDeclarator>::value)
                {
                    CPPLITEPARSER_ASSERT(false);
                }
                else
                {
                    // non-auto
                    ClassType* pClassType = a_pType->asClassType();
                    if (pClassType)
                    {
                        CppLiteErrorReturnIf(pClassType->isAbstract(),
                                             "'%.*s' : class is abstract and cannot be instanciated",
                                             PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType)));
                        CppLiteErrorReturnIf(pClassType->getDefaultConstructor() == nullptr,
                                             "'%s' : no default constructor available for type '%.*s'", name,
                                             PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType)));
                        a_pInitExpression = NewInScope<ConstructorCallExpression>(pClassType->getDefaultConstructor());
                        return true;
                    }
                    else
                    {
                        CppLiteErrorReturnIf(a_pType->asReference(), "'%s' : references must be initialized", name);
                        return true;
                    }
                }
            }
        }
        return true;
    }

    /// <class T, int A...>
    bool traverseTemplateSignature(ast::TemplateSignature* input)
    {
        TemplateSignature* pTemplateSignature = NewInScope<TemplateSignature>();
        bool               bPack = false;
        if (!input->m_TemplateParameters)
        {
            CppLiteMapAndReturn(pTemplateSignature);
            return true;
        }
        CppLiteGuard(pTemplateSignature);
        size_t lasti = input->m_TemplateParameters->size() - 1;
        for (size_t i = 0; i < input->m_TemplateParameters->size(); ++i)
        {
            auto        pTP = (*input->m_TemplateParameters)[i];
            bool        isLastParameter = (lasti == i);
            const char* name = pTP->m_TemplateTypeParameter ? pTP->m_TemplateTypeParameter->m_IDENTIFIER
                                                            : pTP->m_TemplateValueParameter->m_IDENTIFIER;

            bool variadic = false;
            auto handleEllipse = [=, &variadic](auto&& tparam) {
                if (pTemplateSignature->isVariadic() && tparam->m_ELLIPSE.hasValue())
                {
                    CppLiteError("'%s' : template parameter pack already declared", name);
                    return false;
                }
                if (pTemplateSignature->isVariadic() && !tparam->m_ELLIPSE.hasValue())
                {
                    CppLiteError("'%s' : template parameter pack must be the last template "
                                 "parameter in a variadic template signature",
                                 name);
                    return false;
                }

                if (tparam->m_ELLIPSE.hasValue())
                {
                    if (!isLastParameter)
                    {
                        CppLiteError("parameter pack must be the last parameter");
                        return false;
                    }
                    if (pTP->m_TemplateParameterDefault)
                    {
                        CppLiteError("parameter pack cannot have default argument");
                        return false;
                    }
                    variadic = true;
                }
                return true;
            };

            if (pTP->m_TemplateTypeParameter) // class ..
            {
                if (!handleEllipse(pTP->m_TemplateTypeParameter))
                    continue;

                LanguageElement* pDefaultType = nullptr;
                if (pTP->m_TemplateParameterDefault)
                {
                    pTemplateSignature->setOwner(CppLiteGetScope());
                    pDefaultType = CppLiteVisitType(pTP->m_TemplateParameterDefault, pTemplateSignature);
                    CppLiteErrorReturnIf(pDefaultType == nullptr,
                                         "'%s' : invalid template parameter default argument type", name);
                    pTemplateSignature->setOwner(nullptr);
                }
                CppLiteErrorContinueIf(
                pTemplateSignature->getTemplateParameter(pTP->m_TemplateTypeParameter->m_IDENTIFIER) != nullptr,
                "'%s' : a template parameter already exists with the same name", name);
                pTemplateSignature->addTemplateParameter(NewInScope<TemplateParameter>(
                NewInScope<PlaceholderType>(pTP->m_TemplateTypeParameter->m_IDENTIFIER), pDefaultType));
            }
            else // int ..
            {
                if (!handleEllipse(pTP->m_TemplateValueParameter))
                    continue;

                pTemplateSignature->setOwner(CppLiteGetScope());
                Type* pType = pTP->m_TemplateValueParameter->m_FundamentalType
                ? CppLiteVisitType(pTP->m_TemplateValueParameter->m_FundamentalType)
                : CppLiteVisitType(pTP->m_TemplateValueParameter->m_QualifiedName);
                pTemplateSignature->setOwner(nullptr);

                CppLiteErrorReturnIf(pType == nullptr, "invalid template value parameter type");
                CppLiteErrorReturnIf(!(pType->asIntegralType()),
                                     "template value parameter type must be an integral type");
                Expression* pDefaultExpression = nullptr;
                CppLiteGuard(pDefaultExpression);
                if (pTP->m_TemplateParameterDefault)
                {
                    pTemplateSignature->setOwner(CppLiteGetScope());
                    pDefaultExpression = CppLiteVisitExpression(pTP->m_TemplateParameterDefault, pTemplateSignature);
                    CppLiteErrorReturnIf(pDefaultExpression == nullptr,
                                         "'%s' : invalid template parameter default argument expression", name);
                    CppLiteErrorReturnIf(!(pDefaultExpression->isCompileTime()) &&
                                         !(pDefaultExpression->isTemplateDependant()),
                                         "'%s' : template parameter default argument must be a "
                                         "compile-time constant expression",
                                         name);
                    Expression* pConvExpression =
                    CppLiteConv(pDefaultExpression, pType, CppLiteGetScopeAs(LanguageElement));
                    if (pConvExpression == nullptr)
                        return true;
                    pDefaultExpression = pConvExpression;
                    pTemplateSignature->setOwner(nullptr);
                }
                pTemplateSignature->addTemplateParameter(NewInScope<TemplateParameter>(
                NewInScope<PlaceholderConstant>(pType, pTP->m_TemplateValueParameter->m_IDENTIFIER),
                pDefaultExpression));
                CPPLITEPARSER_ASSERT(pDefaultExpression == nullptr || pDefaultExpression->getOwner());
            }
            if (variadic)
                pTemplateSignature->setVariadic(true);
        }
        CppLiteUnguard(pTemplateSignature);
        CppLiteMapAndReturn(pTemplateSignature);
    }

    /// (<type>) varX = X, varY = Y
    bool traverseDeclarator(ast::Declarator* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);

        // check existing local var
        CppLiteErrorReturnIf(pBlock->getLocalVariable(input->m_IDENTIFIER) != nullptr,
                             "local variable '%s' already declared in this block", input->m_IDENTIFIER);
        CppLiteWarningIf(pBlock->getLocalVariableCascade(input->m_IDENTIFIER),
                         "local variable '%s' hides previously declared one", input->m_IDENTIFIER);

        Type*& pDeclType = CppLiteGetInitializationType();

        Expression* pInitExpression = nullptr;
        CppLiteGuard(pInitExpression);
        if (!resolveDeclarator(pDeclType, input, pInitExpression, true, true))
        {
            Type* pType = CppLiteGetInitializationType();
            if (!pType || pType->hasAuto()) // if auto deduction required, fallback to int
                pType = PHANTOM_TYPEOF(int);
            // still add a local variable for minimizing error output
            LocalVariable* pLVErr = NewInScope<LocalVariable>(pType, input->m_IDENTIFIER);
            CppLiteSetCodeRange(pLVErr, CppLiteCodeRange(input->location()));
            addLocalVariable(pBlock, pLVErr);
            return true;
        }
        Type*&         rpType = CppLiteGetInitializationType();
        LocalVariable* pLocalVariable = NewInScope<LocalVariable>(rpType, input->m_IDENTIFIER);
        CppLiteSetCodeRange(pLocalVariable, CppLiteCodeRange(input->location()));
        addLocalVariable(pBlock, pLocalVariable);
        Statement* pStatement;
        if (pInitExpression == nullptr && (CppLiteHasFeature("ZeroInit")))
        {
            pInitExpression = CppLiteGetSemantic()->createZeroInitExpression(pLocalVariable->getValueType());
        }
        if (pInitExpression)
        {
            if (!(pInitExpression->getValueType()->removeQualifiers()->isSame(pLocalVariable->getValueType())))
            {
                Expression* pConv = CppLiteConv(pInitExpression, pLocalVariable->getValueType(), CppLiteGetScope());
                if (pConv == nullptr)
                    return false;
                pInitExpression = pConv;
            }

            pStatement = NewInScope<LocalVariableInitializationStatement>(pLocalVariable, pInitExpression);
            CppLiteMapCodeRange(pStatement, input);
            pBlock->addStatement(pStatement);
            CppLiteMapAndReturn(pInitExpression);
        }
        return true;
    }

    /// (<auto>) varX = X, varY = Y
    bool traverseAutoDeclarator(ast::AutoDeclarator* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);
        CppLiteErrorReturnIfNameUsed(pBlock, input->m_IDENTIFIER);
        Type*       pType = CppLiteGetInitializationType();
        Expression* pInitExpression = nullptr;
        CppLiteGuard(pInitExpression);
        if (!resolveDeclarator(pType, input, pInitExpression, true, true))
            return true;
        CPPLITEPARSER_ASSERT(pInitExpression);
        if (!pInitExpression->isTemplateDependant())
        {
            pType = CppLiteGetSemantic()->autoDeduction(pType, pInitExpression->getValueType());
        }
        if (!pType)
            return true;
        LocalVariable* pLocalVariable = NewInScope<LocalVariable>(pType, input->m_IDENTIFIER);
        CppLiteSetCodeRange(pLocalVariable, CppLiteCodeRange(input->location()));
        if (pInitExpression->getValueType() != pType)
        {
            pInitExpression = CppLiteInitNE(pInitExpression, pType, pBlock);
            CPPLITEPARSER_ASSERT(pInitExpression, "conversion must always work, must be a l-value transformation");
        }
        CppLiteErrorReturnIf(pBlock->getLocalVariable(pLocalVariable->getName()),
                             "local variable '%s' already declared in this block", input->m_IDENTIFIER);
        CppLiteWarningIf(pBlock->getLocalVariableCascade(pLocalVariable->getName()),
                         "local variable '%s' hides previously declared one", input->m_IDENTIFIER);

        addLocalVariable(pBlock, pLocalVariable);
        Statement* pStatement;
        pStatement = NewInScope<LocalVariableInitializationStatement>(
        pLocalVariable,
        pLocalVariable->getValueType()->asReference()
        ? pInitExpression // const& or & or && => we keep the reference if any
        : (pInitExpression = pInitExpression->loadRValueReferenceExpression()) // class, enum or fundamental => we
                                                                               // remove possible r-value reference
        );
        CppLiteMapCodeRange(pStatement, input);
        pBlock->addStatement(pStatement);
        CppLiteMapAndReturn(pInitExpression);
    }
    /// { a, b }
    bool traverseInitializerList(ast::InitializerList* input)
    {
        Type*& pInitType = CppLiteGetInitializationType();

        Expressions exprs;
        CppLitesGuard(exprs);

        // initialization list inside another, while initializing an array : T arr[] = { { } }

        Array* pArrayInitType = pInitType ? pInitType->asArray() : nullptr;
        if (pArrayInitType)
            m_Data.pushInitializationType(pArrayInitType->getItemType());
        else
            m_Data.pushInitializationType(nullptr);

        if (pArrayInitType && pArrayInitType->getItemCount() == 0)
        {
            if (input->m_AssignExpressionOrInitializerLists == nullptr ||
                input->m_AssignExpressionOrInitializerLists->empty())
            {
                pInitType = pArrayInitType->getItemType()->addPointer();
                CppLiteMapAndReturn(CppLiteGetSemantic()->createZeroInitExpression(pInitType));
            }
            else
            {
                pInitType =
                pArrayInitType->getItemType()->makeArray(input->m_AssignExpressionOrInitializerLists->size());
            }
        }

        if (input->m_AssignExpressionOrInitializerLists)
        {
            for (size_t i = 0; i < input->m_AssignExpressionOrInitializerLists->size(); ++i)
            {
                ast::Expression* pAstExpr = (*input->m_AssignExpressionOrInitializerLists)[i];
                Expression*      pExpr = CppLiteVisitExpression(pAstExpr);
                if (pExpr == nullptr)
                {
                    m_Data.popInitializationType();
                    return true;
                }
                exprs.push_back(pExpr);
            }
        }
        Types initTypes;
        for (auto& exp : exprs)
        {
            exp = exp->loadRValueReferenceExpression();
            initTypes.push_back(exp->getValueType());
        }

        m_Data.popInitializationType();

        InitializerListExpression* pInitListExpression =
        NewInScope<InitializerListExpression>(CppLiteGetSource()->initializerListType(initTypes), exprs);
        CppLiteGuard(pInitListExpression);
        CppLitesUnguard(exprs);
        if (pInitType) // list initialization
        {
            Expression* pExpression = CppLiteConv(pInitListExpression, pInitType);
            if (pExpression == nullptr)
                return true;
            if (pExpression == pInitListExpression)
            {
                CppLiteUnguard(pInitListExpression);
            }
            CppLiteMapAndReturn(pExpression);
        }
        CppLiteUnguard(pInitListExpression);
        CppLiteMapAndReturn(pInitListExpression);
    }

    bool traverseDeclType(ast::DeclType* input)
    {
        Expression* pExpression = CppLiteVisitExpression(input->m_Expression);
        if (pExpression == nullptr)
            return true;
        CppLiteErrorReturnIf(pExpression->isTemplateDependant(),
                             "decltype : template dependant decltype not supported yet");
        CppLiteGuard(pExpression);
        Type* pType;
        if (LValueExpression* pLValue = pExpression->as<LValueExpression>())
        {
            pType = pLValue->getRValueType();
        }
        else
        {
            pType = pExpression->getValueType();
        }
        CppLiteMapAndReturn(pType);
    }
    bool traverseTypeofExpression(ast::TypeofExpression* input)
    {
        Type* pType = CppLiteVisitType(input->m_Type);
        ;
        if (pType == nullptr)
            return true;
        Type* pCastPtrMetaT = PHANTOM_TYPEOF(Type*);
        switch (input->m_TYPEOF[0])
        {
        case 'c':
            pCastPtrMetaT = PHANTOM_TYPEOF(Class*);
            break;
        case 'e':
            pCastPtrMetaT = PHANTOM_TYPEOF(Enum*);
            break;
        case 'u':
            pCastPtrMetaT = PHANTOM_TYPEOF(Union*);
            break;
        }

        if (pType->isTemplateDependant())
        {
            CppLiteMapAndReturn(NewInScope<TemplateDependantTypeOfExpression>(pType, pCastPtrMetaT));
        }

        switch (input->m_TYPEOF[0])
        {
        case 'c':
            CppLiteErrorReturnIf(pType->asClass() == nullptr, "classof : type must be a class");
            break;
        case 'e':
            CppLiteErrorReturnIf(pType->asEnum() == nullptr, "enumof : type must be an enum");
            break;
        case 'u':
            CppLiteErrorReturnIf(pType->asUnion() == nullptr, "unionof : type must be an union");
            break;
        }

        Expression* pExpression =
        CppLiteGetSemantic()->convert(NewInScope<ConstantExpression>(createConstant(size_t(pType))), pCastPtrMetaT,
                                      CastKind::Reinterpret, UserDefinedFunctions::None, CppLiteGetScope());
        CPPLITEPARSER_ASSERT(pExpression);
        CppLiteMapAndReturn(pExpression);
    }
    bool traverseSizeofExpression(ast::SizeofExpression* input)
    {
        Type*            pType = nullptr;
        LanguageElement* pElem = CppLiteVisitElement(input->m_Type);
        ;
        if (pElem == nullptr)
            return true;
        if (pElem->isTemplateDependant())
        {
            LanguageElements elems;
            elems.push_back(pElem);
            CppLiteMapAndReturn(NewInScope<TemplateDependantExpression>(
            NewInScope<TemplateDependantElement>(nullptr, input->m_SIZEOF, phantom::NullOpt, MakeArrayView(elems))));
        }
        else
        {
            if (Expression* pExp = pElem->asExpression())
            {
                pType = pExp->getValueType();
            }
            else
            {
                pType = pElem->asType();
            }
            CppLiteErrorReturnIf(pType == nullptr, "invalid %s", input->m_SIZEOF);
            pType = pType->removeReference();
        }
        if (TemplateSpecialization* pSpec = pType->getTemplateSpecialization())
        {
            if (!pSpec->isNative())
            {
                if (pSpec && pSpec->isFull() && pSpec->testFlags(PHANTOM_R_FLAG_IMPLICIT)) // trivial instantiation
                {
                    CPPLITEPARSER_ASSERT(pSpec->getTemplated() == pType);
                    CPPLITEPARSER_ASSERT(pSpec->getInstantiationSpecialization());
                    Symbol* pPrimaryTemplated = pSpec->getInstantiationSpecialization()->getTemplated();
                    CPPLITEPARSER_ASSERT(pPrimaryTemplated);
                    CPPLITEPARSER_ASSERT(pPrimaryTemplated->asClassType());
                    // CppLiteErrorReturnIf(!(pPrimaryTemplated), "sizeof : incomplete type '%s'",
                    // pPrimaryTemplated->getUniqueName().data());
                }
                else if (pSpec->isFull()) // full specialization
                {
                    // CppLiteErrorReturnIf(!(pType->isDefined()), "sizeof : incomplete type '%s'",
                    // pType->getUniqueName().data());
                }
            }
        }
        CppLiteMapAndReturn(NewInScope<ConstantExpression>(
        createConstant(input->m_SIZEOF[0] == 's' ? pType->getSize() : pType->getAlignment())));
    }

    bool traverseTemplateArgumentNoAssign(ast::TemplateArgumentNoAssign* input)
    {
        LanguageElement* pElement = CppLiteVisitElement(input->m_PostFixExpression);
        if (pElement == nullptr)
            return true;
        if (Expression* pExpression = toExpression(pElement))
        {
            CppLiteErrorReturnIf(
            !(pExpression->isTemplateDependant()) &&
            (!(pExpression->isCompileTime()) ||
             !(pExpression->getEvalType()->removeReference()->removeQualifiers()->asIntegralType())),
            "template argument : expects type or compile-time integral constant");
        }
        else
        {
            CppLiteErrorReturnIf(pElement->asType() == nullptr,
                                 "template argument : expects type or compile-time integral constant");
        }
        CppLiteMapAndReturn(pElement);
        return true;
    }

    bool traverseTemplateArgumentAssign(ast::TemplateArgumentAssign* input)
    {
        if (input->m_Type)
        {
            LanguageElement* pElement = CppLiteVisitElement(input->m_Type);
            if (pElement == nullptr)
                return true;
            if (pElement->isTemplateDependant())
            {
                if (pElement->getMetaClass() == PHANTOM_CLASSOF(TemplateParameterPackTypeExpansion))
                {
                    CppLiteMapAndReturn(pElement);
                }
            }
            if (Expression* pExpression = toExpression(pElement))
            {
                CppLiteErrorReturnIf(
                !(pExpression->isTemplateDependant()) &&
                (!(pExpression->isCompileTime()) ||
                 !(pExpression->getEvalType()->removeReference()->removeQualifiers()->asIntegralType())),
                "template argument : expects type or compile-time integral constant");
            }
            else
            {
                CppLiteErrorReturnIf(pElement->asType() == nullptr,
                                     "template argument : expects type or compile-time integral constant");
            }
            CppLiteMapAndReturn(pElement);
        }

        CppLiteMapAndReturn(CppLiteConstantExpression(input->m_INT_VALUE));

        return true;
    }

    bool traverseIntegralConstant(ast::IntegralConstant* input)
    {
        CppLiteMapAndReturn(CppLiteConstantExpression(input->m_INT_VALUE));
    }
    bool traverseFunctionTypeExtent(ast::FunctionTypeExtent* input)
    {
        Type* pReturnType = CppLiteGetInitializationType();
        CPPLITEPARSER_ASSERT(pReturnType);
        OwnersGuard<Types> types;
        if (input->m_Types)
        {
            for (auto pAstType : *input->m_Types)
            {
                Type* pType = CppLiteVisitType(pAstType);
                if (pType == nullptr)
                    return true;
                types.push_back(pType);
            }
        }
        Type* pFuncType = CppLiteGetSource()->functionType(pReturnType, types);
        CppLiteErrorReturnIf(pFuncType == nullptr, "invalid function type");
        CppLiteMapAndReturn(pFuncType);
    }

    bool traverseArrayExtent(ast::ArrayExtent* input)
    {
        Type* pType = CppLiteGetInitializationType();
        CPPLITEPARSER_ASSERT(pType);
        Type* pExtType = pType;
        if (input->m_ArrayExtentExpression)
        {
            Expression* pExpression = CppLiteVisitExpression(input->m_ArrayExtentExpression);
            CppLiteErrorReturnIf(
            !(pExpression->isTemplateDependant()) &&
            (!(pExpression->isCompileTime()) ||
             !(pExpression->getEvalType()->removeReference()->removeQualifiers()->asIntegralType())),
            "[] : array extent must be a compile-time integral constant expression");
            pExpression = CppLiteConv(pExpression, PHANTOM_TYPEOF(size_t));
            CPPLITEPARSER_ASSERT(pExpression);
            if (pExpression->isTemplateDependant())
            {
                CppLiteMapAndReturn(NewInScope<TemplateDependantArray>(pType, pExpression, nullptr));
            }
            else
            {
                size_t extent = 0;
                pExpression->load(&extent);
                CppLiteErrorReturnIf(extent == 0, "[] : illegal zero-size array");
                CppLiteMapAndReturn(pExtType->addArray(extent));
            }
        }
        else
        {
            CppLiteMapAndReturn(pExtType->addArray(0)); // empty [] representation
        }
    }
    bool traverseDerivedExtent(ast::DerivedExtent* input)
    {
        Type* pType = CppLiteGetInitializationType();
        CPPLITEPARSER_ASSERT(pType);
        Type* pExtType = pType;
        switch (input->m_BIN_AND[0])
        {
        case '&':
        {
            CppLiteErrorReturnIf(pExtType->asReference(), "'%.*s' : already a reference type",
                                 PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pExtType)));
            if (input->m_BIN_AND[1] == '&')
                pExtType = pExtType->addRValueReference();
            else
                pExtType = pExtType->addLValueReference();
        }
        break;
        case '*':
        {
            CppLiteErrorReturnIf(pExtType->asReference(), "'%.*s' : references cannot have pointers",
                                 PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pExtType)));
            pExtType = pExtType->addPointer();
        }
        break;
        default:
            if (!applyQualifiers(input, pExtType, input->m_BIN_AND))
                return true;
        }
        CppLiteErrorReturnIf(pExtType == pType, "'%s' : type cannot be derived", input->m_BIN_AND);
        CppLiteMapAndReturn(pExtType);
    }

    bool traverseQualifiedName(ast::QualifiedName* input)
    {
        if (input->m_TYPENAME.hasValue())
        {
            Type* pType = (input->m_Scope) ? CppLiteVisitType(input->m_Scope) : CppLiteVisitType(input->m_Dot);
            if (pType == nullptr)
                return true;
            CppLiteMapAndReturn(pType);
        }
        else
        {
            return input->m_Scope ? this->traverse(input->m_Scope) : this->traverse(input->m_Dot);
        }
        return true;
    }

    void _checkForPackParameterExpansion(LanguageElement* a_pElem)
    {
        // Looking for any parameter pack used
        // if (name->m_TemplateArgumentList == nullptr) // template parameter pack cannot have template arguments
        if (auto pPH = a_pElem->asPlaceholder())
        {
            if (auto pTP = pPH->asSymbol()->getOwner()->asTemplateParameter())
            {
                if (pTP->isPack())
                {
                    m_Data.pushPackArgument(pPH);
                }
            }
        }
        else if (auto pNoExp = a_pElem->removeExpression())
            if (auto pParam = pNoExp->asParameter())
            {
                auto pPrmType = pParam->getValueType();
                if (pPrmType->isTemplateDependant())
                {
                    if (auto pExpa = Object::Cast<TemplateParameterPackTypeExpansion>(pPrmType))
                    {
                        m_Data.pushPackExpansionParameter(pParam);
                    }
                }
            }
    }

    bool traverseQualifiedScopedName(ast::QualifiedScopedName* input)
    {
        // CppLiteSourceByPasser();
        LanguageElement* pLHS = nullptr;
        if (input->m_Name)
        {
            if (input->m_Names == nullptr)
            {
                input->m_Names =
                new (m_Data.m_CppLiteGrammar.getAllocator().allocate(sizeof(phantom::SmallVector<soul::ast::Name*, 4>)))
                phantom::SmallVector<soul::ast::Name*, 4>;
            }
            input->m_Names->insert(input->m_Names->begin(), input->m_Name);
            input->m_Name = nullptr;
            input->m_SCOPE = spell::any();
        }
        else if (input->m_SCOPE.hasValue() && m_Data.m_pSource == nullptr)
        {
            pLHS = phantom::lang::Namespace::Global();
        }
        for (ast::Name* name : *input->m_Names)
        {
            LanguageElements  templateArgs;
            LanguageElements* pTemplateArgs = nullptr;
            if (name->m_TemplateArgumentList)
            {
                pTemplateArgs = &templateArgs;
                resolveTemplateArguments(name->m_TemplateArgumentList, templateArgs);
            }

            Expressions* pFuncArguments = nullptr;
            if (input->m_Names->back() == name) // last
            {
                pFuncArguments = CppLiteGetFunctionArguments();
            }

            String           identifier = NameToString(name);
            LanguageElement* pElement;
            if (pLHS)
            {
                pElement = CppLiteGetSemantic()->qualifiedLookup(pLHS, identifier, MakeOptionalArrayView(pTemplateArgs),
                                                                 phantom::NullOpt, CppLiteGetScope(),
                                                                 CppLiteGetInitializationType(), 0);
                //                 CppLiteErrorReturnIf(pElement == nullptr, "'%s' : no member found with this name
                //                 in
                //                 '%.*s'",
                //                                      identifier.data(),
                //                                      PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pLHS)));
            }
            else
            {
                pElement = CppLiteGetSemantic()->unqualifiedLookup(identifier, MakeOptionalArrayView(pTemplateArgs),
                                                                   phantom::NullOpt, CppLiteGetScope(),
                                                                   CppLiteGetInitializationType(), 0);
                //                 CppLiteErrorReturnIf(pElement == nullptr, "'%s' : no member found with this name
                //                 in
                //                 '%.*s'",
                //                                      identifier.data(),
                //                                      PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(CppLiteGetScope())));
            }

            // Looking for any parameter pack used
            // if (name->m_TemplateArgumentList == nullptr) // template parameter pack cannot have template
            // arguments

            if (pElement == nullptr)
                return true;
            _checkForPackParameterExpansion(pElement);
            pLHS = pElement;
        }
        CppLiteMapAndReturn(pLHS);
    }
    bool traverseQualifiedDotName(ast::QualifiedDotName* input)
    {
        LanguageElement* pLHS = nullptr;
        if (input->m_DOT.hasValue())
        {
            pLHS = phantom::lang::Application::Get()->rootPackageFolder();
        }
        for (ast::Name* name : *input->m_Names)
        {
            LanguageElements  templateArgs;
            LanguageElements* pTemplateArgs = nullptr;
            if (name->m_TemplateArgumentList)
            {
                pTemplateArgs = &templateArgs;
                if (!resolveTemplateArguments(name->m_TemplateArgumentList, templateArgs))
                    return true;
            }

            String identifier = NameToString(name);

            Expressions* pFuncArguments = nullptr;
            if (input->m_Names->back() == name) // last
            {
                pFuncArguments = CppLiteGetFunctionArguments();
            }
            LanguageElement* pElement;
            if (pLHS)
            {
                pElement = CppLiteGetSemantic()->qualifiedLookup(pLHS, identifier, MakeOptionalArrayView(pTemplateArgs),
                                                                 MakeOptionalArrayView(pFuncArguments),
                                                                 CppLiteGetScope(), CppLiteGetInitializationType(), 0);
                //                 CppLiteErrorReturnIf(pElement == nullptr, "'%s' : no member found with this name
                //                 in
                //                 '%.*s'",
                //                                      identifier.data(),
                //                                      PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pLHS)));
            }
            else
            {
                pElement = CppLiteGetSemantic()->unqualifiedLookup(
                identifier, MakeOptionalArrayView(pTemplateArgs), MakeOptionalArrayView(pFuncArguments),
                CppLiteGetScope(), CppLiteGetInitializationType(), 0);
                //                 CppLiteErrorReturnIf(pElement == nullptr, "'%s' : no member found with this name
                //                 in
                //                 '%.*s'",
                //                                      identifier.data(),
                //                                      PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(CppLiteGetScope())));
            }

            // Looking for any parameter pack used
            // if (name->m_TemplateArgumentList == nullptr) // template parameter pack cannot have template
            // arguments

            if (pElement == nullptr)
                return true;
            _checkForPackParameterExpansion(pElement);
            pLHS = pElement;
        }
        CppLiteMapAndReturn(pLHS);
    }
    bool traverseSignedType(ast::SignedType* input)
    {
        if (input->m_Type)
        {
            Type* pType = CppLiteVisitType(input->m_Type);
            CPPLITEPARSER_ASSERT(pType);
            pType = pType->signedType();
            CppLiteErrorReturnIf(pType == nullptr, "'signed' : invalid signed type");
            CppLiteMapAndReturn(pType);
        }
        else
        {
            CppLiteMapAndReturn(PHANTOM_TYPEOF(signed));
        }
    }
    bool traverseUnsignedType(ast::UnsignedType* input)
    {
        if (input->m_Type)
        {
            Type* pType = CppLiteVisitType(input->m_Type);
            CPPLITEPARSER_ASSERT(pType);
            pType = pType->unsignedType();
            CppLiteErrorReturnIf(pType == nullptr, "'unsigned' : invalid unsigned type");
            CppLiteMapAndReturn(pType);
        }
        else
        {
            CppLiteMapAndReturn(PHANTOM_TYPEOF(unsigned));
        }
    }
    bool traverseLongType(ast::LongType* input)
    {
        if (input->m_Type)
        {
            Type* pType = CppLiteVisitType(input->m_Type);
            CPPLITEPARSER_ASSERT(pType);
            pType = pType->longType();
            CppLiteErrorReturnIf(pType == nullptr, "'long' : invalid long type");
            CppLiteMapAndReturn(pType);
        }
        else
        {
            CppLiteMapAndReturn(PHANTOM_TYPEOF(long));
        }
    }
    bool traverseFundamentalTypeBase(ast::FundamentalTypeBase* input)
    {
        Type* pType = Application::Get()->getBuiltInType(input->m_Typename);
        CPPLITEPARSER_ASSERT(pType);
        CppLiteMapAndReturn(pType);
    }
    bool traverseVoidType(ast::VoidType* input) { CppLiteMapAndReturn(PHANTOM_TYPEOF(void)); }

    bool traverseQualifiedType(ast::QualifiedType* input)
    {
        LanguageElement* pElement = CppLiteVisitElement(input->m_QualifiedName);
        if (pElement == nullptr)
            return true;
        CppLiteMapAndReturn(pElement);
    }
    bool traverseExplicitType(ast::ExplicitType* input)
    {
        Type* pType = CppLiteVisitType(input->m_BasicType);
        if (pType == nullptr)
            return true;
        if (!applyQualifiers(input, pType, input->m_CONSTs))
            return true;
        if (input->m_TypeExtents)
        {
            for (auto pExt : *input->m_TypeExtents)
            {
                m_Data.pushInitializationType(pType);
                pType = CppLiteVisitType(pExt);
                m_Data.popInitializationType();
                if (pType == nullptr)
                    return true;
            }
        }
        if (input->m_ELLIPSE.hasValue())
        {
            auto pPackArg = m_Data.popPackArgument();
            CppLiteErrorReturnIf(!pPackArg, "... : parameter pack expansion outside of variadic template");
            auto* pExpa = New<TemplateParameterPackExpansion>(pType, pPackArg);
            pType = New<TemplateParameterPackTypeExpansion>(pExpa);
        }
        CppLiteMapAndReturn(pType);
    }

    bool traverseExplicitTypeNoFunctionType(ast::ExplicitTypeNoFunctionType* input)
    {
        PHANTOM_STATIC_ASSERT(sizeof(ast::ExplicitTypeNoFunctionType) == sizeof(ast::ExplicitType));
        return traverseExplicitType(reinterpret_cast<ast::ExplicitType*>(input));
    }

    bool applyQualifiers(ast::_BaseRule* input, Type*& a_pType, const phantom::SmallVector<spell::any, 4>* a_Quals)
    {
        if (a_Quals)
        {
            for (auto q : *a_Quals)
            {
                if (!applyQualifiers(input, a_pType, q.as_string()))
                    return true;
            }
        }
        return true;
    }

    bool applyQualifiers(ast::_BaseRule* input, Type*& a_pType, const char* a_Qual)
    {
        CppLiteDefaultReturnValue(false);
        CppLiteErrorReturnIf(a_pType->asReference(), "'%s' : reference types cannot be %.*s",
                             PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(a_pType)), a_Qual);
        Type* pQualType;
        switch (a_Qual[0])
        {
        case 'c':
            pQualType = a_pType->addConst();
            CppLiteErrorReturnIf(a_pType == pQualType, "'%.*s' : type already const",
                                 PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(a_pType)));
            a_pType = pQualType;
            break;
        case 'v':
            pQualType = a_pType->addVolatile();
            CppLiteErrorReturnIf(a_pType == pQualType, "'%.*s' : type already volatile",
                                 PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(a_pType)));
            a_pType = pQualType;
            break;
        default:
            CPPLITEPARSER_ASSERT(false);
        }
        return true;
    }

    bool traverseExplicitTypeOrAuto(ast::ExplicitTypeOrAuto* input)
    {
        Type* pType = CppLiteVisitType(input->m_BasicTypeOrAuto);
        if (pType == nullptr)
            return true;
        if (!applyQualifiers(input, pType, input->m_CONSTs))
            return true;
        if (input->m_TypeExtents)
        {
            for (auto pExt : *input->m_TypeExtents)
            {
                m_Data.pushInitializationType(pType);
                pType = CppLiteVisitType(pExt);
                m_Data.popInitializationType();
                if (pType == nullptr)
                    return true;
            }
        }
        CppLiteMapAndReturn(pType);
    }
    bool traverseExpressionType(ast::ExpressionType* input)
    {
        CppLiteMapAndReturn(CppLiteVisitElement(input->m_AssignExpression));
    }

    bool traverseAutoType(ast::AutoType* input)
    {
        Type* pType = Application::Get()->getAuto();
        if (pType == nullptr)
            return true;
        if (!applyQualifiers(input, pType, input->m_CONSTs))
            return true;
        if (input->m_TypeExtents)
        {
            for (auto pExt : *input->m_TypeExtents)
            {
                m_Data.pushInitializationType(pType);
                pType = CppLiteVisitType(pExt);
                m_Data.popInitializationType();
                if (pType == nullptr)
                    return true;
            }
        }
        CppLiteMapAndReturn(pType);
    }
    bool traverseAuto(ast::Auto* input) { CppLiteMapAndReturn(Application::Get()->getAuto()); }

    /// a = b
    bool traverseAssignExpression(ast::AssignExpression* input)
    {
        if (input->m_Greater_RightExpression)
            return traverseBinaryExpression(input->m_LeftExpression, ">", input->m_Greater_RightExpression);
        else if (input->m_RightShift_RightExpression)
            return traverseBinaryExpression(input->m_LeftExpression, ">>", input->m_RightShift_RightExpression);
        else if (input->m_RightShiftAssign_RightExpression)
            return traverseBinaryExpression(input->m_LeftExpression, ">>=", input->m_RightShiftAssign_RightExpression);
        else if (input->m_Op.hasValue())
            return traverseBinaryExpression(input->m_LeftExpression, input->m_Op.as_string(), input->m_RightExpression);
        else
            return this->traverse(input->m_LeftExpression);
    }
    bool traverseExpression(ast::Expression* input)
    {
        if (input->m_Expression_Expression)
            return traverseBinaryExpression(input->m_Expression_AssignExpression, ",", input->m_Expression_Expression);
        else if (input->m_Expression_AssignExpression)
            return this->traverse(input->m_Expression_AssignExpression);
        else
            return base_type::traverseExpression(input);
    }

    template<class XRight>
    bool traverseBinaryExpressions(ast::Expression* a_pLeftExpression, SmallVector<XRight*> const& a_RightExpressions)
    {
        ast::Expression* input = a_pLeftExpression;

        Expression* pExpr = CppLiteVisitExpression(a_pLeftExpression);
        CppLiteGuard(pExpr);

        for (XRight* pRightExp : a_RightExpressions)
        {
            CppLiteErrorReturnIf(pExpr == nullptr, "operator '%s' : invalid left-hand-side expression",
                                 pRightExp->m_Op);
            if (!resolveBinaryExpression(pExpr, pRightExp->m_Op, pRightExp->m_RightExpression, pExpr))
                pExpr = nullptr;
        }
        CppLiteUnguard(pExpr);
        CppLiteMapAndReturn(pExpr);
    }

    template<class XRight, class TypeTransform>
    bool traverseBinaryExpressionsAmb(ast::Expression*            a_pLeftExpression,
                                      SmallVector<XRight*> const& a_RightExpressions, TypeTransform _typeTransform)
    {
        ast::Expression* input = a_pLeftExpression;

        LanguageElement* pElem = CppLiteVisitElement(a_pLeftExpression);
        CppLiteGuard(pElem);
        Expression* pExpr = nullptr;
        for (XRight* pRightExp : a_RightExpressions)
        {
            CppLiteErrorReturnIf(pElem == nullptr, "operator '%s' : invalid left-hand-side expression",
                                 pRightExp->m_Op);
            if (pRightExp->m_RightExpression == nullptr)
            {
                CppLiteErrorReturnIf(a_RightExpressions.size() != 1, "duplicate '%s'", pRightExp->m_Op);
                Type* pType = pElem->toType();
                CppLiteErrorReturnIf(pType == nullptr, "expected type before '%s'", pRightExp->m_Op);
                CppLiteUnguard(pElem);
                CppLiteMapAndReturn(_typeTransform(pType));
            }
            if (!pExpr)
            {
                pExpr = pElem->asExpression();
                if (!pExpr)
                    pExpr = CppLiteGetSemantic()->toExpression(pElem);
                CppLiteErrorReturnIf(pExpr == nullptr, "expected expression before '%s'", pRightExp->m_Op);
            }
            if (!resolveBinaryExpression(pExpr, pRightExp->m_Op, pRightExp->m_RightExpression, pExpr))
                pExpr = nullptr;
            pElem = pExpr;
        }
        CppLiteUnguard(pElem);
        CppLiteMapAndReturn(pElem);
    }

    template<class XRight>
    bool traverseBinaryExpressions_Mul(ast::Expression*            a_pLeftExpression,
                                       SmallVector<XRight*> const& a_RightExpressions)
    {
        ast::Expression* input = a_pLeftExpression;

        Expression* pExpr = CppLiteVisitExpression(a_pLeftExpression);
        CppLiteGuard(pExpr);

        for (XRight* pRightExp : a_RightExpressions)
        {
            String op;
            if (pRightExp->m_OpMul)
            {
                for (auto op_part : *pRightExp->m_OpMul)
                    op += op_part.as_string();
            }
            else
            {
                op = pRightExp->m_Op.as_string();
            }
            CppLiteErrorReturnIf(pExpr == nullptr, "operator '%s' : invalid left-hand-side expression", op.c_str());
            if (!resolveBinaryExpression(pExpr, op.c_str(), pRightExp->m_RightExpression, pExpr))
                pExpr = nullptr;
        }
        CppLiteUnguard(pExpr);
        CppLiteMapAndReturn(pExpr);
    }

    /// a ? b : c
    bool traverseConditionalExpression(ast::ConditionalExpression* input)
    {
        if (input->m_Then == nullptr)
            return this->traverse(input->m_LeftExpression);
        Expression* pTest = CppLiteVisitExpression(input->m_LeftExpression);
        CppLiteGuard(pTest);
        Expression* pThen = CppLiteVisitExpression(input->m_Then);
        CppLiteGuard(pThen);
        Expression* pElse = CppLiteVisitExpression(input->m_Else);
        CppLiteGuard(pElse);
        CppLiteErrorReturnIf(pTest == nullptr, "conditional '?:' : invalid test operand");
        CppLiteErrorReturnIf(pThen == nullptr, "conditional '?:' : invalid then operand");
        CppLiteErrorReturnIf(pElse == nullptr, "conditional '?:' : invalid else operand");
        Expression* pResult =
        CppLiteGetSemantic()->solveConditional(pTest, pThen, pElse, CppLiteGetScopeAs(LanguageElement));
        if (pResult == nullptr)
            return true;
        CppLiteMapAndReturn(pResult);
    }

    /// a || b
    bool traverseLogicalOrExpression(ast::LogicalOrExpression* input)
    {
        if (input->m_LogicalOrExpressionRights)
            return traverseBinaryExpressions(input->m_LeftExpression, *input->m_LogicalOrExpressionRights);
        return this->traverse(input->m_LeftExpression);
    }

    /// a && b
    bool traverseLogicalAndExpression(ast::LogicalAndExpression* input)
    {
        if (input->m_LogicalAndExpressionRights)
        {
            return traverseBinaryExpressionsAmb(input->m_LeftExpression, *input->m_LogicalAndExpressionRights,
                                                [](Type* _type) { return _type->addRValueReference(); });
        }
        return this->traverse(input->m_LeftExpression);
    }

    /// a | b
    bool traverseBinOrExpression(ast::BinOrExpression* input)
    {
        if (input->m_BinOrExpressionRights)
            return traverseBinaryExpressions(input->m_LeftExpression, *input->m_BinOrExpressionRights);
        return this->traverse(input->m_LeftExpression);
    }

    /// a ^ b
    bool traverseXOrExpression(ast::XOrExpression* input)
    {
        if (input->m_XOrExpressionRights)
            return traverseBinaryExpressions(input->m_LeftExpression, *input->m_XOrExpressionRights);
        return this->traverse(input->m_LeftExpression);
    }

    /// a & b
    bool traverseBinAndExpression(ast::BinAndExpression* input)
    {
        if (input->m_BinAndExpressionRights)
        {
            return traverseBinaryExpressionsAmb(input->m_LeftExpression, *input->m_BinAndExpressionRights,
                                                [](Type* _type) { return _type->addLValueReference(); });
        }
        return this->traverse(input->m_LeftExpression);
    }

    /// a == b
    bool traverseEqualityExpression(ast::EqualityExpression* input)
    {
        if (input->m_EqualityExpressionRights)
            return traverseBinaryExpressions(input->m_LeftExpression, *input->m_EqualityExpressionRights);
        return this->traverse(input->m_LeftExpression);
    }

    /// a >< b
    bool traverseRelationalExpression(ast::RelationalExpression* input)
    {
        if (input->m_RelationalExpressionRights)
            return traverseBinaryExpressions_Mul(input->m_LeftExpression, *input->m_RelationalExpressionRights);
        else
            return this->traverse(input->m_LeftExpression);
    }

    /// a >><< b
    bool traverseShiftExpression(ast::ShiftExpression* input)
    {
        if (input->m_ShiftExpressionRights)
            return traverseBinaryExpressions_Mul(input->m_LeftExpression, *input->m_ShiftExpressionRights);
        else
            return this->traverse(input->m_LeftExpression);
    }

    /// a +- b
    bool traverseAddExpression(ast::AddExpression* input)
    {
        if (input->m_AddExpressionRights)
            return traverseBinaryExpressions(input->m_LeftExpression, *input->m_AddExpressionRights);
        else
            return this->traverse(input->m_LeftExpression);
    }

    /// a */ b
    bool traverseMulExpression(ast::MulExpression* input)
    {
        if (input->m_MulExpressionRights)
        {
            return traverseBinaryExpressionsAmb(input->m_LeftExpression, *input->m_MulExpressionRights,
                                                [](Type* _type) { return _type->addPointer(); });
        }
        else
            return this->traverse(input->m_LeftExpression);
    }

    bool resolveBinaryExpression(Expression* a_pLeftExpression, const char* a_Op, ast::Expression* a_pRightExpression,
                                 Expression*& a_pResult)
    {
        CppLiteDefaultReturnValue(false);
        Expression*      pRight = CppLiteVisitExpression(a_pRightExpression);
        ast::Expression* input = a_pRightExpression;
        CppLiteErrorReturnIf(pRight == nullptr, "operator '%s' : invalid right-hand-side expression", a_Op);
        CppLiteGuard(pRight);
        a_pResult = CppLiteGetSemantic()->solveBinaryOperator(a_Op, a_pLeftExpression, pRight, CppLiteGetScope());
        CppLiteErrorReturnIf(a_pResult == nullptr,
                             "operator '%s' : no overload found taking left-hand-side type '%.*s' and "
                             "right-hand-side type '%.*s'",
                             a_Op,
                             PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(a_pLeftExpression->getEvalType())),
                             PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pRight->getEvalType())));
        return true;
    }

    bool traverseBinaryExpression(ast::Expression* a_pLeftExpression, const char* a_Op,
                                  ast::Expression* a_pRightExpression)
    {
        CPPLITEPARSER_ASSERT(a_pRightExpression);
        ast::Expression* input = a_pLeftExpression;
        Expression*      pExpr = CppLiteVisitExpression(a_pLeftExpression);
        CppLiteErrorReturnIf(pExpr == nullptr, "operator '%s' : invalid left-hand-side expression", a_Op);
        CppLiteGuard(pExpr);
        if (!resolveBinaryExpression(pExpr, a_Op, a_pRightExpression, pExpr))
            return true;
        CppLiteUnguard(pExpr);
        CppLiteMapAndReturn(pExpr);
    }

    bool traverseUnaryExpression(ast::UnaryExpression* input)
    {
        if (input->m_Expression)
        {
            Expression* pExpression = CppLiteVisitExpression(input->m_Expression);
            CppLiteErrorReturnIf(pExpression == nullptr, "operator '%s' : invalid right-hand-side expression",
                                 input->m_Op.as_string());
            CppLiteGuard(pExpression);
            Expression* pExpr =
            CppLiteGetSemantic()->solveUnaryPreOperator(input->m_Op.as_string(), pExpression, CppLiteGetScope());
            CppLiteErrorReturnIf(
            pExpr == nullptr, "operator '%s' : no overload found taking right-hand-side type '%.*s'",
            input->m_Op.as_string(), PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pExpression->getEvalType())));
            CppLiteMapAndReturn(pExpr);
        }
        return this->traverse(input->m_PostFixExpression);
    }

    bool traverseFunctionPtrExpression(ast::FunctionPtrExpression* input)
    {
        Symbol* pSymbol = CppLiteVisitSymbol(input->m_Name);
        CppLiteErrorReturnIf(pSymbol->asSubroutine() == nullptr, "@function_ptr : name is not a function");
        SubroutinePointerExpression* pExpr =
        NewInScope<SubroutinePointerExpression>(CppLiteGetSource(), pSymbol->asSubroutine());
        CppLiteMapAndReturn(pExpr);
    }

    bool traverseSpecialCastExpression(ast::SpecialCastExpression* input)
    {
        Expression* pExpression = CppLiteVisitExpression(input->m_Expression);
        CppLiteErrorReturnIf(pExpression == nullptr, "'%s' : invalid cast expression", input->m_CastKind);
        Type* pType = CppLiteVisitType(input->m_Type);
        CppLiteErrorReturnIf(pType == nullptr, "'%s' : invalid cast type", input->m_CastKind);
        LanguageElement* pConv = nullptr;
        switch (input->m_CastKind[0])
        {
        case 's': // static_cast
            pConv = CppLiteGetSemantic()->convert(pExpression, pType, CastKind::Static,
                                                  UserDefinedFunctions::ImplicitsOnly, CppLiteGetScope());
            break;
        case 'c': // const_cast
            pConv = CppLiteGetSemantic()->convert(pExpression, pType, CastKind::Const,
                                                  UserDefinedFunctions::ImplicitsOnly, CppLiteGetScope());
            break;
        case 'r': // reinterpret_cast
            pConv = CppLiteGetSemantic()->convert(pExpression, pType, CastKind::Reinterpret,
                                                  UserDefinedFunctions::ImplicitsOnly, CppLiteGetScope());
            break;
        default:
            PHANTOM_UNREACHABLE();
            break;
        }
        if (pConv == nullptr)
        {
            CppLiteError("%s : cannot convert from '%.*s' to '%.*s'", input->m_CastKind,
                         PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pExpression->getValueType())),
                         PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pType)));
            Delete(pExpression);
            return true;
        }
        CppLiteMapAndReturn(pConv);
        return true;
    }

    bool traverseLambdaExpression(ast::LambdaExpression* input)
    {
        auto pScopeElem = CppLiteGetScope();
        auto pScope = pScopeElem->asScope();
        if (pScope == nullptr)
            pScope = pScopeElem->getEnclosingScope();

        char lambdaName[44];
        phantom::random::str(lambdaName, "lambda$%%%%%%%%_%%%%_%%%%_%%%%_%%%%%%%%%%%%");

        // captures

        SmallMap<StringView, LocalVariable*> capturedVariablesByRef;
        SmallMap<StringView, LocalVariable*> capturedVariablesByCopy;
        SmallSet<LocalVariable*>             explicitlyCaptured;

        Block* pBlock = pScopeElem->asBlock();
        CppLiteErrorReturnIf(input->m_LambdaCaptures != nullptr && !input->m_LambdaCaptures->empty() &&
                             pBlock == nullptr,
                             "lambda : captures outside of block are forbidden");

        bool thisCaptured = false;
        for (ast::LambdaCapture* pLambdaCapture : *input->m_LambdaCaptures)
        {
            bool copyAll = pLambdaCapture->m_ASSIGN.hasValue();
            bool refAll = pLambdaCapture->m_BIN_AND.hasValue() && !pLambdaCapture->m_IDENTIFIER.hasValue();
            if (refAll || copyAll)
            {
                thisCaptured = true;
                if (copyAll && capturedVariablesByCopy.size())
                {
                    CppLiteError("lambda : '=' : capture all conflits with previous capture by copy");
                    continue;
                }
                if (refAll && capturedVariablesByRef.size())
                {
                    CppLiteError("lambda : '&' : capture all conflits with previous capture by reference");
                    continue;
                }
                auto& captureAll = copyAll ? capturedVariablesByCopy : capturedVariablesByRef;
                auto  pCurrBlock = pBlock;
                while (pCurrBlock)
                {
                    for (auto pLocal : pCurrBlock->getLocalVariables())
                    {
                        auto& captured = captureAll[pLocal->getName()];
                        if (captured == nullptr)
                            captured = pLocal;
                    }
                    pCurrBlock = pCurrBlock->getBlock();
                }
            }
            else if (pLambdaCapture->m_THIS.hasValue())
            {
                thisCaptured = true;
                auto pCaptureScopeMethod = pBlock->getSubroutine()->asMethod();
                if (pCaptureScopeMethod == nullptr)
                {
                    CppLiteError("lambda : capturing 'this' outside of method block");
                    continue;
                }
                auto pThis = pCaptureScopeMethod->getThis();
                if (!explicitlyCaptured.insert(pThis).second)
                {
                    CppLiteError("lambda : 'this' already captured");
                }
                capturedVariablesByCopy["this"] = pThis;
                capturedVariablesByRef.erase("this");
            }
            else if (pLambdaCapture->m_IDENTIFIER.hasValue())
            {
                auto name = pLambdaCapture->m_IDENTIFIER.as_string();
                auto pCapturedLocal = pBlock->getLocalVariableCascade(name);
                if (pCapturedLocal == nullptr)
                {
                    CppLiteError("lambda : capturing unknown local '%s'", name);
                    continue;
                }
                if (!explicitlyCaptured.insert(pCapturedLocal).second)
                {
                    CppLiteError("lambda : '%s' already explicitly captured", name);
                    continue;
                }
                if (pLambdaCapture->m_BIN_AND.hasValue())
                {
                    auto& captured = capturedVariablesByRef[name];
                    if (captured)
                    {
                        CppLiteError("lambda : '%s' already captured by capture all '&'", name);
                        continue;
                    }
                    captured = pCapturedLocal;
                    capturedVariablesByCopy.erase(name);
                }
                else
                {
                    auto& captured = capturedVariablesByCopy[name];
                    if (captured)
                    {
                        CppLiteError("lambda : '%s' already captured by capture all '='", name);
                        continue;
                    }
                    captured = pCapturedLocal;
                    capturedVariablesByRef.erase(name);
                }
            }
        }

        Expressions initExpressions;

        auto builder = ClassBuilder::struct_(pScope, lambdaName);
        for (auto& name_pLocalByCopy : capturedVariablesByCopy)
        {
            auto pType = name_pLocalByCopy.second->getValueType();
            auto pTypeNoRef = pType->removeReference()->removeQualifiers();
            if (!pTypeNoRef->isCopyable())
            {
                CppLiteError("lambda : %.* : unable to capture by copy as type '%.*s' is not copyable");
                continue;
            }
            builder.field(pTypeNoRef->addConst(), name_pLocalByCopy.first);
            auto pConvExp = CppLiteConv(New<LocalVariableExpression>(name_pLocalByCopy.second), pTypeNoRef, pScopeElem);
            CPPLITEPARSER_ASSERT(pConvExp);
            initExpressions.push_back(pConvExp);
        }

        for (auto name_pLocalByRef : capturedVariablesByRef)
        {
            auto pAsRef = name_pLocalByRef.second->getValueType();
            auto pPointer = pAsRef->removeReference()->makePointer();
            if (pPointer == nullptr)
            {
                CppLiteError("lambda : %.* : unable to capture by ref as type '%.*s' is not addressable");
                continue;
            }
            builder.field(pPointer, name_pLocalByRef.first);
            auto pConvExp = CppLiteConv(New<LocalVariableExpression>(name_pLocalByRef.second)->address(getSource()),
                                        pPointer, pScopeElem);
            CPPLITEPARSER_ASSERT(pConvExp);
            initExpressions.push_back(pConvExp);
        }

        auto pLambdaClass = builder.finalize();

        auto pLambdaInitCtrct = New<ClassListInitializationExpression>(pLambdaClass, initExpressions);

        Modifiers modifiers = Modifier::Const;

        Subroutine* pLambdaCallOp{};

        Parameters params;

        // signature
        if (!resolveParameters(input, input->m_Parameters, params))
            return true;

        // check name collision between parameters and captured variables
        for (auto param : params)
        {
            CppLiteErrorIf(capturedVariablesByRef.find(param->getName()) != capturedVariablesByRef.end() ||
                           capturedVariablesByCopy.find(param->getName()) != capturedVariablesByCopy.end(),
                           "lambda : parameter name '%.*s' collides with a capture",
                           PHANTOM_STRING_AS_PRINTF_ARG(param->getName()));
        }

        Type* pRetType = nullptr;

        if (input->m_ArrowReturn)
        {
            pRetType = CppLiteVisitType(input->m_ArrowReturn->m_Type);
        }

        auto pAuto = Application::Get()->getAuto();

        if (!pRetType)
            pRetType = pAuto;

        auto pSign = New<Signature>(pRetType, params, Modifier::Const);

        auto pLambdaMethod = New<Method>("operator()", pSign, Modifier::Const);
        pLambdaMethod->setAccess(Access::Public);
        pLambdaClass->addMethod(pLambdaMethod);

        // create block (without 'this')

        Block* pLambdaBlock = pLambdaMethod->New<Block>();
        pLambdaMethod->setBlock(pLambdaBlock);
        size_t count = params.size();
        while (count--) // add them in reverse order for the destruction to happen canonical (left to right) to argument
                        // passing (right to left)
        {
            addLocalVariable(pLambdaBlock, params[count]);
        }

        // add 'this' if captured
        if (thisCaptured)
        {
            auto pCapturedThis = pLambdaClass->getField("this");
            auto pLocal = addLocalVariable(pLambdaBlock, pCapturedThis->getValueType()->addConst(), "this");
            auto pLocalThisAccess = New<LocalVariableExpression>(pLambdaMethod->getThis())->dereference(getSource());
            auto pFieldThisAccess = New<FieldExpression>(pLocalThisAccess, pCapturedThis);
            auto pInitStmt = New<LocalVariableInitializationStatement>(pLocal, pFieldThisAccess->load(getSource()));
            pLambdaBlock->addStatement(pInitStmt);
        }

        // make deref from field int* myCapturedRef to local int&
        // myCapturedRef, inside the operator() block
        for (auto& name_pLocal : capturedVariablesByRef)
        {
            if (name_pLocal.first == "this")
                continue;
            auto pLambdaThisAccess = New<LocalVariableExpression>(pLambdaMethod->getThis())->dereference(getSource());
            auto pField = pLambdaClass->getField(name_pLocal.first);
            auto pFieldAccess = New<FieldExpression>(pLambdaThisAccess, pField);
            auto pLocal = addLocalVariable(pLambdaBlock, pField->getValueType()->removePointer()->addLValueReference(),
                                           name_pLocal.first);
            auto pInitStmt = New<LocalVariableInitializationStatement>(pLocal, pFieldAccess->dereference(getSource()));
            pLambdaBlock->addStatement(pInitStmt);
        }

        // make ref of int myCapturedCopy as int&
        // myCapturedCopy, inside the operator() block
        for (auto& name_pLocal : capturedVariablesByCopy)
        {
            if (name_pLocal.first == "this")
                continue;
            auto pLambdaThisAccess = New<LocalVariableExpression>(pLambdaMethod->getThis())->dereference(getSource());
            auto pField = pLambdaClass->getField(name_pLocal.first);
            auto pFieldAccess = New<FieldExpression>(pLambdaThisAccess, pField);
            auto pLocal =
            addLocalVariable(pLambdaBlock, pField->getValueType()->addLValueReference(), name_pLocal.first);
            auto pInitStmt = New<LocalVariableInitializationStatement>(pLocal, pFieldAccess);
            pLambdaBlock->addStatement(pInitStmt);
        }

        pLambdaMethod->setBlockBuilder([=](Block* a_pBlock) -> bool {
            pLambdaMethod->setBlockBuilder({});
            return CppLiteVisitElement(input->m_FunctionBlock, pLambdaMethod) != nullptr;
        });

        if (pBlock)
            pLambdaMethod->buildBlock();

        CppLiteMapAndReturn(pLambdaInitCtrct);
        return true;
    }

    bool traversePostFixExpression(ast::PostFixExpression* input)
    {
        size_t i = 0;
        if (input->m_PostFixEnds == nullptr)
        {
            Expression* pLHS = CppLiteVisitExpression(input->m_Expression);
            if (pLHS == nullptr) // error must be reported from above call
                return true;
            CppLiteGuard(pLHS);
            CppLiteMapAndReturn(pLHS);
        }
        else
        {
            LanguageElement* pLHS = m_Data.getLeftExpression();
            CppLiteGuard(pLHS);
            ast::Name* pPendingName = input->m_Expression->type() == ast::e_RuleType_PrimaryExpression
            ? static_cast<ast::PrimaryExpression*>(input->m_Expression)->m_Name
            : nullptr;
            if (pPendingName == nullptr ||
                (*input->m_PostFixEnds)[0]->m_CallList == nullptr &&
                (*input->m_PostFixEnds)[0]->m_InitializerList == nullptr)
            {
                pLHS = CppLiteVisitElement(input->m_Expression);
                if (pLHS == nullptr) // error must be reported from above call
                    return true;
                pPendingName = nullptr;
            }
            size_t count = input->m_PostFixEnds->size();
            for (size_t i = 0; i < count; ++i)
            {
                ast::PostFixEnd* pCurrent = (*input->m_PostFixEnds)[i];
                ast::PostFixEnd* pNext = (i < count - 1) ? (*input->m_PostFixEnds)[i + 1] : nullptr;
                if (pCurrent->m_CallList)
                {
                    Expressions expressions;
                    CppLitesGuard(expressions);
                    resolveCallList(pCurrent->m_CallList, expressions);
                    if (pPendingName) // we have a previous name, we use it
                    {
                        LanguageElement* pElement = nullptr;
                        CppLiteGuard(pElement);
                        LanguageElements* pTemplateArguments = nullptr;
                        LanguageElements  templateArguments;
                        CppLitesGuard(templateArguments);
                        String identifier = NameToString(pPendingName);
                        if (pPendingName->m_TemplateArgumentList)
                        {
                            pElement = resolveGenericCall(input, pLHS, identifier.c_str(),
                                                          pPendingName->m_TemplateArgumentList, pCurrent->m_CallList);
                            if (pElement)
                                ++i; // skip next PostFixEnd future treatment because treated
                                     // here
                            else
                            {
                                pTemplateArguments = &templateArguments;
                                if (!resolveTemplateArguments(pPendingName->m_TemplateArgumentList, templateArguments))
                                    return true;
                            }
                        }
                        if (pElement == nullptr)
                        {
                            // CppLiteGetSemantic()->pushTry();
                            if (pLHS)
                                pElement = CppLiteGetSemantic()->qualifiedLookup(
                                pLHS, identifier, MakeOptionalArrayView(pTemplateArguments), MakeArrayView(expressions),
                                CppLiteGetScope(), CppLiteGetInitializationType());
                            else
                                pElement = CppLiteGetSemantic()->unqualifiedLookup(
                                identifier, MakeOptionalArrayView(pTemplateArguments), MakeArrayView(expressions),
                                CppLiteGetScope(), CppLiteGetInitializationType());
                            if (pElement == nullptr)
                                return true;
                            // CppLiteGetSemantic()->popTry();

                            // CppLiteErrorReturnIf(pElement == nullptr,
                            //                  "'%s' : no overload defined taking the given arguments",
                            //                  identifier.data());
                        }
                        pLHS = toExpression(pElement);
                        CppLiteErrorReturnIf(pLHS == nullptr, "'%s' : expecting expression, symbol found",
                                             identifier.data());
                        pPendingName = nullptr;
                        if (expressions.size() == 1 && expressions.front() == pLHS)
                        {
                            CppLiteUnguard(expressions);
                        }
                        CppLiteUnguard(pLHS);
                        CppLiteUnguard(pElement);
                        continue;
                    }
                    else
                    {
                        LanguageElement* pElement = nullptr;
                        if (pLHS)
                            pElement = CppLiteGetSemantic()->qualifiedLookup(
                            pLHS, "operator()", phantom::NullOpt, MakeArrayView(expressions), CppLiteGetScope(),
                            CppLiteGetInitializationType());
                        else
                            pElement = CppLiteGetSemantic()->unqualifiedLookup(
                            "operator()", phantom::NullOpt, MakeArrayView(expressions), CppLiteGetScope(),
                            CppLiteGetInitializationType());
                        if (pElement == nullptr)
                            return true;

                        pLHS = toExpression(pElement);
                        CppLiteErrorReturnIf(pLHS == nullptr, "operator () : expecting expression, symbol found");
                    }
                }
                else if (pCurrent->m_InitializerList)
                {
                    if (pPendingName)
                    {
                        LanguageElements* pTemplateArguments = nullptr;
                        LanguageElements  templateArguments;
                        CppLitesGuard(templateArguments);
                        if (pPendingName->m_TemplateArgumentList)
                        {
                            pTemplateArguments = &templateArguments;
                            if (!resolveTemplateArguments(pPendingName->m_TemplateArgumentList, templateArguments))
                                return true;
                        }

                        String           identifier = NameToString(pPendingName);
                        LanguageElement* pElement = nullptr;
                        CppLiteGuard(pElement);
                        if (pLHS)
                            pElement = CppLiteGetSemantic()->qualifiedLookup(
                            pLHS, identifier, MakeOptionalArrayView(pTemplateArguments), phantom::NullOpt,
                            CppLiteGetScope(), CppLiteGetInitializationType());
                        else
                            pElement = CppLiteGetSemantic()->unqualifiedLookup(
                            identifier, MakeOptionalArrayView(pTemplateArguments), phantom::NullOpt, CppLiteGetScope(),
                            CppLiteGetInitializationType());
                        if (pElement == nullptr)
                            return true;
                        //                         CppLiteErrorReturnIf(pElement == nullptr,
                        //                                              "'%s' : list-initialization : unknown
                        //                                              symbol, expects a class type",
                        //                                              identifier.data());
                        Type* pType = pElement->toType();
                        CppLiteErrorReturnIf(pType == nullptr,
                                             "'%s' : list-initialization : the symbol on the left "
                                             "of '{' is not a type",
                                             identifier.data());

                        //                         if (!pLHS)
                        //                         {
                        //                             // try calling classic constructors
                        //                             ast::CallList callList;
                        //                             callList.m_AssignExpressionOrInitializerLists
                        //                             =
                        //                             static_cast<ast::InitializerList*>(pCurrent->m_InitializerList)
                        //                             ->m_AssignExpressionOrInitializerLists;
                        //                             pElement =
                        //                             CppLiteGetSemantic()->unqualifiedLookup(
                        //                             identifier,
                        //                             MakeOptionalArrayView(pTemplateArguments),
                        //                             phantom::NullOpt, CppLiteGetScope(),
                        //                             CppLiteGetInitializationType());
                        //                         }

                        m_Data.pushInitializationType(pType);
                        Expression* pExpression = CppLiteVisitExpression(pCurrent->m_InitializerList);
                        m_Data.popInitializationType();
                        if (pExpression == nullptr)
                            return true;
                        pPendingName = nullptr;
                        pLHS = pExpression;
                    }
                    else
                    {
                        CPPLITEPARSER_ASSERT(pLHS);
                        CppLiteErrorReturnIf(pLHS->asType() == nullptr,
                                             "list-initialization {} : expression on the left must be "
                                             "a class or a structure");
                        m_Data.pushInitializationType(static_cast<Type*>(pLHS));
                        Expression* pExpression = CppLiteVisitExpression(pCurrent->m_InitializerList);
                        m_Data.popInitializationType();
                        if (pExpression == nullptr)
                            return true;
                        pLHS = pExpression;
                    }
                }
                else if (pCurrent->m_ArrayAccess)
                {
                    CPPLITEPARSER_ASSERT(pLHS);
                    Expression* pIndex =
                    CppLiteVisitExpression(static_cast<ast::ArrayAccess*>(pCurrent->m_ArrayAccess)->m_Expression);
                    CppLiteGuard(pIndex);
                    CppLiteErrorReturnIf(pIndex == nullptr, "operator [] : invalid index expression");
                    Expression* pExpression = toExpression(pLHS);
                    if (pExpression)
                    {
                        Expression* pExpr =
                        CppLiteGetSemantic()->solveBinaryOperator("[]", pExpression, pIndex, CppLiteGetScope());
                        CppLiteErrorReturnIf(
                        pExpr == nullptr, "operator []  : no overload defined taking argument of type '%.*s'",
                        PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pIndex->getEvalType())));

                        pLHS = pExpr;
                    }
                    else
                    {
                        Type* pType = pLHS->asSymbol()->toType();
                        CppLiteErrorReturnIf(
                        pType == nullptr, "operator []  : no overload defined taking argument of type '%.*s'",
                        PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pIndex->getEvalType())));
                        CppLiteErrorReturnIf(
                        !(pIndex->isCompileTime()) &&
                        !(pIndex->getEvalType()->removeReference()->removeQualifiers()->asIntegralType()),
                        "[]  : array size expects integral type");
                        pIndex = CppLiteConv(pIndex, PHANTOM_TYPEOF(size_t));
                        size_t extent = 0;
                        pIndex->load(&extent);
                        CppLiteErrorReturnIf(extent == 0, "[]  : illegal zero-size array");
                        pLHS = pType->addArray(extent);
                        CppLiteErrorReturnIf(pLHS == pType, "[]  : illegal array type");
                    }
                }
                else if (pCurrent->m_INC.hasValue())
                {
                    CPPLITEPARSER_ASSERT(pLHS);
                    Expression* pLeft = toExpression(pLHS);
                    CppLiteErrorReturnIf(
                    pLeft == nullptr, "operator %s  : expects right-hand-side expression, symbol '%.*s' found",
                    pCurrent->m_INC.as_string(), PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pLHS)));
                    Expression* pExpr =
                    CppLiteGetSemantic()->solveUnaryPostOperator(pCurrent->m_INC.as_string(), pLeft);
                    CppLiteErrorReturnIf(pExpr == nullptr, "operator %s : no valid overload exists for type '%.*s'",
                                         pCurrent->m_INC.as_string(),
                                         PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pLeft->getEvalType())));
                    pExpr->setCodeRange(CppLiteCodeRange(pCurrent->location()));
                    pLHS = pExpr;
                }
                else if (pCurrent->m_ELLIPSE.hasValue())
                {
                    if (auto pPackParam = m_Data.popPackExpansionParameter())
                    {
                        CPPLITEPARSER_ASSERT(pLHS);
                        CppLiteErrorReturnIf(!pLHS->asExpression(),
                                             "... : parameter pack expansion expects evaluable expression");
                        pLHS = New<ParameterPackExpressionExpansion>(static_cast<Expression*>(pLHS), pPackParam);
                    }
                    else if (auto pPackArg = m_Data.popPackArgument())
                    {
                        pLHS = New<TemplateParameterPackExpansion>(pLHS, pPackArg);
                    }
                    else
                    {
                        if (auto pSpec = m_Data.getScope()->getEnclosingTemplateSpecialization())
                            if (pSpec->getTemplateSignature()->isVariadic())
                            {
                                CppLiteError(
                                "... : pack expansion without related function argument or template argument pack");
                                return true;
                            }
                        CppLiteError("... : parameter pack expansion outside of variadic template");
                        return true;
                    }
                }
                else if (pCurrent->m_DOT.hasValue())
                {
                    CPPLITEPARSER_ASSERT(pLHS);
                    if (pCurrent->m_DOT.as_string()[0] == '-') // arrow
                    {
                        Expression* pLeft = toExpression(pLHS);
                        CppLiteErrorReturnIf(
                        pLeft == nullptr, "operator %s  : expects right-hand-side expression, symbol '%.*s' found",
                        pCurrent->m_DOT.as_string(), PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pLHS)));
                        Expressions args;
                        args.push_back(pLeft);
                        while (pLeft->getValueType()->removeConstReference()->asPointer() ==
                               nullptr) // left is not a pointer type
                        {
                            Expression* pArrowedExpression = CppLiteGetSemantic()->solveUnaryPostOperator(
                            "->", pLeft, CppLiteGetScopeAs(LanguageElement));
                            CppLiteErrorReturnIf(
                            pArrowedExpression == nullptr, "operator '->' : no valid overload exists for type '%.*s'",
                            PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pLeft->getValueType())));
                            pArrowedExpression->setCodeRange(CppLiteCodeRange(pCurrent->location()));
                            pLeft = pArrowedExpression;
                            if (pLeft->isTemplateDependant())
                                break;
                        }
                        CPPLITEPARSER_ASSERT(pLeft);
                        CPPLITEPARSER_ASSERT(pLeft->isTemplateDependant() ||
                                             pLeft->getValueType()->removeConstReference()->asPointer());
                        pLHS = pLeft->dereference(getSource());
                    }
                    else if (Namespace* pNS = pLHS->asNamespace())
                    {
                        if (i == 0 && pCurrent->m_DOT.as_string()[0] != ':')
                        {
                            pLHS = Application::Get()->getPackageFolder(pNS->getName());
                            CppLiteErrorReturnIf(pLHS == nullptr, "no package named '%s'", pNS->getName().data());
                        }
                    }
                    else
                    {
                        if (Expression* pExpr = pLHS->asExpression())
                        {
                            Type* pNoCVR = pExpr->getValueType()->removeReference()->removeQualifiers();
                            if (pNoCVR->asPointer())
                            {
                                CppLiteErrorReturn("invalid use of '.' on a pointer ('%.*s') : use '->' instead or "
                                                   "switch to 'safe' mode",
                                                   PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pNoCVR)));
                            }
                        }
                    }
                    Expressions* pFunctionArguments = nullptr;
                    Expressions  expressions;
                    CppLitesGuard(expressions);
                    String           identifier = NameToString(pCurrent->m_Name);
                    LanguageElement* pElement = nullptr;
                    bool             genericCallSolvingTried = false;
                    if ((pNext && pNext->m_CallList))
                    {
                        if (pCurrent->m_Name->m_TemplateArgumentList)
                        {
                            pElement = resolveGenericCall(input, pLHS, identifier.c_str(),
                                                          pCurrent->m_Name->m_TemplateArgumentList, pNext->m_CallList);
                            if (pElement)
                                ++i; // skip next PostFixEnd future treatment because treated
                                     // here
                        }
                        if (pElement == nullptr)
                        {
                            if (!(resolveCallList(pNext->m_CallList, expressions)))
                                return true;
                            genericCallSolvingTried = true;
                            pFunctionArguments = &expressions;
                            ++i; // skip next PostFixEnd future treatment because treated here
                        }
                    }
                    if (pElement == nullptr)
                    {
                        LanguageElements* pTemplateArguments = nullptr;
                        LanguageElements  templateArguments;
                        CppLitesGuard(templateArguments);

                        if (pCurrent->m_Name->m_TemplateArgumentList)
                        {
                            if (!genericCallSolvingTried && pNext)
                                pElement =
                                resolveGenericCall(input, pLHS, identifier.c_str(),
                                                   pCurrent->m_Name->m_TemplateArgumentList, pNext->m_CallList);
                            if (pElement)
                                ++i; // skip next PostFixEnd future treatment because treated
                                     // here
                            else
                            {
                                pTemplateArguments = &templateArguments;
                                if (!resolveTemplateArguments(pCurrent->m_Name->m_TemplateArgumentList,
                                                              templateArguments))
                                    return true;
                            }
                        }

                        if (pElement == nullptr)
                        {
                            //                             if (pCurrent->m_DOT.as_string()[0] == ':') // c++ scope
                            //                             {
                            //                                 CppLiteSourceByPasser();
                            //                                 pElement = CppLiteGetSemantic()->qualifiedLookup(
                            //                                 pLHS, identifier,
                            //                                 MakeOptionalArrayView(pTemplateArguments),
                            //                                 MakeOptionalArrayView(pFunctionArguments),
                            //                                 CppLiteGetScope(), CppLiteGetInitializationType(),
                            //                                 0);
                            //                             }
                            //                             else
                            {
                                bool        bOverloadCalledInOverload = false;
                                Expression* pLHSExp = nullptr;
                                // ===================== DISABLE GlobalMethodOverload because causes exponential
                                // compile time FIXME =======================
                                //                                 if (CppLiteHasFeature("GlobalMethodOverload") &&
                                //                                 pFunctionArguments &&
                                //                                     (pLHSExp = pLHS->asExpression()) &&
                                //                                     pLHSExp->getValueType()->asReference())
                                //                                 {
                                //                                     Expressions exps;
                                //                                     exps.push_back(pLHSExp->clone(a_pOwner)->address());
                                //                                     CppLitesGuard(exps);
                                //
                                //                                     for (auto exp : expressions)
                                //                                     {
                                //                                         exps.push_back(exp->clone(a_pOwner));
                                //                                     }
                                //
                                //                                     // try at this source scope
                                //                                     pElement =
                                //                                     CppLiteGetSemantic()->silentUnqualifiedLookup(
                                //                                     identifier,
                                //                                     MakeOptionalArrayView(pTemplateArguments),
                                //                                     MakeArrayView(exps),
                                //                                     CppLiteGetScope()->getSource(),
                                //                                     CppLiteGetInitializationType(), 0);
                                //                                     CPPLITEPARSER_ASSERT(pLHSExp->rtti.metaClass);
                                //                                     if (pElement == nullptr)
                                //                                     {
                                //                                         // then at the instance class source
                                //                                         scope pElement =
                                //                                         CppLiteGetSemantic()->silentUnqualifiedLookup(
                                //                                         identifier,
                                //                                         MakeOptionalArrayView(pTemplateArguments),
                                //                                         MakeOptionalArrayView(pFunctionArguments),
                                //                                         pLHSExp->getValueType()->removePointer()->removeQualifiers()->getSource(),
                                //                                         CppLiteGetInitializationType(), 0);
                                //                                     }
                                //                                     if (pElement &&
                                //                                         (CppLiteGetScope()->hasOwnerCascade(pElement->removeExpression())
                                //                                         ||
                                //                                          pElement->removeExpression()->asFunction()
                                //                                          == nullptr))
                                //                                     {
                                //                                         CPPLITEPARSER_ASSERT(pElement->asExpression());
                                //                                         CppLitesUnguard(exps);
                                //                                         Delete(pElement);
                                //                                         bOverloadCalledInOverload = true;
                                //                                         pElement = nullptr;
                                //                                     }
                                //                                 }
                                if (pElement == nullptr)
                                {
                                    if (Class* pClass = pLHS->asClass()) // the left hand side is a class, it
                                                                         // can be either a static call, either
                                                                         // a super call
                                    {
                                        if (pFunctionArguments) // a call ?
                                        {
                                            Block* pBlock = CppLiteGetScopeAs(Block);
                                            if (pBlock && !pBlock->isTemplateElement())
                                            {
                                                LocalVariable* localthis_ = pBlock->getLocalVariableCascade("this");
                                                if (localthis_)
                                                {
                                                    OwnerGuard<Expression> this_ =
                                                    (NewInScope<LocalVariableExpression>(
                                                     localthis_->getValueType()->removeConst()->removePointer()))
                                                    ->dereference(getSource());
                                                    CPPLITEPARSER_ASSERT(&expressions == pFunctionArguments);
                                                    expressions.insert(expressions.begin(), this_);
                                                    pElement = CppLiteGetSemantic()->silentQualifiedLookup(
                                                    pLHS, identifier, MakeOptionalArrayView(pTemplateArguments),
                                                    MakeOptionalArrayView(pFunctionArguments), CppLiteGetScope(),
                                                    CppLiteGetInitializationType(), 0);
                                                    expressions.erase(expressions.begin());
                                                    if (CallExpression* pCall =
                                                        phantom::Object::Cast<CallExpression>(pElement))
                                                    {
                                                        pCall->setFinal();
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    if (pElement == nullptr) // if left hand side is an expression
                                    {
                                        pElement = CppLiteGetSemantic()->qualifiedLookup(
                                        pLHS, identifier, MakeOptionalArrayView(pTemplateArguments),
                                        MakeOptionalArrayView(pFunctionArguments), CppLiteGetScope(),
                                        CppLiteGetInitializationType(), 0);
                                    }
                                    if (pElement == nullptr && bOverloadCalledInOverload)
                                    {
                                        CppLiteErrorReturn("global overload cannot be accessed with "
                                                           "'.' or '->' inside its own scope");
                                    }
                                }
                            }
                        }
                    }
                    if (pElement == nullptr)
                    {
                        return true;
                    }
                    pLHS = pElement;
                }
            }
            CppLiteUnguard(pLHS);
            CppLiteMapAndReturn(pLHS);
        }
        return true;
    }

    /// 'this' and other keywords
    bool traverseKeywordExpression(ast::KeywordExpression* input)
    {
        switch (input->m_Keyword[1])
        {
        case 'h': // this
        {         //  ^
            Block* pBlock = CppLiteGetScopeAs(Block);
            if (pBlock)
            {
                CppLiteErrorReturnIf(pBlock->getSubroutine()->asMethod() == nullptr,
                                     "this : illegal use in non-method");
                CppLiteMapAndReturn(toExpression(static_cast<Method*>(pBlock->getSubroutine())->getThis()));
            }
            else
            {
                ClassType* pClassType = CppLiteGetScopeAs(ClassType);
                CPPLITEPARSER_ASSERT(pClassType); // field default value
                CppLiteMapAndReturn(NewInScope<LocalVariableExpression>(pClassType));
            }
        }
        break;

        default:
            CPPLITEPARSER_ASSERT(false);
        }
        return true;
    }

    /// nullptr
    bool traverseNullptrExpression(ast::NullptrExpression* input)
    {
        CppLiteMapAndReturn(NewInScope<ConstantExpression>(Application::Get()->getNullptr()));
    }

    /// "" "" "" "" ...
    bool traverseStringLiteralChain(ast::StringLiteralChain* input)
    {
        if (input->m_STRING_LITs == nullptr || input->m_STRING_LITs->empty())
        {
            CppLiteMapAndReturn(
            NewInScope<StringLiteralExpression>(input->m_STRING_LIT, m_Data.unquote(input->m_STRING_LIT)));
        }

        String concatStrings = input->m_STRING_LIT;
        for (auto s : *input->m_STRING_LITs)
        {
            const char* str = s.as_string();
            CPPLITEPARSER_ASSERT(concatStrings.back() == '"');
            concatStrings.pop_back();
            CPPLITEPARSER_ASSERT(*str == '"');
            concatStrings += str + 1;
        }

        CppLiteMapAndReturn(NewInScope<StringLiteralExpression>(concatStrings, m_Data.unquote(concatStrings.c_str())));
    }

    /// break
    bool traverseBreakStatement(ast::BreakStatement* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);
        CppLiteErrorReturnIf(m_Data.getBreakLabel() == nullptr, "break : out of loop or switch ");
        BranchStatement* pStatement = NewInScope<BranchStatement>(m_Data.getBreakLabel());
        pBlock->addStatement(pStatement);
        CppLiteMapAndReturn(pStatement);
    }

    /// continue
    bool traverseContinueStatement(ast::ContinueStatement* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);
        CppLiteErrorReturnIf(m_Data.getContinueLabel() == nullptr, "continue : out of loop");
        BranchStatement* pStatement = NewInScope<BranchStatement>(m_Data.getContinueLabel());
        pBlock->addStatement(pStatement);
        CppLiteMapAndReturn(pStatement);
    }

    /// @assert
    bool traverseAssertStatement(ast::AssertStatement* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);
        Expression* pExpression = CppLiteVisitExpression(input->m_AssignExpression);
        if (pExpression == nullptr)
            return true;
        Expression* pConv = CppLiteConv(pExpression, PHANTOM_TYPEOF(bool));
        if (pConv == nullptr)
            return true;

        Block* pIfStatement = pBlock->newBlock(Block::e_Tag_Assert);
        Label* pEndLabel = NewInScope<Label>(Label::e_Tag_Endif);
        auto*  pBranchIf = NewInScope<BranchIfStatement>(pConv, pEndLabel);
        pIfStatement->addStatement(pBranchIf);
        String      text = m_Data.getSourceText(input->m_AssignExpression);
        Expressions args;
        args.push_back(NewInScope<StringLiteralExpression>('"' + text + '"'));
        auto stream = getSource()->getSourceStream();
        if (stream)
        {
            args.push_back(NewInScope<StringLiteralExpression>('"' + stream->getPath() + '"'));
        }
        else
        {
            args.push_back(NewInScope<StringLiteralExpression>("\"\""));
        }
        args.push_back(NewInScope<ConstantExpression>(createConstant(input->location().begin.line)));

        Label* pAssertNoBreakLabel = NewInScope<Label>();
        pIfStatement->addStatement(NewInScope<BranchIfNotStatement>(
        NewInScope<CallExpression>(m_Data.m_pAssertHandlerFunc, args), pAssertNoBreakLabel));
        pIfStatement->addStatement(NewInScope<CallExpression>(m_Data.m_pBreakFunc));
        pIfStatement->addStatement(pAssertNoBreakLabel);
        pIfStatement->addStatement(pEndLabel);

        return true;
    }

    /// @static_assert
    bool _traverseStaticAssert(ast::StaticAssert* input)
    {
        LanguageElement* pScope = CppLiteGetScopeAs(LanguageElement);
        Expression*      pExpression = CppLiteVisitExpression(input->m_AssignExpression);
        if (pExpression == nullptr)
            return true;
        CppLiteGuard(pExpression);
        String text = m_Data.getSourceText(input->m_AssignExpression);
        if (pExpression->isTemplateDependant())
        {
            NewInScope<StaticAssert>(pExpression, text)->setOwner(pScope);
            return true;
        }
        CppLiteErrorReturnIf(!pExpression->isCompileTime(),
                             "@static_assert : test expression must be a compile-time constant expression");
        Expression* pConv = CppLiteConv(pExpression, PHANTOM_TYPEOF(bool));
        if (pConv == nullptr)
            return true;
        bool value = false;
        pConv->load(&value);
        CppLiteGuard(pConv);

        CppLiteErrorReturnIf(!value, "static assertion failed : %s", text.data());

        return true;
    }

    /// type traits (@is_same ...)
    bool traverseTypeTraitBinary(ast::TypeTraitBinary* input)
    {
        Type* pType0 = CppLiteVisitType(input->m_Type);
        if (pType0 == nullptr)
            return true;
        Type* pType1 = CppLiteVisitType(input->m_Type0);
        if (pType1 == nullptr)
            return true;
        TypeTraitExpression::ETypeTrait trait = TypeTraitExpression::e_TypeTait_Undefined;
        if (strcmp(input->m_IS_SAME, "@is_same") == 0)
        {
            trait = TypeTraitExpression::e_TypeTait_IsSame;
        }
        if (pType0->isTemplateDependant() || pType1->isTemplateDependant())
        {
            CppLiteMapAndReturn(NewInScope<TypeTraitExpression>(trait, Types{pType0, pType1}));
        }
        CppLiteMapAndReturn(
        NewInScope<ConstantExpression>(createConstant(TypeTraitExpression::Resolve(trait, Types{pType0, pType1}))));
    }

    bool traverseCStyleCastExpression(ast::CStyleCastExpression* input);

    bool traverseFundamentalTypeFunctionCast(ast::FundamentalTypeFunctionCast* input);

    bool traversePrimaryExpression(ast::PrimaryExpression* input)
    {
        if (input->m_INT_VALUE.hasValue())
        {
            CppLiteMapAndReturn(CppLiteConstantExpression(input->m_INT_VALUE));
        }
        else if (input->m_Name)
        {
            LanguageElement*  pLHS = nullptr;
            LanguageElements* pTemplateArgs = nullptr;
            LanguageElements  templateArgs;
            CppLitesGuard(templateArgs);
            if (input->m_Name->m_TemplateArgumentList)
            {
                pTemplateArgs = &templateArgs;
                if (!resolveTemplateArguments(input->m_Name->m_TemplateArgumentList, templateArgs))
                    return true;
            }

            String identifier = NameToString(input->m_Name);
            if (input->m_DOT.hasValue())
            {
                if (input->m_DOT.as_string()[0] == '.')
                {
                    pLHS = phantom::lang::Application::Get()->rootPackageFolder();
                }
                else
                {
                    pLHS = phantom::lang::Namespace::Global();
                }

                pLHS = CppLiteGetSemantic()->qualifiedLookup(pLHS, identifier, MakeOptionalArrayView(pTemplateArgs),
                                                             phantom::NullOpt, CppLiteGetScope());
            }
            else
            {
                pLHS = CppLiteGetSemantic()->unqualifiedLookup(identifier, MakeOptionalArrayView(pTemplateArgs),
                                                               phantom::NullOpt, CppLiteGetScope());
            }
            if (pLHS == nullptr)
                return true;
            _checkForPackParameterExpansion(pLHS);
            CppLiteMapAndReturn(pLHS);
        }
        else if (input->m_NullptrExpression)
        {
            return this->traverse(input->m_NullptrExpression);
        }
        else if (input->m_StringLiteralChain)
        {
            return this->traverse(input->m_StringLiteralChain);
        }
        else if (input->m_KeywordExpression)
        {
            return this->traverse(input->m_KeywordExpression);
        }
        else if (input->m_CStyleCastExpression)
        {
            return this->traverse(input->m_CStyleCastExpression);
        }
        else if (input->m_SpecialCastExpression)
        {
            return this->traverse(input->m_SpecialCastExpression);
        }
        else if (input->m_FunctionPtrExpression)
        {
            return this->traverse(input->m_FunctionPtrExpression);
        }
        else if (input->m_LambdaExpression)
        {
            return this->traverse(input->m_LambdaExpression);
        }
        CPPLITEPARSER_ASSERT(false);
        return true;
    }

    bool traverseEnum(ast::Enum* input)
    {
        Scope* pScope = CppLiteGetScopeAs(Scope);
        if (!pScope)
            return true;
        if (CppLiteGetElementAs(Enum))
            return true;
        CppLiteErrorReturnIfNameUsed(pScope->asLanguageElement(), input->m_IDENTIFIER);
        Enum* pEnum = NewInScope<Enum>(input->m_IDENTIFIER, (PrimitiveType*)PHANTOM_TYPEOF(int));
        if (input->m_CLASS.hasValue())
            pEnum->setScoped();
        pEnum->setAccess(CppLiteGetAccess());
        pScope->addType(pEnum);
        if (input->m_EnumConstants)
        {
            phantom::ulonglong      currVal = 0;
            phantom::lang::TypeKind biggestTypeId = phantom::lang::TypeKind::UInt;
            phantom::Strings        names;
            Constants               constants;
            CppLitesGuardScoped(constants);
            for (auto it = input->m_EnumConstants->begin(); it != input->m_EnumConstants->end(); ++it)
            {
                ast::EnumConstant* pValue = *it;
                Expression*        pConstantExpression = CppLiteVisitExpression(pValue, pEnum);
                names.push_back(pValue->m_IDENTIFIER);
                if (pConstantExpression)
                {
                    pConstantExpression->eval(&currVal);
                    Delete(pConstantExpression);
                }
                pEnum->addConstant(
                pEnum->getUnderlyingIntType()->createConstant(pEnum, &currVal, pValue->m_IDENTIFIER, pEnum));
                currVal++;
            }
        }
        CppLiteMapAndReturn(pEnum);
        return true;
    }
    bool traverseEnumConstant(ast::EnumConstant* input)
    {
        if (input->m_AssignExpression)
        {
            Expression* pExpression = CppLiteVisitExpression(input->m_AssignExpression);
            CppLiteErrorReturnIf(pExpression == nullptr, "enum value '%s' : invalid expression", input->m_IDENTIFIER);
            CppLiteErrorReturnIf(!(pExpression->isCompileTime()) || !(pExpression->getValueType()->asIntegralType()),
                                 "enum value '%s' : expression must be a compile-time integral value",
                                 input->m_IDENTIFIER);
            CppLiteMapAndReturn(pExpression);
        }
        return true;
    }

    template<class T>
    T* applyCodeRange(ast::_BaseRule* input, T* a_pElem)
    {
        if (a_pElem)
            CppLiteSetCodeRange(a_pElem, CppLiteCodeRange(input->location()));
        return a_pElem;
    }
    template<class T>
    T* applyCodeRangeNoEnd(ast::_BaseRule* input, T* a_pElem)
    {
        if (a_pElem)
            CppLiteSetCodeRange(a_pElem, CppLiteCodeRangeNoEnd(input->location()));
        return a_pElem;
    }

    template<class TT>
    LanguageElement* resolveElement(TT* input, LanguageElement* a_pScope, LanguageElements* a_pTemplateArguments,
                                    Expressions* a_pFunctionArguments, Type* a_pInitializationType)
    {
        LanguageElement* pElement = nullptr;
        m_Data.pushScope(a_pScope);
        m_Data.pushTemplateArguments(a_pTemplateArguments);
        m_Data.pushFunctionArguments(a_pFunctionArguments);
        m_Data.pushInitializationType(a_pFunctionArguments);
        m_Data.pushReturnAddress(&pElement);
        m_Data.m_pSemantic->pushCodeRangeLocation(CppLiteCodeRangeLocation(input->location()));
        this->visit(input);
        m_Data.m_pSemantic->popCodeRangeLocation();
        m_Data.popScope();
        m_Data.popTemplateArguments();
        m_Data.popFunctionArguments();
        m_Data.popInitializationType();
        m_Data.popReturnAddress();
        return pElement;
    }
    template<class TT>
    LanguageElement* resolveElement(TT* input, LanguageElement* a_pScope, LanguageElements* a_pTemplateArguments,
                                    Expressions* a_pFunctionArguments)
    {
        LanguageElement* pElement = nullptr;
        m_Data.pushScope(a_pScope);
        m_Data.pushTemplateArguments(a_pTemplateArguments);
        m_Data.pushFunctionArguments(a_pFunctionArguments);
        m_Data.pushReturnAddress(&pElement);
        m_Data.m_pSemantic->pushCodeRangeLocation(CppLiteCodeRangeLocation(input->location()));
        this->visit(input);
        m_Data.m_pSemantic->popCodeRangeLocation();
        m_Data.popScope();
        m_Data.popTemplateArguments();
        m_Data.popFunctionArguments();
        m_Data.popReturnAddress();
        return pElement;
    }
    template<class TT>
    LanguageElement* resolveElement(TT* input, LanguageElement* a_pScope, Expressions* a_pFunctionArguments)
    {
        LanguageElement* pElement = nullptr;
        m_Data.pushScope(a_pScope);
        m_Data.pushFunctionArguments(a_pFunctionArguments);
        m_Data.pushReturnAddress(&pElement);
        m_Data.m_pSemantic->pushCodeRangeLocation(CppLiteCodeRangeLocation(input->location()));
        this->traverse(input);
        m_Data.m_pSemantic->popCodeRangeLocation();
        m_Data.popScope();
        m_Data.popFunctionArguments();
        m_Data.popReturnAddress();
        return pElement;
    }
    template<class TT>
    LanguageElement* resolveElement(TT* input, Expressions* a_pFunctionArguments)
    {
        LanguageElement* pElement = nullptr;
        m_Data.pushFunctionArguments(a_pFunctionArguments);
        m_Data.pushReturnAddress(&pElement);
        m_Data.m_pSemantic->pushCodeRangeLocation(CppLiteCodeRangeLocation(input->location()));
        this->traverse(input);
        m_Data.m_pSemantic->popCodeRangeLocation();
        m_Data.popFunctionArguments();
        m_Data.popReturnAddress();
        return pElement;
    }
    template<class TT>
    LanguageElement* resolveElement(TT* input, LanguageElement* a_pScope, LanguageElements* a_pTemplateArguments)
    {
        LanguageElement* pElement = nullptr;
        m_Data.pushScope(a_pScope);
        m_Data.pushTemplateArguments(a_pTemplateArguments);
        m_Data.pushReturnAddress(&pElement);
        m_Data.m_pSemantic->pushCodeRangeLocation(CppLiteCodeRangeLocation(input->location()));
        this->traverse(input);
        m_Data.m_pSemantic->popCodeRangeLocation();
        m_Data.popScope();
        m_Data.popTemplateArguments();
        m_Data.popReturnAddress();
        return pElement;
    }
    template<class TT>
    LanguageElement* resolveElement(TT* input, LanguageElement* a_pScope)
    {
        LanguageElement* pElement = nullptr;
        m_Data.pushScope(a_pScope);
        m_Data.pushReturnAddress(&pElement);
        m_Data.m_pSemantic->pushCodeRangeLocation(CppLiteCodeRangeLocation(input->location()));
        this->traverse(input);
        m_Data.m_pSemantic->popCodeRangeLocation();
        m_Data.popScope();
        m_Data.popReturnAddress();
        return pElement;
    }
    template<class TT>
    LanguageElement* resolveElements(TT* input, LanguageElement* a_pScope)
    {
        LanguageElement* pElement = nullptr;
        m_Data.pushScope(a_pScope);
        m_Data.pushReturnAddress(&pElement);
        this->traverse(input);
        m_Data.popScope();
        m_Data.popReturnAddress();
        return pElement;
    }
    template<class TT>
    LanguageElement* resolveElement(TT* input)
    {
        LanguageElement* pElement = nullptr;
        m_Data.pushReturnAddress(&pElement);
        m_Data.m_pSemantic->pushCodeRangeLocation(CppLiteCodeRangeLocation(input->location()));
        this->traverse(input);
        m_Data.m_pSemantic->popCodeRangeLocation();
        m_Data.popReturnAddress();
        return pElement;
    }

#define CppLitePass_resolve_body(expr, cast_expr)                                                                      \
    {                                                                                                                  \
        LanguageElement* pElement = expr;                                                                              \
        if (pElement)                                                                                                  \
        {                                                                                                              \
            auto pResult = pElement cast_expr;                                                                         \
            if (pResult)                                                                                               \
                return pResult;                                                                                        \
            CppLiteGuard(pElement);                                                                                    \
        }                                                                                                              \
        return nullptr;                                                                                                \
    }

#define CppLitePass_resolve(_type_, cast_expr)                                                                         \
    template<class TT>                                                                                                 \
    _type_* resolve##_type_(TT* input, LanguageElement* a_pScope)                                                      \
    CppLitePass_resolve_body(resolveElement(input, a_pScope), cast_expr) template<class TT>                            \
    _type_* resolve##_type_(TT* input) CppLitePass_resolve_body(resolveElement(input), cast_expr)

    CppLitePass_resolve(Parameter, ->asParameter());

    template<class TT>
    Symbol* resolveSymbol(TT* input)
    {
        LanguageElement* pElement = resolveElement(input);
        if (pElement)
        {
            auto pResult = pElement->asSymbol();
            if (pResult)
            {
                CppLiteSetCodeRange(pResult, CppLiteCodeRange(input->location()));
                return pResult;
            }
            CppLiteGuard(pElement);
        }
        return nullptr;
    }

    template<class TT>
    Symbol* resolveSymbol(TT* input, LanguageElement* a_pScope)
    {
        LanguageElement* pElement = resolveElement(input, a_pScope);
        if (pElement)
        {
            auto pResult = pElement->asSymbol();
            if (pResult)
            {
                CppLiteSetCodeRange(pResult, CppLiteCodeRange(input->location()));
                return pResult;
            }
            CppLiteGuard(pElement);
        }
        return nullptr;
    }

    template<class TT>
    Expression* _resolveAsExpression(TT* input, LanguageElement* a_pElement)
    {
        Expression* pExp = a_pElement->asExpression();
        if (!pExp)
        {
            Expression* pLeft = m_Data.getImplicitThisExpression()
            ? m_Data.getImplicitThisExpression()->clone(a_pElement)->dereference(a_pElement)
            : nullptr;
            CppLiteGuard(pLeft);
            pExp = toExpression(a_pElement, pLeft);
        }
        if (pExp)
        {
            CppLiteSetCodeRange(pExp, CppLiteCodeRange(input->location()));
            return pExp;
        }
        CppLiteGuard(a_pElement);
        return nullptr;
    }

    template<class TT>
    Expression* resolveExpression(TT* input, LanguageElement* a_pScope)
    {
        LanguageElement* pElement = resolveElement(input, a_pScope);
        if (pElement)
        {
            return _resolveAsExpression(input, pElement);
        }
        return nullptr;
    }

    template<class TT>
    Expression* resolveExpression(TT* input)
    {
        LanguageElement* pElement = resolveElement(input);
        if (pElement)
        {
            return _resolveAsExpression(input, pElement);
        }
        return nullptr;
    }

    template<class TT>
    Type* resolveType(TT* input, LanguageElement* a_pScope)
    {
        CPPLITEPARSER_ASSERT(a_pScope);
        LanguageElement* pElement = resolveElement(input, a_pScope);
        if (pElement)
        {
            if (auto pResult = pElement->toType())
                return pResult;
            if (auto pSym = pElement->asSymbol())
            {
                CppLiteErrorMsg("%.*s : not a type", PHANTOM_STRING_AS_PRINTF_ARG(pSym->getName()));
            }
            else
            {
                CppLiteErrorMsg("expression does not produce a type");
            }
            CppLiteGuard(pElement);
        }
        return nullptr;
    }
    template<class TT>
    Type* resolveType(TT* input)
    {
        LanguageElement* pElement = resolveElement(input);
        if (pElement)
        {
            if (auto pResult = pElement->toType())
                return pResult;
            if (auto pSym = pElement->asSymbol())
            {
                CppLiteErrorMsg("%.*s : not a type", PHANTOM_STRING_AS_PRINTF_ARG(pSym->getName()));
            }
            else
            {
                CppLiteErrorMsg("expression does not produce a type");
            }
            CppLiteGuard(pElement);
        }
        return nullptr;
    }

    bool resolveAlias(ast::Alias* input)
    {
        TemplateSpecialization* pTemplateSpec = nullptr;

        if (!(resolveTemplateSpecialization(input, nullptr, pTemplateSpec, false)))
            return true;

        Scope* pScope = CppLiteGetScopeAs(Scope);
        if (pTemplateSpec == nullptr)
        {
            CppLiteErrorReturnIfNameUsed(pScope->asLanguageElement(), input->m_IDENTIFIER);
        }

        Alias* pAlias = NewInScope<Alias>(input->m_IDENTIFIER);
        pAlias->setAccess(CppLiteGetAccess());

        CppLiteGuard(pAlias);

        if (pTemplateSpec)
        {
            pTemplateSpec->setTemplated(pAlias);
        }
        else
        {
            pScope->addAlias(pAlias);
        }
        pAlias->setVisibility(Visibility::Public);

        Symbol* pElement = CppLiteVisitSymbol(input->m_Type, pAlias);
        CppLiteErrorReturnIf(pElement == nullptr, "alias : invalid aliased symbol");

        pAlias->setAliasedSymbol(pElement);

        CppLiteUnguard(pAlias);
        CppLiteMapAndReturn(pAlias);
        return true;
    }

    bool _CheckTemplateParamReferenced(Placeholder* pPlaceHolder, LanguageElement* pArg)
    {
        if (Placeholder* pPH = pArg->asPlaceholder())
        {
            if (pPH->asSymbol()->isSame(pPlaceHolder->asSymbol()))
            {
                return true;
            }
        }
        else if (Type* pType = pArg->asType())
        {
            if (auto* pPackExpans = Object::Cast<TemplateParameterPackTypeExpansion>(pArg))
            {
                return pPackExpans->getPackArgument()->asSymbol()->isSame(pPlaceHolder->asSymbol());
            }
            auto naked = pType->removeEverything();
            if (Placeholder* pNakedAsPlaceholder = naked->asPlaceholder())
            {
                if (pNakedAsPlaceholder->asSymbol()->isSame(pPlaceHolder->asSymbol()))
                {
                    return true;
                }
            }
            else if (FunctionType* pFT = naked->asFunctionType())
            {
                if (_CheckTemplateParamReferenced(pPlaceHolder, pFT->getReturnType()))
                    return true;
                for (auto pParamType : pFT->getParameterTypes())

                    if (_CheckTemplateParamReferenced(pPlaceHolder, pParamType))
                        return true;
            }
        }
        return false;
    }

    template<class T>
    bool resolveTemplateSpecialization(T* input, ast::TemplateArgumentList* a_pTemplateArgumentList,
                                       TemplateSpecialization*& a_pTemplateSpec, bool isSubroutine)
    {
        CppLiteDefaultReturnValue(false);
        Scope* pScope = CppLiteGetScopeAs(Scope);
        CPPLITEPARSER_ASSERT(pScope);
        TemplateSignature* pTemplateSignature = CppLiteGetTemplateSignature();
        Template*          pTemplate = nullptr;
        if (pTemplateSignature)
        {
            pTemplate = pScope->getTemplate(input->m_IDENTIFIER);
            CppLiteErrorReturnIf(pTemplate != nullptr && a_pTemplateArgumentList == nullptr,
                                 "'%s' : a template with same name already exists in the current scope",
                                 input->m_IDENTIFIER);
            Scope* pNamingScope = pScope;
            if (Source* pSource = pScope->asLanguageElement()->asSource())
            {
                Package* pPackage = pSource->getPackage();
                for (auto pPackageInFolder : pPackage->getPackageFolder()->getPackages())
                {
                    if (pPackageInFolder == pPackage)
                        continue;
                    if (Source* pSiblingSource = pPackageInFolder->getSource(pSource->getName()))
                    {
                        if (pSiblingSource->isNative())
                        {
                            // extending native template
                            if ((pTemplate = pSiblingSource->getTemplate(input->m_IDENTIFIER)))
                            {
                                CppLiteErrorReturnIf(pTemplateSignature == nullptr,
                                                     "'%s' : extending native template requires empty or partial "
                                                     "specialization, full provided",
                                                     input->m_IDENTIFIER);
                                if (a_pTemplateArgumentList == nullptr)
                                {
                                    CppLiteErrorReturnIf(
                                    pTemplate->getEmptyTemplateSpecialization()->getTemplated(),
                                    "'%s' : template has already been extended in source '%.*s'", input->m_IDENTIFIER,
                                    PHANTOM_STRING_AS_PRINTF_ARG(pTemplate->getEmptyTemplateSpecialization()
                                                                 ->getTemplated()
                                                                 ->getSource()
                                                                 ->getName()));
                                    CppLiteErrorIf(
                                    pTemplateSignature->getTemplateParameters().size() !=
                                    pTemplate->getTemplateSignature()->getTemplateParameters().size(),
                                    "'%s' : incompatible number of template parameter between native and "
                                    "runtime build override",
                                    input->m_IDENTIFIER);
                                    for (size_t i = 0; i < pTemplateSignature->getTemplateParameters().size(); ++i)
                                    {
                                        if (LanguageElement* pArg = pTemplateSignature->getDefaultArgument(i))
                                            if (pTemplate->getDefaultArgument(i) == nullptr)
                                                pTemplate->getTemplateParameters()[i]->setDefaultArgument(pArg);
                                    }
                                    pTemplateSignature->getSource()->Delete(
                                    pTemplateSignature); // destroy obstolete signature
                                    pTemplateSignature = pTemplate->getTemplateSignature()->clone(getSource());
                                }
                            }
                        }
                    }
                }
            }
        }
        if (a_pTemplateArgumentList)
        {
            CppLiteErrorReturnIf(pTemplateSignature == nullptr,
                                 "'%s' : template specialization : missing template<...> signature",
                                 input->m_IDENTIFIER);
            pTemplateSignature->setOwner(pScope->asLanguageElement());
            LanguageElements specializationArguments;
            if (!resolveTemplateArguments(a_pTemplateArgumentList, pTemplateSignature, specializationArguments))
            {
                pTemplateSignature->setOwner(nullptr);
                return true;
            }

            for (auto pTemplateParam : pTemplateSignature->getTemplateParameters())
            {
                Placeholder* pPlaceHolder = pTemplateParam->getPlaceholder();

                bool referenced = false;

                for (auto pArg : specializationArguments)
                {
                    if ((referenced = _CheckTemplateParamReferenced(pPlaceHolder, pArg)))
                        break;
                    // FIXME
                    //                     for (LanguageElement* pReferencing :
                    //                     pPlaceHolder->asSymbol()->getReferencingElements())
                    //                     {
                    //                         if (pReferencing->isSame(pArg) ||
                    //                         pReferencing->hasOwnerCascade(pArg))
                    //                         {
                    //                             referenced = true;
                    //                             break;
                    //                         }
                    //                     }
                }
                CppLiteErrorReturnIf(!referenced,
                                     "'%s' : template parameter not referenced in specialization arguments",
                                     pTemplateParam->getName().data());
            }

            if (!pTemplate)
            {
                pTemplate = pScope->getTemplate(input->m_IDENTIFIER);
            }
            CppLiteErrorReturnIf(pTemplate && a_pTemplateArgumentList == nullptr,
                                 "'%s' : redefined template in '%.*s' previously defined in '%.*s'",
                                 input->m_IDENTIFIER, PHANTOM_STRING_AS_PRINTF_ARG(CppLiteGetScope()->getName()),
                                 PHANTOM_STRING_AS_PRINTF_ARG(pTemplate->getOwner()->getQualifiedName()));
            PlaceholderMap specializationMap;

            if (!pTemplate)
            {
                Symbols symbols;
                CppLiteGetSemantic()->silentUnqualifiedSymbolsLookup(input->m_IDENTIFIER, symbols, nullptr,
                                                                     pScope->asLanguageElement());
                CppLiteErrorReturnIf(symbols.empty(), "'%s' : undefined template in '%.*s'", input->m_IDENTIFIER,
                                     PHANTOM_STRING_AS_PRINTF_ARG(CppLiteGetScope()->getName()));
                CppLiteErrorReturnIf(symbols.front()->asTemplate() == nullptr, "'%s' : expected template in '%.*s'",
                                     input->m_IDENTIFIER, PHANTOM_STRING_AS_PRINTF_ARG(CppLiteGetScope()->getName()));
                CPPLITEPARSER_ASSERT(symbols.size() == 1, "'%s' : multiple definition of template in '%.*s'",
                                     input->m_IDENTIFIER, PHANTOM_STRING_AS_PRINTF_ARG(CppLiteGetScope()->getName()));
                pTemplate = static_cast<Template*>(symbols.front());
            }
            // if (data.hasFlag(CppLite::e_Flag_CppCompatible))
            {
                // If C++ compatible, ensure the specialization matches the 'primary' template
                CppLiteErrorReturnIf((pTemplate->getTemplateSignature()->getTemplateParameters().size() -
                                      pTemplate->getTemplateSignature()->isVariadic()) > specializationArguments.size(),
                                     "'%s' : template specialization : missing specialization arguments to match "
                                     "primary template signature, %zu expected",
                                     input->m_IDENTIFIER, pTemplate->getTemplateParameters().size());
                CppLiteErrorReturnIf(pTemplate->getTemplateSignature()->getTemplateParameters().size() <
                                     specializationArguments.size(),
                                     "'%s' : template specialization : too many specialization arguments to match "
                                     "primary template signature, %zu expected",
                                     input->m_IDENTIFIER, pTemplate->getTemplateParameters().size());
            }
            //                 else
            //                 {
            //                     // If not C++ compatible we don't have a primary template
            //                     (like in D language) pTemplate =
            //                     NewInScope<Template>(a_pName, CppLiteGetAccess());
            //                 }

            //
            //                 for (size_t i = 0; i < specializationPlaceholders.size(); ++i)
            //                 {
            //                     LanguageElement* pPrimaryPlaceholder =
            //                     pTemplate->getEmptyTemplateSpecialization()->getArgument(i);
            //                     CppLiteErrorReturnIf(!(pPrimaryPlaceholder->isSame(specializationPlaceholders[i])),
            //                     "%s '%s' : specialization : template parameter type %d does
            //                     not match primary template parameter type", a_pContext,
            //                     a_pName, i);
            //                 }

            a_pTemplateSpec = pTemplate->getTemplateSpecialization(specializationArguments);
            CppLiteErrorReturnIf(a_pTemplateSpec && (!a_pTemplateSpec->isNative() || a_pTemplateSpec->isFull()),
                                 "'%s' : template specialization already defined", input->m_IDENTIFIER);
            pTemplateSignature->setOwner(nullptr);
            a_pTemplateSpec =
            pTemplate->createTemplateSpecialization(specializationArguments, nullptr, pTemplateSignature);
            pScope->addTemplateSpecialization(a_pTemplateSpec);
            CPPLITEPARSER_ASSERT(!a_pTemplateSpec->isEmpty());
        }
        else
        {
            if (pTemplateSignature)
            {
                if (pTemplate == nullptr)
                {
                    pTemplate = pScope->getTemplate(input->m_IDENTIFIER);
                    CppLiteErrorReturnIf(pTemplate, "'%s' : template already defined", input->m_IDENTIFIER);
                    if (!isSubroutine)
                        CppLiteErrorReturnIfNameUsed(pScope->asLanguageElement(), input->m_IDENTIFIER);
                    pTemplate = NewInScope<Template>(pTemplateSignature, input->m_IDENTIFIER);
                    pTemplate->setAccess(CppLiteGetAccess());
                    Source* pSource = getSource();
                    pScope->addTemplate(pTemplate);
                    if (pScope == pSource)
                        pSource->getPackage()->getCounterpartNamespace()->addTemplate(pTemplate);
                    pScope->addTemplateSpecialization(pTemplate->getEmptyTemplateSpecialization());
                    // TODO : remove automatic Public assignmen to symbol visibility
                    pTemplate->getEmptyTemplateSpecialization()->setVisibility(Visibility::Private);
                }
                a_pTemplateSpec = pTemplate->getEmptyTemplateSpecialization();
                if (a_pTemplateSpec->isNative()) // if native, we need to build and extended version
                {
                    auto ext = a_pTemplateSpec->clone(getSource(), 0);
                    a_pTemplateSpec->setExtendedSpecialization(ext);
                    pScope->addTemplateSpecialization(ext);
                    // add an alias template in the cpplite source to be able to still solve qualified lookups from
                    // here
                    {
                        Template* pAliasedTemplate =
                        NewInScope<Template>(pTemplateSignature->clone(CppLiteGetSource()), pTemplate->getName());
                        LanguageElements args;
                        for (auto p : pAliasedTemplate->getTemplateSignature()->getTemplateParameters())
                        {
                            args.push_back(p->getPlaceholder()->asSymbol());
                        }
                        Type*  pType = NewInScope<TemplateDependantTemplateInstance>(ext, args);
                        Alias* pAlias = NewInScope<Alias>(pType, pAliasedTemplate->getName());
                        if (auto pNS = pScope->asNamespace())
                            pNS->addAlias(pAlias);
                        pAliasedTemplate->getEmptyTemplateSpecialization()->setTemplated(pAlias);
                        pScope->addTemplate(pAliasedTemplate);
                        pAliasedTemplate->setVisibility(Visibility::Protected);
                    }
                    a_pTemplateSpec = ext;
                }
                CPPLITEPARSER_ASSERT(a_pTemplateSpec);
            }
            else
                a_pTemplateSpec = nullptr;
        }
        return true;
    }

    bool resolveClass(ast::Class* input, Class*& pClass)
    {
        CppLiteDefaultReturnValue(false);
        TemplateSpecialization* pTemplateSpec = nullptr;

        if (!(resolveTemplateSpecialization(input, input->m_TemplateArgumentList, pTemplateSpec, false)))
            return false;

        const char* classOrStructName = input->m_CLASS;
        const char* name = input->m_IDENTIFIER;

        Scope* pScope = CppLiteGetScopeAs(Scope);
        if (pTemplateSpec == nullptr)
        {
            CPPLITEPARSER_ASSERT(pScope);
            CppLiteErrorReturnIfNameUsed(pScope->asLanguageElement(), name);
        }

        if (input->m_MetaClassConstructionExpression)
        {
            Expression* pExp = CppLiteVisitExpression(input->m_MetaClassConstructionExpression);
            if (pExp == nullptr)
                return false;
            Class* pMetaClass = pExp->getValueType()->removeRValueReference()->asClass();
            CppLiteGuard(pExp);
            CppLiteErrorReturnIf(!(pMetaClass->isA(PHANTOM_CLASSOF(Class))),
                                 "meta class expression must be a phantom.lang.Class construction expression");
            void* pAlloc = pMetaClass->allocate();
            pExp->load(pAlloc); // construct
            pClass = reinterpret_cast<Class*>(getSource()->PlacementInit(pMetaClass, pAlloc));
        }
        else
        {
            pClass = NewInScope<Class>(input->m_IDENTIFIER);
            pClass->setAccess(CppLiteGetAccess());
        }
        if (input->m_AlignAs.hasValue())
        {
            int align = input->m_AlignAs.as_int();
            CppLiteErrorReturnIf(align == 0, "'alignas' : forbidden zero alignment", align);
            CppLiteErrorReturnIf(!(isPowerOfTwo(align)), "'alignas' : expects a power of two, given '%d'", align);
            pClass->setAlignment(align);
        }
        if (classOrStructName[0] == 's')
            pClass->setDefaultAccess(Access::Public);
        else
            pClass->setDefaultAccess(Access::Private);

        if (pTemplateSpec)
        {
            if (auto pSrc = pScope->asLanguageElement()->asSource())
            {
                pTemplateSpec->getTemplate()->getNamespace()->addType(pClass);
            }
            pTemplateSpec->setTemplated(pClass);
        }
        else
        {
            for (auto pType : pScope->getTypes())
            {
                if (pType->getName() == pClass->getName())
                {
                    CppLiteErrorReturn(
                    "'%.*s' : type already defined in '%.*s'",
                    PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClass)),
                    PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pScope->asLanguageElement())));
                }
            }
            if (auto pSrc = pScope->asLanguageElement()->asSource())
            {
                for (auto pSym : pSrc->getPackage()->getCounterpartNamespace()->getSymbols(pClass->getNameHash()))
                {
                    if (pSym->getName() == pClass->getName())
                    {
                        CppLiteErrorReturn("'%.*s' : symbol already defined in namespace '%.*s'",
                                           PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pSym)),
                                           PHANTOM_STRING_AS_PRINTF_ARG(
                                           CppLiteElementToString(pSrc->getPackage()->getCounterpartNamespace())));
                    }
                }
            }

            pScope->addType(pClass);
        }
        return true;
    }

    bool resolveClassInheritance(ast::Class* input, Class* pClass)
    {
        CppLiteDefaultReturnValue(false);
        if (input->m_BaseClasses)
        {
            const char* name = input->m_IDENTIFIER;
            for (size_t i = 0; i < input->m_BaseClasses->size(); ++i)
            {
                ast::BaseClass* pBC = (*input->m_BaseClasses)[i];
                Type*           pType = CppLiteVisitType(pBC->m_BasicType, pClass);
                CppLiteErrorReturnIf(pType == nullptr, "'%s' : inheritance : base class %zu : unknown class '%s'",
                                     input->m_IDENTIFIER, i, CppLiteRuleToString(pBC).data());

                Class* pBaseClass = pType->asClass();
                if (pBaseClass == nullptr)
                {
                    if (pType->isTemplateDependant() &&
                        pType->getMetaClass() == PHANTOM_CLASSOF(TemplateDependantTemplateInstance))
                    {
                        pBaseClass = static_cast<TemplateDependantTemplateInstance*>(pType)->promoteAsClass();
                    }
                }
                CppLiteErrorReturnIf(pBaseClass == nullptr,
                                     "'%s' : inheritance : base class %zu : type '%s' is not a class", name, i,
                                     CppLiteRuleToString(pBC).data());
                CppLiteAddTypeDependency(pType);
                if (!pBaseClass->isTemplateDependant() && !pClass->isTemplateDependant())
                {
                    CppLiteErrorReturnIf(pClass->isSame(pBaseClass),
                                         "'%s' : inheritance : base class %zu : class cannot inherit from itself", name,
                                         i);
                    CppLiteErrorReturnIf(pClass->hasBaseClass(pBaseClass),
                                         "'%s' : inheritance : base class %zu : '%.*s' is already a base class", name,
                                         i, PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pBaseClass)));
                    CppLiteErrorReturnIf(pBaseClass->isA(pClass),
                                         "'%s' : inheritance : base class %zu : illegal cyclic "
                                         "inheritance, class '%.*s' derives itself from the current class",
                                         name, i, PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pBaseClass)));
                    CppLiteErrorReturnIf(pClass->isA(pBaseClass),
                                         "'%s' : inheritance : base class %zu : forbidden diamond or other dirty "
                                         "inheritance, class '%.*s' already derived in hierarchy",
                                         name, i, PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pBaseClass)));
                    CppLiteErrorReturnIf(pBaseClass->isNative() && pBaseClass->isAbstract(),
                                         "'%s' : base class %zu : class '%.*s' is a native abstract class "
                                         "and thus cannot be inherited",
                                         name, i, PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pBaseClass)));
                }
                Access access = Access::Public;
                if (pBC->m_PUBLIC.hasValue())
                {
                    switch (pBC->m_PUBLIC.as_string()[2])
                    {
                    case 'b':
                        access = (Access::Public);
                        break;
                    case 'o':
                        access = (Access::Protected);
                        break;
                    case 'i':
                        access = (Access::Private);
                        break;
                    default:
                        CPPLITEPARSER_ASSERT(false);
                    }
                }

                Type* pDepBase = pBaseClass;

                Source* pDepSource = pDepBase->getSource();
                if (auto spec = pDepBase->getTemplateSpecialization())
                {
                    if (auto instSpec = spec->getInstantiationSpecialization())
                        spec = instSpec;
                    pDepSource = spec->getSource();
                }
                else if (pDepBase->isTemplateDependant())
                {
                    if (auto* pTDProm = phantom::Object::Cast<TemplateDependantClassPromotion>(pDepBase))
                    {
                        pDepBase = pTDProm->getBase();
                    }
                    if (auto* pTDTI = phantom::Object::Cast<TemplateDependantTemplateInstance>(pDepBase))
                    {
                        if (auto pSpec = pTDTI->getTemplateSpecialization())
                            pDepSource = pSpec->getSource();
                        else
                            pDepSource = pTDTI->getTemplate()->getSource();
                    }
                }
                CompiledSource::Get(pClass->getSource())->addBuildDependency(pDepSource);
                pClass->addBaseClass(pBaseClass, access);
                //                 if (auto pSpec = pClass->getTemplateSpecialization()) // is a template
                //                 specialization
                //                 {
                //                     if (!pSpec->isFull())
                //                     {
                //                         for(TemplateSpecialization* pOtherSpec :
                //                         pSpec->getTemplate()->getTemplateSpecializations())
                //                         {
                //                             if (pOtherSpec != pSpec &&
                //                             pOtherSpec->getInstantiationSpecialization()
                //                             == pSpec)
                //                             {
                //                                 CppLiteErrorMsg("cycle dependency detected on template base class
                //                                 inheritance (ensure inheritance")
                //                             }
                //
                //                         }
                //                         if (pBaseClass->isTemplateDependant()) // base class is template
                //                         dependant
                //                         {
                //
                //                         }
                //                     }
                //                 }
            }
        }
        return true;
    }

    bool resolveFieldDefaults(ast::Class* input, Class* pClass)
    {
        if (input->m_Members)
        {
            LocalVariableExpression* pThisExp = NewInScope<LocalVariableExpression>(pClass);
            CppLiteGuard(pThisExp);
            m_Data.pushImplicitThisExpression(pThisExp);
            CppLiteVisitElements(input->m_Members, pClass);
            m_Data.popImplicitThisExpression();
        }
        return true;
    }

    bool addTypeDep(ast::_BaseRule* input, Type* a_pType)
    {
        if (a_pType == nullptr)
            return true;
        switch (a_pType->getTypeKind())
        {
        case TypeKind::Pointer:
        case TypeKind::LValueReference:
        case TypeKind::RValueReference:
            break;
        default:
            // if not a pointer or a reference, we need the complete type to be available, so we
            // have a strong dependency on the source holding the type
            Source* pSource = a_pType->removeEverything()->getSource();
            if (pSource && !(m_Data.getSource()->hasDependency(pSource)))
            {
                if (pSource->hasDependency(m_Data.getSource()))
                {
                    CppLiteErrorMsg("'%s' : recursive source dependency on use of type '%s'",
                                    pSource->getUniqueName().data(), a_pType->getName().data());
                    return false;
                }
                if (pSource != m_Data.getSource())
                {
                    m_Data.getSource()->addDependency(pSource);
                }
            }
        }
        return true;
    }

    /// ...int a = defaultValue...
    bool traverseParameter(ast::Parameter* input)
    {
        const char* name = input->m_IDENTIFIER.hasValue() ? input->m_IDENTIFIER.as_string() : "";
        Type*       pType = CppLiteVisitType(input->m_Type);
        if (pType == nullptr)
            return true;
        // CppLiteErrorReturnIf(!(pType->isSized()), "'%s' : incomplete type",
        // PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pType)));
        if (input->m_ArrayExtents)
        {
            for (auto pExt : *input->m_ArrayExtents)
            {
                m_Data.pushInitializationType(pType);
                pType = CppLiteVisitType(pExt);
                m_Data.popInitializationType();
                CppLiteErrorReturnIf(pType == nullptr, "'%s' : invalid array extent", name);
            }
        }
        CppLiteAddTypeDependency(pType);
        Expression* pExpression = nullptr;
        if (input->m_AssignExpressionOrInitializerList)
        {
            pExpression = CppLiteVisitExpression(input->m_AssignExpressionOrInitializerList);
            CppLiteErrorReturnIf(pExpression == nullptr, "'%s' : invalid default parameter expression", name);
            Expression* pConvExpr = CppLiteConv(pExpression, pType, CppLiteGetScope());
            CppLiteGuard(pConvExpr);
            if (pConvExpr == nullptr)
                return true;
            pExpression = pConvExpr;
        }
        Parameter* pParam = NewInScope<Parameter>(pType, name);
        if (pExpression)
            pParam->setDefaultArgumentExpression(pExpression);

        CppLiteMapAndReturn(pParam);
    }

    bool resolveParameters(ast::_BaseRule* input, ast::Parameters* a_pParameters, Parameters& a_Params)
    {
        CppLiteDefaultReturnValue(false);
        if (a_pParameters)
        {
            for (auto pP : *a_pParameters)
            {
                Parameter* pParam = CppLiteVisitParameter(pP);
                CppLiteGuard(pParam);
                if (pParam == nullptr)
                    return false;
                ast::_BaseRule* input = pP;
                if (pParam->getName().size())
                {
                    for (auto p : a_Params)
                    {
                        if (p->getName() == pParam->getName())
                        {
                            for (auto p2 : a_Params)
                            {
                                p2->getSource()->Delete(p2);
                                CppLiteErrorReturn(
                                "'%.*s' : a parameter with this name already exists in the current signature",
                                PHANTOM_STRING_AS_PRINTF_ARG(pParam->getName()));
                            }
                        }
                    }
                }
                a_Params.push_back(pParam);
            }
        }

        return true;
    }

    void addLocalVariable(Block* pBlock, LocalVariable* pLocalVariable)
    {
        auto pValueType = pLocalVariable->getValueType();
        if (!pValueType->isNative())
        {
            if (ClassType* pClassType = pValueType->removeQualifiers()->asClassType())
            {
                m_Data.m_pSemantic->buildClass(pClassType, Semantic::e_ClassBuildState_ImplicitsAndDefaults);
            }
        }

        pBlock->addLocalVariable(pLocalVariable);
    }
    LocalVariable* addLocalVariable(Block* pBlock, Type* a_pType, StringView a_strName, Expression* a_pInit = nullptr,
                                    Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
    {
        if (!a_pType->isNative())
        {
            if (ClassType* pClassType = a_pType->removeQualifiers()->asClassType())
            {
                m_Data.m_pSemantic->buildClass(pClassType, Semantic::e_ClassBuildState_ImplicitsAndDefaults);
            }
        }
        return pBlock->addLocalVariable(a_pType, a_strName, a_pInit, a_Modifiers, a_uiFlags);
    }

    Block* addBlock(Method* a_pMethod)
    {
        Block* pBlock = a_pMethod->New<Block>();
        a_pMethod->setBlock(pBlock);
        size_t count = a_pMethod->getSignature()->getParameterCount();
        while (count--) // add them in reverse order for the destruction to happen canonical (left to right) to argument
                        // passing (right to left)
        {
            addLocalVariable(pBlock, a_pMethod->getSignature()->getParameter(count));
        }
        addLocalVariable(pBlock, a_pMethod->getThis());
        return pBlock;
    }

    Block* addBlock(Subroutine* a_pSubroutine)
    {
        if (Method* pMethod = a_pSubroutine->asMethod())
            return addBlock(pMethod);
        Block* pBlock = a_pSubroutine->New<Block>();
        a_pSubroutine->setBlock(pBlock);
        size_t count = a_pSubroutine->getSignature()->getParameterCount();
        while (count--) // add them in reverse order for the destruction to happen canonical (left to right) to argument
                        // passing (right to left)
        {
            addLocalVariable(pBlock, a_pSubroutine->getSignature()->getParameter(count));
        }
        return pBlock;
    }
};

struct UnaryExtractor : CppLitePass<UnaryExtractor>
{
    UnaryExtractor(CppLitePassData& a_Data, Expression* a_pLeft, Expression** a_ppResult)
        : CppLitePass<UnaryExtractor>(a_Data), m_pLeft(a_pLeft), m_ppResult(a_ppResult)
    {
    }

    bool traversePostFixExpression(ast::PostFixExpression* input)
    {
        if (!CppLitePass<UnaryExtractor>::traversePostFixExpression(input))
            return false;
        *m_ppResult = CppLiteGetElementAs(Expression);
        return true;
    }

    bool traverseUnaryExpression(ast::UnaryExpression* input)
    {
        CPPLITEPARSER_ASSERT(input->m_Expression);
        m_Data.pushLeftExpression(nullptr);
        Expression* pExpression = this->CppLiteVisitExpression(input->m_Expression);
        CppLiteErrorReturnIf(pExpression == nullptr, "operator '%s' : invalid right-hand-side expression",
                             input->m_Op.as_string());
        CppLiteGuard(pExpression);
        m_Data.popLeftExpression();

        Expression* pExpr =
        CppLiteGetSemantic()->solveBinaryOperator(input->m_Op.as_string(), m_pLeft, pExpression, CppLiteGetScope());
        CppLiteErrorReturnIf(pExpr == nullptr, "operator '%s' : no overload found taking right-hand-side type '%.*s'",
                             input->m_Op.as_string(),
                             PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pExpression->getEvalType())));
        *m_ppResult = pExpr;
        return true;
    }

    Expression*  m_pLeft;
    Expression** m_ppResult = nullptr;
};

template<class T>
bool CppLitePass<T>::traverseCStyleCastExpression(ast::CStyleCastExpression* input)
{
    if (input->m_Casted)
    {
        // cstyle cast
        LanguageElement* pElement =
        input->m_Type ? CppLiteVisitElement(input->m_Type) : CppLiteVisitElement(input->m_Expression);
        if (pElement == nullptr)
            return true;
        if (Type* pType = pElement->toType())
        {
            Expression* pCastedExp = CppLiteVisitExpression(input->m_Casted);
            if (pCastedExp == nullptr)
                return true;
            CppLiteGuard(pCastedExp);
            Expression* pConv =
            CppLiteConv(pCastedExp, pType, CastKind::Explicit, UserDefinedFunctions::All, CppLiteGetScope());
            if (pConv == nullptr)
                return true;
            CppLiteMapAndReturn(pConv);
        }
        else if (Expression* pLeft = toExpression(pElement))
        {
            ast::Expression* rightOp = nullptr;
            Expression*      pResult = nullptr;
            {
                m_Data.pushLeftExpression(pLeft);
                UnaryExtractor extractor(m_Data, pLeft, &pResult);
                extractor.traverse(input->m_Casted);
                m_Data.popLeftExpression();
            }
            CppLiteMapAndReturn(pResult);
        }
        else
        {
            CPPLITEPARSER_ASSERT(false);
        }
    }
    else
    {
        CppLiteErrorReturnIf(input->m_Type, "unexpected type between '()', expected expression");
        // parenthesis protected expression
        return this->traverse(input->m_Expression);
    }
    return true;
}

template<class T>
bool CppLitePass<T>::traverseFundamentalTypeFunctionCast(ast::FundamentalTypeFunctionCast* input)
{
    Type* pFundType = CppLiteVisitType(input->m_FundamentalType);
    CPPLITEPARSER_ASSERT(pFundType, "whisper grammar system bug, please report");
    Expression* pExp = CppLiteVisitExpression(input->m_Expression);
    if (pExp == nullptr)
        return true;
    Expression* pConv = CppLiteConv(pExp, pFundType, CastKind::Explicit, UserDefinedFunctions::All, CppLiteGetScope());
    CppLiteMapAndReturn(pConv);
}

// namer
struct CppLiteNamer : public ast::visitor::Recursive<CppLiteNamer>
{
    typedef ast::visitor::Recursive<CppLiteNamer> base_type;

    CppLiteNamer(CppLitePassData& a_Data) : m_Data(a_Data) {}

    template<class T>
    static String Name(CppLitePassData& a_Data, T* rule)
    {
        CppLiteNamer namer(a_Data);
        namer.traverse(rule);
        return namer.m_Name;
    }

    StringView getName() { return m_Name; }
    String     anyToString(spell::any any)
    {
        switch (any.type())
        {
        case spell::any::e_undefined:
            return "<undefined-value>";
        case spell::any::e_cstring:
            return any.as_string();
        case spell::any::e_char:
            return String(&any.ref_char(), 1);
        case spell::any::e_uchar:
            return phantom::lexical_cast<String>(any.as_uchar());
        case spell::any::e_int:
            return phantom::lexical_cast<String>(any.as_int());
        case spell::any::e_uint:
            return phantom::lexical_cast<String>(any.as_uint());
        case spell::any::e_short:
            return phantom::lexical_cast<String>(any.as_short());
        case spell::any::e_ushort:
            return phantom::lexical_cast<String>(any.as_ushort());
        case spell::any::e_long:
            return phantom::lexical_cast<String>(any.as_long());
        case spell::any::e_ulong:
            return phantom::lexical_cast<String>(any.as_ulong());
        case spell::any::e_longlong:
            return phantom::lexical_cast<String>(any.as_longlong());
        case spell::any::e_ulonglong:
            return phantom::lexical_cast<String>(any.as_ulonglong());
        case spell::any::e_double:
            return phantom::lexical_cast<String>(any.as_double());
        case spell::any::e_float:
            return phantom::lexical_cast<String>(any.as_float());
        case spell::any::e_bool:
            return phantom::lexical_cast<String>(any.as_bool());
        default:
            CPPLITEPARSER_ASSERT(false);
            return "<invalid-name>";
        }
    }

    bool traverseExpression(ast::Expression* input)
    {
        LanguageElement* pElem = m_Data.findElement(input);
        if (pElem && pElem->asSymbol())
        {
            m_Name += m_Data.elementToString(static_cast<Symbol*>(pElem));
            return true;
        }
        return base_type::traverseExpression(input);
    }

    bool traverseUnsignedType(ast::UnsignedType* input)
    {
        m_Name += "unsigned ";
        return traverse(input->m_Type);
    }

    bool traverseSignedType(ast::SignedType* input)
    {
        m_Name += "signed ";
        return traverse(input->m_Type);
    }

    bool traverseLongType(ast::LongType* input)
    {
        m_Name += "long ";
        return traverse(input->m_Type);
    }

    bool traverseFundamentalTypeBase(ast::FundamentalTypeBase* input)
    {
        m_Name += input->m_Typename;
        return true;
    }

    bool traverseTemplateArgumentAssign(ast::TemplateArgumentAssign* input)
    {
        if (input->m_Type)
        {
            traverse(input->m_Type);
        }
        else if (input->m_INT_VALUE.hasValue())
        {
            m_Name += anyToString(input->m_INT_VALUE);
        }
        else
        {
            m_Name += "<unevaluated-expression>";
        }
        return true;
    }

    bool traverseTemplateArgumentNoAssign(ast::TemplateArgumentNoAssign* input)
    {
        return traverse(input->m_PostFixExpression);
    }

    bool traverseQualifiedName(ast::QualifiedName* input)
    {
        String sep = ".";
        if (input->type() == ast::e_RuleType_QualifiedScopedName)
        {
            sep = "::";
        }
        auto names = input->m_Scope ? input->m_Scope->m_Names : input->m_Dot->m_Names;
        if (names)
            for (ast::Name* name : *names)
            {
                if (name != names->front())
                    m_Name += sep;
                m_Name += NameToString(name);
                if (name->m_TemplateArgumentList)
                {
                    m_Name += '<';
                    if (name->m_TemplateArgumentList->m_TemplateArgumentListSplit)
                    {
                        SmallVector<ast::_BaseRule*> templateArgs;
                        if (name->m_TemplateArgumentList->m_TemplateArgumentListSplit->m_NotLastTemplateArguments)
                        {
                            for (ast::TemplateArgument* arg :
                                 *name->m_TemplateArgumentList->m_TemplateArgumentListSplit->m_NotLastTemplateArguments)
                            {
                                ast::NotLastTemplateArgument* argcast = static_cast<ast::NotLastTemplateArgument*>(arg);
                                if (argcast->m_NotLastTemplateArgumentAssign)
                                    traverse(argcast->m_NotLastTemplateArgumentAssign);
                                else
                                    traverse(argcast->m_NotLastTemplateArgumentNoAssign);
                                m_Name += ',';
                            }
                        }
                        auto last = static_cast<ast::LastTemplateArgument*>(
                        name->m_TemplateArgumentList->m_TemplateArgumentListSplit->m_LastTemplateArgument);

                        if (last->m_LastTemplateArgumentAssign)
                            traverse(last->m_LastTemplateArgumentAssign);
                        else
                            traverse(last->m_LastTemplateArgumentNoAssign);
                    }
                    m_Name += '>';
                }
            }
        return true;
    }
    bool traverseExplicitType(ast::ExplicitType* input)
    {
        if (input->m_CONSTs)
        {
            for (auto cv : *input->m_CONSTs)
            {
                if (cv.as_string()[0] == 'C')
                    m_Name += "const ";
                else
                    m_Name += "volatile ";
            }
        }
        return base_type::traverseExplicitType(input);
    }

    CppLitePassData& m_Data;
    String           m_Name;
};

// modules
struct CppLitePassModules : public CppLitePass<CppLitePassModules>
{
    CppLitePassModules(CppLitePassData& a_Data) : CppLitePass<CppLitePassModules>(a_Data) {}
    bool traverseModuleDeclaration(ast::ModuleDeclaration* input)
    {
        size_t i = 0;

        String moduleName;
        for (; i < input->m_Names->size(); ++i)
        {
            if (i)
                moduleName += '.';
            moduleName += (*input->m_Names)[i];
        }

        Module* pMod = Application::Get()->getModule(moduleName);
        if (!pMod)
        {
            if (CppLiteGetParser()->getOption("prefer-module-kind") == "native")
            {
                if (Plugin* pPlugin = Application::Get()->getPlugin(moduleName))
                {
                    if (pPlugin->load())
                    {
                        pMod = pPlugin->getModule();
                        CPPLITEPARSER_ASSERT(pMod);
                    }
                }
                if (!pMod)
                {
                    CppLiteErrorReturnIf(
                    !m_Data.m_pBuildSession->addProject(moduleName),
                    "module '%.*s' : not found, either because project no found or project build failed",
                    PHANTOM_STRING_AS_PRINTF_ARG(moduleName));
                }
            }
            else
            {
                m_Data.m_pBuildSession->addProject(moduleName);
                if (!pMod || !pMod->isNative())
                {
                    if (!m_Data.m_pBuildSession->isSuccessful())
                    {
                        CppLiteErrorReturn("module : failed to build project '%.*s'",
                                           PHANTOM_STRING_AS_PRINTF_ARG(moduleName));
                    }
                }
                pMod = Application::Get()->getModule(moduleName);
                if (!pMod)
                {
                    if (Plugin* pPlugin = Application::Get()->getPlugin(moduleName))
                    {
                        if (pPlugin->load())
                        {
                            pMod = pPlugin->getModule();
                            CPPLITEPARSER_ASSERT(pMod);
                        }
                    }
                    if (!pMod)
                    {
                        CppLiteError(
                        "module '%.*s' : not found, either because project no found or project build failed",
                        PHANTOM_STRING_AS_PRINTF_ARG(moduleName));
                        return true;
                    }
                }
            }
        }
        else
        {
            if (!pMod->isNative())
            {
                m_Data.m_pBuildSession->addProject(moduleName);
                if (!m_Data.m_pBuildSession->isSuccessful())
                {
                    CppLiteErrorReturn("module : failed to build project '%.*s'",
                                       PHANTOM_STRING_AS_PRINTF_ARG(moduleName));
                }
            }
        }
        if (!CppLiteGetSource()->getModule()->hasDependency(pMod))
            CppLiteGetSource()->getModule()->addDependency(pMod);

        return true;
    }

    bool traverseSourceDeclaration(ast::SourceDeclaration* input) { return false; }
};

// imports
struct CppLitePassImport : public CppLitePass<CppLitePassImport>
{
    CppLitePassImport(CppLitePassData& a_Data) : CppLitePass<CppLitePassImport>(a_Data) {}

    bool resolveImportNames(ast::ImportDeclaration* input, phantom::ArrayView<spell::any> a_Names)
    {
        bool bStatic = input->m_STATIC.hasValue();
        bool bPublic = input->m_PUBLIC.hasValue();

        SmallVector<StringView, 8> names;
        for (auto& n : a_Names)
            names.push_back(n.as_string());

        return resolveImportNames(input, names, bStatic, bPublic);
    }

    bool resolveImportNames(ast::ImportDeclaration* input, phantom::ArrayView<StringView> a_Names, bool bStatic,
                            bool bPublic)
    {
        Source* pCurrentSource = m_Data.m_pSource;
        CPPLITEPARSER_ASSERT(pCurrentSource);
        CppLiteDefaultReturnValue(false);

        String relPath;
        for (size_t i = 0; i < a_Names.size(); ++i)
        {
            if (i)
                relPath += '.';
            relPath += a_Names[i];
        }
        m_Data.m_pBuildSession->loadSourcesOrPackages(relPath);

        Symbol*  pImported = nullptr;
        Packages packages;
        Sources  sources;

        PackageFolder* currFolder = Application::Get()->rootPackageFolder();
        if (a_Names.size() == 1)
        {
            // special case : source at current package level
            for (Package* pck : pCurrentSource->getPackage()->getPackageFolder()->getPackages())
            {
                if (Source* pSource = pck->getSource(a_Names.front()))
                {
                    sources.push_back(pSource);
                }
            }
        }
        if (sources.empty())
        {
            for (size_t i = 0; i < a_Names.size(); ++i)
            {
                if (PackageFolder* folder = currFolder->getPackageFolder(a_Names[i]))
                {
                    packages = folder->getPackages();
                    currFolder = folder;
                }
                else
                {
                    if (!packages.empty())
                    {
                        if (i == a_Names.size() - 1)
                        {
                            for (auto package : packages)
                            {
                                if (Source* pSource = package->getSource(a_Names[i]))
                                    sources.push_back(pSource);
                            }
                        }
                        packages.clear();
                    }
                    break;
                }
            }
        }

        if (sources.empty())
        {
            String importName;
            for (size_t i = 0; i < a_Names.size(); ++i)
            {
                if (i)
                    importName += '.';
                importName += a_Names[i];
            }
            CppLiteErrorReturn("'%s' : source not found", importName.data());
        }

        CPPLITEPARSER_ASSERT(sources.empty() || packages.empty());

        ArrayView<Symbol*> imports;
        if (packages.empty())
        {
            auto foundSelf = std::find(sources.begin(), sources.end(), pCurrentSource);
            if (foundSelf != sources.end())
            {
                // if we only import outself, it's an error, but if multiple import with same name occur, we just
                // skip ourself
                CppLiteErrorReturnIf(sources.size() == 1, "'%.*s' : source cannot import itself",
                                     PHANTOM_STRING_AS_PRINTF_ARG(pCurrentSource->getName()));
                sources.erase(foundSelf);
            }
            imports = ArrayView<Symbol*>(reinterpret_cast<Symbol**>(sources.data()), sources.size());
        }
        else
        {
            imports = ArrayView<Symbol*>(reinterpret_cast<Symbol**>(packages.data()), packages.size());
            for (auto pck : packages)
                for (auto src : pck->getSources())
                    sources.push_back(src);
        }

        for (auto src : sources)
        {
            // ensure all native class members have been registered so that we have all the
            // template specializations available
            const auto& types = src->getTypes();
            for (size_t i = 0; i < types.size(); ++i)
                types[i]->getElements();
        }

        Module* pCurrentModule = m_Data.getSource()->getModule();
        for (auto pImport : imports)
        {
            if (!(pImport->isNative()) && pCurrentModule != pImport->getModule() &&
                pImport->getModule()->hasDependencyCascade(pCurrentModule))
            {
                String importName;
                for (size_t i = 0; i < a_Names.size(); ++i)
                {
                    if (i)
                        importName += '.';
                    importName += a_Names[i];
                }
                CppLiteErrorReturn("'%s' : module cyclic dependency detected", importName.data());
            }

            phantom::SmallMap<Symbol*, Symbols> collidingSymbols;
            if (!(pCurrentSource->canImport(pImport, Access::Public, Modifiers(bStatic * PHANTOM_R_STATIC), 0,
                                            &collidingSymbols)))
            {
                for (auto it = collidingSymbols.begin(); it != collidingSymbols.end(); ++it)
                {
                    Symbol* pSymbol = it->first;
                    CppLiteErrorMsg("'%s' : import induce name collision for symbol '%s' (use "
                                    "'static' for strict qualified name import)",
                                    pImport->getUniqueName().data(), pSymbol->getUniqueName().data());
                    auto& vec = it->second;
                    for (auto it = vec.begin(); it != vec.end(); ++it)
                    {
                        CppLiteSubError("collides with '%.*s'", PHANTOM_STRING_AS_PRINTF_ARG((*it)->getUniqueName()));
                    }
                }
            }
            else
            {
                //                 if (Source* pSrc = pImport->asSource())
                //                 {
                //                     m_Data.m_pParser->addDependency(pSrc);
                //                 }
                //                 else
                //                 {
                //                     for (auto pSrc : static_cast<Package*>(pImport)->getSources())
                //                     {
                //                         m_Data.m_pParser->addDependency(pSrc);
                //                     }
                //                 }
                pCurrentSource->addImport(pImport, bStatic, bPublic);
            }
        }
        return true;
    }

    bool traverseImportDeclaration(ast::ImportDeclaration* input)
    {
        if (input->m_STRING_LIT.hasValue())
        {
            // static
            StringViews words;
            StringUtil::Split(words, input->m_STRING_LIT.as_string() + 1, ".", false);
            CppLiteErrorReturnIf(words.empty(), "#include \"...\" : invalid empty string");
            if (!words.front().empty())
                words.front() = words.front().substr(1);
            if (!words.back().empty())
                words.back() = words.back().substr(0, words.back().size() - 1);
            for (auto& w : words)
            {
                CppLiteErrorReturnIf(w.empty() || !Symbol::IsCppIdentifier(w), "import : invalid identifier '%.*s'",
                                     PHANTOM_STRING_AS_PRINTF_ARG(w));
            }
            resolveImportNames(input, words, true, false);
            return true;
        }

        if (input->m_Names)
        {
            resolveImportNames(input, *input->m_Names);
            return true;
        }

        auto* imports = input->m_Imports;
        CPPLITEPARSER_ASSERT(imports);

        for (ast::Import* pAstImport : *imports)
        {
            ast::ImportBase* pImportBase = pAstImport->m_ImportBase;
            if (pImportBase->m_Names == nullptr || !resolveImportNames(input, *pImportBase->m_Names))
                return true;
        }
        return true;
    }
    bool traverseSourceDeclaration(ast::SourceDeclaration* input) { return false; }
};

// global names
struct CppLitePassGlobals : public CppLitePass<CppLitePassGlobals>
{
    CppLitePassGlobals(CppLitePassData& a_Data) : CppLitePass<CppLitePassGlobals>(a_Data) {}

    bool traverseStaticAssert(ast::StaticAssert* input) { return true; }

    bool traverseSourceDeclaration(ast::SourceDeclaration* input)
    {
        Symbol* pSymbol = CppLiteVisitSymbol(input->m_Declaration);
        if (pSymbol)
        {
            if (input->m_MetaOrAnnotations)
                resolveMetaOrAnnotations(input->m_MetaOrAnnotations, pSymbol);
        }
        return true;
    }

    bool traverseUsing(ast::Using* input)
    {
        Source* pSource = CppLiteGetScopeAs(Source);
        if (pSource)
        {
            Symbol* pSymbol = CppLiteVisitSymbol(input->m_QualifiedName);
            if (pSymbol)
            {
                CppLiteErrorReturnIfNameUsed(pSource, pSymbol->getName().data());
                Alias* pAlias = pSource->addAlias(pSymbol, pSymbol->getName());
                CppLiteMapAndReturn(pAlias);
            }
        }
        return true;
    }

    bool traverseTemplate(ast::Template* input)
    {
        Source* pSource = CppLiteGetScopeAs(Source);
        CPPLITEPARSER_ASSERT(pSource);
        LanguageElement* pElement = CppLiteVisitElement(input->m_TemplateSignature);
        if (pElement == nullptr)
            return true;
        CPPLITEPARSER_ASSERT(pElement->asTemplateSignature());
        TemplateSignature* pTemplateSignature = static_cast<TemplateSignature*>(pElement);
        m_Data.pushTemplateSignature(pTemplateSignature);
        ast::_BaseRule* templated = nullptr;
        if (templated = input->m_Class)
        {
            traverse(input->m_Class);
        }
        else if (templated = input->m_Union)
        {
            traverse(input->m_Union);
        }
        else if (templated = input->m_Alias)
        {
            traverse(input->m_Alias);
        }
        else if (templated = input->m_TemplateFunction)
        {
            traverse(input->m_TemplateFunction);
        }
        else if (templated = input->m_Constructor)
        {
            traverse(input->m_Constructor);
        }
        else if (templated = input->m_TemplateSignature)
        {
            CppLiteError("Template are forbiden");
        }
        else
            assert(false && "Unconsistent ast::Template");

        m_Data.popTemplateSignature();

        if (templated)
        {
            LanguageElement* elem = m_Data.findElement(templated);
            if (elem)
            {
                if (Symbol* pSymbol = elem->asSymbol())
                {
                    CppLiteMapAndReturn(pSymbol->getTemplateSpecialization());
                }
            }
        }

        return true;
    }

    bool traverseMixinStatementDefinition(ast::MixinStatementDefinition* input)
    {
        m_Data.m_MixinStatementDefinitions[input->m_IDENTIFIER] = input;
        return true;
    }

    bool traverseClass(ast::Class* input)
    {
        Class* pClass = nullptr;
        resolveClass(input, pClass);
        CppLiteMapAndReturn(pClass);
    }

    bool traverseTemplateFunction(ast::TemplateFunction* input)
    {
        if (input->m_STATIC.hasValue())
        {
            CppLiteWarningMsg("static : optional on global functions");
        }

        TemplateSpecialization* pTemplateSpec = nullptr;
        if (!(resolveTemplateSpecialization(input, /*input->m_TemplateArgumentList*/ nullptr, pTemplateSpec, true)))
            return false;

        m_Data.pushScope(pTemplateSpec);
        const char* name = input->m_IDENTIFIER;
        Subroutine* pSubroutine = nullptr;
        if (!resolveFunctionEnd(input->m_TypeOrAuto, name, input->m_FunctionEnd, Modifiers{PHANTOM_R_STATIC},
                                pSubroutine, false))
        {
            m_Data.popScope();
            return true;
        }
        m_Data.popScope();
        CppLiteMapAndReturn(pSubroutine);
    }

    bool traverseFunctionOrVariable(ast::FunctionOrVariable* input)
    {
        Modifiers modifiers = 0;

        Source* pScope = CppLiteGetScopeAs(Source);
        CPPLITEPARSER_ASSERT(pScope, "source must be the current scope at this pass");

        Type* pType = CppLiteVisitType(input->m_TypeOrAuto);
        if (pType == nullptr)
        {
            return true;
        }
        LanguageElement* pElement = nullptr;
        if (input->m_Specifiers)
        {
            for (auto pSpec : *input->m_Specifiers)
            {
                if (pSpec->m_MethodSpecifier)
                {
                    CppLiteErrorMsg("%s : only allowed for methods", pSpec->m_MethodSpecifier->m_STATIC);
                }
                else
                {
                    CppLiteErrorMsg("@filter : only allowed on fields");
                }
            }
        }

        if (auto pFunctionOrVariableEnd = input->m_FunctionOrVariableEnd)
        {
            const char* name = pFunctionOrVariableEnd->m_IDENTIFIER;
            Modifiers   refQualifiers = 0;
            if (pFunctionOrVariableEnd->m_FunctionEnd)
            {
                // pure virtual / override
                CppLiteErrorReturnIf(pFunctionOrVariableEnd->m_FunctionEnd->m_Suffix.hasValue(),
                                     "'%s' : ilegal '=' suffix on global function", name);

                CppLiteErrorReturnIf(pFunctionOrVariableEnd->m_FunctionEnd->m_OVERRIDEs &&
                                     !pFunctionOrVariableEnd->m_FunctionEnd->m_OVERRIDEs->empty(),
                                     "'%s' : illegal 'override' or 'final' on global function", name);
                // ref qualifiers
                CppLiteErrorReturnIf(pFunctionOrVariableEnd->m_FunctionEnd->m_CONSTs,
                                     "'%s' : illegal ref-qualifiers on global function", name);
                // signature
                Parameters params;
                if (!resolveParameters(input, pFunctionOrVariableEnd->m_FunctionEnd->m_Parameters, params))
                    return true;
                Signature* pSignature = New<Signature>(pType, params);

                Types       types = pSignature->getParameterTypes();
                Subroutines subroutines;
                if (!(pScope->acceptsSubroutine(pSignature->getReturnType(), name, types, PHANTOM_R_NONE,
                                                PHANTOM_R_NONE, &subroutines)))
                {
                    CppLiteErrorMsg("'%s' : conflicts with previously defined function%s", name,
                                    subroutines.size() > 1 ? "s" : "");
                    for (auto pSub : subroutines)
                    {
                        CppLiteSubError("%.*s", PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pSub)));
                    }
                }
                Function* pFunction = NewInScope<Function>(name, pSignature, modifiers);
                pScope->addFunction(pFunction);
                addBlock(pFunction);
                MetaDatas metas;
                // CppLiteAddMetas(pFunction);
                for (auto& m : metas)
                {
                    pFunction->setMetaData(m.first, m.second);
                }
                CppLiteMapAndReturn(pFunction);
            }
            else
            {
                const char* name = pFunctionOrVariableEnd->m_IDENTIFIER;
                Type*       pType = CppLiteVisitType(input->m_TypeOrAuto);
                if (pType == nullptr)
                    return true;
                CppLiteErrorReturnIf(pType->isVoid(), "'%s' : illegal void variable", name);
                CppLiteErrorReturnIf(!pType->isDefaultConstructible(),
                                     "'%s' : variable type is not default constructible", name);

                Variable* pVar = NewInScope<Variable>(pType, name);
                pScope->addVariable(pVar);
                if (pFunctionOrVariableEnd->m_CallList)
                {
                    CppLiteErrorMsg("'%s' : global variables are not allowed for hot reload requirements", name);
                }
                else if (pFunctionOrVariableEnd->m_AssignExpressionOrInitializerList)
                {
                    CppLiteErrorMsg("'%s' : global variables are not allowed for hot reload requirements", name);
                }
                else if (pFunctionOrVariableEnd->m_Declarators)
                {
                    CppLiteErrorMsg("'%s' : global variables are not allowed for hot reload requirements", name);
                }
                else if (pFunctionOrVariableEnd->m_TemplateArgumentList)
                {
                    // template function (not supported yet)
                    CppLiteErrorMsg("'%s' : template function : not supported yet", name);
                    return true;
                }
                CppLiteMapAndReturn(pVar);
            }
            return true;
        }
        else if (input->m_OperatorEnd)
        {
            pElement = CppLiteVisitElement(input->m_OperatorEnd);
        }
        return true;
    }
    bool traverseOperatorEnd(ast::OperatorEnd* input) { return true; }

    bool traverseUnion(ast::Union* input)
    {
        TemplateSpecialization* pTemplateSpec = nullptr;

        if (!(resolveTemplateSpecialization(input, input->m_TemplateArgumentList, pTemplateSpec, false)))
            return true;

        Scope* pScope = CppLiteGetScopeAs(Scope);
        if (pTemplateSpec == nullptr)
        {
            CppLiteErrorReturnIfNameUsed(pScope->asLanguageElement(), input->m_IDENTIFIER);
        }

        Union* pUnion = NewInScope<Union>(input->m_IDENTIFIER);
        pUnion->setAccess(CppLiteGetAccess());

        if (pTemplateSpec)
        {
            pTemplateSpec->setTemplated(pUnion);
        }
        else
        {
            pScope->addType(pUnion);
        }
        CppLiteMapAndReturn(pUnion);
    }
    bool traverseAlias(ast::Alias* input) { return resolveAlias(input); }
};

// create members (register source dependencies and add some of them later in another pass when
// every members of each dependency has been added)
struct CppLitePassInheritance : public CppLitePass<CppLitePassInheritance>
{
    CppLitePassInheritance(CppLitePassData& a_Data) : CppLitePass<CppLitePassInheritance>(a_Data) {}

    bool endSource(ast::Source* input) { return true; }
    // skip every function blocks in this pass
    bool traverseFunctionBlock(ast::FunctionBlock* input) { return true; }

    // skip other members than statemachine/states/tracks
    bool traverseMethod(ast::Method* input) { return true; }
    bool traverseConversionFunction(ast::ConversionFunction* input) { return true; }
    bool traverseConstructor(ast::Constructor* input) { return true; }
    bool traverseDestructor(ast::Destructor* input) { return true; }
    bool traverseOperator(ast::Operator* input) { return true; }

    bool traverseUnion(ast::Union* input) { return true; }
    bool traverseTypedef(ast::Typedef* input) { return true; }
    bool traverseUsing(ast::Using* input) { return true; }
    bool traverseFriend(ast::Friend* input) { return true; }
    bool traverseAlias(ast::Alias* input) { return true; }
    bool traverseStaticAssert(ast::StaticAssert* input) { return true; }
    bool traverseMixinStatementDefinition(ast::MixinStatementDefinition* input) { return true; }
    bool traverseFunctionOrVariable(ast::FunctionOrVariable* input) { return true; }
    bool traverseTemplate(ast::Template* input)
    {
        //             Source* pSource = CppLiteGetScopeAs(Source);
        //             CPPLITEPARSER_ASSERT(pSource);
        //             LanguageElement* pElement =
        //             m_Data.findElement<type>(input->m_TemplateSignature,
        //             &LanguageElement::asTemplateSignature); if (pElement == nullptr) return
        //             true; CPPLITEPARSER_ASSERT(pElement->asTemplateSignature()); TemplateSignature*
        //             pTemplateSignature = static_cast<TemplateSignature*>(pElement);
        //             m_Data.pushTemplateSignature(pTemplateSignature);
        if (input->m_Class)
        {
            traverse(input->m_Class);
        }
        //            m_Data.popTemplateSignature();

        return true;
    }

    bool traverseClass(ast::Class* input)
    {
        Class* pClass = CppLiteGetElementAs(Class);
        if (pClass)
            resolveClassInheritance(input, pClass);
        return true;
    }
};

// create members (register source dependencies and add some of them later in another pass when
// every members of each dependency has been added)
struct CppLitePassMembersLocal : public CppLitePass<CppLitePassMembersLocal>
{
    CppLitePassMembersLocal(CppLitePassData& a_Data) : CppLitePass<CppLitePassMembersLocal>(a_Data) {}

    bool traverseMixinStatementDefinition(ast::MixinStatementDefinition* input)
    {
        m_Data.m_MixinStatementDefinitions[input->m_IDENTIFIER] = input;
        return true;
    }

    /// @static_assert
    bool traverseStaticAssert(ast::StaticAssert* input) { return _traverseStaticAssert(input); }

    bool traverseMemberWithMetaOrAnnotation(ast::MemberWithMetaOrAnnotation* input)
    {
        ClassType* pClassType = CppLiteGetScopeAs(ClassType);
        if (pClassType == nullptr)
            return true;
        Symbol* pSymbol = CppLiteVisitSymbol(input->m_Member, pClassType);
        if (pSymbol)
            return resolveMetaOrAnnotations(input->m_MetaOrAnnotations, pSymbol);
        return true;
    }

    bool traverseBasicMemberWithMetaOrAnnotation(ast::BasicMemberWithMetaOrAnnotation* input)
    {
        ClassType* pClassType = CppLiteGetScopeAs(ClassType);
        if (pClassType == nullptr)
            return true;
        Symbol* pSymbol = CppLiteVisitSymbol(input->m_BasicMember, pClassType);
        if (pSymbol)
            return resolveMetaOrAnnotations(input->m_MetaOrAnnotations, pSymbol);
        return true;
    }

    bool traverseTemplate(ast::Template* input)
    {
        if (input->m_Class)
        {
            traverse(input->m_Class);
        }
        else if (input->m_Union)
        {
            traverse(input->m_Union);
        }
        else if (input->m_Alias)
        {
            traverse(input->m_Alias);
        }
        else if (input->m_TemplateFunction)
        {
            LanguageElement* pElement = CppLiteVisitElement(input->m_TemplateSignature);
            if (pElement == nullptr)
                return true;
            CPPLITEPARSER_ASSERT(pElement->asTemplateSignature());
            TemplateSignature* pTemplateSignature = static_cast<TemplateSignature*>(pElement);
            m_Data.pushTemplateSignature(pTemplateSignature);
            traverse(input->m_TemplateFunction);
            m_Data.popTemplateSignature();
        }
        else if (input->m_Constructor)
        {
            LanguageElement* pElement = CppLiteVisitElement(input->m_TemplateSignature);
            if (pElement == nullptr)
                return true;
            CPPLITEPARSER_ASSERT(pElement->asTemplateSignature());
            TemplateSignature* pTemplateSignature = static_cast<TemplateSignature*>(pElement);
            m_Data.pushTemplateSignature(pTemplateSignature);
            traverse(input->m_Constructor);
            m_Data.popTemplateSignature();
        }
        else if (input->m_TemplateSignature)
        {
            CppLiteError("Template are forbiden");
        }
        else
        {
            CPPLITEPARSER_ASSERT(false && "Unconsistent ast::Template");
        }

        return true;
    }

    void buildTemplateInstantiations(Class* a_pClass, Semantic::EClassBuildState a_eBuildState)
    {
        if (auto pTemplateSpec = a_pClass->getTemplateSpecialization())
        {
            if (!pTemplateSpec->isFull()) // partial or empty spec
            {
                for (auto pOtherTemplateSpec : pTemplateSpec->getTemplate()->getTemplateSpecializations())
                {
                    if (pOtherTemplateSpec->isFull())
                    {
                        if (pOtherTemplateSpec->getInstantiationSpecialization() == pTemplateSpec)
                        {
                            CppLiteGetSemantic()->buildClass(static_cast<Class*>(pOtherTemplateSpec->getTemplated()),
                                                             a_eBuildState);
                        }
                    }
                }
            }
        }
    }

    bool traverseClass(ast::Class* input)
    {
        Class*  pClass;
        Classes instantiationsToBuild;
        if (!CppLiteHasMappedElement(input))
        {
            // nested
            CppLitePassGlobals globals(m_Data);
            globals.traverseClass(input);
            CppLitePassInheritance inheritance(m_Data);
            inheritance.traverseClass(input);
        }
        pClass = CppLiteGetElementAs(Class);
        CppLiteCheckShouldWeContinueParsing(pClass);
        if (input->m_Members)
        {
            CppLiteVisitElements(input->m_Members, pClass);
        }

        return true;
    }

    bool traverseUnion(ast::Union* input)
    {
        Union* pUnion = CppLiteGetElementAs(Union);
        if (pUnion == nullptr)
        {
            if (CppLiteGetScope()->asSource() == nullptr)
            {
                // nested class not defined yet
                m_Data.pushReturnAddress((LanguageElement**)&pUnion);
                CppLitePassGlobals globalNames(m_Data);
                globalNames.traverseUnion(input);
                m_Data.popReturnAddress();
                if (pUnion == nullptr)
                    return true;
                CppLiteMapAndReturn(pUnion);
            }
            else
                return true;
        }
        else
        {
            CPPLITEPARSER_ASSERT(pUnion->asUnion());
        }

        if (input->m_BasicMembers)
        {
            CppLiteVisitElements(input->m_BasicMembers, pUnion);
        }
        return true;
    }

    /// <type>() {...}
    bool traverseConstructor(ast::Constructor* input);

    /// ~<type>() {...}
    bool traverseDestructor(ast::Destructor* input)
    {
        ClassType* pClassType = CppLiteGetScopeAs(ClassType);
        if (pClassType == nullptr)
            return true;
        CppLiteErrorReturnIf(pClassType->getName() != input->m_IDENTIFIER,
                             "'%.*s' : destructor name '%s' not matching class type name",
                             PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType)), input->m_IDENTIFIER);
        CppLiteErrorReturnIf(pClassType->getDestructor(),
                             "'%.*s' : a destructor has already been defined for this class type",
                             PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType)));
        Modifiers mods;
        if (input->m_VIRTUAL.hasValue())
            mods |= Modifier::Virtual;
        Method* pDestructor = pClassType->addDestructor(mods);
        if (input->m_OVERRIDEs)
            resolveOverrideAndFinal(input, pDestructor->getName().data(), *input->m_OVERRIDEs, mods);
        pDestructor->addModifiers(mods);
        pDestructor->setAccess(CppLiteGetAccess());
        addBlock(pDestructor);
        CppLiteMapAndReturn(pDestructor);
    }

    /// operator <type>() {...}
    bool traverseConversionFunction(ast::ConversionFunction* input)
    {
        ClassType* pClassType = CppLiteGetScopeAs(ClassType);
        if (pClassType == nullptr)
            return true;
        Type* pType = CppLiteVisitType(input->m_TypeNoFunctionType);
        if (pType == nullptr)
            return true;
        CppLiteAddTypeDependency(pType);
        CppLiteErrorReturnIf(pClassType->getConversionFunction(pType),
                             "'%.*s' : conversion function for type '%.*s' has already been defined "
                             "for this class type",
                             PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType)),
                             PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pType)));
        Method* pMethod =
        NewInScope<Method>("operator " + pType->getUniqueName(), pClassType->NewDeferred<Signature>(pType));
        pMethod->setAccess(CppLiteGetAccess());

        pClassType->addMethod(pMethod);
        addBlock(pMethod);
        CppLiteMapAndReturn(pMethod);
    }

    bool traverseFunctionOrVariable(ast::FunctionOrVariable* input)
    {
        Modifiers modifiers = 0;

        ClassType* pClassType = CppLiteGetScopeAs(ClassType);
        if (pClassType == nullptr)
            return true; // only looking for methods

        if (input->m_Specifiers)
        {
            for (auto pSpec : *input->m_Specifiers)
            {
                if (pSpec->m_MethodSpecifier)
                {
                    CppLiteErrorReturnIf(input->m_FunctionOrVariableEnd == nullptr ||
                                         input->m_FunctionOrVariableEnd->m_FunctionEnd == nullptr,
                                         "%s : only allowed for methods", pSpec->m_MethodSpecifier->m_STATIC);
                    switch (pSpec->m_MethodSpecifier->m_STATIC[0])
                    {
                    case 's':
                        CppLiteErrorReturnIf((modifiers & PHANTOM_R_STATIC) != 0, "static : specifier already used");
                        modifiers |= PHANTOM_R_STATIC;
                        break;
                    case '@':
                        CppLiteErrorReturnIf((modifiers & PHANTOM_R_ABSTRACT) != 0,
                                             "@abstract : specifier already used");
                        modifiers |= PHANTOM_R_ABSTRACT;
                        break;
                    case 'v':
                        CppLiteErrorReturnIf((modifiers & PHANTOM_R_VIRTUAL) != 0, "virtual : specifier already used");
                        modifiers |= PHANTOM_R_VIRTUAL;
                        break;
                    default:
                        CPPLITEPARSER_ASSERT(false);
                    }
                }
            }
        }

        LanguageElement* pElement = nullptr;
        Subroutine*      pSubroutine = nullptr;

        if (auto pFunctionOrVariableEnd = input->m_FunctionOrVariableEnd)
        {
            const char* name = pFunctionOrVariableEnd->m_IDENTIFIER;

            // methods
            if (pFunctionOrVariableEnd->m_FunctionEnd)
            {
                if (!resolveFunctionEnd(input->m_TypeOrAuto, pFunctionOrVariableEnd->m_IDENTIFIER,
                                        pFunctionOrVariableEnd->m_FunctionEnd, modifiers, pSubroutine, false))
                    return true;
            }

            // fields
            else
            {
                Type* pType = CppLiteVisitType(input->m_TypeOrAuto);
                if (pType == nullptr)
                    return true;
                CppLiteAddTypeDependency(pType);
                // read alignment
                int align = 0;
                if (input->m_AlignAs.hasValue())
                {
                    align = input->m_AlignAs.as_int();
                    CppLiteErrorReturnIf(align == 0, "'alignas' : forbidden zero alignment", align);
                    CppLiteErrorReturnIf(!(isPowerOfTwo(align)), "'alignas' : expects a power of two, given '%d'",
                                         align);
                }

                Expression* pInitExpression = nullptr;
                if (pType->hasAuto())
                {
                    ast::AutoDeclarator decl;
                    decl.m_IDENTIFIER = name;
                    decl.m_CallList = pFunctionOrVariableEnd->m_CallList;
                    decl.m_AssignExpressionOrInitializerList =
                    pFunctionOrVariableEnd->m_AssignExpressionOrInitializerList;
                    decl.m_TypeExtents = nullptr;
                    decl.m_ArrayExtents = nullptr;
                    if (!resolveDeclarator(pType, &decl, pInitExpression, true, false))
                    {
                        memset(&decl, 0,
                               sizeof(ast::AutoDeclarator)); // ensure content is not deleted
                        return true;
                    }
                    memset(&decl, 0,
                           sizeof(ast::AutoDeclarator)); // ensure content is not deleted
                }
                else
                {
                    ast::Declarator decl;
                    decl.m_IDENTIFIER = name;
                    decl.m_CallList = pFunctionOrVariableEnd->m_CallList;
                    decl.m_AssignExpressionOrInitializerList =
                    pFunctionOrVariableEnd->m_AssignExpressionOrInitializerList;
                    decl.m_TypeExtents = nullptr;
                    decl.m_ArrayExtents = pFunctionOrVariableEnd->m_ArrayExtents;
                    if (!resolveDeclarator(pType, &decl, pInitExpression, true, false))
                    {
                        memset(&decl, 0,
                               sizeof(ast::Declarator)); // ensure content is not deleted
                        return true;
                    }
                    memset(&decl, 0, sizeof(ast::Declarator)); // ensure content is not deleted
                }
                // field

                Fields fields;
                Field* pField = NewInScope<Field>(pType, name, PHANTOM_R_FILTER_FIELD);
                pField->setAccess(CppLiteGetAccess());
                if (align > pField->getAlignment())
                    pField->setAlignment(align);

                CppLiteGuard(pField);
                bool bSavedApplied = false;
                if (input->m_Specifiers)
                {
                    for (auto pSpec : *input->m_Specifiers)
                    {
                        if (pSpec->m_MethodSpecifier)
                        {
                            CppLiteErrorMsg("%s : only allowed for methods", pSpec->m_MethodSpecifier->m_STATIC);
                            return true;
                        }
                        //                         else
                        //                         {
                        //                             if (!resolveMemberSpecifier(pSpec->m_MemberSpecifier, pField,
                        //                             bSavedApplied))
                        //                                 return true;
                        //                         }
                    }
                }
                m_Data.mapAndReturnElement(input, pField);
                fields.push_back(pField);
                if (pFunctionOrVariableEnd->m_Declarators)
                {
                    for (auto pDecl : *pFunctionOrVariableEnd->m_Declarators)
                    {
                        bool bSavedApplied = false;
                        if (!resolveDeclarator(pType, pDecl, pInitExpression, true, false))
                            return true;
                        Field* pField = NewInScope<Field>(pType, pDecl->m_IDENTIFIER, PHANTOM_R_FILTER_FIELD,
                                                          PHANTOM_R_NONE, PHANTOM_R_FLAG_NONE);
                        pField->setAccess(CppLiteGetAccess());
                        if (align > pField->getAlignment())
                            pField->setAlignment(align);
                        CppLiteGuard(pField);
                        for (auto pSpec : *input->m_Specifiers)
                        {
                            CPPLITEPARSER_ASSERT(!pSpec->m_MethodSpecifier);
                            //                             if (!resolveMemberSpecifier(pSpec->m_MemberSpecifier,
                            //                             pField, bSavedApplied))
                            //                                 return true;
                        }
                        m_Data.mapElement(input, pField);
                        CppLiteUnguard(pField);
                        fields.push_back(pField);
                    }
                }

                for (Field* pField : fields)
                {
                    CppLiteErrorReturnIfNameUsed(pClassType, pField->getName().data());
                    CppLiteErrorReturnIf(
                    pField->getValueType()->removeQualifiers()->removeArray()->hasStrongDependencyOnType(pClassType),
                    "'%s' : field type '%.*s' depends strongly on the current class type '%.*s'",
                    pField->getValueType()->getName().data(),
                    PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pField->getValueType()->removeEverything())),
                    PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pClassType)));
                    CppLiteUnguard(pField);
                    pClassType->addField(pField);
                }
                CppLiteUnguard(pField);
            }
        }
        // operators
        else if (input->m_OperatorEnd)
        {
            char        other[12] = "operator";
            const char* op = nullptr;
            if (input->m_OperatorEnd->m_Operator->m_ASSIGN.hasValue())
            {
                strcpy(&other[8], input->m_OperatorEnd->m_Operator->m_ASSIGN.as_string());
                op = other;
            }
            else if (input->m_OperatorEnd->m_Operator->m_GreaterThan.hasValue())
            {
                op = "operator>";
            }
            else if (input->m_OperatorEnd->m_Operator->m_OpBracket)
            {
                op = "operator[]";
            }
            else if (input->m_OperatorEnd->m_Operator->m_OpParen)
            {
                op = "operator()";
            }
            else if (input->m_OperatorEnd->m_Operator->m_RightShift)
            {
                op = "operator>>";
            }
            else if (input->m_OperatorEnd->m_Operator->m_RightShiftAssign)
            {
                op = "operator>>=";
            }
            CPPLITEPARSER_ASSERT(op);
            if (!resolveFunctionEnd(input->m_TypeOrAuto, op, input->m_OperatorEnd->m_FunctionEnd, modifiers,
                                    pSubroutine, true))
                return true;
            CPPLITEPARSER_ASSERT(pSubroutine->asMethod());
        }

        if (pSubroutine)
        {
            Modifiers modifiers = pSubroutine->getModifiers();
            // CppLiteErrorReturnIf(!(pSubroutine->getReturnType()->isSized()), "'%s' : use of
            // incomplete type '%s'", pSubroutine->getName().data(),
            // PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pSubroutine->getReturnType()->removeEverything())));
            const char* name = pSubroutine->getName().data();
            Subroutines conflictingSubroutines;
            if (!(pClassType->acceptsSubroutine(pSubroutine->getReturnType(), name, pSubroutine->getParameterTypes(),
                                                modifiers, PHANTOM_R_NONE, &conflictingSubroutines)))
            {
                CppLiteErrorMsg("'%s' : conflicts with previously defined function%s", name,
                                conflictingSubroutines.size() > 1 ? "s" : "");
                for (auto pSub : conflictingSubroutines)
                {
                    CppLiteSubError("%.*s", PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pSub)));
                }
                return true;
            }
            if (Function* pFunction = pSubroutine->asFunction())
            {
                pClassType->addFunction(pFunction);
            }
            else if (Method* pMethod = pSubroutine->asMethod())
            {
                pClassType->addMethod(pMethod);
                if ((modifiers & PHANTOM_R_OVERRIDE) == PHANTOM_R_OVERRIDE)
                {
                    Class* pClass = pClassType->asClass();
                    CppLiteGuard(pMethod);
                    CppLiteErrorReturnIf(pClass == nullptr,
                                         "'%s' : method marked 'override' can only be defined in a class", name);
                }
            }
            else
            {
                pSubroutine->setOwner(pClassType);
            }

            if ((input->m_FunctionOrVariableEnd && input->m_FunctionOrVariableEnd->m_FunctionEnd &&
                 input->m_FunctionOrVariableEnd->m_FunctionEnd->m_FunctionBlock) ||
                (input->m_OperatorEnd && input->m_OperatorEnd->m_FunctionEnd))
            {
                addBlock(pSubroutine);
            }
            CppLiteMapAndReturn(pSubroutine);
        }
        return true;
    }

    bool traverseMethod(ast::Method* input)
    {
        auto pClassType = CppLiteGetScopeAs(ClassType);
        CppLiteCheckShouldWeContinueParsing(pClassType);

        Modifiers modifiers = 0;
        if (input->m_MethodSpecifiers && input->m_MethodSpecifiers->size())
        {
            for (auto methodSpecifier : *input->m_MethodSpecifiers)
            {
                switch (methodSpecifier->m_STATIC[0])
                {
                case 's':
                    CppLiteErrorReturnIf((modifiers & PHANTOM_R_STATIC) != 0, "static : specifier already used");
                    modifiers |= PHANTOM_R_STATIC;
                    break;
                case '@':
                    CppLiteErrorReturnIf((modifiers & PHANTOM_R_ABSTRACT) != 0, "@abstract : specifier already used");
                    modifiers |= PHANTOM_R_ABSTRACT;
                    break;
                case 'v':
                    CppLiteErrorReturnIf((modifiers & PHANTOM_R_VIRTUAL) != 0, "virtual : specifier already used");
                    modifiers |= PHANTOM_R_VIRTUAL;
                    break;
                default:
                    CPPLITEPARSER_ASSERT(false);
                }
            }
        }
        if ((modifiers & PHANTOM_R_ABSTRACT) != 0)
        {
            modifiers |= PHANTOM_R_VIRTUAL;
        }
        if ((modifiers & PHANTOM_R_STATIC) != 0)
        {
            CppLiteErrorReturnIf((modifiers & PHANTOM_R_VIRTUAL) != 0, "static : incompatible with virtual|final");
        }

        const char* name = input->m_IDENTIFIER;
        Subroutine* pSubroutine = nullptr;
        if (!resolveFunctionEnd(input->m_TypeOrAuto, name, input->m_FunctionEnd, modifiers, pSubroutine, false))
        {
            return true;
        }

        Subroutines conflictingSubroutines;
        if (!(pClassType->acceptsSubroutine(pSubroutine->getReturnType(), name, pSubroutine->getParameterTypes(),
                                            modifiers, PHANTOM_R_NONE, &conflictingSubroutines)))
        {
            CppLiteErrorMsg("'%s' : conflicts with previously defined function%s", name,
                            conflictingSubroutines.size() > 1 ? "s" : "");
            for (auto pSub : conflictingSubroutines)
            {
                CppLiteSubError("%.*s", PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pSub)));
            }
            return true;
        }
        if (Function* pFunction = pSubroutine->asFunction())
        {
            pClassType->addFunction(pFunction);
        }
        else if (Method* pMethod = pSubroutine->asMethod())
        {
            if ((modifiers & PHANTOM_R_OVERRIDE) == PHANTOM_R_OVERRIDE)
            {
                Class* pClass = pClassType->asClass();
                CppLiteGuard(pMethod);
                CppLiteErrorReturnIf(pClass == nullptr,
                                     "'%s' : method marked 'override' can only be defined in a class", name);
                Methods methods;
                pClass->findOverriddenMethods(pMethod, methods);

                CppLiteErrorReturnIf(!pClass->isTemplateDependant() && methods.empty(),
                                     "'%s' : method marked 'override' does not override "
                                     "any virtual function from any base class",
                                     name);
                CppLiteUnguard(pMethod);
            }
            pClassType->addMethod(pMethod);
        }
        else
        {
            pSubroutine->setOwner(pClassType);
        }
        if (input->m_FunctionEnd->m_FunctionBlock)
        {
            addBlock(pSubroutine);
        }
        CppLiteMapAndReturn(pSubroutine);
    }

    bool traverseTemplateFunction(ast::TemplateFunction* input)
    {
        if (Subroutine* pSubroutine = CppLiteGetElementAs(Subroutine)) // already compiled above
            return true;
        TemplateSpecialization* pTemplateSpec = nullptr;
        if (!(resolveTemplateSpecialization(input, /*input->m_TemplateArgumentList*/ nullptr, pTemplateSpec, true)))
            return false;

        const char* name = input->m_IDENTIFIER;
        Subroutine* pSubroutine = nullptr;
        m_Data.pushScope(pTemplateSpec);
        if (!resolveFunctionEnd(input->m_TypeOrAuto, name, input->m_FunctionEnd,
                                input->m_STATIC.hasValue() ? PHANTOM_R_STATIC : Modifiers{}, pSubroutine, false))
        {
            m_Data.popScope();
            return true;
        }
        m_Data.popScope();
        CppLiteMapAndReturn(pSubroutine);
    }

    bool traverseTypedef(ast::Typedef* input)
    {
        ClassType* pClassType = CppLiteGetScopeAs(ClassType);
        if (pClassType)
        {
            CppLiteErrorReturnIfNameUsed(pClassType, input->m_IDENTIFIER);
            Type* pType = CppLiteVisitType(input->m_Type);
            if (pType == nullptr)
                return true;
            Alias* pAlias = pClassType->addAlias(pType, input->m_IDENTIFIER);
            pAlias->setAccess(CppLiteGetAccess());
            CppLiteMapAndReturn(pAlias);
        }
        return true;
    }

    bool traverseAlias(ast::Alias* input)
    {
        ClassType* pClassType = CppLiteGetScopeAs(ClassType);
        if (pClassType)
        {
            CppLiteErrorReturnIfNameUsed(pClassType, input->m_IDENTIFIER);
            Type* pType = CppLiteVisitType(input->m_Type);
            if (pType == nullptr)
                return true;
            Alias* pAlias = pClassType->addAlias(pType, input->m_IDENTIFIER);
            pAlias->setAccess(CppLiteGetAccess());
            CppLiteMapAndReturn(pAlias);
        }
        return true;
    }

    bool traverseFriend(ast::Friend* input)
    {
        ClassType* pClassType = CppLiteGetScopeAs(ClassType);
        if (pClassType)
        {
            Symbol* pSymbol = CppLiteVisitSymbol(input->m_QualifiedName);
            CppLiteErrorReturnIf(pSymbol == nullptr, "friend : cannot resolve symbol");
            CppLiteErrorReturnIf(pSymbol == pClassType, "friend : cannot be friend with itself");
            pClassType->addFriend(pSymbol);
        }
        return true;
    }
};

struct CppLitePassMembersGlobal : public CppLitePass<CppLitePassMembersGlobal>
{
    CppLitePassMembersGlobal(CppLitePassData& a_Data) : CppLitePass<CppLitePassMembersGlobal>(a_Data) {}

    bool traverseTypedef(ast::Typedef* input) { return true; }
    bool traverseUsing(ast::Using* input) { return true; }
    bool traverseFriend(ast::Friend* input) { return true; }
    bool traverseAlias(ast::Alias* input) { return true; }
    bool traverseMixinStatementDefinition(ast::MixinStatementDefinition* input)
    {
        m_Data.m_MixinStatementDefinitions[input->m_IDENTIFIER] = input;
        return true;
    }

    bool traverseTemplate(ast::Template* input)
    {
        if (input->m_Class)
        {
            traverse(input->m_Class);
        }
        else if (input->m_Union)
        {
            traverse(input->m_Union);
        }
        else if (input->m_Alias)
        {
            traverse(input->m_Alias);
        }
        else if (input->m_TemplateFunction)
        {
            traverse(input->m_TemplateFunction);
        }
        else if (input->m_Constructor)
        {
            traverse(input->m_Constructor);
        }
        else if (input->m_TemplateSignature)
        {
            CppLiteError("Template are forbiden");
        }
        else
        {
            CPPLITEPARSER_ASSERT(false && "Unconsistent ast::Template");
        }

        return true;
    }

    bool traverseClass(ast::Class* input)
    {
        Class* pClass = CppLiteGetElementAs(Class);
        CppLiteCheckShouldWeContinueParsing(pClass);
        for (Class* pBaseClass : pClass->getBaseClasses())
        {
            if (!pBaseClass->isNative())
            {
                CppLiteGetSemantic()->buildClass(pBaseClass, Semantic::e_ClassBuildState_Members);
            }
        }
        pClass->setCurrentAccess(pClass->getDefaultAccess());
        if (input->m_Members)
        {
            CppLiteVisitElements(input->m_Members, pClass);
        }
        return true;
    }

    bool traverseUnion(ast::Union* input)
    {
        Union* pUnion = CppLiteGetElementAs(Union);
        if (pUnion == nullptr)
            return true;
        pUnion->setCurrentAccess(pUnion->getDefaultAccess());
        if (input->m_BasicMembers)
        {
            CppLiteVisitElements(input->m_BasicMembers, pUnion);
        }
        return true;
    }

    // skip everything except notify statement to know if we need @notify signals in @property
    bool traverseStatement(ast::Statement* input) { return true; }

    bool traverseStaticAssert(ast::StaticAssert* input) { return true; }

    bool traverseConstructor(ast::Constructor* input);

    bool traverseFunctionOrVariable(ast::FunctionOrVariable* input)
    {
        ClassType* pClassType = CppLiteGetScopeAs(ClassType);
        if (pClassType == nullptr)
            return true; // only looking for methods

        return true;
    }

    bool traverseTemplateFunction(ast::TemplateFunction* input);

    bool traverseMethod(ast::Method* input)
    {
        ClassType* pClassType = CppLiteGetScopeAs(ClassType);
        if (pClassType == nullptr)
            return true; // only looking for methods
        if (Subroutine* pSubroutine = CppLiteGetElementAs(Subroutine))
        {
        }
        return true;
    }
};

// members
struct CppLitePassStates : public CppLitePass<CppLitePassStates>
{
    CppLitePassStates(CppLitePassData& a_Data) : CppLitePass<CppLitePassStates>(a_Data) {}

    // skip every function blocks in this pass
    bool traverseFunctionBlock(ast::FunctionBlock* input) { return true; }

    // skip other members than statemachine/states/tracks
    bool traverseMethod(ast::Method* input) { return true; }
    bool traverseConversionFunction(ast::ConversionFunction* input) { return true; }
    bool traverseConstructor(ast::Constructor* input) { return true; }
    bool traverseDestructor(ast::Destructor* input) { return true; }
    bool traverseOperator(ast::Operator* input) { return true; }
    bool traverseTypedef(ast::Typedef* input) { return true; }
    bool traverseUsing(ast::Using* input) { return true; }
    bool traverseFriend(ast::Friend* input) { return true; }
    bool traverseAlias(ast::Alias* input) { return true; }
    bool traverseMixinStatementDefinition(ast::MixinStatementDefinition* input) { return true; }
    bool traverseStaticAssert(ast::StaticAssert* input) { return true; }

    bool traverseFunctionOrVariable(ast::FunctionOrVariable* input) { return true; }

    bool traverseTemplate(ast::Template* input)
    {
        if (input->m_Class)
        {
            traverse(input->m_Class);
        }
        return true;
    }

    bool traverseClass(ast::Class* input)
    {
        if (CppLiteHasMappedElement(input))
        {
            Class* pClass = CppLiteGetElementAs(Class);
            CppLiteCheckShouldWeContinueParsing(pClass);
            if (input->m_Members)
            {
                CppLiteVisitElements(input->m_Members, pClass);
            }
        }
        return true;
    }
};

// field default values
struct CppLitePassFieldDefaults : public CppLitePass<CppLitePassFieldDefaults>
{
    CppLitePassFieldDefaults(CppLitePassData& a_Data) : CppLitePass<CppLitePassFieldDefaults>(a_Data) {}
    bool traverseMixinStatementDefinition(ast::MixinStatementDefinition* input) { return true; }

    bool traverseTemplate(ast::Template* input)
    {
        if (input->m_Class)
        {
            traverse(input->m_Class);
        }
        else if (input->m_Union)
        {
            traverse(input->m_Union);
        }
        else if (input->m_Alias)
        {
            traverse(input->m_Alias);
        }
        else if (input->m_TemplateFunction)
        {
            return true;
        }
        else if (input->m_Constructor)
        {
            return true;
        }
        else if (input->m_TemplateSignature)
        {
            CppLiteError("Template are forbiden");
        }
        else
        {
            CPPLITEPARSER_ASSERT(false && "Unconsistent ast::Template");
        }

        return true;
    }

    bool traverseClass(ast::Class* input)
    {
        Class* pClass = CppLiteGetElementAs(Class);
        if (CppLiteHasMappedElement(input))
        {
            CppLiteCheckShouldWeContinueParsing(pClass);
            resolveFieldDefaults(input, pClass);
        }
        return true;
    }
    bool traverseUnion(ast::Union* input)
    {
        Union* pUnion = CppLiteGetElementAs(Union);
        if (pUnion == nullptr)
            return true;
        if (input->m_BasicMembers)
        {
            LocalVariableExpression* pThisExp = NewInScope<LocalVariableExpression>(pUnion);
            CppLiteGuard(pThisExp);
            m_Data.pushImplicitThisExpression(pThisExp);
            CppLiteVisitElements(input->m_BasicMembers, pUnion);
            m_Data.popImplicitThisExpression();
        }
        return true;
    }

    // skip every function blocks in this pass
    bool traverseFunctionBlock(ast::FunctionBlock* input) { return true; }

    // skip other members than fields
    bool traverseMethod(ast::Method* input) { return true; }
    bool traverseConversionFunction(ast::ConversionFunction* input) { return true; }
    bool traverseConstructor(ast::Constructor* input) { return true; }
    bool traverseDestructor(ast::Destructor* input) { return true; }
    bool traverseOperator(ast::Operator* input) { return true; }
    bool traverseUsing(ast::Using* input) { return true; }
    bool traverseFriend(ast::Friend* input) { return true; }
    bool traverseTypedef(ast::Typedef* input) { return true; }
    bool traverseAlias(ast::Alias* input) { return true; }
    bool traverseStaticAssert(ast::StaticAssert* input) { return true; }

    bool traverseFunctionOrVariable(ast::FunctionOrVariable* input)
    {
        Modifiers modifiers = 0;

        LanguageElements elements = m_Data.findElements(input);
        if (elements.empty())
            return true;

        Fields fields;
        for (auto elem : elements)
        {
            if (Field* pField = elem->asField())
            {
                fields.push_back(pField);
            }
        }

        if (fields.empty())
            return true;

        for (size_t i = 0; i < fields.size(); ++i)
        {
            Field* pField = fields[i];

            Type* pType = pField->getValueType();

            if (auto pFunctionOrVariableEnd = input->m_FunctionOrVariableEnd)
            {
                const char* name = pFunctionOrVariableEnd->m_IDENTIFIER;
                if (i == 0)
                {
                    if ((pFunctionOrVariableEnd->m_CallList ||
                         pFunctionOrVariableEnd->m_AssignExpressionOrInitializerList))
                    {
                        Expression* pInitExpression = nullptr;
                        if (pType->hasAuto())
                        {
                            ast::AutoDeclarator decl;
                            decl.m_IDENTIFIER = name;
                            decl.m_CallList = pFunctionOrVariableEnd->m_CallList;
                            decl.m_AssignExpressionOrInitializerList =
                            pFunctionOrVariableEnd->m_AssignExpressionOrInitializerList;
                            decl.m_TypeExtents = nullptr;
                            decl.m_ArrayExtents = nullptr;
                            if (!resolveDeclarator(pType, &decl, pInitExpression, false, true))
                            {
                                memset(&decl, 0,
                                       sizeof(ast::AutoDeclarator)); // ensure content is not deleted
                                return true;
                            }
                            memset(&decl, 0,
                                   sizeof(ast::AutoDeclarator)); // ensure content is not deleted
                        }
                        else
                        {
                            ast::Declarator decl;
                            decl.m_IDENTIFIER = name;
                            decl.m_CallList = pFunctionOrVariableEnd->m_CallList;
                            decl.m_AssignExpressionOrInitializerList =
                            pFunctionOrVariableEnd->m_AssignExpressionOrInitializerList;
                            decl.m_TypeExtents = nullptr;
                            decl.m_ArrayExtents = pFunctionOrVariableEnd->m_ArrayExtents;
                            if (!resolveDeclarator(pType, &decl, pInitExpression, false, true))
                            {
                                memset(&decl, 0,
                                       sizeof(ast::Declarator)); // ensure content is not deleted
                                return true;
                            }
                            memset(&decl, 0,
                                   sizeof(ast::Declarator)); // ensure content is not deleted
                        }
                        if (pInitExpression)
                        {
                            if (!(pField->isTemplateElement()))
                            {
                                pInitExpression =
                                CppLiteInitNE(pInitExpression, pField->getValueType(), pField->getOwner());
                                if (pInitExpression == nullptr)
                                    return true;
                            }
                            pField->setDefaultExpression(pInitExpression);
                        }
                    }
                }
                else
                {
                    Expression* pInitExpression = nullptr;
                    CPPLITEPARSER_ASSERT(pFunctionOrVariableEnd->m_Declarators);
                    if (!resolveDeclarator(pType, (*pFunctionOrVariableEnd->m_Declarators)[i - 1], pInitExpression,
                                           false, true))
                        return true;
                    if (pInitExpression)
                    {
                        pField->setDefaultExpression(pInitExpression);
                    }
                }
            }
        }
        return true;
    }
};

// blocks
struct CppLitePassBlocks : public CppLitePass<CppLitePassBlocks>
{
    using BaseType = CppLitePass<CppLitePassBlocks>;

    CppLitePassBlocks(CppLitePassData& a_Data) : BaseType(a_Data) {}

    template<class TT>
    Type* resolveType(TT* input)
    {
        return resolveType(input, m_Data.getScope());
    }
    template<class TT>
    Type* resolveType(TT* input, LanguageElement* a_pScope)
    {
        CPPLITEPARSER_ASSERT(a_pScope);
        LanguageElement* pElement = resolveElement(input, a_pScope);
        if (pElement)
        {
            auto pResult = pElement->toType();
            if (pResult)
            {
                if (!pResult->isTemplateDependant() && !pResult->isNative() && pResult->asClassType())
                {
                    m_Data.getSemantic()->edifyClass(static_cast<ClassType*>(pResult));
                }
                return pResult;
            }
            CppLiteErrorMsg("'%.*s' : symbol found but not a type", PHANTOM_STRING_AS_PRINTF_ARG(pElement->getName()));
            CppLiteGuard(pElement);
        }
        return nullptr;
    }

    bool traverseStatement(ast::Statement* input)
    {
        BaseType::traverseStatement(input);

        while (auto pA = m_Data.popPackArgument())
            CppLiteError("'%.*s' : missing pack expansion", PHANTOM_STRING_AS_PRINTF_ARG(pA->asSymbol()->getName()));
        while (auto pP = m_Data.popPackExpansionParameter())
            CppLiteError("'%.*s' : missing pack expansion", PHANTOM_STRING_AS_PRINTF_ARG(pP->getName()));

        return true;
    }

    // skip useless members
    bool traverseUsing(ast::Using* input)
    {
        if (auto pScope = CppLiteGetScopeAs(Scope))
        {
            Symbol* pSymbol = CppLiteVisitSymbol(input->m_QualifiedName);
            if (pSymbol)
            {
                CppLiteErrorReturnIfNameUsed(pScope->asLanguageElement(), pSymbol->getName().data());
                Alias* pAlias = pScope->addAlias(pSymbol, pSymbol->getName());
                CppLiteMapAndReturn(pAlias);
            }
        }
        else
        {
            CppLiteError("using : declaration out of valid scope");
        }
        return true;
    }
    bool traverseFriend(ast::Friend* input) { return true; }
    bool traverseTypedef(ast::Typedef* input) { return true; }
    bool traverseAlias(ast::Alias* input)
    {
        if (CppLiteGetScopeAs(Block))
            return resolveAlias(input);
        return true;
    }

    bool traverseTemplate(ast::Template* input)
    {
        if (input->m_Class)
        {
            traverse(input->m_Class);
        }
        else if (input->m_Union)
        {
            traverse(input->m_Union);
        }
        else if (input->m_Alias)
        {
            traverse(input->m_Alias);
        }
        else if (input->m_TemplateFunction)
        {
            traverse(input->m_TemplateFunction);
        }
        else if (input->m_Constructor)
        {
            traverse(input->m_Constructor);
        }
        else if (input->m_TemplateSignature)
        {
            CppLiteError("Template are forbiden");
        }
        else
            assert(false && "Unconsistent ast::Template");

        return true;
    }

    /// @static_assert
    bool traverseStaticAssert(ast::StaticAssert* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        if (pBlock)
            return _traverseStaticAssert(input);
        return true;
    }

    bool traverseMixinStatementDefinition(ast::MixinStatementDefinition* input)
    {
        m_Data.m_MixinStatementDefinitions[input->m_IDENTIFIER] = input;
        return true;
    }

    bool traverseClass(ast::Class* input)
    {
        if (!CppLiteHasMappedElement(input))
        {
            CppLitePassGlobals(m_Data).traverseClass(input);
            Class* pClass = CppLiteGetElementAs(Class);
            if (!pClass)
                return true;
            CppLitePassInheritance(m_Data).traverseClass(input);
            CppLiteGetSemantic()->buildClass(pClass, Semantic::e_ClassBuildState_Inheritance);
            CppLitePassMembersLocal(m_Data).traverseClass(input); // in block
            CppLitePassMembersGlobal(m_Data).traverseClass(input);
            CppLiteGetSemantic()->buildClass(pClass, Semantic::e_ClassBuildState_Sized);
            CppLitePassFieldDefaults(m_Data).traverseClass(input);
            CppLiteGetSemantic()->buildClass(pClass, Semantic::e_ClassBuildState_ImplicitsAndDefaults);
        }
        Class* pClass = CppLiteGetElementAs(Class);
        CppLiteCheckShouldWeContinueParsing(pClass);
        if (input->m_Members)
        {
            LocalVariableExpression* pThisExp = NewInScope<LocalVariableExpression>(pClass);
            CppLiteGuard(pThisExp);
            m_Data.pushImplicitThisExpression(pThisExp);
            CppLiteVisitElements(input->m_Members, pClass);
            m_Data.popImplicitThisExpression();
        }
        return true;
    }

    bool traverseUnion(ast::Union* input)
    {
        Union* pUnion = CppLiteGetElementAs(Union);
        CppLiteCheckShouldWeContinueParsing(pUnion);
        if (input->m_BasicMembers)
        {
            LocalVariableExpression* pThisExp = NewInScope<LocalVariableExpression>(pUnion);
            CppLiteGuard(pThisExp);
            m_Data.pushImplicitThisExpression(pThisExp);
            CppLiteVisitElements(input->m_BasicMembers, pUnion);
            m_Data.popImplicitThisExpression();
        }
        return true;
    }

    CallExpression* createPrintCall(StringView a_Text)
    {
        static Function* pPrintFunc = CppLite::Get()->findFunction("phantom::print(const char*,int,int)");
        CPPLITEPARSER_ASSERT(pPrintFunc);

        Expressions args;
        args.push_back(NewInScope<StringLiteralExpression>('"' + a_Text + '"'));
        args.push_back(NewInScope<ConstantExpression>(createConstant<int>(1)));
        args.push_back(NewInScope<ConstantExpression>(createConstant<int>(0)));

        return (NewInScope<CallExpression>(pPrintFunc, args));
    }

    bool traverseFunctionBlock(ast::FunctionBlock* input)
    {
        Block*      pBlock = CppLiteGetScopeAs(Block);
        Subroutine* pSubroutine;
        if (!pBlock)
        {
            pSubroutine = CppLiteGetScopeAs(Subroutine);
            CppLiteCheckShouldWeContinueParsing(pSubroutine);
            if ((pBlock = pSubroutine->getBlock()) == nullptr)
                return true;
        }
        else
            pSubroutine = pBlock->getSubroutine();

        if (CppLiteHasFeature("DebugTraceFunction"))
        {
            Expression* pExpression = createPrintCall(pSubroutine->getUniqueName() + '\n');
            pBlock->addStatement(pExpression);
        }

        // cancel deferred block building as we are building it now
        pSubroutine->setBlockBuilder({});

        m_Data.pushImplicitThisExpression(nullptr);
        if (input->m_Statements)
        {
            for (auto pS : *input->m_Statements)
            {
                CppLiteVisitElement(pS, pBlock);
            }
        }
        m_Data.popImplicitThisExpression();

        auto pAuto = Application::Get()->getAuto();
        auto pSignature = pSubroutine->getSignature();
        if (pSignature->getReturnType() == pAuto)
        {
            pSignature->setReturnType(PHANTOM_TYPEOF(void));
        }
        CppLiteErrorReturnIf(
        pSignature->getReturnType() != PHANTOM_TYPEOF(void) &&
        (pBlock->getStatements().empty() || (pBlock->getStatements().back())->asReturnStatement() == nullptr),
        "'%s' : function must return a value at end of block", CppLiteSubroutineName(pSubroutine).data());
        CppLiteMapAndReturn(pBlock);
    }
    bool traverseConstructorInitializer(ast::ConstructorInitializer* input) { return true; }
    bool traverseConstructor(ast::Constructor* input)
    {
        Constructor* pConstructor = CppLiteGetElementAs(Constructor);
        CppLiteCheckShouldWeContinueParsing(pConstructor);

        if (input->m_FunctionBlock)
        {
            //
            //         if (auto pSpec = pConstructor->getTemplateSpecialization())
            //         {
            //             if (pConstructor->getBlockBuilder().empty())
            //                 return true;
            //             pConstructor->setBlockBuilder({});
            //
            //             CppLiteErrorReturnIf((pConstructor->testModifiers(Modifier::Defaulted)),
            //                                  "template constructors cannot be defaulted");
            //         }
            //         else if (!(pConstructor->testModifiers(Modifier::Deleted)) &&
            //                  !(pConstructor->testModifiers(Modifier::Defaulted)))
            //         {
            //             addBlock(pConstructor);
            //         }
            //         else
            //         {
            //             return true;
            //         }

            ClassType* pCtorClassType = pConstructor->getOwnerClassType();

            ast::ConstructorInitializers* ctorInitializers = input->m_ConstructorInitializers;
            ast::ConstructorInitializers  emptyCtorInitializers;
            if (!ctorInitializers)
                ctorInitializers = &emptyCtorInitializers;

            phantom::SmallVector<Pair<Class*, ast::ConstructorInitializer*>>     declBaseClasses;
            phantom::SmallVector<Pair<Field*, ast::ConstructorInitializer*>, 16> declFields;
            phantom::SmallVector<Pair<Class*, ast::ConstructorInitializer*>>     orderedBaseClasses;
            phantom::SmallVector<Pair<Field*, ast::ConstructorInitializer*>, 16> orderedFields;
            bool                                                                 inheritedConstructor = false;
            for (ast::ConstructorInitializer* ctorInit : *ctorInitializers)
            {
                Symbol* pSym = CppLiteVisitSymbol(ctorInit->m_BasicType, pCtorClassType);
                if (!pSym)
                {
                    continue;
                }
                if (Class* pClass = pSym->asClass())
                {
                    if (pClass == pCtorClassType)
                    {
                        inheritedConstructor = true;
                        pConstructor->addFlags(PHANTOM_R_FLAG_INHERITED);
                    }
                    declBaseClasses.emplace_back(pClass, ctorInit);
                }
                else if (Field* pField = pSym->asField())
                {
                    declFields.emplace_back(pField, ctorInit);
                }
                else if (pSym->isTemplateDependant())
                {
                    if (pSym->getMetaClass() == PHANTOM_CLASSOF(TemplateDependantElement))
                    {
                        auto pTDE = static_cast<TemplateDependantElement*>(pSym);
                        if (!pTDE->getTemplateArguments())
                        {
                            if (Field* pField = pCtorClassType->getField(pSym->getName()))
                            {
                                declFields.emplace_back(pField, ctorInit);
                                continue;
                            }
                        }
                    }
                    Type* pType = pSym->toType();
                    assert(pType);
                    pClass = pType->asClass();
                    if (!pClass)
                        pClass = NewInScope<TemplateDependantClassPromotion>(pType);
                    if (pClass->getQualifiedDecoratedName() == pCtorClassType->getQualifiedDecoratedName())
                    {
                        inheritedConstructor = true;
                        pConstructor->addFlags(PHANTOM_R_FLAG_INHERITED);
                    }
                    declBaseClasses.emplace_back(pClass, ctorInit);
                }
                else
                {
                    CppLiteErrorMsg(
                    "constructor initializer must be either a field, a class or a template dependant expression");
                    continue;
                }
            }

            Block* pBlock = pConstructor->getBlock();
            CPPLITEPARSER_ASSERT(pConstructor->getBlock());

            Class* pCtorClass = pCtorClassType->asClass();
            if (pCtorClass)
            {
                if (!inheritedConstructor)
                {
                    for (Class* pBaseClass : pCtorClass->getBaseClasses())
                    {
                        bool initialized = false;
                        for (auto& declBaseClass : declBaseClasses)
                        {
                            if (pBaseClass->isSame(declBaseClass.first))
                            {
                                CppLiteErrorReturnIf(
                                initialized, "'%.*s' : base class already initialized",
                                PHANTOM_STRING_AS_PRINTF_ARG(declBaseClass.first->getDecoratedName()));
                                initialized = true;
                                orderedBaseClasses.emplace_back(declBaseClass);
                            }
                        }
                        if (!initialized)
                        {
                            orderedBaseClasses.emplace_back(pBaseClass, nullptr);
                        }
                    }
                }
            }
            if (!inheritedConstructor)
            {
                for (auto pField : pCtorClassType->getFields())
                {
                    bool initialized = false;
                    for (auto declField : declFields)
                    {
                        if (pField == declField.first)
                        {
                            CppLiteErrorReturnIf(initialized, "'%.*s' : field already initialized",
                                                 PHANTOM_STRING_AS_PRINTF_ARG(declField.first->getName()));
                            initialized = true;
                            orderedFields.emplace_back(declField);
                        }
                    }
                    if (!initialized)
                    {
                        orderedFields.emplace_back(pField, nullptr);
                    }
                }
                //
                //             bool memberNotFound = false;
                //             for (size_t i = 0; i < existingCtorInits.size(); ++i)
                //             {
                //                 if (!existingCtorInits[i])
                //                 {
                //                     memberNotFound = true;
                //                     CppLiteErrorMsg("'%s' : undefined field or base class in constructor
                //                     initializer",
                //                                     (*ctorInitializers)[i]->m_IDENTIFIER);
                //                 }
                //             }
                //             if (memberNotFound)
                //             {
                //                 return true;
                //             }
            }
            else
            {
                orderedBaseClasses = std::move(declBaseClasses);
            }

            for (auto& sym_ci : orderedBaseClasses)
            {
                Class*                       pBaseClass = sym_ci.first;
                ast::ConstructorInitializer* pCI = sym_ci.second;
                if (pCI) // explicit base class constructor call
                {
                    if (pCI->m_CallList)
                    {
                        Expressions expressions;
                        CppLitesGuard(expressions);
                        if (!(resolveCallList(pCI->m_CallList, expressions, pBlock)))
                            return true;

                        bool bTemplateDependant = pBaseClass->isTemplateDependant();
                        if (!bTemplateDependant)
                        {
                            for (auto exp : expressions)
                                if (exp->isTemplateDependant())
                                {
                                    bTemplateDependant = true;
                                    break;
                                }
                        }

                        if (bTemplateDependant)
                        {
                            LanguageElements elems(expressions.begin(), expressions.end());
                            pBlock->addStatement(NewInScope<BaseConstructorCallStatement>(
                            pCtorClass,
                            NewInScope<TemplateDependantExpression>(NewInScope<TemplateDependantElement>(
                            pBaseClass, "", phantom::NullOpt, MakeArrayView(elems)))));
                        }
                        else
                        {
                            LanguageElement* pExp = CppLiteGetSemantic()->qualifiedLookup(
                            pBaseClass, "", phantom::NullOpt, MakeArrayView(expressions), pBlock);
                            if (pExp == nullptr)
                                return true;
                            CPPLITEPARSER_ASSERT(pExp->asExpression());
                            pBlock->addStatement(NewInScope<BaseConstructorCallStatement>(
                            pCtorClass, static_cast<Expression*>(pExp)->loadRValueReferenceExpression()));
                        }
                    }
                    else
                    {
                        CPPLITEPARSER_ASSERT(pCI->m_InitializerList);
                        m_Data.pushInitializationType(pBaseClass);
                        Expression* pExpression = CppLiteVisitExpression(pCI->m_InitializerList, pBlock);
                        CppLiteGuard(pExpression);
                        m_Data.popInitializationType();
                        if (pExpression == nullptr)
                            return true;
                        pBlock->addStatement(NewInScope<BaseConstructorCallStatement>(pCtorClass, pExpression));
                    }
                }
            }
            for (auto pair : orderedFields)
            {
                Field*                       pField = pair.first;
                ast::ConstructorInitializer* pCI = pair.second;
                if (pCI)
                {
                    if (pCI->m_CallList)
                    {
                        Expressions expressions;
                        CppLitesGuard(expressions);
                        if (!(resolveCallList(pCI->m_CallList, expressions, pBlock)))
                            return true;

                        bool bTemplateDependant = pField->isTemplateDependant();
                        if (!bTemplateDependant)
                        {
                            for (auto exp : expressions)
                                if (exp->isTemplateDependant())
                                {
                                    bTemplateDependant = true;
                                    break;
                                }
                        }

                        if (bTemplateDependant)
                        {
                            LanguageElements elems(expressions.begin(), expressions.end());
                            pBlock->addStatement(NewInScope<FieldInitializationStatement>(
                            pField,
                            NewInScope<TemplateDependantExpression>(NewInScope<TemplateDependantElement>(
                            pField->getValueType(), "", phantom::NullOpt, MakeArrayView(elems)))));
                        }
                        else if (expressions.size() == 1)
                        {
                            Expression* pConv = CppLiteInitNE(expressions.back(), pField->getValueType(), pBlock);
                            if (pConv == nullptr)
                                return true;
                            CppLiteGuard(pConv);
                            pBlock->addStatement(NewInScope<FieldInitializationStatement>(pField, pConv));
                        }
                        else
                        {
                            // test by field type default construction
                            LanguageElement* pExp = CppLiteGetSemantic()->qualifiedLookup(
                            pField->getValueType(), pField->getValueType()->getName(), phantom::NullOpt,
                            MakeArrayView(expressions), pBlock);
                            if (pExp == nullptr)
                                return true;
                            CPPLITEPARSER_ASSERT(pExp->asExpression());
                            pBlock->addStatement(NewInScope<FieldInitializationStatement>(
                            pField, pExp->asExpression()->loadRValueReferenceExpression()));
                        }
                    }
                    else
                    {
                        CPPLITEPARSER_ASSERT(pCI->m_InitializerList);
                        m_Data.pushInitializationType(pField->getValueType());
                        Expression* pExpression = CppLiteVisitExpression(pCI->m_InitializerList, pBlock);
                        CppLiteGuard(pExpression);
                        m_Data.popInitializationType();
                        if (pExpression == nullptr)
                            return true;
                        Expression* pExpressionConv = CppLiteInitNE(pExpression, pField->getValueType(), pBlock);
                        if (pExpressionConv == nullptr)
                            return true;
                        pBlock->addStatement(NewInScope<FieldInitializationStatement>(pField, pExpressionConv));
                    }
                }
                else
                {
                    if (pField->getDefaultExpression())
                    {
                        auto implFieldInit = NewInScope<FieldInitializationStatement>(
                        pField,
                        CppLiteInitNE(pField->getDefaultExpression()->clone(CppLiteGetSource()), pField->getValueType(),
                                      CppLiteGetScope()));
                        implFieldInit->setImplicit();
                        pBlock->addStatement(implFieldInit);
                    }
                    else
                    {
                        if (ClassType* pFieldClassType = pField->getValueType()->removeQualifiers()->asClassType())
                        {
                            if (pFieldClassType->isTemplateDependant())
                            {
                                LanguageElements args;
                                pBlock->addStatement(NewInScope<FieldInitializationStatement>(
                                pField,
                                NewInScope<TemplateDependantExpression>(NewInScope<TemplateDependantElement>(
                                pFieldClassType, pFieldClassType->getName(), phantom::NullOpt, MakeArrayView(args)))));
                            }
                            else
                            {
                                Constructor* pFieldConstructor = pFieldClassType->getDefaultConstructor();
                                CppLiteErrorReturnIf(
                                pFieldClassType->isAbstract(), "'%.*s' : '%s.*' is abstract and cannot be cosntructed",
                                PHANTOM_STRING_AS_PRINTF_ARG(pField->getName()),
                                PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pFieldClassType)));
                                CppLiteErrorReturnIf(
                                pFieldConstructor == nullptr, "'%.*s' : '%.*s' has no default constructor",
                                PHANTOM_STRING_AS_PRINTF_ARG(pField->getName()),
                                PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pFieldClassType)));
                                CppLiteErrorReturnIf(
                                !(CppLiteGetSemantic()->isSymbolAccessible(pFieldConstructor, pCtorClassType)),
                                "'%s' : constructor is not accessible", pFieldConstructor->getName().data());
                                pBlock->addStatement(NewInScope<FieldInitializationStatement>(
                                pField, NewInScope<ConstructorCallExpression>(pFieldConstructor)));
                            }
                        }
                        else // fundamentals
                        {
                            CppLiteErrorReturnIf(pField->getValueType()->asReference(),
                                                 "'%s' : references must be initialized", pField->getName().data());
                            CppLiteErrorReturnIf(pField->getValueType()->asEnum(), "'%s' : enums must be initialized",
                                                 pField->getName().data());
                            // pBlock->addStatement(NewInScope<MemSetStatement>(toExpression(pField,
                            // pConstructor->getThis()->dereference())->address(), 0,
                            // pField->getValueType()->getSize()));
                        }
                    }
                }
            }
            CppLiteVisitElement(input->m_FunctionBlock, pConstructor);
        }
        return true;
    }
    bool traverseDestructor(ast::Destructor* input)
    {
        Destructor* pDestructor = CppLiteGetElementAs(Destructor);
        CppLiteCheckShouldWeContinueParsing(pDestructor);
        CppLiteVisitElement(input->m_FunctionBlock, pDestructor);
        return true;
    }
    bool traverseTemplateFunction(ast::TemplateFunction* input)
    {
        Subroutine* pSubroutine = CppLiteGetElementAs(Subroutine);
        CppLiteCheckShouldWeContinueParsing(pSubroutine);
        if (pSubroutine->getBlockBuilder()) // block builder is still here, it means we haven't built the template
                                            // function body block yet
        {
            pSubroutine->setBlockBuilder({});
            CPPLITEPARSER_ASSERT(input->m_FunctionEnd->m_FunctionBlock ||
                                 (pSubroutine->asMethod() && pSubroutine->isPureVirtual()));
            if (input->m_FunctionEnd->m_FunctionBlock)
            {
                CppLiteVisitElement(input->m_FunctionEnd->m_FunctionBlock, pSubroutine);
            }
        }
        return true;
    }
    bool traverseMethod(ast::Method* input)
    {
        Subroutine* pSubroutine = CppLiteGetElementAs(Subroutine);
        CppLiteCheckShouldWeContinueParsing(pSubroutine);
        CPPLITEPARSER_ASSERT(input->m_FunctionEnd->m_FunctionBlock || pSubroutine->isPureVirtual());
        if (input->m_FunctionEnd->m_FunctionBlock)
        {
            CppLiteVisitElement(input->m_FunctionEnd->m_FunctionBlock, pSubroutine);
        }
        return true;
    }

    bool traverseConversionFunction(ast::ConversionFunction* input)
    {
        Method* pMethod = CppLiteGetElementAs(Method);
        CppLiteCheckShouldWeContinueParsing(pMethod);
        CppLiteVisitElement(input->m_FunctionBlock, pMethod);
        return true;
    }

    bool traverseFunctionOrVariable(ast::FunctionOrVariable* input)
    {
        Modifiers modifiers = 0;

        Block* pBlock = CppLiteGetScopeAs(Block);

        // Not at block scope--------------------
        Type*  pType = nullptr;
        Field* pField = nullptr;
        if (pBlock == nullptr)
        {
            if (Subroutine* pSubroutine = CppLiteGetElementAs(Subroutine))
            {
                if (pSubroutine->getOwner() == nullptr) // failed somewhere in the previous pass
                    return true;

                if (input->m_FunctionOrVariableEnd)
                {
                    CPPLITEPARSER_ASSERT(input->m_FunctionOrVariableEnd->m_FunctionEnd->m_FunctionBlock ||
                                         pSubroutine->isPureVirtual());
                    if (input->m_FunctionOrVariableEnd->m_FunctionEnd->m_FunctionBlock)
                    {
                        CPPLITEPARSER_ASSERT(pSubroutine->getBlock());
                        CppLiteVisitElement(input->m_FunctionOrVariableEnd->m_FunctionEnd->m_FunctionBlock,
                                            pSubroutine);
                    }
                }
                else if (input->m_OperatorEnd)
                {
                    if (input->m_OperatorEnd->m_FunctionEnd->m_FunctionBlock)
                    {
                        CPPLITEPARSER_ASSERT(pSubroutine->getBlock());
                        CppLiteVisitElement(input->m_OperatorEnd->m_FunctionEnd->m_FunctionBlock, pSubroutine);
                    }
                    else
                    {
                        CPPLITEPARSER_ASSERT(pSubroutine->testModifiers(Modifier::Deleted) ||
                                             pSubroutine->testModifiers(Modifier::Defaulted));
                    }
                }
                return true;
            }
            else
            {
                pField = CppLiteGetElementAs(Field);
                if (pField == nullptr)
                    return true;
                pType = pField->getValueType();
            }
        }
        else
        {
            CPPLITEPARSER_ASSERT(m_Data.findElement(input) == nullptr);
            pType = CppLiteVisitType(input->m_TypeOrAuto);
            if (pType == nullptr)
            {
                return true;
            }
        }

        LanguageElement* pElement = nullptr;

        Expression* pInitExpression = nullptr;
        const char* name = nullptr;
        if (auto pFunctionOrVariableEnd = input->m_FunctionOrVariableEnd)
        {
            name = pFunctionOrVariableEnd->m_IDENTIFIER;
            if (pFunctionOrVariableEnd->m_CallList || pFunctionOrVariableEnd->m_AssignExpressionOrInitializerList)
            {
                if (pFunctionOrVariableEnd->m_CallList)
                {
                    if (pFunctionOrVariableEnd->m_TemplateArgumentList)
                    {
                        if (!resolveGenericCall(input, nullptr, name, pFunctionOrVariableEnd->m_TemplateArgumentList,
                                                pFunctionOrVariableEnd->m_CallList))
                            return true;
                    }
                    else
                    {
                        CppLiteErrorReturnIf((modifiers & PHANTOM_R_STATIC) != 0,
                                             "'%s' : static variables are not supported for hot reload requirements",
                                             name);
                        // construction list for field/variable
                        if (pType->hasAuto())
                        {
                            CppLiteErrorReturnIf(
                            pFunctionOrVariableEnd->m_CallList->m_AssignExpressionOrInitializerLists == nullptr ||
                            pFunctionOrVariableEnd->m_CallList->m_AssignExpressionOrInitializerLists->size() != 1,
                            "'%s' : in an auto declaration context, a single expression in the "
                            "construction-list is expected to deduce the underlying type",
                            name);

                            pInitExpression = CppLiteVisitExpression(
                            pFunctionOrVariableEnd->m_CallList->m_AssignExpressionOrInitializerLists->front());
                            if (pInitExpression == nullptr)
                                return true;
                            PlaceholderMap deductions;
                            CppLiteGetSemantic()->callTemplateArgumentDeduction(pType, pInitExpression->getValueType(),
                                                                                deductions);
                            CppLiteErrorReturnIf(deductions.size() != 1,
                                                 "'%s' : auto deduction failed to match the pattern", name);
                            CPPLITEPARSER_ASSERT(deductions.begin()->second->asType(), "deduced type is not a type !?");
                            pType = pType->replicate(static_cast<Type*>(deductions.begin()->second.front()));
                        }
                        else
                        {
                            if (pFunctionOrVariableEnd->m_CallList->m_AssignExpressionOrInitializerLists == nullptr ||
                                pFunctionOrVariableEnd->m_CallList->m_AssignExpressionOrInitializerLists->size() != 1)
                            {
                                // 0 or >1 arguments => construction list
                                Expressions arguments;
                                resolveCallList(pFunctionOrVariableEnd->m_CallList, arguments);
                                m_Data.pushFunctionArguments(&arguments);
                                Expression* pConstructionExpression = CppLiteVisitExpression(input->m_TypeOrAuto);
                                m_Data.popFunctionArguments();
                                CppLiteErrorReturnIf(pConstructionExpression == nullptr,
                                                     "'%s' : no constructor found which takes the given arguments",
                                                     name);
                                pInitExpression = pConstructionExpression;
                            }
                            else
                            {
                                // 1 argument => conversion
                                Expression* pInitExpr = CppLiteVisitExpression(
                                pFunctionOrVariableEnd->m_CallList->m_AssignExpressionOrInitializerLists->front());
                                if (pInitExpr == nullptr)
                                    return true;
                                Expression* pConv = CppLiteConv(pInitExpr, pType, CastKind::Implicit,
                                                                UserDefinedFunctions::All, CppLiteGetScope());
                                if (pConv == nullptr)
                                    return true;
                                pInitExpression = pConv;
                            }
                        }
                    }
                }
                else if (pFunctionOrVariableEnd->m_AssignExpressionOrInitializerList)
                {
                    // initialized field/variable
                    // 1 argument => conversion
                    m_Data.pushInitializationType(pType);
                    Symbol* pSymbol = CppLiteVisitSymbol(pFunctionOrVariableEnd->m_AssignExpressionOrInitializerList);
                    m_Data.popInitializationType();
                    if (pSymbol == nullptr)
                        return true;
                    Expression* pInitExpr = toExpression(pSymbol);
                    CppLiteErrorReturnIf(pInitExpr == nullptr && pSymbol->asField(),
                                         "'%s' : default value : access to non-static field '%s'", name,
                                         pSymbol->getName().data());
                    CppLiteErrorReturnIf(pInitExpr == nullptr,
                                         "'%s' : default value : symbol '%s' does not evaluate to a "
                                         "valid expression in the current scope",
                                         name, pSymbol->getName().data());
                    Expression* pConv =
                    CppLiteConv(pInitExpr, pType, CastKind::Implicit, UserDefinedFunctions::All, CppLiteGetScope());
                    if (pField)
                    {
                        if (pConv == nullptr)
                            return true;
                    }
                    else
                    {
                        if (pConv == nullptr)
                            return true;
                    }
                    pInitExpression = pConv;
                }
            }
            else if (pFunctionOrVariableEnd->m_FunctionEnd)
            {
                return traverse(pFunctionOrVariableEnd->m_FunctionEnd->m_FunctionBlock);
            }
        }

        if (pInitExpression == nullptr && (CppLiteHasFeature("ZeroInit")))
        {
            pInitExpression = CppLiteGetSemantic()->createZeroInitExpression(pType);
        }
        if (pInitExpression)
        {
            if (pBlock) // if block => local variable
            {
                // local variable
                LocalVariable* pLocalVariable = NewInScope<LocalVariable>(pType, name);
                CppLiteSetCodeRange(pLocalVariable, CppLiteCodeRange(input->location()));
                addLocalVariable(pBlock, pLocalVariable);
                Statement* pStatement;
                pBlock->addStatement(pStatement =
                                     NewInScope<LocalVariableInitializationStatement>(pLocalVariable, pInitExpression));
                CppLiteMapAndReturn(pStatement);
            }
            else
            {
                CPPLITEPARSER_ASSERT(pField);
                pField->setDefaultExpression(pInitExpression);
            }
        }
        else if (pField == nullptr)
        {
            CppLiteErrorReturnIf(pType->asReference(), "'%s' : references must be initialized", name);
        }
        return true;
    }
    bool visitIfOrWhileCondition(ast::IfOrWhileCondition* input) { return true; }
    bool endIfOrWhileCondition(ast::IfOrWhileCondition* input) { return true; }

    /// mixin statement;
    bool traverseMixinStatement(ast::MixinStatement* input)
    {
        ast::MixinStatementDefinition* pMixinDef = m_Data.getMixinStatementDefinition(input->m_IDENTIFIER);
        CppLiteErrorReturnIf(pMixinDef == nullptr, "'%s' : unrecognized macro", input->m_IDENTIFIER);
        if (pMixinDef->m_Statements)
            for (auto pStatement : *pMixinDef->m_Statements)
                traverse(pStatement);
        return true;
    }

    /// return value;
    bool traverseReturnStatement(ast::ReturnStatement* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock && pBlock->getSubroutine());
        auto             pSignature = pBlock->getSubroutine()->getSignature();
        Type*            pReturnType = pBlock->getSubroutine()->getReturnType();
        ReturnStatement* pReturnStatement = nullptr;
        if (input->m_pExpression)
        {
            OwnerGuard<Expression> pExpression = CppLiteVisitExpression(input->m_pExpression, pBlock);
            if (pExpression == nullptr)
                return true;
            if (!pExpression->isTemplateDependant() && pReturnType->removeEverything() == Application::Get()->getAuto())
            {
                pReturnType = CppLiteGetSemantic()->autoDeduction(pReturnType, pExpression->getValueType());
                if (!pReturnType)
                    return true;
                pSignature->setReturnType(pReturnType);
            }

            if (pReturnType != PHANTOM_TYPEOF(void))
            {
                OwnerGuard<Expression> pConvExpression = CppLiteConv(pExpression.take(), pReturnType, pBlock);
                if (pConvExpression == nullptr)
                    return true;

                pReturnStatement = New<ReturnStatement>(pConvExpression);
                applyCodeRange(input, pReturnStatement);
                pBlock->addStatement(pReturnStatement);
            }
            else
            {
                CppLiteErrorReturnIf(pExpression->getValueType() != PHANTOM_TYPEOF(void),
                                     "return : void function returning value");
                pReturnStatement = New<ReturnStatement>(pExpression);
                applyCodeRange(input, pReturnStatement);
                pBlock->addStatement(pReturnStatement);
            }
        }
        else
        {
            if (pReturnType == Application::Get()->getAuto())
            {
                pReturnType = PHANTOM_TYPEOF(void);
                pSignature->setReturnType(pReturnType);
            }

            CppLiteErrorReturnIf(pReturnType != PHANTOM_TYPEOF(void), "return : non-void function must return a value");
            pReturnStatement = New<ReturnStatement>();
            applyCodeRange(input, pReturnStatement);
            pBlock->addStatement(pReturnStatement);
        }
        CppLiteMapAndReturn(pReturnStatement);
    }

    /// goto label;
    bool traverseGotoStatement(ast::GotoStatement* input)
    {
        StringView id = input->m_IDENTIFIER;
        CPPLITEPARSER_ASSERT(!id.empty(), "ill-formed label name");
        CppLiteErrorReturnIf(id.find("_PHNTM_") == 0, "'%s' is a reserved identifier an cannot be used", id.data());
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);
        CPPLITEPARSER_ASSERT(pBlock->getSubroutine());
        Label*           pLabel = m_Data.findOrCreateLabel(pBlock->getSubroutine(), id);
        BranchStatement* pBranch = NewInScope<BranchStatement>(pLabel);
        pBlock->addStatement(pBranch);
        CppLiteMapAndReturn(pBranch);
    }

    /// for(...) ...
    bool traverseForStatement(ast::ForStatement* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);

        if (input->m_ForeachSignature)
        {
            // for (container)
            Block* pForStatement = pBlock->newBlock(Block::e_Tag_Foreach);

            // type
            CPPLITEPARSER_ASSERT(input->m_ForeachSignature->m_TypeOrAuto);
            Type* pType = CppLiteVisitType(input->m_ForeachSignature->m_TypeOrAuto);
            CppLiteErrorReturnIf(pType == nullptr, "for : invalid type in for each local variable declaration");

            // ---------------------------------------------
            // container
            Expression* pContainerExp = CppLiteVisitExpression(input->m_ForeachSignature->m_Expression);
            if (pContainerExp == nullptr)
                return true;
            CppLiteGuard(pContainerExp);

            // auto&& c
            auto pContainerLocalVarType = Application::Get()->getAuto()->addRValueReference();

            Expressions emptyArguments;
            if (!pContainerExp->isTemplateDependant())
            {
                pContainerLocalVarType =
                CppLiteGetSemantic()->autoDeduction(pContainerLocalVarType, pContainerExp->getValueType());
                if (!pContainerLocalVarType) // auto deduction failed
                    return true;
            }

            LocalVariable* pTempLocal_Container =
            addLocalVariable(pForStatement, pContainerLocalVarType, "$c",
                             CppLiteGetSemantic()->convert(pContainerExp, pContainerLocalVarType));

            // ---------------------------------------------
            // begin()
            Expression* pContainerExpCloneBegin = pForStatement->New<LocalVariableExpression>(pTempLocal_Container);
            CppLiteGuard(pContainerExpCloneBegin);
            Expression* pBeginCall = phantom::Object::Cast<Expression>(
            CppLiteGetSemantic()->silentQualifiedLookup(pContainerExpCloneBegin, "begin", phantom::NullOpt,
                                                        MakeArrayView(emptyArguments), pForStatement, nullptr, 0));

            CppLiteGuard(pBeginCall);
            CppLiteErrorReturnIf(pBeginCall == nullptr, "no 'begin()' method defined, accessible or callable in '%.*s'",
                                 PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(CppLiteScopeType(pContainerExp))));

            // ---------------------------------------------
            // end()
            Expression* pContainerExpCloneEnd = pForStatement->New<LocalVariableExpression>(pTempLocal_Container);
            CppLiteGuard(pContainerExpCloneEnd);
            Expression* pEndCall = phantom::Object::Cast<Expression>(CppLiteGetSemantic()->silentQualifiedLookup(
            pContainerExpCloneEnd, "end", phantom::NullOpt, MakeArrayView(emptyArguments), pForStatement, nullptr, 0));
            CppLiteGuard(pEndCall);
            CppLiteErrorReturnIf(pEndCall == nullptr, "no 'end()' method defined, accessible or callable in '%.*s'",
                                 PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(CppLiteScopeType(pContainerExp))));

            // ---------------------------------------------
            // auto __begin = begin()

            LocalVariable* pBeginVar = createLocalVariable(
            pForStatement,
            pBeginCall->isTemplateDependant() ? pForStatement->New<TemplateDependantDeclType>(pBeginCall)
                                              : pBeginCall->getValueType()->removeReference()->removeConst(),
            "$begin");

            addLocalVariable(pForStatement, pBeginVar);
            CppLiteSetCodeRange(pBeginVar, m_Data.CppLiteCodeRange(input->m_ForeachSignature));

            pForStatement->addStatement(
            pForStatement->New<LocalVariableInitializationStatement>(pBeginVar, pBeginCall->load(getSource())));

            // ---------------------------------------------
            // auto __end = end()

            LocalVariable* pEndVar = createLocalVariable(pForStatement,
                                                         pEndCall->isTemplateDependant()
                                                         ? NewInScope<TemplateDependantDeclType>(pEndCall)
                                                         : pEndCall->getValueType()->removeReference()->removeConst(),
                                                         "$end");
            addLocalVariable(pForStatement, pEndVar);
            CppLiteSetCodeRange(pEndVar, m_Data.CppLiteCodeRange(input->m_ForeachSignature));

            pForStatement->addStatement(
            pForStatement->New<LocalVariableInitializationStatement>(pEndVar, pEndCall->load(getSource())));

            // ---------------------------------------------
            // for(; __begin != __end; ++__end)

            // ---------------------------------------------
            // *__begin
            Expression* pDeref = CppLiteGetSemantic()->solveUnaryPreOperator(
            "*", pForStatement->New<LocalVariableExpression>(pBeginVar), pForStatement);
            CppLiteGuard(pDeref);
            CppLiteErrorReturnIf(
            pDeref == nullptr, "for (each) : no unary prefix operator '*' defined in '%s'",
            PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(CppLiteScopeType(pBeginVar->getValueType()))));
            CppLiteMapCodeRange(pDeref, input->m_ForeachSignature->m_Expression);

            // __begin != __end
            Expression* pTest = CppLiteGetSemantic()->solveBinaryOperator(
            "!=", NewInScope<LocalVariableExpression>(pBeginVar), pForStatement->New<LocalVariableExpression>(pEndVar),
            pForStatement);
            CppLiteGuard(pTest);
            CppLiteErrorReturnIf(
            pTest == nullptr, "for (each) : no binary operator '!=' defined in '%.*s'",
            PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(CppLiteScopeType(pBeginVar->getValueType()))));
            CppLiteMapCodeRange(pTest, input->m_ForeachSignature->m_Expression);
            if (pTest->getValueType() != PHANTOM_TYPEOF(bool))
            {
                CppLiteErrorReturnIf(
                !pTest->isTemplateDependant(),
                "range-based for each test 'begin() != end()' must resolve to a boolean expression");
                pTest = CppLiteConv(pTest, PHANTOM_TYPEOF(bool), CppLiteGetScopeAs(LanguageElement));
            }

            // ++__begin
            Expression* pUpdate = CppLiteGetSemantic()->solveUnaryPreOperator(
            "++", NewInScope<LocalVariableExpression>(pBeginVar), pForStatement);
            CppLiteGuard(pUpdate);
            CppLiteErrorReturnIf(
            pUpdate == nullptr, "for : no unary prefix operator '++' defined in '%.*s'",
            PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(CppLiteScopeType(pBeginVar->getValueType()))));
            // CppLiteMapCodeRange(pIteratorUpdate, input->m_ForeachSignature->m_Expression);

            // --- internal cooking ---
            BreakLabelGuard    breakLabel(m_Data);
            ContinueLabelGuard continueLabel(m_Data);
            Label*             pBreakLabel = breakLabel.get();
            Label*             pContinueLabel = continueLabel.get();

            Label* pTestLabel = pForStatement->New<Label>();

            // Test
            pForStatement->addStatement(pTestLabel);

            // branch : if not 'test' => break
            BranchIfNotStatement* pBranchIfNotStatement = NewInScope<BranchIfNotStatement>(pTest, pBreakLabel);
            CppLiteMapCodeRange(pBranchIfNotStatement, input->m_ForeachSignature->m_Expression);
            pForStatement->addStatement(pBranchIfNotStatement);

            // ---------------------------------------------
            // {
            {
                // Code
                // Inner block
                Block* pInnerBlock = pForStatement->newBlock();
                CppLiteMapCodeRange(pInnerBlock, input->m_Statement);

                // range-declaration
                // <type> var = *__begin;

                // look for an 'auto' to deduce in "pType"
                Placeholder* pPlaceholder = pType->removeEverything()->asPlaceholder();
                if (pType->hasAuto() && !(pDeref->isTemplateDependant()))
                {
                    pType = CppLiteGetSemantic()->autoDeduction(pType, pDeref->getValueType());
                }

                // Create the local variable which receive each element
                LocalVariable* pElementVar =
                createLocalVariable(pInnerBlock, pType, input->m_ForeachSignature->m_IDENTIFIER);

                addLocalVariable(pInnerBlock, pElementVar);

                Expression* pIteratorContentAccessConv =
                CppLiteInitNE(pDeref, pElementVar->getValueType(), CppLiteGetScope());
                if (!pIteratorContentAccessConv)
                    return true;

                LocalVariableInitializationStatement* pElementInit =
                pInnerBlock->New<LocalVariableInitializationStatement>(pElementVar, pIteratorContentAccessConv);
                CppLiteMapCodeRange(pElementInit, input->m_ForeachSignature->m_TypeOrAuto);
                pInnerBlock->addStatement(pElementInit);

                // Resolve user defined inner statement
                CppLiteVisitElement(input->m_Statement, pInnerBlock);
            }
            // ==============================================================

            // 'continue'
            pForStatement->addStatement(pContinueLabel);

            // updates
            pForStatement->addStatement(pUpdate);

            // branch to 'test' label
            pForStatement->addStatement(pForStatement->New<BranchStatement>(pTestLabel));

            // 'break' goes here
            pForStatement->addStatement(pBreakLabel);

            CppLiteMapAndReturn(pForStatement);
        }
        else
        {
            Block* pForStatement = pBlock->newBlock(Block::e_Tag_For);

            if (input->m_Init)
            {
                CppLiteVisitElement(input->m_Init, pForStatement);
            }

            {
                BreakLabelGuard    breakLabel(m_Data);
                ContinueLabelGuard continueLabel(m_Data);
                Label*             pBreakLabel = breakLabel.get();
                Label*             pContinueLabel = continueLabel.get();

                Label* pCodeStartLabel = pBlock->New<Label>();

                // Condition
                CppLiteErrorReturnIf(input->m_Test == nullptr, "for : invalid condition expression");
                Expression* pCond = CppLiteVisitExpression(input->m_Test, pForStatement);
                CppLiteErrorReturnIf(pCond == nullptr, "for : invalid condition expression");
                Expression* pCondConv = CppLiteConv(pCond, PHANTOM_TYPEOF(bool), pForStatement);
                if (pCondConv == nullptr)
                    return true;

                Expression* pCondClone = pCondConv ? pCondConv->clone(CppLiteGetSource()) : nullptr;

                BranchIfNotStatement* pBranchIfNotStatement = nullptr;
                if (pCond)
                {
                    pBranchIfNotStatement = NewInScope<BranchIfNotStatement>(pCondConv, pBreakLabel);
                }

                BranchIfStatement* pBranchIfStatement = nullptr;
                if (pCondClone)
                    pBranchIfStatement = NewInScope<BranchIfStatement>(pCondClone, pCodeStartLabel);

                // If test exists, branch to break if test fails
                if (pBranchIfNotStatement)
                {
                    pForStatement->addStatement(pBranchIfNotStatement);
                }

                // Code start label
                pForStatement->addStatement(pCodeStartLabel);

                // Code
                Block* pInnerBlock = pForStatement->newBlock();
                CppLiteVisitElement(input->m_Statement, pInnerBlock);

                // Updates

                // 'continue' goes here
                pForStatement->addStatement(pContinueLabel);

                // updates
                if (input->m_Update)
                {
                    Expression* pUpdate = CppLiteVisitExpression(input->m_Update, pForStatement);
                    if (pUpdate == nullptr)
                    {
                        return true;
                    }
                    pForStatement->addStatement(pUpdate);
                }

                // branch to code if test exists and succeed, or direct branch if test doesn't
                // exist
                pForStatement->addStatement(pBranchIfStatement);

                // 'break' goes here
                pForStatement->addStatement(pBreakLabel);
            }

            CppLiteMapAndReturn(pForStatement);
        }
    }

    /// while(exp) ...
    bool traverseWhileStatement(ast::WhileStatement* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);

        BreakLabelGuard    breakLabel(m_Data);
        ContinueLabelGuard continueLabel(m_Data);
        Label*             pBreakLabel = breakLabel.get();
        Label*             pContinueLabel = continueLabel.get();

        pBlock->addStatement(pContinueLabel);

        // while
        {
            Block* pWhileStatement = pBlock->newBlock(Block::e_Tag_While);

            // (condition)

            Expression* pCond = CppLiteVisitExpression(input->m_Condition, pWhileStatement);
            CppLiteErrorReturnIf(pCond == nullptr, "while : invalid condition");
            if (pCond == nullptr)
                return true;
            CppLiteGuard(pCond);

            Expression* pCondConv = CppLiteConvNoCodeRange(pCond, PHANTOM_TYPEOF(bool), pBlock);
            CppLiteGuard(pCondConv);
            if (pCondConv == nullptr)
                return true;

            BranchIfNotStatement* pBranchIfNotStatement = NewInScope<BranchIfNotStatement>(pCondConv, pBreakLabel);

            BranchStatement* pBranchToContinue = NewInScope<BranchStatement>(pContinueLabel);

            // Branch to break if test fails
            pWhileStatement->addStatement(pBranchIfNotStatement);

            // {
            {
                // Code
                Block* pInnerBlock = pWhileStatement->newBlock();

                CppLiteVisitElement(input->m_Statement, pInnerBlock);
            }
            // }

            // branch to code if test succeed
            pWhileStatement->addStatement(pBranchToContinue);

            // 'break' goes here
            pWhileStatement->addStatement(pBreakLabel);

            CppLiteMapAndReturn(pWhileStatement);
        }
    }

    /// { ... }
    bool traverseBlockStatement(ast::BlockStatement* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);
        Block* pNewBlock = pBlock->newBlock();
        if (input->m_Statements)
        {
            for (auto pStatement : *input->m_Statements)
            {
                CppLiteVisitElement(pStatement, pNewBlock);
            }

            // FIXME rebuild the local variable use count
            //             for (LocalVariable* pVar : pNewBlock->getLocalVariables())
            //             {
            //                 if (pVar->getReferencingElements().size() <= 1)
            //                 {
            //                     if (pVar->getReferencingElements().size() == 1)
            //                     {
            //                         Statement* pStatement = pVar->getReferencingElements()[0]->asStatement();
            //                         if (pStatement == nullptr ||
            //                         pStatement->asLocalVariableInitializationStatement()
            //                         == nullptr)
            //                         {
            //                             continue;
            //                         }
            //                     }
            //                     CppLiteWarningMsgCodeRangeLocation(pVar->getCodeRangeLocation(), "unreferenced
            //                     local variable '%s'",
            //                                                        pVar->getName().data());
            //                 }
            //             }
        }
        CppLiteMapAndReturn(pNewBlock);
    }

    /// do { ... } while(exp)
    bool visitDoWhileStatement(ast::DoWhileStatement* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);
        Block* pDoWhileStatement = pBlock->newBlock(Block::e_Tag_Do);

        Expression* pCond = CppLiteVisitExpression(input->m_Expression, pDoWhileStatement);
        CppLiteErrorReturnIf(pCond == nullptr, "do : invalid condition expression");
        Expression* pCondConv = CppLiteConv(pCond, PHANTOM_TYPEOF(bool), pBlock);
        if (pCondConv == nullptr)
            return true;

        {
            BreakLabelGuard    breakLabel(m_Data);
            ContinueLabelGuard continueLabel(m_Data);
            Label*             pBreakLabel = breakLabel.get();
            Label*             pContinueLabel = continueLabel.get();
            Label*             pCodeStartLabel = pDoWhileStatement->New<Label>();

            BranchIfStatement* pBranchIfStatement = pDoWhileStatement->New<BranchIfStatement>(pCond, pCodeStartLabel);

            // Code start label
            pDoWhileStatement->addStatement(pCodeStartLabel);

            // Code
            Block* pInnerBlock = pDoWhileStatement->newBlock();
            CppLiteVisitElement(input->m_BlockStatement, pInnerBlock);

            // 'continue' goes here
            pDoWhileStatement->addStatement(pContinueLabel);

            // branch to code if test succeed
            pDoWhileStatement->addStatement(pBranchIfStatement);

            // 'break' goes here
            pDoWhileStatement->addStatement(pBreakLabel);
        }

        CppLiteMapAndReturn(pDoWhileStatement);
    }

    /// if(...) ... else ...
    bool traverseIfStatement(ast::IfStatement* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);
        Block*      pIfStatement = pBlock->newBlock(Block::e_Tag_If);
        Expression* pCond = CppLiteVisitExpression(input->m_Condition, pIfStatement);
        if (pCond == nullptr)
            return true;
        CppLiteGuard(pCond);

        Expression* pConvCond = CppLiteConvNoCodeRange(pCond, PHANTOM_TYPEOF(bool), pBlock);
        CppLiteGuard(pConvCond);
        if (pConvCond == nullptr)
            return true;
        if (!m_Data.m_pParser->hasFlag(CppLiteParser::e_Flag_NoCodeLocation))
            CppLiteSetCodeRange(pConvCond, pCond->getCodeRange());
        Label*                pElseLabel = pIfStatement->New<Label>(Label::e_Tag_Else);
        Label*                pEndLabel = nullptr;
        BranchIfNotStatement* pBranchIfNot = pIfStatement->New<BranchIfNotStatement>(pConvCond, pElseLabel);
        CppLiteMapCodeRange(pBranchIfNot, input->m_Condition);
        pIfStatement->addStatement(pBranchIfNot);
        Block* pThenBlock = pIfStatement->newBlock();
        Block* pElseBlock;
        CppLiteVisitElement(input->m_Then, pThenBlock);
        CppLiteMapCodeRange(pThenBlock, input->m_Then);
        if (input->m_Else)
        {
            // at the end of 'then' part, if 'else' exists, jump to end
            pEndLabel = pIfStatement->New<Label>(Label::e_Tag_Endif);
            BranchStatement* pBranchToEnd = pIfStatement->New<BranchStatement>(pEndLabel);
            pIfStatement->addStatement(pBranchToEnd);
        }
        pIfStatement->addStatement(pElseLabel);
        if (pEndLabel) // means we have else
        {
            pElseBlock = pIfStatement->newBlock();
            CppLiteVisitElement(input->m_Else, pElseBlock);
            CppLiteMapCodeRange(pElseBlock, input->m_Else);
            pIfStatement->addStatement(pEndLabel);
        }
        CppLiteMapAndReturn(pIfStatement);
    }

    /// if(int var = X)
    bool traverseIfOrWhileLocalVariableDecl(ast::IfOrWhileLocalVariableDecl* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);

        Type* pType = CppLiteVisitType(input->m_TypeOrAuto);
        if (pType == nullptr)
            return true;

        m_Data.pushInitializationType(pType);

        Expression* pInitExpression = nullptr;
        if (pType->hasAuto())
        {
            ast::AutoDeclarator decl;
            decl.__location = input->__location;
            decl.m_IDENTIFIER = input->m_IDENTIFIER;
            decl.m_CallList = nullptr;
            decl.m_AssignExpressionOrInitializerList = input->m_Expression;
            decl.m_TypeExtents = nullptr;
            decl.m_ArrayExtents = nullptr;
            traverseAutoDeclarator(&decl);
        }
        else
        {
            ast::Declarator decl;
            decl.__location = input->__location;
            decl.m_IDENTIFIER = input->m_IDENTIFIER;
            decl.m_CallList = nullptr;
            decl.m_AssignExpressionOrInitializerList = input->m_Expression;
            decl.m_TypeExtents = nullptr;
            decl.m_ArrayExtents = nullptr;
            traverseDeclarator(&decl);
        }
        m_Data.popInitializationType();

        if (LocalVariable* pLV = pBlock->getLocalVariable(input->m_IDENTIFIER))
            CppLiteMapAndReturn(NewInScope<LocalVariableExpression>(pLV));
        return true;
    }

    /// int var = X
    bool traverseExplicitLocalVariableDecl(ast::ExplicitLocalVariableDecl* input)
    {
        Type* pType = CppLiteVisitType(input->m_BasicType);
        if (pType == nullptr)
            return true;

        if (!applyQualifiers(input, pType, input->m_CONSTs))
            return true;

        m_Data.pushInitializationType(pType);
        for (auto decl : *input->m_Declarators)
        {
            CppLiteVisitElement(decl);
        }
        m_Data.popInitializationType();

        return true;
    }

    /// auto var = X
    bool traverseAutoLocalVariableDecl(ast::AutoLocalVariableDecl* input)
    {
        Type* pType = Application::Get()->getAuto();

        if (!applyQualifiers(input, pType, input->m_CONSTs))
            return true;

        m_Data.pushInitializationType(pType);
        for (auto decl : *input->m_AutoDeclarators)
        {
            CppLiteVisitElement(decl);
        }
        m_Data.popInitializationType();

        return true;
    }

    /// expr;
    bool traverseExpressionStatement(ast::ExpressionStatement* input)
    {
        Expression* pExp = CppLiteVisitExpression(input->m_Expression);
        CppLiteGuard(pExp);
        if (pExp == nullptr) // error must be reported from above call
            return true;
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);
        CppLiteMapAndReturn(pBlock->addStatement(pExp));
    }

    /// label:
    bool traverseLabelStatement(ast::LabelStatement* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);
        CPPLITEPARSER_ASSERT(pBlock);
        Label* pLabel = pBlock->New<Label>(input->m_IDENTIFIER);
        pBlock->addStatement(pLabel);
        CppLiteMapAndReturn(pLabel);
    }
    bool endLabelStatement(ast::LabelStatement* input) { return true; }

    /// switch(...) { ... }
    bool traverseSwitchStatement(ast::SwitchStatement* input)
    {
        Block* pBlock = CppLiteGetScopeAs(Block);

        CPPLITEPARSER_ASSERT(pBlock);

        Block* pSwitchStatement = pBlock->newBlock(Block::e_Tag_Switch);

        Expression* pTestValue = CppLiteVisitExpression(input->m_Expression, pSwitchStatement);
        CppLiteErrorReturnIf(pTestValue == nullptr, "switch : invalid test expression");
        CppLiteErrorReturnIf(pTestValue->getValueType()->removeReference()->removeConst()->asIntegralType() == nullptr,
                             "switch : value type '%s' is illegal, must be an integral expression",
                             pTestValue->getValueType()->getQualifiedDecoratedName().data());

        Block* pInnerBlock = pSwitchStatement;

        // unlike c++ we build a block in between each case label
        Block* pCaseBLock = nullptr;

        {
            BreakLabelGuard breakLabel(m_Data);
            Label*          pBreakLabel = breakLabel.get();

            Statements       branchIfs;
            BranchStatement* pDefaultBranch = nullptr;
            if (input->m_StatementOrCases)
            {
                for (auto pStatement : *input->m_StatementOrCases)
                {
                    if (pStatement->type() == ast::e_RuleType_CaseStatement)
                    {
                        Label*           pCaseLabel = pInnerBlock->New<Label>("case");
                        auto             pCaseStatement = static_cast<ast::CaseStatement*>(pStatement);
                        ast::Expression* pExpr = pCaseStatement->m_Expression;

                        Expression* pCaseExpr = CppLiteVisitExpression(pExpr, pInnerBlock);

                        CppLiteErrorReturnIf(
                        pCaseExpr == nullptr,
                        "invalid 'case' value expression, must be an integral "
                        "constant expression instead of '%.*s'",
                        PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pTestValue->getValueType())));

                        pCaseExpr->setCodeRange(pTestValue->getCodeRange());

                        if (!pCaseExpr->isTemplateDependant())
                        {
                            ConstantExpression* pCaseExprAsCst = phantom::Object::Cast<ConstantExpression>(pCaseExpr);
                            CppLiteErrorReturnIf(
                            pCaseExprAsCst == nullptr,
                            "invalid 'case' value expression, must be an integral "
                            "constant expression instead of '%.*s'",
                            PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pTestValue->getValueType())));
                        }

                        Expression* pTest = CppLiteGetSemantic()->solveBinaryOperator("==", pTestValue, pCaseExpr, 0);

                        pTest->setCodeRange(pTestValue->getCodeRange());

                        CppLiteErrorReturnIf(
                        pTest == nullptr,
                        "invalid 'case' value expression, must be an integral "
                        "constant expression instead of '%.*s'",
                        PHANTOM_STRING_AS_PRINTF_ARG(CppLiteElementToString(pTestValue->getValueType())));

                        BranchIfStatement* pBranch = pInnerBlock->New<BranchIfStatement>(pTest, pCaseLabel);
                        pInnerBlock->addStatement(pCaseLabel);
                        branchIfs.push_back(pBranch);
                        pCaseBLock = pInnerBlock->newBlock();
                    }
                    else if (pStatement->type() == ast::e_RuleType_DefaultStatement)
                    {
                        if (pDefaultBranch)
                        {
                            CppLiteErrorMsg("default : already defined");
                            return true;
                        }
                        else
                        {
                            auto pDef = pInnerBlock->New<Label>("default");
                            pDefaultBranch = pInnerBlock->New<BranchStatement>(pDef);
                            pInnerBlock->addStatement(pDef);
                            pCaseBLock = pInnerBlock->newBlock();
                        }
                    }
                    else
                    {
                        CppLiteErrorReturnIf(pCaseBLock == nullptr, "switch : illegal code before case labels");
                        CppLiteVisitElement(pStatement, pCaseBLock);
                    }
                }
            }
            if (pDefaultBranch)
                branchIfs.push_back(pDefaultBranch);
            pInnerBlock->prependStatements(branchIfs);
            pInnerBlock->addStatement(pBreakLabel);
        }
        CppLiteMapAndReturn(pSwitchStatement);
    }
}; // namespace lang

bool CppLitePassMembersGlobal::traverseConstructor(ast::Constructor* input)
{
    return true;
}

bool CppLitePassMembersLocal::traverseConstructor(ast::Constructor* input)
{
    ClassType* pClassType = CppLiteGetScopeAs(ClassType);

    TemplateSignature* pTSign = CppLiteGetTemplateSignature();

    TemplateSpecialization* pTSpec{};
    if (pTSign)
    {
        if (!resolveTemplateSpecialization(input, nullptr, pTSpec, true))
            return true;
    }

    CPPLITEPARSER_ASSERT(pClassType);
    CppLiteErrorReturnIf(pClassType->getName() != input->m_IDENTIFIER,
                         "'%s' : constructor name does not match class type name '%s'", input->m_IDENTIFIER,
                         pClassType->getName().data());

    if (pTSpec)
        m_Data.pushScope(pTSpec);

    auto onScopeExit = makeScopeExit([&]() {
        if (pTSpec)
            m_Data.popScope();
    });

    // signature
    Parameters params;
    if (!resolveParameters(input, input->m_Parameters, params))
        return false;

    Signature* pSignature = New<Signature>(PHANTOM_TYPEOF(void), params);

    Modifiers mods = 0;
    if (input->m_Suffix.hasValue())
    {
        CppLiteErrorReturnIf(
        pSignature->getParameterCount() > 1 ||
        (pSignature->getParameterCount() == 1 && !(pSignature->getParameterType(0)->isTemplateDependant()) &&
         !(pSignature->getParameterType(0)->isSame(pClassType->addConst()->addLValueReference())) &&
         !(pSignature->getParameterType(0)->isSame(pClassType->addRValueReference()))),
        "'%s' : expecting a default, copy or move constructor", input->m_IDENTIFIER);

        if (strcmp(input->m_Suffix.as_string(), "delete") == 0)
        {
            mods |= Modifier::Deleted;
        }
        else if (strcmp(input->m_Suffix.as_string(), "default") == 0)
        {
            mods |= Modifier::Defaulted;
        }
        else
        {
            CPPLITEPARSER_ASSERT(false);
        }
    }
    Constructor* pConstructor = NewInScope<Constructor>(pClassType->getName(), pSignature, mods, PHANTOM_R_FLAG_NONE);
    pConstructor->setAccess(CppLiteGetAccess());
    if (pTSpec)
        pTSpec->setTemplated(pConstructor);

    CppLiteErrorReturnIf(input->m_IDENTIFIER != pClassType->getName(),
                         "'%s' : symbol which looks like a constructor has an invalid constructor name",
                         input->m_IDENTIFIER);

    if (auto pSpec = pConstructor->getTemplateSpecialization())
    {
        if (!pConstructor->testModifiers(Modifier::Deleted | Modifier::Defaulted))
        {
            if (input->m_FunctionBlock)
            {
                pConstructor->createThis(pClassType);
                addBlock(pConstructor);
            }
            CppLitePassData* data = &m_Data;
            pConstructor->setBlockBuilder([=](Block* a_pBlock) {
                CppLitePassBlocks blocks(*data);
                return blocks.traverse(input);
            });
        }
    }
    else
    {
        CppLiteErrorReturnIf(pClassType->getConstructor(pConstructor->getParameterTypes()),
                             "'%s' : a constructor already exists with this signature in the current scope",
                             input->m_IDENTIFIER);
        pClassType->addConstructor(pConstructor);
        if (input->m_FunctionBlock)
            addBlock(pConstructor);
    }

    CppLiteMapAndReturn(pConstructor);
}

bool CppLitePassMembersGlobal::traverseTemplateFunction(ast::TemplateFunction* input)
{
    if (Subroutine* pSubroutine = CppLiteGetElementAs(Subroutine))
    {
        if (input->m_FunctionEnd->m_FunctionBlock)
        {
            CppLitePassData* data = &m_Data;
            addBlock(pSubroutine);
            pSubroutine->setBlockBuilder([=](Block* a_pBlock) {
                CppLitePassBlocks blocks(*data);
                return blocks.traverse(input);
            });
        }
    }
    return true;
}

#if DUMP_CODE_RANGES
static void dumpCodeRanges(LanguageElement* a_pLanguageElement, size_t level = 0)
{
    for (size_t i = 0; i < level; ++i)
        phantom::print("  ");
    if (Symbol* pSymbol = a_pLanguageElement->asSymbol())
    {
        phantom::print(pSymbol->getName());
    }
    else
    {
        phantom::print(phantom::Rtti::Find(a_pLanguageElement).getObjectClass()->getName());
    }
    phantom::print(' ');
    phantom::print(a_pLanguageElement->getCodeRange().begin.line);
    phantom::print('|');
    phantom::print(a_pLanguageElement->getCodeRange().begin.column);
    phantom::print("->");
    phantom::print(a_pLanguageElement->getCodeRange().end.line);
    phantom::print('|');
    phantom::print(a_pLanguageElement->getCodeRange().end.column);
    phantom::print('\n');
    for (auto pElem : a_pLanguageElement->getElements())
    {
        dumpCodeRanges(pElem, level + 1);
    }
}
#endif

void CppLiteParser::begin(BuildSession& a_BuildSession)
{
    if (m_pPassData)
        delete_<CppLitePassData>(m_pPassData);
    auto*                     pAlloc = getSource()->getAllocator();
    CppLiteGrammar::Allocator alloc;
    alloc.allocate = [pAlloc](size_t _size) { return pAlloc->allocFunc(_size, 8); };
    alloc.deallocate = [pAlloc](void* _ptr, size_t) { pAlloc->deallocFunc(_ptr); };
    m_pPassData = phantom::new_<CppLitePassData>(this, &a_BuildSession, alloc);
}

int CppLiteParser::parse(uint pass)
{
    switch (pass)
    {
    case e_Pass_Parsing:
    {
        phantom::SmallString<char, 1024> filebuffer;
        std::string                      filebuffer2;

        std::istream* pInput = getSource()->getSourceStream()->createInputStream();
        if (pInput->fail())
        {
            getSource()->getSourceStream()->destroyInputStream(pInput);
            getMessage()->error("failed to open source file");
            return 1;
        }
        filebuffer.assign(std::istreambuf_iterator<char>(*pInput), std::istreambuf_iterator<char>());
        m_pPassData->m_pAstSource = m_pPassData->m_CppLiteGrammar.parseSource(
        filebuffer.data(), filebuffer.size(),
        CppLiteGrammar::ErrorDelegate(m_pPassData, &CppLitePassData::errorDelegate));
        getSource()->getSourceStream()->destroyInputStream(pInput);
        if (m_pPassData->m_pAstSource == nullptr ||
            m_pPassData->m_pParser->getMessage()->getMostValuableMessageType() == MessageType::Error)
            return 1;
    }
    break;
    case e_Pass_Modules:
    {
        if (!hasFlag(e_Flag_NoCodeLocation))
            getSource()->setCodeRange(m_pPassData->CppLiteCodeRange(m_pPassData->m_pAstSource->location()));
        if (m_pPassData->m_pAstSource)
        {
            CppLitePassModules modules(*m_pPassData);

            // automatically
            modules.traverseSource(m_pPassData->m_pAstSource);
        }
    }
    break;
    case e_Pass_Imports:
    {
        if (!hasFlag(e_Flag_NoCodeLocation))
            getSource()->setCodeRange(m_pPassData->CppLiteCodeRange(m_pPassData->m_pAstSource->location()));
        if (m_pPassData->m_pAstSource)
        {
            CppLitePassImport imports(*m_pPassData);

            // automatically
            imports.traverseSource(m_pPassData->m_pAstSource);
        }
    }
    break;
    case e_Pass_Globals:
    {
        if (m_pPassData->m_pAstSource)
        {
            CppLitePassGlobals visitor(*m_pPassData);
            visitor.traverseSource(m_pPassData->m_pAstSource);
        }
    }
    break;
    case e_Pass_Inheritance:
    {
        if (m_pPassData->m_pAstSource)
        {
            CppLitePassInheritance visitor(*m_pPassData);
            visitor.traverseSource(m_pPassData->m_pAstSource);
        }
    }
    break;
    case e_Pass_MemberLocal:
    {
        m_pPassData->m_pSemantic->buildScopeClasses(m_pPassData->getSource(), Semantic::e_ClassBuildState_Inheritance);
        if (m_pPassData->m_pAstSource)
        {
            CppLitePassMembersLocal visitor(*m_pPassData);
            visitor.traverseSource(m_pPassData->m_pAstSource);
        }
    }
    break;
    case e_Pass_MemberGlobal:
    {
        if (m_pPassData->m_pAstSource)
        {
            m_pPassData->m_pSemantic->buildScopeClasses(m_pPassData->getSource(), Semantic::e_ClassBuildState_Members);
            CppLitePassMembersGlobal global(*m_pPassData);
            global.traverseSource(m_pPassData->m_pAstSource);
            CppLitePassStates states(*m_pPassData);
            states.traverseSource(m_pPassData->m_pAstSource);
        }
    }
    break;
    case e_Pass_Sizing:
    {
        if (m_pPassData->m_pAstSource)
        {
            m_pPassData->m_pSemantic->buildScopeClasses(m_pPassData->getSource(), Semantic::e_ClassBuildState_Sized);
        }
    }
    break;
    case e_Pass_FieldDefaults:
    {
        if (m_pPassData->m_pAstSource)
        {
            CppLitePassFieldDefaults visitor(*m_pPassData);
            visitor.traverseSource(m_pPassData->m_pAstSource);
            m_pPassData->m_pSemantic->buildScopeClasses(m_pPassData->getSource(),
                                                        Semantic::e_ClassBuildState_ImplicitsAndDefaults);
        }
    }
    break;
    case e_Pass_Implicits:
    {
        if (m_pPassData->m_pAstSource)
        {
        }
    }
    break;
    case e_Pass_Blocks:
    {
        if (m_pPassData->m_pAstSource)
        {
            CppLitePassBlocks members(*m_pPassData);
            members.traverseSource(m_pPassData->m_pAstSource);

            // only perform block code generation if no error
            if (getMessage()->getMostValuableMessageType() != MessageType::Error)
                m_pPassData->m_pSemantic->buildScopeClasses(m_pPassData->getSource(),
                                                            Semantic::e_ClassBuildState_Blocks);
        }

#if DUMP_CODE_RANGES
        dumpCodeRanges(m_pPassData->m_pSource);
#endif
    }
    break;
    }
    return getMessage()->getMostValuableMessageType() == MessageType::Error;
}

void CppLiteParser::end()
{
    m_Allocator.reset();
}
//
// void CppLiteParser::addDependency(Source* a_pSource)
// {
//     if (getSemantic()->isStandardLookup())
//         return; // skip for expression parsing
//     Compiler::Get()->getCompiledSource(getSource())->currentBuild().addDependency(a_pSource);
// }

Expression* CppLiteParser::parseExpression(StringView a_strName, LanguageElement* a_pScope)
{
    ForwardHeapSequence       heap{4096};
    CppLiteGrammar::Allocator alloc;
    alloc.allocate = [&heap](size_t _size) { return heap.allocate(_size, 8); };
    alloc.deallocate = [&heap](void*, size_t) {};
    BuildSession    session;
    CppLitePassData passData(this, &session, alloc);
    passData.m_pAstExpression = passData.m_CppLiteGrammar.parseExpression(
    a_strName.begin(), a_strName.size(), [](spell::Location, ast::EError, const char*) {});
    if (passData.m_pAstExpression)
    {
        CppLitePassBlocks pass(passData);
        Expression*       pExpression = pass.resolveExpression(passData.m_pAstExpression, a_pScope);
        if (pExpression)
        {
#if CPPLIGHT_ENABLED_DEBUG_LEAK_CHECKS
            CPPLITEPARSER_ASSERT(passData.m_DebugCheckLeaks.size() == 1 &&
                                 passData.m_DebugCheckLeaks.back() == pExpression);
#endif
            passData.m_DebugCheckLeaks.clear();
            passData.m_RuleToElement.clear();
            return pExpression;
        }
    }
    return nullptr;
}

CppLiteParser::~CppLiteParser()
{
    if (m_pPassData)
        delete_<CppLitePassData>(m_pPassData);
}

} // namespace lang
} // namespace phantom
