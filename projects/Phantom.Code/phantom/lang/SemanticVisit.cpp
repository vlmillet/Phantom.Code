// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
// #define PHANTOM_USE_BOOST_SPIRIT_CPLUSPLUS_PARSER // uncomment to disable parser
#include "Semantic.h"
//
// #include "detail/CxxLexer.h"
// #include "detail/CxxGrammar.h"
// #include "detail/CxxDriver.h"
// #include "detail/CxxTokenizer.h"

#include <phantom/lang/FundamentalsT.h>

#ifndef __DOXYGEN__
#    include <fstream>
#    include <phantom/utils/StringUtil.h>
#    include <stdio.h>
#endif
/* *********************************************** */
#pragma warning(disable : 4800)
#pragma warning(disable : 4244)

#include "AllocateExpression.h"
#include "ArrayExpression.h"
#include "BaseConstructorCallStatement.h"
#include "BinaryOperationExpression.h"
#include "Block.h"
#include "BranchIfNotStatement.h"
#include "BranchIfStatement.h"
#include "BuiltInConversionExpression.h"
#include "BuiltInOperatorExpression.h"
#include "CallExpression.h"
#include "ClassListInitializationExpression.h"
#include "ClassTypeListInitializationExpression.h"
#include "CommaExpression.h"
#include "ConditionalExpression.h"
#include "ConstantExpression.h"
#include "ConstructorCallExpression.h"
#include "DeallocateExpression.h"
#include "DeleteExpression.h"
#include "FieldExpression.h"
#include "FieldInitializationStatement.h"
#include "FieldPointerAdjustmentExpression.h"
#include "FieldPointerExpression.h"
#include "IdentityExpression.h"
#include "InitializerListExpression.h"
#include "LValueInitializationStatement.h"
#include "Label.h"
#include "LoadExpression.h"
#include "LocalVariableExpression.h"
#include "LocalVariableInitializationStatement.h"
#include "Message.h"
#include "MethodPointerAdjustmentExpression.h"
#include "MethodPointerCallExpression.h"
#include "NewExpression.h"
#include "OwnerGuard.h"
#include "PlacementNewExpression.h"
#include "PointerAdjustmentExpression.h"
#include "PropertyExpression.h"
#include "RValueReferenceExpression.h"
#include "RValueToConstLValueExpression.h"
#include "SemanticPrivate.h"
#include "StaticAssert.h"
#include "StringLiteralExpression.h"
#include "StructureExpression.h"
#include "SubroutinePointerExpression.h"
#include "SymbolReferenceExpression.h"
#include "TemplateDependantDeclType.h"
#include "TemplateDependantExpression.h"
#include "TemplateDependantTypeOfExpression.h"
#include "TypeTraitExpression.h"
#include "UnaryPostOperationExpression.h"
#include "UnaryPreOperationExpression.h"
#include "VariableExpression.h"
#include "VirtualMethodTableSetupStatement.h"

#include <phantom/lang/AnonymousStruct.h>
#include <phantom/lang/AnonymousUnion.h>
#include <phantom/lang/Application.h>
#include <phantom/lang/CompiledSource.h>
#include <phantom/lang/ConstVolatileType.h>
#include <phantom/lang/Constructor.h>
#include <phantom/lang/Field.h>
#include <phantom/lang/FieldPointer.h>
#include <phantom/lang/FunctionPointer.h>
#include <phantom/lang/FunctionType.h>
#include <phantom/lang/Import.h>
#include <phantom/lang/InitializerListType.h>
#include <phantom/lang/LValueReference.h>
#include <phantom/lang/LocalVariable.h>
#include <phantom/lang/MapClass.h>
#include <phantom/lang/MemberAnonymousStruct.h>
#include <phantom/lang/MemberAnonymousUnion.h>
#include <phantom/lang/Message.h>
#include <phantom/lang/MethodPointer.h>
#include <phantom/lang/Module.h>
#include <phantom/lang/Namespace.h>
#include <phantom/lang/Package.h>
#include <phantom/lang/PackageFolder.h>
#include <phantom/lang/Parameter.h>
#include <phantom/lang/PlaceholderClass.h>
#include <phantom/lang/PlaceholderClassType.h>
#include <phantom/lang/PlaceholderConstant.h>
#include <phantom/lang/PlaceholderTemplate.h>
#include <phantom/lang/PlaceholderType.h>
#include <phantom/lang/Pointer.h>
#include <phantom/lang/Property.h>
#include <phantom/lang/RValueReference.h>
#include <phantom/lang/SequentialContainerClass.h>
#include <phantom/lang/SetClass.h>
#include <phantom/lang/Signature.h>
#include <phantom/lang/Structure.h>
#include <phantom/lang/TemplateDependantArray.h>
#include <phantom/lang/TemplateDependantElement.h>
#include <phantom/lang/TemplateDependantTemplateInstance.h>
#include <phantom/lang/TemplateDependantType.h>
#include <phantom/lang/TemplateParameter.h>
#include <phantom/lang/TemplateSignature.h>
#include <phantom/lang/Union.h>
#include <phantom/lang/Variable.h>
#include <phantom/lang/VectorClass.h>
#include <phantom/lang/VirtualMethodTable.h>

namespace phantom
{
namespace lang
{
#include "detail/conversions.inl"
}
} // namespace phantom

#include "Compiler.h"
#include "CppLite.h"
#include "ExpressionStatement.h"
#include "ReturnStatement.h"
#include "TemplateParameterPackExpansion.h"
#include "TemplateParameterPackExpressionExpansion.h"
#include "TemplateParameterPackTypeExpansion.h"
#include "VarArgCallExpression.h"

#include <phantom/lang/TypeIndex.h>

#define PHANTOM_SEMANTIC_ASSERT_ElementIsVisible(...)

#pragma optimize("", off)

namespace phantom
{
namespace lang
{
struct StaticExecutionContext : ExecutionContext
{
    virtual bool call(Subroutine*, void**, size_t) override
    {
        PHANTOM_SEMANTIC_ASSERT(false, "The method or operation is not implemented.");
        return false;
    }

    virtual void setNextStatement(Statement*) override
    {
        PHANTOM_SEMANTIC_ASSERT(false, "The method or operation is not implemented.");
    }

    virtual byte* getStackPointer() const override
    {
        PHANTOM_SEMANTIC_ASSERT(false, "The method or operation is not implemented.");
        return nullptr;
    }

    virtual byte* getBasePointer() const override
    {
        PHANTOM_SEMANTIC_ASSERT(false, "The method or operation is not implemented.");
        return nullptr;
    }

    virtual void* getReturnAddress() const override
    {
        PHANTOM_SEMANTIC_ASSERT(false, "The method or operation is not implemented.");
        return nullptr;
    }

    virtual byte* addressOfThis() const override
    {
        PHANTOM_SEMANTIC_ASSERT(false, "The method or operation is not implemented.");
        return nullptr;
    }

    virtual byte* addressOf(LocalVariable*) const override
    {
        PHANTOM_SEMANTIC_ASSERT(false, "The method or operation is not implemented.");
        return nullptr;
    }
};

StringBuffer Semantic::FormatElement(LanguageElement* a_pSymbol)
{
    StringBuffer txt;
    if (a_pSymbol->asSource())
        return StringBuffer(a_pSymbol->getName());
    else
    {
        do
        {
            FormatElement(a_pSymbol, txt);
            a_pSymbol = Object::Cast<Symbol>(a_pSymbol->getOwner());
        } while (txt.empty() && a_pSymbol);
    }
    return txt;
}

void Semantic::FormatElement(LanguageElement* a_pSymbol, StringBuffer& a_Txt)
{
    if (Expression* pExp = a_pSymbol->asExpression())
    {
        FormatElement(pExp->getValueType()->removeReference()->removeQualifiers(), a_Txt);
        return;
    }
    if (a_pSymbol->asSource())
        a_Txt = a_pSymbol->getName();
    else if (!m_NameFormatDelegate.empty())
    {
        m_NameFormatDelegate(a_pSymbol, a_Txt);
    }
    else
    {
        a_pSymbol->getQualifiedDecoratedName(a_Txt);
    }
}

StringBuffer Semantic::FormatArgumentList(Expression* const* a_ppArguments, size_t a_Count)
{
    StringBuffer txt;
    for (size_t i = 0; i < a_Count; ++i)
    {
        if (i)
            txt += ", ";
        FormatElement(a_ppArguments[i]->getValueType()->removeReference(), txt);
    }
    return (StringBuffer &&) txt;
}

StringBuffer Semantic::FormatElementList(LanguageElement* const* a_ppLanguageElements, size_t a_Count)
{
    StringBuffer txt;
    for (size_t i = 0; i < a_Count; ++i)
    {
        if (i)
            txt += ", ";
        FormatElement(a_ppLanguageElements[i], txt);
    }
    return (StringBuffer &&) txt;
}

void Semantic::ErrorTemplateInstantiationFailure(TemplateDependantElement* a_pInput, LanguageElements*, Expressions*)
{
    CxxSemanticError("'%s' : template instantiation failure", FormatCStr(a_pInput));
}

static bool ElementIsVisible(LanguageElement* a_pElement)
{
    if (Symbol* s = a_pElement->asSymbol())
        return (!(s->getVisibility() == Visibility::Private) &&
                (s->getOwner() == nullptr || ElementIsVisible(s->getOwner())));
    return false;
}

// --------------------------------------------

struct NoNumericConversion
{
    static NumericConversion* apply(Semantic*, bool) { return nullptr; }
};

template<typename t_Input, typename t_Output, CastKind a_CastKind>
struct NumericConversionMaker
{
    static NumericConversion* apply(Semantic* a_pSem, bool a_bPromotion)
    {
        return a_pSem->newConv<NumericConversionT<t_Input, t_Output, a_CastKind>>(a_bPromotion);
    }
    static NumericConversion* apply(Semantic* a_pSem, Type* a_pInput, Type* a_pOutput, bool a_bPromotion)
    {
        return a_pSem->newConv<NumericConversionT<t_Input, t_Output, a_CastKind>>(a_pInput, a_pOutput, a_bPromotion);
    }
};

template<typename t_Input, typename t_Output>
struct NumericConversionMaker<t_Input, t_Output, CastKind::Reinterpret> : public NoNumericConversion
{
};

template<typename t_Ty>
struct NumericConversionMaker<t_Ty, t_Ty, CastKind::Reinterpret>
{
    static NumericConversion* apply(Semantic* a_pSem, bool a_bPromotion)
    {
        return a_pSem->newConv<NumericConversionT<t_Ty, t_Ty, CastKind::Reinterpret>>(a_bPromotion);
    }
    static NumericConversion* apply(Semantic* a_pSem, Type* a_pInput, Type* a_pOutput, bool a_bPromotion)
    {
        return a_pSem->newConv<NumericConversionT<t_Ty, t_Ty, CastKind::Reinterpret>>(a_pInput, a_pOutput,
                                                                                      a_bPromotion);
    }
};

template<typename t_Ty, CastKind a_CastKind>
NumericConversion* Semantic::arithmeticConversion(ArithmeticTypeT<t_Ty>* a_pInput, Type* pOutput,
                                                  bool a_bPromotion /*= false*/)
{
    TypeKind outputTypeId = pOutput->getTypeKind();
    switch (outputTypeId)
    {
    case lang::TypeKind::Bool:
        return NumericConversionMaker<t_Ty, bool, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::Char:
        return NumericConversionMaker<t_Ty, char, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::UChar:
        return NumericConversionMaker<t_Ty, uchar, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::SChar:
        return NumericConversionMaker<t_Ty, schar, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::Short:
        return NumericConversionMaker<t_Ty, short, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::UShort:
        return NumericConversionMaker<t_Ty, ushort, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::Int:
        return NumericConversionMaker<t_Ty, int, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::UInt:
        return NumericConversionMaker<t_Ty, uint, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::Long:
        return NumericConversionMaker<t_Ty, long, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::ULong:
        return NumericConversionMaker<t_Ty, ulong, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::LongLong:
        return NumericConversionMaker<t_Ty, longlong, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::ULongLong:
        return NumericConversionMaker<t_Ty, ulonglong, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::Float:
        return NumericConversionMaker<t_Ty, float, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::Double:
        return NumericConversionMaker<t_Ty, double, a_CastKind>::apply(this, a_bPromotion);
    case lang::TypeKind::LongDouble:
        return NumericConversionMaker<t_Ty, longdouble, a_CastKind>::apply(this, a_bPromotion);
#if PHANTOM_HAS_BUILT_IN_WCHAR_T
    case lang::TypeKind::WChar:
        return NumericConversionMaker<t_Ty, wchar_t, a_CastKind>::apply(this, a_bPromotion);
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR16_T
    case lang::TypeKind::Char16:
        return NumericConversionMaker<t_Ty, char16_t, a_CastKind>::apply(this, a_bPromotion);
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR32_T
    case lang::TypeKind::Char32:
        return NumericConversionMaker<t_Ty, char32_t, a_CastKind>::apply(this, a_bPromotion);
#endif
    case lang::TypeKind::Enum:
    {
        Enum* pOutputEnum = static_cast<Enum*>(pOutput);
        if (a_CastKind == CastKind::Implicit) // no implicit conversion from arithmetic to enum
            return nullptr;
        NumericConversion* conv =
        arithmeticConversion<t_Ty, a_CastKind>(a_pInput, pOutputEnum->getUnderlyingIntType(), a_bPromotion);
        if (conv)
            conv->output = pOutputEnum;
        return conv;
    }
    default:
        break;
    }
    return nullptr;
}

template<typename t_Ty, CastKind a_CastKind>
NumericConversion* Semantic::floatingPointConversion(FloatingPointTypeT<t_Ty>* a_pInput, Type* pOutput,
                                                     bool a_bPromotion /*= false*/)
{
    return arithmeticConversion<t_Ty, a_CastKind>(a_pInput, pOutput, a_bPromotion);
}

template<typename t_Ty, CastKind a_CastKind>
Conversion* Semantic::floatingPointConversion(FloatingPointTypeT<t_Ty>* a_pInput, Type* pOutput, bool a_bPromotion,
                                              ERefType refType)
{
    NumericConversion* pConversion = floatingPointConversion<t_Ty, a_CastKind>(a_pInput, pOutput, a_bPromotion);
    if (pConversion)
    {
        if ((refType != e_RefType_None))
            return newConv<DefaultConversionSequence>(pConversion,
                                                      newConv<RefConversion>(pOutput, refType == e_RefType_RValue));
        return newConv<DefaultConversionSequence>(pConversion); // canonical conversion
    }
    return nullptr;
}

template<typename t_Ty, CastKind a_CastKind>
struct any_to_ptr_conversion
{
    static NumericConversion* apply(Semantic* a_pSem, Type* a_pOutput, bool a_bPromotion)
    {
        return a_pSem->newConv<NumericConversionT<t_Ty, void*, a_CastKind>>(PHANTOM_TYPEOF(t_Ty), a_pOutput,
                                                                            a_bPromotion);
    }
};

template<typename t_Ty>
struct any_to_ptr_conversion<t_Ty, CastKind::Static>
{
    static NumericConversion* apply(Semantic*, Type*, bool) { return nullptr; }
};

template<typename t_Ty>
struct any_to_ptr_conversion<t_Ty, CastKind::Implicit>
{
    static NumericConversion* apply(Semantic*, Type*, bool) { return nullptr; }
};

template<typename t_Ty, CastKind a_CastKind>
NumericConversion* Semantic::integralConversion(IntegralTypeT<t_Ty>* a_pInput, Type* pOutput, bool a_bPromotion)
{
    NumericConversion* pConversion = arithmeticConversion<t_Ty, a_CastKind>(a_pInput, pOutput, a_bPromotion);
    if (pConversion)
        return pConversion;
    TypeKind outputTypeId = pOutput->getTypeKind();
    switch (outputTypeId)
    {
    case lang::TypeKind::Pointer:
    case lang::TypeKind::FunctionPointer:
    {
        if (!std::is_same<bool, t_Ty>::value)
        {
            return any_to_ptr_conversion<t_Ty, a_CastKind>::apply(this, pOutput, a_bPromotion);
        }
        break;
    }
    case lang::TypeKind::Enum:
    {
        if (a_CastKind == CastKind::Explicit || a_CastKind == CastKind::Static)
        {
            return integralConversion<t_Ty, a_CastKind>(a_pInput, pOutput->asEnum()->getUnderlyingIntType(),
                                                        a_bPromotion);
        }
    }
    default:
        break;
    }
    return nullptr;
}

bool Semantic::_useExplicitTemplateArguments(Subroutine* a_pInput, TemplateSpecialization* a_pTSpec,
                                             OptionalArrayView<LanguageElement*> a_ExplicitTemplateArguments,
                                             PlaceholderMap& a_Deductions, const char* a_SubroutineKind)
{
    if (a_ExplicitTemplateArguments)
    {
        auto& tparams = a_pTSpec->getTemplate()->getTemplateParameters();
        if (a_ExplicitTemplateArguments->size() > tparams.size())
        {
            CxxSemanticError("too many template arguments to '%.*s'",
                             PHANTOM_STRING_AS_PRINTF_ARG(a_pInput->getName()));
            return false;
        }
        size_t i = 0;
        for (; i < a_ExplicitTemplateArguments->size(); ++i)
        {
            a_Deductions[a_pTSpec->getArgument(i)->asPlaceholder()] = (*a_ExplicitTemplateArguments)[i];
        }
    }
    return true;
}

LanguageElement* Semantic::_findInstantiation(TemplateSubstitution const& a_TSubs, Evaluable* a_pPrimary)
{
    if (auto instantiation = a_TSubs.getInstantiation())
    {
        return templateInstantiations()[instantiation][a_pPrimary];
    }
    return nullptr;
}
LanguageElement* Semantic::_findInstantiation(TemplateSubstitution const& a_TSubs, LanguageElement* a_pPrimary)
{
    if (auto instantiation = a_TSubs.getInstantiation())
    {
        auto& instance = templateInstantiations()[instantiation][a_pPrimary];
        if (instance)
            return instance;
        if (!a_pPrimary->isTemplateDependant())
        {
            PHANTOM_SEMANTIC_ASSERT(!a_pPrimary->isTemplateElement());
            return instance = a_pPrimary;
        }
    }
    if (!a_pPrimary->isTemplateDependant())
    {
        PHANTOM_SEMANTIC_ASSERT(!a_pPrimary->isTemplateElement());
        return a_pPrimary;
    }
    return nullptr;
}

template<typename t_Ty, CastKind a_CastKind>
Conversion* Semantic::integralConversion(IntegralTypeT<t_Ty>* a_pInput, Type* pOutput, bool a_bPromotion,
                                         ERefType refType)
{
    NumericConversion* pConversion = integralConversion<t_Ty, a_CastKind>(a_pInput, pOutput, a_bPromotion);
    if (pConversion)
    {
        if ((refType != e_RefType_None))
            return newConv<DefaultConversionSequence>(pConversion,
                                                      newConv<RefConversion>(pOutput, refType == e_RefType_RValue));
        return newConv<DefaultConversionSequence>(pConversion); // canonical conversion
    }
    return nullptr;
}

void Semantic::visit(Alias* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        EClassBuildState  in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*  in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*& pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnerGuard<Alias> pInstantiatedAlias = nullptr;

        if (in_eClassBuildState == e_ClassBuildState_Members)
        {
            if (auto pAS = a_pInput->getAliasedSymbol())
            {
                Symbol* pSymbol = o_resolveT(Symbol, pAS);
                if (pSymbol == nullptr)
                    return;
                pInstantiatedAlias = New<Alias>(pSymbol, a_pInput->getName(), a_pInput->getModifiers(),
                                                a_pInput->getFlags() & ~PHANTOM_R_FLAG_TEMPLATE_DEPENDANT);
            }
            else
            {
                pInstantiatedAlias = New<Alias>(a_pInput->getName(), a_pInput->getModifiers(),
                                                a_pInput->getFlags() & ~PHANTOM_R_FLAG_TEMPLATE_DEPENDANT);
            }
            CxxSemanticCheckSymbolName(a_pInput->getName(), in_pContextScope);
            in_pContextScope->asScope()->addAlias(pInstantiatedAlias);
            pInstanciated = pInstantiatedAlias;
        }
    }
        return;
    }
    if (auto pAS = a_pInput->getAliasedSymbol())
    {
        pAS->visit(this, a_Data);
    }
    else
        visit(static_cast<Symbol*>(a_pInput), a_Data);
}

void Semantic::visit(AllocateExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Type*                       pType = o_resolveT(Type, a_pInput->getType());
        if (pType == nullptr)
            return;
        if (pType->asReference())
        {
            CxxSemanticError("'%s' references cannot be allocated", FormatCStr(pType));
            return;
        }
        OwnerGuard<Expression> pExp = o_instantiateT(Expression, a_pInput->getSizeExpression());
        if (pExp == nullptr)
            return;
        Expression* pConv = CxxSemanticConversionNE(pExp, PHANTOM_TYPEOF(size_t));
        if (pConv == nullptr)
            return;
        pInstanciated = New<AllocateExpression>(pType, pConv, a_pInput->isOnStack());
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(AnonymousSection* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(AnonymousStruct* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        AnonymousStruct*            pAnonymousStruct = New<AnonymousStruct>();
        for (auto p : a_pInput->getVariables())
        {
            pAnonymousStruct->addVariable(o_resolveT(Variable, p));
        }
        pInstanciated = pAnonymousStruct;
    }
        return;
    }
    visit(static_cast<AnonymousSection*>(a_pInput), a_Data);
}
void Semantic::visit(AnonymousUnion* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        AnonymousUnion*             pAnonymousUnion = New<AnonymousUnion>();
        for (auto p : a_pInput->getVariables())
        {
            pAnonymousUnion->addVariable(o_resolveT(Variable, p));
        }
        pInstanciated = pAnonymousUnion;
    }
        return;
    }
    visit(static_cast<AnonymousSection*>(a_pInput), a_Data);
}

Type* deduceConditionalResultType(Type* pThenType, Type* pElseType, Expression* pThenExpression,
                                  Expression* pElseExpression)
{
    // If both expressions are of the same type, the result is of that type.
    if (pThenType->isSame(pElseType))
    {
        return pThenType;
    }
    else if (pThenType->asLValueReference() && pElseType->asLValueReference())
    {
        Type* pCommon = pThenType->asLValueReference()->getReferencedType()->getCommonBaseAncestor(
        pElseType->asLValueReference()->getReferencedType());
        if (pCommon == nullptr)
            return deduceConditionalResultType(pThenType->removeReference()->removeQualifiers(),
                                               pElseType->removeReference()->removeQualifiers(), pThenExpression,
                                               pElseExpression);
        return pCommon->makeLValueReference();
    }
    else if (pThenType->asArithmeticType() && pElseType->asArithmeticType())
    {
        return pThenType->getTypeKind() > pElseType->getTypeKind() ? pThenType : pElseType;
    }
    // If both expressions are of pointer types or if one is a pointer type and the other is a
    // constant expression that evaluates to 0, pointer conversions are performed to convert them to
    // a common type.
    else if (pThenType->asPointer() && pElseExpression->as<ConstantExpression>() &&
             pElseExpression->as<ConstantExpression>()->getConstant()->isIntegralZero())
    {
        return pThenType;
    }
    else if (pElseType->asPointer() && pThenExpression->as<ConstantExpression>() &&
             pThenExpression->as<ConstantExpression>()->getConstant()->isIntegralZero())
    {
        return pElseType;
    }
    else if (pThenType->asPointer() && pElseType->asPointer())
    {
        Type* pCommon =
        pThenType->asPointer()->getPointeeType()->getCommonBaseAncestor(pElseType->asPointer()->getPointeeType());
        return (pCommon == nullptr) ? PHANTOM_TYPEOF(void*) : pCommon->makePointer();
    }
    // If both expressions are of reference types, reference conversions are performed to convert
    // them to a common type.

    else if (pThenType->asRValueReference() && pElseType->asRValueReference())
    {
        Type* pCommon = pThenType->asRValueReference()->getReferencedType()->getCommonBaseAncestor(
        pElseType->asRValueReference()->getReferencedType());
        return (pCommon == nullptr) ? pThenType : pCommon->makeRValueReference();
    }
    return pThenType;
}

void Semantic::visit(Application* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_QualifiedLookup:
    case e_VisitorFunction_UnqualifiedLookup:
    {
        LanguageElement*& out_pResult = *(LanguageElement**)a_Data.out[0];
        visit(static_cast<Symbol*>(a_pInput), a_Data);
        if (out_pResult == nullptr)
        {
            return Namespace::Global()->visit(this, a_Data);
        }
    }
        return;
    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView       in_name = *(StringView*)a_Data.in[0];
        Symbols&         out_candidates = *(Symbols*)a_Data.out[0];
        LanguageElement* pElement = a_pInput->getBuiltInType(in_name);
        if (pElement)
        {
            PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((pElement));
            out_candidates.push_back(static_cast<Symbol*>(pElement));
            return;
        }
        for (auto& p : a_pInput->getElements())
        {
            if ((p)->asAlias())
            {
                if (static_cast<Alias*>(p)->getName() == in_name)
                {
                    PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((p));
                    out_candidates.push_back(static_cast<Alias*>(p));
                }
            }
        }
    }
    break;
    case e_VisitorFunction_BinaryOperator:
    case e_VisitorFunction_UnaryPreOperator:
    case e_VisitorFunction_UnaryPostOperator:
    {
        StringView       in_op = *(StringView*)a_Data.in[0];
        Expression**     in_ppArguments = (Expression**)a_Data.in[1];
        LanguageElement* in_pContextScope = *(LanguageElement**)a_Data.in[2];
        Expression*&     out_pExpression = *(Expression**)a_Data.out[0];
        if (in_op == "->" &&
            in_ppArguments[0]
            ->getValueType()
            ->removeReference()
            ->removeQualifiers()
            ->asPointer()) // default -> operator on pointers
        {
            out_pExpression = in_ppArguments[0];
            return;
        }
        if (in_op == ",")
        {
            out_pExpression = New<CommaExpression>(in_ppArguments[0], in_ppArguments[1]);
            return;
        }
        else if (in_op == "?:")
        {
            Expression* pTest = CxxSemanticConversion(in_ppArguments[0], PHANTOM_TYPEOF(bool));
            if (pTest == nullptr)
                return;
            Expression* pThenExpression = in_ppArguments[1];
            Expression* pElseExpression = in_ppArguments[2];

            Type* pThenType = pThenExpression->getValueType();
            Type* pElseType = pElseExpression->getValueType();
            Type* pResultType = deduceConditionalResultType(pThenType, pElseType, pThenExpression, pElseExpression);

            OwnerGuard<Expression> pConvertedThen = CxxSemanticConversion(pThenExpression, pResultType);
            if (pConvertedThen == nullptr)
                return;
            OwnerGuard<Expression> pConvertedElse = CxxSemanticConversion(pElseExpression, pResultType);
            if (pConvertedElse == nullptr)
                return;
            out_pExpression = New<ConditionalExpression>(pTest, pConvertedThen, pConvertedElse);
            return;
        }
        BuiltInOperator* pOperator = a_Data.id == e_VisitorFunction_BinaryOperator
        ? getBuiltInBinaryOperator(in_op)
        : a_Data.id == e_VisitorFunction_UnaryPreOperator ? getBuiltInPreUnaryOperator(in_op)
                                                          : getBuiltInPostUnaryOperator(in_op);
        if (!pOperator)
        {
            CxxSemanticError("'%s' : unknown built in operator", (const char*)in_op.data());
            out_pExpression = nullptr;
            return;
        }

        PHANTOM_SEMANTIC_ASSERT(in_ppArguments);

        switch (a_Data.id)
        {
        case e_VisitorFunction_BinaryOperator:
            if (in_ppArguments[0]->isTemplateDependant())
            {
                out_pExpression = New<BinaryOperationExpression>(in_ppArguments[0]->getValueType(), in_op,
                                                                 in_ppArguments[0], in_ppArguments[1]);
                return;
            }
            else if (in_ppArguments[1]->isTemplateDependant())
            {
                out_pExpression = New<BinaryOperationExpression>(in_ppArguments[1]->getValueType(), in_op,
                                                                 in_ppArguments[0], in_ppArguments[1]);
                return;
            }
            break;
        case e_VisitorFunction_UnaryPreOperator:
            if (in_ppArguments[0]->isTemplateDependant())
            {
                out_pExpression =
                New<UnaryPreOperationExpression>(in_ppArguments[0]->getValueType(), in_op, in_ppArguments[0]);
                return;
            }
            break;
        case e_VisitorFunction_UnaryPostOperator:
            if (in_ppArguments[0]->isTemplateDependant())
            {
                out_pExpression =
                New<UnaryPostOperationExpression>(in_ppArguments[0]->getValueType(), in_op, in_ppArguments[0]);
                return;
            }
            break;
        }

        if (pOperator)
        {
            size_t                                 operandCount = pOperator->getOperandCount();
            SmallVector<BuiltInOperator::Overload> overloads;
            SmallVector<BuiltInOperator::Overload> viableOverloads;
            SmallVector<ConversionResults>         overloadConversions;
            SmallVector<ConversionResults*>        viableImplicitConversions;
            pOperator->newImplicitConversions(in_ppArguments, overloadConversions, overloads);
            for (size_t i = 0; i < overloadConversions.size(); ++i)
            {
                if (overloadConversions[i].hasNull())
                    continue;
                viableImplicitConversions.push_back(&overloadConversions[i]);
                viableOverloads.push_back(overloads[i]);
            }

            if (viableImplicitConversions.size())
            {
                /// Find best viable operator or display ambiguity error
                ConversionResults*         pBest = nullptr;
                BuiltInOperator::Overload* pBestOverload = nullptr;
                if (viableImplicitConversions.size() == 1)
                {
                    pBest = viableImplicitConversions.back();
                    pBestOverload = &viableOverloads.back();
                }
                else
                {
                    size_t bestIndex = 0;
                    for (size_t i = 0; i < viableImplicitConversions.size(); ++i)
                    {
                        int    bestResult = -1;
                        size_t j = 0;
                        for (; j < viableImplicitConversions.size(); ++j)
                        {
                            if (i == j)
                                continue;
                            int result = viableImplicitConversions[i]->compare(*viableImplicitConversions[j]);
                            PHANTOM_SEMANTIC_ASSERT(
                            viableImplicitConversions[j]->compare(*viableImplicitConversions[i]) ==
                            -result); /// compare() coherence test
                            if (result >= 0)
                                bestResult = std::max(bestResult, result);
                            else
                                break;
                        }
                        if (j == viableImplicitConversions.size()) /// superior or equal to every one (not worst to any)
                        {
                            if (bestResult == 1)
                            {
                                /// found a best match (superior to at least one other signature and
                                /// equal to the others)
                                pBest = viableImplicitConversions[i];
                                pBestOverload = &viableOverloads[i];
                                goto end_of_selection;
                            }
                        }
                    }
                }
            end_of_selection:
                if (pBest)
                {
                    PHANTOM_SEMANTIC_ASSERT(pBestOverload);
                    PHANTOM_SEMANTIC_ASSERT(pBest->size() == operandCount);
                    Expression* arguments[3];
                    for (size_t k = 0; k < operandCount; ++k)
                    {
                        arguments[k] = (*pBest)[k]->convert(this, in_ppArguments[k]); /// convert arguments
                        if (in_ppArguments[k]
                            ->getValueType()
                            ->removeReference()
                            ->removeQualifiers()
                            ->asClassType()) // one input argument is a class type
                        {
                            in_ppArguments[k] =
                            arguments[k]; // replace input by eval argument (user defined conversion happened here)
                        }
                    }
                    out_pExpression = New<BuiltInOperatorExpression>(pOperator, pBestOverload->result, in_ppArguments,
                                                                     arguments, pBestOverload->delegate);
                }
                else
                {
                    CxxSemanticError("'%s' : ambiguous call to multiple overloads, argument(s) are "
                                     "(%s), viable overloads are :",
                                     (const char*)in_op.data(),
                                     FormatArgumentList(in_ppArguments, operandCount).data());
                    for (size_t i = 0; i < viableOverloads.size(); ++i)
                    {
                        CxxSemanticSubError(
                        "(%s)", FormatElementList((LanguageElement**)&viableOverloads[i].op0, operandCount).data());
                    }
                }
            }
            else
            {
                if (in_op == "=" &&
                    (!in_ppArguments[1]->getValueType()->removeReference()->removeQualifiers()->isSame(
                    in_ppArguments[0]->getValueType()->removeReference()->removeQualifiers())))
                {
                    OwnerGuard<Expression> pConv =
                    CxxSemanticConversionNE(in_ppArguments[1], in_ppArguments[0]->getValueType()->removeReference());
                    if (pConv)
                    {
                        out_pExpression = solveBinaryOperator(in_op, in_ppArguments[0], pConv);
                        if (out_pExpression == nullptr && in_ppArguments[1]->getOwner())
                            in_ppArguments[1]->detach();
                    }
                }
                if (out_pExpression == nullptr)
                {
                    CxxSemanticError("'%s' : no viable overload found, argument(s) are (%s)", (const char*)in_op.data(),
                                     FormatArgumentList(in_ppArguments, operandCount).data());
                }
            }
            pOperator->deleteConversions(overloadConversions);
        }
    }
    break;
    }
}
void Semantic::visit(ArrayExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnersGuard<Expressions>    expressions;
        for (auto pValue : a_pInput->getValues())
        {
            Expression* pExpression = o_instantiateT(Expression, pValue);
            if (pExpression == nullptr)
                return;
            Expression* pConv =
            CxxSemanticConversionNE(pExpression, static_cast<Array*>(a_pInput->getValueType())->getItemType());
            if (pConv == nullptr)
            {
                Delete(pExpression);
                return;
            }
            expressions.push_back(pConv);
        }
        pInstanciated = New<ArrayExpression>(static_cast<Array*>(a_pInput->getValueType()), expressions);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(Array* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ComputeSizeAndAlignment:
        a_pInput->getItemType()->visit(this, a_Data);
        if (a_pInput->getItemCount())
        {
            a_pInput->setSize(a_pInput->getItemType()->getSize() * a_pInput->getItemCount());
            a_pInput->setAlignment(a_pInput->getItemType()->getAlignment());
        }
        else
        {
            PHANTOM_SEMANTIC_ASSERT(a_pInput->getSize() && a_pInput->getAlignment());
        }
        return;

    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*  pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        Array*            pArray = pArgument->asArray();
        if (pArray)
        {
            if (pArray->isTemplateDependant())
            {
                return;
            }
            if (pArray->getItemCount() == a_pInput->getItemCount())
            {
                Type* pItemDeducedType =
                templateArgumentDeduction(a_pInput->getItemType(), pArray->getItemType(), deductions);
                if (pItemDeducedType)
                {
                    pDeducedArgument = pItemDeducedType->makeArray(a_pInput->getItemCount());
                }
            }
        }
        break;
    }
        return;
    case e_VisitorFunction_CallTemplateArgumentDeduction:
    {
        LanguageElement*  pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        Array*            pArray = pArgument->asArray();
        if (pArray)
        {
            if (pArray->isTemplateDependant())
            {
                return;
            }
            if (pArray->getItemCount() == a_pInput->getItemCount())
            {
                Type* pItemDeducedType =
                templateArgumentDeduction(a_pInput->getItemType(), pArray->getItemType(), deductions);
                if (pItemDeducedType)
                {
                    pDeducedArgument = pItemDeducedType->makeArray(a_pInput->getItemCount());
                }
            }
        }
        break;
    }
        return;

    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        Type*                       pNewItemType = o_resolveT(Type, a_pInput->getItemType());
        pResolved = pNewItemType ? pNewItemType->makeArray(a_pInput->getItemCount()) : nullptr;
    }
        return;

    case e_VisitorFunction_TypeConversion:
    {
        Type*                   pOutput = *(Type* const*)a_Data.in[0];
        CastKind                castKind = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions    iUserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        LanguageElement*        pContextScope = *(LanguageElement**)a_Data.in[3];
        Conversion*&            conv = *(Conversion**)a_Data.out[0];
        std::pair<Type*, Type*> constAdjustment;
        int                     adjustmentCount = 0;
        Type*                   pInputArrayItemType = a_pInput->getItemType();
        if (pOutput->getTypeKind() == TypeKind::Array)
        {
            Array* pOutputArray = static_cast<Array*>(pOutput);
            Type*  pOutputArrayItemType = pOutputArray->getItemType();
            if (pOutputArray->getItemCount() == a_pInput->getItemCount() /// output array has same bound
                || pOutputArray->getItemCount() == 0)                    /// output array has unknown bound
            {
                if (pOutputArrayItemType->asConstType())
                {
                    /// X[N] -> X const[N]
                    Array* adjustmentInput =
                    pOutputArrayItemType->removeConst()->makeArray(pOutputArray->getItemCount());
                    constAdjustment.first = adjustmentInput;
                    /// merge const adjustments
                    constAdjustment.second = pOutputArray;
                    adjustmentCount++;
                    pOutputArray = adjustmentInput;
                    pOutputArrayItemType = pOutputArray->getItemType();
                }
                else if (pInputArrayItemType->asConstType())
                {
                    return; /// X const[N] -/-> X[N]
                }
                if (pOutputArrayItemType == a_pInput->getItemType())
                {
                    if (adjustmentCount)
                        conv = newConv<DefaultConversionSequence>(newConv<QualificationAdjustConversion>(
                        constAdjustment.first, constAdjustment.second, adjustmentCount));
                    conv = newConv<DefaultConversionSequence>(a_pInput,
                                                              (ConversionVal)pOutput->removeQualifiers()); /// trivial
                    return; /// success = X[N] -> X const[N]
                }
                if ((pInputArrayItemType->asClass() != nullptr) && (pOutputArrayItemType->asClass() != nullptr))
                {
                    lang::Class* pInputClass = static_cast<lang::Class*>(pInputArrayItemType);
                    lang::Class* pOutputClass = static_cast<lang::Class*>(pOutputArrayItemType);
                    size_t       level = pInputClass->getInheritanceLevelFromBase(pOutputClass);
                    if (level != ~size_t(0)) // level != -1 => inheritance relation => pointers are related
                    {
                        auto* pPtr = newConv<PointerConversion>(a_pInput, pOutputArrayItemType, level,
                                                                pInputClass->getBaseClassOffsetCascade(pOutputClass));
                        if (adjustmentCount)
                            conv = newConv<DefaultConversionSequence>(
                            pPtr,
                            newConv<QualificationAdjustConversion>(constAdjustment.first, constAdjustment.second,
                                                                   adjustmentCount));
                        conv = newConv<DefaultConversionSequence>(pPtr);
                        return; /// success Derived[N] -> Base (const)[N]
                    }
                }
            }
        }
        /// Array to pointer conversion
        else if (pOutput->asPointer())
        {
            Pointer* pOutputPointerType = static_cast<Pointer*>(pOutput);
            a_pInput->removeAllExtents()->makePointer()->visit(this, a_Data);
            if (conv) /// success X[N] -> Y*
            {
                conv->input = a_pInput;
            }
        }
    }
        return;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void Semantic::visit(BaseConstructorCallStatement* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        PHANTOM_SEMANTIC_ASSERT(pInstanciated == nullptr);
        Class*      pResolvedClass = o_resolveT(Class, a_pInput->getDerivedClass());
        Expression* pResolvedExpr = o_instantiateT(Expression, a_pInput->getConstructorCallExpression());
        pInstanciated = New<BaseConstructorCallStatement>(pResolvedClass, pResolvedExpr);
    }
        return;
    default:
        break;
    }
    visit(static_cast<Statement*>(a_pInput), a_Data);
}
void Semantic::visit(Block* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_Access:
    {
        a_pInput->getSubroutine()->visit(this, a_Data);
    }
        return;
    case e_VisitorFunction_QualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        Type*                               in_pInitializationType = (Type*)a_Data.in[4];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];
        if (!in_pTemplateArguments && !in_pFunctionArguments)
        {
            for (LocalVariable* pLocal : a_pInput->getLocalVariables())
            {
                if (pLocal->getName() == in_name)
                {
                    out_pResult = pLocal;
                    return;
                }
            }
        }
        visit(static_cast<Statement*>(a_pInput), a_Data);
    }
        return;

    case e_VisitorFunction_UnqualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        Type*                               in_pInitializationType = (Type*)a_Data.in[4];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];
        if (!in_pTemplateArguments)
        {
            LocalVariable* pLocal = a_pInput->getLocalVariableCascade(in_name);
            if (pLocal)
            {
                if (in_pFunctionArguments)
                {
                    OwnerGuard<Expression> exp = New<LocalVariableExpression>(pLocal);
                    out_pResult = internalQualifiedLookup(exp, "operator()", NullOpt, in_pFunctionArguments, a_pInput,
                                                          in_pInitializationType);
                    return;
                }
                else
                    out_pResult = New<LocalVariableExpression>(pLocal);
                return;
            }
        }
        if (!(a_pInput->isTemplateElement())) /// if template element, 'this' is template dependant
                                              /// and cannot be used in unqualified lookup
        {
            LocalVariable* pThis = a_pInput->getLocalVariable("this");
            if (pThis)
            {
                LanguageElement* in_pContextScope = *(LanguageElement**)a_Data.in[3];
                Type*            in_pInitializationType = (Type*)a_Data.in[4];
                Expression*      pThisExpression = toExpression(pThis);
                pThisExpression->setImplicit();
                auto deref = pThisExpression->dereference(getSource());
                deref->setImplicit();
                out_pResult = silentQualifiedLookup(deref, in_name, in_pTemplateArguments, in_pFunctionArguments,
                                                    in_pContextScope, in_pInitializationType, a_Data.flags);
                if (out_pResult)
                    return;
                // Delete(pThisExpression);
            }
        }
        else
        {
            if (ClassType* pEnclClassType = a_pInput->getEnclosingClassType()) // inside a class
            {
                LocalVariable* pThis = a_pInput->getLocalVariable("this");
                if (pThis) // inside a method
                {
                    if (in_pFunctionArguments == nullptr)
                    {
                        if (Field* pField = pEnclClassType->getField(in_name))
                        {
                            LanguageElement* in_pContextScope = *(LanguageElement**)a_Data.in[3];
                            Type*            in_pInitializationType = (Type*)a_Data.in[4];
                            Expression*      pThisExpression = toExpression(pThis);
                            out_pResult = New<FieldExpression>(pThisExpression->dereference(getSource()), pField);
                            return;
                        }
                    }
                }
            }
        }
        visit(static_cast<Statement*>(a_pInput), a_Data);
    }
        return;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        LocalVariable*              pThis = a_pInput->getLocalVariable("this");
        if (pInstanciated == nullptr)
        {
            PHANTOM_SEMANTIC_ASSERT(in_pContextScope->asBlock());
            pInstanciated = static_cast<Block*>(in_pContextScope)->newBlock();
        }
        if (in_eClassBuildState == e_ClassBuildState_Blocks)
        {
            Block* pBlock = static_cast<Block*>(pInstanciated);
            PHANTOM_SEMANTIC_ASSERT(pBlock->getBlockStatementCount() == 0);
            PHANTOM_SEMANTIC_ASSERT(pBlock != a_pInput);
            for (LocalVariable* pLocal : a_pInput->getLocalVariables())
            {
                if (pLocal->getName() != "this" && pLocal->asParameter() == nullptr)
                {
                    Type* pType = pLocal->getValueType();
                    if (pType->hasAuto()) // auto
                    {
                        LocalVariableInitializationStatement* pStatement =
                        a_pInput->findLocalVariableInitializationStatement(pLocal);
                        PHANTOM_SEMANTIC_ASSERT(pStatement);
                        Expression* pExp =
                        o_instantiateT(Expression, pStatement->getInitializationExpression(), pBlock);
                        if (pExp == nullptr)
                            continue;
                        pType = autoDeduction(pType, pExp->getValueType());
                        Delete(pExp); // we don't need this expression anymore as it will be reinstantiated
                                      // when instancing LocalVariableInitializationStatement
                        if (pType == nullptr)
                            return;
                    }
                    else
                    {
                        pType = o_resolveT(Type, pType, pBlock);
                    }
                    if (pType == nullptr)
                    {
                        return;
                    }
                    auto pInstanciated = createLocalVariable(pType, pLocal->getName(), pLocal->getModifiers());
                    templateInstantiations()[in_TemplateSubstitution.getInstantiation()][pLocal] = pInstanciated;
                    pBlock->addLocalVariable(pInstanciated);
                }
            }
            for (Statement* pStatement : a_pInput->getStatements())
            {
                Statement* pNewStatement = o_instantiateT(Statement, pStatement, pBlock);
                if (pNewStatement)
                {
                    if (pNewStatement->asBlock() == nullptr)
                    {
                        pBlock->addStatement(pNewStatement);
                    }
                }
                else
                {
                    pInstanciated = nullptr; // TODO : improve all the error handling system to avoid this kind of sh*t
                }
            }
            if (!pInstanciated)
                return;
            visit(static_cast<Scope*>(a_pInput), a_Data);
        }
    }
        return;
    }
    visit(static_cast<Statement*>(a_pInput), a_Data);
}
void Semantic::visit(BranchIfNotStatement* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnerGuard<Expression>      pExp = o_instantiateT(Expression, a_pInput->getExpression());
        if (pExp == nullptr)
            return;
        Expression* pConv = CxxSemanticConversionNE(pExp, PHANTOM_TYPEOF(bool));
        if (pConv == nullptr)
            return;
        Label* pLabel = o_findT(Label, a_pInput->getLabel());
        if (pLabel == nullptr)
        {
            o_mapT(a_pInput->getLabel(),
                   pLabel = New<Label>(a_pInput->getLabel()->getLabelName(), a_pInput->getLabel()->getTag()));
        }
        pInstanciated = New<BranchIfNotStatement>(pConv, pLabel);
    }
        return;
    }
    visit(static_cast<BranchStatement*>(a_pInput), a_Data);
}
void Semantic::visit(BranchIfStatement* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnerGuard<Expression>      pExp = o_instantiateT(Expression, a_pInput->getExpression());
        if (pExp == nullptr)
            return;
        Expression* pConv = CxxSemanticConversionNE(pExp, PHANTOM_TYPEOF(bool));
        if (pConv == nullptr)
            return;
        Label* pLabel = o_findT(Label, a_pInput->getLabel());
        pInstanciated =
        New<BranchIfStatement>(pConv, pLabel ? pLabel : o_instantiate_asT(Label, a_pInput->getLabel(), a_pInput));
    }
        return;
    }
    visit(static_cast<BranchStatement*>(a_pInput), a_Data);
}
void Semantic::visit(BranchStatement* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Label*                      pLabel = o_findT(Label, a_pInput->getLabel());
        pInstanciated =
        New<BranchStatement>(pLabel ? pLabel : o_instantiate_asT(Label, a_pInput->getLabel(), a_pInput));
    }
        return;
    }
    visit(static_cast<Statement*>(a_pInput), a_Data);
}

void Semantic::visit(BuiltInConversionExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (a_pInput->isTemplateDependant())
        {
            OwnerGuard<Expression> pExp = o_instantiateT(Expression, a_pInput->getInputExpression());
            if (pExp == nullptr)
                return;
            PHANTOM_SEMANTIC_ASSERT(!(pExp->isTemplateDependant()));
            Type* pType = a_pInput->getValueType();
            if (pType->hasAuto())
            {
                pType = autoDeduction(pType, pExp->getValueType());
            }
            else
            {
                pType = o_resolveT(Type, pType);
            }
            if (pType == nullptr)
                return;
            PHANTOM_SEMANTIC_ASSERT(!(pType->isTemplateDependant()));
            pInstanciated = pType->isSame(pExp->getValueType())
            ? (Expression*)pExp
            : convert(pExp, pType, a_pInput->getConversionType(),
                      a_pInput->getConversionType() == CastKind::Implicit ? UserDefinedFunctions::ImplicitsOnly
                                                                          : UserDefinedFunctions::All,
                      in_pContextScope);
            if (pInstanciated == pExp)
            {
                pExp.take(); // cancel guard
            }
            return;
        }
        else
        {
            if (Expression* pExp = o_instantiateT(Expression, a_pInput->getInputExpression()))
            {
                pInstanciated = New<BuiltInConversionExpression>(pExp, a_pInput->getConversion()->clone(getSource()),
                                                                 a_pInput->getConversionType());
            }
        }
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(BuiltInOperator* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(BuiltInOperatorExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_HasSideEffect:
    {
        switch (a_pInput->getOperator()->getId())
        {
        case Operator::Assignment:
        case Operator::AssignmentAdd:
        case Operator::AssignmentSubtract:
        case Operator::AssignmentMultiply:
        case Operator::AssignmentDivide:
        case Operator::AssignmentBitAnd:
        case Operator::AssignmentBitOr:
        case Operator::AssignmentModulo:
        case Operator::AssignmentShiftLeft:
        case Operator::AssignmentShiftRight:
        case Operator::AssignmentXOr:
        case Operator::PreDecrement:
        case Operator::PreIncrement:
        case Operator::PostDecrement:
        case Operator::PostIncrement:
            *(bool*)a_Data.out[0] = true;
            break;
        default:
            visit(static_cast<Expression*>(a_pInput), a_Data);
            break;
        }
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnersGuard<Expressions>    arguments;
        size_t                      count = a_pInput->getOperator()->getOperandCount();
        for (size_t i = 0; i < count; ++i)
        {
            Expression* pArgument = o_instantiateT(Expression, a_pInput->getInputArgument(i));
            if (pArgument == nullptr)
                return;
            PHANTOM_SEMANTIC_ASSERT(!(pArgument->isTemplateDependant()),
                                    "template instantiation still template dependant, anormal");
            arguments.push_back(pArgument);
        }
        switch (a_pInput->getOperator()->getOperatorType())
        {
        case OperatorKind::Binary:
            pInstanciated = solveBinaryOperator(a_pInput->getOperator()->getId(), arguments.elements()[0],
                                                arguments.elements()[1], in_pContextScope);
            break;
        case OperatorKind::UnaryPrefixed:
            pInstanciated =
            solveUnaryPreOperator(a_pInput->getOperator()->getId(), arguments.elements()[0], in_pContextScope);
            break;
        case OperatorKind::UnaryPostfixed:
            pInstanciated =
            solveUnaryPostOperator(a_pInput->getOperator()->getId(), arguments.elements()[0], in_pContextScope);
            break;
        case OperatorKind::Ternary:
            pInstanciated = solveConditional(arguments.elements()[0], arguments.elements()[1], arguments.elements()[3],
                                             in_pContextScope);
            break;
        }
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(CallExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_HasSideEffect:
    {
        *(bool*)a_Data.out[0] = true;
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnersGuard<Expressions>    newArguments;
        for (auto pArgument : a_pInput->getArguments())
        {
            //             PHANTOM_SEMANTIC_ASSERT(!(pArgument->isTemplateDependant()),
            //                            "subroutine must be valid because no template dependant expression must "
            //                            "be involved, TemplateDependantExpression would be used instead");
            Expression* pInst = o_instantiateT(Expression, pArgument);
            if (pInst == nullptr)
                return;
            newArguments.push_back(pInst);
        }
        Subroutine* pSubroutine = o_resolveT(Subroutine, a_pInput->getSubroutine());
        PHANTOM_SEMANTIC_ASSERT(pSubroutine,
                                "subroutine must be valid because no template dependant expression must be "
                                "involved, TemplateDependantExpression would be used instead");
        pInstanciated = New<CallExpression>(pSubroutine, newArguments.elements());
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}

void Semantic::visit(Class* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_BuildClass:
    {
        LanguageElement* in_pContextScope = a_pInput;
        EClassBuildState buildState = *(EClassBuildState*)a_Data.in;

        /// Ensure base classes are in the same build state (to access their trivial constructors,
        /// etc...)
        for (auto& baseClass : a_pInput->getBaseClasses())
        {
            if (!(baseClass.baseClass->isNative()))
            {
                // TODO : find a way to void this Compiler dependency here
                Compiler::Get()
                ->getCompiledSource(baseClass.baseClass->getSource())
                ->getSemantic()
                ->buildClass(baseClass.baseClass, buildState);
            }
        }

        switch (buildState)
        {
        case e_ClassBuildState_Sized:
        {
            PHANTOM_SEMANTIC_ASSERT(!a_pInput->isNative());
            Class::ExtraData* pCompilationData = static_cast<Class::ExtraData*>(a_pInput->getExtraData());
            PHANTOM_SEMANTIC_ASSERT(pCompilationData);

            for (auto& baseClass : a_pInput->getBaseClasses())
            {
                Class* pBaseClass = baseClass.baseClass;
                if (!pBaseClass->isNative())
                {
                    sizeClass(pBaseClass); // ensure size is computed
                }
            }

            // Check if we really need a vtableptr

            if ((a_pInput->getBaseClasses().size() == 0                           // no baseclass
                 || a_pInput->getBaseClass(0)->getVirtualMethodTables().empty())) // or base class has no vtable
            {
                pCompilationData->m_bHasVTablePtr = false;
                for (auto pMethod : a_pInput->getMethods())
                {
                    if (pMethod->isVirtual())
                    {
                        pCompilationData->m_bHasVTablePtr = true;
                        break;
                    }
                }
                // Shift base class offset depending on if we have a vtableptr
                if (pCompilationData->m_bHasVTablePtr)
                {
                    pCompilationData->m_AlignmentComputer.push(PHANTOM_TYPEOF(void*));
                }
            }

            // look for abstract / pure virtual member functionss
            bool bAbstract = false;
            for (auto pMethod : a_pInput->getMethods())
            {
                if (pMethod->isPureVirtual())
                {
                    bAbstract = true;
                    break;
                }
            }

            for (auto& baseClass : a_pInput->getBaseClasses())
            {
                Class* pBaseClass = baseClass.baseClass;
                bool   bBaseAbstract = pBaseClass->isAbstract();
                for (auto pBaseTable : pBaseClass->getVirtualMethodTables())
                {
                    VirtualMethodTable* pDerivedVTable = a_pInput->deriveVirtualMethodTable(pBaseTable);

                    // base class is abstract and we are not, for now .... => ,
                    if (bBaseAbstract && !(bAbstract))
                    {
                        // we check if we override every pure virtual methods from base class...
                        for (size_t i = 0; i < pBaseTable->getMethodCount(); ++i)
                        {
                            Method* pMF = pBaseTable->getMethod(i);
                            if (pMF && pMF->isPureVirtual())
                            {
                                bool overrideFound = false;
                                for (auto pOverrideCandidate : a_pInput->getMethods())
                                {
                                    if (pMF->isOverridableBy(pOverrideCandidate))
                                    {
                                        overrideFound = true;
                                        break;
                                    }
                                }
                                if (!(overrideFound)) // ...else we become abstract ourself
                                {
                                    bAbstract = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                a_pInput->setBaseClassOffset(baseClass.baseClass,
                                             pCompilationData->m_AlignmentComputer.push(pBaseClass));
            }

            a_pInput->setModifiers(a_pInput->getModifiers() |
                                   Modifiers(PHANTOM_R_ABSTRACT * bAbstract)); // set 'abstract' if so

            for (auto pMethod : a_pInput->getMethods())
            {
                if (pMethod->isVirtual() && a_pInput->getVirtualMethodTables().empty())
                {
                    a_pInput->addNewVirtualMethodTable();
                    break;
                }
            }

            visit(static_cast<ClassType*>(a_pInput), a_Data);
        }
            return;

        case e_ClassBuildState_ImplicitsAndDefaults:
        {
            for (auto& bc : a_pInput->getBaseClasses())
            {
                if (!bc.baseClass->isNative())
                    buildClass(bc.baseClass, buildState);
            }

            visit(static_cast<ClassType*>(a_pInput), a_Data);

            if (a_pInput->getTemplateSpecialization() == nullptr || a_pInput->getTemplateSpecialization()->isFull())
            {
                /// Ensure default constructor, copy constructor and destructor are available (if
                /// allowed)
                if (a_pInput->getDefaultConstructor() == nullptr && a_pInput->canHaveImplicitDefaultConstructor())
                {
                    a_pInput->addImplicitDefaultConstructor();
                }
                if (a_pInput->getCopyConstructor() == nullptr && a_pInput->canHaveImplicitCopyConstructor())
                {
                    a_pInput->addImplicitCopyConstructor();
                }
                if (a_pInput->getCopyAssignmentOperator() == nullptr &&
                    a_pInput->canHaveImplicitCopyAssignmentOperator())
                {
                    a_pInput->addImplicitCopyAssignmentOperator();
                }
                if (a_pInput->getMoveConstructor() == nullptr && a_pInput->canHaveImplicitMoveConstructor())
                {
                    a_pInput->addImplicitMoveConstructor();
                }
                if (a_pInput->getMoveAssignmentOperator() == nullptr &&
                    a_pInput->canHaveImplicitMoveAssignmentOperator())
                {
                    a_pInput->addImplicitMoveAssignmentOperator();
                }
                if (a_pInput->getDestructor() == nullptr)
                {
                    a_pInput->addImplicitDestructor();
                }
            }

            // insert methods in vtable
            for (auto pMethod : a_pInput->getMethods())
            {
                if (pMethod->testModifiers(Modifier::Override))
                {
                    Methods methods;
                    a_pInput->findOverriddenMethods(pMethod, methods);

                    if (methods.empty())
                    {
                        pushCodeRangeLocation(pMethod->getCodeRangeLocation());
                        CxxSemanticError("'%.*s' : method marked 'override' does not override "
                                         "any virtual function from any base class",
                                         PHANTOM_STRING_AS_PRINTF_ARG(pMethod->getName()));
                        popCodeRangeLocation();
                    }
                }

                size_t i = a_pInput->getVirtualMethodTables().size();
                bool   inserted = false;
                while (i--)
                {
                    // if not the zero-vtable (the one at offset-0 from the instance memory
                    // address), we only insert the function if it overrides an existing if the zero
                    // vtable. if already inserted, we only insert the function if it overrides
                    // existing
                    bool insertOnlyIfOverrides = (i != 0) || inserted;
                    inserted =
                    a_pInput->getVirtualMethodTables()[i]->insertMethod(pMethod, insertOnlyIfOverrides) || inserted;
                }
            }

            bool hasNonImplicitConstructor = false;
            for (auto pCtor : a_pInput->getConstructors())
            {
                if (!(pCtor->testFlags(PHANTOM_R_FLAG_IMPLICIT)))
                {
                    hasNonImplicitConstructor = true;
                    break;
                }
            }
            if (!(hasNonImplicitConstructor)) // all trivial constructors (no user defined)
            {
                CxxSemanticErrorReturnIf(a_pInput->getDefaultConstructor() == nullptr,
                                         "'%s' : no default constructor available", FormatCStr(a_pInput));
                for (auto pDM : a_pInput->getFields())
                {
                    if (ClassType* pClassType = pDM->getValueType()->removeAllQualifiers()->asClassType())
                    {
                        Constructor* pDCtor = pClassType->getDefaultConstructor();
                        if (!(pClassType->isDefaultConstructible()) || pDCtor == nullptr || pDM->getDefaultExpression())
                            continue;
                        CxxSemanticErrorIfInaccessible(pDCtor, a_pInput);
                    }
                    else
                    {
                        CxxSemanticErrorReturnIf(pDM->getValueType()->asReference(), "reference must be initialized")
                    }
                }
            }
        }
            return;

        case e_ClassBuildState_Blocks:
        {
            visit(static_cast<ClassType*>(a_pInput), a_Data);

            /// TRIVIAL DEFAULT CONSTRUCTOR

            generateImplicitDefaultConstructorCode(a_pInput);

            /// TRIVIAL COPY CONSTRUCTOR

            generateImplicitCopyConstructorCode(a_pInput);

            /// TRIVIAL COPY ASSIGNMENT OPERATOR

            generateImplicitCopyAssignmentOperatorCode(a_pInput);

            /// TRIVIAL MOVE CONSTRUCTOR

            generateImplicitMoveConstructorCode(a_pInput);

            /// TRIVIAL MOVE ASSIGNMENT OPERATOR

            generateImplicitMoveAssignmentOperatorCode(a_pInput);

            /// DESTRUCTOR

            generateImplicitDestructorCode(a_pInput);

            /// USER CONSTRUCTORS INITIALIZERS

            for (auto pCtor : a_pInput->getConstructors())
            {
                if (pCtor->testModifiers(Modifier::Deleted))
                    continue;
                if (pCtor->testFlags(PHANTOM_R_FLAG_INHERITED))
                    continue;
                Block* pBlock = pCtor->getBlock();
                PHANTOM_SEMANTIC_ASSERT(pBlock);
                /// Add data member constructor calls if not already added manually

                size_t i = a_pInput->getFields().size();

                while (i--)
                {
                    Field* pField = a_pInput->getFields()[i];
                    bool   alreadyConstructed = false;
                    for (Statement* pStatement : pBlock->getStatements())
                    {
                        FieldInitializationStatement* pBaseConstructorCallStatement;
                        if ((pBaseConstructorCallStatement = pStatement->asFieldInitializationStatement()) &&
                            pBaseConstructorCallStatement->getField() == pField)
                        {
                            alreadyConstructed = true;
                            break;
                        }
                    }
                    if (!alreadyConstructed)
                    {
                        ClassType*   pFieldClassType;
                        Array*       pFieldArray;
                        Constructor* pFieldConstructor = nullptr;
                        if (pFieldClassType = pField->getValueType()->removeQualifiers()->asClassType())
                        {
                            if (auto pDefExp = pField->getDefaultExpression())
                            {
                                auto implFieldInit = New<FieldInitializationStatement>(
                                pField, convertIfNotEqual(pDefExp, pField->getValueType(), in_pContextScope));
                                implFieldInit->setImplicit();
                                pBlock->prependStatement(implFieldInit);
                            }
                            else
                            {
                                pFieldConstructor = pFieldClassType->getDefaultConstructor();
                                if (pFieldClassType->isAbstract())
                                {
                                    CxxSemanticError("field '%.*s' cannot be initialized because type is abstract",
                                                     PHANTOM_STRING_AS_PRINTF_ARG(pField->getName()));
                                }
                                if (pFieldConstructor == nullptr)
                                {
                                    CxxSemanticError(
                                    "field '%.*s' cannot be initialized because type has no default constructor",
                                    PHANTOM_STRING_AS_PRINTF_ARG(pField->getName()));
                                }
                            }
                        }
                        else if (pFieldArray = pField->getValueType()->asArray())
                        {
                            if (auto pDefExp = pField->getDefaultExpression())
                            {
                                auto implFieldInit = New<FieldInitializationStatement>(
                                pField, convertIfNotEqual(pDefExp, pField->getValueType(), in_pContextScope));
                                implFieldInit->setImplicit();
                                pBlock->prependStatement(implFieldInit);
                            }
                            else
                            {
                                Type* pItemType = pFieldArray->getItemType();
                                if (ClassType* pItemClassType = pItemType->asClassType())
                                {
                                    Expressions  exps;
                                    Constructor* pArrayCtor = pItemClassType->getDefaultConstructor();
                                    PHANTOM_SEMANTIC_ASSERT(pArrayCtor != nullptr && !pArrayCtor->isAbstract());
                                    for (size_t i = 0; i < pFieldArray->getItemCount(); ++i)
                                    {
                                        exps.push_back(New<ConstructorCallExpression>(pArrayCtor));
                                    }
                                    auto implFieldInit =
                                    New<FieldInitializationStatement>(pField, New<ArrayExpression>(pFieldArray, exps));
                                    implFieldInit->setImplicit();
                                    pBlock->prependStatement(implFieldInit);
                                }
                            }
                        }
                        else
                        {
                            PHANTOM_SEMANTIC_ASSERT(pField->getValueType()->asReference() == nullptr);
                            if (auto pDefExp = pField->getDefaultExpression())
                            {
                                auto implFieldInit = New<FieldInitializationStatement>(
                                pField, convertIfNotEqual(pDefExp, pField->getValueType(), in_pContextScope));
                                implFieldInit->setImplicit();
                                pBlock->prependStatement(implFieldInit);
                            }
#pragma message(PHANTOM_TODO "add ZeroInit option on Semantic to ensure here every field is zero-init if we want")
                        }
                        if (pFieldConstructor)
                        {
                            auto implFieldInit = New<FieldInitializationStatement>(
                            pField, New<ConstructorCallExpression>(pFieldConstructor));
                            implFieldInit->setImplicit();
                            pBlock->prependStatement(implFieldInit);
                        }
                    }
                }

                /// Add base constructor calls if not already added manually

                i = a_pInput->getBaseClasses().size();

                while (i--)
                {
                    Class*    pBaseClass = a_pInput->getBaseClasses()[i];
                    ptrdiff_t uiOffset = a_pInput->getBaseClassOffset(i);
                    bool      alreadyConstructed = false;
                    for (Statement* pStatement : pBlock->getStatements())
                    {
                        BaseConstructorCallStatement* pBaseConstructorCallStatement;
                        if ((pBaseConstructorCallStatement = pStatement->asBaseConstructorCallStatement()) &&
                            pBaseConstructorCallStatement->getBaseClass() == pBaseClass)
                        {
                            alreadyConstructed = true;
                            break;
                        }
                    }
                    if (!alreadyConstructed)
                    {
                        Constructor* pBaseCtor = pBaseClass->getDefaultConstructor();
                        CxxSemanticErrorReturnIf(pBaseCtor == nullptr, "'%s' : no default constructor available",
                                                 FormatCStr(pBaseClass));
                        CxxSemanticErrorIfInaccessible(pBaseCtor, a_pInput);
                        // CxxSemanticErrorReturnIf(pBaseCtor->isPrivate() &&
                        // !(a_pInput->hasFriend(pBaseClass)), e_Error_InaccessibleSymbol,
                        // &pBaseClass);

                        auto implBaseInit =
                        New<BaseConstructorCallStatement>(a_pInput, New<ConstructorCallExpression>(pBaseCtor));
                        implBaseInit->setImplicit();
                        pBlock->prependStatement(implBaseInit);
                    }
                }

                /// Add vtable setup
                bool       vtableInstalled = false;
                Statement* pInsertionStatement = nullptr;
                for (Statement* pStatement : pBlock->getStatements())
                {
                    if (pStatement->asBaseConstructorCallStatement() == nullptr)
                    {
                        for (VirtualMethodTable* pTable : a_pInput->getVirtualMethodTables())
                        {
                            VirtualMethodTableSetupStatement* pVTableStatement =
                            New<VirtualMethodTableSetupStatement>(pTable);
                            if (pInsertionStatement)
                                pBlock->insertStatementAfter(pInsertionStatement, pVTableStatement);
                            else
                                pBlock->prependStatement(pVTableStatement);
                            pInsertionStatement = pVTableStatement;
                        }
                        vtableInstalled = true;
                        break;
                    }
                    pInsertionStatement = pStatement;
                }
                if (!vtableInstalled)
                {
                    for (VirtualMethodTable* pTable : a_pInput->getVirtualMethodTables())
                    {
                        pBlock->addStatement(New<VirtualMethodTableSetupStatement>(pTable));
                    }
                }
            }
        }
            return;
        }
        visit(static_cast<ClassType*>(a_pInput), a_Data);
    }
        return;

    case e_VisitorFunction_QualifiedNameLookup:
    {
        Symbols&  out_candidates = *(Symbols*)a_Data.out[0];
        Accesses& out_accesses = *(Accesses*)a_Data.out[1];
        visit(static_cast<ClassType*>(a_pInput), a_Data);
        if (out_candidates.size())
            return;
        if (a_pInput->isTemplateElement()) /// We don't browse base classes if in template scope look up
            return;
        StringView in_name = *(StringView*)a_Data.in[0];
        for (auto& p : a_pInput->getBaseClasses())
        {
            if (p.baseClass->getName() == in_name)
            {
                PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((p.baseClass));
                out_candidates.push_back(p.baseClass);
                out_accesses.push_back(p.access);
                return;
            }
        }
        for (auto& baseClass : a_pInput->getBaseClasses())
        {
            baseClass.baseClass->visit(this, a_Data);
        }
    }
        return;

    case e_VisitorFunction_SubroutineResolution:
    {
        LanguageElement*& out_pResult = *(LanguageElement**)a_Data.out[0];
        if (out_pResult)
            return;
        visit(static_cast<ClassType*>(a_pInput), a_Data);
        if (out_pResult)
            return;
        for (auto& p : a_pInput->getBaseClasses())
        {
            VisitorData      subData = a_Data;
            LanguageElement* pSubResult = nullptr;
            void*            out[1] = {&pSubResult};
            subData.out = out;
            p.baseClass->visit(this, subData);
            if (pSubResult)
            {
                if (out_pResult)
                {
                    // CxxSemanticError("ambiguous base member function selection");
                    out_pResult = nullptr;
                    return;
                }
                else
                {
                    out_pResult = pSubResult;
                }
            }
        }
    }
        return;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];

        EClassBuildState currBuildState = pInstanciated
        ? EClassBuildState(static_cast<ClassType*>(pInstanciated)->getExtraData()->m_BuildState)
        : e_ClassBuildState_None;

        PHANTOM_SEMANTIC_ASSERT(currBuildState == in_eClassBuildState);
        //
        //         if (destBuildState < in_eClassBuildState)
        //         {
        //             *(EClassBuildState*)a_Data.in[1] = destBuildState;
        //             a_pInput->visit(this, a_Data);
        //             *(EClassBuildState*)a_Data.in[1] = in_eClassBuildState;
        //         }

        if (in_eClassBuildState == e_ClassBuildState_None)
        {
            PHANTOM_SEMANTIC_ASSERT(pInstanciated == nullptr);
            pInstanciated = New<Class>(a_pInput->getName(), a_pInput->getModifiers());
            return;
        }
        else if (in_eClassBuildState == e_ClassBuildState_Inheritance)
        {
            Class* pClass = static_cast<Class*>(pInstanciated);
            for (auto& p : a_pInput->getBaseClasses())
            {
                Class* pBaseClass = o_resolveT(Class, p.baseClass);
                if (pBaseClass)
                {
                    CompiledSource::Get(pClass->getSource())->addBuildDependency(pBaseClass->getSource());
                    pClass->addBaseClass(pBaseClass, p.access);
                }
            }
        }
        visit(static_cast<ClassType*>(a_pInput), a_Data);
    }
        return;

    case e_VisitorFunction_Access:
    {
        Symbol* in_pSymbol = *(Symbol**)a_Data.in[0];
        bool&   out_bResult = *(bool*)a_Data.out[0];

        if (in_pSymbol->isPublic() || (in_pSymbol->isProtected() && a_pInput->hasMemberCascade(in_pSymbol)) ||
            (in_pSymbol->isPrivate() && a_pInput->hasElement(in_pSymbol)) || isFriendWith(in_pSymbol, a_pInput))
        {
            out_bResult = true;
            return;
        }
        visit(static_cast<Symbol*>(a_pInput), a_Data);
    }
        return;
    }
    visit(static_cast<ClassType*>(a_pInput), a_Data);
}

void Semantic::visit(ClassType* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TypeConversion:
    {
        Type*&       pOutput = *(Type**)a_Data.in[0];
        Conversion*& conv = *(Conversion**)a_Data.out[0];
        if (pOutput->asRValueReference())
        {
            // X -> X&&
            if (pOutput->getUnderlyingType()->isSame(a_pInput))
            {
                conv = newConv<DefaultConversionSequence>(newConv<RefConversion>(a_pInput, true));
                return;
            }
            else
            {
                // X&& -> Y&&
                a_pInput->addRValueReference()->visit(this, a_Data);
                if (conv)
                {
                    if (conv->semantic)
                        conv = conv->clone(getSource());
                    static_cast<DefaultConversionSequence*>(conv)->addRValuePromotion();
                    return;
                }
                else
                {
                    // X -> Y -> Y&&
                    Type* pPrevOutput = pOutput;
                    pOutput = pOutput->getUnderlyingType();
                    a_pInput->visit(this, a_Data);
                    if (conv)
                    {
                        if (conv->semantic)
                            conv = conv->clone(getSource());
                        static_cast<DefaultConversionSequence*>(conv)->addRValueRef();
                        return;
                    }
                    pOutput = pPrevOutput;
                }
            }
        }
    }
    break;
    case e_VisitorFunction_ComputeSizeAndAlignment:
        buildClass(a_pInput, e_ClassBuildState_Sized);
        return;
    case e_VisitorFunction_BuildClass:
    {
        EClassBuildState buildState = *(EClassBuildState*)a_Data.in;
        PHANTOM_SEMANTIC_ASSERT(buildState != e_ClassBuildState_None);
        if (TemplateSpecialization* pSpec = a_pInput->getTemplateSpecialization())
        {
            if (pSpec->isFull())
            {
                PHANTOM_SEMANTIC_ASSERT(pSpec->getTemplated() == a_pInput);
                PHANTOM_SEMANTIC_ASSERT(buildState > e_ClassBuildState_None);
                TemplateSpecialization* pPrimary = pSpec->getInstantiationSpecialization();
                if (pPrimary)
                {
                    PHANTOM_SEMANTIC_ASSERT(templateInstantiations()[pSpec][pPrimary->getTemplated()],
                                            "template instantiations is unknown to this semantic");
                    PHANTOM_SEMANTIC_ASSERT(
                    templateInstantiations()[pSpec][pPrimary->getTemplated()] == a_pInput,
                    "template instantiations has been started in another Semantic. Use same semantic to "
                    "perform full build/template specializations of the same types");
                    instantiateTemplateElement(pPrimary->getTemplated(), pSpec, buildState, pSpec, 0);
                }
            }
        }

        switch (buildState)
        {
        case e_ClassBuildState_Sized:
        {
            PHANTOM_SEMANTIC_ASSERT(!a_pInput->isNative());
            size_t preferredAlignment = a_pInput->getAlignment();
            if (preferredAlignment > a_pInput->getExtraData()->m_AlignmentComputer.maxAlignment())
                a_pInput->getExtraData()->m_AlignmentComputer.setMaxAlignement(preferredAlignment);
            a_pInput->getExtraData()->m_AlignmentComputer.align();
            // setup instance data members and compute total size
            size_t uiSize = 0;
            size_t uiAlignment = 0;
            Fields allFields;
            a_pInput->getAllFields(allFields);
            for (auto pField : allFields)
            {
                if (ClassType* pClassType = pField->getValueType()->removeQualifiers()->removeArray()->asClassType())
                    if (!(pClassType->isNative()))
                        sizeClass(pClassType);
            }
            a_pInput->getExtraData()->m_AlignmentComputer.alignStruct(a_pInput->getDataElements(), uiSize, uiAlignment);
            a_pInput->setSize(uiSize);
            if (preferredAlignment == 0)
            {
                a_pInput->setAlignment(uiAlignment);
            }
            else
            {
                CxxSemanticErrorReturnIf(a_pInput->getAlignment() < uiAlignment,
                                         "required alignment is above user specified alignment");
            }
            for (size_t i = 0; i < a_pInput->getExtendedTypes().size(); ++i)
                computeSizeAndAlignment(a_pInput->getExtendedTypes()[i]);
        }
        break;
        case e_ClassBuildState_ImplicitsAndDefaults:
        {
            Fields allFields;
            a_pInput->getAllFields(allFields);
            for (auto pField : allFields)
            {
                if (ClassType* pClassType = pField->getValueType()->removeQualifiers()->removeArray()->asClassType())
                    if (!(pClassType->isNative()))
                        edifyClass(pClassType);
            }
        }
        break;
        }
    }
        return;

    case e_VisitorFunction_SubroutineResolution:
        visit(static_cast<Scope*>(a_pInput), a_Data);
        return;
    case e_VisitorFunction_QualifiedLookup:
        visit(static_cast<Type*>(a_pInput), a_Data);
        return;

    case e_VisitorFunction_QualifiedNameLookup:
    {
        LanguageElement* in_pContextScope = *(LanguageElement**)a_Data.in[1];
        visit(static_cast<Scope*>(a_pInput), a_Data);
        Symbols& out_candidates = *(Symbols*)a_Data.out[0];
        if (out_candidates.size())
        {
            StringView in_name = *(StringView*)a_Data.in[0];
            if (std::find_if(out_candidates.begin(), out_candidates.end(), [](Symbol* s) {
                    return s->asFunction() ||
                    s->asAlias() && static_cast<Alias*>(s)->getAliasedSymbol()->asSubroutine();
                }) != out_candidates.end())
            {
                for (auto pMethod : a_pInput->getMethods())
                {
                    if (pMethod->getName() == in_name)
                    {
                        out_candidates.push_back(pMethod);
                    }
                }
            }
            return;
        }

        TemplateSpecialization* pSpec = a_pInput->getTemplateSpecialization();

        // triggers full instantiation of the template to access its members
        // (we don't want to do that on unqualified lookups because it means we are inside the scope
        // of the template specialization itself)
        if ((a_Data.flags & e_VisitorFlag_Unqualified) == 0)
        {
            if (pSpec && pSpec->isFull() && pSpec->testFlags(PHANTOM_R_FLAG_IMPLICIT) &&
                !(pSpec->isNative())) // instantiation
            {
                PHANTOM_SEMANTIC_ASSERT(pSpec->getTemplated() == a_pInput);
                PHANTOM_SEMANTIC_ASSERT(pSpec->getInstantiationSpecialization());
                Symbol* pPrimaryTemplated = pSpec->getInstantiationSpecialization()->getTemplated();
                PHANTOM_SEMANTIC_ASSERT(pPrimaryTemplated);
                PHANTOM_SEMANTIC_ASSERT(pPrimaryTemplated->asClassType());
                sizeClass(a_pInput); // ensure sizing
            }
            else if (pSpec == nullptr || (pSpec && pSpec->isFull())) // full specialization
            {
                CxxSemanticErrorReturnIf(!(a_pInput->isNative()) &&
                                         a_pInput->getExtraData()->m_BuildState < e_ClassBuildState_Sized,
                                         "'%s' : incomplete type", FormatCStr(a_pInput));
            }
        }
        StringView in_name = *(StringView*)a_Data.in[0];
        if (in_name.empty())
        {
            out_candidates.insert(out_candidates.end(), a_pInput->getConstructors().begin(),
                                  a_pInput->getConstructors().end());
            return;
        }
        if (a_pInput->isNative() && a_pInput->getTemplateSpecialization()
            /*&& phantom::lang::detail::currentScope()*/) /// for native C++ lang
                                                          /// declaration symbol resolution
        {
            LanguageElement* pElement = a_pInput->getTemplateSpecialization()->getArgument(in_name);
            if (pElement && pElement->asSymbol())
            {
                PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((pElement));
                out_candidates.push_back(static_cast<Symbol*>(pElement));
                return;
            }
        }
    }
    break;

    case e_VisitorFunction_UnqualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        LanguageElement*                    in_pContextScope = *(LanguageElement**)a_Data.in[3];
        Type*                               in_pInitializationType = (Type*)a_Data.in[4];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];
        if (in_name == a_pInput->getName()) /// ex : in namespace phantom, we can access to ourself, i.e
                                            /// 'phantom', so we use qualified ::phantom to find it
        {
            if (isStandardLookup()) /// No translation unit implies traditional
                                    /// namespaces are required to find any symbol
            {
                if (a_pInput->getNamespace())
                {
                    out_pResult = silentQualifiedLookup(a_pInput->getNamespace(), in_name, in_pTemplateArguments,
                                                        in_pFunctionArguments, in_pContextScope, in_pInitializationType,
                                                        a_Data.flags);
                    return;
                }
            }
            if (!in_pFunctionArguments && !in_pTemplateArguments && in_pInitializationType == nullptr)
            {
                out_pResult = a_pInput;
            }
            else if (a_pInput->getOwner())
            {
                out_pResult =
                silentQualifiedLookup(a_pInput->getOwner(), in_name, in_pTemplateArguments, in_pFunctionArguments,
                                      in_pContextScope, in_pInitializationType, a_Data.flags);
            }
            return;
        }
    }
    break;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];

        PHANTOM_SEMANTIC_ASSERT(pInstanciated && pInstanciated->asClassType());
        ClassType* pInstanceClassType = static_cast<ClassType*>(pInstanciated);
        for (auto& p : a_pInput->getElements())
        {
            Symbol* pSymbol = (p)->asSymbol();
            if (p->testFlags(PHANTOM_R_FLAG_IMPLICIT) || (pSymbol && pSymbol->getVisibility() == Visibility::Private))
                continue;
            auto pInstance = instantiateTemplateElement(p, in_TemplateSubstitution, in_eClassBuildState,
                                                        pInstanceClassType, a_Data.flags);
            if (pSymbol && pInstance && in_eClassBuildState == e_ClassBuildState_Members)
            { // apply same access as instance symbol
                static_cast<Symbol*>(pInstance)->setAccess(pSymbol->getAccess());
            }
        }
    } // case
        return;

    case e_VisitorFunction_Access:
    {
        Symbol* in_pSymbol = *(Symbol**)a_Data.in[0];
        bool&   out_bResult = *(bool*)a_Data.out[0];
        if (in_pSymbol->isPublic() || a_pInput->hasElement(in_pSymbol) || isFriendWith(in_pSymbol, a_pInput))
        {
            out_bResult = true;
            return;
        }
        visit(static_cast<Symbol*>(a_pInput), a_Data);
        return;
    }
    break;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void Semantic::visit(ConditionalExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];

        Expression* pCond = o_instantiateT(Expression, a_pInput->getConditionExpression());
        if (pCond == nullptr)
            return;
        Expression* pThen = o_instantiateT(Expression, a_pInput->getThenExpression());
        if (pThen == nullptr)
            return;
        Expression* pElse = o_instantiateT(Expression, a_pInput->getElseExpression());
        if (pElse == nullptr)
            return;
        pInstanciated = solveConditional(pCond, pThen, pElse);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(Constant* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ToExpression:
    {
        Expression*  in_pLeftExpression = *(Expression**)a_Data.in[0];
        Expression*& out_pExpression = *(Expression**)a_Data.out[0];
        if (in_pLeftExpression)
            return;
        out_pExpression = New<ConstantExpression>(a_pInput);
        return;
    }
    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        *(LanguageElement**)a_Data.out[0] = a_pInput;
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        *(LanguageElement**)a_Data.out[0] = a_pInput->clone(getSource());
    }
        return;

    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*  pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        Constant*         pConstant = pArgument->asConstant();
        Expression*       pCExp;
        if (pConstant == nullptr && (pCExp = pArgument->asExpression()) && pCExp->as<ConstantExpression>())
        {
            pConstant = static_cast<ConstantExpression*>(pArgument)->getConstant()->asConstant();
        }
        if (pConstant // argument is a constant, and
            && ((pConstant->asPlaceholder() == nullptr) ||
                (a_pInput->asPlaceholder() != nullptr))) // argument is not a placeholder or parameter is a placeholder
                                                         // => non-placeholders cannot accept placeholders
        {
            byte  bufferArgument[256];
            byte  bufferParameter[256];
            byte  bufferArgumentConv[256];
            byte* bufferArgumentUsed = bufferParameter;
            pConstant->getValue(bufferArgument);
            a_pInput->getValue(bufferParameter);

            // if constant types not isSame, we perform first an implicit conversion
            if (!(pConstant->getValueType()->isSame(a_pInput->getValueType())))
            {
                bufferArgumentUsed = bufferArgumentConv;
                Conversion* pConv = newConversion(pConstant->getValueType(), a_pInput->getValueType(),
                                                  CastKind::Implicit, UserDefinedFunctions::None, nullptr);
                if (pConv == nullptr)
                {
                    LanguageElement* in_pContextScope = nullptr;
                    pushCodeRangeLocation(pConstant->getCodeRangeLocation());
                    CxxSemanticErrorConversion(pConstant->getValueType(), a_pInput->getValueType());
                    popCodeRangeLocation();
                    return;
                }
                pConv->apply(bufferArgument, bufferArgumentConv);
                PHANTOM_SEMANTIC_ASSERT(pConv->semantic);
            }
            if (a_pInput->getValueType()->equal(bufferParameter, bufferArgumentUsed))
            {
                pDeducedArgument = a_pInput;
                return;
            }
        }
        return;
    }
    }
}
void Semantic::visit(ConstantExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        pInstanciated = New<ConstantExpression>(o_resolveT(Constant, a_pInput->getConstant()));
    }
        return;
    case e_VisitorFunction_ExpressionConversion:
    {
        Type*                in_pOutput = *(Type* const*)a_Data.in[0];
        CastKind             in_ConversionType = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions in_UserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        LanguageElement*     in_pContextScope = *(LanguageElement* const*)a_Data.in[3];
        Expression*&         out_pResult = *(Expression**)a_Data.out[0];
        visit(static_cast<Expression*>(a_pInput), a_Data);
        if (out_pResult == nullptr)
        {
            if (a_pInput->getValueType()->asIntegralType())
            {
                long long l = 0;
                a_pInput->getConstant()->getValue(&l);
                if (l == 0)
                {
                    Expression* pNull = New<ConstantExpression>(newConstant(nullptr));
                    visit(pNull, a_Data);
                    if (out_pResult)
                    {
                        Delete(a_pInput);
                    }
                    else
                    {
                        Delete(pNull);
                    }
                }
            }
        }
        return;
    }
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(Constructor* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_CreateCallExpression:
    {
        ExpressionsView in_Arguments = *(const ExpressionsView*)a_Data.in[0];
        Expression*     in_pContextScope = *(Expression**)a_Data.in[1];
        Expression*&    out_pResult = *(Expression**)a_Data.out[0];
        out_pResult = createCallExpression(a_pInput, in_Arguments, in_pContextScope);
    }
        return;
    case e_VisitorFunction_IsViableCallCandidate:
    {
        OptionalArrayView<LanguageElement*> templateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[0];
        TypesView                           functionArguments = *(const TypesView*)a_Data.in[1];
        Modifiers                           modifiers = *(Modifiers*)a_Data.in[2];
        LanguageElement*                    pScope = *(LanguageElement**)a_Data.in[3];
        UserDefinedFunctions                convUserDefinedFunctions = *(UserDefinedFunctions*)a_Data.in[4];
        ConversionResults&                  out_conversions = *(ConversionResults*)a_Data.out[0];
        Subroutine*&                        out_subroutine = *(Subroutine**)a_Data.out[1];
        PlaceholderMap&                     out_deductions = *(PlaceholderMap*)a_Data.out[2];
        out_subroutine = nullptr;

        if (functionArguments.size() < a_pInput->getRequiredArgumentCount() ||
            (!(a_pInput->getSignature()->isVariadic()) &&
             (functionArguments.size() > a_pInput->getParameters().size())))
        {
            return;
        }
        Types argumentTypes;
        auto& params = a_pInput->getParameters();
        for (size_t i = 0; i < params.size(); ++i)
        {
            if (i < functionArguments.size())
            {
                argumentTypes.push_back(functionArguments[i]);
            }
        }

        if (auto pTSpec = a_pInput->getTemplateSpecialization())
        {
            if (!_useExplicitTemplateArguments(a_pInput, pTSpec, templateArguments, out_deductions, "constructor"))
                return;
            newImplicitConversionsWithArgDeductions(a_pInput->getSignature(), argumentTypes, out_conversions,
                                                    out_deductions, pScope, convUserDefinedFunctions);
        }
        else
            newImplicitConversions(a_pInput->getSignature(), argumentTypes, out_conversions, pScope,
                                   convUserDefinedFunctions);
        if (!out_conversions.hasNull())
        {
            out_subroutine = a_pInput;
        }
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];

        auto pInstanceClassType = in_pContextScope->asClassType();

        TemplateSpecialization* pSpec = {};
        if (!pInstanceClassType)
        {
            pSpec = in_pContextScope->asTemplateSpecialization();
            PHANTOM_SEMANTIC_ASSERT(pSpec);
            // in_TemplateSubstitution.getInstantiation()->getInstantiationSpecialization() == a_pInput->getOwner());
            pInstanceClassType = static_cast<ClassType*>(pSpec->getNamingScope());
        }
        else
        {
            if (a_pInput->testFlags(PHANTOM_R_FLAG_IMPLICIT) || a_pInput->getVisibility() == Visibility::Private)
            {
                // not instanciating trivial members
                return;
            }
        }

        switch (in_eClassBuildState)
        {
        case e_ClassBuildState_None:
        {
            if (!pSpec)
                break;
        }
        case e_ClassBuildState_Members:
        {
            Signature* pSignature = o_instantiateT(Signature, a_pInput->getSignature());
            if (pSignature == nullptr)
                return;
            OwnerGuard<Constructor> pInstanceConstructor =
            New<Constructor>(a_pInput->getName(), pSignature, a_pInput->getModifiers(),
                             a_pInput->getFlags() & ~PHANTOM_R_FLAG_TEMPLATE_DEPENDANT);
            Subroutines conflicting;
            if (!(pInstanceClassType->acceptsSubroutine(pInstanceConstructor.element(), &conflicting)))
            {
                CxxSemanticError("'%s' : constructor conflicts without previously declared one(s) :",
                                 FormatCStr(pInstanceConstructor.element()));
                for (auto c : conflicting)
                {
                    CxxSemanticSubError("'%s'", FormatCStr(c));
                }
                return;
            }
            pInstanciated = pInstanceConstructor;
            if (!pSpec)
            {
                pInstanceClassType->addConstructor(pInstanceConstructor);
                break;
            }
            else
            {
                pSpec->setTemplated(pInstanceConstructor);
                pInstanceConstructor->createThis(pInstanceClassType);
            }
        }
        case e_ClassBuildState_Blocks:
        {
            if (!a_pInput->buildBlock())
            {
                CxxSemanticError("failed to instantiate block of template constructor '%.*s'",
                                 PHANTOM_STRING_AS_PRINTF_ARG(a_pInput->getName()));
                pInstanciated = nullptr;
                return;
            }
            if (pInstanciated)
            {
                Constructor* pInstanceConstructor = static_cast<Constructor*>(pInstanciated);

                // deleted / defaulted / implicit constructors have no code to be instantiated as they will be
                // generated at end of template specialization
                if (!(pInstanceConstructor->testModifiers(Modifier::Deleted)) &&
                    !(pInstanceConstructor->testModifiers(Modifier::Defaulted)) &&
                    !(pInstanceConstructor->testFlags(PHANTOM_R_FLAG_IMPLICIT)))
                {
                    PHANTOM_SEMANTIC_ASSERT(a_pInput->getBlock());
                    PHANTOM_SEMANTIC_ASSERT(pInstanceConstructor->getBlock() == nullptr);
                    o_mapT(a_pInput->getBlock(), addBlock(pInstanceConstructor, true));
                    o_mapT(a_pInput->getThis(), pInstanceConstructor->getThis());

                    pInstanceConstructor->setBlockBuilder([=](Block* a_pBlock) {
                        pInstanceConstructor->setBlockBuilder(
                        {}); // avoid cyclic builder invocation if function is recursive and calls itself
                        PHANTOM_SEMANTIC_ASSERT(a_pBlock == pInstanceConstructor->getBlock());
                        SemanticPrivate::SemanticInstantiateTemplateElement<Block>(
                        this, a_pInput->getBlock(), in_TemplateSubstitution, e_ClassBuildState_Blocks,
                        pInstanceConstructor, a_Data.flags, &LanguageElement::asBlock);
                        return true;
                    });

#if PHANTOM_CODE_LAZY_COMPILATION_ENABLED == 0
                    pInstanceConstructor->buildBlock();
#endif
                }
            }
            return;
        }
        break;
        }
    }
        return;
    }
    visit(static_cast<Method*>(a_pInput), a_Data);
}
void Semantic::visit(ConstructorCallExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_HasSideEffect:
    {
        *(bool*)a_Data.out[0] = true;
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnersGuard<Expressions>    newArguments;
        for (auto& p : a_pInput->getArguments())
        {
            Expression* pArg = o_instantiateT(Expression, p);
            if (pArg == nullptr)
                return;
            newArguments.push_back(pArg);
        }
        Constructor* pInstanceConstructor = o_resolveT(Constructor, a_pInput->getConstructor());
        if (pInstanceConstructor == nullptr)
            return;
        Expression* pExpression = createCallExpression(pInstanceConstructor, newArguments, in_pContextScope);
        if (pExpression == nullptr)
            return;
        pExpression->setTemporary(a_pInput->isTemporary());
        pInstanciated = pExpression;
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(ConstType* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ComputeSizeAndAlignment:
        a_pInput->getUnderlyingType()->visit(this, a_Data);
        a_pInput->setSize(a_pInput->getUnderlyingType()->getSize());
        a_pInput->setAlignment(a_pInput->getUnderlyingType()->getAlignment());
        return;

    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*  pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        ConstType*        pType = pArgument->asConstType();
        if (pType)
        {
            Type* pDeducedType =
            templateArgumentDeduction(a_pInput->getConstedType(), pType->getConstedType(), deductions);
            if (pDeducedType)
            {
                pDeducedArgument = pDeducedType->makeConst();
            }
        }
        else
        {
            ConstVolatileType* pType = pArgument->asConstVolatileType();
            if (pType)
            {
                Type* pDeducedType =
                templateArgumentDeduction(a_pInput->getConstedType(), pType->removeConst(), deductions);
                if (pDeducedType)
                {
                    pDeducedArgument = pDeducedType->makeConst();
                }
            }
        }
    }
        return;

    case e_VisitorFunction_TypeConversion:
    {
        /// Substitution
        a_pInput->getConstedType()->visit(this, a_Data);
    }
        return;

    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        Type*                       pType = o_resolveT(Type, a_pInput->getConstedType());
        if (pType == nullptr)
            return;
        pResolved = pType->addConst();
    }
        return;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void Semantic::visit(ConstVolatileType* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ComputeSizeAndAlignment:
        a_pInput->getUnderlyingType()->visit(this, a_Data);
        a_pInput->setSize(a_pInput->getUnderlyingType()->getSize());
        a_pInput->setAlignment(a_pInput->getUnderlyingType()->getAlignment());
        return;
    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*   pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*&  pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&    deductions = *(PlaceholderMap*)a_Data.out[1];
        ConstVolatileType* pType = pArgument->asConstVolatileType();
        if (pType)
        {
            Type* pDeducedType =
            templateArgumentDeduction(a_pInput->getConstVolatiledType(), pType->getConstVolatiledType(), deductions);
            if (pDeducedType)
            {
                pDeducedArgument = pDeducedType->makeConstVolatile();
            }
        }
    }
        return;

    case e_VisitorFunction_TypeConversion:
    {
        /// Substitution
        a_pInput->getConstVolatiledType()->visit(this, a_Data);
    }
        return;

    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        Type*                       pType = o_resolveT(Type, a_pInput->getConstVolatiledType());
        if (pType == nullptr)
            return;
        pResolved = pType->addConstVolatile();
    }
        return;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void Semantic::visit(ContainerClass* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        PHANTOM_SEMANTIC_ASSERT(false);
    }
    break;
    case e_VisitorFunction_BuildClass:
    {
        EClassBuildState in_eClassBuildState = *(EClassBuildState*)a_Data.in;
        if (in_eClassBuildState == e_ClassBuildState_Sized)
        {
            if (Alias* pValueTypeAlias = a_pInput->getAlias("value_type"))
            {
                if (Type* pType = pValueTypeAlias->getAliasedSymbol()->asType())
                {
                    a_pInput->setValueType(pType);
                }
                else
                {
                    CxxSemanticError("'value_type' must be a type alias in container");
                }
            }
            else
            {
                CxxSemanticError("missing 'value_type' type alias in container");
            }
        }
    }
    break;
    }
    visit(static_cast<Class*>(a_pInput), a_Data);
}
void Semantic::visit(Field* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ToExpression:
    {
        Expression*  in_pLeftExpression = *(Expression**)a_Data.in[0];
        Expression*& out_pExpression = *(Expression**)a_Data.out[0];
        if (in_pLeftExpression == nullptr)
            return;
        LanguageElement* in_pContextScope = nullptr;
        Type*            pOwnerType = a_pInput->getOwnerClassType();
        CxxSemanticErrorReturnIf(a_pInput->getSize() == 0 || a_pInput->getOffset() == Field::InvalidOffset,
                                 "'%s' : incomplete type", FormatCStr(pOwnerType));
        if (in_pLeftExpression->getValueType()->asReference() == nullptr)
        {
            CxxSemanticErrorReturnIf(in_pLeftExpression->getValueType()->removeQualifiers()->asClassType() == nullptr,
                                     "'%s' : expected class-type", FormatCStr(in_pLeftExpression->getValueType()));
            in_pLeftExpression = New<RValueReferenceExpression>(in_pLeftExpression->clone(getSource()));
        }
        Expression* pConv =
        CxxSemanticConversion(in_pLeftExpression, in_pLeftExpression->getValueType()->replicate(pOwnerType));
        if (pConv == nullptr)
            return;
        out_pExpression = New<FieldExpression>(pConv, a_pInput);
    }
        return;

    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];

        ClassType* pResolvedOwnerClassType = o_resolveT(ClassType, a_pInput->getOwnerClassType());
        if (pResolvedOwnerClassType == nullptr)
            return;
        pResolved = pResolvedOwnerClassType->getField(a_pInput->getName());
    }
        return;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];

        ClassType* pInstanceClassType = static_cast<ClassType*>(in_pContextScope);
        PHANTOM_SEMANTIC_ASSERT(pInstanceClassType->asClassType());

        if (in_eClassBuildState == e_ClassBuildState_Members)
        {
            CxxSemanticCheckSymbolName(a_pInput->getName(), pInstanceClassType);
            Type* pInstanceValueType = o_resolveT(Type, a_pInput->getValueType());
            if (pInstanceValueType == nullptr)
            {
                // CxxSemanticError(e_Error_TemplateInstantiationFailure, a_pInput);
                return;
            }
            Field* pInstanceField =
            New<Field>(pInstanceValueType, a_pInput->getName(), a_pInput->getFilterMask(), a_pInput->getModifiers());
            pInstanciated = pInstanceField;
            pInstanceClassType->addField(pInstanceField);
        }
        else if (in_eClassBuildState == e_ClassBuildState_ImplicitsAndDefaults)
        {
            if (pInstanciated == nullptr)
                return;
            Field* pInstanceField = static_cast<Field*>(pInstanciated);
            PHANTOM_SEMANTIC_ASSERT(pInstanceField);
            OwnerGuard<Expression> pExpression = o_instantiateT(Expression, a_pInput->getDefaultExpression());
            if (pExpression == nullptr)
                return;
            auto        codeRange = pExpression->getCodeRange();
            Expression* pConv = convertIfNotEqual(pExpression, pInstanceField->getValueType(), in_pContextScope);
            if (pConv == nullptr)
                return;
            pExpression.take();
            pConv->setCodeRange(codeRange);
            pInstanceField->setDefaultExpression(pConv);
        }
    }
        return;
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(FieldExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Field*                      pDM = o_resolveT(Field, a_pInput->getField());
        if (pDM == nullptr)
            return;
        OwnerGuard<Expression> pExp = o_instantiateT(Expression, a_pInput->getObjectExpression());
        if (pExp == nullptr)
            return;
        Type* pReplicatedOwnerClassType =
        a_pInput->getObjectExpression()->getValueType()->replicate(pDM->getOwnerClassType());
        Expression* pConv = convertIfNotEqual(pExp, pReplicatedOwnerClassType, in_pContextScope);
        if (pConv == nullptr)
            return;
        pInstanciated = New<FieldExpression>(pConv, pDM);
    }
        return;
    }
    visit(static_cast<LValueExpression*>(a_pInput), a_Data);
}
void Semantic::visit(FieldInitializationStatement* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (a_pInput->isTemplateElement())
        {
            if (TemplateDependantExpression* pTDE =
                a_pInput->getInitializationExpression()->as<TemplateDependantExpression>())
            {
                const LanguageElements& unresolvedArguments =
                *pTDE->getTemplateDependantElement()->getFunctionArguments();
                OwnersGuard<Expressions> newArguments;
                for (auto it = unresolvedArguments.begin(); it != unresolvedArguments.end(); ++it)
                {
                    LanguageElement* pElement = o_instantiateT(LanguageElement, *it);
                    if (pElement)
                    {
                        Expression* pExpression = pElement->asExpression();
                        if (pExpression == nullptr)
                        {
                            CxxSemanticError("template dependant element is not an expression", FormatCStr(pElement));
                            //                             o_mapT(*it, nullptr);
                            //                             Delete(pElement);
                            return;
                        }
                        else
                            newArguments.push_back(pExpression);
                    }
                    else
                    {
                        return;
                    }
                }

                Field* pField = o_findT(Field, a_pInput->getField());
                PHANTOM_SEMANTIC_ASSERT(pField);
                Expression* pInitExp{};
                if (newArguments->size() == 0)
                    pInitExp = defaultConstruct(pField->getValueType(), in_pContextScope);
                else if (newArguments->size() == 1)
                    pInitExp = initialize(newArguments->front(), pField->getValueType(), in_pContextScope);
                else if (auto pCT = pField->getValueType()->asClassType())
                {
                    auto templateArgs = pTDE->getTemplateDependantElement()->getTemplateArguments();
                    pInitExp = static_cast<Expression*>(qualifiedLookup(
                    pCT, pCT->getName(), templateArgs ? OptionalArrayView<LanguageElement*>(*templateArgs) : NullOpt,
                    MakeArrayView(newArguments.elements()), in_pContextScope, nullptr, 0));
                }

                if (!pInitExp)
                    return;

                pInstanciated = New<FieldInitializationStatement>(pField, pInitExp);
                //                     :
                //
                //                 pInstanciated = unqualifiedLookup(
                //                 a_pInput->getField()->getName(), phantom::NullOpt,
                //                 MakeArrayView(newArguments.elements()),
                //                 in_pContextScope->asBlock()->getLocalVariableCascade("this"), nullptr, 0);
            }
            else
            {
                OwnerGuard<Expression> pInstanceInitExpression =
                o_instantiateT(Expression, a_pInput->getInitializationExpression());
                if (pInstanceInitExpression == nullptr)
                    return;
                Field* pField = o_findT(Field, a_pInput->getField());
                PHANTOM_SEMANTIC_ASSERT(pField);
                Expression* pConv =
                convertIfNotEqual(pInstanceInitExpression, pField->getValueType(), in_pContextScope);
                if (pConv == nullptr)
                    return;
                pInstanciated = New<FieldInitializationStatement>(pField, pConv);
            }
        }
        else
        {
            pInstanciated = New<FieldInitializationStatement>(
            a_pInput->getField(), a_pInput->getInitializationExpression()->clone(getSource()));
        }
    }
        return;
    }
    visit(static_cast<Statement*>(a_pInput), a_Data);
}
void Semantic::visit(FieldPointerExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        PHANTOM_SEMANTIC_ASSERT(!(a_pInput->isTemplateDependant()));

        OwnerGuard<Expression> pObjExp = o_instantiateT(Expression, a_pInput->getObjectExpression());
        if (pObjExp == nullptr)
            return;
        OwnerGuard<Expression> pPtrExp = o_instantiateT(Expression, a_pInput->getFieldPointerExpression());
        if (pPtrExp == nullptr)
            return;
        FieldPointer* pDMPT = pPtrExp->getValueType()->removeReference()->asFieldPointer();
        CxxSemanticErrorReturnIf(pDMPT == nullptr, "template dependant : expected member pointer type expression");
        Expression* pConv =
        CxxSemanticConversionNE(pObjExp, pObjExp->getValueType()->replicate(pDMPT->getObjectType()));
        if (pConv == nullptr)
            return;
        pInstanciated = New<FieldPointerExpression>(pConv, pPtrExp);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(FieldPointer* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TypeConversion:
    {
        Type*                pOutput = *(Type* const*)a_Data.in[0];
        CastKind             castKind = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions iUserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        Conversion*&         conv = *(Conversion**)a_Data.out[0];
        ERefType             refType = baseConversion(a_pInput, pOutput, castKind);

        if (pOutput->isSame(a_pInput))
        {
            conv = (refType != e_RefType_None)
            ? newConv<DefaultConversionSequence>(newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
            : newConv<DefaultConversionSequence>((ConversionVal)a_pInput, (ConversionVal)pOutput);
            return; /// success X (Class::*) -> X (Class::*)
        }

        if (pOutput->asFieldPointer())
        {
            FieldPointer* pOutputFieldPointer = static_cast<FieldPointer*>(pOutput);

            if (a_pInput->getValueType()->isSame(pOutputFieldPointer->getValueType())) /// same data member Type* type
            {
                if ((a_pInput->getObjectType()->asClass() != nullptr) &&
                    (pOutputFieldPointer->getObjectType()->asClass() != nullptr))
                {
                    lang::Class* pInputClass = static_cast<lang::Class*>(a_pInput->getObjectType());
                    lang::Class* pOutputClass = static_cast<lang::Class*>(pOutputFieldPointer->getObjectType());
                    size_t       level = pInputClass->getInheritanceLevelFromBase(pOutputClass);
                    if (level != ~size_t(0)) // level != -1 => inheritance relation => pointers are related
                    {
                        auto* pPtr = newConv<FieldPointerConversion>(
                        a_pInput, pOutputFieldPointer, level, pInputClass->getBaseClassOffsetCascade(pOutputClass));

                        //                     if(adjustmentCount)
                        //                     {
                        //                         conv = (refType != e_RefType_None)
                        //                             ?
                        //                             newConv<DefaultConversionSequence>(pPtr,
                        //                             newConv<QualificationAdjustConversion>(constAdjustment.first,
                        //                             constAdjustment.second, adjustmentCount),
                        //                             newConv<RefConversion>(pOutput, refType
                        //                             == e_RefType_RValue)) :
                        //                             newConv<DefaultConversionSequence>(pPtr,
                        //                             newConv<QualificationAdjustConversion>(constAdjustment.first,
                        //                             constAdjustment.second, adjustmentCount));
                        //                     }
                        conv = (refType != e_RefType_None)
                        ? newConv<DefaultConversionSequence>(
                          pPtr, newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
                        : newConv<DefaultConversionSequence>(pPtr);
                        return; /// success X (Derived::*) -> X (Base::*)
                    }
                }
            }
        }
    }
    break;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        ClassType*                  pObjectType = o_resolveT(ClassType, a_pInput->getObjectType());
        if (pObjectType == nullptr)
            return;
        ClassType* pValueType = o_resolveT(ClassType, a_pInput->getValueType());
        if (pValueType == nullptr)
            return;
        pInstanciated = getSource()->fieldPointerType(pObjectType, pValueType);
    }
        return;
    }
    visit(static_cast<MemberPointer*>(a_pInput), a_Data);
}
void Semantic::visit(Pointer* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*  pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        Pointer*          pPointer = pArgument->asPointer();
        if (pPointer)
        {
            Type* pPointedDeducedType =
            templateArgumentDeduction(a_pInput->getPointeeType(), pPointer->getPointeeType(), deductions);
            if (pPointedDeducedType)
            {
                pDeducedArgument = pPointedDeducedType->makePointer();
            }
        }
    }
        return;

    case e_VisitorFunction_TypeConversion:
    {
        Type*                   pOutput = *(Type* const*)a_Data.in[0];
        CastKind                castKind = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions    iUserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        Conversion*&            conv = *(Conversion**)a_Data.out[0];
        std::pair<Type*, Type*> constAdjustment;
        int                     adjustmentCount = 0;
        ERefType                refType;
        conv = baseStandardConversion(a_pInput, pOutput, castKind, refType);
        if (conv)
            return;
        if (pOutput == PHANTOM_TYPEOF(bool))
        {
            auto numconv = newConv<NumericConversionT<void*, bool, CastKind::Implicit>>(a_pInput, PHANTOM_TYPEOF(bool));
            conv = refType != e_RefType_None
            ? newConv<DefaultConversionSequence>(numconv, newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
            : newConv<DefaultConversionSequence>(numconv);
            return;
        }
        switch (castKind)
        {
        case CastKind::Const:
        {
            Type* pInputNoConst = a_pInput->removeAllQualifiers();
            Type* pOutputNoConst = pOutput->removeAllQualifiers();
            if (pInputNoConst->isSame(a_pInput) && pOutputNoConst->isSame(pOutput))
            {
                return; /// input or output do not have const (or volatile ?) => const_cast fail
            }
            if (pInputNoConst->isSame(pOutputNoConst)) /// no-cv input == no-cv output => const_cast ok
            {
                conv = newConv<DefaultConversionSequence>(a_pInput, pOutput);
            }
            return;
        }
        case CastKind::Explicit:
        {
            if (pOutput->asIntegralType())
            {
                conv = newConv<DefaultConversionSequence>(a_pInput, (ConversionVal)pOutput);
                return; /// success X* -> int/float/double/...*
            }
            else if (pOutput->asPointerType())
            {
                // both class pointers => perform static_cast
                if (a_pInput->getPointeeType()->removeQualifiers()->asClassType() &&
                    pOutput->getUnderlyingType()->removeQualifiers()->asClassType())
                {
                    conv =
                    newConversion(a_pInput->removeAllQualifiers(), pOutput->removeAllQualifiers(), CastKind::Static);
                    return;
                }
                conv = newConv<DefaultConversionSequence>(a_pInput, (ConversionVal)pOutput);
                return; /// success X* -> Y*
            }
            break;
        }

        case CastKind::Reinterpret:
        {
            if (pOutput->asPointerType() || pOutput->asIntegralType())
            {
                if ((pOutput->removePointer()->asConstType() != nullptr) ==
                    (a_pInput->removePointer()->asConstType() != nullptr))
                // no const should be involved in the conversion
                {
                    conv = newConv<DefaultConversionSequence>(a_pInput, (ConversionVal)pOutput);
                }
                return; /// successX* -> Y*
            }
            break;
        }
        }
        /// ex: X*
        Pointer* pOutputPointerType = pOutput->asPointer();
        /// ex X const*
        if (pOutputPointerType)
        {
            if (pOutputPointerType->getPointeeType()->asConstType())
            {
                if (a_pInput->getPointeeType()->asConstType())
                {
                    conv =
                    newConversion(a_pInput->getUnderlyingType()->getUnderlyingType()->makePointer(),
                                  pOutput->getUnderlyingType()->getUnderlyingType()->makePointer(), CastKind::Static);
                    return;
                }
                /// X* => Y const*
                Pointer* adjustmentInput =
                static_cast<Pointer*>(pOutputPointerType->getPointeeType()->removeConst()->makePointer());
                constAdjustment.first = adjustmentInput;
                constAdjustment.second = pOutputPointerType;
                adjustmentCount++;
                pOutputPointerType = adjustmentInput;
            }
            else if (a_pInput->getPointeeType()->asConstType())
            {
                return; /// X const* -/-> Y*
            }
            if (a_pInput->isSame(pOutputPointerType))
            {
                if (adjustmentCount)
                {
                    conv = (refType != e_RefType_None)
                    ? newConv<DefaultConversionSequence>(newConv<QualificationAdjustConversion>(constAdjustment.first,
                                                                                                constAdjustment.second,
                                                                                                adjustmentCount),
                                                         newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
                    : newConv<DefaultConversionSequence>(newConv<QualificationAdjustConversion>(
                      constAdjustment.first, constAdjustment.second, adjustmentCount));
                }
                else
                {
                    conv = (refType != e_RefType_None)
                    ? newConv<DefaultConversionSequence>(newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
                    : newConv<DefaultConversionSequence>(a_pInput); // canonical conversion
                }
                return; /// success X* -> X const*
            }
            if (pOutputPointerType->getDataPointerLevel() == 1)
            {
                Type* pOutputPointeeType = pOutputPointerType->getPointeeType();
                if (a_pInput->getDataPointerLevel() == 1)
                {
                    Type* pInputPointeeType = a_pInput->getPointeeType();
                    if ((pInputPointeeType->asClass() != nullptr) && (pOutputPointeeType->asClass() != nullptr))
                    {
                        lang::Class* pInputClass = static_cast<lang::Class*>(pInputPointeeType);
                        lang::Class* pOutputClass = static_cast<lang::Class*>(pOutputPointeeType);
                        size_t       level = pInputClass->getInheritanceLevelFromBase(pOutputClass);

                        if (level != ~size_t(0)) // level != -1 => inheritance relation => pointers are related
                        {
                            auto* pPtr = newConv<PointerConversion>(
                            a_pInput, pOutputPointerType, level, pInputClass->getBaseClassOffsetCascade(pOutputClass));
                            if (adjustmentCount)
                            {
                                conv = (refType != e_RefType_None)
                                ? newConv<DefaultConversionSequence>(
                                  pPtr,
                                  newConv<QualificationAdjustConversion>(constAdjustment.first, constAdjustment.second,
                                                                         adjustmentCount),
                                  newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
                                : newConv<DefaultConversionSequence>(
                                  pPtr,
                                  newConv<QualificationAdjustConversion>(constAdjustment.first, constAdjustment.second,
                                                                         adjustmentCount));
                            }
                            else
                                conv = (refType != e_RefType_None)
                                ? newConv<DefaultConversionSequence>(
                                  pPtr, newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
                                : newConv<DefaultConversionSequence>(pPtr); // canonical conversion
                            return;                                         /// success Derived* -> Base*
                        }
                        else if (castKind == CastKind::Explicit || castKind == CastKind::Static)
                        {
                            level = pOutputClass->getInheritanceLevelFromBase(pInputClass);
                            if (level != ~size_t(0)) // level != -1 => inheritance relation => pointers are related
                            {
                                auto* pPtr = newConv<PointerConversion>(
                                a_pInput, pOutputPointerType, level,
                                -(ptrdiff_t)pOutputClass->getBaseClassOffsetCascade(pInputClass));
                                conv = (refType != e_RefType_None)
                                ? newConv<DefaultConversionSequence>(
                                  pPtr, newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
                                : newConv<DefaultConversionSequence>(pPtr);
                                return; /// success Base* -> Derived*
                            }
                        }
                    }
                }
                if (pOutputPointeeType == BuiltInTypes::TYPE_VOID)
                {
                    auto* pPtr = newConv<NumericConversion>(a_pInput, pOutputPointerType, false, ~size_t(0));
                    if (adjustmentCount)
                    {
                        conv = (refType != e_RefType_None)
                        ? newConv<DefaultConversionSequence>(
                          pPtr,
                          newConv<QualificationAdjustConversion>(constAdjustment.first, constAdjustment.second,
                                                                 adjustmentCount),
                          newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
                        : newConv<DefaultConversionSequence>(
                          pPtr,
                          newConv<QualificationAdjustConversion>(constAdjustment.first, constAdjustment.second,
                                                                 adjustmentCount));
                    }
                    else
                        conv = (refType != e_RefType_None)
                        ? newConv<DefaultConversionSequence>(
                          pPtr, newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
                        : newConv<DefaultConversionSequence>(pPtr); // canonical conversion
                    return;                                         /// success X* -> void*
                }
            }
            if (castKind == CastKind::Explicit) /// If nothing has been found for explicit until
                                                /// now, consider it as a pointer reinterpretation
            {
                if (pOutput->removeQualifiers()->asPointer() ||
                    pOutput->asIntegralType() && pOutput->getSize() >= sizeof(void*))
                {
                    conv = newConv<DefaultConversionSequence>(a_pInput, (ConversionVal)pOutput);
                    return; /// success X* -> Y* or int_X -> Y*
                }
            }
        }
    }
    break;

    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        Type*                       pType = o_resolveT(Type, a_pInput->getPointeeType());
        if (pType == nullptr)
            return;
        if (pType->asReference())
            CxxSemanticError("'%s' : pointer to reference is illegal", FormatCStr(pType));

        Type* pResolvedType = pType->makePointer();
        PHANTOM_SEMANTIC_ASSERT(pResolvedType);
        pResolved = pResolvedType;
    }
        return;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void Semantic::visit(DeallocateExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Expression*                 pExp = o_instantiateT(Expression, a_pInput->getAddressExpression());
        if (pExp == nullptr)
            return;
        Type* pType = o_resolveT(Type, a_pInput->getType());
        if (pType == nullptr)
            return;
        if (pType->asReference())
        {
            CxxSemanticError("'%s' : references cannot be allocated", FormatCStr(pType));
            return;
        }
        Expression* pSizeExp = o_instantiateT(Expression, a_pInput->getSizeExpression());
        if (pSizeExp == nullptr)
            return;
        OwnerGuard<Expression> pSizeExpConv = CxxSemanticConversionNE(pSizeExp, PHANTOM_TYPEOF(size_t));
        if (pSizeExpConv == nullptr)
            return;
        OwnerGuard<Expression> pExpConv = CxxSemanticConversionNE(pExp, pType->makePointer());
        if (pExpConv == nullptr)
            return;
        pInstanciated = New<DeallocateExpression>(pType, pSizeExpConv, pExpConv);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(DeleteExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Class*                      pClass = nullptr;
        if (a_pInput->getClass())
        {
            pClass = o_resolveT(Class, a_pInput->getClass());
            if (pClass == nullptr)
                return;
        }
        Expression* pDeletedExp = o_instantiateT(Expression, a_pInput->getExpression());
        if (pDeletedExp == nullptr)
            return;
        pInstanciated = New<DeleteExpression>(pClass, pDeletedExp);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(Ellipsis* a_pInput, VisitorData a_Data)
{ /*visit(static_cast<LanguageElement*>(a_pInput), a_Data);*/
}
void Semantic::visit(Enum* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TypeConversion:
    {
        Type*                pOutput = *(Type* const*)a_Data.in[0];
        CastKind             castKind = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions iUserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        Conversion*&         conv = *(Conversion**)a_Data.out[0];
        ERefType             refType;
        baseStandardConversion(a_pInput, pOutput, castKind, refType);
        if (pOutput->isSame(a_pInput))
        {
            conv = (refType != e_RefType_None)
            ? newConv<DefaultConversionSequence>(newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
            : newConv<DefaultConversionSequence>(a_pInput);
            return;
        }
        visit(static_cast<Type*>(a_pInput), a_Data);
        if (conv == nullptr)
        {
            if (!a_pInput->isScoped() || castKind >= CastKind::Reinterpret)
            {
                a_pInput->getUnderlyingIntType()->visit(this, a_Data);
                if (conv)
                {
                    if (conv->semantic)
                        conv = conv->clone(getSource());
                    conv->input = a_pInput;
                    if ((refType != e_RefType_None))
                    {
                        static_cast<DefaultConversionSequence*>(conv)->addConstRef();
                    }
                    return;
                }
            }
        }
        return;
    }
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnerGuard<Enum>            pInstantiatedEnum = nullptr;

        PrimitiveType* pIntType = o_resolveT(PrimitiveType, a_pInput->getUnderlyingIntType());

        CxxSemanticErrorReturnIf(!pIntType, "template instantiation : unable to solve enum type '%.*s'",
                                 PHANTOM_STRING_AS_PRINTF_ARG(a_pInput->getUnderlyingIntType()->getName()));
        if (!pIntType)
            return;
        pInstantiatedEnum = New<Enum>(a_pInput->getName(), pIntType);
        for (auto pInputCst : a_pInput->getConstants())
        {
            Constant* pCst = o_instantiateT(Constant, pInputCst);
            CxxSemanticErrorReturnIf(!pCst, "template instantiation : unable to solve enum constant '%.*s'",
                                     PHANTOM_STRING_AS_PRINTF_ARG(pInputCst->getName()));
            unsigned long long val;
            pCst->getValue(&val);
            Delete(pCst);
            pInstantiatedEnum->addConstant(pInstantiatedEnum->createConstant(&val, pInputCst->getName()));
        }
        return;
    }
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void Semantic::visit(Evaluable* a_pInput, VisitorData a_Data)
{
    visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}

void Semantic::visit(Expression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_HasSideEffect:
    {
        bool& bResult = *(bool*)a_Data.out[0];
        for (auto pElem : a_pInput->getElements())
        {
            if (Expression* pExp = pElem->asExpression())
            {
                if (hasSideEffects(pExp))
                {
                    bResult = true;
                    return;
                }
            }
        }
        bResult = false;
    }
        return;
    case e_VisitorFunction_ExpressionConversion:
    {
        Type*                in_pOutput = *(Type* const*)a_Data.in[0];
        CastKind             in_ConversionType = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions in_UserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        LanguageElement*     in_pContextScope = *(LanguageElement* const*)a_Data.in[3];
        bool                 in_bInitialize = *(bool* const*)a_Data.in[4];
        Expression*&         out_pResult = *(Expression**)a_Data.out[0];
        Conversion*          conv = newConversion(a_pInput->getValueType(), in_pOutput, in_ConversionType,
                                         in_UserDefinedConversionsAllowed, in_pContextScope, in_bInitialize);
        if (conv)
        {
            out_pResult = conv->convert(this, a_pInput);
            PHANTOM_SEMANTIC_ASSERT(out_pResult);
            PHANTOM_SEMANTIC_ASSERT(conv->semantic);
        }
    }
        return;
    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        *(LanguageElement**)a_Data.out[0] =
        o_instantiateT(Expression,
                       a_pInput); // if resolve is required for an expression, fallback to instantiation
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        PHANTOM_SEMANTIC_ASSERT(false);
    }
        return;

    case e_VisitorFunction_ToExpression:
    {
        Expression*  in_pLeftExpression = *(Expression**)a_Data.in[0];
        Expression*& out_pExpression = *(Expression**)a_Data.out[0];
        if (in_pLeftExpression)
            return;
        out_pExpression = a_pInput;
    }
        return;

    case e_VisitorFunction_QualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        LanguageElement*                    in_pContextScope = *(LanguageElement**)a_Data.in[3];
        Type*                               in_pInitializationType = (Type*)a_Data.in[4];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];

        Type* pExpNoRefType = a_pInput->getValueType()->removeReference();
        Type* pExpressionEffectiveType = pExpNoRefType->removeConst();
        if (pExpressionEffectiveType->isTemplateDependant())
        {
            out_pResult = New<TemplateDependantExpression>(New<TemplateDependantElement>(
            a_pInput, in_name, in_pTemplateArguments, *(OptionalArrayView<LanguageElement*>*)&in_pFunctionArguments));
            return;
        }
        if (ClassType* pExpCT = pExpressionEffectiveType->asClassType())
        {
            // special .cpplite placement new syntax :
            // ex : stringViewPtr->StringView()
            if (pExpressionEffectiveType->getName() == in_name && in_pFunctionArguments && !a_pInput->isImplicit())
            {
                ConstructorCallExpression* pCtorCallExpr = static_cast<ConstructorCallExpression*>(
                silentQualifiedLookup(pExpressionEffectiveType, pExpressionEffectiveType->getName(),
                                      in_pTemplateArguments, in_pFunctionArguments, in_pContextScope, nullptr));
                if (!pCtorCallExpr)
                    return;
                PHANTOM_SEMANTIC_ASSERT(pCtorCallExpr->getMetaClass() == PHANTOM_CLASSOF(ConstructorCallExpression));
                // forbid no-ref
                CxxSemanticErrorReturnIf(pExpNoRefType == a_pInput->getValueType(),
                                         "'%.*s': illegal placement on non-address value");
                // forbid const
                CxxSemanticErrorReturnIf(pExpNoRefType != pExpressionEffectiveType,
                                         "'%.*s': cannot invoke placement constructor on const value",
                                         PHANTOM_STRING_AS_PRINTF_ARG(in_name));

                out_pResult = New<PlacementNewExpression>(a_pInput->address(getSource()), pCtorCallExpr);
                return;
            }

            // continue with c++ traditional lookup
            /// Find all candidates
            Symbols  candidates;
            Accesses outAccesses;
            internalQualifiedSymbolsLookup(pExpressionEffectiveType, in_name, candidates, outAccesses, in_pContextScope,
                                           Access::Public);

            if (candidates.size())
            {
                bool bIsTemplateDependant = a_pInput->isTemplateElement();

                for (auto it = candidates.begin(); it != candidates.end(); ++it)
                {
                    Symbol* pSymbol = (*it)->asSymbol();
                    Alias*  pAlias = pSymbol->asAlias();
                    if ((a_Data.flags & e_VisitorFlag_KeepAliases) == 0 && pAlias && pAlias->getAliasedSymbol())
                    {
                        *it = pAlias->getAliasedSymbol();
                    }
                }

                /// If this is a call
                if (in_pFunctionArguments)
                {
                    for (auto it = in_pFunctionArguments->begin(); it != in_pFunctionArguments->end(); ++it)
                    {
                        bIsTemplateDependant = bIsTemplateDependant || (*it)->isTemplateDependant();
                    }
                    if (bIsTemplateDependant)
                    {
                        out_pResult = New<TemplateDependantExpression>(New<TemplateDependantElement>(
                        a_Data.hasFlag(e_VisitorFlag_Unqualified) ? nullptr : a_pInput, in_name, NullOpt,
                        *(OptionalArrayView<LanguageElement*>*)&in_pFunctionArguments));
                        return;
                    }
                    Expressions arguments;
                    arguments.push_back(a_pInput); // add 'this'
                    arguments.insert(arguments.end(), in_pFunctionArguments->begin(),
                                     in_pFunctionArguments->end()); // then arguments
                    Subroutines viableCallCandidates;
                    selectCallCandidate(viableCallCandidates, candidates, in_pTemplateArguments, arguments, out_pResult,
                                        in_name, in_pContextScope, in_pInitializationType, 0, 0);
                    if (out_pResult == nullptr)
                    {
                        if (viableCallCandidates.size())
                        {
                            _selectCallCandidateError(in_name, candidates, viableCallCandidates, arguments);
                            return;
                        }
                        Field* pField = nullptr;
                        if (candidates.size() == 1 && (pField = candidates.back()->asField()))
                        {
                            Expression* pLHS = arguments[0];
                            if (pLHS->getValueType()->asReference() == nullptr)
                                pLHS = New<RValueReferenceExpression>(pLHS);

                            FieldExpression* pFieldExpression = New<FieldExpression>(pLHS, pField);
                            arguments.erase(arguments.begin());
                            if (MethodPointer* pMFPT = pFieldExpression->getValueType()
                                                       ->removeReference()
                                                       ->removeQualifiers()
                                                       ->asMethodPointer())
                            {
                                if (arguments.empty())
                                    return;
                                Expression* pThis = arguments.front();
                                arguments.erase(arguments.begin());
                                if (pMFPT->isTemplateDependant() || pThis->isTemplateDependant())
                                {
                                    out_pResult = New<TemplateDependantExpression>(New<TemplateDependantElement>(
                                    a_Data.hasFlag(e_VisitorFlag_Unqualified) ? nullptr : a_pInput, in_name, NullOpt,
                                    *(OptionalArrayView<LanguageElement*>*)&in_pFunctionArguments));
                                    return;
                                }
                                if (pMFPT->acceptsCallerExpressionType(pThis->getValueType()))
                                {
                                    if (pMFPT->getParameterTypeCount() > arguments.size())
                                    {
                                        CxxSemanticError("'%s' : not enough arguments", FormatCStr(pMFPT));
                                        return;
                                    }
                                    if (pMFPT->getParameterTypeCount() < arguments.size())
                                    {
                                        CxxSemanticError("'%s' : too many arguments", FormatCStr(pMFPT));
                                        return;
                                    }
                                    ConversionResults convs;
                                    newImplicitConversions(pMFPT->getFunctionType(), arguments, convs, in_pContextScope,
                                                           UserDefinedFunctions::ImplicitsOnly);
                                    if (!convs.hasNull())
                                    {
                                        for (size_t i = 0; i < arguments.size(); ++i)
                                        {
                                            arguments[i] = convs[i]->convert(this, arguments[i]);
                                            PHANTOM_SEMANTIC_ASSERT(arguments[i]);
                                        }
                                        out_pResult = New<MethodPointerCallExpression>(
                                        pThis->dereference(getSource()), pFieldExpression->load(getSource()),
                                        arguments);
                                    }
                                }
                                return;
                            }
                            else
                            {
                                out_pResult =
                                qualifiedLookup(pFieldExpression, "operator()", NullOpt, MakeArrayView(arguments),
                                                in_pContextScope, in_pInitializationType, a_Data.flags);
                            }
                        }
                        else
                        {
                            CxxSemanticError("'%s' : no overload found matching the arguments (%s), possible "
                                             "overloads are :",
                                             (const char*)in_name.data(),
                                             FormatArgumentList(arguments.data() + 1, arguments.size() - 1).data());
                            for (size_t i = 0; i < candidates.size(); ++i)
                            {
                                CxxSemanticSubError("%s", FormatCStr(candidates[i]));
                            }
                            out_pResult = nullptr;
                        }
                    }
                }
                else
                {
                    if (in_pTemplateArguments)
                    {
                        out_pResult = nullptr; // template calls not supported yet
                        return;
                    }
                    /// if no function signature && multiple candidates => ambiguous
                    if (candidates.size() == 1)
                    {
                        Expression* pExpression = toExpression(static_cast<Symbol*>(candidates[0]), a_pInput);
                        if (pExpression)
                        {
                            out_pResult = pExpression;
                        }
                        else
                        {
                            out_pResult = nullptr;
                            // CxxSemanticError(e_Error_NoMember, &in_name,
                            // &pExpressionEffectiveType);
                            return;
                        }
                    }
                    else
                    {
                        out_pResult = nullptr;
                        auto   errArgs = candidates.data();
                        size_t count = candidates.size();
                        CxxSemanticError("'%s' : ambiguous access to multiple members, candidates are :",
                                         (const char*)in_name.data());
                        for (auto c : candidates)
                        {
                            CxxSemanticSubError("%s", FormatCStr(c));
                        }
                        return;
                    }
                }
            }
            else
            {
                out_pResult = nullptr;
                // CxxSemanticError(e_Error_NoMember, &in_name, &pExpressionEffectiveType);
                return;
            }
        }
        else
        {
            out_pResult = nullptr;
            // CxxSemanticError(e_Error_UnexpectedMember, &in_name, &pExpressionEffectiveType);
            return;
        }
    }
    break;
    }
}
void Semantic::visit(ExpressionStatement* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Expression*                 pExp = o_instantiateT(Expression, a_pInput->getExpression());
        if (pExp == nullptr)
            return;
        pInstanciated = New<ExpressionStatement>(pExp);
    }
        return;
    }
    visit(static_cast<Statement*>(a_pInput), a_Data);
}

#if 0
Expression* resolveGenericCall(ast::_BaseRule* input, LanguageElement* a_pLHS, const char* a_Identifier,
                               ast::TemplateArgumentList* a_pTemplateArgs, ast::CallList* a_pCallList)
{
    CppLiteDefaultReturnValue(false);

    //             Symbols symbols;
    //             Accesses accesses;
    //             CppLiteGetSemantic()->unqualifiedSymbolsLookup(a_Identifier, symbols, &accesses,
    //             CppLiteGetScopeAs(LanguageElement)); if (symbols.empty())
    //             {
    //                 return nullptr;
    //             }
    //             Symbols candidates;
    //             for (Symbol* pSymbol : symbols)
    //             {
    //                 if (Subroutine * pSubroutine = pSymbol->asSubroutine())
    //                 {
    //                     if (!checkGenericSignature(pSubroutine->getSignature(),
    //                     a_pTemplateArgs->m_TemplateArguments->size()))
    //                         continue;
    //                     candidates.push_back(pSubroutine);
    //                 }
    //                 else
    //                 {
    //                     CppLiteError("'%s' is not a method or function", a_Identifier);
    //                     return nullptr;
    //                 }
    //             }
    //             if (candidates.empty())
    //             {
    //                 CppLiteError("'%s' is not a template method or function", a_Identifier);
    //                 return nullptr;
    //             }

    Expressions args;
    CppLitesGuard(args);

    LanguageElements templateArgs;
    CppLitesGuard(templateArgs);
    if (!resolveTemplateArguments(a_pTemplateArgs->m_TemplateArguments, templateArgs))
        return nullptr;

    Types requiredBaseTypes;

    Types templateArgTypes;
    for (auto templateArg : templateArgs)
    {
        if (Type* pType = templateArg->asType())
        {
            templateArgTypes.push_back(pType);
            continue;
        }
        // CppLiteError("'%s' : non-type template parameter are not supported yet", a_Identifier);
        return nullptr;
    }

    // push generic types
    for (Type* pType : templateArgTypes)
    {
        args.push_back(CppLiteGetSemantic()->convert(New<ConstantExpression>(newConstant(size_t(pType))),
                                                  PHANTOM_TYPEOF(Type*), CastKind::Reinterpret,
                                                  UserDefinedFunctions::None, CppLiteGetScope()));
    }

    if (!resolveCallList(a_pCallList, args))
        return false;

    LanguageElement* pResult = nullptr;
    if (a_pLHS)
        pResult = CppLiteGetSemantic()->silentQualifiedLookup(a_pLHS, a_Identifier, phantom::NullOpt, MakeArrayView(args),
                                                           CppLiteGetScope(), CppLiteGetInitializationType());
    else
        pResult = CppLiteGetSemantic()->silentUnqualifiedLookup(a_Identifier, phantom::NullOpt, MakeArrayView(args),
                                                             CppLiteGetScope(), CppLiteGetInitializationType());

    if (!pResult)
        return nullptr;

    if (Expression* pExp = pResult->asExpression())
    {
        CppLiteGuard(pExp);
        if (CallExpression* pCallExp = phantom::Object::Cast<CallExpression>(pExp->removeRValueReferenceExpression()))
        {
            Subroutine* pSubroutine = pCallExp->getSubroutine();
            if (!checkGenericSignature(pSubroutine->getSignature(), a_pTemplateArgs->m_TemplateArguments->size()))
            {
                // CppLiteError("'%s' : invalid generic signature : invalid generic parameters
                // count", a_Identifier);
                return nullptr;
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
                                    i + (pSubroutine->asMethod() != nullptr), [&](Expression*) {
                                        return CppLiteGetSemantic()->convert(args[i], pExpectedType, CppLiteGetScope());
                                    }))
                                {
                                    // failed to match argument pattern
                                    return nullptr;
                                }
                            }
                        }
                        else
                        {
                            CppLiteError("'%s' : invalid generic signature : param index "
                                      "'%zu' out of range",
                                      a_Identifier, value);
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
                        pExp = CppLiteGetSemantic()->convert(pExp, pReturnTypeBase, CastKind::Explicit,
                                                          UserDefinedFunctions::All, CppLiteGetScope());
                        PHANTOM_SEMANTIC_ASSERT(pExp, "operator X() should have been called");
                        Expression* pConv = CppLiteGetSemantic()->convert(pExp, pReturnType, CastKind::Reinterpret);
                        if (pConv == nullptr)
                        {
                            CppLiteError("'%s' : cannot convert to generic return", a_Identifier);
                            return nullptr;
                        }
                        return pConv;
                    }
                    else
                    {
                        CppLiteError("'%s' : invalid generic signature : param index '%zu' out of range", a_Identifier,
                                  value);
                        return nullptr;
                    }
                }

                int paramIndex;
            }
        }
        return pExp;
    }
    CppLiteError("'%s' : not an expression", a_Identifier);
    return nullptr;
}

#endif

void Semantic::visit(Function* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_CreateCallExpression:
    {
        ExpressionsView in_Arguments = *(const ExpressionsView*)a_Data.in[0];
        Expression*     in_pContextScope = *(Expression**)a_Data.in[1];
        Expression*&    out_pResult = *(Expression**)a_Data.out[0];
        out_pResult = createCallExpression(a_pInput, in_Arguments, in_pContextScope);
    }
        return;
    case e_VisitorFunction_IsViableCallCandidate:
    {
        OptionalArrayView<LanguageElement*>& templateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[0];
        TypesView                            functionArguments = *(const TypesView*)a_Data.in[1];
        Modifiers                            modifiers = *(Modifiers*)a_Data.in[2];
        LanguageElement*                     pScope = *(LanguageElement**)a_Data.in[3];
        UserDefinedFunctions                 convUserDefinedFunctions = *(UserDefinedFunctions*)a_Data.in[4];
        ConversionResults&                   out_conversions = *(ConversionResults*)a_Data.out[0];
        Subroutine*&                         out_subroutine = *(Subroutine**)a_Data.out[1];
        PlaceholderMap&                      out_deductions = *(PlaceholderMap*)a_Data.out[2];
        out_subroutine = nullptr;

        if (modifiers & PHANTOM_R_STATIC) // static call (no this implied)
        {
            if (functionArguments.size() < a_pInput->getRequiredArgumentCount() ||
                (!(a_pInput->getSignature()->isVariadic()) &&
                 functionArguments.size() > a_pInput->getParameters().size()))
            {
                return;
            }
            Types argumentTypes;
            auto& params = a_pInput->getParameters();
            for (size_t i = 0; i < params.size(); ++i)
            {
                if (i < functionArguments.size())
                {
                    argumentTypes.push_back(functionArguments[i]);
                }
            }

            if (auto pTSpec = a_pInput->getTemplateSpecialization())
            {
                if (!_useExplicitTemplateArguments(a_pInput, pTSpec, templateArguments, out_deductions, "function"))
                    return;
                newImplicitConversionsWithArgDeductions(a_pInput->getSignature(), argumentTypes, out_conversions,
                                                        out_deductions, pScope, convUserDefinedFunctions);
            }
            else
                newImplicitConversions(a_pInput->getSignature(), argumentTypes, out_conversions, pScope,
                                       convUserDefinedFunctions);

            if (!out_conversions.hasNull())
            {
                out_subroutine = a_pInput;
            }
        }
        else /// not static => first argument is the implicit object argument => we skip it but
             /// first assert that it is compatible
        {
#if PHANTOM_DEBUG_LEVEL >= PHANTOM_DEBUG_LEVEL_ASSERTS
            auto owner = a_pInput->getOwner();
            auto ts = owner->asTemplateSpecialization();
            auto pOwnerClassType = ts ? ts->getTemplate()->getOwner()->asClassType() : owner->asClassType();

            PHANTOM_SEMANTIC_ASSERT(pOwnerClassType);
            PHANTOM_SEMANTIC_ASSERT(functionArguments.front()
                                    ->getValueType()
                                    ->removeReference()
                                    ->removeRValueReference()
                                    ->removeQualifiers()
                                    ->isA(pOwnerClassType));
#endif
            if ((functionArguments.size() - 1) < a_pInput->getRequiredArgumentCount() ||
                (!a_pInput->getSignature()->isVariadic() &&
                 ((functionArguments.size() - 1) > a_pInput->getParameters().size())))
            {
                out_subroutine = nullptr;
                return;
            }
            Types argTypes;
            auto& params = a_pInput->getParameters();
            for (size_t i = 0; i < params.size(); ++i)
            {
                if (i < (functionArguments.size() - 1))
                {
                    argTypes.push_back(functionArguments[i + 1]);
                }
            }
            if (auto pTSpec = a_pInput->getTemplateSpecialization())
            {
                if (!_useExplicitTemplateArguments(a_pInput, pTSpec, templateArguments, out_deductions, "function"))
                    return;
                newImplicitConversionsWithArgDeductions(a_pInput->getSignature(), argTypes, out_conversions,
                                                        out_deductions, pScope, convUserDefinedFunctions);
            }
            else
                newImplicitConversions(a_pInput->getSignature(), argTypes, out_conversions, pScope,
                                       convUserDefinedFunctions);
            if (!out_conversions.hasNull())
            {
                out_subroutine = a_pInput;
            }
        }
        return;
    }
    case e_VisitorFunction_IsViableSignatureCandidate:
    {
        const LanguageElements* templateArguments = a_Data.in[0] ? (const LanguageElements*)a_Data.in[0] : nullptr;
        TypesView               types = *(const Types*)a_Data.in[1];
        Modifiers               modifiers = *(Modifiers*)a_Data.in[2];
        bool&                   out_result = *(bool*)a_Data.out[0];

        out_result = templateArguments ? a_pInput->matches(*templateArguments, types, modifiers)
                                       : a_pInput->matches(types, modifiers);
    }
    break;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];

        ClassType*              pInstanceClassType = in_pContextScope->asClassType();
        TemplateSpecialization* pSpec = {};
        if (!pInstanceClassType)
        {
            if (pSpec = in_pContextScope->asTemplateSpecialization())
                pInstanceClassType = pSpec->getTemplate()->getOwner()->asClassType();
        }

        switch (in_eClassBuildState)
        {
        case e_ClassBuildState_None:
            if (pSpec)
            {
                Signature* pSignature = o_instantiateT(Signature, a_pInput->getSignature());
                if (pSignature == nullptr)
                    return;
                PHANTOM_SEMANTIC_ASSERT(pSpec);
                OwnerGuard<Function> pInstanceFunction =
                New<Function>(a_pInput->getName(), pSignature, a_pInput->getABI(), a_pInput->getModifiers());

                // TODO : rework all the template instantiation final process
                // small hack to ensure unqualified look ups are OK !
                pSpec->setTemplated(pInstanceFunction);

                bool        accepted = false;
                Subroutines conflicting;
                if (pInstanceClassType)
                    accepted = pInstanceClassType->acceptsSubroutine(pInstanceFunction.element(), &conflicting);
                else
                    accepted = pSpec->getTemplate()->getOwner()->asSource()->acceptsSubroutine(
                    pInstanceFunction.element(), &conflicting);
                if (!accepted)
                {
                    CxxSemanticError("'%s' : function conflicts without previously declared one(s) :",
                                     FormatCStr(pInstanceFunction.element()));
                    for (auto c : conflicting)
                    {
                        CxxSemanticSubError("'%s'", FormatCStr(c));
                    }
                    return;
                }
                pInstanciated = pInstanceFunction;
                goto instantiate_blocks;

                // continue to case e_ClassBuildState_Blocks when template function instantiation
            }
            if (!pSpec)
                break;
        case e_ClassBuildState_Members:
        {
            Signature* pSignature = o_instantiateT(Signature, a_pInput->getSignature());
            if (pSignature == nullptr)
                return;
            if (pInstanceClassType == nullptr)
                return;
            OwnerGuard<Function> pInstanceFunction =
            New<Function>(a_pInput->getName(), pSignature, a_pInput->getABI(), a_pInput->getModifiers());
            Subroutines conflicting;
            if (!(pInstanceClassType->acceptsSubroutine(pInstanceFunction.element(), &conflicting)))
            {
                CxxSemanticError("'%s' : function conflicts without previously declared one(s) :",
                                 FormatCStr(pInstanceFunction.element()));
                for (auto c : conflicting)
                {
                    CxxSemanticSubError("'%s'", FormatCStr(c));
                }
                return;
            }
            pInstanceClassType->addFunction(pInstanceFunction);
            pInstanciated = pInstanceFunction;
        }
        break;
        case e_ClassBuildState_Blocks:
        {
        instantiate_blocks:
            if (pInstanciated && a_pInput->getBlock())
            {
                if (!a_pInput->buildBlock())
                {
                    CxxSemanticError("failed to instantiate block of function '%.*s'",
                                     PHANTOM_STRING_AS_PRINTF_ARG(a_pInput->getName()));
                    pInstanciated = nullptr;
                    return;
                }
                Function* pInstanceFunc = static_cast<Function*>(pInstanciated);
                o_mapT(a_pInput->getBlock(), addBlock(pInstanceFunc, true));

                pInstanceFunc->setBlockBuilder([=](Block* a_pBlock) {
                    pInstanceFunc->setBlockBuilder(
                    {}); // avoid cyclic builder invocation if function is recursive and calls itself
                    PHANTOM_SEMANTIC_ASSERT(a_pBlock == pInstanceFunc->getBlock());
                    if (!SemanticPrivate::SemanticInstantiateTemplateElement<Block>(
                        this, a_pInput->getBlock(), in_TemplateSubstitution, e_ClassBuildState_Blocks, pInstanceFunc,
                        a_Data.flags, &LanguageElement::asBlock))
                        return false;
                    return true;
                });

#if PHANTOM_CODE_LAZY_COMPILATION_ENABLED == 0
                if (!pInstanceFunc->buildBlock())
                {
                    CxxSemanticError("failed to instantiate block of function '%.*s'",
                                     PHANTOM_STRING_AS_PRINTF_ARG(a_pInput->getName()));
                    pInstanciated = nullptr;
                }
#endif
            }
        }
        break;
        }
    }
        return;
    }
    visit(static_cast<Subroutine*>(a_pInput), a_Data);
} // namespace lang

void Semantic::visit(FunctionPointer* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TypeConversion:
    {
        Type*                pOutput = *(Type* const*)a_Data.in[0];
        CastKind             castKind = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions iUserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        Conversion*&         conv = *(Conversion**)a_Data.out[0];

        ERefType refType = baseConversion(a_pInput, pOutput, castKind);
        if (pOutput == BuiltInTypes::TYPE_VOID_PTR)
        {
            auto* pPtr = newConv<PointerConversion>(a_pInput, pOutput);
            conv = (refType != e_RefType_None)
            ? newConv<DefaultConversionSequence>(pPtr, newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
            : newConv<DefaultConversionSequence>(pPtr);
            return; /// success X(*)(Y,Z) -> void*
        }
        else if (pOutput->isSame(a_pInput))
        {
            conv = (refType != e_RefType_None)
            ? newConv<DefaultConversionSequence>(newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
            : newConv<DefaultConversionSequence>(a_pInput, (ConversionVal)pOutput);
            return; /// success X(*)(Y,Z) -> X(*)(Y,Z)
        }
    }
    break;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Types                       parameters;
        for (auto p : a_pInput->getFunctionType()->getParameterTypes())
        {
            parameters.push_back(o_resolveT(Type, p));
        }
        pInstanciated =
        getSource()->functionPointerType(o_resolveT(FunctionType, a_pInput->getFunctionType()), a_pInput->getABI());
    }
        return;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}

void Semantic::visit(FunctionType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}

void Semantic::visit(IdentityExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_HasSideEffect:
    {
        a_pInput->getExpression()->visit(this, a_Data);
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Expression*                 pExpression = o_instantiateT(Expression, a_pInput->getExpression());
        if (pExpression == nullptr)
        {
            return;
        }
        pInstanciated = New<IdentityExpression>(o_resolveT(Type, a_pInput->getValueType()), pExpression);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}

void Semantic::visit(InitializerListExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ExpressionConversion:
    {
        Type*    in_pOutput = *(Type* const*)a_Data.in[0];
        CastKind in_ConversionType = *(CastKind const*)a_Data.in[1];
        if (in_ConversionType != CastKind::Implicit)
            return;

        UserDefinedFunctions in_UserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        LanguageElement*     in_pContextScope = *(LanguageElement* const*)a_Data.in[3];
        Expression*&         out_pResult = *(Expression**)a_Data.out[0];

        if (in_pOutput->isTemplateDependant() || a_pInput->isTemplateDependant())
        {
            out_pResult = a_pInput;
        }

        ERefType refType = in_pOutput->asConstLValueReference()
        ? e_RefType_LValue
        : in_pOutput->asRValueReference() ? e_RefType_RValue : e_RefType_None;
        Type* pOutput = in_pOutput->removeConstReference();
        pOutput = pOutput->removeRValueReference();

        TemplateSpecialization* pTemplateSpecialization = pOutput->getTemplateSpecialization();
        if (pTemplateSpecialization &&
            pTemplateSpecialization->getTemplate()->getQualifiedName() == "std::initializer_list")
        {
            if (in_pOutput->isSame(a_pInput->getValueType()))
            {
                out_pResult = a_pInput;
                return;
            }
            Conversion* conv = newConversion(a_pInput->getValueType(), in_pOutput, in_ConversionType,
                                             in_UserDefinedConversionsAllowed, in_pContextScope);
            if (conv)
            {
                out_pResult = conv->convert(this, a_pInput);
                PHANTOM_SEMANTIC_ASSERT(conv->semantic);
                return;
            }
        }

        // list initialization
        OwnersGuard<Expressions> exprs;
        Expressions const&       initListExpressions = a_pInput->getExpressions();
        CxxSemanticErrorReturnIf(in_UserDefinedConversionsAllowed != UserDefinedFunctions::All &&
                                 initListExpressions.empty(),
                                 "{} : illegal empty initializer list");
        ClassType* pClassType;
        Array*     pArray = in_pOutput->asArray();
        if (pArray)
        {
            CxxSemanticErrorReturnIf(in_ConversionType != CastKind::Implicit,
                                     "'%s' : illegal explicit array conversion", FormatCStr(pArray));
            CxxSemanticErrorReturnIf(pArray->hasAuto(), "'%s' : illegal auto array", FormatCStr(pArray));
            // array initializer
            for (size_t i = 0; i < initListExpressions.size(); ++i)
            {
                Expression* pExpr = initListExpressions[i];
                Expression* pConvExpr = CxxSemanticConversion(pExpr, pArray->getItemType());
                if (pConvExpr == nullptr)
                    return;
                exprs.push_back(pConvExpr);
            }
            out_pResult = New<ArrayExpression>(pArray, exprs);
            return;
        }
        else if (pClassType = in_pOutput->asClassType())
        {
            if (initListExpressions.empty())
            {
                PHANTOM_SEMANTIC_ASSERT(in_ConversionType == CastKind::Explicit);
                // default construction
                out_pResult = defaultConstruct(pOutput);
            }
            else
            {
                // list initialization or std::initializer_list copy constructor
                {
                    Expressions expressions;
                    expressions.push_back(a_pInput);
                    LanguageElement* pElement =
                    silentQualifiedLookup(pClassType, pClassType->getName(), NullOpt, MakeArrayView(expressions),
                                          in_pContextScope, nullptr, 0);
                    if (pElement)
                    {
                        PHANTOM_SEMANTIC_ASSERT(pElement->asExpression() || pElement->isTemplateDependant());
                        Expression* pExp = static_cast<Expression*>(pElement)->loadRValueReferenceExpression();
                        PHANTOM_SEMANTIC_ASSERT(pExp);
                        out_pResult = pExp;
                        return;
                    }
                }

                // try aggregate initialization
                {
                    for (auto pCtor : pClassType->getConstructors())
                    {
                        if (pCtor->testModifiers(Modifier::Defaulted) || pCtor->testModifiers(Modifier::Deleted) ||
                            !(pCtor->testFlags(PHANTOM_R_FLAG_IMPLICIT)) || pCtor->testModifiers(PHANTOM_R_EXPLICIT) ||
                            pCtor->testFlags(PHANTOM_R_FLAG_INHERITED))
                        {
                            // not a pure aggregate type, cannot aggregate-initialize

                            // last chance : construction with {} as if it was ()
                            Expressions expressions;
                            expressions.push_back(a_pInput);
                            LanguageElement* pElement = silentQualifiedLookup(
                            pClassType, pClassType->getName(), NullOpt, MakeArrayView(a_pInput->getExpressions()),
                            in_pContextScope, nullptr, 0);
                            if (pElement)
                            {
                                PHANTOM_SEMANTIC_ASSERT(pElement->asExpression() || pElement->isTemplateDependant());
                                Expression* pExp = static_cast<Expression*>(pElement)->loadRValueReferenceExpression();
                                PHANTOM_SEMANTIC_ASSERT(pExp);
                                out_pResult = pExp;
                                return;
                            }
                            return;
                        }
                    }
                }

                Expression* pExpression = nullptr;

                CxxSemanticErrorReturnIf(((refType != e_RefType_None)) && pClassType->isAbstract(),
                                         "'%s' : abstract class cannot be instantiated", FormatCStr(pClassType));
                OwnersGuard<Expressions> exprs;
                for (auto pCtor : pClassType->getConstructors())
                {
                    CxxSemanticErrorReturnIf(pCtor->testModifiers(Modifier::Deleted),
                                             "'%s' : illegal deleted constructor in aggregate initialization");
                    CxxSemanticErrorReturnIf(
                    pCtor->testModifiers(Modifier::Defaulted),
                    "'%s' : illegal explicitely defaulted constructor in aggregate initialization");
                    CxxSemanticErrorReturnIf(!(pCtor->testFlags(PHANTOM_R_FLAG_IMPLICIT)),
                                             "'%s' : illegal non implicit constructor in aggregate initialization",
                                             FormatCStr(pCtor));
                    CxxSemanticErrorReturnIf(pCtor->testModifiers(PHANTOM_R_EXPLICIT),
                                             "'%s' illegal explicit constructor in aggregate initialization",
                                             FormatCStr(pCtor));
                    CxxSemanticErrorReturnIf(pCtor->testFlags(PHANTOM_R_FLAG_INHERITED),
                                             "'%s' illegal inherited constructor in aggregate initialization",
                                             FormatCStr(pCtor));
                }
                for (auto pDM : pClassType->getFields())
                {
                    CxxSemanticErrorReturnIf(pDM->isPrivate() || pDM->isProtected(),
                                             "'%s' : illegal non public field in aggregate initialization",
                                             FormatCStr(pDM));
                }
                Class* pClass;
                if (pClass = pClassType->asClass())
                {
                    CxxSemanticErrorReturnIf(pClass->getVirtualMethodTables().size() != 0,
                                             "'%s' : illegal polymorphic class in aggregate initialization",
                                             FormatCStr(pClass));
                    for (auto baseClass : pClass->getBaseClasses())
                    {
                        CxxSemanticErrorReturnIf(baseClass.access != Access::Public,
                                                 "'%s' : illegal non public base class in aggregate initialization",
                                                 FormatCStr(baseClass.baseClass));
                    }
                }
                PHANTOM_SEMANTIC_ASSERT(pClassType->getDefaultConstructor(),
                                        "if we reach here, we are supposed to have a default constructor");
                if (initListExpressions.empty())
                {
                    Constructor* pCtor = pClassType->getDefaultConstructor();
                    // default construction
                    CxxSemanticErrorIfInaccessible(pCtor, in_pContextScope);
                    out_pResult = New<ConstructorCallExpression>(pClassType->getDefaultConstructor());
                }
                else
                {
                    size_t c = 0;
                    if (pClass)
                    {
                        for (Class* pBaseClass : pClass->getBaseClasses())
                        {
                            if (c < initListExpressions.size())
                            {
                                Expression* pExpression = initListExpressions[c++];
                                if (!(pExpression->getEvalType()->isSame(pBaseClass)))
                                {
                                    Expression* pConv = CxxSemanticConversion(pExpression, pBaseClass);
                                    if (pConv == nullptr)
                                        return;
                                    pExpression = pConv;
                                }
                                exprs.push_back(pExpression);
                            }
                            else
                            {
                                Constructor* pCtor = pBaseClass->getDefaultConstructor();
                                CxxSemanticErrorReturnIf(pCtor == nullptr, "'%s' : no default constructor available",
                                                         FormatCStr(pBaseClass));
                                CxxSemanticErrorIfInaccessible(pCtor, in_pContextScope);
                                exprs.push_back(New<ConstructorCallExpression>(pCtor));
                            }
                        }
                    }
                    for (auto pField : pClass->getFields())
                    {
                        Type* pFieldType = pField->getValueType();

                        if (c < initListExpressions.size())
                        {
                            Expression* pExpression = initListExpressions[c++];
                            if (!(pExpression->getEvalType()->isSame(pFieldType)))
                            {
                                Expression* pConv = CxxSemanticConversion(pExpression, pFieldType);
                                if (pConv == nullptr)
                                    return;
                                pExpression = pConv;
                            }
                            exprs.push_back(pExpression);
                        }
                        else
                        {
                            if (ClassType* pClassType = pFieldType->asClassType())
                            {
                                Constructor* pCtor = pClassType->getDefaultConstructor();
                                CxxSemanticErrorReturnIf(pCtor == nullptr, "'%s' : no default constructor available",
                                                         FormatCStr(pClassType));
                                CxxSemanticErrorIfInaccessible(pCtor, in_pContextScope);
                                exprs.push_back(New<ConstructorCallExpression>(pCtor));
                            }
                            else
                                exprs->push_back(nullptr);
                        }
                    }
                    out_pResult = pClass ? (Expression*)(New<ClassListInitializationExpression>(pClass, exprs))
                                         : New<ClassTypeListInitializationExpression>(pClassType, exprs);
                    if ((refType != e_RefType_None))
                    {
                        out_pResult = New<RValueToConstLValueExpression>(out_pResult);
                    }
                }
            }
        }
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}

void Semantic::visit(InitializerListType* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TypeConversion:
    {
        Type*                pOutput = *(Type* const*)a_Data.in[0];
        Types                types{nullptr, PHANTOM_TYPEOF(void), PHANTOM_TYPEOF(bool&)};
        CastKind             castKind = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions iUserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        LanguageElement*     pContextScope = *(LanguageElement* const*)a_Data.in[3];
        Conversion*&         conv = *(Conversion**)a_Data.out[0];
        switch (castKind)
        {
        case CastKind::Implicit:
        {
            bool addConstRef = pOutput->asConstLValueReference() != nullptr;
            pOutput = pOutput->removeConstReference();

            // if init list is empty '{}'
            if (a_pInput->getTypes().empty())
            {
                // and output is a class type
                if (ClassType* pClassType = pOutput->asClassType())
                {
                    // check for accessible default construction
                    if (Constructor* pDCtor = pClassType->getDefaultConstructor())
                    {
                        CxxSemanticErrorIfInaccessible(pDCtor, pContextScope);
                        conv = newConv<DefaultConversionSequence>(pOutput);
                        if (addConstRef)
                        {
                            if (conv->semantic)
                                conv = conv->clone(getSource());
                            static_cast<DefaultConversionSequence*>(conv)->addConstRef();
                        }
                        return;
                    }
                }
            }

            TemplateSpecialization* pTemplateSpecialization = pOutput->getTemplateSpecialization();
            if (pTemplateSpecialization &&
                pTemplateSpecialization->getTemplate()->getQualifiedName() == "std::initializer_list")
            {
                Template*                pTemplate = pTemplateSpecialization->getTemplate();
                SmallVector<Conversion*> viableImplicitConversions;
                Types                    viableTypes;
                Type*                    pInitializerType = static_cast<Type*>(pTemplateSpecialization->getArgument(0));
                SmallSet<TypeIndex, 4>   candidateTypes;
                for (auto pType : a_pInput->getTypes())
                {
                    candidateTypes.insert(pType);
                }
                for (auto pType : candidateTypes)
                {
                    Conversion* conv = newConversion(pType, pInitializerType, CastKind::Implicit,
                                                     UserDefinedFunctions::ImplicitsOnly, pContextScope);
#pragma message(PHANTOM_TODO "leaking here on newConversion (need whole conversion refactor without allocations)")
                    if (conv)
                    {
                        viableImplicitConversions.push_back(conv);
                        viableTypes.push_back(pType);
                    }
                }

                if (viableImplicitConversions.size())
                {
                    /// Find worst viable call or display ambiguity error
                    Conversion* pWorst = nullptr;
                    Type*       pWorstType = nullptr;
                    if (viableImplicitConversions.size() == 1)
                    {
                        pWorst = viableImplicitConversions.back();
                        pWorstType = viableTypes.back();
                    }
                    else
                    {
                        size_t bestIndex = 0;
                        for (size_t i = 0; i < viableImplicitConversions.size(); ++i)
                        {
                            int    worstResult = 1;
                            size_t j = 0;
                            for (; j < viableImplicitConversions.size(); ++j)
                            {
                                if (i == j)
                                    continue;
                                int result = viableImplicitConversions[i]->compare(*viableImplicitConversions[j]);
                                PHANTOM_SEMANTIC_ASSERT(
                                viableImplicitConversions[j]->compare(*viableImplicitConversions[i]) ==
                                -result); /// compare() coherence test
                                if (result <= 0)
                                    worstResult = std::min(worstResult, result);
                                else
                                    break;
                            }
                            if (j ==
                                viableImplicitConversions.size()) /// superior or equal to every one (not worst to any)
                            {
                                if (worstResult == -1)
                                {
                                    /// found a best match (superior to at least one other signature
                                    /// and equal to the others)
                                    pWorst = viableImplicitConversions[i];
                                    pWorstType = viableTypes[i];
                                    goto end_of_selection;
                                }
                            }
                        }
                    }
                end_of_selection:
                    viableImplicitConversions.clear();
                    viableTypes.clear();
                    if (pWorst)
                    {
                        TemplateSignature* pTemplateSignature = pTemplate->getTemplateSignature();
                        pWorst->input = a_pInput; // we don't really care about the exact content of 'input' as long
                                                  // it is an InitializerListType (this just serves as a marker for
                                                  // the conversion to Expression process)
                        pWorst->output = pOutput;
                        conv = pWorst;
                        if (addConstRef)
                        {
                            if (conv->semantic)
                                conv = conv->clone(getSource());
                            static_cast<DefaultConversionSequence*>(conv)->addConstRef();
                        }
                        return; /// success {...} -> std::initializer_list<X>
                    }
                }
            }
            else
            {
                // list initialization
                ClassType*   pClassType;
                Array*       pArray = pOutput->asArray();
                Types const& types = a_pInput->getTypes();
                if (pArray)
                {
                    if (types.empty())
                    {
                        conv = nullptr;
                        return;
                    }
                    if (pArray->getItemCount()) // we know the array size we want to convert to
                    {
                        if (pArray->getItemCount() != types.size()) // sizes do not match
                        {
                            conv = nullptr; // no conversion available
                            return;
                        }
                    }
                    else
                    {
                        pArray = pArray->getItemType()->makeArray(
                        types.size()); // define array size to the initializer list size
                    }
                    for (size_t i = 0; i < types.size(); ++i)
                    {
                        Conversion* cv = newConversion(types[i], pArray->getItemType(), CastKind::Implicit,
                                                       UserDefinedFunctions::ImplicitsOnly, pContextScope);
                        if (cv == nullptr)
                        {
                            conv = nullptr;
                            return;
                        }
                        PHANTOM_SEMANTIC_ASSERT(cv->semantic);
                    }
                    conv = newConv<DefaultConversionSequence>(a_pInput, pArray);
                }
                else if (pClassType = pOutput->asClassType())
                {
                    bool aggregate = true;
                    for (auto pCtor : pClassType->getConstructors())
                    {
                        if (pCtor->testModifiers(Modifier::Defaulted) || pCtor->testModifiers(Modifier::Deleted) ||
                            !(pCtor->testFlags(PHANTOM_R_FLAG_IMPLICIT)) || pCtor->testModifiers(PHANTOM_R_EXPLICIT) ||
                            pCtor->testFlags(PHANTOM_R_FLAG_INHERITED))
                        {
                            if (aggregate && a_pInput->getTypes().size() == 1)
                            {
                                conv = newConversion(a_pInput->getTypes().front(), pClassType, pContextScope);
                                // visit(static_cast<Type*>(a_pInput), a_Data);
                                if (conv)
                                    return;
                            }
                            aggregate = false;
                            // not a pure aggregate type, cannot value/list-initialize
                            ConversionResults argConvs;
                            newImplicitConversions(pCtor->getSignature(), a_pInput->getTypes(), argConvs, pContextScope,
                                                   UserDefinedFunctions::ImplicitsOnly);
                            if (argConvs.size() && !argConvs.hasNull())
                            {
                                argConvs.destroy(getSource());
                                CxxSemanticErrorIfInaccessible(pCtor, pContextScope);
                                auto dcs = newConv<DefaultConversionSequence>(a_pInput, pClassType);
                                conv = dcs;
                                dcs->m_user_defined = newConv<UserDefinedConversion>(pCtor);
                                return;
                            }
                        }
                    }
                    if (!aggregate)
                    {
                        conv = nullptr;
                        return;
                    }

                    if (types.empty())
                    {
                        if (pClassType->getDefaultConstructor() == nullptr)
                        // default construction
                        {
                            conv = nullptr;
                            return;
                        }
                    }
                    else
                    {
                        size_t c = 0;
                        if (Class* pClass = pOutput->asClass())
                        {
                            for (BaseClass const& bc : pClass->getBaseClasses())
                            {
                                Class* pBaseClass = bc.baseClass;
                                if (bc.access != Access::Public)
                                {
                                    conv = nullptr;
                                    return;
                                }
                            }

                            for (BaseClass const& bc : pClass->getBaseClasses())
                            {
                                Class* pBaseClass = bc.baseClass;
                                if (c < types.size())
                                {
                                    Type*       pType = types[c++];
                                    Conversion* cv = newConversion(pType, pBaseClass, CastKind::Implicit,
                                                                   UserDefinedFunctions::ImplicitsOnly, pContextScope);
                                    if (cv == nullptr)
                                    {
                                        conv = nullptr;
                                        return;
                                    }
                                    PHANTOM_SEMANTIC_ASSERT(cv->semantic);
                                }
                                else
                                {
                                    if (pBaseClass->getDefaultConstructor() == nullptr)
                                    {
                                        conv = nullptr;
                                        return;
                                    }
                                }
                            }
                        }
                        if (pClassType = pOutput->asClassType())
                        {
                            for (auto pField : pClassType->getFields())
                            {
                                if (!pField->isPublic())
                                {
                                    conv = nullptr;
                                    return;
                                }
                            }
                            for (auto pField : pClassType->getFields())
                            {
                                Type* pFieldType = pField->getValueType();

                                if (c < types.size())
                                {
                                    Type*       pType = types[c++];
                                    Conversion* cv = newConversion(pType, pFieldType, CastKind::Implicit,
                                                                   UserDefinedFunctions::ImplicitsOnly, pContextScope);
                                    if (cv == nullptr)
                                    {
                                        conv = nullptr;
                                        return;
                                    }
                                    PHANTOM_SEMANTIC_ASSERT(cv->semantic);
                                }
                                else
                                {
                                    ClassType* pClassType;
                                    if (pClassType = pFieldType->asClassType())
                                    {
                                        if (pClassType->getDefaultConstructor() == nullptr)
                                        {
                                            conv = nullptr;
                                            return;
                                        }
                                    }
                                }
                            }
                        }
                        if (c < types.size())
                        {
                            // too many initializers compared to base-class + field count
                            conv = nullptr;
                            return;
                        }
                        conv = newConv<DefaultConversionSequence>(ConversionVal(a_pInput), ConversionVal(pOutput));
                    }
                }
            }
        }
        break;
        } // switch
    }
    break;
    } // switch
}

void Semantic::visit(Label* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (pInstanciated == nullptr)
            pInstanciated = New<Label>(a_pInput->getLabelName(), a_pInput->getTag());
    }
        return;
    }
    visit(static_cast<Statement*>(a_pInput), a_Data);
}

void Semantic::visit(LanguageElement* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_HasSideEffect: // ensure every other element than expression is silent
                                          // relative to side effects
    {
        *(bool*)a_Data.out[0] = false;
    }
        return;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        PHANTOM_SEMANTIC_ASSERT(false, "missing instanciate template case for the given element type");
    }
    break;
    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*  pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        if (pArgument->isSame(a_pInput))
        {
            pDeducedArgument = pArgument;
        }
    }
        return;
    case e_VisitorFunction_UnqualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        LanguageElement*                    in_pContextScope = *(LanguageElement**)a_Data.in[3];
        Type*                               in_pInitializationType = (Type*)a_Data.in[4];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];
        out_pResult = silentQualifiedLookup(a_pInput, in_name, in_pTemplateArguments, in_pFunctionArguments,
                                            in_pContextScope, in_pInitializationType, a_Data.flags);
        if (out_pResult)
            return;
        if (isStandardLookup())
        {
            Symbol* pSymbol = a_pInput->asSymbol();
            if (pSymbol && pSymbol->getNamespace())
            {
                pSymbol->getNamespace()->visit(this, a_Data);
                return;
            }
        }
        if (a_pInput->getOwner())
        {
            a_pInput->getOwner()->visit(this, a_Data);
        }
    }
        return;
    case e_VisitorFunction_UnqualifiedNameLookup:
    {
        StringView       in_name = *(StringView*)a_Data.in[0];
        LanguageElement* in_pContextScope = *(LanguageElement**)a_Data.in[1];
        Access*          in_Access = (Access*)a_Data.in[2];

        Symbols*  out_pSymbols = (Symbols*)a_Data.out[0];
        Accesses* out_Accesses = (Accesses*)a_Data.out[1];
        internalQualifiedSymbolsLookup(a_pInput, in_name, *out_pSymbols, *out_Accesses, in_pContextScope, *in_Access,
                                       a_Data.flags);
        if (out_pSymbols->size())
            return;
        if (isStandardLookup())
        {
            Symbol* pSymbol = a_pInput->asSymbol();
            if (pSymbol && pSymbol->getNamespace())
            {
                pSymbol->getNamespace()->visit(this, a_Data);
                return;
            }
        }
        if (a_pInput->getOwner())
        {
            a_pInput->getOwner()->visit(this, a_Data);
        }
    }
        return;

    case e_VisitorFunction_QualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        LanguageElement*                    in_pContextScope = *(LanguageElement**)a_Data.in[3];
        Type*                               in_pInitializationType = (Type*)a_Data.in[4];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];

        /// Find all candidates
        Symbols  candidates;
        Accesses outAccesses;

        internalQualifiedSymbolsLookup(a_pInput, in_name, candidates, outAccesses, in_pContextScope, Access::Public,
                                       a_Data.flags);

        if (candidates.size())
        {
            bool bIsTemplateDependant = a_pInput->isTemplateElement();

            /// If this is a call
            if (in_pFunctionArguments)
            {
                // remove aliases in every case (even if told not to) if we have a function argument list
                for (auto& pCandidate : candidates)
                {
                    while (Alias* pAlias = pCandidate->asAlias())
                    {
                        if (Symbol* Aliased = pAlias->getAliasedSymbol())
                        {
                            pCandidate = Aliased;
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                std::sort(candidates.begin(), candidates.end());
                candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());

                for (auto it = in_pFunctionArguments->begin(); it != in_pFunctionArguments->end(); ++it)
                {
                    bIsTemplateDependant = bIsTemplateDependant || (*it)->isTemplateDependant();
                }
                Class*         pBaseClass = nullptr;
                Field*         pField = nullptr;
                LocalVariable* pLocalVariable = nullptr;
                Variable*      pVariable = nullptr;
                bool           ctorInitializer = false;
                ClassType*     pContextClassType = nullptr;

                // single candidate is a variable (global or static)
                if (candidates.size() == 1 && (pVariable = candidates.front()->asVariable()))
                {
                    OwnerGuard<Expression> pVarExp = New<VariableExpression>(pVariable);
                    out_pResult =
                    silentQualifiedLookup(pVarExp, "operator()", in_pTemplateArguments, in_pFunctionArguments,
                                          in_pContextScope, in_pInitializationType);
                    return;
                }

                // context is local variable initialization
                if (in_pContextScope && in_pContextScope->asLocalVariable())
                {
                    ctorInitializer = true;
                    pContextClassType = in_pContextScope->asLocalVariable()->getSubroutine()->getOwner()->asClassType();
                    /// constructor initializers
                    size_t count = candidates.size();
                    if (count > 1)
                    {
                        CxxSemanticError("'%s' : ambiguous access to multiple members, candidates are :",
                                         (const char*)in_name.data());
                        for (auto c : candidates)
                        {
                            CxxSemanticSubError("%s", FormatCStr(c));
                        }
                        out_pResult = nullptr;
                        return;
                    }
                    LanguageElement* pCandidate = candidates.back();
                    for (size_t i = 0; i < count; ++i)
                    {
                        pField = pCandidate->asField();
                        pBaseClass = pCandidate->asClass();
                        pLocalVariable = pCandidate->asLocalVariable();
                        if (pField && pField->getOwner() == pContextClassType)
                        {
                            candidates[i] = nullptr;
                            ClassType* pType = pField->getValueType()->removeQualifiers()->asClassType();
                            if (pType)
                            {
                                if (pType->isTemplateDependant())
                                {
                                    out_pResult = New<FieldInitializationStatement>(
                                    pField,
                                    New<TemplateDependantExpression>(New<TemplateDependantElement>(
                                    pType, "", NullOpt,
                                    *(OptionalArrayView<LanguageElement*>*)&in_pFunctionArguments)));
                                    return;
                                }
                                /// replace data member by class type constructors
                                for (auto p : pType->getConstructors())
                                {
                                    if (m_pSource)
                                    {
                                        CxxSemanticErrorIfInaccessible(p, pContextClassType,
                                                                       (void)(out_pResult = nullptr))
                                    }
                                    if (candidates[i] == nullptr)
                                        candidates[i] = p;
                                    else
                                        candidates.push_back(p);
                                }
                            }
                        }
                        else if (pBaseClass && pContextClassType->isA(pBaseClass))
                        {
                            if (pBaseClass->isTemplateDependant())
                            {
                                out_pResult = New<BaseConstructorCallStatement>(
                                pContextClassType->asClass(),
                                New<TemplateDependantExpression>(New<TemplateDependantElement>(
                                pBaseClass, "", NullOpt,
                                *(OptionalArrayView<LanguageElement*>*)&in_pFunctionArguments)));
                                return;
                            }
                            candidates[i] = nullptr;
                            /// replace data member by class type constructors
                            for (auto p : pBaseClass->getConstructors())
                            {
                                if (m_pSource)
                                {
                                    CxxSemanticErrorIfInaccessible(p, pContextClassType, void(out_pResult = nullptr))
                                }
                                if (candidates[i] == nullptr)
                                    candidates[i] = p;
                                else
                                    candidates.push_back(p);
                            }
                        }
                        else if (pLocalVariable && pLocalVariable == in_pContextScope) /// Local variable initialization
                        {
                            if (pLocalVariable->getValueType()->isTemplateDependant())
                            {
                                out_pResult = New<LocalVariableInitializationStatement>(
                                pLocalVariable,
                                New<TemplateDependantExpression>(New<TemplateDependantElement>(
                                pLocalVariable->getValueType()->removeReference()->removeConst(), "", NullOpt,
                                *(OptionalArrayView<LanguageElement*>*)&in_pFunctionArguments)));
                                return;
                            }
                            candidates[i] = nullptr;
                            ClassType* pType = pLocalVariable->getValueType()->removeQualifiers()->asClassType();
                            if (pType)
                            {
                                /// replace data member by class type constructors
                                for (auto p : pType->getConstructors())
                                {
                                    if (m_pSource)
                                    {
                                        CxxSemanticErrorIfInaccessible(p, pContextClassType,
                                                                       void(out_pResult = nullptr))
                                    }
                                    if (candidates[i] == nullptr)
                                        candidates[i] = p;
                                    else
                                        candidates.push_back(p);
                                }
                            }
                            else
                                return;
                        }
                    }

                    /// No argument => default construction ?
                    if (in_pFunctionArguments->size() == 0)
                    {
                        if (pField)
                        {
                            if (ClassType* pClass = pField->getValueType()->removeQualifiers()->asClassType())
                            {
                                if (Constructor* pDCtor = pClass->getDefaultConstructor())
                                {
                                    if (pDCtor->testModifiers(Modifier::Deleted))
                                    {
                                        CxxSemanticError("attempt to access deleted default constructor");
                                        return;
                                    }
                                    CxxSemanticErrorIfInaccessible(pDCtor, in_pContextScope);
                                    out_pResult =
                                    New<FieldInitializationStatement>(pField, New<ConstructorCallExpression>(pDCtor));
                                    return;
                                }
                                CxxSemanticError("no default constructor");
                                return;
                            }
                            if (pField->getValueType()->asReference())
                            {
                                CxxSemanticError("reference must be initialized");
                                return;
                            }
                            out_pResult =
                            New<FieldInitializationStatement>(pField, createZeroInitExpression(pField->getValueType()));
                            return;
                        }
                        else if (pLocalVariable)
                        {
                            if (ClassType* pClass = pLocalVariable->getValueType()->removeQualifiers()->asClassType())
                            {
                                if (Constructor* pDCtor = pClass->getDefaultConstructor())
                                {
                                    if (pDCtor->testModifiers(Modifier::Deleted))
                                    {
                                        CxxSemanticError("attempt to access deleted default constructor");
                                        return;
                                    }
                                    CxxSemanticErrorIfInaccessible(pDCtor, in_pContextScope);
                                    out_pResult = New<LocalVariableInitializationStatement>(
                                    pLocalVariable, New<ConstructorCallExpression>(pDCtor));
                                    return;
                                }
                                CxxSemanticError("no default constructor");
                            }
                            if (pLocalVariable->getValueType()->asReference())
                            {
                                CxxSemanticError("reference must be initialized");
                                return;
                            }
                            out_pResult = New<LocalVariableInitializationStatement>(
                            pLocalVariable, createZeroInitExpression(pLocalVariable->getValueType()));
                            return;
                        }
                    }
                    /// Only one argument => conversion ?
                    else if (in_pFunctionArguments->size() == 1)
                    {
                        if (pField)
                        {
                            /// use explicit conversions way instead of call candidates way
                            Expression* pConversion =
                            convert(in_pFunctionArguments->back(), pField->getValueType(), CastKind::Explicit,
                                    UserDefinedFunctions::All, in_pContextScope);
                            if (pConversion)
                            {
                                out_pResult = New<FieldInitializationStatement>(pField, pConversion);
                            }
                            return;
                        }
                        else if (pLocalVariable)
                        {
                            /// use explicit conversions way instead of call candidates way
                            Expression* pConversion =
                            convert(in_pFunctionArguments->back(), pLocalVariable->getValueType(), CastKind::Explicit,
                                    UserDefinedFunctions::All, in_pContextScope);
                            if (pConversion)
                            {
                                out_pResult = New<LocalVariableInitializationStatement>(pLocalVariable, pConversion);
                            }
                            return;
                        }
                    }
                }
                else
                {
                    size_t count = candidates.size();
                    for (size_t i = 0; i < count; ++i)
                    {
                        Type*      pType = candidates[i]->asType();
                        ClassType* pClassType = nullptr;
                        if (pType == nullptr)
                        {
                            Template* pTemplate = candidates[i]->asTemplate();
                            if (pTemplate)
                            {
                                if (auto pEmptyspec = pTemplate->getEmptyTemplateSpecialization())
                                    if (auto pTemplated = pEmptyspec->getTemplated())
                                        if (pTemplated->asSubroutine()) // if subroutine skip template resolution (will
                                                                        // be done in 'selectCallCandidates')
                                            break;
                                /// constructor initializers
                                if (candidates.size() > 1)
                                {
                                    CxxSemanticError("'%s' : ambiguous access to multiple "
                                                     "constructors, overloads are :",
                                                     (const char*)in_name.data());
                                    for (auto c : candidates)
                                    {
                                        CxxSemanticSubError("'%s'", FormatCStr(c));
                                    }
                                    out_pResult = nullptr;
                                    return;
                                }
                                if (in_pTemplateArguments)
                                {
                                    if (pTemplate->getTemplateParameters().size() < in_pTemplateArguments->size())
                                    {
                                        /// TODO : handle parameter pack
                                        auto   errArgs = in_pTemplateArguments->data();
                                        size_t count = in_pTemplateArguments->size();
                                        CxxSemanticError("'%s' : too many template arguments, %d given, %d expected",
                                                         FormatCStr(pTemplate), count,
                                                         pTemplate->getTemplateParameters().size());
                                        out_pResult = nullptr;
                                        return;
                                    }
                                    else
                                    {
                                        LanguageElement* pResolvedTemplate =
                                        instantiateTemplate(pTemplate, *in_pTemplateArguments, in_pContextScope);
                                        if ((pType = (pResolvedTemplate) ? pResolvedTemplate->toType() : nullptr))
                                        {
                                            if (!pType->isNative())
                                            {
                                                if ((pClassType = pType->asClassType()))
                                                {
                                                    // ensure members are all present so that we can resolve
                                                    // template class constructor overloads
                                                    buildClass(pClassType, e_ClassBuildState_Members);
                                                }
                                            }
                                        }

                                        // reset template arguments so that they are not used later in overload
                                        // resolution
                                        in_pTemplateArguments = NullOpt;
                                    }
                                }
                                else
                                {
                                    CxxSemanticError("'%s' : expects template arguments", FormatCStr(pTemplate));
                                    out_pResult = nullptr;
                                    return;
                                }
                            }
                        }
                        if (pType)
                        {
                            /// constructor initializers
                            if (candidates.size() > 1)
                            {
                                CxxSemanticError("'%s' : ambiguous access to multiple members, candidates are :",
                                                 (const char*)in_name.data());
                                for (auto c : candidates)
                                {
                                    CxxSemanticSubError("'%s'", FormatCStr(c));
                                }
                                out_pResult = nullptr;
                                return;
                            }
                            if (pType->isTemplateDependant())
                            {
                                out_pResult = New<TemplateDependantExpression>(New<TemplateDependantElement>(
                                pType, "", NullOpt, *(OptionalArrayView<LanguageElement*>*)&in_pFunctionArguments));
                                return;
                            }
                            /// Only one argument => conversion ?
                            if (in_pFunctionArguments->size() == 1)
                            {
                                /// use explicit conversions way instead of call candidates way
                                Expression* pConversion =
                                internalConvert(in_pFunctionArguments->back(), pType, CastKind::Explicit,
                                                UserDefinedFunctions::All, in_pContextScope, true);
                                if (pConversion)
                                {
                                    // PHANTOM_DEFERRED_DYN_DELETE in_pFunctionArguments->back(); //
                                    // we cloned the argument so we can delete it
                                    out_pResult = pConversion;
                                    return;
                                }
                            }
                            if (!pClassType)
                                pClassType = pType->asClassType();

                            if (pClassType)
                            {
                                candidates[i] = nullptr;
                                /// replace data member by class type constructors
                                if (pClassType->getConstructors().empty())
                                {
                                    CxxSemanticError("'%s' : no constructor available", FormatCStr(pClassType));
                                    out_pResult = nullptr;
                                    return;
                                }
                                for (auto p : pClassType->getConstructors())
                                {
                                    if (candidates[i] == nullptr)
                                        candidates[i] = p;
                                    else
                                        candidates.push_back(p);
                                }
                                PHANTOM_SEMANTIC_ASSERT(candidates[i]);
                            }
                        }
                        else
                        {
                        }
                    }
                }
                if (bIsTemplateDependant)
                {
                    out_pResult = New<TemplateDependantExpression>(New<TemplateDependantElement>(
                    a_Data.hasFlag(e_VisitorFlag_Unqualified) ? nullptr : a_pInput, in_name, in_pTemplateArguments,
                    *(OptionalArrayView<LanguageElement*>*)&in_pFunctionArguments));
                    return;
                }
                Expressions arguments;
                arguments.insert(arguments.end(), in_pFunctionArguments->begin(),
                                 in_pFunctionArguments->end()); // then arguments
                Subroutines viableCallCandidates;
                selectCallCandidate(viableCallCandidates, candidates, in_pTemplateArguments, arguments, out_pResult,
                                    in_name, in_pContextScope, in_pInitializationType, PHANTOM_R_STATIC, 0);
                if (!out_pResult)
                {
                    _selectCallCandidateError(in_name, candidates, viableCallCandidates, arguments);
                }
                else if (ctorInitializer)
                {
                    if (pBaseClass || pField || pLocalVariable)
                    {
                        ConstructorCallExpression* pCallExpression =
                        out_pResult->asExpression()->loadRValueReferenceExpression()->as<ConstructorCallExpression>();
                        pCallExpression->setTemporary(false); /// if expression was considered temporary, it's not
                                                              /// anymore due to its 'initialization' purpose
                                                              /// (destruction action is transfered to the statement)
                        PHANTOM_SEMANTIC_ASSERT(pCallExpression);
                        if (pField)
                        {
                            out_pResult = New<FieldInitializationStatement>(pField, pCallExpression);
                        }
                        else if (pBaseClass)
                        {
                            out_pResult =
                            New<BaseConstructorCallStatement>(pContextClassType->asClass(), pCallExpression);
                        }
                        else
                        {
                            PHANTOM_SEMANTIC_ASSERT(pLocalVariable);
                            out_pResult = New<LocalVariableInitializationStatement>(pLocalVariable, pCallExpression);
                        }
                    }
                    else
                    {
                        Delete(out_pResult);
                        out_pResult = nullptr;
                    }
                }
            }
            else
            {
                // remove any alias if not explicitely told to keep them
                bool unaliased = false;
                if ((a_Data.flags & e_VisitorFlag_KeepAliases) == 0)
                {
                    for (auto& pCandidate : candidates)
                    {
                        while (Alias* pAlias = pCandidate->asAlias())
                        {
                            if (Symbol* pAliased = pAlias->getAliasedSymbol())
                            {
                                pCandidate = pAliased;
                                unaliased = true;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    // re-ensure unicity (two aliases may indirect to same symbol)
                    std::sort(candidates.begin(), candidates.end());
                    candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());
                }

                if (unaliased && candidates.size() == 1 && candidates.back()->asSource())
                {
                    // when encountering an aliased source, we continue search with same name inside it
                    candidates.back()->visit(this, a_Data);
                    if (out_pResult)
                        return;
                }

                if (bIsTemplateDependant && candidates.size() > 1) /// template element but not ctor initializer
                {
                    out_pResult =
                    New<TemplateDependantElement>(a_Data.hasFlag(e_VisitorFlag_Unqualified) ? nullptr : a_pInput,
                                                  in_name, in_pTemplateArguments, NullOpt);
                    return;
                }
                /// if no function signature && multiple candidates => ambiguous
                if (candidates.size() == 1)
                {
                    Symbol* pWinner = static_cast<Symbol*>(candidates[0]);
                    if (pWinner->asConstructor()) /// avoid getting unique constructor while looking
                                                  /// for current class type
                        return;

                    if (in_pTemplateArguments)
                    {
                        if (pWinner->asTemplate())
                        {
                            Template* pTemplate = static_cast<Template*>(pWinner);
                            if (!(pTemplate->isVariadic()) &&
                                pTemplate->getTemplateParameters().size() < in_pTemplateArguments->size())
                            {
                                auto   errArgs = in_pTemplateArguments->data();
                                size_t count = in_pTemplateArguments->size();
                                CxxSemanticError("'%s' : too many template arguments, %d given, %d expected",
                                                 FormatCStr(pTemplate), count,
                                                 pTemplate->getTemplateParameters().size());
                                out_pResult = nullptr;
                                return;
                            }
                            else
                            {
                                out_pResult = instantiateTemplate(pTemplate, *in_pTemplateArguments, in_pContextScope);
                            }
                        }
                    }
                    else
                    {
                        if (pWinner->asPlaceholder())
                        {
                            out_pResult = pWinner;
                            return;
                        }
                        if (bIsTemplateDependant)
                        {
                            if (pWinner->isTemplateDependant())
                                out_pResult = pWinner;
                            else
                                out_pResult = New<TemplateDependantElement>(
                                a_Data.hasFlag(e_VisitorFlag_Unqualified) ? nullptr : a_pInput, in_name,
                                in_pTemplateArguments, NullOpt);
                            return;
                        }
                        Expression* pExpression = toExpression(pWinner);
                        if (pExpression)
                        {
                            out_pResult = pExpression;
                        }
                        else
                        {
                            out_pResult = pWinner;
                        }
                    }
                }
                else
                {
                    auto   errArgs = candidates.data();
                    size_t count = candidates.size();
                    CxxSemanticError("'%s' : ambiguous access to multiple members, candidates are :",
                                     (const char*)in_name.data());
                    for (auto c : candidates)
                    {
                        CxxSemanticSubError("'%s'", FormatCStr(c));
                    }
                    out_pResult = nullptr;
                }
            }
        }
    }
    break;

    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView       in_name = *(StringView*)a_Data.in[0];
        LanguageElement* in_pScope = *(LanguageElement**)a_Data.in[1];
        Access           in_context_access = *(Access*)a_Data.in[2];
        Symbols&         out_candidates = *(Symbols*)a_Data.out[0];
        Accesses&        out_context_accesses = *(Accesses*)a_Data.out[1];

        for (auto& p : a_pInput->getElements())
        {
            Symbol* pSymbol = (p)->asSymbol();
            if (!pSymbol)
                continue;

            auto vis = pSymbol->getVisibility();
            if (vis == Visibility::Private ||
                (vis == Visibility::Protected && in_pScope != a_pInput && !in_pScope->hasOwnerCascade(a_pInput)))
                continue;

            if (pSymbol->getName() == in_name)
            {
                if (TemplateSpecialization* pSpec = pSymbol->asTemplateSpecialization())
                {
                    if (!(pSpec->testFlags(PHANTOM_R_FLAG_IMPLICIT))) // not an instantiation but a specialization
                    {
                        Template* pTemplate = static_cast<TemplateSpecialization*>(pSymbol)->getTemplate();
                        if (std::find(out_candidates.begin(), out_candidates.end(), pTemplate) == out_candidates.end())
                        {
                            PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((pTemplate));
                            out_candidates.push_back(pTemplate);
                        }
                    }
                }
                else
                {
                    PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((pSymbol));
                    out_candidates.push_back(pSymbol);
                }
            }
            else if (Enum* pEnum = pSymbol->asEnum())
            {
                if (!pEnum->isScoped()) // not "enum class"
                {
                    pSymbol->visit(this, a_Data);
                }
            }
        }
    }
    break;

    case e_VisitorFunction_Access:
    {
        Symbol* in_pSymbol = *(Symbol**)a_Data.in[0];
        bool&   out_bResult = *(bool*)a_Data.out[0];
        out_bResult = in_pSymbol->isPublic() || in_pSymbol->isSame(a_pInput) || a_pInput->hasElement(in_pSymbol) ||
        (a_pInput->getOwner() && isSymbolAccessible(in_pSymbol, a_pInput->getOwner()));
    }
    break;
    }
}

void Semantic::visit(LoadExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        // PHANTOM_SEMANTIC_ASSERT(pInstanciated == nullptr);
        Expression* pExpression = o_instantiateT(Expression, a_pInput->getLoadedExpression());
        if (pExpression == nullptr)
            return;
        pInstanciated = New<LoadExpression>(pExpression);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}

void Semantic::visit(LocalVariable* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_QualifiedLookup:
    {
        LanguageElement*&      pResult = *(LanguageElement**)a_Data.out[0];
        OwnerGuard<Expression> pExpression = New<LocalVariableExpression>(a_pInput);
        pExpression->visit(this, a_Data);
    }
        return;
    case e_VisitorFunction_ToExpression:
    {
        *(LanguageElement**)a_Data.out[0] = New<LocalVariableExpression>(a_pInput);
    }
        return;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (pInstanciated == nullptr)
        {
            Type* pType = o_resolveT(Type, a_pInput->getValueType());
            if (pType == nullptr)
                return;
            pInstanciated = createLocalVariable(pType, a_pInput->getName(), a_pInput->getModifiers());
        }
        return;
    }
        //     case e_VisitorFunction_ResolveTemplateDependencies:
        //     {
        //         TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        //         EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        //         LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        //         LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        //         PHANTOM_SEMANTIC_ASSERT(in_pContextScope->asBlock());
        //         Block* pBlock = static_cast<Block*>(in_pContextScope);
        //         pResolved =  pBlock->getLocalVariableCascade(a_pInput->getName());
        //         return;
        //     }
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(LocalVariableExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (a_pInput->getLocalVariable())
        {
            LocalVariable* pLocVar = o_resolveT(LocalVariable, a_pInput->getLocalVariable());
            if (pLocVar)
            {
                pInstanciated = New<LocalVariableExpression>(pLocVar);
                PHANTOM_SEMANTIC_ASSERT(((LocalVariableExpression*)pInstanciated)->getLocalVariable() !=
                                        a_pInput->getLocalVariable());
            }
        }
        else
        {
            Type* pType = o_resolveT(Type, a_pInput->getValueType());
            if (pType == nullptr)
            {
                // CxxSemanticErrorReturnIf(pType == nullptr, e_Error_TemplateResolutionFailure,
                // a_pInput);
                return;
            }
            PHANTOM_SEMANTIC_ASSERT(!(pType->isTemplateDependant()));
            pInstanciated = New<LocalVariableExpression>(pType->removeReference()->removeConst()->removePointer());
        }
        return;
    }
    }
    visit(static_cast<LValueExpression*>(a_pInput), a_Data);
}
void Semantic::visit(LocalVariableInitializationStatement* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (TemplateDependantExpression* pTDE =
            a_pInput->getInitializationExpression()->as<TemplateDependantExpression>())
        {
            if (pTDE->getTemplateDependantElement()->getFunctionArguments() &&
                pTDE->getTemplateDependantElement()->getName().empty())
            {
                const LanguageElements& unresolvedArguments =
                *pTDE->getTemplateDependantElement()->getFunctionArguments();
                OwnersGuard<Expressions> newArguments;
                for (auto it = unresolvedArguments.begin(); it != unresolvedArguments.end(); ++it)
                {
                    OwnerGuard<LanguageElement> pElement = o_instantiateT(LanguageElement, *it);
                    if (pElement)
                    {
                        Expression* pExpression = pElement->asExpression();
                        if (pExpression == nullptr)
                        {
                            CxxSemanticError("'%s' : template dependant element is not an expression",
                                             FormatCStr(pElement.element()));
                            return;
                        }
                        else
                            newArguments.push_back(pExpression);
                    }
                    else
                    {
                        return;
                    }
                }
                pInstanciated = unqualifiedLookup(
                a_pInput->getLocalVariable()->getName(), phantom::NullOpt, MakeArrayView(newArguments.elements()),
                in_pContextScope->asBlock()->getLocalVariable(a_pInput->getLocalVariable()->getName()), nullptr, 0);
            }
            else
            {
                pTDE->visit(this, a_Data);
                if (pInstanciated)
                {
                    LocalVariable* pLocal =
                    in_pContextScope->asBlock()->getLocalVariable(a_pInput->getLocalVariable()->getName());
                    PHANTOM_SEMANTIC_ASSERT(pLocal);
                    Expression* pConv = CxxSemanticConversionNE(pInstanciated->asExpression(), pLocal->getValueType());
                    if (pConv == nullptr)
                        return;
                    pInstanciated = New<LocalVariableInitializationStatement>(pLocal, pConv);
                }
            }
        }
        else
        {
            if (LocalVariable* pLocal =
                static_cast<Block*>(in_pContextScope)->getLocalVariable(a_pInput->getLocalVariable()->getName()))
            {
                OwnerGuard<Expression> pExp = o_instantiateT(Expression, a_pInput->getInitializationExpression());
                if (pExp == nullptr)
                    return;
                Expression* pConv = CxxSemanticConversionNE(pExp, pLocal->getValueType());
                if (pConv == nullptr)
                    return;
                pInstanciated = New<LocalVariableInitializationStatement>(pLocal, pConv);
            }
        }
    }
        return;
    }
    visit(static_cast<Statement*>(a_pInput), a_Data);
}
void Semantic::visit(LValueExpression* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(LValueInitializationStatement* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Statement*>(a_pInput), a_Data);
}

void Semantic::visit(LValueReference* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*  pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        LValueReference*  pReference = pArgument->asLValueReference();
        if (pReference)
        {
            Type* pReferencedDeducedType =
            templateArgumentDeduction(a_pInput->getReferencedType(), pReference->getReferencedType(), deductions);
            if (pReferencedDeducedType)
            {
                pDeducedArgument = pReferencedDeducedType->makeLValueReference();
            }
        }
    }
    break;

    case e_VisitorFunction_TypeConversion:
    {
        Type*                pOutput = *(Type* const*)a_Data.in[0];
        CastKind             castKind = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions iUserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        LanguageElement*     pContextScope = *(LanguageElement* const*)a_Data.in[3];
        bool                 in_bInitialize = *(bool*)a_Data.in[4];
        Conversion*&         conv = *(Conversion**)a_Data.out[0];

        if (pOutput->removeQualifiers()->isSame(a_pInput))
        {
            if (!pOutput->isSame(a_pInput))
            {
                conv = newConv<DefaultConversionSequence>(
                newConv<StandardConversionSequence>(newConv<QualificationAdjustConversion>(
                a_pInput, pOutput, 1))); /// use a standard conversion to allow comparison with
                                         /// equivalent lvalue transformation : X& -> X
            }
            else
            {
                conv = newConv<DefaultConversionSequence>(
                newConv<StandardConversionSequence>(a_pInput)); /// use a standard conversion to allow comparison
                                                                /// with equivalent lvalue transformation : X& -> X
            }
            return; /// success X& -> X&
        }

        switch (castKind)
        {
        case CastKind::Const:
        {
            Type* pInputNoConst = a_pInput->removeAllQualifiers();
            Type* pOutputNoConst = pOutput->removeAllQualifiers();
            if (pInputNoConst->isSame(a_pInput) && pOutputNoConst->isSame(pOutput))
                return; /// input or output do not have const (or volatile ?) => const_cast fail
            if (pInputNoConst->isSame(pOutputNoConst)) /// no-cv input == no-cv output => const_cast ok
            {
                conv = newConv<DefaultConversionSequence>(a_pInput, pOutput);
            }
            /// X& -> X const&
            else
            {
                a_pInput->getReferencedType()->visit(this, a_Data);
                if (conv)
                {
                    PHANTOM_SEMANTIC_ASSERT(conv->input.type->removeAllQualifiers() ==
                                            a_pInput->getReferencedType()->removeAllQualifiers());
                    if (conv->semantic)
                        conv = conv->clone(getSource());
                    static_cast<DefaultConversionSequence*>(conv)->addLValueTransform(nullptr);
                }
            }
            return; /// const_cast ends here (no more user defined or else can be done)
        }
        case CastKind::Reinterpret:
        {
            if (pOutput->asReference())
            {
                if ((pOutput->removeReference()->asConstType() != nullptr) ==
                    (a_pInput->removeReference()->asConstType() != nullptr))
                {
                    conv = newConv<DefaultConversionSequence>(a_pInput, (ConversionVal)pOutput);
                }
                return; /// success X& -> Y&(&)
            }
            break;
        }
        case CastKind::Explicit:
        {
            if (pOutput->asReference())
            {
                conv = newConv<DefaultConversionSequence>(a_pInput, (ConversionVal)pOutput);
                return; /// success X& -> Y&(&)
            }
            break;
        }
        }

        // No implicit conversions from lvalue-references to rvalue-references
        if (pOutput->asRValueReference())
            return;

        LValueReference* pOutputReference = pOutput->asLValueReference();
        if (pOutputReference == nullptr) /// output is not a reference type => lvalue -> rvalue transformation
        {
            pOutput = pOutput->removeQualifiers();

            ClassType* pOutputClassType = pOutput->asClassType();

            if (pOutput->isSame(a_pInput->getReferencedType()))
            {
                Constructor* pCtor = nullptr;
                if (pOutputClassType)
                {
                    if ((pCtor = pOutputClassType->getCopyConstructor()))
                    {
                        if (!isSymbolAccessible(pCtor, pContextScope))
                        {
                            // CxxSemanticError("copy constructor is inaccessible");
                            conv = nullptr;
                            return;
                        }
                    }
                }

                /// X& -> X

                conv = newConv<DefaultConversionSequence>(newConv<LValueTransformationConversion>(a_pInput, pCtor));

                // NEED INVESTIGATION
                // auto userDefined = dconv->m_user_defined;
                // if (!userDefined || userDefined->input->asReference() == nullptr)
                // // not user defined conversion || the user defined conversion expects we remove
                // // the reference (l-value -> r-value)
                // {
                //     static_cast<DefaultConversionSequence*>(conv)->addLValueTransform(pCtor);
                // }
                // else if (userDefined && dconv->m_standard && dconv->m_standard->m_const_ref)
                // {
                //     Delete<RefConversion>(dconv)->m_standard->m_const_ref;
                //     dconv->m_standard->m_const_ref = nullptr; // remove X -> const X& because we already have a
                //                                               // reference as input
                // }
                // conv->input = a_pInput;
                return;
            }
            /// TypeX& -> ClassY
            else if (pOutputClassType)
            {
                if (iUserDefinedConversionsAllowed != UserDefinedFunctions::None)
                {
                    /// X& -> Y(X&)
                    conv = userDefinedConversionByConstruction(
                    a_pInput, pOutputClassType,
                    (castKind == CastKind::Implicit) ? (iUserDefinedConversionsAllowed == UserDefinedFunctions::All)
                                                     : true,
                    in_bInitialize, pContextScope, e_RefType_None);
                    if (conv)
                        return;
                    if (Class* pInputClass = a_pInput->getReferencedType()->asClass())
                    {
                        conv = userDefinedConversionByConversionFunction(
                        pInputClass, pOutputClassType,
                        (castKind == CastKind::Implicit) ? (iUserDefinedConversionsAllowed == UserDefinedFunctions::All)
                                                         : true,
                        pContextScope, e_RefType_None);
                    }
                    else if (ClassType* pInputClassType = a_pInput->getReferencedType()->asClassType())
                    {
                        conv = userDefinedConversionByConversionFunction(
                        pInputClassType, pOutputClassType,
                        (castKind == CastKind::Implicit) ? (iUserDefinedConversionsAllowed == UserDefinedFunctions::All)
                                                         : true,
                        pContextScope, e_RefType_None);
                        if (conv)
                            return;
                    }
                }
            }
            else
            {
                /// X& -> Y
                a_pInput->getReferencedType()->visit(this, a_Data);
                if (conv)
                {
                    PHANTOM_SEMANTIC_ASSERT(conv->input.type->removeAllQualifiers() ==
                                            a_pInput->getReferencedType()->removeAllQualifiers());
                    /// We found a conversion from X -> Y, we just add a l-value transform to it to
                    /// obtain X& -> Y
                    if (conv->semantic)
                        conv = conv->clone(getSource());
                    static_cast<DefaultConversionSequence*>(conv)->addLValueTransform(nullptr);
                    /// success X& -> Y
                }
            }
            return;
        }
        else /// X& -> Y& (output is a reference type)
        {
            Type* pInputReferencedType = a_pInput->getReferencedType();          /// X
            Type* pOutputReferencedType = pOutputReference->getReferencedType(); /// X

            //                 if(pInputReferencedType->asPlaceholder() &&
            //                 pOutputReferencedType->asPlaceholder())
            //                 {
            //                     /// T& -> U&
            //                     conv = newConv<DefaultConversionSequence>(a_pInput, pOutput,
            //                     10); return;
            //                 }
            /// X& -> Y&
            std::pair<Type*, Type*> constAdjustment;
            int                     adjustmentCount = 0;
            Type*                   pOutputReferencedTypeNoConst = pOutputReferencedType->removeConst(); /// Y
            Type*                   pInputReferencedTypeNoConst = pInputReferencedType->removeConst();   /// X
            if (pOutputReferencedType->asConstType())
            {
                /// X& -> Y const&
                if (pInputReferencedTypeNoConst->isA(pOutputReferencedTypeNoConst))
                {
                    /// X and Y are isSame => make const adjustment
                    LValueReference* adjustmentInput = pOutputReferencedTypeNoConst->makeLValueReference();
                    constAdjustment.first = adjustmentInput;
                    /// merge const adjustments
                    constAdjustment.second = pOutputReference;
                    adjustmentCount++;
                    pOutputReference = adjustmentInput;

                    /// if X == Y => just a const adjustement
                    if (pInputReferencedTypeNoConst->isSame(pOutputReferencedTypeNoConst))
                    {
                        conv = newConv<DefaultConversionSequence>(newConv<QualificationAdjustConversion>(
                        constAdjustment.first, constAdjustment.second, adjustmentCount));
                        return; /// success X& -> X const&
                    }
                }
                else
                {
                    /// X and Y are not related => it's possibly a const ref conversion
                    /// Search conversion X& -> Y and, if found, add const ref conversion then to do
                    /// X& -> Y const&
                    a_Data.in[0] = &pOutputReferencedTypeNoConst; // modify output in visitor data
                    a_pInput->getReferencedType()->visit(this, a_Data);
                    if (conv)
                    {
                        if (conv->semantic)
                            conv = conv->clone(getSource());
                        DefaultConversionSequence* dcs = static_cast<DefaultConversionSequence*>(conv);
                        if (dcs->m_user_defined)
                        {
                            conv->input = a_pInput;
                            /// if a user defined conversion has been requested it may have
                            /// requested a const ref adjustment which is not necessary due to the
                            /// fact we removed the reference to perform the conversion
                            if (dcs->m_standard)
                            {
#pragma message("TODO : implement 'DefaultConversionSequence::removeConstRef'")
                                if (dcs->m_standard->m_const_ref)
                                {
                                    deleteVirtual(dcs->m_standard->m_const_ref);
                                    dcs->m_standard->m_const_ref = nullptr;
                                }
                                else /// if no reference was needed, it means we must restore X& ->
                                     /// X preambule conversion
                                {
                                    dcs->m_standard->addLValueTransform(nullptr);
                                }
                            }
                        }
                        else
                        {
                            dcs->addLValueTransform(nullptr);
                        }
                        dcs->addConstRef();
                        return; /// success X& -> Y const&
                    }
                    return;
                }
            }
            else if (a_pInput->getReferencedType()->asConstType())
            {
                return; /// failure X const& -/-> Y&
            }
            Class* pInputClass;
            Class* pOutputClass;
            if (((pInputClass = pInputReferencedTypeNoConst->asClass()) != nullptr) &&
                ((pOutputClass = pOutputReferencedTypeNoConst->asClass()) != nullptr))
            {
                size_t level = pInputClass->getInheritanceLevelFromBase(pOutputClass);
                if (level != ~size_t(0)) // level != -1 => inheritance relation => pointers are related
                {
                    auto* pPtr = newConv<PointerConversion>(a_pInput, pOutputReference, level,
                                                            pInputClass->getBaseClassOffsetCascade(pOutputClass));
                    if (adjustmentCount)
                        conv = newConv<DefaultConversionSequence>(
                        pPtr,
                        newConv<QualificationAdjustConversion>(constAdjustment.first, constAdjustment.second,
                                                               adjustmentCount));
                    else
                        conv = newConv<DefaultConversionSequence>(pPtr);
                    return; /// success Derived& -> Base(const)&
                }
                else if (castKind == CastKind::Static || castKind == CastKind::Explicit)
                {
                    size_t level = pOutputClass->getInheritanceLevelFromBase(pInputClass);
                    if (level != ~size_t(0)) // level != -1 => inheritance relation => pointers are related
                    {
                        auto* pPtr = newConv<PointerConversion>(a_pInput, pOutputReference, level,
                                                                pOutputClass->getBaseClassOffsetCascade(pInputClass));
                        if (adjustmentCount)
                            conv = newConv<DefaultConversionSequence>(
                            pPtr,
                            newConv<QualificationAdjustConversion>(constAdjustment.first, constAdjustment.second,
                                                                   adjustmentCount));
                        else
                            conv = newConv<DefaultConversionSequence>(pPtr);
                        return; /// success Base(const)& -> Derived&
                    }
                    else if (castKind == CastKind::Explicit)
                    {
                        conv = newConv<DefaultConversionSequence>(a_pInput, (ConversionVal)pOutput);
                        return; /// success X& -> Y&
                    }
                }
            }
        }
    }
    break;

    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        Type*                       pNewReferencedType = o_resolveT(Type, a_pInput->getReferencedType());
        if (pNewReferencedType == nullptr)
            return;
        pResolved = pNewReferencedType->addLValueReference();
        if (pResolved == pNewReferencedType)
        {
            CxxSemanticError("'%s' : type cannot have l-value reference", FormatCStr(pResolved));
            pResolved = nullptr;
        }
    }
        return;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        PHANTOM_SEMANTIC_ASSERT(false);
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Type*                       pType = o_instantiateT(Type, a_pInput->getReferencedType());
        if (pType)
        {
            pInstanciated = pType->makeLValueReference();
        }
    }
        return;
    }
    visit(static_cast<Reference*>(a_pInput), a_Data);
}
void Semantic::visit(MemberAnonymousSection* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(MemberAnonymousStruct* a_pInput, VisitorData a_Data)
{
    visit(static_cast<MemberAnonymousSection*>(a_pInput), a_Data);
}
void Semantic::visit(MemberAnonymousUnion* a_pInput, VisitorData a_Data)
{
    visit(static_cast<MemberAnonymousSection*>(a_pInput), a_Data);
}
void Semantic::visit(Method* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_CreateCallExpression:
    {
        ExpressionsView  in_Arguments = *(const ExpressionsView*)a_Data.in[0];
        LanguageElement* in_pContextScope = *(LanguageElement**)a_Data.in[1];
        PHANTOM_SEMANTIC_ASSERT(in_Arguments.size() > 0);
        Expression* pCaller = in_Arguments.front();
        in_Arguments = ExpressionsView(in_Arguments.data() + 1, in_Arguments.size() - 1);
        Expression*& out_pResult = *(Expression**)a_Data.out[0];
        out_pResult = createCallExpression(a_pInput, pCaller, in_Arguments, in_pContextScope);
    }
        return;
    case e_VisitorFunction_IsViableCallCandidate:
    {
        OptionalArrayView<LanguageElement*> templateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[0];
        TypesView                           functionArguments = *(const TypesView*)a_Data.in[1];
        Modifiers                           modifiers = *(Modifiers*)a_Data.in[2];
        LanguageElement*                    pScope = *(LanguageElement**)a_Data.in[3];
        UserDefinedFunctions                convUserDefinedFunctions = *(UserDefinedFunctions*)a_Data.in[4];
        ConversionResults&                  out_conversions = *(ConversionResults*)a_Data.out[0];
        Subroutine*&                        out_subroutine = *(Subroutine**)a_Data.out[1];
        PlaceholderMap&                     out_deductions = *(PlaceholderMap*)a_Data.out[2];
        out_subroutine = nullptr;

        // NOTE : this asserts is not really an assert but more an error by itself (the assert can
        // be triggered legitimately when calling A::myMethod() (explicit nonvirtual call) inside B
        // scope
        // PHANTOM_SEMANTIC_ASSERT(functionArguments.front()->getValueType()->removeReference()->removeRValueReference()->removeConst()->isA(a_pInput->getOwner()->asClassType()));

        if ((functionArguments.size() - 1) < a_pInput->getRequiredArgumentCount() ||
            (!(a_pInput->getSignature()->isVariadic()) &&
             ((functionArguments.size() - 1) > a_pInput->getParameters().size())))
        {
            return;
        }
        if (a_pInput->acceptsCallerExpressionType(functionArguments.front()))
        {
            /// implicit conversion of the implicit object parameter
            Type*       pOutput = a_pInput->getImplicitObjectParameterType();
            Conversion* pConversion = nullptr;
            VisitorData conversionData;
            conversionData.id = e_VisitorFunction_TypeConversion;
            CastKind         castKind = CastKind::Implicit;
            bool             iUserDefinedConversionsAllowed = false;
            LanguageElement* pContextScope = pScope;
            bool             init = false;
            const void*      in[5] = {&pOutput, &castKind, &iUserDefinedConversionsAllowed, &pContextScope, &init};
            void*            out[1] = {&pConversion};
            conversionData.in = in;
            conversionData.out = out;
            Type* pInputType = functionArguments[0];
            if (pInputType->getTypeKind() == TypeKind::RValueReference &&
                pOutput->getTypeKind() == TypeKind::LValueReference)
            {
                pInputType->removeRValueReference()->addLValueReference()->visit(this, conversionData);
            }
            else if (pInputType->asClassType())
            {
                pInputType->addRValueReference()->visit(this, conversionData);
                if (pConversion) // try && signatures
                {
                    if (pConversion->semantic)
                        pConversion = pConversion->clone(getSource());
                    // add X -> X&& rvalue 'promotion'
                    static_cast<DefaultConversionSequence*>(pConversion)->addRValuePromotion();
                }
                else // fallback to & signatures
                {
                    // search for X& -> ?
                    pInputType->addLValueReference()->visit(this, conversionData);
                    if (auto pSeq = static_cast<DefaultConversionSequence*>(pConversion))
                    {
                        if (pSeq->semantic)
                            pSeq = pSeq->clone(getSource());
                        // reinterpret X& to X&&
                        pSeq->input = pInputType;
                        if (pSeq->m_standard)
                            pSeq->m_standard->input = pInputType;

                        // prepend X -> X&& rvalue 'promotion'
                        pSeq->addRValuePromotion();
                        pConversion = pSeq;
                    }
                }
            }
            else
            {
                pInputType->addLValueReference()->visit(this, conversionData);
            }
            out_conversions.push_back(pConversion);

            /// list implicit conversions of arguments
            Types argumentTypes;

            auto& params = a_pInput->getParameters();
            for (size_t i = 0; i < params.size(); ++i)
            {
                if (i < (functionArguments.size() - 1))
                {
                    argumentTypes.push_back(functionArguments[i + 1]);
                }
            }

            if (auto pTSpec = a_pInput->getTemplateSpecialization())
            {
                if (!_useExplicitTemplateArguments(a_pInput, pTSpec, templateArguments, out_deductions, "method"))
                    return;
                newImplicitConversionsWithArgDeductions(a_pInput->getSignature(), argumentTypes, out_conversions,
                                                        out_deductions, pScope, convUserDefinedFunctions);
            }
            else
                newImplicitConversions(a_pInput->getSignature(), argumentTypes, out_conversions, pScope,
                                       convUserDefinedFunctions);

            if (!out_conversions.hasNull())
            {
                out_subroutine = a_pInput;
            }
        }
    }
        return;
    case e_VisitorFunction_IsViableSignatureCandidate:
    {
        const LanguageElements* templateArguments = a_Data.in[0] ? (const LanguageElements*)a_Data.in[0] : nullptr;
        TypesView               types = *(const Types*)a_Data.in[1];
        Modifiers               modifiers = *(Modifiers*)a_Data.in[2];
        bool&                   out_result = *(bool*)a_Data.out[0];

        out_result = templateArguments ? a_pInput->matches(*templateArguments, types, modifiers)
                                       : a_pInput->matches(types, modifiers);
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];

        auto pInstanceClassType = in_pContextScope->asClassType();

        TemplateSpecialization* pSpec = {};
        if (!pInstanceClassType)
        {
            pSpec = in_pContextScope->asTemplateSpecialization();
            PHANTOM_SEMANTIC_ASSERT(pSpec);
            // in_TemplateSubstitution.getInstantiation()->getInstantiationSpecialization() == a_pInput->getOwner());
            pInstanceClassType = static_cast<ClassType*>(pSpec->getNamingScope());
        }
        else
        {
            if (a_pInput->testFlags(PHANTOM_R_FLAG_IMPLICIT) || a_pInput->getVisibility() == Visibility::Private)
            {
                // not instanciating trivial members
                return;
            }
        }
        switch (in_eClassBuildState)
        {
        case e_ClassBuildState_None:
            if (!pSpec)
                break;
        case e_ClassBuildState_Members:
        {
            Signature* pSignature = o_instantiateT(Signature, a_pInput->getSignature());
            if (pSignature == nullptr)
                return;
            String name = a_pInput->getName();
            if (name.size() > 8 && name[8] == ' ' && name.find("operator") == 0)
            {
                name = "operator " + pSignature->getReturnType()->getUniqueName();
            }
            OwnerGuard<Method> pInstanceMethod = New<Method>(name, pSignature, a_pInput->getModifiers(),
                                                             a_pInput->getFlags() & ~PHANTOM_R_FLAG_TEMPLATE_DEPENDANT);
            Subroutines        conflicting;
            if (!(pInstanceClassType->acceptsSubroutine(pInstanceMethod.element(), &conflicting)))
            {
                auto   errArgs = conflicting.data();
                size_t count = conflicting.size();
                CxxSemanticError("'%s' : function conflicts without previously declared one(s) :",
                                 FormatCStr(pInstanceMethod.element()));
                for (auto c : conflicting)
                {
                    CxxSemanticSubError("'%s'", FormatCStr(c));
                }
                return;
            }
            pInstanciated = pInstanceMethod;
            if (!pSpec)
            {
                pInstanceClassType->addMethod(pInstanceMethod);
                break;
            }
            else
            {
                pSpec->setTemplated(pInstanceMethod);
                pInstanceMethod->createThis(pInstanceClassType);
            }
            // continue to case e_ClassBuildState_Blocks when template function instantiation
        }

        case e_ClassBuildState_Blocks:
        {
            if (!a_pInput->buildBlock())
            {
                CxxSemanticError("failed to instantiate block of template method '%.*s'",
                                 PHANTOM_STRING_AS_PRINTF_ARG(a_pInput->getName()));
                pInstanciated = nullptr;
                return;
            }
            if (pInstanciated)
            {
                Method* pInstanceMethod = static_cast<Method*>(pInstanciated);
                if (!(pInstanceMethod->testModifiers(Modifier::Deleted)) &&
                    !(pInstanceMethod->testModifiers(Modifier::Defaulted)) &&
                    !(pInstanceMethod->testFlags(PHANTOM_R_FLAG_IMPLICIT)))
                {
                    Block* pPrimary = a_pInput->getBlock();
                    PHANTOM_SEMANTIC_ASSERT(pPrimary);
                    PHANTOM_SEMANTIC_ASSERT(pInstanceMethod->getBlock() == nullptr);
                    o_mapT(pPrimary, addBlock(pInstanceMethod, true));
                    o_mapT(a_pInput->getThis(), pInstanceMethod->getThis());

                    pInstanceMethod->setBlockBuilder([=](Block* a_pBlock) {
                        pInstanceMethod->setBlockBuilder(
                        {}); // avoid cyclic builder invocation if function is recursive and calls itself
                        PHANTOM_SEMANTIC_ASSERT(a_pBlock == pInstanceMethod->getBlock());
                        SemanticPrivate::SemanticInstantiateTemplateElement<Block>(
                        this, pPrimary, in_TemplateSubstitution, e_ClassBuildState_Blocks, pInstanceMethod,
                        a_Data.flags, &LanguageElement::asBlock);
                        return true;
                    });

#if PHANTOM_CODE_LAZY_COMPILATION_ENABLED == 0
                    pInstanceMethod->buildBlock();
#endif
                }
            }
        }
        break;

        default:
            break;
        }
    }
        return;

    case e_VisitorFunction_Access:
    {
        if (a_pInput->getOwner())
            a_pInput->getOwner()->visit(this, a_Data);
    }
        return;
    }
    visit(static_cast<Subroutine*>(a_pInput), a_Data);
}

void Semantic::visit(MethodPointerCallExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_HasSideEffect:
    {
        *(bool*)a_Data.out[0] = true;
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnersGuard<Expressions>    newArguments;
        for (auto pArgument : a_pInput->getArguments())
        {
            Expression* pExp = o_instantiateT(Expression, pArgument);
            if (pExp == nullptr)
                return;
            newArguments.push_back(pExp);
        }
        OwnerGuard<Expression> pObjExp = o_instantiateT(Expression, a_pInput->getObjectExpression());
        if (pObjExp == nullptr)
            return;
        OwnerGuard<Expression> pPtrExp = o_instantiateT(Expression, a_pInput->getMemberExpression());
        if (pPtrExp == nullptr)
            return;
        pInstanciated = createCallExpression(pObjExp, pPtrExp, newArguments, in_pContextScope);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(MethodPointer* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TypeConversion:
    {
        Type*                   pOutput = *(Type* const*)a_Data.in[0];
        CastKind                castKind = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions    iUserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        LanguageElement*        pContextScope = *(LanguageElement**)a_Data.in[3];
        Conversion*&            conv = *(Conversion**)a_Data.out[0];
        std::pair<Type*, Type*> constAdjustment;
        int                     adjustmentCount = 0;
        ERefType                refType = baseConversion(a_pInput, pOutput, castKind);

        if (pOutput->isSame(a_pInput))
        {
            if (adjustmentCount)
            {
                conv = (refType != e_RefType_None)
                ? newConv<DefaultConversionSequence>(newConv<QualificationAdjustConversion>(
                                                     constAdjustment.first, constAdjustment.second, adjustmentCount),
                                                     newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
                : newConv<DefaultConversionSequence>(newConv<QualificationAdjustConversion>(
                  constAdjustment.first, constAdjustment.second, adjustmentCount));
            }
            conv = (refType != e_RefType_None)
            ? newConv<DefaultConversionSequence>(newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
            : newConv<DefaultConversionSequence>(a_pInput, (ConversionVal)pOutput);
            return; /// success X (Class::*)(...) -> X (Class::*)(...)
        }

        if (pOutput->removeQualifiers() == PHANTOM_TYPEOF(bool))
        {
            auto numconv = newConv<NumericConversionT<void*, bool, CastKind::Implicit>>(a_pInput, PHANTOM_TYPEOF(bool));
            conv = refType != e_RefType_None
            ? newConv<DefaultConversionSequence>(numconv, newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
            : newConv<DefaultConversionSequence>(numconv);
            return;
        }

        if (pOutput->asMethodPointer())
        {
            MethodPointer* pOutputMethodPointer = static_cast<MethodPointer*>(pOutput);

            if (a_pInput->getFunctionType()->isSame(pOutputMethodPointer->getFunctionType())) /// same signature
            {
                if ((a_pInput->getObjectType()->asClass() != nullptr) &&
                    (pOutputMethodPointer->getObjectType()->asClass() != nullptr))
                {
                    lang::Class* pInputClass = static_cast<lang::Class*>(a_pInput->getObjectType());
                    lang::Class* pOutputClass = static_cast<lang::Class*>(pOutputMethodPointer->getObjectType());
                    size_t       level = pInputClass->getInheritanceLevelFromBase(pOutputClass);
                    if (level != ~size_t(0)) // level != -1 => inheritance relation => pointers are related
                    {
                        auto* pPtr = newConv<MethodPointerConversion>(
                        a_pInput, pOutputMethodPointer, level, pInputClass->getBaseClassOffsetCascade(pOutputClass));

                        if (adjustmentCount)
                        {
                            conv = (refType != e_RefType_None)
                            ? newConv<DefaultConversionSequence>(
                              pPtr,
                              newConv<QualificationAdjustConversion>(constAdjustment.first, constAdjustment.second,
                                                                     adjustmentCount),
                              newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
                            : newConv<DefaultConversionSequence>(
                              pPtr,
                              newConv<QualificationAdjustConversion>(constAdjustment.first, constAdjustment.second,
                                                                     adjustmentCount));
                        }
                        conv = (refType != e_RefType_None)
                        ? newConv<DefaultConversionSequence>(
                          pPtr, newConv<RefConversion>(pOutput, refType == e_RefType_RValue))
                        : newConv<DefaultConversionSequence>(pPtr);
                        return; /// success X (Derived::*)(...) -> X (Base::*)(...)
                    }
                }
            }
        }
    }
    break;

    case e_VisitorFunction_ResolveTemplateDependencies:
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnersGuard<Types>          parameters;
        for (auto pType : a_pInput->getFunctionType()->getParameterTypes())
        {
            if (Type* pParamType = o_resolveT(Type, pType))
            {
                parameters.push_back(o_resolveT(Type, pType));
            }
            else
            {
                return; // error must have been reported deeper
            }
        }
        ClassType* pObjectType = o_resolveT(ClassType, a_pInput->getObjectType());
        pInstanciated = getSource()->methodPointerType(
        pObjectType, o_resolveT(Type, a_pInput->getFunctionType()->getReturnType()), parameters,
        a_pInput->getModifiers() & (PHANTOM_R_LVALUEREF | PHANTOM_R_CONST | PHANTOM_R_RVALUEREF));
    }
        return;
    }
    visit(static_cast<MemberPointer*>(a_pInput), a_Data);
}
void Semantic::visit(MemberPointer* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void Semantic::visit(Module* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_UnqualifiedLookup:
    {
        a_pInput->getOwner()->visit(this, a_Data);
    }
    break;
    }
}
void Semantic::visit(Symbol* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        if (auto pSpec = a_pInput->getTemplateSpecialization())
        {
            Template*        pTemplate = pSpec->getTemplate();
            LanguageElements resolvedArguments;
            _resolveTemplateSpecializationArguments(pSpec, in_TemplateSubstitution, in_eClassBuildState,
                                                    in_pContextScope, resolvedArguments);
            pResolved = instantiateTemplate(pTemplate, resolvedArguments, in_pContextScope);
            if (pResolved == nullptr)
            {
                auto   errArgs = in_TemplateSubstitution.getArguments().data();
                size_t count = in_TemplateSubstitution.getArguments().size();
                CxxSemanticError("'%s' : no template specialization found, arguments are (%s)", FormatCStr(pTemplate),
                                 FormatElementList(in_TemplateSubstitution.getArguments().data(),
                                                   in_TemplateSubstitution.getArguments().size())
                                 .data());
                return;
            }
        }
        else
        {
            pResolved = o_findT(LanguageElement, a_pInput);
            if (pResolved == nullptr)
                pResolved = a_pInput;
        }
    }
        return;

    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView       in_name = *(StringView*)a_Data.in[0];
        LanguageElement* in_pScope = *(LanguageElement**)a_Data.in[1];
        Access           in_context_access = *(Access*)a_Data.in[2];
        Symbols&         out_candidates = *(Symbols*)a_Data.out[0];
        Accesses&        out_context_accesses = *(Accesses*)a_Data.out[1];

        /// search for all elements with , => these are the candidates
        for (auto& pElm : a_pInput->getElements())
        {
            Symbol* pSymbol = pElm->asSymbol();
            if (!pSymbol)
                continue;

            auto vis = pSymbol->getVisibility();
            if (vis == Visibility::Private ||
                (vis == Visibility::Protected && in_pScope != a_pInput && !in_pScope->hasOwnerCascade(a_pInput)))
                continue;

            if (pSymbol->getName() == in_name)
            {
                //
                //                     {
                //                         if (!(pSpec->testFlags(PHANTOM_R_FLAG_IMPLICIT))) // not an
                //                         instantiation but a specialization
                //                         {
                //                             Template* pTemplate =
                //                             static_cast<TemplateSpecialization*>(pSymbol)->getTemplate(); if
                //                             (std::find(out_candidates.begin(), out_candidates.end(),
                //                             pTemplate)
                //                             ==
                //                                 out_candidates.end())
                //                             {
                //                                 PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((pTemplate));
                //                                 out_candidates.push_back(pTemplate);
                //                             }
                //                         }
                //                     }
                //                     else
                if (!pSymbol->asTemplateSpecialization())
                {
                    PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((pSymbol));
                    out_candidates.push_back(pSymbol);
                }
            }
            else if (Enum* pEnum = pSymbol->asEnum())
            {
                if (!pEnum->isScoped()) // not "enum class"
                {
                    pSymbol->visit(this, a_Data);
                }
            }
        }

        /// if no element found matching the name, we look for every anonymous element, and restart
        /// search inside them recursively
        if (a_Data.flags & e_VisitorFlag_Unqualified)
        {
            if (out_candidates.empty())
            {
                for (auto& pElm : a_pInput->getElements())
                {
                    Symbol* pSymbol = pElm->asSymbol();
                    if (!pSymbol)
                        continue;
                    auto vis = pSymbol->getVisibility();
                    if (vis == Visibility::Private ||
                        (vis == Visibility::Protected && in_pScope != a_pInput &&
                         !in_pScope->hasOwnerCascade(a_pInput)))
                        continue;
                    if (pSymbol && pSymbol->getName().empty() && isSymbolAccessible(pSymbol, in_pScope))
                    {
                        Access access = in_context_access;
                        /// Inherits access from anonymous element
                        if (pSymbol->getAccess() > access)
                            access = pSymbol->getAccess();
                        Symbols  sub_candidates;
                        Accesses sub_context_accesses;
                        internalQualifiedSymbolsLookup(pSymbol, in_name, sub_candidates, sub_context_accesses,
                                                       in_pScope, access);
                        for (auto sc : sub_candidates)
                            PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((sc));
                        out_candidates.insert(out_candidates.end(), sub_candidates.begin(), sub_candidates.end());
                        out_context_accesses.insert(out_context_accesses.end(), sub_context_accesses.begin(),
                                                    sub_context_accesses.end());
                    }
                }
            }
        }
    }
        return;

    case e_VisitorFunction_Access:
    {
        Symbol* in_pSymbol = *(Symbol**)a_Data.in[0];
        bool&   out_bResult = *(bool*)a_Data.out[0];

        visit(static_cast<LanguageElement*>(a_pInput), a_Data);

        out_bResult = out_bResult || in_pSymbol->isPublic() || isFriendWith(in_pSymbol, a_pInput);
    }
        return;
    }
    visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}

void Semantic::visit(SymbolReferenceExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        pInstanciated = New<SymbolReferenceExpression>(o_resolveT(Symbol, a_pInput->getSymbol()));
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}

void Semantic::visit(Namespace* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_SubroutineResolution:
        visit(static_cast<Scope*>(a_pInput), a_Data);
        return;

    case e_VisitorFunction_QualifiedLookup:
        visit(static_cast<Symbol*>(a_pInput), a_Data);
        return;

    case e_VisitorFunction_UnqualifiedLookup:
        if (a_pInput == Namespace::Global())
        {
            a_Data.id = e_VisitorFunction_QualifiedLookup;
            return visit(a_pInput, a_Data);
        }
        visit(static_cast<Symbol*>(a_pInput), a_Data);
        return;

    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView       in_name = *(StringView*)a_Data.in[0];
        LanguageElement* in_pScope = *(LanguageElement**)a_Data.in[1];
        Access           in_context_access = *(Access*)a_Data.in[2];
        Symbols&         out_candidates = *(Symbols*)a_Data.out[0];
        Accesses&        out_context_accesses = *(Accesses*)a_Data.out[1];

        visit(static_cast<Symbol*>(a_pInput), a_Data);

        if (out_candidates.size())
            return;

        /// In a namespace - except for child namespaces - elements are not owned, they are
        /// referenced, that's why we browse reference elements

        /// search for all elements with , => these are the candidates
        for (auto& pSymbol : a_pInput->getSymbols())
        {
            if (((pSymbol->getSource() == nullptr) || !(pSymbol->getSource()->getVisibility() == Visibility::Private)))
            {
                if (pSymbol->getName() == in_name && !(pSymbol->asTemplateSpecialization()))
                {
                    PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((pSymbol));
                    out_candidates.push_back(pSymbol);
                }
                else if (Enum* pEnum = pSymbol->asEnum())
                {
                    if (!pEnum->isScoped()) // not "enum class"
                    {
                        pSymbol->visit(this, a_Data);
                    }
                }
            }
        }

        /// if no element found matching the name, we look for every anonymous element, and restart
        /// search inside them recursively
        if (out_candidates.empty())
        {
            for (Symbol* pSymbol : a_pInput->getSymbols())
            {
                Visibility vis{};
                if (pSymbol->getName().empty() &&
                    (pSymbol->getSource() == nullptr ||
                     !(pSymbol->getSource()->getVisibility() == Visibility::Private)) /// archived => cannot be found
                                                                                      /// via namespace resolution
                    && !((vis = pSymbol->getVisibility()) == Visibility::Private) &&
                    (pSymbol->getSource() == in_pScope->getSource() || !(vis == Visibility::Protected)))
                {
                    Access access = in_context_access;
                    /// Inherits access from anonymous element
                    if (pSymbol->getAccess() > access)
                        access = pSymbol->getAccess();
                    Symbols  sub_candidates;
                    Accesses sub_context_accesses;
                    internalQualifiedSymbolsLookup(pSymbol, in_name, sub_candidates, sub_context_accesses, in_pScope,
                                                   access);
                    for (auto sc : sub_candidates)
                        PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((sc));
                    out_candidates.insert(out_candidates.end(), sub_candidates.begin(), sub_candidates.end());
                    out_context_accesses.insert(out_context_accesses.end(), sub_context_accesses.begin(),
                                                sub_context_accesses.end());
                }
            }
        }

        /// search among inline namespaces
        for (auto& p : a_pInput->getNamespaces())
        {
            Namespace* pNamespace = p;
            if (pNamespace->testModifiers(PHANTOM_R_INLINE))
            {
                pNamespace->visit(this, a_Data);
            }
        }
    }
    break;
    }
}
void Semantic::visit(NewExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        // FIXME : update to new/delete expressions
        OwnerGuard<Expression> pExp = o_instantiateT(Expression, a_pInput->getInitializationExpression());
        if (pExp == nullptr)
            return;
        OwnerGuard<Expression> pCount = nullptr;
        if (a_pInput->getCountExpression())
        {
            pCount = o_instantiateT(Expression, a_pInput->getCountExpression());
            if (pCount == nullptr)
                return;
            Expression* pConvCount = CxxSemanticConversionNE(pCount, PHANTOM_TYPEOF(size_t));
            if (pConvCount == nullptr)
                return;
            pCount = pConvCount;
        }
        pInstanciated = New<NewExpression>(pExp, pCount);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(Package* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
        //     case e_VisitorFunction_QualifiedNameLookup:
        //         {
        //             StringView in_name = *(StringView*)a_Data.in[0];
        //             LanguageElement* in_pScope = *(LanguageElement**)a_Data.in[1];
        //             Access in_context_access = *(Access*)a_Data.in[2];
        //             Symbols& out_candidates = *(Symbols*)a_Data.out[0];
        //             Accesses& out_context_accesses = *(Accesses*)a_Data.out[1];
        //             for(auto it = a_pInput->beginSources(); it != a_pInput->endSources(); ++it)
        //             {
        //                 (p)->visit(this, a_Data);
        //             }
        //             if(out_candidates.size())
        //                 return;
        //         }
        //         break;
    case e_VisitorFunction_QualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        LanguageElement*                    in_pContextScope = *(LanguageElement**)a_Data.in[3];
        Type*                               in_pInitializationType = (Type*)a_Data.in[4];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];
        for (auto& p : a_pInput->getSources())
        {
            if ((p)->getName() == in_name)
            {
                if (in_pTemplateArguments || in_pFunctionArguments) /// if we have a template signature or function
                                                                    /// signature associated with a source name, we
                                                                    /// look inside the source itself
                {
                    (p)->visit(this, a_Data);
                    return;
                }
                if (out_pResult == nullptr)
                {
                    out_pResult = p;
                }
                return;
            }
        }
    }
        return;

    case e_VisitorFunction_UnqualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];
        if (in_name == a_pInput->getName())
        {
            if (in_pFunctionArguments || in_pTemplateArguments)
                return;
            out_pResult = a_pInput;
            return;
        }
        Application::Get()->visit(this, a_Data);
    }
        return;
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(PackageFolder* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView       in_name = *(StringView*)a_Data.in[0];
        LanguageElement* in_pScope = *(LanguageElement**)a_Data.in[1];
        Access           in_context_access = *(Access*)a_Data.in[2];
        Symbols&         out_candidates = *(Symbols*)a_Data.out[0];
        Accesses&        out_context_accesses = *(Accesses*)a_Data.out[1];
        Source*          pSource = a_pInput->getSource(in_name);
        if (pSource)
        {
            PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((pSource));
            out_candidates.push_back(pSource);
            return;
        }
        for (auto& p : a_pInput->getPackageFolders())
        {
            if ((p)->getName() == in_name)
            {
                PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((p));
                out_candidates.push_back(p);
                return;
            }
        }
    }
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(Parameter* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Type*                       pType = o_resolveT(Type, a_pInput->getValueType());
        if (pType == nullptr)
            return;
        Parameter* pParam = New<Parameter>(pType, a_pInput->getName(), a_pInput->getModifiers());
        if (Expression* pExp = a_pInput->getDefaultArgumentExpression())
        {
            pParam->setDefaultArgumentExpression(o_instantiateT(Expression, pExp));
        }

        pInstanciated = pParam;
    }
        return;
    }
    visit(static_cast<LocalVariable*>(a_pInput), a_Data);
}
void Semantic::visit(Placeholder* a_pInput, VisitorData a_Data) {}
void Semantic::visit(PlaceholderClass* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView in_name = *(StringView*)a_Data.in[0];
        Symbols&   out_candidates = *(Symbols*)a_Data.out[0];
        out_candidates.push_back(New<TemplateDependantElement>(a_pInput, in_name));
    }
    break;
    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        Placeholder*                pPlaceholder = a_pInput->getOwner()->asPlaceholder();
        LanguageElement*            pArgument = in_TemplateSubstitution.getArgument(pPlaceholder);
        if (pArgument)
        {
            pResolved = pArgument->asClass();
            if (pResolved == nullptr)
            {
                CxxSemanticError("'%s' : not a class", FormatCStr(pArgument));
            }
        }
        else
            pResolved = a_pInput;
    }
        return;
    }
}
void Semantic::visit(PlaceholderClassType* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView in_name = *(StringView*)a_Data.in[0];
        Symbols&   out_candidates = *(Symbols*)a_Data.out[0];
        out_candidates.push_back(New<TemplateDependantElement>(a_pInput, in_name));
    }
    break;
    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        Placeholder*                pPlaceholder = a_pInput->getOwner()->asPlaceholder();
        LanguageElement*            pArgument = in_TemplateSubstitution.getArgument(pPlaceholder);
        if (pArgument)
        {
            pResolved = pArgument->asClassType();
            if (pResolved == nullptr)
            {
                CxxSemanticError("'%s' : not a class-type", FormatCStr(pArgument));
            }
        }
        else
            pResolved = a_pInput;
    }
        return;
    }
}
void Semantic::visit(PlaceholderConstant* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ToExpression:
    {
        Expression*  in_pLeftExpression = *(Expression**)a_Data.in[0];
        Expression*& out_pExpression = *(Expression**)a_Data.out[0];
        if (in_pLeftExpression)
            return;
        out_pExpression = New<ConstantExpression>(a_pInput);
        return;
    }
    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*  pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        Expression*       pArgumentExpression = pArgument->asExpression();
        Constant*         pArgumentConstant = nullptr;
        if (pArgumentExpression == nullptr)
        {
            pArgumentConstant = pArgument->asConstant();
            if (pArgumentConstant)
            {
                auto alreadyDeduced = deductions.find(a_pInput);
                if (alreadyDeduced != deductions.end())
                {
                    /// already deduced => needs perfect match
                    if (pArgumentConstant->getValueType()->isSame(alreadyDeduced->second))
                    {
                        pDeducedArgument = pArgumentConstant;
                    }
                }
                else
                {
                    Conversion* pConv = newConversion(pArgumentConstant->getValueType(), a_pInput->getValueType(),
                                                      CastKind::Implicit, UserDefinedFunctions::None, nullptr);
                    if (pConv == nullptr)
                        return;
                    PHANTOM_SEMANTIC_ASSERT(pConv->semantic);
                    pDeducedArgument = pArgumentConstant;
                    deductions[a_pInput] = pArgumentConstant;
                }
            }
        }
        else
        {
            if (!(pArgument->isCompileTime())) /// argument not an expression or not compile time element
                return;

            auto alreadyDeduced = deductions.find(a_pInput);
            if (alreadyDeduced != deductions.end())
            {
                /// already deduced => needs perfect match
                if (pArgumentExpression->getValueType()->removeReference()->removeConst()->isSame(
                    alreadyDeduced->second))
                {
                    pDeducedArgument = pArgumentExpression;
                }
            }
            else
            {
                if (pDeducedArgument = convert(pArgumentExpression, a_pInput->getValueType(), CastKind::Implicit,
                                               UserDefinedFunctions::None, nullptr))
                {
                    deductions[a_pInput] = pDeducedArgument;
                }
            }
        }
    }
        return;

    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        LanguageElement*            pArgument = in_TemplateSubstitution.getArgument(a_pInput);
        if (pArgument)
        {
            pResolved = pArgument->asConstant();
            if (pResolved == nullptr)
            {
                CxxSemanticError("'%s' : not a constant", FormatCStr(pArgument));
            }
        }
        else
            pResolved = a_pInput;
    }
        return;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstantiated = *(LanguageElement**)a_Data.out[0];
        auto                        pResolvedType = o_resolveT(Type, a_pInput->getValueType());
        pInstantiated = New<PlaceholderConstant>(pResolvedType, a_pInput->getName());
    }
        return;
    }
    visit(static_cast<Constant*>(a_pInput), a_Data);
}
void Semantic::visit(PlaceholderTemplate* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*  pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        ;
        auto&  inputParms = a_pInput->getTemplateSignature()->getTemplateParameters();
        size_t count = inputParms.size();
        if (pArgument->isTemplateDependant() &&
            pArgument->getMetaClass() == PHANTOM_CLASSOF(TemplateDependantTemplateInstance))
        {
            TemplateDependantTemplateInstance* pTDTI = static_cast<TemplateDependantTemplateInstance*>(pArgument);
            /// Both template template parameter and template argument has same
            /// parameter/argument count
            if (pTDTI->getTemplate()->getTemplateSignature()->getTemplateParameters().size() == count)
            {
                size_t i = 0;
                for (; i < count; ++i)
                {
                    TemplateParameter* pParameter = inputParms[i];
                    LanguageElement*   pArgument = pTDTI->getArguments()[i];
                    if (pArgument == nullptr)
                        pArgument = pParameter->getDefaultArgument();
                    if (pArgument == nullptr)
                        return;
                    if (!(templateArgumentDeduction(pParameter->getPlaceholder()->asSymbol(), pArgument, deductions)))
                        return;
                }
                pDeducedArgument = pArgument;
            }
        }
        if (ClassType* pClassTypeArgument = pArgument->asClassType())
        {
            TemplateSpecialization* pSpec =
            pClassTypeArgument->getOwner() ? pClassTypeArgument->getOwner()->asTemplateSpecialization() : nullptr;
            if (pSpec)
            {
                /// Both template template parameter and template argument has same
                /// parameter/argument count
                if (pSpec->getTemplateSignature()->getTemplateParameters().size() == count)
                {
                    size_t i = 0;
                    for (; i < count; ++i)
                    {
                        LanguageElement*   pArgument = pSpec->getArgument(i);
                        TemplateParameter* pParameter = inputParms[i];
                        Symbol*            pPlaceholder = pParameter->getPlaceholder()->asSymbol();
                        if (!(templateArgumentDeduction(pPlaceholder, pArgument, deductions)))
                            return;
                    }
                    pDeducedArgument = pClassTypeArgument;
                }
            }
        }
    }
        return;
    }
    visit(static_cast<Template*>(a_pInput), a_Data);
}
void Semantic::visit(PlaceholderType* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*  pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        Type*             pType = pArgument->asType();
        if (pType)
        {
            pDeducedArgument = pType;
        }
        if (pDeducedArgument)
        {
            auto found = deductions.find(a_pInput);
            if (found != deductions.end())
            {
                if (!(found->second->isSame(pDeducedArgument)))
                {
                    pDeducedArgument = nullptr;
                    return;
                }
            }
            deductions[a_pInput] = pDeducedArgument;
        }
    }
    break;

    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView in_name = *(StringView*)a_Data.in[0];
        Symbols&   out_candidates = *(Symbols*)a_Data.out[0];
        out_candidates.push_back(New<TemplateDependantElement>(a_pInput, in_name));
    }
        return;
    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        LanguageElement*            pArgument = in_TemplateSubstitution.getArgument(a_pInput);
        if (!pArgument)
        {
            if (in_TemplateSubstitution.getInstantiation() == nullptr)
                return;
            if (in_TemplateSubstitution.getInstantiation() == a_pInput->getEnclosingTemplateSpecialization())
            {
                CxxSemanticError("'%s' : template parameter cannot be deduced from arguments", FormatCStr(a_pInput));
            }
            else
            {
                pResolved = a_pInput;
            }
            return;
        }
        if (pArgument)
        {
            pResolved = pArgument->asType();
            if (pResolved == nullptr)
            {
                CxxSemanticError("'%s' : not a type", FormatCStr(pArgument));
            }
        }
        else
            pResolved = a_pInput;
    }
        return;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstantiated = *(LanguageElement**)a_Data.out[0];
        pInstantiated = New<PlaceholderType>(a_pInput->getName());
    }
        return;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}

void Semantic::visit(PlacementNewExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnerGuard<Expression>      pPlaceExp = o_instantiateT(Expression, a_pInput->getPlaceExpression());
        if (pPlaceExp == nullptr)
            return;
        OwnerGuard<Expression> pCtorExp = o_instantiateT(Expression, a_pInput->getConstructionExpression());
        if (pCtorExp == nullptr)
            return;
        Expression* pPlaceConv =
        CxxSemanticConversionNE(pPlaceExp, pCtorExp->getValueType()->removeRValueReference()->makePointer());
        if (pPlaceConv == nullptr)
            return;
        pInstanciated = New<PlacementNewExpression>(pPlaceConv, pCtorExp.take()->loadRValueReferenceExpression());
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(PrimitiveType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void Semantic::visit(Property* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ToExpression:
    {
        OwnerGuard<Expression> in_pLeftExpression = *(Expression**)a_Data.in[0];
        if (in_pLeftExpression == nullptr)
            return;
        Expression*&     out_pExpression = *(Expression**)a_Data.out[0];
        LanguageElement* in_pContextScope = nullptr;
        Type*            pOwnerType = a_pInput->getOwnerClassType();
        if (in_pLeftExpression->getValueType()->asReference() == nullptr)
        {
            CxxSemanticErrorReturnIf(in_pLeftExpression->getValueType()->removeQualifiers()->asClassType() == nullptr,
                                     "'%s' : expected class-type", FormatCStr(in_pLeftExpression->getValueType()));
            in_pLeftExpression = New<RValueReferenceExpression>(in_pLeftExpression->clone(getSource()));
        }
        Expression* pConv =
        CxxSemanticConversion(in_pLeftExpression, in_pLeftExpression->getValueType()->replicate(pOwnerType));
        if (pConv == nullptr)
            return;
        in_pLeftExpression.take();
        out_pExpression = New<PropertyExpression>(pConv, a_pInput);
    }
        return;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];

        ClassType* pInstanceClassType = static_cast<ClassType*>(in_pContextScope);
        PHANTOM_SEMANTIC_ASSERT(pInstanceClassType->asClassType());

        switch (in_eClassBuildState)
        {
        case e_ClassBuildState_Members:
        {
            CxxSemanticCheckSymbolName(a_pInput->getName(), pInstanceClassType);
            Type* pValueType = o_resolveT(Type, a_pInput->getValueType());
            if (pValueType == nullptr)
                return;
            Property* pInstanceProperty =
            New<Property>(pValueType, a_pInput->getName(), a_pInput->getFilterMask(), a_pInput->getModifiers());
            pInstanceClassType->addProperty(pInstanceProperty);
            if (a_pInput->getSet())
            {
                pInstanceProperty->addSet(a_pInput->getSet()->getName());
            }

            if (a_pInput->getGet())
            {
                pInstanceProperty->addGet(a_pInput->getGet()->getName());
            }

            pInstanciated = pInstanceProperty;
        }
        break;
        case e_ClassBuildState_Blocks:
        {
            Property* pInstanceProperty = o_findT(Property, a_pInput);
            if (pInstanceProperty == nullptr)
                return;
            if (a_pInput->getSet())
            {
                o_mapT(a_pInput->getSet(), pInstanceProperty->getSet());
                o_mapT(a_pInput->getSet()->getBlock(), addBlock(pInstanceProperty->getSet(), true));
                o_mapT(a_pInput->getSet()->getParameters().front(),
                       pInstanceProperty->getSet()->getParameters().front());
                o_instantiateT(Method, a_pInput->getSet()->getBlock());
            }

            if (a_pInput->getGet())
            {
                o_mapT(a_pInput->getGet(), pInstanceProperty->getGet());
                o_mapT(a_pInput->getGet()->getBlock(), addBlock(pInstanceProperty->getGet(), true));
                o_instantiateT(Method, a_pInput->getGet()->getBlock());
            }
        }
        break;
        }
    }
        return;
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(PropertyExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_HasSideEffect:
    {
        *(bool*)a_Data.out[0] = true;
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Property*                   pProp = o_resolveT(Property, a_pInput->getProperty());
        if (pProp == nullptr)
            return;
        OwnerGuard<Expression> pExp = o_instantiateT(Expression, a_pInput->getObjectExpression());
        if (pExp == nullptr)
            return;
        Expression* pConv = convertIfNotEqual(
        pExp, a_pInput->getObjectExpression()->getValueType()->replicate(pProp->getOwnerClassType()), in_pContextScope);
        if (pConv == nullptr)
            return;
        pInstanciated = New<PropertyExpression>(pConv, pProp);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(Reference* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void Semantic::visit(ReturnStatement* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (a_pInput->getExpression())
        {
            OwnerGuard<Expression> pExp = o_instantiateT(Expression, a_pInput->getExpression());
            if (pExp == nullptr)
                return;
            Expression* pConv =
            CxxSemanticConversionNE(pExp, in_pContextScope->asBlock()->getSubroutine()->getReturnType());
            if (pConv == nullptr)
                return;
            pInstanciated = New<ReturnStatement>(pConv);
        }
        else
            pInstanciated = New<ReturnStatement>();
    }
        return;
    }
    visit(static_cast<Statement*>(a_pInput), a_Data);
}
void Semantic::visit(RValueReference* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        Type*           pArgument = *(Type* const*)a_Data.in[0];
        Type*&          pDeducedType = *(Type**)a_Data.out[0];
        PlaceholderMap& deductions = *(PlaceholderMap*)a_Data.out[1];

        bool bUniversalReference = a_pInput->getReferencedType()->asPlaceholder() !=
        nullptr; /// T&& represents a universal reference => it can be deduced as lvalue& or
                 /// rvalue&& reference but T const&& is not an universal reference

        RValueReference* pRValueReferenceArgument = pArgument->asRValueReference();
        if (pRValueReferenceArgument)
        {
            Type* pReferencedDeducedType = templateArgumentDeduction(
            a_pInput->getReferencedType(), pRValueReferenceArgument->getReferencedType(), deductions);
            if (pReferencedDeducedType)
            {
                pDeducedType = pReferencedDeducedType->makeRValueReference();
            }
        }
        else if (bUniversalReference) /// if an universal reference (T&&) we can accept lvalue
                                      /// references
        {
            LValueReference* pLValueReferenceArgument = pArgument->asLValueReference();
            if (pLValueReferenceArgument)
            {
                Type* pReferencedDeducedType = templateArgumentDeduction(
                a_pInput->getReferencedType(), pLValueReferenceArgument->getReferencedType(), deductions);
                if (pReferencedDeducedType)
                {
                    pDeducedType = pReferencedDeducedType->makeLValueReference();
                }
            }
        }
    }
    break;
    case e_VisitorFunction_TypeConversion:
    {
        Type*                pOutput = *(Type* const*)a_Data.in[0];
        CastKind             castKind = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions iUserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        LanguageElement*     pContextScope = *(LanguageElement**)a_Data.in[3];
        bool                 in_bInitialize = *(bool*)a_Data.in[4];
        Conversion*&         conv = *(Conversion**)a_Data.out[0];
        if (pOutput->removeQualifiers()->isSame(a_pInput))
        {
            conv = newConv<DefaultConversionSequence>(
            newConv<StandardConversionSequence>(a_pInput)); /// use a standard conversion to allow comparison
                                                            /// with equivalent lvalue transformation : X& -> X
            return;                                         /// success X& -> X&
        }
        switch (castKind)
        {
        case CastKind::Const:
        {
            Type* pInputNoConst = a_pInput->removeAllQualifiers();
            Type* pOutputNoConst = pOutput->removeAllQualifiers();
            if (pInputNoConst->isSame(a_pInput) && pOutputNoConst->isSame(pOutput))
                return; /// input or output do not have const (or volatile ?) => const_cast fail
            if (pInputNoConst->isSame(pOutputNoConst)) /// no-cv input == no-cv output => const_cast ok
                conv = newConv<DefaultConversionSequence>(pInputNoConst);
            return; /// const_cast ends here (no more user defined or else can be done)
        }
        case CastKind::Reinterpret:
        {
            if (pOutput->asReference())
            {
                if ((pOutput->removeReference()->asConstType() != nullptr) ==
                    (a_pInput->removeReference()->asConstType() != nullptr))
                {
                    conv = newConv<DefaultConversionSequence>(a_pInput, (ConversionVal)pOutput);
                }
                return; /// success X& -> Y&
            }
            break;
        }
        case CastKind::Explicit:
        {
            if (pOutput->asReference())
            {
                conv = newConv<DefaultConversionSequence>(a_pInput, (ConversionVal)pOutput);
                return; /// success X& -> Y&
            }
            break;
        }
        }
        RValueReference* pOutputReference = pOutput->asRValueReference();
        if (pOutputReference == nullptr) /// output is not a r-value reference type => lvalue -> rvalue transformation
        {
            ERefType         refType = e_RefType_None;
            LValueReference* pOutputLValueReference = pOutput->asLValueReference();
            if (pOutputLValueReference)
            {
                if (pOutputLValueReference->getReferencedType()->asConstType())
                {
                    /// X&& -> Y const& -> lvalue
                    /// Search conversion X& -> const Y&
                    a_pInput->removeRValueReference()->addLValueReference()->visit(this, a_Data);
                    if (conv)
                    {
                        if (auto stdr = static_cast<DefaultConversionSequence*>(conv)->m_standard)
                        {
                            if (stdr->input != conv->input)
                            {
                                if (conv->semantic == nullptr)
                                {
                                    delete_<DefaultConversionSequence>(static_cast<DefaultConversionSequence*>(conv));
                                }
                                conv = nullptr;
                                return;
                            }
                            stdr->input = a_pInput;
                        }
                        PHANTOM_SEMANTIC_ASSERT(conv->input == a_pInput->removeRValueReference()->addLValueReference());
                        conv->input = a_pInput;
                        return; /// success X&& -> Y const&
                    }
                    return;
                }
                else
                {
                    /// X&& -> Y& equivalent to X& -> Y&
                    a_pInput->getReferencedType()->makeLValueReference()->visit(this, a_Data);
                    if (auto dconv = static_cast<DefaultConversionSequence*>(conv))
                    {
                        if (dconv->semantic)
                            dconv = dconv->clone(getSource());
                        // retransform input X& to X&&
                        dconv->input = a_pInput;
                        if (dconv->m_standard)
                            dconv->m_standard->input = a_pInput;

                        conv = dconv;
                    }
                    return;
                }
            }
            else
            {
                pOutput = pOutput->removeQualifiers();
                ClassType* pOutputClassType = pOutput->asClassType();
                if (pOutputClassType)
                {
                    if (iUserDefinedConversionsAllowed != UserDefinedFunctions::None)
                    {
                        /// X&& -> Y(X&&)
                        conv = userDefinedConversionByConstruction(
                        a_pInput, pOutputClassType,
                        (castKind == CastKind::Implicit) ? (iUserDefinedConversionsAllowed == UserDefinedFunctions::All)
                                                         : true,
                        in_bInitialize, pContextScope, e_RefType_None);
                        if (conv)
                            return;
                    }
                }
                else
                {
                    if (pOutput->isSame(a_pInput->getReferencedType()))
                    {
                        /// X&& -> X
                        conv =
                        newConv<DefaultConversionSequence>(newConv<LValueTransformationConversion>(a_pInput, nullptr));
                        return;
                    }
                }
                /// X&& -> Y
                a_pInput->getReferencedType()->visit(this, a_Data);
                if (conv)
                {
                    if (conv->semantic)
                        conv = conv->clone(getSource());
                    /// We found a conversion from X -> Y, we just add a
                    /// LValueTransformationConversion to it to obtain X& -> Y
                    static_cast<DefaultConversionSequence*>(conv)->addLValueTransform(nullptr);
                    return; /// success X& -> Y
                }
            }
        }
        else /// X&& -> Y&& (output is a reference type)
        {
            Type* pInputReferencedType = a_pInput->getReferencedType();              /// X (const)
            Type* pOutputReferencedType = pOutputReference->getReferencedType();     /// Y (const)
            Type* pInputReferencedTypeNoConst = pInputReferencedType->removeConst(); /// Y
            Type* pOutputReferencedTypeNoConst = pOutputReferencedType;              /// Y

            /// X&& -> Y&&
            std::pair<Type*, Type*> constAdjustment;
            int                     adjustmentCount = 0;
            if (pOutputReferencedType->asConstType())
            {
                /// X&& -> Y const&&
                pOutputReferencedTypeNoConst = pOutputReferencedType->removeConst(); /// Y
                if (pOutputReferencedTypeNoConst->isA(pInputReferencedTypeNoConst))
                {
                    /// X and Y are related => make const adjustment
                    RValueReference* adjustmentInput = pOutputReferencedTypeNoConst->makeRValueReference();
                    constAdjustment.first = adjustmentInput;
                    /// merge const adjustments
                    constAdjustment.second = pOutputReference;
                    adjustmentCount++;
                    pOutputReference = adjustmentInput;

                    /// if X == Y => just a const adjustement
                    if (pOutputReferencedTypeNoConst->isSame(pInputReferencedTypeNoConst))
                    {
                        conv = newConv<DefaultConversionSequence>(newConv<QualificationAdjustConversion>(
                        constAdjustment.first, constAdjustment.second, adjustmentCount));
                        return; /// success X&& -> X const&&
                    }
                }
                //                     else
                //                     {
                //                         /// X and Y are not related => it's possibly a const ref
                //                         conversion
                //                         /// Search conversion X& -> Y and, if found, add const
                //                         ref conversion then to do X& -> Y const& a_Data.in[0] =
                //                         &pOutputReferencedTypeNoConst; // modify output in
                //                         visitor data a_pInput->getReferencedType()->visit(this,
                //                         a_Data); if(conv)
                //                         {
                //                             static_cast<DefaultConversionSequence*>(conv)->addConstRef();
                //                             return; /// success X&& -> Y const&
                //                         }
                //                         return;
                //                     }
            }
            else if (a_pInput->getReferencedType()->asConstType())
            {
                return; /// failure X const&& -/-> Y&&
            }
            if ((pInputReferencedTypeNoConst->asClass() != nullptr) &&
                (pOutputReferencedTypeNoConst->asClass() != nullptr))
            {
                lang::Class* pInputClass = static_cast<lang::Class*>(pInputReferencedTypeNoConst);
                lang::Class* pOutputClass = static_cast<lang::Class*>(pOutputReferencedTypeNoConst);
                size_t       level = pInputClass->getInheritanceLevelFromBase(pOutputClass);
                if (level != ~size_t(0)) // level != -1 => inheritance relation => pointers are related
                {
                    auto* pPtr = newConv<PointerConversion>(a_pInput, pOutputReference, level,
                                                            pInputClass->getBaseClassOffsetCascade(pOutputClass));
                    if (adjustmentCount)
                        conv = newConv<DefaultConversionSequence>(
                        pPtr,
                        newConv<QualificationAdjustConversion>(constAdjustment.first, constAdjustment.second,
                                                               adjustmentCount));
                    conv = newConv<DefaultConversionSequence>(pPtr);
                    /// success Derived&& -> Base(const)&&
                    return;
                }
            }
        }
    }
    break;

    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        Type*                       pNewReferencedType = o_resolveT(Type, a_pInput->getReferencedType());
        pResolved = pNewReferencedType ? pNewReferencedType->makeRValueReference() : nullptr;
    }
        return;
    }
    visit(static_cast<Reference*>(a_pInput), a_Data);
}
void Semantic::visit(RValueToConstLValueExpression* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(RValueReferenceExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ExpressionConversion:
    {
        Type*    in_pOutput = *(Type* const*)a_Data.in[0];
        CastKind in_ConversionType = *(CastKind const*)a_Data.in[1];
        if (in_ConversionType != CastKind::Implicit)
            break;
        if (in_pOutput->removeQualifiers()->isSame(a_pInput->getValueType()->removeReference()->removeQualifiers()))
        {
            *(Expression**)a_Data.out[0] = a_pInput->load(getSource());
            return;
        }
    }
    }

    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(Scope* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_SubroutineResolution:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        const TypesView*                    in_pFunctionTypes = (const TypesView*)a_Data.in[2];
        Modifiers                           in_Qualifiers = *(Modifiers*)a_Data.in[3];
        LanguageElement*                    in_pContextScope = *(LanguageElement**)a_Data.in[4];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];
        if (in_pTemplateArguments)
        {
            /// TODO : template subroutines
            out_pResult = nullptr;
            return; // PHANTOM_SEMANTIC_ASSERT_NO_IMPL();
        }
        Subroutine* pSubroutine = a_pInput->getSubroutine(in_name, *in_pFunctionTypes, in_Qualifiers);
        if (pSubroutine)
        {
            out_pResult = pSubroutine;
            return;
        }
    }
    break;

    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView       in_name = *(StringView*)a_Data.in[0];
        Symbols&         out_candidates = *(Symbols*)a_Data.out[0];
        LanguageElement* in_pScope = *(LanguageElement**)a_Data.in[1];
        LanguageElement* pInput = a_pInput->asLanguageElement();

        for (auto pElem : a_pInput->getVariables())
        {
            auto vis = pElem->getVisibility();
            if (vis == Visibility::Private ||
                (vis == Visibility::Protected && in_pScope != pInput && !in_pScope->hasOwnerCascade(pInput)))
                continue;
            if (pElem->getName() == in_name)
                return out_candidates.push_back(pElem);
        }

        for (auto pElem : a_pInput->getTypes())
        {
            auto vis = pElem->getVisibility();
            if (vis == Visibility::Private ||
                (vis == Visibility::Protected && in_pScope != pInput && !in_pScope->hasOwnerCascade(pInput)))
                continue;
            if (pElem->getName() == in_name)
                return out_candidates.push_back(pElem);
        }

        for (auto pElem : a_pInput->getTemplates())
        {
            auto vis = pElem->getVisibility();
            if (vis == Visibility::Private ||
                (vis == Visibility::Protected && in_pScope != pInput && !in_pScope->hasOwnerCascade(pInput)))
                continue;
            if (pElem->getName() == in_name)
                return out_candidates.push_back(pElem);
        }

        for (auto pElem : a_pInput->getFunctions())
        {
            auto vis = pElem->getVisibility();
            if (vis == Visibility::Private ||
                (vis == Visibility::Protected && in_pScope != pInput && !in_pScope->hasOwnerCascade(pInput)))
                continue;
            if (pElem->getName() == in_name)
                out_candidates.push_back(pElem);
        }

        for (auto pElem : a_pInput->getAliases())
        {
            auto vis = pElem->getVisibility();
            if (vis == Visibility::Private ||
                (vis == Visibility::Protected && in_pScope != pInput && !in_pScope->hasOwnerCascade(pInput)))
                continue;
            if (pElem->getName() == in_name)
            {
                if (Symbol* pAliased = pElem->getAliasedSymbol())
                {
                    out_candidates.push_back(pElem);
                    if (!pAliased->isTemplateDependant() && pAliased->asSubroutine() == nullptr)
                        return;
                }
            }
        }
    }
    break;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (pInstanciated)
        {
            Scope* pInstanceScope = pInstanciated->asScope();
            for (auto pFunction : a_pInput->getFunctions())
            {
                if (!(pFunction->testFlags(PHANTOM_R_FLAG_IMPLICIT)))
                {
                    o_instantiateT(Function, pFunction);
                }
            }
            for (auto pVar : a_pInput->getVariables())
            {
                if (!(pVar->testFlags(PHANTOM_R_FLAG_IMPLICIT)))
                {
                    o_instantiateT(Variable, pVar);
                }
            }
            for (auto pAlias : a_pInput->getAliases())
            {
                if (!(pAlias->testFlags(PHANTOM_R_FLAG_IMPLICIT)))
                {
                    o_instantiateT(Alias, pAlias);
                }
            }
            for (auto pType : a_pInput->getTypes())
            {
                if (!(pType->testFlags(PHANTOM_R_FLAG_IMPLICIT)))
                {
                    o_instantiateT(Type, pType);
                }
            }
            for (auto pTemplate : a_pInput->getTemplates())
            {
                if (!(pTemplate->testFlags(PHANTOM_R_FLAG_IMPLICIT)))
                {
                    o_instantiateT(Template, pTemplate);
                }
            }
            for (auto pTemplateSpec : a_pInput->getTemplateSpecializations())
            {
                if (!(pTemplateSpec->testFlags(PHANTOM_R_FLAG_IMPLICIT)))
                {
                    o_instantiateT(TemplateSpecialization, pTemplateSpec);
                }
            }
        }
    }
        return;
    }
}

void Semantic::visit(SequentialContainerClass* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        PHANTOM_SEMANTIC_ASSERT(false, "SequentialContainerClass abstract class");
    }
    break;
    }
    visit(static_cast<ContainerClass*>(a_pInput), a_Data);
}

void Semantic::visit(MapClass* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (in_eClassBuildState == e_ClassBuildState_None)
        {
            PHANTOM_SEMANTIC_ASSERT(pInstanciated == nullptr);
            pInstanciated = New<MapClass>(a_pInput->getName(), a_pInput->getModifiers());
            return;
        }
        return visit(static_cast<Class*>(a_pInput), a_Data);
    }
    break;
    }
    visit(static_cast<ContainerClass*>(a_pInput), a_Data);
}

void Semantic::visit(SetClass* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (in_eClassBuildState == e_ClassBuildState_None)
        {
            PHANTOM_SEMANTIC_ASSERT(pInstanciated == nullptr);
            pInstanciated = New<SetClass>(a_pInput->getName(), a_pInput->getModifiers());
            return;
        }
        return visit(static_cast<Class*>(a_pInput), a_Data);
    }
    break;
    }
    visit(static_cast<ContainerClass*>(a_pInput), a_Data);
}

void Semantic::visit(VectorClass* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (in_eClassBuildState == e_ClassBuildState_None)
        {
            PHANTOM_SEMANTIC_ASSERT(pInstanciated == nullptr);
            pInstanciated = New<VectorClass>(a_pInput->getName(), a_pInput->getModifiers());
            return;
        }
        return visit(static_cast<Class*>(a_pInput), a_Data);
    }
    break;
    }
    visit(static_cast<SequentialContainerClass*>(a_pInput), a_Data);
}
void Semantic::visit(Signature* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnersGuard<Parameters>     parameters;
        for (auto* pUnresolvedParam : a_pInput->getParameters())
        {
            if (pUnresolvedParam->isTemplateDependant())
            {
                // Parameter pack expansion
                if (auto pPackExp = Object::Cast<TemplateParameterPackTypeExpansion>(pUnresolvedParam->getValueType()))
                {
                    auto             pPackArg = pPackExp->getPackArgument();
                    LanguageElements variadicArguments;
                    auto             variadicSubs =
                    in_TemplateSubstitution.splitVariadics(pPackExp->getPackArgument(), variadicArguments);
                    for (size_t i = 0; i < variadicArguments.size(); ++i)
                    {
                        variadicSubs.substituteVariadic(pPackArg, variadicArguments[i]);
                        Parameter* pParam = SemanticPrivate::SemanticInstantiateTemplateElement<Parameter>(
                        this, pUnresolvedParam, variadicSubs, in_eClassBuildState, in_pContextScope, a_Data.flags,
                        &LanguageElement::asParameter);
                        if (pParam == nullptr)
                            return;
                        char idxBuf[8];
                        sprintf(idxBuf, "$%d", int(i));
                        pParam->setName(pParam->getName() + StringView(idxBuf));
                        PHANTOM_SEMANTIC_ASSERT(!pParam->isTemplateDependant());
                        parameters.push_back(pParam);
                    }
                    continue;
                }
            }
            Parameter* pParam = o_instantiateT(Parameter, pUnresolvedParam);
            if (pParam == nullptr)
                return;
            parameters.push_back(pParam);
        }
        Type* pReturnType = o_resolveT(Type, a_pInput->getReturnType());
        if (pReturnType == nullptr)
            return;
        pInstanciated = New<Signature>(pReturnType, parameters, a_pInput->getModifiers());
    }
        return;
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(Source* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_QualifiedNameLookup:
        visit(static_cast<Scope*>(a_pInput), a_Data);
        if ((*(Symbols*)a_Data.out[0]).size())
            return;

    case e_VisitorFunction_UnqualifiedLookup:
    case e_VisitorFunction_QualifiedLookup:
    case e_VisitorFunction_UnqualifiedNameLookup:

        visit(static_cast<Symbol*>(a_pInput), a_Data);
        break;

    case e_VisitorFunction_Access:
    {
        Symbol* in_pSymbol = *(Symbol**)a_Data.in[0];
        bool&   out_bResult = *(bool*)a_Data.out[0];
        Alias*  pAlias = in_pSymbol->asAlias();
        Source* pImportedSource;
        if (pAlias && pAlias->getAliasedSymbol() && (pImportedSource = pAlias->getAliasedSymbol()->asSource()))
        {
            out_bResult = pAlias->isPublic() || (pAlias->getSource() == a_pInput && pAlias->isProtected());
            return;
        }
        else
            visit(static_cast<Symbol*>(a_pInput), a_Data);
    }
    break;
    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        *(LanguageElement**)a_Data.out[0] = a_pInput;
    }
    break;
    }
}
void Semantic::visit(Statement* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Evaluable*>(a_pInput), a_Data);
}
void Semantic::visit(StaticAssert* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        if (pInstanciated == nullptr)
        {
            Expression* pExpression = o_instantiateT(Expression, a_pInput->getExpression());
            if (pExpression == nullptr)
                return;
            bool result = false;
            {
                StaticExecutionContext ctx;
                pExpression->load(ctx, &result);
            }
            // #pragma message(__FILE__ PHANTOM_PP_QUOTE(__LINE__) ": pExpression is leaked here,
            // find a better way")
            pInstanciated = in_pContextScope->New<StaticAssert>(
            pExpression,
            a_pInput->getText()); // we dont care if destroyed, it's just to check if its not already done
            CxxSemanticErrorReturnIf(!result, "%s", (const char*)a_pInput->getText().data());
        }
    }
        return;
    }
    visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}
void Semantic::visit(StringLiteralExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
        o_instantiate_template_clone;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(Structure* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Structure*                  pInstanceStructure = New<Structure>(a_pInput->getName(), a_pInput->getModifiers());
        pInstanciated = pInstanceStructure;
        Scope* pScope = in_pContextScope->asScope();
        if (pScope)
            pScope->addType(pInstanceStructure);
        visit(static_cast<ClassType*>(a_pInput), a_Data);
    }
        return;
    }
    visit(static_cast<ClassType*>(a_pInput), a_Data);
}
void Semantic::visit(StructureExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Type*                       pStructureType = o_resolveT(Type, a_pInput->getEvalType());
        Fields                      resolvedFields;
        for (auto pField : a_pInput->getFields())
        {
            resolvedFields.push_back(o_resolveT(Field, pField));
        }
        PHANTOM_SEMANTIC_ASSERT(resolvedFields.size() == a_pInput->getValues().size());
        OwnersGuard<Expressions> expressions;
        for (size_t i = 0; i < resolvedFields.size(); ++i)
        {
            Expression* pExpression = o_instantiateT(Expression, a_pInput->getValues()[i]);
            if (pExpression == nullptr)
                return;
            Expression* pConv = CxxSemanticConversionNE(pExpression, resolvedFields[i]->getValueType());
            if (pConv == nullptr)
            {
                Delete(pExpression);
                return;
            }
            expressions->push_back(pConv);
        }
        pInstanciated = New<StructureExpression>(pStructureType->asStructure(), resolvedFields, expressions);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(Subroutine* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_Access:
    {
        if (a_pInput->getOwner() && a_pInput->getOwner()->asClassType())
            a_pInput->getOwner()->visit(this, a_Data);
    }
        return;
    case e_VisitorFunction_QualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];
        if (!in_pTemplateArguments && !in_pFunctionArguments)
        {
            for (auto p : a_pInput->getSignature()->getParameters())
            {
                if ((p)->getName() == in_name)
                {
                    out_pResult = p;
                    return;
                }
            }
        }
    }
    break;
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(SubroutinePointerExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Subroutine*                 pSubroutine = o_resolveT(Subroutine, a_pInput->getSubroutine());
        if (pSubroutine == nullptr)
            return;
        pInstanciated = New<SubroutinePointerExpression>(in_pContextScope->getSource(), pSubroutine);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(BinaryOperationExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        Expression*                 pLeft = o_instantiateT(Expression, a_pInput->getLeftOperandExpression());
        if (pLeft == nullptr)
            return;
        Expression* pRight = o_instantiateT(Expression, a_pInput->getRightOperandExpression());
        if (pRight == nullptr)
            return;
        pInstanciated = solveBinaryOperator(a_pInput->getOperatorName(), pLeft, pRight, in_pContextScope);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(UnaryPostOperationExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnerGuard<Expression>      pExpression = o_instantiateT(Expression, a_pInput->getOperandExpression());
        if (pExpression == nullptr)
            return;
        pInstanciated = solveUnaryPostOperator(a_pInput->getOperatorName(), pExpression, in_pContextScope);
        if (pExpression == pInstanciated) // in case of silent "->"
            pExpression.take();
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(UnaryPreOperationExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnerGuard<Expression>      pExpression = o_instantiateT(Expression, a_pInput->getOperandExpression());
        if (pExpression == nullptr)
            return;
        pInstanciated = solveUnaryPreOperator(a_pInput->getOperatorName(), pExpression, in_pContextScope);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(Template* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_IsViableCallCandidate:
    {
        if (auto pEmpty = a_pInput->getEmptyTemplateSpecialization())
        {
            if (auto pTemplated = pEmpty->getTemplated())
            {
                if (auto pSubr = pTemplated->asSubroutine())
                {
                    return pSubr->visit(this, a_Data);
                }
            }
        }
    }
    break;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];

        TemplateSignature* pInstanceSignature = o_instantiateT(TemplateSignature, a_pInput->getTemplateSignature());
        if (!pInstanceSignature)
            return;
        Template* pInstanceTemplate = New<Template>(pInstanceSignature, a_pInput->getName());

        auto pInstanceTemplated = o_instantiateT(Symbol, a_pInput->getEmptyTemplateSpecialization()->getTemplated());
        if (!pInstanceTemplated)
            return;
        pInstanceTemplate->getEmptyTemplateSpecialization()->setTemplated(pInstanceTemplated);

        in_pContextScope->asScope()->addTemplate(pInstanceTemplate);

        return;
    }
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(TemplateDependantArray* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ResolveTemplateDependencies:
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];

        Type* pType = o_resolveT(Type, a_pInput->getItemType());
        if (pType == nullptr)
            return;
        Expression* pCountExp = a_pInput->getItemCountExpression();

        if (pCountExp->isTemplateDependant())
            pCountExp = o_instantiateT(Expression, a_pInput->getItemCountExpression());

        if (pCountExp == nullptr)
            return;
        CxxSemanticErrorReturnIf(!pCountExp->isCompileTime(),
                                 "array extent expression expects a compile-time integer value");
        Expression* pConvCountExp = convert(pCountExp, PHANTOM_TYPEOF(size_t));
        if (pConvCountExp == nullptr)
        {
            CxxSemanticError("array extent expression expects a compile-time integer value");
            Delete(pCountExp);
            return;
        }
        bool isSame = false;
        {
            StaticExecutionContext ctx;
            size_t                 count = 0;
            pConvCountExp->load(ctx, &count);
            Delete(pConvCountExp);

            if (count == 0)
            {
                CxxSemanticError("invalid zero-sized array");
                return;
            }
            pResolved = pType->makeArray(count);
            if (!pResolved)
            {
                CxxSemanticError("type cannot be used in array");
            }
        }
        return;
    }
    break;
    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        Type*                pArgument = *(Type* const*)a_Data.in[0];
        Type*&               pDeducedType = *(Type**)a_Data.out[0];
        PlaceholderMap&      deductions = *(PlaceholderMap*)a_Data.out[1];
        PlaceholderConstant* pDeductibleConstant = a_pInput->getDeductibleConstant();
        if (pDeductibleConstant)
        {
            Array* pArrayArgument = pArgument->asArray();
            if (pArrayArgument)
            {
                auto found = deductions.find(pDeductibleConstant);
                if (found != deductions.end())
                {
                    // explicitly passed as argument or already deduced
                    Expression* pOldExpression = found->second->asExpression();
                    Expression* pNewExpression = a_pInput->getItemCountExpression();
                    if (pOldExpression && pOldExpression->isCompileTime() && pNewExpression &&
                        pNewExpression->isCompileTime() &&
                        pOldExpression->getValueType()->removeReference()->isSame(
                        pNewExpression->getValueType()->removeReference()))
                    {
                        Expression* pExpression = solveBinaryOperator("==", pOldExpression, pNewExpression, nullptr);
                        PHANTOM_SEMANTIC_ASSERT(pExpression->isCompileTime());
                        bool isSame = false;
                        {
                            StaticExecutionContext ctx;
                            pExpression->load(ctx, &isSame);
                        }
                        Delete(pExpression);
                        if (!isSame)
                            return;
                    }
                    else
                        return;
                }
                else
                    deductions[pDeductibleConstant] =
                    New<ConstantExpression>(newConstant<size_t>(pArrayArgument->getItemCount()));
                pDeducedType = pArgument;
            }
        }
    }
    break;
    }
    visit(static_cast<Array*>(a_pInput), a_Data);
}

void Semantic::visit(TemplateDependantDeclType* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ResolveTemplateDependencies:
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];

        Expression* pExpression = o_resolveT(Expression, a_pInput->getExpression());
        if (pExpression == nullptr)
            return;

        Type* pType = pExpression->getValueType();

        if (Type* pAutoType = a_pInput->getAutoType())
        {
            PlaceholderMap deductions;
            Type*          pDeducedArgumentType = callTemplateArgumentDeduction(pAutoType, pType, deductions);
            CxxSemanticErrorReturnIf(pDeducedArgumentType == nullptr,
                                     "auto : cannot deduce arguments type"); // deduction succeeded
            PHANTOM_SEMANTIC_ASSERT(deductions.size() == 1);
            Type* pDeducedAutoType = deductions.begin()->second->asType();
            pType = pType->replicate(pDeducedAutoType);
        }
        pResolved = pType;
    }
        return;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void Semantic::visit(TemplateDependantElement* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ToExpression:
    {
        Expression*  in_pLeftExpression = *(Expression**)a_Data.in[0];
        Expression*& out_pExpression = *(Expression**)a_Data.out[0];
        out_pExpression = New<TemplateDependantExpression>(a_pInput);
    }
        return;
    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView in_name = *(StringView*)a_Data.in[0];
        Symbols&   out_candidates = *(Symbols*)a_Data.out[0];
        out_candidates.push_back(New<TemplateDependantElement>(a_pInput, in_name));
    }
        return;
    case e_VisitorFunction_QualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];
        out_pResult = New<TemplateDependantElement>(a_pInput, in_name, in_pTemplateArguments,
                                                    *(OptionalArrayView<LanguageElement*>*)&in_pFunctionArguments);
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        if (a_pInput->getName() == "sizeof" || a_pInput->getName() == "alignof")
        {
            LanguageElement* pArg = o_resolveT(LanguageElement, a_pInput->getFunctionArguments()->front());
            Type*            pType = pArg->asType();
            if (pType == nullptr)
            {
                Expression* pExp = pArg->asExpression();
                PHANTOM_SEMANTIC_ASSERT(pExp);
                pType = pExp->getValueType()->removeReference()->removeQualifiers();
            }
            pResolved = (New<ConstantExpression>(
            newConstant(a_pInput->getName()[0] == 's' ? pType->getSize() : pType->getAlignment())));
            return;
        }
        bool             bDeepResolution = true;
        LanguageElement* pLHS = in_pContextScope;
        if (a_pInput->getLeft())
        {
            bDeepResolution = false;
            pLHS = o_resolveT(LanguageElement, a_pInput->getLeft());
            if (pLHS == nullptr)
            {
                return;
            }
        }
        OwnersGuard<Expressions>      functionArguments;
        OwnersGuard<Types>            functionTypes;
        OwnersGuard<LanguageElements> templateArguments;
        Expressions*                  pFunctionArguments = nullptr;
        Types*                        pFunctionTypes = nullptr;
        LanguageElements*             pTemplateArguments = nullptr;
        bool                          bExpressions = true;
        bool                          bTypes = true;
        if (a_pInput->getFunctionArguments())
        {
            for (auto pArgument : *a_pInput->getFunctionArguments())
            {
                if (pArgument->isTemplateDependant())
                {
                    if (auto pPackExp = Object::Cast<TemplateParameterPackExpressionExpansion>(pArgument))
                    {
                        auto   pExpParam = pPackExp->getExpandedParameter();
                        auto&  templateDepParams = static_cast<Signature*>(pExpParam->getOwner())->getParameters();
                        size_t templateDepParamsCount = templateDepParams.size();
                        for (size_t i = 0; i < templateDepParamsCount; ++i)
                        {
                            if (templateDepParams[i] == pExpParam)
                            {
                                Subroutine* pInstanceSubroutine =
                                o_resolveT(Subroutine, in_pContextScope->getEnclosingSubroutine());
                                auto&  resolvedParams = pInstanceSubroutine->getParameters();
                                size_t resolvedParamSize = resolvedParams.size();
                                if (resolvedParamSize < templateDepParamsCount)
                                {
                                    // variadic expanded without arguments => we skip the function argument
                                    pExpParam = nullptr;
                                    break;
                                }
                                for (size_t p = i; p < i + (resolvedParamSize - templateDepParamsCount); ++p)
                                {
                                    templateInstantiations()[in_TemplateSubstitution.getInstantiation()][pExpParam] =
                                    resolvedParams[p];
                                    LanguageElement* pElement = o_instantiateT(LanguageElement, pArgument);
                                    if (pElement->asExpression() && bExpressions)
                                    {
                                        bTypes = false;
                                        functionArguments.push_back(pElement->asExpression());
                                        pFunctionArguments = &functionArguments.elements();
                                    }
                                    else
                                    {
                                        CxxSemanticError("invalid template variadic argument expression");
                                        return;
                                    }
                                }
                                break;
                            }
                        }
                        if (!pExpParam) // argument was skipped by variadic resolution
                            continue;
                    }
                }
                LanguageElement* pElement = o_resolveT(LanguageElement, pArgument);
                PHANTOM_SEMANTIC_ASSERT(pElement == nullptr || !pElement->isTemplateDependant());
                if (pElement == nullptr)
                {
                    return;
                }
                else if (pElement->asExpression() && bExpressions)
                {
                    bTypes = false;
                    functionArguments.push_back(pElement->asExpression());
                    pFunctionArguments = &functionArguments.elements();
                }
                else if (pElement->asType() && bTypes)
                {
                    bExpressions = false;
                    functionTypes.push_back(pElement->asType());
                    pFunctionTypes = &functionTypes.elements();
                }
                else
                {
                    CxxSemanticError("invalid template argument expression");
                }
            }
            if (pFunctionArguments == nullptr)
                pFunctionArguments = &functionArguments.elements();
        }
        if (a_pInput->getTemplateArguments())
        {
            pTemplateArguments = &templateArguments.elements();
            for (auto pTemplateArg : *a_pInput->getTemplateArguments())
            {
                LanguageElement* pElement = o_resolveT(LanguageElement, pTemplateArg);
                if (pElement == nullptr)
                {
                    // CxxSemanticError("invalid template argument in template dependant expression
                    // resolution");
                    return;
                }
                templateArguments.push_back(pElement);
            }
        }
        if (pFunctionArguments)
        {
            if (a_pInput->getName()[0] == '~')
            {
                if (pLHS)
                {
                    if (Expression* pLHSExp = pLHS->asExpression())
                    {
                        if (pLHSExp->getValueType()->removeReference()->removeQualifiers()->asClassType() == nullptr)
                        {
                            LanguageElement* pType = silentUnqualifiedLookup(a_pInput->getName().substr(1),
                                                                             MakeOptionalArrayView(pTemplateArguments),
                                                                             NullOpt, in_pContextScope, nullptr);
                            CxxSemanticErrorReturnIf(pType !=
                                                     pLHSExp->getValueType()->removeReference()->removeQualifiers(),
                                                     "invalid named destructor on incompatible type");
                            pResolved = pLHSExp; // destructor called on a fundamental type => ignored => result is
                                                 // original left-hand-side expression
                            return;
                        }
                    }
                }
                // for destructors, we are looking for a local scope type (which furthermore might
                // have be an alias, like ~value_type, where 'value_type' is an alias), so we
                // perform an unqualified lookup in every case
                pResolved =
                silentUnqualifiedLookup(a_pInput->getName().substr(1), MakeOptionalArrayView(pTemplateArguments),
                                        NullOpt, in_pContextScope, nullptr);
                if (pResolved == nullptr)
                {
                    ErrorTemplateInstantiationFailure(a_pInput, pTemplateArguments, pFunctionArguments);
                    return;
                }
                Type* pType = pResolved->asType();
                if (pType == nullptr)
                {
                    CxxSemanticError("'%s' : type expected during template instantiation", FormatCStr(a_pInput));
                    return;
                }

                pType = pType->removeQualifiers(); // remove potential qualifiers

                // we then perform a classical expression lookup if necessary
                pResolved = bDeepResolution
                ? silentUnqualifiedLookup('~' + pType->getName(), MakeOptionalArrayView(pTemplateArguments),
                                          MakeOptionalArrayView(pFunctionArguments), in_pContextScope, nullptr)
                : silentQualifiedLookup(pLHS, '~' + pType->getName(), MakeOptionalArrayView(pTemplateArguments),
                                        MakeOptionalArrayView(pFunctionArguments), in_pContextScope, nullptr);
            }
            else
            {
                pResolved = bDeepResolution
                ? silentUnqualifiedLookup(a_pInput->getName(), MakeOptionalArrayView(pTemplateArguments),
                                          MakeOptionalArrayView(pFunctionArguments), in_pContextScope, nullptr)
                : silentQualifiedLookup(pLHS, a_pInput->getName(), MakeOptionalArrayView(pTemplateArguments),
                                        MakeOptionalArrayView(pFunctionArguments), in_pContextScope, nullptr);

                if (pResolved == nullptr)
                {
                    pResolved = silentUnqualifiedLookup(a_pInput->getName(), MakeOptionalArrayView(pTemplateArguments),
                                                        NullOpt, in_pContextScope, nullptr);
                    if (pResolved == nullptr)
                    {
                        ErrorTemplateInstantiationFailure(a_pInput, pTemplateArguments, pFunctionArguments);
                        return;
                    }
                    Type* pType = pResolved->asType();
                    if (pType == nullptr)
                    {
                        if (pResolved->asMethod())
                        {
                            pResolved = nullptr;
#pragma message(PHANTOM_TODO "store method candidates during lookup")
                            // we re-execute lookup to have the real error
                            if (bDeepResolution)
                            {
                                internalUnqualifiedLookup(
                                a_pInput->getName(), MakeOptionalArrayView(pTemplateArguments),
                                MakeOptionalArrayView(pFunctionArguments), in_pContextScope, nullptr);
                            }
                            else
                            {
                                internalQualifiedLookup(
                                pLHS, a_pInput->getName(), MakeOptionalArrayView(pTemplateArguments),
                                MakeOptionalArrayView(pFunctionArguments), in_pContextScope, nullptr);
                            }
                        }
                        else
                        {
                            pResolved = nullptr;
                            CxxSemanticError("'%s' : type expected during template instantiation",
                                             FormatCStr(a_pInput));
                        }
                        return;
                    }

                    pType = pType->removeQualifiers(); // remove potential qualifiers

                    // -- TRY type.type(...)

                    // we then perform a classical expression lookup if necessary
                    pResolved =
                    silentQualifiedLookup(pType, pType->getName(), MakeOptionalArrayView(pTemplateArguments),
                                          MakeOptionalArrayView(pFunctionArguments), in_pContextScope, nullptr);

                    if (Expression* pResolvedExp = phantom::Object::Cast<Expression>(pResolved))
                    {
                        if (pLHS)
                        {
                            if (Expression* pLHSExp = pLHS->asExpression())
                            {
                                if (pType->asClassType())
                                {
                                    if (ConstructorCallExpression* pCtorCall =
                                        phantom::Object::Cast<ConstructorCallExpression>(
                                        pResolvedExp->removeRValueReferenceExpression()))
                                    {
                                        if (pLHSExp->getValueType()->isSame(pType->addLValueReference()))
                                        {
                                            pResolved =
                                            New<PlacementNewExpression>(pLHSExp->address(getSource()),
                                                                        pResolvedExp->loadRValueReferenceExpression());
                                            return;
                                        }
                                    }
                                }
                                else
                                {
                                    pResolved = solveBinaryOperator(Operator::Assignment, pLHSExp, pResolvedExp);
                                }
                            }
                        }
                    }

                    // -- END type.type(...)
                }
            }

            if (pResolved == nullptr)
            {
                if (a_pInput->getName().empty() && pLHS && pFunctionArguments) // constructor call
                {
                    ClassType* pClass = pLHS->asClassType();
                    if (pClass == nullptr)
                    {
                        if (Expression* pExp = pLHS->asExpression())
                            pClass = pExp->getValueType()->removeReference()->removeQualifiers()->asClassType();
                    }
                    if (pClass)
                    {
                        CxxSemanticError("'%s' : no constructor found taking the given arguments",
                                         FormatCStr(pClass) /*, pTemplateArguments, pFunctionTypes*/);
                        return;
                    }
                }
                ErrorTemplateInstantiationFailure(a_pInput, pTemplateArguments, pFunctionArguments);
            }
            return;
        }
        else if (pFunctionTypes)
        {
            Subroutine* pSubroutine = bDeepResolution
            ? subroutineLookupCascade(pLHS, a_pInput->getName(), MakeOptionalArrayView(pTemplateArguments),
                                      *pFunctionTypes, a_pInput->getModifiers(), 0, in_pContextScope)
            : subroutineLookup(pLHS, a_pInput->getName(), MakeOptionalArrayView(pTemplateArguments), *pFunctionTypes,
                               a_pInput->getModifiers(), 0, in_pContextScope);
            if (pSubroutine)
            {
                pResolved = pSubroutine;
            }
            else
            {
                CxxSemanticError("'%s' : template subroutine lookup failure",
                                 FormatCStr(a_pInput) /*, pTemplateArguments, pFunctionTypes*/);
            }
            return;
        }
        pResolved = bDeepResolution
        ? unqualifiedLookup(a_pInput->getName(), MakeOptionalArrayView(pTemplateArguments), phantom::NullOpt,
                            in_pContextScope, nullptr)
        : qualifiedLookup(pLHS, a_pInput->getName(), MakeOptionalArrayView(pTemplateArguments), phantom::NullOpt,
                          in_pContextScope, nullptr);
        if (pResolved == nullptr)
        {
            ErrorTemplateInstantiationFailure(a_pInput, pTemplateArguments, nullptr);
        }
    }
        return;
    }
}

void Semantic::visit(TemplateDependantExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView in_name = *(StringView*)a_Data.in[0];
        Symbols&   out_candidates = *(Symbols*)a_Data.out[0];
        out_candidates.push_back(New<TemplateDependantElement>(a_pInput, in_name));
    }
    break;
    case e_VisitorFunction_ResolveTemplateDependencies:
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        a_pInput->getTemplateDependantElement()->visit(this, a_Data);
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(TemplateDependantTemplateInstance* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_QualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];
        out_pResult = New<TemplateDependantElement>(a_pInput, in_name, in_pTemplateArguments,
                                                    *(OptionalArrayView<LanguageElement*>*)&in_pFunctionArguments);
    }
        return;

    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*  pInputArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        auto&             inputTemplateParams = a_pInput->getTemplate()->getTemplateParameters();
        size_t            inputTemplateParamCount = inputTemplateParams.size();
        if (pInputArgument->isTemplateDependant() &&
            pInputArgument->getMetaClass() == PHANTOM_CLASSOF(TemplateDependantTemplateInstance))
        {
            TemplateDependantTemplateInstance* pTDTI = static_cast<TemplateDependantTemplateInstance*>(pInputArgument);
            /// Both template template parameter and template argument has same
            /// parameter/argument count
            ///
            auto&& tdTplParams = pTDTI->getTemplate()->getTemplateSignature()->getTemplateParameters();
            if (tdTplParams.size() == inputTemplateParamCount)
            {
                size_t i = 0;
                for (; i < inputTemplateParamCount; ++i)
                {
                    LanguageElement* pParameter = a_pInput->getArguments()[i];
                    LanguageElement* pArgument = pTDTI->getArguments()[i];
                    if (pArgument == nullptr)
                        return;
                    if (!(templateArgumentDeduction(pParameter, pArgument, deductions)))
                        return;
                }
                pDeducedArgument = pInputArgument;
            }
        }
        else if (ClassType* pClassTypeArgument = pInputArgument->asClassType())
        {
            TemplateSpecialization* pInputArgumentSpec =
            pClassTypeArgument->getOwner() ? pClassTypeArgument->getOwner()->asTemplateSpecialization() : nullptr;
            if (pInputArgumentSpec)
            {
                size_t requiredArgumentCount = 0;
                for (requiredArgumentCount = 0; requiredArgumentCount < inputTemplateParams.size();
                     ++requiredArgumentCount)
                {
                    if (inputTemplateParams[requiredArgumentCount]->getDefaultArgument())
                        break;
                }
                /// Both template template parameter and template argument has same
                /// parameter/argument count
                if (pInputArgumentSpec->getArguments().size() == inputTemplateParamCount)
                {
                    size_t i = 0;
                    for (; i < inputTemplateParamCount; ++i)
                    {
                        LanguageElement* pParameter = (i < a_pInput->getArguments().size())
                        ? a_pInput->getArguments()[i]
                        : inputTemplateParams[i]->getDefaultArgument();
                        LanguageElement* pArgument = pInputArgumentSpec->getArguments()[i];
                        ;
                        if (!(templateArgumentDeduction(pParameter, pArgument, deductions)))
                            return;
                    }
                    pDeducedArgument = pClassTypeArgument;
                }
            }
        }
        return;
    }

    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView in_name = *(StringView*)a_Data.in[0];
        Symbols&   out_candidates = *(Symbols*)a_Data.out[0];
        out_candidates.push_back(New<TemplateDependantElement>(a_pInput, in_name));
    }
        return;

    case e_VisitorFunction_InstanciateTemplateElement:
    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const&   in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState              in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*              in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&             pResolved = *(LanguageElement**)a_Data.out[0];
        LanguageElement*              pElement = nullptr;
        OwnersGuard<LanguageElements> newArguments;
        size_t                        i = 0;
        for (auto pArg : a_pInput->getArguments())
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
                    in_pContextScope, a_Data.flags, &LanguageElement::asType);
                    if (pExpandedType == nullptr)
                        return;
                    newArguments.push_back(pExpandedType);
                }
            }
            else
            {
                LanguageElement* pResolvedArg = o_resolveT(LanguageElement, pArg);
                if (pResolvedArg == nullptr)
                {
                    // CxxSemanticError(e_Error_TemplateResolutionFailure, &pArg);
                    return;
                }
                newArguments.push_back(pResolvedArg);
            }
            ++i;
        }
        if ((pResolved = instantiateTemplate(a_pInput->getTemplate(), newArguments, in_pContextScope)))
        {
            if (Alias* pAlias = pResolved->asAlias())
            {
                pResolved = pAlias->getAliasedSymbol();
            }
        }
        else
        {
            CxxSemanticError("failed to instantiate dependant template instance '%.*s'",
                             PHANTOM_STRING_AS_PRINTF_ARG(a_pInput->getQualifiedDecoratedName()));
        }

        newArguments.cancel();
    }
        return;
    }
}
void Semantic::visit(TemplateDependantType* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView in_name = *(StringView*)a_Data.in[0];
        Symbols&   out_candidates = *(Symbols*)a_Data.out[0];
        out_candidates.push_back(New<TemplateDependantElement>(a_pInput, in_name));
    }
    break;
    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        if (a_pInput->getTemplateDependantElement())
        {
            a_pInput->getTemplateDependantElement()->visit(this, a_Data);
            if (pResolved)
            {
                pResolved = pResolved->asType();
                if (pResolved == nullptr)
                {
                    CxxSemanticError("'%s' : expected expression", FormatCStr(pResolved));
                    return;
                }
            }
        }
    }
        return;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void Semantic::visit(TemplateDependantTypeOfExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstantiated = *(LanguageElement**)a_Data.out[0];
        Type*                       pType = o_resolveT(Type, a_pInput->getType());
        if (pType)
        {
            pInstantiated = convert(New<ConstantExpression>(newConstant(size_t(pType))), a_pInput->getValueType(),
                                    CastKind::Reinterpret, UserDefinedFunctions::None, in_pContextScope);
        }
        else
        {
            CxxSemanticError("typeof : undefined type");
        }
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(TemplateParameter* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstantiated = *(LanguageElement**)a_Data.out[0];

        auto pInstancePH = o_instantiateT(Placeholder, a_pInput->getPlaceholder()->asSymbol());
        if (!pInstancePH)
            return;

        LanguageElement* pInstanceDefaultArg = nullptr;
        if (auto pDArg = a_pInput->getDefaultArgument())
            pInstanceDefaultArg = o_instantiateT(LanguageElement, pDArg);

        pInstantiated = New<TemplateParameter>(pInstancePH, pInstanceDefaultArg);

        return;
    }
    }
    visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}
void Semantic::visit(TemplateParameterPackExpansion* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ResolveTemplateDependencies:
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        return a_pInput->getPackArgument()->asSymbol()->visit(this, a_Data);
    }
    }
    PHANTOM_UNREACHABLE();
}
void Semantic::visit(TemplateParameterPackTypeExpansion* a_pInput, VisitorData a_Data)
{
    return a_pInput->getExpansion()->visit(this, a_Data);
}
void Semantic::visit(TemplateParameterPackExpressionExpansion* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ResolveTemplateDependencies:
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstantiated = *(LanguageElement**)a_Data.out[0];
        auto                        pExpParam = a_pInput->getExpandedParameter();
        auto&                       templateDepParams = static_cast<Signature*>(pExpParam->getOwner())->getParameters();
        size_t                      templateDepParamsCount = templateDepParams.size();
        for (size_t i = 0; i < templateDepParamsCount; ++i)
        {
            if (templateDepParams[i] == pExpParam)
            {
                Subroutine* pInstanceSubroutine = o_resolveT(Subroutine, in_pContextScope->getEnclosingSubroutine());
                auto&       resolvedParams = pInstanceSubroutine->getParameters();
                size_t      resolvedParamSize = resolvedParams.size();
                if (resolvedParamSize < templateDepParamsCount)
                {
                    // variadic expanded without arguments => skip
                    pExpParam = nullptr;
                    break;
                }
                Expression* pLeft = nullptr;
                for (size_t p = i; p < i + (resolvedParamSize - templateDepParamsCount) + 1; ++p)
                {
                    templateInstantiations()[in_TemplateSubstitution.getInstantiation()][pExpParam] = resolvedParams[p];
                    LanguageElement* pElement = o_instantiateT(LanguageElement, a_pInput->getExpandedExpression());
                    CxxSemanticErrorReturnIf(pElement->asExpression() == nullptr,
                                             "expected evaluable expression in function variadic parameter expansion");
                    if (pLeft == nullptr)
                        pLeft = static_cast<Expression*>(pElement);
                    else
                        pLeft = New<CommaExpression>(pLeft, static_cast<Expression*>(pElement));
                }
                pInstantiated = pLeft;
                return;
            }
        }
    }
    }
    PHANTOM_UNREACHABLE();
    visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}
void Semantic::visit(TemplateSignature* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView         in_name = *(StringView*)a_Data.in[0];
        Symbols&           out_candidates = *(Symbols*)a_Data.out[0];
        TemplateParameter* pParam = a_pInput->getTemplateParameter(in_name);
        if (pParam)
        {
            out_candidates.push_back(pParam->getPlaceholder()->asSymbol());
            return;
        }
    }
    break;

    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstantiated = *(LanguageElement**)a_Data.out[0];

        TemplateParameters tparams;
        for (auto pTPararm : a_pInput->getTemplateParameters())
        {
            auto pInstantiatedTParam = o_instantiateT(TemplateParameter, pTPararm);
            if (!pInstantiatedTParam)
                continue;
            tparams.push_back(pInstantiatedTParam);
        }

        pInstantiated = TemplateSignature::Create(getSource(), tparams);
    }
        return;
    }
    visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}
void Semantic::visit(TemplateSpecialization* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_QualifiedNameLookup:
    {
        StringView in_name = *(StringView*)a_Data.in[0];
        if (in_name == a_pInput->getTemplate()->getName() && m_pSource) // only allow that for non-C++ mode
        {
            Symbols& out_candidates = *(Symbols*)a_Data.out[0];
            PHANTOM_SEMANTIC_ASSERT_ElementIsVisible((a_pInput->getTemplate()));
            out_candidates.push_back(a_pInput->getTemplate());
        }
    }
        return;

    case e_VisitorFunction_UnqualifiedLookup:
    {
        StringView                          in_name = *(StringView*)a_Data.in[0];
        OptionalArrayView<LanguageElement*> in_pTemplateArguments = *(OptionalArrayView<LanguageElement*>*)a_Data.in[1];
        OptionalArrayView<Expression*>      in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
        LanguageElement*                    in_pContextScope = *(LanguageElement**)a_Data.in[3];
        Type*                               in_pInitializationType = (Type*)a_Data.in[4];
        LanguageElement*&                   out_pResult = *(LanguageElement**)a_Data.out[0];
        TemplateSignature*                  pTemplateSignature = a_pInput->getTemplateSignature();
        if (pTemplateSignature == nullptr)
        {
            // we are currently solving a name during a template instantiation
            if (TemplateSpecialization* pInstantiationSpec = a_pInput->getInstantiationSpecialization())
            {
                pTemplateSignature = pInstantiationSpec->getTemplateSignature();

                auto& templateParams = pTemplateSignature->getTemplateParameters();
                for (size_t i = 0; i < templateParams.size(); ++i)
                {
                    if (templateParams[i]->getName() == in_name)
                    {
                        out_pResult = a_pInput->getArguments()[i];
                        break;
                    }
                }
                if (out_pResult)
                {
                    if (in_pFunctionArguments == nullptr)
                    {
                        return;
                    }
                    // we grab the template signature of the instantiation specialization
                    out_pResult = silentQualifiedLookup(out_pResult, out_pResult->getName(), in_pTemplateArguments,
                                                        in_pFunctionArguments, in_pContextScope, in_pInitializationType,
                                                        a_Data.flags);
                    if (out_pResult)
                    {
                        return;
                    }
                }
            }
            // template full specialization
            else
            {
                // do nothing, template signature stay empty n the case of a full specialization
            }
        }

        if (pTemplateSignature)
        {
            // we grab the template signature of the instantiation specialization
            out_pResult =
            silentQualifiedLookup(pTemplateSignature, in_name, in_pTemplateArguments, in_pFunctionArguments,
                                  in_pContextScope, in_pInitializationType, a_Data.flags);
            if (out_pResult)
            {
                return;
            }
        }

        if (isStandardLookup()) // expression parsing
        {
            if (a_pInput->getTemplate()->getNamingScope() == nullptr)
                return;
            in_pContextScope = a_pInput->getTemplate()->getNamingScope()->asLanguageElement();
            out_pResult = silentUnqualifiedLookup(in_name, in_pTemplateArguments, in_pFunctionArguments,
                                                  in_pContextScope, in_pInitializationType, a_Data.flags);
            if (out_pResult)
                return;
        }
        else
        {
            if (a_pInput->testFlags(PHANTOM_R_FLAG_IMPLICIT)) // we are instancing the specialization
            {
                if (a_pInput->getTemplate()->getOwner() == nullptr)
                    return;
                if (a_pInput->getInstantiationSpecialization())
                {
                    in_pContextScope = a_pInput->getInstantiationSpecialization()->getOwner();
                }
                else
                {
                    in_pContextScope = a_pInput->getTemplate()->getOwner();
                }
                out_pResult = silentUnqualifiedLookup(in_name, in_pTemplateArguments, in_pFunctionArguments,
                                                      in_pContextScope, in_pInitializationType, a_Data.flags);
                if (out_pResult)
                    return;
            }
            // else we are in the body of the template specialization
            //             else if(in_pContextScope->hasOwnerCascade(a_pInput))
            //             {
            //
            //             }
        }

        //                 }
        //                 else
        //                 {
        //                     in_pContextScope = a_pInput->getOwner();
        //                 }
        //                 out_pResult = silentUnqualifiedLookup(in_name, in_pTemplateArguments,
        //                 in_pFunctionArguments, in_pContextScope, in_pInitializationType,
        //                 a_Data.flags); if (out_pResult)
        //                     return;
    }
    break;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        PHANTOM_ASSERT(!a_pInput->isEmpty());

        LanguageElements resolvedArguments;
        _resolveTemplateSpecializationArguments(a_pInput, in_TemplateSubstitution, in_eClassBuildState,
                                                in_pContextScope, resolvedArguments);

        Template* pInstanceTemplate = o_resolveT(Template, a_pInput->getTemplate());
        auto      pInstanceSignature = o_instantiateT(TemplateSignature, a_pInput->getTemplateSignature());
        if (!pInstanceSignature)
            return;
        pInstanciated = in_pContextScope->asScope()->addTemplateSpecialization(pInstanceTemplate, pInstanceSignature,
                                                                               resolvedArguments);
    }
        return;
    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        PHANTOM_SEMANTIC_ASSERT(false);
    }
        return;
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(Type* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TypeConversion:
    {
        Type*                pOutput = *(Type* const*)a_Data.in[0];
        CastKind             castKind = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions iUserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        LanguageElement*     pContextScope = *(LanguageElement* const*)a_Data.in[3];
        bool                 in_bInitialize = *(bool const*)a_Data.in[4];
        Conversion*&         conv = *(Conversion**)a_Data.out[0];
        ERefType             refType = baseConversion(a_pInput, pOutput, castKind);
        if ((refType != e_RefType_None) && a_pInput->isA(pOutput))
        {
            if (a_pInput->isSame(pOutput))
            {
                conv =
                newConv<DefaultConversionSequence>(newConv<RefConversion>(a_pInput, refType == e_RefType_RValue));
            }
            else
            {
                // first compute the cast conversion between references (rvalue or const lvalue
                // depending on the context)
                *(Type**)a_Data.in[0] =
                refType == e_RefType_RValue ? pOutput->addRValueReference() : pOutput->addConst()->addLValueReference();
                Type* pNewInput = refType == e_RefType_RValue ? a_pInput->addRValueReference()
                                                              : a_pInput->addConst()->addLValueReference();
                pNewInput->visit(this, a_Data);

                // if the conversion exists
                if (conv)
                {
                    // then add the reference conversion (MyClass => (MyClass&& | MyClass const&))
                    if (conv->semantic)
                        conv = conv->clone(getSource());
                    DefaultConversionSequence* seq = static_cast<DefaultConversionSequence*>(conv);
                    seq->m_user_defined = newConv<UserDefinedConversion>(seq->m_standard);
                    seq->m_standard =
                    newConv<StandardConversionSequence>(newConv<RefConversion>(a_pInput, refType == e_RefType_RValue));
                }
            }
            return;
        }
        if (iUserDefinedConversionsAllowed != UserDefinedFunctions::None)
        {
            conv = userDefinedConversion(
            a_pInput, pOutput,
            (castKind == CastKind::Implicit) ? (iUserDefinedConversionsAllowed == UserDefinedFunctions::All) : true,
            in_bInitialize, pContextScope, refType);
        }
        return;
    }
    case e_VisitorFunction_QualifiedLookup:
    {
        // fundamentals
        if (a_pInput->asClassType() == nullptr)
        {
            StringView                     in_name = *(StringView*)a_Data.in[0];
            OptionalArrayView<Expression*> in_pFunctionArguments = *(OptionalArrayView<Expression*>*)a_Data.in[2];
            LanguageElement*&              out_pResult = *(LanguageElement**)a_Data.out[0];
            LanguageElement*               in_pContextScope = *(LanguageElement* const*)a_Data.in[3];

            // C++ style function cast
            if ((in_name.empty() || in_name == a_pInput->getName()) && in_pFunctionArguments)
            {
                if (in_pFunctionArguments->size() == 1)
                {
                    out_pResult = CxxSemanticConversionNE(in_pFunctionArguments->back(), a_pInput);
                }
                else if (in_pFunctionArguments->size() == 0)
                {
                    out_pResult = convert(New<ConstantExpression>(newConstant<size_t>(0)), a_pInput, CastKind::Explicit,
                                          UserDefinedFunctions::None, in_pContextScope);
                }
                return;
            }
        }
    }
    break;
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(TypeTraitExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        bool                        result = false;
        Types const&                types = a_pInput->getTypes();
        switch (a_pInput->getTypeTrait())
        {
        case TypeTraitExpression::e_TypeTait_IsSame:
        {
            Type* pType0 = o_resolveT(Type, types[0]);
            if (pType0 == nullptr)
                return;
            Type* pType1 = o_resolveT(Type, types[1]);
            if (pType1 == nullptr)
                return;
            result = pType0->isSame(pType1);
        }
        break;
        }
        pInstanciated = New<ConstantExpression>(newConstant(result));
    }
        return;
    }
    visit(static_cast<Expression*>(a_pInput), a_Data);
}
void Semantic::visit(Union* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        pInstanciated = New<Union>(a_pInput->getName(), a_pInput->getModifiers());
        visit(static_cast<ClassType*>(a_pInput), a_Data);
    }
        return;
    }
    visit(static_cast<ClassType*>(a_pInput), a_Data);
}
void Semantic::visit(ValueMember* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}

void Semantic::visit(VarArgCallExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        OwnersGuard<Expressions>    newArguments;
        for (auto pArgument : a_pInput->getArguments())
        {
            PHANTOM_SEMANTIC_ASSERT(!(pArgument->isTemplateDependant()),
                                    "subroutine must be valid because no template dependant expression must "
                                    "be involved, TemplateDependantExpression would be used instead");
            Expression* pInst = o_instantiateT(Expression, pArgument);
            if (pInst == nullptr)
                return;
            newArguments.push_back(pInst);
        }
        Subroutine* pSubroutine = o_resolveT(Subroutine, a_pInput->getSubroutine());
        PHANTOM_SEMANTIC_ASSERT(pSubroutine,
                                "subroutine must be valid because no template dependant expression must be "
                                "involved, TemplateDependantExpression would be used instead");
        pInstanciated = New<VarArgCallExpression>(pSubroutine, newArguments.elements());
    }
        return;
    }
    visit(static_cast<CallExpression*>(a_pInput), a_Data);
}

void Semantic::visit(Variable* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ToExpression:
    {
        Expression*  in_pLeftExpression = *(Expression**)a_Data.in[0];
        Expression*& out_pExpression = *(Expression**)a_Data.out[0];
        out_pExpression = New<VariableExpression>(a_pInput);
    }
        return;
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        pInstanciated =
        New<Variable>(o_resolveT(Type, a_pInput->getValueType()), a_pInput->getName(), a_pInput->getModifiers());
    }
        return;
    }
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(VariableExpression* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_InstanciateTemplateElement:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pInstanciated = *(LanguageElement**)a_Data.out[0];
        pInstanciated = New<VariableExpression>(o_resolveT(Variable, a_pInput->getVariable()));
    }
        return;
    }
    visit(static_cast<LValueExpression*>(a_pInput), a_Data);
}
void Semantic::visit(VirtualMethodTable* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void Semantic::visit(VolatileType* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_ComputeSizeAndAlignment:
        a_pInput->getUnderlyingType()->visit(this, a_Data);
        a_pInput->setSize(a_pInput->getUnderlyingType()->getSize());
        a_pInput->setAlignment(a_pInput->getUnderlyingType()->getAlignment());
        return;
    case e_VisitorFunction_TemplateArgumentDeduction:
    {
        LanguageElement*  pArgument = *(LanguageElement* const*)a_Data.in[0];
        LanguageElement*& pDeducedArgument = *(LanguageElement**)a_Data.out[0];
        PlaceholderMap&   deductions = *(PlaceholderMap*)a_Data.out[1];
        VolatileType*     pType = pArgument->asVolatileType();
        if (pType)
        {
            Type* pDeducedType =
            templateArgumentDeduction(a_pInput->getVolatiledType(), pType->getVolatiledType(), deductions);
            if (pDeducedType)
            {
                pDeducedArgument = pDeducedType->makeVolatile();
            }
        }
        else
        {
            ConstVolatileType* pType = pArgument->asConstVolatileType();
            if (pType)
            {
                Type* pDeducedType =
                templateArgumentDeduction(a_pInput->getVolatiledType(), pType->removeVolatile(), deductions);
                if (pDeducedType)
                {
                    pDeducedArgument = pDeducedType->makeVolatile();
                }
            }
        }
    }
        return;

    case e_VisitorFunction_TypeConversion:
    {
        a_pInput->getVolatiledType()->visit(this, a_Data);
    }
        return;

    case e_VisitorFunction_ResolveTemplateDependencies:
    {
        TemplateSubstitution const& in_TemplateSubstitution = *(TemplateSubstitution const*)a_Data.in[0];
        EClassBuildState            in_eClassBuildState = *(EClassBuildState*)a_Data.in[1];
        LanguageElement*            in_pContextScope = *(LanguageElement**)a_Data.in[2];
        LanguageElement*&           pResolved = *(LanguageElement**)a_Data.out[0];
        Type*                       pType = o_resolveT(Type, a_pInput->getVolatiledType());
        pResolved = pType ? pType->makeVolatile() : nullptr;
    }
        return;
    }
    visit(static_cast<Type*>(a_pInput), a_Data);
}

void Semantic::visit(FundamentalTypeT<void>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void Semantic::visit(FundamentalTypeT<std::nullptr_t>* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TypeConversion:
    {
        Type*        pOutput = (*(Type* const*)a_Data.in[0])->removeQualifiers();
        CastKind     castKind = *(CastKind const*)a_Data.in[1];
        Conversion*& conv = *(Conversion**)a_Data.out[0];
        auto         pOutputAsConstRef = pOutput->asConstLValueReference();
        Type*        pOutputAsRVRef = nullptr;
        if (pOutputAsConstRef)
            pOutput = pOutputAsConstRef->getUnderlyingType()->removeQualifiers();
        else if ((pOutputAsRVRef = pOutput->asRValueReference()))
            pOutput = pOutputAsRVRef->getUnderlyingType()->removeQualifiers();

        if (pOutput->asPointer() || pOutput->asMethodPointer() || pOutput->asFunctionPointer())
        {
            NumericConversion* nconv = castKind == CastKind::Implicit
            ? static_cast<NumericConversion*>(
              newConv<NumericConversionT<std::nullptr_t, void*, CastKind::Implicit>>(pOutput))
            : castKind == CastKind::Static
            ? static_cast<NumericConversion*>(
              newConv<NumericConversionT<std::nullptr_t, void*, CastKind::Static>>(pOutput))
            : castKind == CastKind::Explicit
            ? static_cast<NumericConversion*>(
              newConv<NumericConversionT<std::nullptr_t, void*, CastKind::Explicit>>(pOutput))
            : castKind == CastKind::Reinterpret
            ? static_cast<NumericConversion*>(
              newConv<NumericConversionT<std::nullptr_t, void*, CastKind::Reinterpret>>(pOutput))
            : nullptr;
            PHANTOM_SEMANTIC_ASSERT(nconv);
            conv = newConv<DefaultConversionSequence>(nconv);
        }
        else if (pOutput == PHANTOM_TYPEOF(bool))
        {
            NumericConversion* nconv = castKind == CastKind::Implicit
            ? static_cast<NumericConversion*>(
              newConv<NumericConversionT<std::nullptr_t, bool, CastKind::Implicit>>(pOutput))
            : castKind == CastKind::Static
            ? static_cast<NumericConversion*>(
              newConv<NumericConversionT<std::nullptr_t, bool, CastKind::Static>>(pOutput))
            : castKind == CastKind::Explicit
            ? static_cast<NumericConversion*>(
              newConv<NumericConversionT<std::nullptr_t, bool, CastKind::Explicit>>(pOutput))
            : castKind == CastKind::Reinterpret
            ? static_cast<NumericConversion*>(
              newConv<NumericConversionT<std::nullptr_t, bool, CastKind::Reinterpret>>(pOutput))
            : nullptr;
            PHANTOM_SEMANTIC_ASSERT(nconv);
            conv = newConv<DefaultConversionSequence>(nconv);
        }
        if (conv)
        {
            if (pOutputAsConstRef)
                static_cast<DefaultConversionSequence*>(conv)->addConstRef();
            else if (pOutputAsRVRef)
                static_cast<DefaultConversionSequence*>(conv)->addRValueRef();
            return;
        }
    }
    }
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}

template<typename t_Ty>
Conversion* Semantic::baseIntegralConversion(IntegralTypeT<t_Ty>* a_pInput, Type*& pOutput, CastKind castKind,
                                             ERefType& refType)
{
    Conversion* conv = baseStandardConversion(a_pInput, pOutput, castKind, refType);
    if (conv)
        return conv;

    switch (castKind)
    {
    case CastKind::Explicit:
    {
        if (pOutput->removeQualifiers()->asType() || pOutput->removeQualifiers()->asEnum())
            return integralConversion<t_Ty, CastKind::Explicit>(a_pInput, pOutput->removeQualifiers(), false,
                                                                (refType));
    }
    break;
    case CastKind::Reinterpret:
    {
        if (pOutput->removeQualifiers()->asType() || pOutput->removeQualifiers()->asEnum())
            return integralConversion<t_Ty, CastKind::Reinterpret>(a_pInput, pOutput->removeQualifiers(), false,
                                                                   (refType));
    }
    break;
    }
    return nullptr;
}

#define _PHNTM_SEMA_INTCONV(a_pInput, pOutput, bPromotion, refType)                                                    \
    castKind == CastKind::Implicit                                                                                     \
    ? integralConversion<t_Ty, CastKind::Implicit>(a_pInput, pOutput, bPromotion, (refType))                           \
    : castKind == CastKind::Static                                                                                     \
    ? integralConversion<t_Ty, CastKind::Static>(a_pInput, pOutput, bPromotion, (refType))                             \
    : castKind == CastKind::Explicit                                                                                   \
    ? integralConversion<t_Ty, CastKind::Explicit>(a_pInput, pOutput, bPromotion, (refType))                           \
    : castKind == CastKind::Reinterpret                                                                                \
    ? integralConversion<t_Ty, CastKind::Reinterpret>(a_pInput, pOutput, bPromotion, (refType))                        \
    : nullptr

#define _PHNTM_SEMA_FLTCONV(a_pInput, pOutput, bPromotion, refType)                                                    \
    castKind == CastKind::Implicit                                                                                     \
    ? floatingPointConversion<t_Ty, CastKind::Implicit>(a_pInput, pOutput, bPromotion, (refType))                      \
    : castKind == CastKind::Static                                                                                     \
    ? floatingPointConversion<t_Ty, CastKind::Static>(a_pInput, pOutput, bPromotion, (refType))                        \
    : castKind == CastKind::Explicit                                                                                   \
    ? floatingPointConversion<t_Ty, CastKind::Explicit>(a_pInput, pOutput, bPromotion, (refType))                      \
    : castKind == CastKind::Reinterpret                                                                                \
    ? floatingPointConversion<t_Ty, CastKind::Reinterpret>(a_pInput, pOutput, bPromotion, (refType))                   \
    : nullptr

template<typename t_Ty>
void Semantic::visitIntegral(IntegralTypeT<t_Ty>* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TypeConversion:
    {
        Type*                pOutput = *(Type* const*)a_Data.in[0];
        CastKind             castKind = *(CastKind const*)a_Data.in[1];
        UserDefinedFunctions iUserDefinedConversionsAllowed = *(UserDefinedFunctions const*)a_Data.in[2];
        LanguageElement*     pContextScope = *(LanguageElement* const*)a_Data.in[3];
        Conversion*&         conv = *(Conversion**)a_Data.out[0];
        ERefType             refType;
        conv = baseIntegralConversion(a_pInput, pOutput, castKind, (refType));
        if (!conv)
        {
            TypeKind typeId = pOutput->getTypeKind();
            if (typeId >= TypeKind::Bool && typeId <= TypeKind::LongDouble) // integral and floating
            {
                if (pOutput->removeQualifiers() == a_pInput->promote())
                    conv = _PHNTM_SEMA_INTCONV(a_pInput, pOutput, true, (refType));
                else
                    conv = _PHNTM_SEMA_INTCONV(a_pInput, pOutput, false, (refType));
            }
            else if ((castKind >= CastKind::Reinterpret &&
                      (typeId == TypeKind::Pointer || typeId == TypeKind::FunctionPointer)) ||
                     typeId == TypeKind::Enum)
            {
                conv = _PHNTM_SEMA_INTCONV(a_pInput, pOutput, false, (refType));
            }
        }
        if (conv)
            return;
    }
    }
    visit(static_cast<ArithmeticTypeT<t_Ty>*>(a_pInput), a_Data);
}

void Semantic::visit(IntegralTypeT<bool>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

void Semantic::visit(IntegralTypeT<char>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

void Semantic::visit(IntegralTypeT<uchar>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

void Semantic::visit(IntegralTypeT<schar>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

#if PHANTOM_HAS_BUILT_IN_WCHAR_T
void Semantic::visit(IntegralTypeT<wchar_t>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}
#endif

#if PHANTOM_HAS_BUILT_IN_CHAR16_T
void Semantic::visit(IntegralTypeT<char16_t>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}
#endif

#if PHANTOM_HAS_BUILT_IN_CHAR32_T
void Semantic::visit(IntegralTypeT<char32_t>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

#endif

void Semantic::visit(IntegralTypeT<short>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

void Semantic::visit(IntegralTypeT<ushort>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

void Semantic::visit(IntegralTypeT<int>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

void Semantic::visit(IntegralTypeT<uint>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

void Semantic::visit(IntegralTypeT<long>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

void Semantic::visit(IntegralTypeT<ulong>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

void Semantic::visit(IntegralTypeT<longlong>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

void Semantic::visit(IntegralTypeT<ulonglong>* a_pInput, VisitorData a_Data)
{
    visitIntegral(a_pInput, a_Data);
}

template<typename t_Ty>
void Semantic::visitFloatingPoint(FloatingPointTypeT<t_Ty>* a_pInput, VisitorData a_Data)
{
    switch (a_Data.id)
    {
    case e_VisitorFunction_TypeConversion:
    {
        Type*        pOutput = *(Type* const*)a_Data.in[0];
        CastKind     castKind = *(CastKind const*)a_Data.in[1];
        Conversion*& conv = *(Conversion**)a_Data.out[0];
        ERefType     refType;
        conv = baseStandardConversion(a_pInput, pOutput, castKind, (refType));
        if (!conv)
        {
            TypeKind typeId = pOutput->getTypeKind();
            if (typeId >= TypeKind::Bool && typeId <= TypeKind::LongDouble) // integral and floating
            {
                if (pOutput->removeQualifiers() == a_pInput->promote())
                    conv = _PHNTM_SEMA_FLTCONV(a_pInput, pOutput, true, (refType));
                else
                    conv = _PHNTM_SEMA_FLTCONV(a_pInput, pOutput, false, (refType));
            }
            else if (castKind >= CastKind::Reinterpret && typeId == TypeKind::Enum)
            {
                conv = _PHNTM_SEMA_FLTCONV(a_pInput, pOutput, false, (refType));
            }
        }
        if (conv)
            return;
    }
    }
    visit(static_cast<ArithmeticTypeT<t_Ty>*>(a_pInput), a_Data);
}

void Semantic::visit(FloatingPointTypeT<float>* a_pInput, VisitorData a_Data)
{
    visitFloatingPoint(a_pInput, a_Data);
}

void Semantic::visit(FloatingPointTypeT<double>* a_pInput, VisitorData a_Data)
{
    visitFloatingPoint(a_pInput, a_Data);
}

void Semantic::visit(FloatingPointTypeT<longdouble>* a_pInput, VisitorData a_Data)
{
    visitFloatingPoint(a_pInput, a_Data);
}
} // namespace lang
} // namespace phantom
