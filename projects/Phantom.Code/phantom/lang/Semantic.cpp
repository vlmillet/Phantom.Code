// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Semantic.h"

#include "ArrayExpression.h"
#include "BaseConstructorCallStatement.h"
#include "BinaryOperationExpression.h"
#include "Block.h"
#include "BranchIfNotStatement.h"
#include "BranchIfStatement.h"
#include "BuildSource.h"
#include "BuildSystem.h"
#include "BuiltInConversionExpression.h"
#include "BuiltInOperator.h"
#include "BuiltInOperatorExpression.h"
#include "CallExpression.h"
#include "ConditionalExpression.h"
#include "ConstantExpression.h"
#include "ConstructorCallExpression.h"
#include "CppLite.h"
#include "DeleteExpression.h"
#include "ExpressionStatement.h"
#include "FieldInitializationStatement.h"
#include "IdentityExpression.h"
#include "InitializerListExpression.h"
#include "Interpreter.h"
#include "Label.h"
#include "LoadExpression.h"
#include "LocalVariableExpression.h"
#include "MemCopyStatement.h"
#include "Message.h"
#include "MethodPointerCallExpression.h"
#include "OwnerGuard.h"
#include "PointerAdjustmentExpression.h"
#include "PropertyExpression.h"
#include "RValueReferenceExpression.h"
#include "SemanticPrivate.h"
#include "TemplateDependantExpression.h"
#include "TemplateParameterPackExpansion.h"
#include "TemplateParameterPackTypeExpansion.h"
#include "UnaryPostOperationExpression.h"
#include "UnaryPreOperationExpression.h"
#include "phantom/lang/Module.h"
#include "phantom/lang/Package.h"
#include "phantom/lang/TemplateDependantElement.h"
#include "phantom/thread/SpinMutex.h"

#include <fstream>
#include <phantom/lang/Alias.h>
#include <phantom/lang/Application.h>
#include <phantom/lang/Constant.h>
#include <phantom/lang/Constructor.h>
#include <phantom/lang/Field.h>
#include <phantom/lang/Function.h>
#include <phantom/lang/FunctionType.h>
#include <phantom/lang/LValueReference.h>
#include <phantom/lang/LocalVariable.h>
#include <phantom/lang/MethodPointer.h>
#include <phantom/lang/Namespace.h>
#include <phantom/lang/Parameter.h>
#include <phantom/lang/Placeholder.h>
#include <phantom/lang/PlaceholderType.h>
#include <phantom/lang/Pointer.h>
#include <phantom/lang/PrimitiveType.h>
#include <phantom/lang/Property.h>
#include <phantom/lang/RValueReference.h>
#include <phantom/lang/Source.h>
#include <phantom/lang/Template.h>
#include <phantom/lang/TemplateDependantTemplateInstance.h>
#include <phantom/lang/TemplateParameter.h>
#include <phantom/lang/TemplateSignature.h>
#include <phantom/utils/StringUtil.h>
#include <stdio.h>
/* *********************************************** */
#pragma warning(disable : 4800)
#pragma warning(disable : 4244)

namespace phantom
{
namespace lang
{
SpinMutex                                         m_CachedConversionsMtx;
std::unordered_map<ConversionParams, Conversion*> m_CachedConversions;

Semantic::ERefType Semantic::baseConversion(Type* a_pInput, Type*& pOutput, CastKind iConversionType)
{
    /// X => X const
    pOutput = pOutput->removeQualifiers();
    Type* pOutputConstRef = pOutput->asConstLValueReference();
    if (pOutputConstRef)
    {
        /// X -> const X& => temporary object (only for implicit conversions)
        if (iConversionType != CastKind::Implicit)
            return e_RefType_None;
        pOutput = pOutputConstRef->removeConstReference();
        return e_RefType_LValue;
    }
    return e_RefType_None;
}

Conversion* Semantic::baseStandardConversion(Type* a_pInput, Type*& pOutput, CastKind iConversionType,
                                             ERefType& a_RefType)
{
    a_RefType = baseConversion(a_pInput, pOutput, iConversionType);
    if (a_pInput->isSame(pOutput))
    {
        if (iConversionType == CastKind::Const)
        {
            return nullptr; /// const_cast is forbidden for identity conversion
        }
        return (a_RefType != e_RefType_None)
        ? newConv<DefaultConversionSequence>(newConv<RefConversion>(pOutput, a_RefType == e_RefType_RValue))
        : newConv<DefaultConversionSequence>(a_pInput);
    }
    return nullptr;
}

String formatIndexString(String a_Input, size_t a_CharCount = 10)
{
    long long ConversionVal = 0;
    if (sscanf(a_Input.data(), "%lld", &ConversionVal))
    {
        while (a_Input.size() < a_CharCount)
        {
            a_Input = ' ' + a_Input;
        }
    }
    return a_Input;
}

Semantic::Semantic(Source* a_pSource, Message* a_pMessage) : m_pSource(a_pSource), m_pMessage(a_pMessage)
{
    PHANTOM_SEMANTIC_ASSERT(m_pSource);
    PHANTOM_SEMANTIC_ASSERT(m_pMessage);
    PHANTOM_SEMANTIC_ASSERT(m_pMessage->getChildren().size() == 0);
    pushCodeRangeLocation(CodeRangeLocation());
}

Semantic::~Semantic(void)
{
    popCodeRangeLocation();
}

BuiltInOperator* Semantic::getBuiltInBinaryOperator(StringView a_strName) const
{
    auto op = CppLite::Get()->getBuiltInBinaryOperatorId(a_strName);
    return op == Operator::Unknown ? nullptr : CppLite::Get()->_builtInOperator(op);
}

BuiltInOperator* Semantic::getBuiltInPreUnaryOperator(StringView a_strName) const
{
    auto op = CppLite::Get()->getBuiltInPreUnaryOperatorId(a_strName);
    return op == Operator::Unknown ? nullptr : CppLite::Get()->_builtInOperator(op);
}

BuiltInOperator* Semantic::getBuiltInPostUnaryOperator(StringView a_strName) const
{
    auto op = CppLite::Get()->getBuiltInPostUnaryOperatorId(a_strName);
    return op == Operator::Unknown ? nullptr : CppLite::Get()->_builtInOperator(op);
}

static const char* g_SemanticDefaultErrorText[] = {"OK",
                                                   "No Conversion",
                                                   "No Overload",
                                                   "No Member",
                                                   "No Builtin Operator",
                                                   "No Binary Operator",
                                                   "No Unary Prefix Operator",
                                                   "No Unary Postfix Operator",
                                                   "No Constructor",
                                                   "No Default Constructor",
                                                   "No Template Declaration",
                                                   "No Template Specialization",
                                                   "Expected Template Signature",
                                                   "References Cannot Be Allocated",
                                                   "References Must Be Initialized",
                                                   "Unknown Builtin Operator",
                                                   "Ambiguous Call To Multiple Builtin Operator",
                                                   "Ambiguous Access To Multiple Member",
                                                   "Ambiguous Access To Multiple Constructor",
                                                   "Ambiguous Call To Multiple Overloads",
                                                   "Template Dependant Element Is Not An Expression",
                                                   "Inaccessible Symbol",
                                                   "Unexpected Member",
                                                   "Unexpected Template Argument List",
                                                   "Too Many Template Arguments",
                                                   "Not Enough Template Arguments",
                                                   "Template Instantiation Failure",
                                                   "Template Resolution Failure",
                                                   "Instantiation Of Undefined Template Specialization",
                                                   "Expected Expression",
                                                   "State Machine Already Defined In Base",
                                                   "Qualified Lookup Failure",
                                                   "Unqualified Lookup Failure",
                                                   "Qualified Symbols Lookup Failure",
                                                   "Unqualified Symbols Lookup Failure",
                                                   "Too Many Function Arguments",
                                                   "Not Enough Function Arguments",
                                                   "Missing Member Function Caller",
                                                   "Expected Member Pointer Type Expression",
                                                   "Function Conflicts With Previously Declared",
                                                   "Symbol Already Exists With Name",
                                                   "Constructor Already Defined",
                                                   "Type Cannot Have Qualifiers",
                                                   "Type Cannot Have Pointer",
                                                   "Type Cannot HaveL Value Reference",
                                                   "Template Subroutine Lookup Failure",
                                                   "Expected Type",
                                                   "Expected Class Type",
                                                   "Expected Class",
                                                   "Expected Constant",
                                                   "Incomplete Type",
                                                   "Illegal Auto Array",
                                                   "Empty Initializer List",
                                                   "Illegal Explicit Array Conversion",
                                                   "Illegal Inherited Constructor In Aggregate Initialization",
                                                   "Illegal Explicit Constructor In Aggregate Initialization",
                                                   "Illegal Non Implicit Constructor In Aggregate Initialization",
                                                   "Illegal Non Public Field In Aggregate Initialization",
                                                   "Illegal Non Public Base Class In Aggregate Initialization",
                                                   "Illegal Virtual Function In Aggregate Initialization",
                                                   "Cannot Instantiate Abstract Class",
                                                   "Static Assert Failed"};

const char* Semantic::DefaultErrorText(EError a_eError)
{
    return g_SemanticDefaultErrorText[a_eError];
}

ConstantExpression* Semantic::createNullptrExpression()
{
    return New<ConstantExpression>(Application::Get()->getNullptr());
}

#define CreateZeroInitExpression_CaseFundamental(T, K)                                                                 \
    case TypeKind::K:                                                                                                  \
    {                                                                                                                  \
        T v(0);                                                                                                        \
        return New<ConstantExpression>(static_cast<PrimitiveType*>(PHANTOM_TYPEOF(T))->createConstant(&v));            \
    }

Expression* Semantic::createZeroInitExpression(Type* pType)
{
    Type* pTypeNoQual = pType->removeQualifiers();
    switch (pTypeNoQual->getTypeKind())
    {
    case TypeKind::Enum:
    {
        Enum*   pEnum = static_cast<Enum*>(pTypeNoQual);
        int64_t zero = 0;
        return New<ConstantExpression>(pEnum->getUnderlyingIntType()->createConstant(&zero, "", pEnum));
    }
    case TypeKind::Array:
    {
        Array*      pArray = static_cast<Array*>(pTypeNoQual);
        Expressions exps;
        for (size_t i = 0; i < pArray->getItemCount(); ++i)
        {
            exps.push_back(createZeroInitExpression(pArray->getItemType()));
        }
        return New<ArrayExpression>(pArray, exps);
    }
    case TypeKind::Pointer:
    {
        return convert(New<ConstantExpression>(Application::Get()->getNullptr()), pType);
    }
        CreateZeroInitExpression_CaseFundamental(bool, Bool);
        CreateZeroInitExpression_CaseFundamental(char, Char);
        CreateZeroInitExpression_CaseFundamental(int8, Int8);
        CreateZeroInitExpression_CaseFundamental(uint8, UInt8);
        CreateZeroInitExpression_CaseFundamental(int16, Int16);
        CreateZeroInitExpression_CaseFundamental(uint16, UInt16);
        CreateZeroInitExpression_CaseFundamental(int32, Int32);
        CreateZeroInitExpression_CaseFundamental(uint32, UInt32);
        CreateZeroInitExpression_CaseFundamental(int64, Int64);
        CreateZeroInitExpression_CaseFundamental(uint64, UInt64);
        CreateZeroInitExpression_CaseFundamental(float, Float);
        CreateZeroInitExpression_CaseFundamental(double, Double);
        PHANTOM_IF_CHAR16_T(CreateZeroInitExpression_CaseFundamental(char16_t, Char16));
        PHANTOM_IF_CHAR32_T(CreateZeroInitExpression_CaseFundamental(char32_t, Char32));
        PHANTOM_IF_WCHAR_T(CreateZeroInitExpression_CaseFundamental(wchar_t, WChar));
    default:
        return nullptr;
    }
}

LocalVariable* Semantic::createLocalVariable(Type* a_pValueType, StringView a_Name,
                                             Modifiers a_Modifiers /*= PHANTOM_R_NONE*/,
                                             Modifiers a_Flags /*= PHANTOM_R_NONE*/)
{
    if (!a_pValueType->isNative())
    {
        if (ClassType* pClassType = a_pValueType->removeQualifiers()->asClassType())
        {
            buildClass(pClassType, Semantic::e_ClassBuildState_Blocks);
        }
    }
    return New<LocalVariable>(a_pValueType, a_Name, a_Modifiers, a_Flags);
}

DeleteExpression* Semantic::createDeleteExpression(Class* a_pClass, Expression* a_pDeletedExpression,
                                                   LanguageElement* in_pContextScope)
{
    PHANTOM_SEMANTIC_ASSERT(a_pClass);
    Expression* pConv = CxxSemanticConversion(a_pDeletedExpression, a_pClass->addPointer());
    if (pConv == nullptr)
        return nullptr;
    return New<DeleteExpression>(a_pClass, pConv);
}

DeleteExpression* Semantic::createDeleteExpression(Expression* a_pDeletedExpression, bool a_bRTTIBasedDelete,
                                                   LanguageElement* a_pScope)
{
    return a_pScope->New<DeleteExpression>(a_pDeletedExpression, a_bRTTIBasedDelete);
}

Expression* Semantic::solveBinaryOperator(Operator a_eOp, Expression* a_pLeft, Expression* a_pRight,
                                          LanguageElement* a_pScope)
{
    return solveBinaryOperator(BuiltInOperator::OperatorName(a_eOp), a_pLeft, a_pRight, a_pScope);
}
Expression* Semantic::solveUnaryPreOperator(Operator a_eOp, Expression* a_pExpression, LanguageElement* a_pScope)
{
    return solveUnaryPreOperator(BuiltInOperator::OperatorName(a_eOp), a_pExpression, a_pScope);
}
Expression* Semantic::solveUnaryPostOperator(Operator a_eOp, Expression* a_pExpression, LanguageElement* a_pScope)
{
    return solveUnaryPostOperator(BuiltInOperator::OperatorName(a_eOp), a_pExpression, a_pScope);
}

Expression* Semantic::solveConditional(Expression* a_pCond, Expression* a_pThen, Expression* a_pElse,
                                       LanguageElement* a_pScope)
{
    if (a_pCond->isTemplateDependant() || a_pThen->isTemplateDependant() || a_pElse->isTemplateDependant())

        return a_pScope->New<ConditionalExpression>(a_pCond, a_pThen, a_pElse);

    /// Built in operator
    VisitorData binaryOperatorData;
    Expressions operatorArguments;
    operatorArguments.push_back(a_pCond);
    operatorArguments.push_back(a_pThen);
    operatorArguments.push_back(a_pElse);
    binaryOperatorData.id = e_VisitorFunction_BinaryOperator;
    StringView  op = "?:";
    const void* binaryOperatorDataIn[3] = {&op, operatorArguments.data(), &a_pScope};
    binaryOperatorData.in = binaryOperatorDataIn;
    LanguageElement* pResolved = nullptr;
    void*            binaryOperatorDataOut[1] = {&pResolved};
    binaryOperatorData.out = binaryOperatorDataOut;
    Application::Get()->visit(this, binaryOperatorData);
    if (pResolved)
        return pResolved->asExpression();
    return nullptr;
}

Expression* Semantic::solveBinaryOperator(StringView a_strOp, Expression* a_pLeft, Expression* a_pRight,
                                          LanguageElement* a_pScope)
{
    if (a_pLeft->isTemplateDependant() || a_pRight->isTemplateDependant())
    {
        return a_pScope->New<BinaryOperationExpression>(a_strOp, a_pLeft, a_pRight);
    }
    if (a_pScope == nullptr)
        a_pScope = Namespace::Global();
    Type* pLeftNakedType = a_pLeft->getValueType()->removeReference()->removeQualifiers();
    Type* pRightNakedType = a_pRight->getValueType()->removeReference()->removeQualifiers();

    bool bLeftIsClassOrEnumeration = pLeftNakedType->asClassType() || pLeftNakedType->asEnum();
    bool bRightIsClassOrEnumeration = pRightNakedType->asClassType() || pRightNakedType->asEnum();

    LanguageElement* pResolved = nullptr;
    Modifiers        access = 0;
    void*            binaryOperatorDataOut[1] = {&pResolved};

    if (bLeftIsClassOrEnumeration || bRightIsClassOrEnumeration)
    {
        /// standard 13.3.1.1.2.2
        /// If either operand has a type that is a class or an enumeration, a user-defined operator
        /// function might be declared that implements this operator or a user-defined conversion
        /// can be necessary to convert the operand to a type that is appropriate for a built-in
        /// operator. In this case, overload resolution is used to determine which operator function
        /// or built-in operator is to be invoked to implement the operator. Therefore, the operator
        /// notation is first transformed to the equivalent function-call notation as summarized in
        /// Table 10 (where @ denotes one of the operators covered in the specified subclause).

        /// first solving try with left.operator@(right) call
        String operatorFunctionName = "operator";
        operatorFunctionName += a_strOp;
        Expressions operatorArguments;
        operatorArguments.push_back(a_pRight);
        pResolved = silentQualifiedLookup(a_pLeft, operatorFunctionName, phantom::NullOpt,
                                          MakeArrayView(operatorArguments), a_pScope);
        if (pResolved)
        {
            return pResolved->asExpression();
        }

        /// second solving try with ::operator@(left, right) call
        operatorArguments.front() = a_pLeft;
        operatorArguments.push_back(a_pRight);
        LanguageElement* pScope = a_pScope ? a_pScope->getSource() : nullptr;
        if (pScope || (pScope = a_pScope ? a_pScope->getEnclosingNamespace() : nullptr))
        {
            pResolved =
            silentUnqualifiedLookup(operatorFunctionName, phantom::NullOpt, MakeArrayView(operatorArguments), pScope);
        }

        /// argument dependant lookup
        if (pResolved == nullptr)
        {
            if (auto pNS = pLeftNakedType->getNamingScope())
                pResolved = silentUnqualifiedLookup(operatorFunctionName, phantom::NullOpt,
                                                    MakeArrayView(operatorArguments), pNS->asLanguageElement());
            if (pResolved == nullptr)
            {
                if (auto pNS = pRightNakedType->getNamingScope())
                    pResolved = silentUnqualifiedLookup(operatorFunctionName, phantom::NullOpt,
                                                        MakeArrayView(operatorArguments), pNS->asLanguageElement());
            }
        }

        if (pResolved)
        {
            return pResolved->asExpression();
        }
    }
    /// standard 13.3.1.1.2.1
    /// If no operand of an operator in an expression has a type that is a class or an enumeration,
    /// the operator is assumed to be a built-in operator and interpreted according to Clause 5. [
    /// Note: Because ., .*, and :: cannot be overloaded, these operators are always built-in
    /// operators interpreted according to Clause 5. ?: cannot be overloaded, but the rules in this
    /// subclause are used to determine the conversions to be applied to the second and third
    /// operands when they have class or enumeration type (5.16).

    /// Built in operator
    VisitorData binaryOperatorData;
    Expressions operatorArguments;
    operatorArguments.push_back(a_pLeft);
    operatorArguments.push_back(a_pRight);
    binaryOperatorData.id = e_VisitorFunction_BinaryOperator;
    const void* binaryOperatorDataIn[3] = {&a_strOp, operatorArguments.data(), &a_pScope};
    binaryOperatorData.in = binaryOperatorDataIn;
    binaryOperatorData.out = binaryOperatorDataOut;
    Application::Get()->visit(this, binaryOperatorData);
    if (pResolved == nullptr)
    {
        CxxSemanticError("'%s' no binary operator overload found, arguments are (%s)", (const char*)a_strOp.data(),
                         FormatArgumentList(operatorArguments.data(), operatorArguments.size()).data());
        return nullptr;
    }
    if (Expression* pResolvedExpression = pResolved->asExpression())
    {
        return pResolvedExpression;
    }
    CxxSemanticError("'%s' expected expression", FormatCStr(pResolved));
    return nullptr;
}
Expression* Semantic::solveUnaryPreOperator(StringView a_strOp, Expression* a_pExpression,
                                            LanguageElement* in_pContextScope)
{
    if (a_pExpression->isTemplateDependant())
    {
        return New<UnaryPreOperationExpression>(a_strOp, a_pExpression);
    }
    if (in_pContextScope == nullptr)
        in_pContextScope = Namespace::Global();
    Type* pNakedType = a_pExpression->getValueType()->removeReference()->removeQualifiers();

    if (a_strOp == "*" && pNakedType->asPointer())
    {
        return a_pExpression->dereference(in_pContextScope);
    }

    if (a_strOp == "&" && a_pExpression->getValueType()->asReference())
    {
        return a_pExpression->address(in_pContextScope);
    }

    bool bIsClassOrEnumeration = pNakedType->asClassType() || pNakedType->asEnum();

    LanguageElement* pResolved = nullptr;
    void*            binaryOperatorDataOut[1] = {&pResolved};

    if (bIsClassOrEnumeration)
    {
        /// standard 13.3.1.1.2.2
        /// If either operand has a type that is a class or an enumeration, a user-defined operator
        /// function might be declared that implements this operator or a user-defined conversion
        /// can be necessary to convert the operand to a type that is appropriate for a built-in
        /// operator. In this case, overload resolution is used to determine which operator function
        /// or built-in operator is to be invoked to implement the operator. Therefore, the operator
        /// notation is first transformed to the equivalent function-call notation as summarized in
        /// Table 10 (where @ denotes one of the operators covered in the specified subclause).

        /// first solving try with expr.operator@() call
        VisitorData userDefinedbinaryOperatorData;
        String      operatorFunctionName = "operator";
        operatorFunctionName += a_strOp;
        Expressions operatorArguments;
        pResolved = internalQualifiedLookup(a_pExpression, operatorFunctionName, phantom::NullOpt,
                                            MakeArrayView(operatorArguments), in_pContextScope);
        if (pResolved)
        {
            return pResolved->asExpression();
        }

        /// second solving try with ::operator@(expr) call
        operatorArguments.push_back(a_pExpression);
        LanguageElement* pScope = in_pContextScope->getSource();
        if (pScope || (pScope = in_pContextScope->getEnclosingNamespace()))
        {
            pScope->visit(this, userDefinedbinaryOperatorData);
        }
        if (pResolved)
        {
            return pResolved->asExpression();
        }

        /// argument dependant lookup
        if (pResolved == nullptr)
        {
            if (pNakedType->getNamingScope())
            {
                pResolved =
                silentUnqualifiedLookup(operatorFunctionName, phantom::NullOpt, MakeArrayView(operatorArguments),
                                        pNakedType->getNamingScope()->asLanguageElement());
            }
        }
    }
    /// standard 13.3.1.1.2.1
    /// If no operand of an operator in an expression has a type that is a class or an enumeration,
    /// the operator is assumed to be a built-in operator and interpreted according to Clause 5. [
    /// Note: Because ., .*, and :: cannot be overloaded, these operators are always built-in
    /// operators interpreted according to Clause 5. ?: cannot be overloaded, but the rules in this
    /// subclause are used to determine the conversions to be applied to the second and third
    /// operands when they have class or enumeration type (5.16).

    /// Built in operator
    VisitorData binaryOperatorData;
    Expressions operatorArguments;
    operatorArguments.push_back(a_pExpression);
    binaryOperatorData.id = e_VisitorFunction_UnaryPreOperator;
    const void* binaryOperatorDataIn[3] = {&a_strOp, operatorArguments.data(), &in_pContextScope};
    binaryOperatorData.in = binaryOperatorDataIn;
    binaryOperatorData.out = binaryOperatorDataOut;
    Application::Get()->visit(this, binaryOperatorData);
    if (pResolved == nullptr)
    {
        CxxSemanticError("'%s' no unary prefixed operator overload found, arguments are (%s)",
                         (const char*)a_strOp.data(), FormatArgumentList(&a_pExpression, 1).data());
        return nullptr;
    }
    if (Expression* pResolvedExpression = pResolved->asExpression())
    {
        return pResolvedExpression;
    }
    CxxSemanticError("'%s' expected expression", FormatCStr(pResolved));
    return nullptr;
}
Expression* Semantic::solveUnaryPostOperator(StringView a_strOp, Expression* a_pExpression,
                                             LanguageElement* in_pContextScope)
{
    if (a_pExpression->isTemplateDependant())
    {
        return New<UnaryPostOperationExpression>(a_strOp, a_pExpression);
    }
    if (in_pContextScope == nullptr)
        in_pContextScope = Namespace::Global();
    ConstantExpression* pZeroInt = New<ConstantExpression>(newConstant<int>(0));

    Type* pNakedType = a_pExpression->getValueType()->removeReference()->removeQualifiers();

    bool bIsClassOrEnumeration = pNakedType->asClassType() || pNakedType->asEnum();

    LanguageElement* pResolved = nullptr;
    void*            binaryOperatorDataOut[1] = {&pResolved};

    if (bIsClassOrEnumeration)
    {
        /// standard 13.3.1.1.2.2
        /// If either operand has a type that is a class or an enumeration, a user-defined operator
        /// function might be declared that implements this operator or a user-defined conversion
        /// can be necessary to convert the operand to a type that is appropriate for a built-in
        /// operator. In this case, overload resolution is used to determine which operator function
        /// or built-in operator is to be invoked to implement the operator. Therefore, the operator
        /// notation is first transformed to the equivalent function-call notation as summarized in
        /// Table 10 (where @ denotes one of the operators covered in the specified subclause).

        /// first solving try with expr.operator@(0) call
        VisitorData userDefinedbinaryOperatorData;
        String      operatorFunctionName = "operator";
        operatorFunctionName += a_strOp;
        Expressions         operatorArguments;
        ConstantExpression* pZeroInt = nullptr;
        bool                accessOperator = a_strOp == "->" || a_strOp == ".*" || a_strOp == "->*";
        if (!accessOperator)
        {
            pZeroInt = New<ConstantExpression>(newConstant<int>(0));
            operatorArguments.push_back(pZeroInt);
        }
        pResolved = internalQualifiedLookup(a_pExpression, operatorFunctionName, phantom::NullOpt,
                                            MakeArrayView(operatorArguments), in_pContextScope);
        if (pResolved)
        {
            return pResolved->asExpression();
        }
        if (accessOperator)
        {
            CxxSemanticError(
            "'%.*s' no operator defined for '%.*s'", PHANTOM_STRING_AS_PRINTF_ARG(a_strOp),
            PHANTOM_STRING_AS_PRINTF_ARG(FormatElement(a_pExpression->getValueType()->removeReference())));
            return nullptr;
        }

        // at least one argument
        if (operatorArguments.size())
        {
            /// second solving try with ::operator@(expr, 0) call
            operatorArguments.front() = a_pExpression;
            operatorArguments.push_back(pZeroInt);
            LanguageElement* pScope = in_pContextScope->getSource();
            if (pScope || (pScope = in_pContextScope->getEnclosingNamespace()))
            {
                pScope->visit(this, userDefinedbinaryOperatorData);
            }
            if (pResolved)
            {
                return pResolved->asExpression();
            }
        }

        /// argument dependant lookup
        if (pResolved == nullptr)
        {
            if (pNakedType->getNamingScope())
            {
                pResolved =
                silentUnqualifiedLookup(operatorFunctionName, phantom::NullOpt, MakeArrayView(operatorArguments),
                                        pNakedType->getNamingScope()->asLanguageElement());
            }
        }
    }
    /// standard 13.3.1.1.2.1
    /// If no operand of an operator in an expression has a type that is a class or an enumeration,
    /// the operator is assumed to be a built-in operator and interpreted according to Clause 5. [
    /// Note: Because ., .*, and :: cannot be overloaded, these operators are always built-in
    /// operators interpreted according to Clause 5. ?: cannot be overloaded, but the rules in this
    /// subclause are used to determine the conversions to be applied to the second and third
    /// operands when they have class or enumeration type (5.16).

    /// Built in operator
    VisitorData binaryOperatorData;
    Expressions operatorArguments;
    operatorArguments.push_back(a_pExpression);
    operatorArguments.push_back(pZeroInt);
    binaryOperatorData.id = e_VisitorFunction_UnaryPostOperator;
    const void* binaryOperatorDataIn[3] = {&a_strOp, operatorArguments.data(), &in_pContextScope};
    binaryOperatorData.in = binaryOperatorDataIn;
    binaryOperatorData.out = binaryOperatorDataOut;
    Application::Get()->visit(this, binaryOperatorData);
    if (pResolved == nullptr)
    {
        if (pZeroInt)
        {
            pZeroInt->getSource()->Delete(pZeroInt);
        }
        CxxSemanticError("'%.*s' no unary postfixed operator overload found, arguments are (%s)",
                         PHANTOM_STRING_AS_PRINTF_ARG(a_strOp), FormatArgumentList(&a_pExpression, 1).data());
        return nullptr;
    }
    if (Expression* pResolvedExpression = pResolved->asExpression())
    {
        return pResolvedExpression;
    }
    CxxSemanticError("'%.*s' expected expression", PHANTOM_STRING_AS_PRINTF_ARG(FormatElement(pResolved)));
    return nullptr;
}

LanguageElement* Semantic::resolveTemplateDependency(LanguageElement*            a_pElement,
                                                     TemplateSubstitution const& a_TemplateSubstitution,
                                                     EClassBuildState a_uiPass, LanguageElement* a_pScope, int a_Flags)
{
    if (a_pScope == nullptr)
        a_pScope = Namespace::Global();
    LanguageElement* pResult = nullptr;
    VisitorData      data;
    data.id = e_VisitorFunction_ResolveTemplateDependencies;
    const void* in[3] = {&a_TemplateSubstitution, &a_uiPass, &a_pScope};
    void*       out[1] = {&pResult};
    data.in = in;
    data.out = out;
    data.flags = a_Flags;
    a_pElement->visit(this, data);
    PHANTOM_SEMANTIC_ASSERT_SEMANTIC_HAS_ERROR_REPORT(pResult);
    return pResult;
}

bool Semantic::checkGenericSignature(Signature* a_pSignature, size_t genericParamCount)
{
    auto const& params = a_pSignature->getParameters();
    if (params.size() < genericParamCount)
        return false;
    int count = 0;
    for (size_t i = 0; i < genericParamCount; ++i)
    {
        if (params[i]->getValueType()->getQualifiedName() != "phantom::Generic::Param")
        {
            return false;
        }
    }
    return true;
}

Expression* Semantic::solveGenericCall(LanguageElement* a_pLHS, StringView a_Name, LanguageElementsView a_TemplateArgs,
                                       OptionalArrayView<Expression*> a_FunctionArgs, LanguageElement* a_pScope,
                                       Type* a_pInitializationType)
{
    Types requiredBaseTypes;

    bool templateDependant = false;

    Types templateArgTypes;
    for (auto templateArg : a_TemplateArgs)
    {
        templateDependant = templateDependant || templateArg->isTemplateDependant();

        if (Type* pType = templateArg->asType())
        {
            templateArgTypes.push_back(pType);
            continue;
        }
        // CppLiteError("'%s' : non-type template parameter are not supported yet", a_Identifier);
        return nullptr;
    }

    OwnersGuard<Expressions> args;
    // push generic types
    for (Type* pType : templateArgTypes)
    {
        args.push_back(convert(New<ConstantExpression>(createConstant(size_t(pType))), PHANTOM_TYPEOF(Type*),
                               CastKind::Reinterpret, UserDefinedFunctions::None, a_pScope));
    }

    if (a_FunctionArgs)
        for (auto arg : *a_FunctionArgs)
            args.push_back(arg);

    LanguageElement* pResult = nullptr;
    if (a_pLHS)
        pResult =
        silentQualifiedLookup(a_pLHS, a_Name, phantom::NullOpt, MakeArrayView(*args), a_pScope, a_pInitializationType);
    else
        pResult =
        silentUnqualifiedLookup(a_Name, phantom::NullOpt, MakeArrayView(*args), a_pScope, a_pInitializationType);

    if (!pResult)
        return nullptr;

    if (templateDependant)
    {
        return New<TemplateDependantExpression>(New<TemplateDependantElement>(
        a_pLHS, a_Name, a_TemplateArgs, (OptionalArrayView<LanguageElement*>&)a_FunctionArgs));
    }

    if (OwnerGuard<Expression> pExp = pResult->asExpression())
    {
        if (CallExpression* pCallExp = phantom::Object::Cast<CallExpression>(pExp->removeRValueReferenceExpression()))
        {
            Subroutine* pSubroutine = pCallExp->getSubroutine();
            if (templateArgTypes.size() > 0)
            {
                if (!checkGenericSignature(pSubroutine->getSignature(), templateArgTypes.size()))
                {
                    // CppLiteError("'%s' : invalid generic signature : invalid generic parameters
                    // count", a_Identifier);
                    return nullptr;
                }
            }

            // check return

            auto const& params = pSubroutine->getParameters();
            for (size_t i = 0; i < templateArgTypes.size(); ++i)
            {
                auto pRequiredArgType = params[i]->getValueType()->getTemplateSpecialization()->getArgument(0);
                if (pRequiredArgType == PHANTOM_TYPEOF(phantom::Generic::Auto))
                    continue;
                if (!templateArgTypes[i]->isA(static_cast<Type*>(pRequiredArgType)))
                {
                    // CppLiteError("'%s' : generic parameter pattern match failed '%d'",
                    // a_Identifier, int(i));
                    return nullptr;
                }
            }

            // check arguments passing

            for (size_t i = templateArgTypes.size(); i < args.size(); ++i)
            {
                Expression* pArgument = pCallExp->getArguments()[i];
                Type*       pArgType = pArgument->getValueType()->removeReference();
                Type*       pParamType = params[i]->getValueType();
                if (pParamType->getQualifiedName() == "phantom::Generic::Arg")
                {
                    Type* pParamTypePattern = pParamType->getTemplateSpecialization()->getArgument(0)->asType();
                    Type* pParamTypePatternNaked = pParamTypePattern->removeEverything();
                    if (pParamTypePatternNaked->getQualifiedName() == "phantom::Generic::ParamType")
                    {
                        Constant* pConstant =
                        pParamTypePatternNaked->getTemplateSpecialization()->getArgument(0)->asConstant();
                        size_t value = 0;
                        pConstant->getValue(&value);
                        if (value < templateArgTypes.size())
                        {
                            Type* pExpectedType = pParamTypePattern->replicate(templateArgTypes[value]);
                            if (!pArgType->isSame(pExpectedType))
                            {
                                if (!pCallExp->transformArgument(
                                    this, i + (pSubroutine->asMethod() != nullptr),
                                    [&](Expression*) { return convert((*args)[i], pExpectedType, a_pScope); }))
                                {
                                    // failed to match argument pattern
                                    return nullptr;
                                }
                            }
                        }
                        else
                        {
                            CxxSemanticError("'%.*s' : invalid generic signature : param index "
                                             "'%zu' out of range",
                                             a_Name, value);
                            return nullptr;
                        }
                    }
                }
            }
        }
        else
        {
            return nullptr;
        }
        Type* pType = pExp->getValueType();
        Type* pNakedType = pType->removeEverything();
        if (Template* pTemplate = pNakedType->getTemplate())
        {
            if (pTemplate->getQualifiedName() == "phantom::Generic::Return")
            {
                // TODO : re-uncomment that (it is just an optimization, not a big deal)
                // pExp->setTemporary(false); // ensure we don't call destructors for Generic Return
                Type* pReturnTypeBase = pNakedType->getTemplateSpecialization()->getArgument(1)->asType();
                Type* pReturnTypePattern = pNakedType->getTemplateSpecialization()->getArgument(0)->asType();
                Type* pReturnTypePatternNaked = pReturnTypePattern->removeEverything();
                if (pReturnTypePatternNaked->getQualifiedName() == "phantom::Generic::ParamType")
                {
                    Constant* pConstant =
                    pReturnTypePatternNaked->getTemplateSpecialization()->getArgument(0)->asConstant();
                    size_t value = 0;
                    pConstant->getValue(&value);
                    if (value < templateArgTypes.size())
                    {
                        Type* pBaseType = templateArgTypes[value];
                        Type* pReturnType = pReturnTypePattern->replicate(pBaseType);
                        pExp = convert(pExp, pReturnTypeBase, CastKind::Explicit, UserDefinedFunctions::All, a_pScope);
                        PHANTOM_ASSERT(pExp, "operator X() should have been called");
                        Expression* pConv = convert(pExp, pReturnType, CastKind::Reinterpret);
                        if (pConv == nullptr)
                        {
                            CxxSemanticError("'%.*s' : cannot convert to generic return", a_Name);
                            return nullptr;
                        }
                        pExp = pConv;
                    }
                    else
                    {
                        CxxSemanticError("'%.*s' : invalid generic signature : param index '%zu' out of range", a_Name,
                                         value);
                        return nullptr;
                    }
                }
            }
        }
        for (auto type : templateArgTypes)
            pExp->addReferencedElement(
            type); // ensure we have a dependency between template argument source and current source
        return pExp;
    }
    CxxSemanticError("'%.*s' : not an expression", a_Name);
    return nullptr;
}

LanguageElement* Semantic::instantiateTemplateElement(LanguageElement*        a_pPrimary,
                                                      TemplateSpecialization* a_pInstantiation, EClassBuildState a_Pass,
                                                      LanguageElement* a_pScope, int a_Flags)
{
    return instantiateTemplateElement(a_pPrimary, a_pInstantiation->getArgumentSubstitution(), a_Pass, a_pScope,
                                      a_Flags);
}
LanguageElement* Semantic::instantiateTemplateElement(LanguageElement*            a_pPrimary,
                                                      const TemplateSubstitution& a_TemplateSubstitution,
                                                      EClassBuildState a_Pass, LanguageElement* a_pScope, int a_Flags)
{
    if (a_pScope == nullptr)
        a_pScope = Namespace::Global();
    LanguageElement* pInstance = templateInstantiations()[a_TemplateSubstitution.getInstantiation()][a_pPrimary];
    auto             pResult = pInstance;
    VisitorData      data;
    data.id = e_VisitorFunction_InstanciateTemplateElement;
    const void* in[5] = {&a_TemplateSubstitution, &a_Pass, &a_pScope};
    void*       out[1] = {&pResult};
    data.in = in;
    data.out = out;
    data.flags = a_Flags;
    pushCodeRangeLocation(a_pPrimary->getCodeRangeLocation());
    a_pPrimary->visit(this, data);
    popCodeRangeLocation();
    if (pResult == nullptr)
        return nullptr;
    if (pInstance == nullptr)
    {
        templateInstantiations()[a_TemplateSubstitution.getInstantiation()][a_pPrimary] = pResult;
        pResult->setCodeRange(a_pPrimary->getCodeRange());
        if (auto pPrimarySym = a_pPrimary->asSymbol())
        {
            static_cast<Symbol*>(pResult)->setMetaDatas(pPrimarySym->getMetaDatas());
        }
    }
    return pResult;
}

void Semantic::setAccessModifiersEnabled(bool a_bEnabled)
{
    m_bAccessModifiersEnabled = a_bEnabled;
}

void Semantic::pushCodeRangeLocation(const CodeRangeLocation& a_Loc)
{
    m_CodeRangeLocationStack.push_back(a_Loc);
    if (m_CodeRangeLocationStack.back().source == nullptr)
        m_CodeRangeLocationStack.back().source = getSource();
}

void Semantic::popCodeRangeLocation()
{
    m_CodeRangeLocationStack.pop_back();
}

LanguageElement* Semantic::instantiateTemplate(Template* a_pInput, const LanguageElements& a_Arguments,
                                               LanguageElement* in_pContextScope /*= nullptr*/)
{
    auto pPrevMessage = m_pMessage;
    m_pMessage = new_<Message>();
    auto onExit = phantom::makeScopeExit([&]() {
        if (m_pMessage->getChildCount())
        {
            pPrevMessage->addChild(m_pMessage);
        }
        m_pMessage = pPrevMessage;
    });
    if (in_pContextScope == nullptr)
    {
        // use the "default" source inside the main module (the only one we know it will last forReeVeEeerr)
        in_pContextScope = Application::Get()->getMainModule()->getDefaultPackage()->getOrCreateSource("default");
    }

    /// Do template accepts given arguments ?
    bool empty = true;
    bool full = true;
    for (auto pArg : a_Arguments)
    {
        if (pArg->isTemplateDependant())
        {
            /// if any template dependant element found, it's not a full instanciation
            full = false;
        }
        else
        {
            /// if any template independant element found, it's not an empty instanciation
            empty = false;
        }
    }
    if (full)
    {
        LanguageElements              arguments = a_Arguments;
        OwnersGuard<LanguageElements> defaultArgs;

        /// Full => start first by looking for full specializations

        /// if arguments are full, instantiate missing template default arguments
        size_t argCount = a_Arguments.size();
        auto&  params = a_pInput->getTemplateParameters();
        size_t paramCount = params.size();
        bool   variadic = a_pInput->getTemplateSignature()->isVariadic();
        auto*  pVariadicPH =
        variadic ? a_pInput->getTemplateSignature()->getTemplateParameters().back()->getPlaceholder() : nullptr;
        if (paramCount < argCount)
        {
            if (!variadic)
            {
                CxxSemanticError("'%s' too many template arguments, %zu given, %zu expected", FormatCStr(a_pInput),
                                 argCount, paramCount);
                return nullptr;
            }
        }
        else if (a_pInput->getDefaultArgumentCount() < (paramCount - argCount))
        {
            CxxSemanticError("'%s' not enough template arguments, %zu given, %zu required at minimum",
                             FormatCStr(a_pInput), argCount, paramCount - a_pInput->getDefaultArgumentCount());
            return nullptr;
        }

        // prepare for substitution
        TemplateSubstitution explicitSubstitution;
        auto&                parms = a_pInput->getTemplateParameters();
        for (size_t i = 0; i < a_Arguments.size(); ++i)
        {
            arguments[i] = arguments[i]->hatchExpression();
            explicitSubstitution.insert(parms[std::min(i, parms.size() - 1)]->getPlaceholder(), arguments[i]);
        }

        if (paramCount - argCount)
        {
            for (; argCount < paramCount; ++argCount)
            {
                LanguageElement* pSolvedArgument =
                resolveTemplateDependency(a_pInput->getDefaultArgument(argCount), explicitSubstitution,
                                          e_ClassBuildState_Instantiated, in_pContextScope, 0);
                if (pSolvedArgument == nullptr)
                {
                    return nullptr;
                }
                if (Expression* pExpression = pSolvedArgument->asExpression())
                {
                    if (!(pExpression->isTemplateDependant()))
                    {
#pragma message(PHANTOM_TODO                                                                                           \
                "create errors for 'not compile time' and 'not integral constant' + solve expression leaks here")
                        if (!(pExpression->isCompileTime()))
                        {
                            CxxSemanticError("expected compile-time constant");
                            Delete(pExpression);
                            return nullptr;
                        }
                        if (pExpression->getValueType()->asIntegralType() == nullptr)
                        {
                            CxxSemanticError("expected integral constant");
                            Delete(pExpression);
                            return nullptr;
                        }
                        char               buffer[128];
                        InterpreterContext ctx;
                        pExpression->load(ctx, buffer);
                        pSolvedArgument = pExpression->getValueType()->asIntegralType()->createConstant(buffer);
                        Delete(pExpression);
                    }
                }
                explicitSubstitution.insert(params[argCount]->getPlaceholder(), pSolvedArgument);
                defaultArgs.push_back(pSolvedArgument);
            }
        }
        //         if (pVariadicPH)
        //         {
        //             // variadic substitution
        //             for (size_t i = paramCount; i < argCount; ++i)
        //             {
        //                 substitution.insert(pVariadicPH, a_Arguments[i]);
        //             }
        //         }
        PHANTOM_SEMANTIC_ASSERT(!variadic || explicitSubstitution.size() >= params.size() - 1);

        Module* pCurrModule = this->getSource()->getModule();

        // already instantiated ?

        if (TemplateSpecialization* pAlreadyThere =
            a_pInput->getTemplateSpecializationForModule(explicitSubstitution.getArguments(), pCurrModule))
        {
            Symbol* pTemplated{};
            if (auto pExtended = pAlreadyThere->getExtendedSpecialization())
                pTemplated = pExtended->getTemplated();
            else
                pTemplated = pAlreadyThere->getTemplated();
            if (pTemplated &&
                (pTemplated->getModule()->isNative() || pCurrModule == pTemplated->getModule() ||
                 pCurrModule->hasDependencyCascade(pTemplated->getModule())))
                return pTemplated;
        }

        /// Find viable specializations
        TemplateSpecializations           viableSpecializations;
        SmallVector<TemplateSubstitution> viableSubstitutions;
        PlaceholderMap                    deductionMap;
        for (auto pT0 : a_pInput->getTemplateSpecializations())
        {
            if (pT0->getSource()->getVisibility() == Visibility::Private)
                continue;

            // we excluded every full specializations (as we should already have found a full matching just above)
            if (pT0->isFull())
                continue;

            if (pT0->isNative() && pT0->getTemplated() == nullptr) // avoid considering native partial template
                                                                   // specializations as valid specializations to use
                continue;

            deductionMap.clear(); // reset template argument deductions for this spec treatment

            bool   result = true;
            size_t argumentsCount = explicitSubstitution.size();
            size_t specArgCount = pT0->getArguments().size();

            TemplateSubstitution specSubstitution;
            for (size_t i = 0; i < specArgCount - variadic; ++i)
            {
                LanguageElement* pParameter = pT0->getArgument(i);
                LanguageElement* pArgument = explicitSubstitution.getArgument(i);
                if (!(templateArgumentDeduction(pParameter, pArgument, deductionMap)))
                {
                    result = false;
                    break;
                }
            }
            for (auto& ph_deductions : deductionMap)
            {
                for (auto& deduction : ph_deductions.second)
                {
                    specSubstitution.insert(ph_deductions.first, deduction);
                }
            }
            if (result && variadic && pT0->isEmpty())
            {
                for (size_t i = specArgCount - 1; i < argumentsCount; ++i)
                {
                    PlaceholderMap   tempDeductionMap;
                    LanguageElement* pParameter =
                    pT0->getArgument(specArgCount - 1); // variadic argument stays the same for arg deduction
                    LanguageElement* pArgument =
                    explicitSubstitution.getArgument(i); // only the substitution varies with 'i'
                    if (!(templateArgumentDeduction(pParameter, pArgument, tempDeductionMap)))
                    {
                        result = false;
                        break;
                    }
                    for (auto& ph_deductions : tempDeductionMap)
                    {
                        for (auto&& deduction : ph_deductions.second)
                            specSubstitution.insert(pParameter->asPlaceholder(), deduction);
                    }
                }
            }
            if (result && a_pInput->isNative())
            {
                while (argumentsCount < pT0->getArgumentCount())
                {
                    LanguageElement* pDefaultArgument = pT0->getDefaultArgument(argumentsCount);
                    if (pDefaultArgument == nullptr ||
                        !(pT0->getArgument(argumentsCount)
                          ->isSame(pDefaultArgument))) /// this specialization doesn't use default
                                                       /// argument expected for missing argument =>
                                                       /// not a valid specialization for our search
                    {
                        result = false;
                        break;
                    }
                    argumentsCount++;
                }
            }
            if (result)
            {
                if (auto pTSpecSign = pT0->getTemplateSignature())
                {
                    if (pTSpecSign->isVariadic())
                    {
                        result = specSubstitution.size() >= (pTSpecSign->getTemplateParameters().size() - 1);
                    }
                    else
                    {
                        result = (specSubstitution.size() == pTSpecSign->getTemplateParameters().size());
                    }
                }
                else
                {
                    result = (specSubstitution.size() == 0);
                }
                if (result)
                {
                    viableSpecializations.push_back(pT0);
                    viableSubstitutions.push_back(std::move(specSubstitution));
                }
            }
        }
        /// Find best specialization
        TemplateSpecialization* pBestTemplateSpecialization = nullptr;
        TemplateSubstitution*   pBestSubstitutions = nullptr;
        size_t                  viableSpecializationsCount = viableSpecializations.size();
        if (viableSpecializationsCount)
        {
            if (viableSpecializationsCount == 1)
            {
                pBestTemplateSpecialization = viableSpecializations.back();
                pBestSubstitutions = &viableSubstitutions.back();
            }
            else
            {
                for (size_t i = 0; i < viableSpecializationsCount; ++i)
                {
                    TemplateSpecialization* pT0 = viableSpecializations[i];

                    int    bestResult = -1;
                    size_t j = 0;
                    for (; j < viableSpecializationsCount; ++j)
                    {
                        TemplateSpecialization* pT1 = viableSpecializations[j];
                        if ((pT0 == pT1))
                            continue;
                        int result = compareSpecialized(pT0, pT1);
                        PHANTOM_SEMANTIC_ASSERT(-result == compareSpecialized(pT1, pT0));
                        if (result >= 0)
                            bestResult = std::max(bestResult, result);
                        else
                            break;
                    }
                    if (j == viableSpecializationsCount)
                    {
                        if (bestResult == 1)
                        {
                            pBestTemplateSpecialization = pT0;
                            pBestSubstitutions = &viableSubstitutions[i];
                            break;
                        }
                    }
                }
            }
        }

        if (pBestTemplateSpecialization)
        {
            TemplateSpecialization* pUsedTemplateSpecialization = pBestTemplateSpecialization;
            if (auto pExtended = pBestTemplateSpecialization->getExtendedSpecialization())
                pUsedTemplateSpecialization = pExtended;
            if (pUsedTemplateSpecialization->getTemplated() == nullptr)
            {
                CxxSemanticError("'%s' undefined template specialization", FormatCStr(pUsedTemplateSpecialization));
                return nullptr;
            }

            if (pUsedTemplateSpecialization->getTemplate()->getNamingScope() == nullptr)
            {
                //                 CxxSemanticError("'%s' invalid or incomplete template definition (compilation of
                //                 template might have "
                //                                  "failed previous to the specialization)",
                //                                  FormatCStr(pUsedTemplateSpecialization));
                return nullptr;
            }

            // If full just return the "templated" symbol
            if (pUsedTemplateSpecialization->isFull())
            {
                return pUsedTemplateSpecialization->getTemplated();
            }

            LanguageElement* pTemplated = nullptr;

            TemplateSpecialization* pInstantiation = nullptr;

            // alias template ?
            if (pUsedTemplateSpecialization->getTemplated()->asAlias())
            {
                Symbol* pSymbol = static_cast<Symbol*>(
                resolveTemplateDependency(pUsedTemplateSpecialization->getTemplated(), explicitSubstitution,
                                          e_ClassBuildState_Instantiated, in_pContextScope, 0));
                if (pSymbol == nullptr)
                    return nullptr;

                pInstantiation = (getSource() ? getSource() : in_pContextScope->getSource())
                                 ->addTemplateInstantiation(pUsedTemplateSpecialization,
                                                            explicitSubstitution.getArguments(), *pBestSubstitutions);

                size_t i = defaultArgs->size();
                size_t j = pInstantiation->getArguments().size();
                while (i-- && j--)
                {
                    pInstantiation->setDefaultArgument(j, (*defaultArgs)[i]);
                }

                pTemplated =
                pInstantiation->New<Alias>(pSymbol, pUsedTemplateSpecialization->getTemplated()->getName());
            }
            else // class template
            {
                pInstantiation = (getSource() ? getSource() : in_pContextScope->getSource())
                                 ->addTemplateInstantiation(pUsedTemplateSpecialization,
                                                            explicitSubstitution.getArguments(), *pBestSubstitutions);
                pTemplated = instantiateTemplateElement(pUsedTemplateSpecialization->getTemplated(), pInstantiation,
                                                        e_ClassBuildState_Instantiated, pInstantiation, 0);
            }
            if (pTemplated == nullptr)
                return nullptr;
            PHANTOM_SEMANTIC_ASSERT(pTemplated->asSymbol());
            if (auto pNS = pInstantiation->getTemplate()->getNamespace())
            {
                if (auto pType = pTemplated->asType())
                {
                    pNS->addType(pType);
                }
                else
                {
                    pNS->addCustomSymbol(static_cast<Symbol*>(pTemplated));
                }
            }
            pInstantiation->setTemplated(static_cast<Symbol*>(pTemplated));
            PHANTOM_SEMANTIC_ASSERT(pTemplated->asSymbol());
            return pTemplated;
        }
        return nullptr;
    }
    else
    {
        TemplateSpecialization* pAscendantTemplateSpecialization =
        in_pContextScope->getEnclosingTemplateSpecialization();
        PHANTOM_SEMANTIC_ASSERT(
        pAscendantTemplateSpecialization == nullptr ||
        pAscendantTemplateSpecialization->getTemplateSignature()->getTemplateParameters().size() != 0);

        Type* pTemplatedType = nullptr;

        /// Declaration outside a template declaration
        /// This case is for deferred template specialization member function definition
        /// ex: template<typename T> void Foo<T, int>::bar() {}
        /// => we look for the specialization matching exactly the given arguments (not partially)
        if (pAscendantTemplateSpecialization == nullptr
            /// not solving template signature default parameters
            && in_pContextScope->asTemplateSignature() == nullptr)
        {
            TemplateSpecialization* pTemplateSpecialization =
            empty ? a_pInput->getEmptyTemplateSpecialization() : a_pInput->getTemplateSpecialization(a_Arguments);
            if (pTemplateSpecialization == nullptr)
            {
                if (empty)
                {
                    CxxSemanticError("'%s' : no matching template declaration for specialization",
                                     a_pInput->getName().data());
                }
                else
                {
                    CxxSemanticError("'%s' : no template specialization matching arguments (%s)",
                                     a_pInput->getName().data(),
                                     FormatElementList(a_Arguments.data(), a_Arguments.size()).data());
                }
                return nullptr;
            }
            else if (pTemplateSpecialization->getTemplated() == nullptr)
            {
                pTemplatedType = (New<TemplateDependantTemplateInstance>(a_pInput, a_Arguments));
            }
            else
                pTemplatedType = pTemplateSpecialization->getTemplated()->asType();
        }
        else /// Declaration inside a template declaration => we just look for any instance matching
             /// the template
        {
            pTemplatedType = New<TemplateDependantTemplateInstance>(a_pInput, a_Arguments);
        }
        return pTemplatedType;
    }
}

Conversion* Semantic::userDefinedConversion(Type* a_pInput, Type* a_pOutput, bool a_bExplicit, bool a_bInitialize,
                                            LanguageElement* a_pContextScope /*= nullptr*/,
                                            ERefType         a_RefType /*= e_RefType_None*/)
{
    ConversionResults possibleConversions;
    ClassType*        pInputClassType = a_pInput->removeQualifiers()->asClassType();
    ClassType*        pOutputClassType = a_pOutput->removeQualifiers()->asClassType();
    if (pOutputClassType)
    {
        Conversion* byConstruction = userDefinedConversionByConstruction(a_pInput, pOutputClassType, a_bExplicit,
                                                                         a_bInitialize, a_pContextScope, a_RefType);
        if (byConstruction)
            possibleConversions.push_back(byConstruction);
    }
    if (pInputClassType)
    {
        Conversion* byConversionFunction;
        if (Class* pInputClass = a_pInput->asClass())
        {
            byConversionFunction =
            userDefinedConversionByConversionFunction(pInputClass, a_pOutput, a_bExplicit, a_pContextScope, a_RefType);
        }
        else
        {
            byConversionFunction = userDefinedConversionByConversionFunction(pInputClassType, a_pOutput, a_bExplicit,
                                                                             a_pContextScope, a_RefType);
        }
        if (byConversionFunction)
            possibleConversions.push_back(byConversionFunction);
    }
    size_t                     index = 0;
    DefaultConversionSequence* pBest = static_cast<DefaultConversionSequence*>(possibleConversions.takeBest(&index));
    if (pBest)
    {
        if (pBest->semantic)
            pBest = pBest->clone(getSource());
        if (a_RefType == e_RefType_LValue)
            pBest->addConstRef();
        else if (a_RefType == e_RefType_RValue)
            pBest->addRValueRef();
        possibleConversions.destroy(getSource());
        return pBest;
    }
    possibleConversions.destroy(getSource());
    return nullptr;
}
Conversion* Semantic::userDefinedConversionByConstruction(Type* a_pInput, ClassType* a_pOutput, bool a_bExplicits,
                                                          bool a_bInitialize, LanguageElement* a_pContextScope,
                                                          ERefType a_RefType)
{
    Constructors      constructors;
    ConversionResults convs;
    Symbols           candidates;
    if (a_bExplicits)
    {
        for (Constructor* pCtor : a_pOutput->getConstructors())
        {
            if (pCtor->getRequiredArgumentCount() == 1)
            {
                candidates.push_back(pCtor);
            }
        }
        for (Template* pTpl : a_pOutput->getTemplates())
        {
            if (pTpl->getName() == a_pOutput->getName()) // template constructors
            {
                if (auto pES = pTpl->getEmptyTemplateSpecialization())
                    if (auto pTpled = pES->getTemplated())
                        candidates.push_back(pTpled);
            }
        }
    }
    else
    {
        for (Constructor* pCtor : a_pOutput->getConstructors())
        {
            if (pCtor->getRequiredArgumentCount() == 1 && !pCtor->testModifiers(PHANTOM_R_EXPLICIT))
            {
                candidates.push_back(pCtor);
            }
        }

        for (Template* pTpl : a_pOutput->getTemplates())
        {
            if (auto pES = pTpl->getEmptyTemplateSpecialization())
                if (auto pTpled = pES->getTemplated())
                    if (pTpled->getName() == a_pOutput->getName() &&
                        !pTpled->testModifiers(PHANTOM_R_EXPLICIT)) // template non explicit constructors
                    {
                        candidates.push_back(pTpled);
                    }
        }
    }

    Types types;
    types.push_back(a_pInput);

    SelectedOverloadInfos viableOverloads;
    findCallOverloads(viableOverloads, candidates, NullOpt, types, a_pContextScope,
                      a_bInitialize ? UserDefinedFunctions::ImplicitsOnly : UserDefinedFunctions::None, Modifiers{}, 0);

    auto pPerfectMatch = findPerfectMatchOverload(viableOverloads);
    auto pBestOverload = pPerfectMatch;
    if (pPerfectMatch == nullptr)
    {
        applyOverloadsSFINAE(viableOverloads, a_pContextScope);
        pBestOverload = selectBestOverload(viableOverloads);
    }
    if (pBestOverload)
    {
        Symbol* pBestCandidateAccessibleSymbol = pBestOverload->subroutine->getTemplateSpecialization();
        if (!pBestCandidateAccessibleSymbol)
            pBestCandidateAccessibleSymbol = pBestOverload->subroutine;
        else
            pBestCandidateAccessibleSymbol =
            static_cast<TemplateSpecialization*>(pBestCandidateAccessibleSymbol)->getTemplate();

        if (m_bAccessModifiersEnabled && m_pSource)
        {
            CxxSemanticErrorIfInaccessible(pBestCandidateAccessibleSymbol, a_pContextScope, nullptr);
        }
        PHANTOM_SEMANTIC_ASSERT(pBestOverload->subroutine->asConstructor());
        //         if (!pBestOverload->subroutine->isNative() && !pBestOverload->subroutine->buildBlock())
        //         {
        //             if (m_pLastSilentMessage)
        //             {
        //                 m_pMessage->addChild(m_pLastSilentMessage->clone(true));
        //                 m_pLastSilentMessage = nullptr;
        //             }
        //             // failed to instantiate templated block
        //             return nullptr;
        //         }

        auto pCtor = static_cast<Constructor*>(pBestOverload->subroutine);
        if (!pCtor->testModifiers(Modifier::Deleted))
        {
            DefaultConversionSequence* pBest =
            static_cast<DefaultConversionSequence*>(pBestOverload->conversions[0]->clone(getSource()));
            pBest->addUserDefinedByConstruction(pCtor);
            return pBest;
        }
        else
        {
            CxxSemanticError("'%.*s' : attempt to invoke a deleted constructor",
                             PHANTOM_STRING_AS_PRINTF_ARG(pCtor->getName()));
        }
    }
    return nullptr;
}

Conversion* Semantic::userDefinedConversionByConversionFunction(Class* a_pInput, Type* a_pOutput, bool a_bExplicit,
                                                                LanguageElement* a_pContextScope, ERefType a_RefType)
{
    Classes     allBases{a_pInput};
    Conversion* conv = nullptr;
    for (size_t i = 0; i < allBases.size(); ++i)
    {
        Class* pBaseClass = allBases[i];
        if (conv = userDefinedConversionByConversionFunction(static_cast<ClassType*>(pBaseClass), a_pOutput,
                                                             a_bExplicit, a_pContextScope, a_RefType))
        {
            if (allBases[i] != a_pInput)
            {
                size_t level = a_pInput->getInheritanceLevelFromBase(pBaseClass);
                PHANTOM_SEMANTIC_ASSERT(level != ~size_t(0));
                {
                    ptrdiff_t offset = a_pInput->getBaseClassOffsetCascade(allBases[i]);
                    if (offset != 0)
                    {
                        DefaultConversionSequence* seq = static_cast<DefaultConversionSequence*>(conv);
                        PHANTOM_SEMANTIC_ASSERT(seq->m_user_defined->m_standard);
                        PHANTOM_SEMANTIC_ASSERT(seq->m_user_defined->m_standard->m_numeric_conversion == nullptr);
                        seq->m_user_defined->m_standard->m_numeric_conversion =
                        newConv<PointerConversion>(a_pInput, pBaseClass, level, offset);
                    }
                }
            }
            conv->input = a_pInput;
            return conv;
        }
        // FIXME : handle base class access
        for (auto bc : pBaseClass->getBaseClasses())
        {
            allBases.push_back(bc);
        }
    }
    return nullptr;
}

Conversion* Semantic::userDefinedConversionByConversionFunction(ClassType* a_pInput, Type* a_pOutput, bool a_bExplicit,
                                                                LanguageElement* a_pContextScope, ERefType a_RefType)
{
    Methods           conversionFunctions;
    ConversionResults convs;
    if (a_bExplicit)
    {
        for (Method* pMethod : a_pInput->getMethods())
        {
            StringView name = pMethod->getName();
            if (name.size() >= 8 && name[8] == ' ') // TODO : find a faster way to distinguish conversion functions
            {
                Conversion* conv = nullptr;
                if (pMethod->getReturnType()->isSame(a_pOutput))
                {
                    conv = newConv<DefaultConversionSequence>(ConversionVal(a_pInput),
                                                              ConversionVal(a_pOutput)); /// identity
                }
                else
                {
                    conv = newConversion(pMethod->getReturnType(), a_pOutput, CastKind::Implicit,
                                         UserDefinedFunctions::None, a_pContextScope);
                }
                if (conv)
                {
                    conversionFunctions.push_back(pMethod);
                    convs.push_back(conv);
                }
            }
        }
    }
    else /// implicits
    {
        for (Method* pMethod : a_pInput->getMethods())
        {
            StringView name = pMethod->getName();
            if (!(pMethod->testModifiers(PHANTOM_R_EXPLICIT)) && name.size() >= 8 &&
                name[8] == ' ') // TODO : find a faster way to distinguish conversion functions
            {
                Conversion* conv = nullptr;
                if (pMethod->getReturnType()->isSame(a_pOutput))
                {
                    conv = newConv<DefaultConversionSequence>(a_pOutput); /// identity
                }
                else
                {
                    conv = newConversion(pMethod->getReturnType(), a_pOutput, CastKind::Implicit,
                                         UserDefinedFunctions::None, a_pContextScope);
                }
                if (conv)
                {
                    conversionFunctions.push_back(pMethod);
                    convs.push_back(conv);
                }
            }
        }
    }
    size_t                     index = 0;
    DefaultConversionSequence* pBest = static_cast<DefaultConversionSequence*>(convs.takeBest(&index));
    if (pBest)
    {
        if (pBest->semantic)
            pBest = pBest->clone(getSource());
        pBest->addUserDefinedByConversionFunction(conversionFunctions[index]);
        /// the best found is in fact the best second standard conversion sequence of an user
        /// defined conversion sequence by conversion function so we switch first standard and
        /// second standard in current conversion sequence
        PHANTOM_SEMANTIC_ASSERT(pBest->m_user_defined->m_standard == nullptr);
        pBest->m_user_defined->m_standard = pBest->m_standard;
        pBest->m_standard = nullptr;
        convs.destroy(getSource());
    }
    return pBest;
}

Block* Semantic::addBlock(Subroutine* a_pSubroutine, bool a_EnsureTemplateInstanceComplete)
{
    if (a_EnsureTemplateInstanceComplete)
    {
        Block* pBlock = a_pSubroutine->New<Block>();
        a_pSubroutine->setBlock(pBlock);
        size_t count = a_pSubroutine->getSignature()->getParameterCount();
        for (size_t i = 0; i < count; ++i)
        {
            Parameter* pParam = a_pSubroutine->getParameters()[i];
            auto       pValueType = pParam->getValueType();
            if (!pValueType->isNative())
            {
                if (ClassType* pClassType = pValueType->removeQualifiers()->asClassType())
                {
                    buildClass(pClassType, e_ClassBuildState_Blocks);
                }
            }
            pBlock->addLocalVariable(a_pSubroutine->getSignature()->getParameter(i));
        }
        return pBlock;
    }
    else
    {
        return AddBlock(a_pSubroutine);
    }
}

Block* Semantic::addBlock(Method* a_pMethod, bool a_EnsureTemplateInstanceComplete)
{
    if (a_EnsureTemplateInstanceComplete)
    {
        Block* pBlock = a_pMethod->New<Block>();
        a_pMethod->setBlock(pBlock);
        pBlock->addLocalVariable(a_pMethod->getThis());
        size_t count = a_pMethod->getSignature()->getParameterCount();
        for (size_t i = 0; i < count; ++i)
        {
            Parameter* pParam = a_pMethod->getParameters()[i];
            auto       pValueType = pParam->getValueType();
            if (!pValueType->isNative())
            {
                if (ClassType* pClassType = pValueType->removeQualifiers()->asClassType())
                {
                    buildClass(pClassType, e_ClassBuildState_Blocks);
                }
            }
            pBlock->addLocalVariable(pParam);
        }
        return pBlock;
    }
    else
    {
        return AddBlock(a_pMethod);
    }
}

Block* Semantic::AddBlock(Method* a_pMethod)
{
    Block* pBlock = a_pMethod->New<Block>();
    a_pMethod->setBlock(pBlock);
    pBlock->addLocalVariable(a_pMethod->getThis());
    size_t count = a_pMethod->getSignature()->getParameterCount();
    for (size_t i = 0; i < count; ++i)
    {
        pBlock->addLocalVariable(a_pMethod->getSignature()->getParameter(i));
    }
    return pBlock;
}

Block* Semantic::AddBlock(Subroutine* a_pSubroutine)
{
    Block* pBlock = a_pSubroutine->New<Block>();
    a_pSubroutine->setBlock(pBlock);
    size_t count = a_pSubroutine->getSignature()->getParameterCount();
    for (size_t i = 0; i < count; ++i)
    {
        pBlock->addLocalVariable(a_pSubroutine->getSignature()->getParameter(i));
    }
    return pBlock;
}

void Semantic::newImplicitConversions(Signature* a_pSignature, TypesView a_ArgTypes, ConversionResults& a_Out,
                                      LanguageElement*     a_pContextScope,
                                      UserDefinedFunctions a_ConversionAllowedUserDefinedFunctions)
{
    size_t paramCount = a_pSignature->getParameters().size();
    size_t argCount = a_ArgTypes.size();
    if ((argCount < a_pSignature->getRequiredArgumentCount()) ||
        (argCount > paramCount && !(a_pSignature->isVariadic())))
        return;
    size_t i = 0;
    for (; i < argCount; ++i)
    {
        Type* pParamType = a_pSignature->getParameters()[i]->getValueType();
        Type* pInputType =
        (i < a_ArgTypes.size()) ? a_ArgTypes[i] : a_pSignature->getParameterDefaultValueExpression(i)->getValueType();
        a_Out.push_back(
        newConversion(pInputType, pParamType, a_pContextScope, a_ConversionAllowedUserDefinedFunctions, false));
    }
}

void Semantic::newImplicitConversions(Signature* a_pSignature, ExpressionsView a_Args, ConversionResults& a_Out,
                                      LanguageElement*     a_pContextScope,
                                      UserDefinedFunctions a_ConversionAllowedUserDefinedFunctions)
{
    Types types;
    for (auto& arg : a_Args)
    {
        types.push_back(arg->getValueType());
    }
    newImplicitConversions(a_pSignature, types, a_Out, a_pContextScope, a_ConversionAllowedUserDefinedFunctions);
}

void Semantic::newImplicitConversions(FunctionType* a_pFuncType, TypesView a_ArgTypes, ConversionResults& a_Out,
                                      LanguageElement*     a_pContextScope,
                                      UserDefinedFunctions a_ConversionAllowedUserDefinedFunctions)
{
    size_t paramCount = a_pFuncType->getParameterTypes().size();
    size_t argCount = a_ArgTypes.size();
    if ((argCount != paramCount))
        return;
    size_t i = 0;
    for (; i < argCount; ++i)
    {
        Type* pParamType = a_pFuncType->getParameterTypes()[i];
        a_Out.push_back(
        newConversion(a_ArgTypes[i], pParamType, a_pContextScope, a_ConversionAllowedUserDefinedFunctions, false));
    }
}

void Semantic::newImplicitConversions(FunctionType* a_pFuncType, ExpressionsView a_Args, ConversionResults& a_Out,
                                      LanguageElement*     a_pContextScope,
                                      UserDefinedFunctions a_ConversionAllowedUserDefinedFunctions)
{
    Types types;
    for (auto& arg : a_Args)
    {
        types.push_back(arg->getValueType());
    }
    newImplicitConversions(a_pFuncType, types, a_Out, a_pContextScope, a_ConversionAllowedUserDefinedFunctions);
}
void Semantic::newImplicitConversionsWithArgDeductions(Signature* a_pSignature, TypesView a_ArgTypes,
                                                       ConversionResults&    a_Out,
                                                       TemplateSubstitution& a_TemplateArgDeductions,
                                                       LanguageElement*      a_pContextScope,
                                                       UserDefinedFunctions  a_ConversionAllowedUserDefinedFunctions)
{
    size_t paramCount = a_pSignature->getParameters().size();
    size_t argCount = a_ArgTypes.size();
    if ((argCount < a_pSignature->getRequiredArgumentCount()) ||
        (argCount > paramCount && !(a_pSignature->isVariadic())))
        return;
    auto&                  params = a_pSignature->getParameters();
    size_t                 i = 0;
    SmallSet<Placeholder*> deduced;
    for (; i < argCount; ++i)
    {
        Type* pParamType = params[i]->getValueType();
        Type* pInputType = a_ArgTypes[i];
        if (pParamType->isTemplateDependant())
        {
            if (pParamType->getMetaClass() == PHANTOM_CLASSOF(TemplateParameterPackTypeExpansion))
            {
                // template pack param expansion

                // we treat remaining arguments as variadic ones
                for (; i < argCount; ++i)
                {
                    PlaceholderMap deductions;
                    auto pResolvedVariadicType = callTemplateArgumentDeductionRef(pParamType, pInputType, deductions);
                    for (auto& ph_deductions : deductions)
                    {
                        for (auto& deduction : ph_deductions.second)
                            a_TemplateArgDeductions.insert(ph_deductions.first, deduction);
                    }
                    a_Out.push_back(newConversion(pInputType, pResolvedVariadicType, a_pContextScope,
                                                  a_ConversionAllowedUserDefinedFunctions, false));
                }
                break;
            }
            Type* pParamTypeResolvedOrDeduced = nullptr;
            if (a_TemplateArgDeductions.size())
            {
                pParamTypeResolvedOrDeduced = static_cast<Type*>(resolveTemplateDependency(
                pParamType, a_TemplateArgDeductions, e_ClassBuildState_Members, a_pContextScope, 0));
            }
            if (!pParamTypeResolvedOrDeduced)
            {
                PlaceholderMap deductions;
                pParamTypeResolvedOrDeduced = callTemplateArgumentDeductionRef(pParamType, pInputType, deductions);

                for (auto& ph_deductions : deductions)
                {
                    for (auto& deduction : ph_deductions.second)
                        a_TemplateArgDeductions.insert(ph_deductions.first, deduction);
                }

                if (!pParamTypeResolvedOrDeduced)
                {
                    a_Out.push_back(nullptr);
                }
                else
                {
                    a_Out.push_back(newConversion(pInputType, pParamTypeResolvedOrDeduced, a_pContextScope,
                                                  a_ConversionAllowedUserDefinedFunctions, false));
                }
            }
            else
                a_Out.push_back(newConversion(pInputType, pParamTypeResolvedOrDeduced, a_pContextScope,
                                              a_ConversionAllowedUserDefinedFunctions, false));
        }
        else
        {
            a_Out.push_back(
            newConversion(pInputType, pParamType, a_pContextScope, a_ConversionAllowedUserDefinedFunctions, false));
        }
    }
}

void Semantic::newImplicitConversionsWithArgDeductions(Signature* a_pSignature, ExpressionsView a_Args,
                                                       ConversionResults&    a_Out,
                                                       TemplateSubstitution& a_TemplateArgDeductions,
                                                       LanguageElement*      a_pContextScope,
                                                       UserDefinedFunctions  a_ConversionAllowedUserDefinedFunctions)
{
    Types types;
    for (auto& arg : a_Args)
    {
        types.push_back(arg->getValueType());
    }
    newImplicitConversionsWithArgDeductions(a_pSignature, types, a_Out, a_TemplateArgDeductions, a_pContextScope,
                                            a_ConversionAllowedUserDefinedFunctions);
}

void Semantic::newImplicitConversionsWithArgDeductions(FunctionType* a_pFuncType, TypesView a_ArgTypes,
                                                       ConversionResults&    a_Out,
                                                       TemplateSubstitution& a_TemplateArgDeductions,
                                                       LanguageElement*      a_pContextScope,
                                                       UserDefinedFunctions  a_ConversionAllowedUserDefinedFunctions)
{
    size_t                 paramCount = a_pFuncType->getParameterTypeCount();
    size_t                 argCount = a_ArgTypes.size();
    auto&&                 types = a_pFuncType->getParameterTypes();
    size_t                 i = 0;
    SmallSet<Placeholder*> deduced;
    for (; i < argCount; ++i)
    {
        Type* pParamType = types[i];
        Type* pInputType = a_ArgTypes[i];
        if (pParamType->isTemplateDependant())
        {
            if (pParamType->getMetaClass() == PHANTOM_CLASSOF(TemplateParameterPackTypeExpansion))
            {
                // template pack param expansion

                // we treat remaining arguments as variadic ones
                for (; i < argCount; ++i)
                {
                    PlaceholderMap deductions;
                    auto pResolvedVariadicType = callTemplateArgumentDeductionRef(pParamType, pInputType, deductions);
                    for (auto& ph_deductions : deductions)
                    {
                        for (auto& deduction : ph_deductions.second)
                            a_TemplateArgDeductions.insert(ph_deductions.first, deduction);
                    }
                    a_Out.push_back(newConversion(pInputType, pResolvedVariadicType, a_pContextScope,
                                                  a_ConversionAllowedUserDefinedFunctions, false));
                }
                break;
            }
            Type* pParamTypeResolvedOrDeduced = nullptr;
            if (a_TemplateArgDeductions.size())
            {
                pParamTypeResolvedOrDeduced = static_cast<Type*>(resolveTemplateDependency(
                pParamType, a_TemplateArgDeductions, e_ClassBuildState_Members, a_pContextScope, 0));
            }
            if (!pParamTypeResolvedOrDeduced)
            {
                PlaceholderMap deductions;
                pParamTypeResolvedOrDeduced = callTemplateArgumentDeductionRef(pParamType, pInputType, deductions);

                for (auto& ph_deductions : deductions)
                {
                    for (auto& deduction : ph_deductions.second)
                        a_TemplateArgDeductions.insert(ph_deductions.first, deduction);
                }

                if (!pParamTypeResolvedOrDeduced)
                {
                    a_Out.push_back(nullptr);
                }
                else
                {
                    a_Out.push_back(newConversion(pInputType, pParamTypeResolvedOrDeduced, a_pContextScope,
                                                  a_ConversionAllowedUserDefinedFunctions, false));
                }
            }
            else
                a_Out.push_back(newConversion(pInputType, pParamTypeResolvedOrDeduced, a_pContextScope,
                                              a_ConversionAllowedUserDefinedFunctions, false));
        }
        else
        {
            a_Out.push_back(
            newConversion(pInputType, pParamType, a_pContextScope, a_ConversionAllowedUserDefinedFunctions, false));
        }
    }
}

void Semantic::newImplicitConversionsWithArgDeductions(FunctionType* a_pFuncType, ExpressionsView a_Args,
                                                       ConversionResults&    a_Out,
                                                       TemplateSubstitution& a_TemplateArgDeductions,
                                                       LanguageElement*      a_pContextScope,
                                                       UserDefinedFunctions  a_ConversionAllowedUserDefinedFunctions)
{
    Types types;
    for (auto& arg : a_Args)
    {
        types.push_back(arg->getValueType());
    }
    newImplicitConversionsWithArgDeductions(a_pFuncType, types, a_Out, a_TemplateArgDeductions, a_pContextScope,
                                            a_ConversionAllowedUserDefinedFunctions);
}

Expression* Semantic::_createFieldCopyConstruction(Class* a_pThis, Class* a_pClass, Expression* a_pWhere)
{
    Constructor* pCtor = a_pClass->getCopyConstructor();
    PHANTOM_SEMANTIC_ASSERT(pCtor);
    PHANTOM_SEMANTIC_ASSERT(
    !(pCtor == nullptr || (pCtor->isPrivate() || pCtor->isProtected()) && !(a_pClass->hasFriend(a_pThis))));
    return New<ConstructorCallExpression>(pCtor, a_pWhere);
}

Expression* Semantic::_createFieldCopyConstruction(Class* a_pThis, Array* a_pArray, Expression* a_pWhere)
{
    Class*      pClass = a_pArray->getItemType()->asClass();
    Type*       pPointer = a_pWhere->getValueType()->removeReference()->removeArray()->addPointer();
    Expressions exps;
    for (size_t i = 0; i < a_pArray->getItemCount(); ++i)
    {
        Expression* pAdjust = nullptr;
        Expression* pPointerExp = convert(a_pWhere->clone(getSource()), pPointer);
        PHANTOM_SEMANTIC_ASSERT(pPointerExp);
        if (i == 0)
        {
            pAdjust = New<IdentityExpression>(a_pArray->getItemType()->addLValueReference(), pPointerExp);
        }
        else
        {
            pAdjust = New<PointerAdjustmentExpression>(a_pArray->getItemType()->addLValueReference(), pPointerExp,
                                                       i * a_pArray->getItemType()->getSize());
        }
        exps.push_back(pAdjust);
    }
    Delete(a_pWhere);
    if (pClass)
    {
        Constructor* pCtor = pClass->getCopyConstructor();
        PHANTOM_SEMANTIC_ASSERT(
        !(pCtor == nullptr || (pCtor->isPrivate() || pCtor->isProtected()) && !(pClass->hasFriend(a_pThis))));
        for (auto& exp : exps)
        {
            exp = New<ConstructorCallExpression>(pCtor, exp);
        }
    }
    else
    {
        for (auto& exp : exps)
        {
            exp = exp->load(getSource());
        }
    }
    return New<ArrayExpression>(a_pArray, exps);
}

void Semantic::_copyAssignField(Class* a_pThis, Block* a_pBlock, Expression* a_pDME, Class* a_pClass,
                                Expression* a_pWhere)
{
    Method* pCtor = a_pClass->getCopyAssignmentOperator();
    PHANTOM_SEMANTIC_ASSERT(
    !(pCtor == nullptr || (pCtor->isPrivate() || pCtor->isProtected()) && !(a_pClass->hasFriend(a_pThis))));
    if (pCtor)
    {
        a_pBlock->addStatement(solveBinaryOperator(Operator::Assignment, a_pDME, a_pWhere));
    }
}

void Semantic::_copyAssignField(Class* a_pThis, Block* a_pBlock, Expression* a_pDME, Array* a_pArray,
                                Expression* a_pWhere)
{
    Source* pSource = getSource();
    Class*  pClass = a_pArray->getItemType()->asClass();
    if (pClass)
    {
        Method* pCtor = pClass->getCopyAssignmentOperator();
        PHANTOM_SEMANTIC_ASSERT(
        !(pCtor == nullptr || (pCtor->isPrivate() || pCtor->isProtected()) && !(pClass->hasFriend(a_pThis))));
        for (size_t i = 0; i < a_pArray->getItemCount(); ++i)
        {
            auto pAdjustThis = a_pDME->clone(pSource)->adjust(pSource, i * a_pArray->getItemType()->getSize(),
                                                              a_pArray->getItemType()->addLValueReference());
            auto pAdjustOther = a_pWhere->clone(pSource)->adjust(pSource, i * a_pArray->getItemType()->getSize(),
                                                                 a_pArray->getItemType()->addLValueReference());
            a_pBlock->addStatement(solveBinaryOperator(Operator::Assignment, pAdjustThis, pAdjustOther));
        }
        Delete(a_pWhere);
        Delete(a_pDME);
    }
    else
    {
        for (size_t i = 0; i < a_pArray->getItemCount(); ++i)
        {
            auto pAdjustThis =
            a_pDME->clone(getSource())
            ->adjust(pSource, i * a_pArray->getItemType()->getSize(), a_pArray->getItemType()->addLValueReference());
            auto pAdjustOther =
            a_pWhere->clone(getSource())
            ->adjust(pSource, i * a_pArray->getItemType()->getSize(), a_pArray->getItemType()->addLValueReference());
            a_pBlock->addStatement(New<MemCopyStatement>(pAdjustThis->address(pSource), pAdjustOther->address(pSource),
                                                         a_pArray->getItemType()->getSize()));
        }
        Delete(a_pWhere);
        Delete(a_pDME);
    }
}

Expression* Semantic::_createFieldMoveConstruction(Class* a_pThis, Block* a_pBlock, Field* a_pDM, Class* a_pClass,
                                                   Expression* a_pWhere)
{
    Constructor* pCtor = a_pClass->getMoveConstructor();
    PHANTOM_SEMANTIC_ASSERT(pCtor);
    PHANTOM_SEMANTIC_ASSERT(
    !(pCtor == nullptr || (pCtor->isPrivate() || pCtor->isProtected()) && !(a_pClass->hasFriend(a_pThis))));
    return New<ConstructorCallExpression>(pCtor, a_pWhere);
}

Expression* Semantic::_createFieldMoveConstruction(Class* a_pThis, Block* a_pBlock, Field* a_pDM, Array* a_pArray,
                                                   Expression* a_pWhere)
{
    Class*      pClass = a_pArray->getItemType()->asClass();
    Type*       pPointer = a_pWhere->getValueType()->removeReference()->removeArray()->addPointer();
    Expressions exps;
    for (size_t i = 0; i < a_pArray->getItemCount(); ++i)
    {
        Expression* pAdjust = nullptr;
        Expression* pPointerExp = convert(a_pWhere->clone(getSource()), pPointer);
        if (i == 0)
        {
            pAdjust = New<IdentityExpression>(a_pArray->getItemType()->addRValueReference(), pPointerExp);
        }
        else
        {
            pAdjust = New<PointerAdjustmentExpression>(a_pArray->getItemType()->addRValueReference(), pPointerExp,
                                                       i * a_pArray->getItemType()->getSize());
        }
        exps.push_back(pAdjust);
    }
    Delete(a_pWhere);
    if (pClass)
    {
        Constructor* pCtor = pClass->getMoveConstructor();
        PHANTOM_SEMANTIC_ASSERT(
        !(pCtor == nullptr || (pCtor->isPrivate() || pCtor->isProtected()) && !(pClass->hasFriend(a_pThis))));
        for (auto& exp : exps)
        {
            exp = New<ConstructorCallExpression>(pCtor, exp);
        }
    }
    else
    {
        for (auto& exp : exps)
        {
            exp = exp->load(getSource());
        }
    }
    return New<ArrayExpression>(a_pArray, exps);
}

void Semantic::_moveAssignField(Class* a_pThis, Block* a_pBlock, Expression* a_pDME, Class* a_pClass,
                                Expression* a_pWhere)
{
    Method* pCtor = a_pClass->getMoveAssignmentOperator();
    PHANTOM_SEMANTIC_ASSERT(
    !(pCtor == nullptr || (pCtor->isPrivate() || pCtor->isProtected()) && !(a_pClass->hasFriend(a_pThis))));
    if (pCtor)
    {
        a_pBlock->addStatement(solveBinaryOperator(Operator::Assignment, a_pDME, a_pWhere));
    }
}

void Semantic::_moveAssignField(Class* a_pThis, Block* a_pBlock, Expression* a_pDME, Array* a_pArray,
                                Expression* a_pWhere)
{
    Source* pSource = getSource();
    Class*  pClass = a_pArray->getItemType()->asClass();
    if (pClass)
    {
        Method* pCtor = pClass->getMoveAssignmentOperator();
        PHANTOM_SEMANTIC_ASSERT(
        !(pCtor == nullptr || (pCtor->isPrivate() || pCtor->isProtected()) && !(pClass->hasFriend(a_pThis))));
        for (size_t i = 0; i < a_pArray->getItemCount(); ++i)
        {
            auto pAdjustThis = a_pDME->clone(pSource)->adjust(pSource, i * a_pArray->getItemType()->getSize(),
                                                              a_pArray->getItemType()->addLValueReference());
            auto pAdjustOther = a_pWhere->clone(pSource)->adjust(pSource, i * a_pArray->getItemType()->getSize(),
                                                                 a_pArray->getItemType()->addRValueReference());
            a_pBlock->addStatement(solveBinaryOperator(Operator::Assignment, pAdjustThis, pAdjustOther));
        }
        Delete(a_pWhere);
        Delete(a_pDME);
    }
    else
    {
        for (size_t i = 0; i < a_pArray->getItemCount(); ++i)
        {
            auto pAdjustThis = a_pDME->clone(pSource)->adjust(pSource, i * a_pArray->getItemType()->getSize(),
                                                              a_pArray->getItemType()->addLValueReference());
            auto pAdjustOther = a_pWhere->clone(pSource)->adjust(pSource, i * a_pArray->getItemType()->getSize(),
                                                                 a_pArray->getItemType()->addRValueReference());
            a_pBlock->addStatement(New<MemCopyStatement>(pAdjustThis->address(pSource), pAdjustOther->address(pSource),
                                                         a_pArray->getItemType()->getSize()));
        }
        Delete(a_pWhere);
        Delete(a_pDME);
    }
}

void Semantic::generateImplicitCopyAssignmentOperatorCode(Class* a_pClass)
{
    Source* pSource = getSource();
    Method* pCopyAssignmentOperator = a_pClass->getCopyAssignmentOperator();
    if (pCopyAssignmentOperator == nullptr)
    {
        PHANTOM_SEMANTIC_ASSERT(!(a_pClass->canHaveImplicitCopyAssignmentOperator()));
    }
    else if (pCopyAssignmentOperator->testFlags(PHANTOM_R_FLAG_IMPLICIT) ||
             pCopyAssignmentOperator->testModifiers(PHANTOM_R_DEFAULTED))
    {
        Block*                   pBlock = addBlock(pCopyAssignmentOperator);
        LocalVariable*           pThis = pBlock->getLocalVariable("this");
        Expression*              pThisExpr = New<LocalVariableExpression>(pThis);
        LocalVariableExpression* pParamExpression =
        New<LocalVariableExpression>(pCopyAssignmentOperator->getParameters().front());

        {
            for (auto& bc : a_pClass->getBaseClasses())
            {
                Method* pBaseCopyOp = bc.baseClass->getCopyAssignmentOperator();
                PHANTOM_SEMANTIC_ASSERT(
                !(pBaseCopyOp == nullptr || (pBaseCopyOp->isPrivate() && !(bc.baseClass->hasFriend(a_pClass)))));
                if (pBaseCopyOp)
                {
                    Expressions args;
                    args.push_back(convert(pThisExpr->clone(pSource), bc.baseClass->makePointer()));
                    args.push_back(
                    convert(pParamExpression->clone(pSource), pBaseCopyOp->getParameters().front()->getValueType()));
                    pBlock->addStatement(New<CallExpression>(pBaseCopyOp, args));
                }
            }
        }
        for (Field* pDM : a_pClass->getFields())
        {
            Type*  pType = pDM->getValueType()->removeQualifiers();
            Class* pClass = pType->asClass();
            Array* pArray;
            if (pClass)
            {
                _copyAssignField(a_pClass, pBlock, toExpression(pDM, pThisExpr->clone(pSource)->dereference(pSource)),
                                 pClass, toExpression(pDM, pParamExpression->clone(pSource)));
            }
            else if (pArray = pType->asArray())
            {
                _copyAssignField(a_pClass, pBlock, toExpression(pDM, pThisExpr->clone(pSource)->dereference(pSource)),
                                 pArray, toExpression(pDM, pParamExpression->clone(pSource)));
            }
            else
            {
                Expression* pAss =
                solveBinaryOperator("=", toExpression(pDM, pThisExpr->clone(pSource)->dereference(pSource)),
                                    toExpression(pDM, pParamExpression->clone(pSource)));
                PHANTOM_SEMANTIC_ASSERT(pAss);
                pBlock->addStatement(pAss);
            }
        }

        Delete(pParamExpression);
        Delete(pThisExpr);
    }
}

void Semantic::generateImplicitCopyConstructorCode(Class* a_pClass)
{
    Source*      pSource = getSource();
    Constructor* pCopyConstructor = a_pClass->getCopyConstructor();
    if (pCopyConstructor == nullptr)
    {
        PHANTOM_SEMANTIC_ASSERT(!(a_pClass->canHaveImplicitCopyConstructor()));
    }
    else if (pCopyConstructor->testFlags(PHANTOM_R_FLAG_IMPLICIT) ||
             pCopyConstructor->testModifiers(PHANTOM_R_DEFAULTED))
    {
        Block*         pBlock = addBlock(pCopyConstructor);
        LocalVariable* pThis = pBlock->getLocalVariable("this");
        Expression*    pThisExpr = New<LocalVariableExpression>(pThis);
        Expression*    pParamExpression = toExpression(pCopyConstructor->getParameters().front());

        {
            for (auto& bc : a_pClass->getBaseClasses())
            {
                Constructor* pBaseCopyCtor = bc.baseClass->getCopyConstructor();
                PHANTOM_SEMANTIC_ASSERT(
                !(pBaseCopyCtor == nullptr || (pBaseCopyCtor->isPrivate() && !(bc.baseClass->hasFriend(a_pClass)))));
                if (pBaseCopyCtor)
                {
                    pBlock->addStatement(New<BaseConstructorCallStatement>(
                    a_pClass,
                    New<ConstructorCallExpression>(pBaseCopyCtor,
                                                   convert(pParamExpression->clone(pSource),
                                                           pBaseCopyCtor->getParameters().front()->getValueType()))));
                }
            }
        }

        for (auto pDM : a_pClass->getFields())
        {
            Type*       pType = pDM->getValueType()->removeQualifiers();
            Class*      pClass = pType->asClass();
            Array*      pArray;
            Expression* pInitCopyExpr;
            if (pClass)
            {
                pInitCopyExpr =
                _createFieldCopyConstruction(a_pClass, pClass, toExpression(pDM, pParamExpression->clone(pSource)));
            }
            else if (pArray = pType->asArray())
            {
                pInitCopyExpr =
                _createFieldCopyConstruction(a_pClass, pArray, toExpression(pDM, pParamExpression->clone(pSource)));
            }
            else
            {
                pInitCopyExpr = toExpression(pDM, pParamExpression->clone(pSource));
                if (pDM->getValueType()->asReference() == nullptr)
                    pInitCopyExpr = pInitCopyExpr->load(pSource);
            }
            pBlock->addStatement(New<FieldInitializationStatement>(pDM, pInitCopyExpr));
        }
    }
}

void Semantic::generateImplicitMoveConstructorCode(Class* a_pClass)
{
    Source*      pSource = getSource();
    Constructor* pMoveConstructor = a_pClass->getMoveConstructor();
    if (pMoveConstructor == nullptr)
    {
        PHANTOM_SEMANTIC_ASSERT(!(a_pClass->canHaveImplicitMoveConstructor()));
    }
    else if (pMoveConstructor->testFlags(PHANTOM_R_FLAG_IMPLICIT) ||
             pMoveConstructor->testModifiers(PHANTOM_R_DEFAULTED))
    {
        Block*                   pBlock = addBlock(pMoveConstructor);
        LocalVariable*           pThis = pBlock->getLocalVariable("this");
        Expression*              pThisExpr = New<LocalVariableExpression>(pThis);
        LocalVariableExpression* pParamExpression =
        New<LocalVariableExpression>(pMoveConstructor->getParameters().front());

        {
            for (auto& bc : a_pClass->getBaseClasses())
            {
                Constructor* pBaseMoveCtor = bc.baseClass->getMoveConstructor();
                PHANTOM_SEMANTIC_ASSERT(
                !(pBaseMoveCtor == nullptr || (pBaseMoveCtor->isPrivate() && !(bc.baseClass->hasFriend(a_pClass)))));
                if (pBaseMoveCtor)
                {
                    pBlock->addStatement(New<BaseConstructorCallStatement>(
                    a_pClass,
                    New<ConstructorCallExpression>(pBaseMoveCtor,
                                                   convert(pParamExpression->clone(pSource),
                                                           pBaseMoveCtor->getParameters().front()->getValueType(),
                                                           CastKind::Explicit))));
                }
            }
        }

        for (auto pDM : a_pClass->getFields())
        {
            Type*       pType = pDM->getValueType()->removeQualifiers();
            Class*      pClass = pType->asClass();
            Array*      pArray;
            Expression* pDMExpr = toExpression(pDM, pParamExpression->clone(pSource));
            Expression* pInitMoveExpr;
            if (pClass)
            {
                Expression* pWhere =
                convert(pDMExpr, pDMExpr->getValueType()->removeReference()->removeQualifiers()->addRValueReference(),
                        CastKind::Explicit);
                PHANTOM_SEMANTIC_ASSERT(pWhere);
                pInitMoveExpr = _createFieldMoveConstruction(a_pClass, pBlock, pDM, pClass, pWhere);
            }
            else if (pArray = pType->asArray())
            {
                Expression* pWhere =
                convert(pDMExpr, pDMExpr->getValueType()->removeReference()->removeQualifiers()->addRValueReference(),
                        CastKind::Explicit);
                PHANTOM_SEMANTIC_ASSERT(pWhere);
                pInitMoveExpr = _createFieldMoveConstruction(a_pClass, pBlock, pDM, pArray, pWhere);
            }
            else
            {
                pInitMoveExpr = pDMExpr;
                if (pDM->getValueType()->asReference() == nullptr)
                    pInitMoveExpr = pInitMoveExpr->load(pSource);
            }
            pBlock->addStatement(New<FieldInitializationStatement>(pDM, pInitMoveExpr));
        }
    }
}

void Semantic::generateImplicitMoveAssignmentOperatorCode(Class* a_pClass)
{
    Source* pSource = getSource();

    Method* pMoveAssignmentOperator = a_pClass->getMoveAssignmentOperator();
    if (pMoveAssignmentOperator == nullptr)
    {
        PHANTOM_SEMANTIC_ASSERT(!(a_pClass->canHaveImplicitMoveAssignmentOperator()));
    }
    else if (pMoveAssignmentOperator->testFlags(PHANTOM_R_FLAG_IMPLICIT) ||
             pMoveAssignmentOperator->testModifiers(PHANTOM_R_DEFAULTED))
    {
        Block*                   pBlock = addBlock(pMoveAssignmentOperator);
        LocalVariable*           pThis = pBlock->getLocalVariable("this");
        Expression*              pThisExpr = New<LocalVariableExpression>(pThis);
        LocalVariableExpression* pParamExpression =
        New<LocalVariableExpression>(pMoveAssignmentOperator->getParameters().front());
        {
            for (auto& bc : a_pClass->getBaseClasses())
            {
                Method* pBaseMoveOp = bc.baseClass->getMoveAssignmentOperator();
                PHANTOM_SEMANTIC_ASSERT(
                !(pBaseMoveOp == nullptr || (pBaseMoveOp->isPrivate() && !(bc.baseClass->hasFriend(a_pClass)))));
                if (pBaseMoveOp)
                {
                    Expressions args;
                    args.push_back(convert(pThisExpr->clone(pSource), bc.baseClass->makePointer()));
                    args.push_back(convert(pParamExpression->clone(pSource),
                                           pBaseMoveOp->getParameters().front()->getValueType(), CastKind::Explicit));
                    pBlock->addStatement(New<CallExpression>(pBaseMoveOp, args));
                }
            }
        }

        for (auto pDM : a_pClass->getFields())
        {
            Type*       pType = pDM->getValueType()->removeQualifiers();
            Class*      pClass = pType->asClass();
            Array*      pArray;
            Expression* pParamDMExpr = toExpression(pDM, pParamExpression->clone(pSource));
            Expression* pThisDMExpr = toExpression(pDM, pThisExpr->clone(pSource)->dereference(pSource));
            if (pClass)
            {
                _moveAssignField(a_pClass, pBlock, pThisDMExpr, pClass,
                                 convert(pParamDMExpr, pDM->getValueType()->addRValueReference(), CastKind::Explicit));
            }
            else if (pArray = pType->asArray())
            {
                _moveAssignField(a_pClass, pBlock, pThisDMExpr, pArray,
                                 convert(pParamDMExpr, pDM->getValueType()->addRValueReference(), CastKind::Explicit));
            }
            else
            {
                pBlock->addStatement(solveBinaryOperator("=", pThisDMExpr, pParamDMExpr));
            }
        }

        Delete(pParamExpression);
        Delete(pThisExpr);
    }
}

void Semantic::generateImplicitDefaultConstructorCode(Class* a_pClass)
{
    Constructor* pDefaultContructor = a_pClass->getDefaultConstructor();
    if (pDefaultContructor == nullptr)
    {
        PHANTOM_SEMANTIC_ASSERT(!(a_pClass->canHaveImplicitDefaultConstructor()));
    }
    else if (pDefaultContructor->testFlags(PHANTOM_R_FLAG_IMPLICIT) ||
             pDefaultContructor->testModifiers(PHANTOM_R_DEFAULTED))
    {
        addBlock(pDefaultContructor);
    }
}

Expression* Semantic::createDestructionExpression(Type* a_pType, Expression* a_pObjExpression,
                                                  LanguageElement* a_pScope)
{
    switch (a_pType->getTypeKind())
    {
    case TypeKind::MapClass:
    case TypeKind::StringClass:
    case TypeKind::VectorClass:
    case TypeKind::SetClass:
    case TypeKind::ArrayClass:
    case TypeKind::Class:
    {
        PHANTOM_SEMANTIC_ASSERT(a_pType->asClass());
        Class* pClass = static_cast<Class*>(a_pType);
        return createCallExpression(
        pClass->getDestructor(),
        convert(a_pObjExpression, a_pObjExpression->getValueType()->removeAllQualifiers(), CastKind::Explicit),
        Expressions{}, a_pScope);
    }
    default:
        return nullptr;
    }
}

Expression* Semantic::createDestructionExpression(Field* a_pDM, Expression* a_pObjExpression, LanguageElement* a_pScope)
{
    return createDestructionExpression(a_pDM->getValueType()->removeQualifiers(), toExpression(a_pDM, a_pObjExpression),
                                       a_pScope);
}

void Semantic::generateImplicitDestructorCode(Class* a_pClass)
{
    Source* pSource = getSource();

    Method* pDestructor = a_pClass->getDestructor();

    if (pDestructor)
    {
        Block* pDestructorBlock = pDestructor->getBlock();
        if (pDestructorBlock == nullptr)
        {
            PHANTOM_SEMANTIC_ASSERT(pDestructor->testFlags(PHANTOM_R_FLAG_IMPLICIT),
                                    "block missing in non trivial destructor");
            pDestructorBlock = addBlock(pDestructor);
        }

        {
            for (auto& bc : a_pClass->getBaseClasses())
            {
                Method* pBaseDestructor = bc.baseClass->getDestructor();
                if (pBaseDestructor)
                {
                    CxxSemanticErrorIfInaccessible(pBaseDestructor, a_pClass);
                    auto pCallAsFinal = New<CallExpression>(
                    pBaseDestructor,

                    convert(New<LocalVariableExpression>(pDestructor->getThis()),
                            pBaseDestructor->getImplicitObjectParameterType()->removeReference()->makePointer()));
                    pCallAsFinal->setFinal(); // polymorphism
                    pDestructorBlock->addScopedDestruction(pCallAsFinal);
                }
            }
        }

        {
            for (auto pDM : a_pClass->getFields())
            {
                Expression* pDestExp = createDestructionExpression(
                pDM, (New<LocalVariableExpression>(pDestructor->getThis()))->dereference(pSource), a_pClass);
                if (pDestExp)
                {
                    pDestructorBlock->addScopedDestruction(pDestExp);
                }
            }
        }
    }
}

void Semantic::findCallOverloads(SelectedOverloadInfos& a_OutOverloads, Symbols& a_Candidates,
                                 OptionalArrayView<LanguageElement*> in_pTemplateArguments, TypesView a_ArgTypes,
                                 LanguageElement*     in_pContextScope,
                                 UserDefinedFunctions a_ConversionAllowedUserDefinedFunctions,
                                 Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    /// Filter viable calls
    for (auto it = a_Candidates.begin(); it != a_Candidates.end(); ++it)
    {
        SelectedOverloadInfo overload;
        VisitorData          data;
        const void*          in[5] = {&in_pTemplateArguments, &a_ArgTypes, &a_Modifiers, &in_pContextScope,
                             &a_ConversionAllowedUserDefinedFunctions};
        void*                out[3] = {&overload.conversions, &overload.subroutine, &overload.deductions};
        data.id = e_VisitorFunction_IsViableCallCandidate;
        data.in = in;
        data.out = out;
        (*it)->visit(this, data);
        if (overload.subroutine) /// viable
        {
            a_OutOverloads.push_back(std::move(overload));
        }
    }
}

void Semantic::applyOverloadsSFINAE(SelectedOverloadInfos& a_InOverloads, LanguageElement* a_pInstantiationScope)
{
    for (auto it = a_InOverloads.begin(); it != a_InOverloads.end();)
    {
        if (auto pTSpec = it->subroutine->getTemplateSpecialization())
        {
            LanguageElements tplArguments;
            tplArguments.resize(it->deductions.size());
            size_t variadicArgCount = 0;
            auto   pTSign = pTSpec->getTemplateSignature();
            size_t paramCount = pTSign->getTemplateParameters().size();
            for (size_t i = 0; i < tplArguments.size(); ++i)
            {
                auto   arg = it->deductions.getArgument(i);
                size_t index = pTSign->getTemplateParameterIndex(it->deductions.getPlaceholder(i));
                if (index == paramCount - 1)
                {
                    tplArguments[index + variadicArgCount++] = arg;
                }
                else
                {
                    tplArguments[index] = arg;
                }
            }
            it->subroutine =
            static_cast<Subroutine*>(instantiateTemplate(pTSpec->getTemplate(), tplArguments, a_pInstantiationScope));
            if (it->subroutine == nullptr) // SFINAE
            {
                it = a_InOverloads.erase(it);
                continue;
            }
        }
        ++it;
    }
}

Semantic::SelectedOverloadInfo const* Semantic::selectBestOverload(SelectedOverloadInfos const& a_InOverloads)
{
    if (a_InOverloads.size())
    {
        /// Find best viable call or display ambiguity error
        if (a_InOverloads.size() == 1)
        {
            return &a_InOverloads.back();
        }
        else
        {
            size_t bestIndex = 0;
            for (size_t i = 0; i < a_InOverloads.size(); ++i)
            {
                int    bestResult = -1;
                size_t j = 0;
                for (; j < a_InOverloads.size(); ++j)
                {
                    if (i == j)
                        continue;
                    if (a_InOverloads[i].deductions.size() == a_InOverloads[j].deductions.size())
                    {
                        int result = a_InOverloads[i].conversions.compare(a_InOverloads[j].conversions);
                        PHANTOM_SEMANTIC_ASSERT(a_InOverloads[j].conversions.compare(a_InOverloads[i].conversions) ==
                                                -result); /// compare() coherence test
                        if (result == -1)
                            break;
                        bestResult = std::max(bestResult, result);
                    }
                    else
                    {
                        if (a_InOverloads[i].deductions.size() > a_InOverloads[i].deductions.size())
                            break;
                        bestResult = 1;
                    }
                }
                if (j == a_InOverloads.size()) /// superior or equal to every one (not worst to any)
                {
                    if (bestResult == 1)
                    {
                        /// found a best match (superior to at least one other signature and equal
                        /// to the others)
                        return &a_InOverloads[i];
                    }
                }
            }
        }
    }
    return nullptr;
}

Semantic::SelectedOverloadInfo const* Semantic::findPerfectMatchOverload(SelectedOverloadInfos const& a_InOverloads)
{
    SmallVector<Semantic::SelectedOverloadInfo const*, 1> perfectMatches;
    if (a_InOverloads.size())
    {
        for (size_t i = 0; i < a_InOverloads.size(); ++i)
        {
            if (a_InOverloads[i].deductions.size())
                continue;
            bool allConvCanonical = true;
            for (auto* conv : a_InOverloads[i].conversions)
            {
                if (!conv->isCanonical())
                {
                    allConvCanonical = false;
                    break;
                }
            }
            if (allConvCanonical)
                perfectMatches.push_back(&a_InOverloads[i]);
        }
    }
    if (perfectMatches.size() == 1)
        return perfectMatches.front();
    return nullptr;
}

void Semantic::selectCallCandidate(Subroutines& inoutViableCandidates, Symbols& candidates,
                                   OptionalArrayView<LanguageElement*> in_pTemplateArguments, Expressions& arguments,
                                   LanguageElement*& out_pResult, StringView in_name, LanguageElement* in_pContextScope,
                                   Type* in_pInitializationType, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    Source* pSource = getSource();

    if (in_pTemplateArguments)
    {
        for (auto candidate : candidates)
            if (candidate->asTemplate())
                goto skipGenericCall;
        // template arguments are only resolved with 'phantom::Generic' calls for now

        Types requiredBaseTypes;

        Types templateArgTypes;
        for (auto templateArg : *in_pTemplateArguments)
        {
            if (templateArg->isTemplateDependant())
                goto skipGenericCall;

            if (Type* pType = templateArg->asType())
            {
                templateArgTypes.push_back(pType);
                continue;
            }
            // Error("'%s' : non-type template parameter are not supported yet", a_Identifier);
            return;
        }

        Expressions templateArgsAsMetaValue;

        // push generic types
        for (Type* pType : templateArgTypes)
        {
            templateArgsAsMetaValue.push_back(convert(New<ConstantExpression>(newConstant(size_t(pType))),
                                                      PHANTOM_TYPEOF(Type*), CastKind::Reinterpret,
                                                      UserDefinedFunctions::None, in_pContextScope));
        }

        arguments.insert(arguments.begin(), templateArgsAsMetaValue.begin(), templateArgsAsMetaValue.end());
        in_pTemplateArguments = OptionalArrayView<LanguageElement*>{};
    }

skipGenericCall:

    Types types;
    types.reserve(arguments.size());

    for (auto* pArg : arguments)
        types.push_back(pArg->getValueType());

    SelectedOverloadInfos viableOverloads;
    findCallOverloads(viableOverloads, candidates, in_pTemplateArguments, types, in_pContextScope,
                      UserDefinedFunctions::ImplicitsOnly, a_Modifiers, a_uiFlags);

    applyOverloadsSFINAE(viableOverloads, in_pContextScope);

    if (auto pBestOverload = selectBestOverload(viableOverloads))
    {
        Symbol* pBestCandidateAccessibleSymbol = pBestOverload->subroutine->getTemplateSpecialization();
        if (!pBestCandidateAccessibleSymbol)
            pBestCandidateAccessibleSymbol = pBestOverload->subroutine;
        else
            static_cast<TemplateSpecialization*>(pBestCandidateAccessibleSymbol)->getTemplate();

        if (m_bAccessModifiersEnabled && m_pSource)
        {
            CxxSemanticErrorIfInaccessible(pBestCandidateAccessibleSymbol, in_pContextScope);
        }
        //
        //         if (!pBestOverload->subroutine->isNative() && !pBestOverload->subroutine->buildBlock())
        //         {
        //             if (m_pLastSilentMessage)
        //             {
        //                 m_pMessage->addChild(m_pLastSilentMessage);
        //                 m_pLastSilentMessage = nullptr;
        //             }
        //             // failed to instantiate templated block
        //             out_pResult = nullptr;
        //             return;
        //         }

        bool thisCall = static_cast<Subroutine*>(pBestOverload->subroutine)->asMethod() != nullptr &&
        static_cast<Subroutine*>(pBestOverload->subroutine)->asConstructor() == nullptr;
        if (pBestOverload->conversions.size() == arguments.size() - 1)
        {
            PHANTOM_SEMANTIC_ASSERT((a_Modifiers & PHANTOM_R_STATIC) == 0);
            Delete(arguments.front());          // destroy useless 'this' expression
            arguments.erase(arguments.begin()); // remove 'this' expression
        }
        auto& parms = pBestOverload->subroutine->getParameters();
        for (size_t k = 0; k < parms.size() + thisCall; ++k)
        {
            if (k < arguments.size())
            {
                // convert arguments
                Expression* newArgConv = pBestOverload->conversions[k]->convert(this, arguments[k]->clone(getSource()));
                PHANTOM_ASSERT(size_t(newArgConv) != 0xfeeefeeefeeefeee);
                Expression* newArg = nullptr;
                if (thisCall && k == 0)
                    newArg = newArgConv->address(pSource);
                else
                    newArg = newArgConv;
                PHANTOM_ASSERT(size_t(newArg) != 0xfeeefeeefeeefeee);
                arguments[k] = newArg;
            }
            else
            {
                Expression* pExp = parms[k - thisCall]->getDefaultArgumentExpression();
                CxxSemanticErrorReturnIf(
                pExp == nullptr, "cannot parse or resolve native default argument expression : '%.*s'",
                PHANTOM_STRING_AS_PRINTF_ARG(parms[k - thisCall]->getNativeDefaultArgumentString()));
                auto pConv = CxxSemanticConversionNE(pExp->clone(pSource), parms[k - thisCall]->getValueType());
                if (!pConv)
                    return;
                PHANTOM_ASSERT(size_t(pConv) != 0xfeeefeeefeeefeee);

                arguments.push_back(pConv);
            }
        }
        out_pResult = createCallExpression(pBestOverload->subroutine, arguments, in_pContextScope);
    }
    else
    {
        out_pResult = nullptr;
    }
}

LanguageElement* Semantic::qualifiedLookup(LanguageElement* a_pElement, StringView a_strName,
                                           OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                           OptionalArrayView<Expression*>      a_pFunctionArguments,
                                           LanguageElement* in_pContextScope, Type* a_pInitializationType, int flags)
{
    auto    pPrevMessage = m_pMessage;
    Message msg;
    m_pMessage = &msg;
    auto             onExit = phantom::makeScopeExit([&]() { m_pMessage = pPrevMessage; });
    LanguageElement* pResult =
    internalQualifiedLookup(a_pElement, a_strName, a_pTemplateArguments, a_pFunctionArguments, in_pContextScope,
                            a_pInitializationType, flags | e_VisitorFlag_KeepAliases);
    if (pResult == nullptr)
    {
        if (m_pMessage->getChildren().empty())
        {
            if (a_pFunctionArguments)
            {
                Symbols  symbols;
                Accesses accesses;
                internalQualifiedSymbolsLookup(a_pElement, a_strName, symbols, accesses, in_pContextScope,
                                               Access::Public, flags);
                if (symbols.size())
                {
                    if (Function* pFunction = symbols.front()->asFunction())
                    {
                        CxxSemanticError("'%.*s' : no function overload found with arguments :",
                                         PHANTOM_STRING_AS_PRINTF_ARG(a_strName));
                    }
                    else
                    {
                        CxxSemanticError("'%.*s' : no method overload found with arguments :",
                                         PHANTOM_STRING_AS_PRINTF_ARG(a_strName));
                    }
                    for (auto args : *a_pFunctionArguments)
                    {
                        CxxSemanticSubError("%s",
                                            FormatCStr(args->getValueType()->removeReference()->removeQualifiers()));
                    }
                    goto after_error_hack;
                }
            }

            Expression* pExp = a_pElement->asExpression();
            if ((pExp == nullptr && a_pElement->asClassType()) ||
                pExp && pExp->getValueType()->removeReference()->removeQualifiers()->asClassType())
                CxxSemanticError(
                "'%s' : undefined in '%s'", (const char*)a_strName.data(),
                FormatCStr(
                a_pElement) /*, &a_strName, &a_pTemplateArguments, &a_pFunctionArguments, &a_pInitializationType*/);
            else
                CxxSemanticError(
                "'%s' : undefined in '%s'", (const char*)a_strName.data(),
                FormatCStr(
                a_pElement) /*, &a_strName, &a_pTemplateArguments, &a_pFunctionArguments, &a_pInitializationType*/);
        }
    }
    else if (Symbol* pSymbol = pResult->asSymbol())
    {
        if (pSymbol && m_pSource)
        {
            if (!(isSymbolAccessible(pSymbol, in_pContextScope)))
            {
                CxxSemanticError("'%s' : inaccessible in '%s'", FormatElement(pSymbol).data(),
                                 FormatElement(in_pContextScope).data());
                pResult = nullptr;
            }
        }
    }
    if (pResult)
    {
        while (Alias* pAlias = pResult->asAlias())
        {
            if (auto pAliased = pAlias->getAliasedSymbol())
            {
                pResult = pAliased;
            }
            else
                break;
        }
        if (pResult)
        {
            if (m_pSource && pResult->asSymbol())
            {
                CxxSemanticErrorIfInaccessible(static_cast<Symbol*>(pResult), in_pContextScope, (nullptr));
            }
        }
    }

after_error_hack:
    if (pPrevMessage)
    {
        while (msg.getChildren().size())
        {
            Message* pMessage = msg.getChildren().front();
            msg.removeChild(pMessage);
            pPrevMessage->addChild(pMessage);
        }
    }
    return pResult;
}

void Semantic::_pushSilentMessage()
{
    m_pMessage = new_<Message>();
    m_silentMessageStack.push_back(m_pMessage);
}

void Semantic::_popSilentMessage()
{
    auto pMessage = m_pMessage;
    m_silentMessageStack.pop_back();
    if (m_silentMessageStack.size() && pMessage->getChildCount())
        m_silentMessageStack.back()->addChild(pMessage);
}

LanguageElement* Semantic::silentQualifiedLookup(LanguageElement* a_pElement, StringView a_strName,
                                                 OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                                 OptionalArrayView<Expression*>      a_pFunctionArguments,
                                                 LanguageElement* a_pScope, Type* a_pInitializationType, int flags)
{
    auto pPrevMessage = m_pMessage;
    _pushSilentMessage();
    LanguageElement* pResult = internalQualifiedLookup(a_pElement, a_strName, a_pTemplateArguments,
                                                       a_pFunctionArguments, a_pScope, a_pInitializationType, flags);
    _popSilentMessage();
    if (pResult)
        m_pLastSilentMessage = nullptr;
    else
        m_pLastSilentMessage = m_pMessage;
    m_pMessage = pPrevMessage;
    return pResult;
}
LanguageElement* Semantic::silentUnqualifiedLookup(StringView                          a_strName,
                                                   OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                                   OptionalArrayView<Expression*>      a_pFunctionArguments,
                                                   LanguageElement* a_pScope, Type* a_pInitializationType, int flags)
{
    PHANTOM_ASSERT(a_pScope);
    auto pPrevMessage = m_pMessage;
    _pushSilentMessage();
    LanguageElement* pResult = internalUnqualifiedLookup(a_strName, a_pTemplateArguments, a_pFunctionArguments,
                                                         a_pScope, a_pInitializationType, flags);
    _popSilentMessage();
    if (pResult)
        m_pLastSilentMessage = nullptr;
    else
        m_pLastSilentMessage = m_pMessage;
    m_pMessage = pPrevMessage;
    return pResult;
}
LanguageElement* Semantic::internalQualifiedLookup(LanguageElement* a_pElement, StringView a_strName,
                                                   OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                                   OptionalArrayView<Expression*>      a_pFunctionArguments,
                                                   LanguageElement* a_pScope, Type* a_pInitializationType, int flags)
{
    PHANTOM_ASSERT(a_pScope);
    LanguageElement* pResult = nullptr;
    VisitorData      data;
    data.id = e_VisitorFunction_QualifiedLookup;
    const void* in[5] = {&a_strName, &a_pTemplateArguments, &a_pFunctionArguments, &a_pScope, a_pInitializationType};
    void*       out[1] = {&pResult};
    data.in = in;
    data.out = out;
    data.flags = flags;
    a_pElement->visit(this, data);

    if ((flags & e_VisitorFlag_KeepAliases) == 0 && pResult && pResult->asAlias())
    {
        Alias* pAlias = static_cast<Alias*>(pResult);
        if (pAlias->getAliasedSymbol())
            pResult = pAlias->getAliasedSymbol();
    }
    return pResult;
}

LanguageElement* Semantic::unqualifiedLookup(StringView                          a_strName,
                                             OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                             OptionalArrayView<Expression*>      a_pFunctionArguments,
                                             LanguageElement* in_pContextScope, Type* a_pInitializationType, int flags)
{
    auto    pPrevMessage = m_pMessage;
    Message msg;
    m_pMessage = &msg;
    auto             onExit = phantom::makeScopeExit([&]() { m_pMessage = pPrevMessage; });
    LanguageElement* pResult = internalUnqualifiedLookup(a_strName, a_pTemplateArguments, a_pFunctionArguments,
                                                         in_pContextScope, a_pInitializationType, flags);
    if (pResult == nullptr)
    {
        // TODO : replace this error interpretation by a new qualified/unqualified merged with collected symbols
        // inside which triggers itself the right errors
        if (m_pMessage->getChildren().empty())
        {
            if (a_pFunctionArguments)
            {
                auto pLastSilent = m_pLastSilentMessage;

                Subroutine* candidate = nullptr;
                if (LanguageElement* pResultNoFuncArguments =
                    silentUnqualifiedLookup(a_strName, a_pTemplateArguments, NullOpt, in_pContextScope, nullptr, flags))
                {
                    if (ClassType* pClassType = pResultNoFuncArguments->asClassType())
                    {
                        CxxSemanticError("'%.*s' : no constructor overload found with arguments :",
                                         PHANTOM_STRING_AS_PRINTF_ARG(a_strName));
                        for (auto args : *a_pFunctionArguments)
                        {
                            CxxSemanticSubError(
                            "%s", FormatCStr(args->getValueType()->removeReference()->removeQualifiers()));
                        }

                        if (pLastSilent && pLastSilent->getMostValuableMessageType() == MessageType::Error)
                        {
                            m_pMessage->addChild(pLastSilent->clone(true));
                        }

                        goto after_error_hack;
                    }
                    else
                        candidate = pResultNoFuncArguments->asSubroutine();
                }
                if (!candidate)
                {
                    Symbols  symbols;
                    Accesses accesses;
                    internalUnqualifiedSymbolsLookup(a_strName, symbols, accesses, in_pContextScope, Access::Public,
                                                     flags);
                    if (symbols.size())
                    {
                        candidate = symbols.front()->asSubroutine();
                    }
                }
                if (candidate)
                {
                    if (Function* pFunction = candidate->asFunction())
                    {
                        if (a_pFunctionArguments->empty())
                            CxxSemanticError("'%.*s' : no function overload found with arguments :",
                                             PHANTOM_STRING_AS_PRINTF_ARG(a_strName));
                        else
                            CxxSemanticError("'%.*s' : no function overload found",
                                             PHANTOM_STRING_AS_PRINTF_ARG(a_strName));
                    }
                    else
                    {
                        if (a_pFunctionArguments->empty())
                            CxxSemanticError("'%.*s' : no method overload found with arguments :",
                                             PHANTOM_STRING_AS_PRINTF_ARG(a_strName));
                        else
                            CxxSemanticError("'%.*s' : no method overload found",
                                             PHANTOM_STRING_AS_PRINTF_ARG(a_strName));
                    }
                    for (auto args : *a_pFunctionArguments)
                    {
                        CxxSemanticSubError("%s",
                                            FormatCStr(args->getValueType()->removeReference()->removeQualifiers()));
                    }
                    goto after_error_hack;
                }
            }
            CxxSemanticError(
            "'%.*s' : undefined in '%s'", PHANTOM_STRING_AS_PRINTF_ARG(a_strName),
            FormatCStr(
            in_pContextScope) /*, &a_strName, &a_pTemplateArguments, &a_pFunctionArguments, &a_pInitializationType*/);
        }
    }
    else
    {
        if (m_pSource && pResult->asSymbol())
        {
            CxxSemanticErrorIfInaccessible(static_cast<Symbol*>(pResult), in_pContextScope, (nullptr));
        }
    }
    m_pLastSilentMessage = nullptr;
after_error_hack:
    if (pPrevMessage)
    {
        while (msg.getChildren().size())
        {
            Message* pMessage = msg.getChildren().front();
            msg.removeChild(pMessage);
            pPrevMessage->addChild(pMessage);
        }
    }
    return pResult;
}

void Semantic::qualifiedSymbolsLookup(LanguageElement* a_pLHS, StringView a_strName, Symbols& a_Symbols,
                                      Accesses* a_Accesses, LanguageElement* in_pContextScope /*= nullptr*/,
                                      Access a_Access, int flags /*= 0*/)
{
    Accesses dump;
    internalQualifiedSymbolsLookup(a_pLHS, a_strName, a_Symbols, a_Accesses ? *a_Accesses : dump, in_pContextScope,
                                   a_Access, flags);
    if (a_Symbols.empty())
    {
        CxxSemanticError(
        "'%s' : undefined in '%s'", (const char*)a_strName.data(),
        FormatCStr(a_pLHS) /*, &a_strName, &a_pTemplateArguments, &a_pFunctionArguments, &a_pInitializationType*/);
    }
}

void Semantic::unqualifiedSymbolsLookup(StringView a_strName, Symbols& a_Symbols, Accesses* a_Accesses,
                                        LanguageElement* in_pContextScope /*= nullptr*/,
                                        Access a_Access /*= Access::Public*/, int flags /*= 0*/)
{
    Accesses dump;
    internalUnqualifiedSymbolsLookup(a_strName, a_Symbols, a_Accesses ? *a_Accesses : dump, in_pContextScope, a_Access,
                                     flags);
    if (a_Symbols.empty())
    {
        CxxSemanticError(
        "'%s' : undefined in '%s'", (const char*)a_strName.data(),
        FormatCStr(
        in_pContextScope) /*, &a_strName, &a_pTemplateArguments, &a_pFunctionArguments, &a_pInitializationType*/);
    }
}

void Semantic::internalQualifiedSymbolsLookup(LanguageElement* a_pLHS, StringView a_strName, Symbols& a_Symbols,
                                              Accesses& a_Accesses, LanguageElement* a_pScope /*= nullptr*/,
                                              Access a_Access /*= Access::Public*/, int flags /*= 0*/)
{
    if (a_pScope == nullptr)
        a_pScope = Namespace::Global();

    VisitorData data;
    data.id = e_VisitorFunction_QualifiedNameLookup;
    const void* in[3] = {&a_strName, &a_pScope, &a_Access};
    void*       out[2] = {&a_Symbols, &a_Accesses};
    data.in = in;
    data.out = out;
    data.flags = flags;
    a_pLHS->visit(this, data);
    if ((flags & e_VisitorFlag_KeepAliases) == 0)
    {
        for (auto& pSymbol : a_Symbols)
        {
            if (pSymbol && pSymbol->asAlias())
            {
                Alias* pAlias = static_cast<Alias*>(pSymbol);
                if (pAlias->getAliasedSymbol())
                    pSymbol = pAlias->getAliasedSymbol();
            }
        }
    }
}

void Semantic::internalUnqualifiedSymbolsLookup(StringView a_strName, Symbols& a_Symbols, Accesses& a_Accesses,
                                                LanguageElement* a_pScope /*= nullptr*/,
                                                Access a_Access /*= Access::Public*/, int flags /*= 0*/)
{
    if (a_pScope == nullptr)
        a_pScope = Namespace::Global();

    VisitorData data;
    data.id = e_VisitorFunction_UnqualifiedNameLookup;
    const void* in[3] = {&a_strName, &a_pScope, &a_Access};
    void*       out[2] = {&a_Symbols, &a_Accesses};
    data.in = in;
    data.out = out;
    data.flags = flags;
    data.flags |= e_VisitorFlag_Unqualified;
    a_pScope->visit(this, data);
    if ((flags & e_VisitorFlag_KeepAliases) == 0)
    {
        for (auto& pSymbol : a_Symbols)
        {
            if (pSymbol && pSymbol->asAlias())
            {
                Alias* pAlias = static_cast<Alias*>(pSymbol);
                if (pAlias->getAliasedSymbol())
                    pSymbol = pAlias->getAliasedSymbol();
            }
        }
    }
}

LanguageElement* Semantic::internalUnqualifiedLookup(StringView                          a_strName,
                                                     OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                                     OptionalArrayView<Expression*>      a_pFunctionArguments,
                                                     LanguageElement* a_pScope, Type* a_pInitializationType, int flags)
{
    if (a_pScope == nullptr)
        a_pScope = Namespace::Global();
    LanguageElement* pResult = nullptr;
    VisitorData      data;
    data.id = e_VisitorFunction_UnqualifiedLookup;
    const void* in[5] = {&a_strName, &a_pTemplateArguments, &a_pFunctionArguments, &a_pScope, a_pInitializationType};
    void*       out[1] = {&pResult};
    data.in = in;
    data.out = out;
    data.flags = flags;
    data.flags |= e_VisitorFlag_Unqualified;
    a_pScope->visit(this, data);
    if ((flags & e_VisitorFlag_KeepAliases) == 0 && pResult && pResult->asAlias())
    {
        Alias* pAlias = static_cast<Alias*>(pResult);
        if (pAlias->getAliasedSymbol())
            pResult = pAlias->getAliasedSymbol();
    }
    return pResult;
}

Subroutine* Semantic::subroutineLookup(LanguageElement* a_pElement, StringView a_strName,
                                       OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                       TypesView a_ParameterTypes, Modifiers a_Modifiers, uint a_uiFlags,
                                       LanguageElement* a_pScope)
{
    Subroutine* pResult = nullptr;
    VisitorData data;
    Modifiers   access = 0;
    data.id = e_VisitorFunction_SubroutineResolution;
    const void* in[5] = {&a_strName, &a_pTemplateArguments, &a_ParameterTypes, &a_Modifiers, &a_pScope};
    void*       out[2] = {&pResult, &access};
    data.in = in;
    data.out = out;
    a_pElement->visit(this, data);
    return pResult;
}

Subroutine* Semantic::subroutineLookupCascade(LanguageElement* a_pElement, StringView a_strName,
                                              OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                              TypesView a_ParameterTypes, Modifiers a_Modifiers, uint a_uiFlags,
                                              LanguageElement* a_pScope)
{
    Subroutine* pSubroutine =
    subroutineLookup(a_pElement, a_strName, a_pTemplateArguments, a_ParameterTypes, a_Modifiers, a_uiFlags, a_pScope);
    if (pSubroutine)
        return pSubroutine;
    Symbol* pSymbol = a_pElement->asSymbol();
    if (pSymbol && pSymbol->getNamespace())
    {
        return subroutineLookupCascade(pSymbol->getNamespace(), a_strName, a_pTemplateArguments, a_ParameterTypes,
                                       a_Modifiers, a_uiFlags, a_pScope);
    }
    if (a_pElement->getOwner())
    {
        return subroutineLookupCascade(a_pElement->getOwner(), a_strName, a_pTemplateArguments, a_ParameterTypes,
                                       a_Modifiers, a_uiFlags, a_pScope);
    }
    return nullptr;
}

Type* Semantic::callTemplateArgumentDeduction(Type* a_pParameter, Type* a_pArgument, PlaceholderMap& a_Deductions)
{
    Type* P = a_pParameter;
    Type* A = a_pArgument;
    // 1) If P is not a reference type,
    if (P->asReference() == nullptr)
    {
        // a) if A is an array type, A is replaced by the pointer type obtained from
        // array-to-pointer conversion;
        if (A->asArray())
            A->removeArray()->addPointer();

        // b) otherwise, if A is a function type, A is replaced by the pointer type obtained from
        // function-to-pointer conversion;
        else if (A->asFunctionType())
        {
            A = A->addPointer();
        }
        // c) otherwise, if A is a cv-qualified type, the top-level cv-qualifiers are ignored for
        // deduction:
        else
        {
            A = A->removeReference()->removeQualifiers();
        }
    }

    // 2) If P is a cv-qualified type, the top-level cv-qualifiers are ignored for deduction.
    P = P->removeQualifiers();

    // 3) If P is a reference type, the type referred to by P is used for deduction.
    if (P->asLValueReference())
    {
        P = P->removeReference();
        A = A->removeReference();
    }

    // 4) If P is an rvalue reference to a cv-unqualified template parameter (so-called "forwarding
    // reference"),
    //    and the corresponding function call argument is an lvalue, the type lvalue reference to A
    //    is used in place of A for deduction (Note: this is the basis for the action of
    //    std::forward):

    // if (P->asRValueReference() && A->asReference()) => automatic in our design (lvalues are considered references
    // directly)

    if (P->asRValueReference() && A->asReference() == nullptr) // A is an R-Value
    {
        P = P->removeReference();
    }

    // After these transformations, the deduction processes as described below (cf. section
    // "Deduction from type") and attempts to find such template arguments that would make the
    // deduced A (that is, P after adjustments listed above and the substitution of the deduced
    // template parameters) identical to the transformed A, that is A after the adjustments listed
    // above.
    Type* pDeducedA = templateArgumentDeduction(P, A, a_Deductions);

    // If the usual deduction from P and A fails, the following alternatives are additionally
    // considered:
    if (pDeducedA == nullptr)
    {
        // 1) If P is a reference type, the deduced A (i.e., the type referred to by the reference)
        // can be more cv-qualified than the transformed A:
        if (P->asConstType())
        {
            A = A->addConst();
            pDeducedA = templateArgumentDeduction(P, A, a_Deductions);
        }
        else if (P->asVolatileType())
        {
            A = A->addVolatile();
            pDeducedA = templateArgumentDeduction(P, A, a_Deductions);
        }
        else if (P->asConstVolatileType())
        {
            A = A->addConstVolatile();
            pDeducedA = templateArgumentDeduction(P, A, a_Deductions);
        }
        else if (a_pArgument->asReference())
        {
            return callTemplateArgumentDeduction(a_pParameter, a_pArgument->getUnderlyingType(), a_Deductions);
        }
    }
    return pDeducedA;
}

Expression* Semantic::autoDeduction(Type* a_pParameter, Expression* a_pArgument)
{
    Type* pVarType = autoDeduction(a_pParameter, a_pArgument->getValueType());
    if (!pVarType)
        return nullptr;
    return convert(a_pArgument, pVarType);
}

Type* Semantic::autoDeduction(Type* a_pParameter, Type* a_pArgument)
{
    PlaceholderMap deductions;
    Type*          pDeduced = callTemplateArgumentDeduction(a_pParameter, a_pArgument, deductions);
    if (pDeduced == nullptr || deductions.size() != 1)
    {
        CxxSemanticError("auto type deduction failed, check your initialization "
                         "expression"); // deduction succeeded
        return nullptr;
    }
    Type* pDeducedAutoType = static_cast<Type*>(deductions.begin()->second.front());
    if (a_pParameter->asReference() == nullptr) // !auto ...&
    {
        pDeducedAutoType = pDeducedAutoType->removeReference()->removeQualifiers();
    }
    else if (a_pParameter->asRValueReference() &&
             pDeduced->asLValueReference()) // universal reference (auto&&) combined with l-value keeps l-value
    {
        return a_pParameter->addLValueReference()->replicate(pDeducedAutoType); // drop r-value if argument i a l-value
    }
    return a_pParameter->replicate(pDeducedAutoType);
}

Type* Semantic::callTemplateArgumentDeductionRef(Type* a_pParameter, Type* a_pArgument, PlaceholderMap& a_Deductions)
{
    // Apply template argument deduction from a function call
    Type* pDeduced = callTemplateArgumentDeduction(a_pParameter, a_pArgument, a_Deductions);
    if (pDeduced == nullptr || a_Deductions.size() == 0)
    {
        CxxSemanticError("auto type deduction failed, check your initialization "
                         "expression"); // deduction succeeded
        return nullptr;
    }
    Type* pDeducedAutoType = pDeduced;
    if (a_pParameter->asReference() == nullptr) // !auto ...&
    {
        pDeducedAutoType = pDeducedAutoType->removeReference()->removeQualifiers();
    }
    else
    {
        if (a_pParameter->asRValueReference() &&
            pDeduced->asLValueReference()) // universal reference (auto&&) combined with l-value keeps l-value
        {
            return a_pParameter->addLValueReference()->replicate(
            pDeducedAutoType); // drop r-value if argument i a l-value
        }
        return a_pParameter->replicate(pDeducedAutoType);
    }
    return pDeducedAutoType;
}

Type* Semantic::templateArgumentDeduction(LanguageElement* a_pParameter, Type* a_pArgument, PlaceholderMap& deductions)
{
    LanguageElement* pElement = templateArgumentDeduction(a_pParameter, (LanguageElement*)a_pArgument, deductions);
    return pElement ? pElement->asType() : nullptr;
}

LanguageElement* Semantic::templateArgumentDeduction(LanguageElement* a_pParameter, LanguageElement* a_pArgument,
                                                     PlaceholderMap& deductions)
{
    LanguageElement* pResult = nullptr;
    VisitorData      data;
    data.id = e_VisitorFunction_TemplateArgumentDeduction;
    const void* in[1] = {&a_pArgument};
    void*       out[2] = {&pResult, &deductions};
    data.in = in;
    data.out = out;
    a_pParameter->visit(this, data);
    return pResult;
}

int Semantic::compareSpecialized(TemplateSpecialization* a_pT0, TemplateSpecialization* a_pT1)
{
    TemplateDependantTemplateInstance* fictiousT0 =
    New<TemplateDependantTemplateInstance>(a_pT0->getTemplate(), a_pT0->getArguments());
    TemplateDependantTemplateInstance* fictiousT1 =
    New<TemplateDependantTemplateInstance>(a_pT1->getTemplate(), a_pT1->getArguments());

    bool           result0 = true;
    bool           result1 = true;
    PlaceholderMap deductions0;
    PlaceholderMap deductions1;
    result0 = templateArgumentDeduction(fictiousT1, fictiousT0, deductions1) != nullptr;
    result1 = templateArgumentDeduction(fictiousT0, fictiousT1, deductions0) != nullptr;

    Delete(fictiousT0);
    Delete(fictiousT1);

    return (result0 > result1) - (result0 < result1);
}

int Semantic::partialOrdering(const LanguageElements& P0, const LanguageElements& A0, const LanguageElements& P1,
                              const LanguageElements& A1)
{
    bool result0 = true;
    bool result1 = true;
    PHANTOM_SEMANTIC_ASSERT(P0.size() == A0.size() && A0.size() == P1.size() && P1.size() == A1.size());
    for (size_t i = 0; i < P0.size(); ++i)
    {
        PlaceholderMap deductions;
        if (!(templateArgumentDeduction(P0[i], A1[i], deductions)))
        {
            result0 = false;
            break;
        }
    }
    for (size_t i = 0; i < P1.size(); ++i)
    {
        PlaceholderMap deductions;
        if (!(templateArgumentDeduction(P1[i], A0[i], deductions)))
        {
            result1 = false;
            break;
        }
    }
    return (result0 > result1) - (result0 < result1);
}

void Semantic::generateArgumentsText(const Expressions& arguments, String& argumentsText)
{
    argumentsText = "(";
    for (auto it = arguments.begin(); it != arguments.end(); ++it)
    {
        if (it != arguments.begin())
            argumentsText += ", ";
        argumentsText += (*it)->getValueType()->getQualifiedDecoratedName();
    }
    argumentsText += ')';
}

SmallMap<TemplateSpecialization*, SmallMap<LanguageElement*, LanguageElement*>>& Semantic::templateInstantiations()
{
    if (m_pTemplateInstantiations == nullptr)
        m_pTemplateInstantiations =
        new_<SmallMap<TemplateSpecialization*, SmallMap<LanguageElement*, LanguageElement*>>>();
    return *m_pTemplateInstantiations;
}

bool Semantic::isIdentifier(StringView a_strName) const
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

bool Semantic::hasSideEffects(Expression* a_pExp) const
{
    bool        bResult = true;
    VisitorData data;
    data.id = e_VisitorFunction_HasSideEffect;
    void* out[1] = {&bResult};
    data.in = nullptr;
    data.out = out;
    a_pExp->visit(const_cast<Semantic*>(this), data);
    return bResult;
}

Expression* Semantic::createCallExpression(Subroutine* a_pSubroutine, ExpressionsView a_Arguments,
                                           LanguageElement* in_pContextScope)
{
    Expression* pResult = nullptr;
    VisitorData data;
    data.id = e_VisitorFunction_CreateCallExpression;
    const void* in[2] = {&a_Arguments, &in_pContextScope};
    void*       out[1] = {&pResult};
    data.in = in;
    data.out = out;
    a_pSubroutine->visit(this, data);
    return pResult;
}

Expression* Semantic::createCallExpression(Constructor* a_pInput, ExpressionsView a_Arguments,
                                           LanguageElement* in_pContextScope)
{
    if (a_Arguments.size() < a_pInput->getRequiredArgumentCount())
    {
        CxxSemanticError("'%s' : not enough arguments, %zu given, %zu required at minimum", FormatCStr(a_pInput),
                         a_Arguments.size(), a_pInput->getRequiredArgumentCount());
        return nullptr;
    }
    if (a_Arguments.size() > a_pInput->getParameters().size())
    {
        CxxSemanticError("'%s' : too many arguments, %zu given, %zu expected", FormatCStr(a_pInput), a_Arguments.size(),
                         a_pInput->getParameters().size());
        return nullptr;
    }
    if (a_pInput->testModifiers(Modifier::Deleted))
    {
        CxxSemanticError("'%.*s' : attempt to invoke a deleted constructor",
                         PHANTOM_STRING_AS_PRINTF_ARG(a_pInput->getName()));
        return nullptr;
    }
    OwnersGuard<Expressions> args;
    for (size_t i = 0; i < a_Arguments.size(); ++i)
    {
        Expression* pExp = convertIfNotEqual(a_Arguments[i], a_pInput->getParameterType(i), in_pContextScope);
        if (pExp == nullptr)
            return nullptr;
        args.push_back(pExp);
    }
    return New<ConstructorCallExpression>(a_pInput, args);
}

Expression* Semantic::createCallExpression(Function* a_pInput, ExpressionsView a_Arguments,
                                           LanguageElement* in_pContextScope)
{
    if (a_Arguments.size() < a_pInput->getRequiredArgumentCount())
    {
        CxxSemanticError("'%s' : not enough arguments, %zu given, %zu required at minimum", FormatCStr(a_pInput),
                         a_Arguments.size(), a_pInput->getRequiredArgumentCount());
        return nullptr;
    }
    if (a_Arguments.size() > a_pInput->getParameters().size())
    {
        CxxSemanticError("'%s' : too many arguments, %zu given, %zu expected", FormatCStr(a_pInput), a_Arguments.size(),
                         a_pInput->getParameters().size());
        return nullptr;
    }
    OwnersGuard<Expressions> args;
    for (size_t i = 0; i < a_Arguments.size(); ++i)
    {
        Expression* pExp = convert(a_Arguments[i], a_pInput->getParameterType(i), in_pContextScope);
        if (pExp == nullptr)
            return nullptr;
        args.push_back(pExp);
    }
    // if (a_pInput->getReturnType()->removeQualifiers()->asClassType())
    //    return New<RValueReferenceExpression>(New<CallExpression>(a_pInput, args));
    return New<CallExpression>(a_pInput, args);
}

Expression* Semantic::createCallExpression(Method* a_pInput, Expression* a_pObjectExpression,
                                           ExpressionsView a_Arguments, LanguageElement* in_pContextScope)
{
    if (a_Arguments.size() < a_pInput->getRequiredArgumentCount())
    {
        CxxSemanticError("'%s' : not enough arguments, %zu given, %zu required at minimum", FormatCStr(a_pInput),
                         a_Arguments.size(), a_pInput->getRequiredArgumentCount());
        return nullptr;
    }
    if (a_Arguments.size() > a_pInput->getParameters().size())
    {
        CxxSemanticError("'%s' : too many arguments, %zu given, %zu expected", FormatCStr(a_pInput), a_Arguments.size(),
                         a_pInput->getParameters().size());
        return nullptr;
    }
    OwnersGuard<Expressions> newExprs;

    Expression* pThis = nullptr;
    if (a_pObjectExpression->getValueType()->removeQualifiers()->asPointer())
    {
        pThis =
        convert(a_pObjectExpression, a_pInput->getImplicitObjectParameterType()->addPointer(), in_pContextScope);
        if (pThis == nullptr)
            return nullptr;
    }
    else if (a_pInput->acceptsCallerExpressionType(a_pObjectExpression->getValueType()))
    {
        /// implicit conversion of the implicit object parameter
        Type* pOutput = a_pInput->getImplicitObjectParameterType();
        pThis = convert(a_pObjectExpression, pOutput, in_pContextScope);
        if (pThis == nullptr)
            return nullptr;
        pThis = pThis->address(getSource());
        PHANTOM_SEMANTIC_ASSERT(pThis);
    }
    newExprs.push_back(pThis);

    for (size_t i = 0; i < a_Arguments.size(); ++i)
    {
        Expression* pExp = convertIfNotEqual(a_Arguments[i], a_pInput->getParameterType(i), in_pContextScope);
        if (pExp == nullptr)
            return nullptr;
        newExprs.push_back(pExp);
    }
    // if (a_pInput->getReturnType()->removeQualifiers()->asClassType())
    //    return New<RValueReferenceExpression>(New<CallExpression>(a_pInput, newExprs));
    return New<CallExpression>(a_pInput, newExprs);
}

Expression* Semantic::createCallExpression(Expression* a_pObjectExpression, Expression* a_pMethodPointerExpression,
                                           ExpressionsView a_Arguments, LanguageElement* in_pContextScope)
{
    MethodPointer* a_pInput = a_pMethodPointerExpression->getValueType()->removeReference()->asMethodPointer();
    if (a_pInput == nullptr)
    {
        CxxSemanticError("expected member pointer type expression");
        return nullptr;
    }
    if (a_Arguments.size() < a_pInput->getParameterTypeCount())
    {
        CxxSemanticError("'%s' : not enough arguments, %zu given, %zu required at minimum", FormatCStr(a_pInput),
                         a_Arguments.size(), a_pInput->getParameterTypeCount());
        return nullptr;
    }
    if (a_Arguments.size() > a_pInput->getParameterTypeCount())
    {
        CxxSemanticError("'%s' : too many arguments, %zu given, %zu expected", FormatCStr(a_pInput), a_Arguments.size(),
                         a_pInput->getParameterTypeCount());
        return nullptr;
    }
    OwnersGuard<Expressions> newExprs;
    for (size_t i = 0; i < a_Arguments.size(); ++i)
    {
        Expression* pExp = convert(a_Arguments[i], a_pInput->getParameterType(i), in_pContextScope);
        if (pExp == nullptr)
            return nullptr;
        newExprs.push_back(pExp);
    }
    Expression* pThis = nullptr;
    if (a_pObjectExpression->getValueType()->removeQualifiers()->asPointer())
    {
        pThis =
        convert(a_pObjectExpression, a_pInput->getImplicitObjectParameterType()->addPointer(), in_pContextScope);
        if (pThis == nullptr)
            return nullptr;
    }
    else if (a_pInput->acceptsCallerExpressionType(a_pObjectExpression->getValueType()))
    {
        /// implicit conversion of the implicit object parameter
        Type* pOutput = a_pInput->getImplicitObjectParameterType();
        pThis = convert(a_pObjectExpression, pOutput, in_pContextScope);
        if (pThis == nullptr)
            return nullptr;
        pThis = pThis->address(getSource());
        PHANTOM_SEMANTIC_ASSERT(pThis);
    }
    return New<MethodPointerCallExpression>(pThis, a_pMethodPointerExpression, newExprs);
}

Expression* Semantic::internalConvert(
Expression* a_pExpression, Type* a_pOutputType, CastKind a_eConversionType /*= CastKind::Implicit*/,
UserDefinedFunctions a_eUserDefinedAllowedConversions /*= UserDefinedFunctions::ImplicitsOnly*/,
LanguageElement* in_pContextScope /*= nullptr*/, bool a_bInitialize /* = false*/)
{
    // no cv
    a_pOutputType = a_pOutputType->removeQualifiers();

    if (a_pExpression->isTemplateDependant() || a_pOutputType->isTemplateDependant())
    {
        return New<BuiltInConversionExpression>(
        a_pExpression, newConv<TemplateDependantConversion>(a_pExpression->getValueType(), a_pOutputType),
        a_eConversionType);
    }
    if (a_pOutputType->isVoid() && a_eConversionType == CastKind::Explicit)
    {
        return New<IdentityExpression>(PHANTOM_TYPEOF(void), a_pExpression);
    }
    Expression* pResult = nullptr;
    VisitorData data;
    data.id = e_VisitorFunction_ExpressionConversion;
    const void* in[5] = {&a_pOutputType, &a_eConversionType, &a_eUserDefinedAllowedConversions, &in_pContextScope,
                         &a_bInitialize};
    void*       out[1] = {&pResult};
    data.in = in;
    data.out = out;
    CodeRange codeRange = a_pExpression->getCodeRange();
    a_pExpression->visit(this, data);
    //     if (pResult)
    //         pResult->setCodeRange(codeRange);
    return pResult;
}

Expression* Semantic::defaultConstruct(Type* a_pType, LanguageElement* a_pContextScope, bool a_bZeroMem)
{
    if (auto pCT = a_pType->asClassType())
    {
        if (auto pCtor = pCT->getDefaultConstructor())
        {
            CxxSemanticErrorIfInaccessible(pCtor, a_pContextScope, nullptr);
            CxxSemanticErrorReturnValIf(nullptr, pCtor->testModifiers(Modifier::Deleted),
                                        "'%.*s' : attempt to invoke a deleted default constructor",
                                        PHANTOM_STRING_AS_PRINTF_ARG(pCT->getName()));
            return New<ConstructorCallExpression>(pCtor);
        }
    }
    else
    {
        PHANTOM_SEMANTIC_ASSERT(a_bZeroMem);
        return createZeroInitExpression(a_pType);
    }
    return nullptr;
}

Expression*
Semantic::convert(Expression* a_pExpression, Type* a_pOutputType, CastKind a_eConversionType /*= CastKind::Implicit*/,
                  UserDefinedFunctions a_eUserDefinedAllowedConversions /*= UserDefinedFunctions::ImplicitsOnly*/,
                  LanguageElement*     a_pContextScope /*= nullptr*/)
{
    if (a_pExpression->getMetaClass() == PHANTOM_CLASSOF(InitializerListExpression))
    {
        if (static_cast<InitializerListExpression*>(a_pExpression)->getExpressions().empty())
        {
            return defaultConstruct(a_pOutputType->removeQualifiers());
        }
    }
    Expression* pResult = internalConvert(a_pExpression, a_pOutputType, a_eConversionType,
                                          a_eUserDefinedAllowedConversions, a_pContextScope, false);
    if (pResult == nullptr)
    {
        CxxSemanticErrorConversion(a_pExpression, a_pOutputType);
        return nullptr;
    }
    return pResult;
}

Expression* Semantic::convert(Expression* a_pExpression, Type* a_pOutputType,
                              LanguageElement* a_pContextScope /*= nullptr*/)
{
    return convert(a_pExpression, a_pOutputType, CastKind::Implicit, UserDefinedFunctions::ImplicitsOnly,
                   a_pContextScope);
}

Expression* Semantic::convertIfNotEqual(Expression* a_pExpression, Type* a_pOutputType,
                                        LanguageElement* a_pContextScope)
{
    if (a_pExpression->getValueType()->removeQualifiers()->isSame(a_pOutputType->removeQualifiers()))
        return a_pExpression;
    return convert(a_pExpression, a_pOutputType, CastKind::Implicit, UserDefinedFunctions::ImplicitsOnly,
                   a_pContextScope);
}

Expression* Semantic::convertIfNotEqual(Expression* a_pExpression, Type* a_pOutputType,
                                        UserDefinedFunctions a_eUserDefinedAllowedConversions,
                                        LanguageElement*     a_pContextScope)
{
    if (a_pExpression->getValueType()->removeQualifiers()->isSame(a_pOutputType->removeQualifiers()))
        return a_pExpression;
    return convert(a_pExpression, a_pOutputType, CastKind::Implicit, a_eUserDefinedAllowedConversions, a_pContextScope);
}

phantom::lang::Expression*
Semantic::initialize(Expression* a_pExpression, Type* a_pOutputType,
                     UserDefinedFunctions a_eUserDefinedAllowedConversions /*= UserDefinedFunctions::ImplicitsOnly*/,
                     LanguageElement*     a_pContextScope /*= nullptr*/)
{
    Expression* pResult = internalConvert(a_pExpression, a_pOutputType, CastKind::Implicit,
                                          a_eUserDefinedAllowedConversions, a_pContextScope, true);
    if (pResult == nullptr)
    {
        CxxSemanticErrorConversion(a_pExpression, a_pOutputType);
        return nullptr;
    }
    return pResult;
}

Expression* Semantic::initialize(Expression* a_pExpression, Type* a_pOutputType, LanguageElement* a_pContextScope)
{
    return initialize(a_pExpression, a_pOutputType, UserDefinedFunctions::ImplicitsOnly, a_pContextScope);
}

Expression* Semantic::initializeIfNotEqual(Expression* a_pExpression, Type* a_pOutputType,
                                           LanguageElement* a_pContextScope)
{
    if (a_pExpression->getValueType()->removeQualifiers()->isSame(a_pOutputType->removeQualifiers()))
        return a_pExpression;
    return initialize(a_pExpression, a_pOutputType, a_pContextScope);
}

Expression* Semantic::initializeIfNotEqual(Expression* a_pExpression, Type* a_pOutputType,
                                           UserDefinedFunctions a_eUserDefinedAllowedConversions,
                                           LanguageElement*     a_pContextScope)
{
    if (a_pExpression->getValueType()->removeQualifiers()->isSame(a_pOutputType->removeQualifiers()))
        return a_pExpression;
    return initialize(a_pExpression, a_pOutputType, a_eUserDefinedAllowedConversions, a_pContextScope);
}

PHANTOM_FORCEINLINE void combineHash(hash64& a_rSeed, hash64 a_Value)
{
    a_rSeed ^= a_Value + 0x9e3779b99e3779b9 + (a_rSeed << 6) + (a_rSeed >> 2); // inspired from boost
}

Conversion*
Semantic::newConversion(Type* a_pInputType, Type* a_pOutputType, CastKind a_eConversionType /*= CastKind::Implicit*/,
                        UserDefinedFunctions a_eUserDefinedAllowedConversions /*= UserDefinedFunctions::ImplicitsOnly*/,
                        LanguageElement* in_pContextScope /*= nullptr*/, bool a_bInitialize /*= false*/)
{
    a_pOutputType = a_pOutputType->removeQualifiers();
    a_pInputType = a_pInputType->removeQualifiers();

    bool initializerTypeConv = a_pInputType->asInitializerListType() != nullptr;

    bool isFundamental = (a_pOutputType->removeReference()->removeQualifiers()->asClassType() == nullptr &&
                          a_pInputType->removeReference()->removeQualifiers()->asClassType() ==
                          nullptr); // might create user defined and protected

    ConversionParams params{};

    if (!initializerTypeConv)
    {
        LanguageElement* cachedScope = in_pContextScope;
        if (isFundamental)
        {
            in_pContextScope = nullptr;
            a_bInitialize = false;
            a_eUserDefinedAllowedConversions = UserDefinedFunctions::None;
        }
        else if (in_pContextScope && in_pContextScope->asClass() == nullptr)
        {
            // access protection might change only in the context of a class
            in_pContextScope = in_pContextScope->getEnclosingClassType();
        }
        params = ConversionParams{a_pInputType,
                                  a_pOutputType,
                                  in_pContextScope,
                                  uint16_t(a_eConversionType),
                                  uint8_t(a_eUserDefinedAllowedConversions),
                                  a_bInitialize};
        auto lock = m_CachedConversionsMtx.autoLock();
        auto found = m_CachedConversions.find(params);
        if (found != m_CachedConversions.end())
        {
            return found->second;
        }
    }

    Conversion* conv = nullptr;
    if (a_pInputType->isSame(a_pOutputType))
    {
        // identity conversion
        conv = newConv<DefaultConversionSequence>(ConversionVal(a_pInputType), ConversionVal(a_pOutputType));
    }
    else
    {
        VisitorData data;
        data.id = e_VisitorFunction_TypeConversion;
        const void* in[5] = {&a_pOutputType, &a_eConversionType, &a_eUserDefinedAllowedConversions, &in_pContextScope,
                             &a_bInitialize};
        void*       out[1] = {&conv};
        data.in = in;
        data.out = out;
        a_pInputType->visit(this, data);
    }

    auto lock = m_CachedConversionsMtx.autoLock();
    if (!initializerTypeConv)
    {
        m_CachedConversions[params] = conv;
        if (conv)
            conv->semantic = this;
    }
    if (conv)
        static_cast<DefaultConversionSequence*>(conv)->setKind(a_eConversionType);
    return conv;
}

Expression* Semantic::toExpression(LanguageElement* a_pElement, Expression* a_pObjectExpression)
{
    Expression* pResult = nullptr;
    VisitorData data;
    data.id = e_VisitorFunction_ToExpression;
    const void* in[1] = {&a_pObjectExpression};
    void*       out[1] = {&pResult};
    data.in = in;
    data.out = out;
    a_pElement->visit(this, data);
    return pResult;
}

Expression* Semantic::toExpression(Field* a_pField, Expression* a_pObjectExpression)
{
    if (a_pObjectExpression->getValueType()->asReference() == nullptr)
    {
        if (a_pObjectExpression->getValueType()->removeQualifiers()->asClassType() == nullptr)
            return nullptr;
        // for example when calling MyClass().myField, we need to create MyClass&& to access the field
        a_pObjectExpression = New<RValueReferenceExpression>(a_pObjectExpression);
    }
    return toExpression((LanguageElement*)a_pField, a_pObjectExpression);
}

Expression* Semantic::toExpression(Property* a_pProperty, Expression* a_pObjectExpression)
{
    if (a_pObjectExpression->getValueType()->asReference() == nullptr)
    {
        if (a_pObjectExpression->getValueType()->removeQualifiers()->asClassType() == nullptr)
            return nullptr;
        a_pObjectExpression = New<RValueReferenceExpression>(a_pObjectExpression);
    }
    a_pObjectExpression =
    convert(a_pObjectExpression, a_pObjectExpression->getValueType()->replicate(a_pProperty->getOwnerClassType()));
    if (a_pObjectExpression == nullptr)
        return nullptr;
    return New<PropertyExpression>(a_pObjectExpression, (Property*)this);
}

void Semantic::deleteConversion(Conversion* a_pConversion)
{
    if (a_pConversion->semantic)
    {
        PHANTOM_SEMANTIC_ASSERT(a_pConversion->semantic == this);
        return;
    }
    a_pConversion->delete_();
}

void Semantic::deleteConversions(ConversionResults& a_Conversions)
{
    for (auto conv : a_Conversions)
        deleteConversion(conv);
}

void Semantic::computeSizeAndAlignment(Type* a_pType)
{
    VisitorData data;
    data.id = e_VisitorFunction_ComputeSizeAndAlignment;
    a_pType->visit(this, data);
}

void Semantic::buildClass(ClassType* a_pClassType, EClassBuildState a_eBuildState)
{
    PHANTOM_SEMANTIC_ASSERT(!a_pClassType->isNative());
    Source* pSource = a_pClassType->getSource();
    if (pSource != getSource()) // soruce do not match => find the semantic associated with it
    {
        // TODO : find a way to void this Compiler dependency here
        auto* pCS = BuildSystem::Get()->getCompiledSource(pSource);
        if (pCS)
            return pCS->getSemantic()->buildClass(a_pClassType, a_eBuildState);
        Message  msg;
        Semantic sema(pSource, &msg);
        return sema.buildClass(a_pClassType, a_eBuildState);
    }

    // ensure we have an intermediate message indicating the current template instantiation source
    auto instantiationSpec = a_pClassType->getTemplateSpecialization();

    if (instantiationSpec)
        instantiationSpec = instantiationSpec->getInstantiationSpecialization();

    Message* pPrevMessage = nullptr;
    if (instantiationSpec)
    {
        pPrevMessage = m_pMessage;
        m_pMessage = new_<Message>(MessageType::Undefined, instantiationSpec->getSource()->getName());
    }

    // start building the class by going step by step until reaching the desired build state
    auto& buildState = a_pClassType->getExtraData()->m_BuildState;
    while (buildState < uint(a_eBuildState))
    {
        buildState = EClassBuildState(buildState + 1);
        VisitorData data;
        data.id = e_VisitorFunction_BuildClass;
        data.in = (void const**)&buildState;
        a_pClassType->visit(this, data);
        // TODO : implement getChildScopes()
        for (auto pMethod : a_pClassType->getMethods())
        {
            for (auto pParam : pMethod->getParameters())
            {
                if (ClassType* pClassType = pParam->getValueType()->removeQualifiers()->asClassType())
                {
                    if (!pClassType->isNative())
                    {
                        buildClass(pClassType, EClassBuildState(buildState));
                    }
                }
            }

            if (Block* pBlock = pMethod->getBlock())
                buildScopeClasses(pBlock, EClassBuildState(buildState));
        }
        buildScopeClasses(a_pClassType, a_eBuildState);
    }
    PHANTOM_SEMANTIC_ASSERT(buildState >= uint(a_eBuildState));
    if (pPrevMessage)
    {
        if (m_pMessage->getChildCount())
            pPrevMessage->addChild(m_pMessage);
        m_pMessage = pPrevMessage;
    }
}

void Semantic::sizeClass(ClassType* a_pClassType)
{
    buildClass(a_pClassType, e_ClassBuildState_Sized);
}

void Semantic::edifyClass(ClassType* a_pClassType)
{
    buildClass(a_pClassType, e_ClassBuildState_ImplicitsAndDefaults);
}

void Semantic::finalizeClass(ClassType* a_pClassType)
{
    buildClass(a_pClassType, e_ClassBuildState_Blocks);
}

void Semantic::buildScopeClasses(Block* a_pBlock, EClassBuildState a_eBuildState)
{
    buildScopeClasses(static_cast<Scope*>(a_pBlock), a_eBuildState);

    for (Statement* pSubStatement : a_pBlock->getStatements())
    {
        if (Block* pSubBlock = pSubStatement->asBlock())
        {
            buildScopeClasses(pSubBlock, a_eBuildState);
        }
    }
}

void Semantic::buildScopeClasses(Scope* a_pScope, EClassBuildState a_eBuildState)
{
    for (auto pType : a_pScope->getTypes())
    {
        if (ClassType* pClassType = pType->asClassType())
            buildClass(pClassType, a_eBuildState);
    }
    for (auto pFunction : a_pScope->getFunctions())
    {
        if (Block* pBlock = pFunction->getBlock())
            buildScopeClasses(pBlock, a_eBuildState);
    }
    auto& specs = a_pScope->getTemplateSpecializations();
    for (size_t i = 0; i < specs.size(); ++i)
    {
        TemplateSpecialization* pSpec = specs[i];
        if (pSpec->isFull() && pSpec->getTemplated())
            if (ClassType* pClassType = pSpec->getTemplated()->asClassType())
                buildClass(pClassType, a_eBuildState);
            else if (auto pSubRoutine = pSpec->getTemplated()->asSubroutine())
                pSubRoutine->buildBlock();
    }
}

bool Semantic::isSymbolAccessible(Symbol* a_pSymbol, LanguageElement* a_pScope) const
{
    if (!(m_bAccessModifiersEnabled) || a_pSymbol->getAccess() == Access::Undefined ||
        a_pSymbol->getAccess() == Access::Public)
        return true;

    VisitorData data;
    data.id = e_VisitorFunction_Access;
    bool        bResult = a_pSymbol->isPublic();
    const void* in[1] = {&a_pSymbol};
    void*       out[1] = {&bResult};
    data.in = in;
    data.out = out;
    a_pScope->visit((Semantic*)this, data);

    // we are accessible in this scope if we are accessible from outer scope
    return bResult;
}

bool Semantic::isFriendWith(Symbol* a_pWith, Symbol* a_pIsFriend) const
{
    LanguageElement* pWith = a_pWith;
    while (pWith)
    {
        if (pWith->hasFriend(a_pIsFriend))
            return true;
        pWith = pWith->getOwner();
    }
    return false;
}

void Semantic::ConversionError(Expression* a_pFrom, Type* a_pTo)
{
    Source* pSource = a_pFrom->getSource();
    if (pSource)
        pushCodeRangeLocation(a_pFrom->getCodeRangeLocation());
    ConversionError(a_pFrom->getValueType(), a_pTo);
    if (pSource)
        popCodeRangeLocation();
}

void Semantic::ConversionError(Type* a_pFrom, Type* a_pTo)
{
    CxxSemanticError("no conversion exists from '%s' to '%s'",
                     FormatCStr(a_pFrom->removeReference()->removeQualifiers()), FormatCStr(a_pTo));
}

void Semantic::_errorToMessage(const CodeRangeLocation& loc, const char* txt)
{
    m_pMessage->errorD(loc, txt);
}

void Semantic::_selectCallCandidateError(StringView name, Symbols const& candidates,
                                         Subroutines const& viableCandidates, Expressions const& arguments)
{
    if (viableCandidates.size())
    {
        CxxSemanticError("'%.*s' : ambiguous call to multiple overloads, argument(s) are (%s), "
                         "possible overloads are :",
                         PHANTOM_STRING_AS_PRINTF_ARG(name),
                         FormatArgumentList(arguments.data(), arguments.size()).data());
        for (size_t i = 0; i < viableCandidates.size(); ++i)
        {
            CxxSemanticSubError("viable : %s", FormatCStr(viableCandidates[i]));
        }
        for (size_t i = 0; i < candidates.size(); ++i)
        {
            if (std::find(viableCandidates.begin(), viableCandidates.end(), candidates[i]) == viableCandidates.end())
                CxxSemanticSubError("not viable : %s", FormatCStr(candidates[i]));
        }
    }
    else
    {
        CxxSemanticError("'%.*s' : no overload found with argument(s) : (%s), "
                         "possible overloads are :",
                         PHANTOM_STRING_AS_PRINTF_ARG(name),
                         FormatArgumentList(arguments.data(), arguments.size()).data());
        for (size_t i = 0; i < candidates.size(); ++i)
        {
            CxxSemanticSubError("%s", FormatCStr(candidates[i]));
        }
    }
}

void Semantic::_resolveTemplateSpecializationArguments(TemplateSpecialization*     a_pInput,
                                                       TemplateSubstitution const& in_TemplateSubstitution,
                                                       EClassBuildState            in_eClassBuildState,
                                                       LanguageElement*            in_pContextScope,
                                                       LanguageElements&           resolvedArguments)
{
    for (auto* pArg : a_pInput->getArguments())
    {
        if (pArg->getMetaClass() == PHANTOM_CLASSOF(TemplateParameterPackTypeExpansion))
        {
            auto             pPackTypeExp = static_cast<TemplateParameterPackTypeExpansion*>(pArg);
            auto             pPackArg = pPackTypeExp->getPackArgument();
            LanguageElements variadicArguments;
            auto             variadicSubs = in_TemplateSubstitution.splitVariadics(pPackArg, variadicArguments);
            for (size_t i = 0; i < variadicArguments.size(); ++i)
            {
                variadicSubs.substituteVariadic(pPackArg, variadicArguments[i]);
                auto pExpandedType = SemanticPrivate::SemanticResolveTemplateDependency<Type>(
                this, pPackTypeExp->getExpansion()->getExpandedElement(), variadicSubs, in_eClassBuildState,
                in_pContextScope, 0, &LanguageElement::asType);
                if (pExpandedType == nullptr)
                    continue;
                resolvedArguments.push_back(pExpandedType);
            }
        }
        else
        {
            auto pResolved = SemanticPrivate::SemanticResolveTemplateDependency<LanguageElement>(
            this, pArg, in_TemplateSubstitution, in_eClassBuildState, in_pContextScope, 0,
            &LanguageElement::asLanguageElement);
            if (!pResolved)
                continue;
            resolvedArguments.push_back(pResolved);
        }
    }
}

} // namespace lang
} // namespace phantom
