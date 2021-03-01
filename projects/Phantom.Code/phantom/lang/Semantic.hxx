#pragma once

// haunt {

// clang-format off

#include "Semantic.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/struct>
#include <phantom/enum>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/Delegate.hxx>
#include <phantom/utils/Optional.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Semantic")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Enums) { this_().enum_<EValueCategory>().values({
            {"e_prvalue",e_prvalue},
            {"e_rvalue",e_rvalue},
            {"e_glvalue",e_glvalue},
            {"e_lvalue",e_lvalue},
            {"e_xvalue",e_xvalue}});
        }
        PHANTOM_CLASS(Semantic)
        {
            using Accesses = typedef_< phantom::lang::Accesses>;
            using EClassBuildState = typedef_<_::EClassBuildState>;
            using EError = typedef_<_::EError>;
            using ERefType = typedef_<_::ERefType>;
            using EVisitorFunction = typedef_<_::EVisitorFunction>;
            using ErrorDelegate = typedef_<_::ErrorDelegate>;
            using Expressions = typedef_< phantom::lang::Expressions>;
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using NameFormatDelegate = typedef_<_::NameFormatDelegate>;
            using NameLookupData = typedef_<_::NameLookupData>;
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
            using QualifiedLookupData = typedef_<_::QualifiedLookupData>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Subroutines = typedef_< phantom::lang::Subroutines>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            using UnqualifiedLookupData = typedef_<_::UnqualifiedLookupData>;
            this_()
            .inherits<::phantom::Object, ::phantom::lang::LanguageElementVisitorEx>()
        
        .public_()
            .enum_<EClassBuildState>().values({
                {"e_ClassBuildState_None",_::e_ClassBuildState_None},
                {"e_ClassBuildState_Inheritance",_::e_ClassBuildState_Inheritance},
                {"e_ClassBuildState_Members",_::e_ClassBuildState_Members},
                {"e_ClassBuildState_Sized",_::e_ClassBuildState_Sized},
                {"e_ClassBuildState_States",_::e_ClassBuildState_States},
                {"e_ClassBuildState_ImplicitsAndDefaults",_::e_ClassBuildState_ImplicitsAndDefaults},
                {"e_ClassBuildState_Blocks",_::e_ClassBuildState_Blocks}})
            .end()
            .enum_<>().values({
                {"e_ClassBuildState_Count",_::e_ClassBuildState_Count}})
            .end()
            .enum_<ERefType>().values({
                {"e_RefType_None",_::e_RefType_None},
                {"e_RefType_LValue",_::e_RefType_LValue},
                {"e_RefType_RValue",_::e_RefType_RValue}})
            .end()
            .enum_<>().values({
                {"e_VisitorFlag_None",_::e_VisitorFlag_None},
                {"e_VisitorFlag_KeepAliases",_::e_VisitorFlag_KeepAliases}})
            .end()
            .constructor<void(Source*, Message*)>()
            .method<void(LanguageElement*)>("Delete", &_::Delete)
            .method<void(bool)>("setStandardLookup", &_::setStandardLookup)
            .method<bool() const>("isStandardLookup", &_::isStandardLookup)
            .method<BuiltInOperator*(StringView) const>("getBuiltInBinaryOperator", &_::getBuiltInBinaryOperator)
            .method<BuiltInOperator*(StringView) const>("getBuiltInPreUnaryOperator", &_::getBuiltInPreUnaryOperator)
            .method<BuiltInOperator*(StringView) const>("getBuiltInPostUnaryOperator", &_::getBuiltInPostUnaryOperator)
            .enum_<EVisitorFunction>().values({
                {"e_VisitorFunction_TypeConversion",_::e_VisitorFunction_TypeConversion},
                {"e_VisitorFunction_ExpressionConversion",_::e_VisitorFunction_ExpressionConversion},
                {"e_VisitorFunction_ToExpression",_::e_VisitorFunction_ToExpression},
                {"e_VisitorFunction_CreateCallExpression",_::e_VisitorFunction_CreateCallExpression},
                {"e_VisitorFunction_SubroutineResolution",_::e_VisitorFunction_SubroutineResolution},
                {"e_VisitorFunction_UnqualifiedLookup",_::e_VisitorFunction_UnqualifiedLookup},
                {"e_VisitorFunction_TemplateArgumentDeduction",_::e_VisitorFunction_TemplateArgumentDeduction},
                {"e_VisitorFunction_CallTemplateArgumentDeduction",_::e_VisitorFunction_CallTemplateArgumentDeduction},
                {"e_VisitorFunction_QualifiedLookup",_::e_VisitorFunction_QualifiedLookup},
                {"e_VisitorFunction_UnqualifiedNameLookup",_::e_VisitorFunction_UnqualifiedNameLookup},
                {"e_VisitorFunction_QualifiedNameLookup",_::e_VisitorFunction_QualifiedNameLookup},
                {"e_VisitorFunction_Access",_::e_VisitorFunction_Access},
                {"e_VisitorFunction_IsViableCallCandidate",_::e_VisitorFunction_IsViableCallCandidate},
                {"e_VisitorFunction_IsViableSignatureCandidate",_::e_VisitorFunction_IsViableSignatureCandidate},
                {"e_VisitorFunction_InstanciateTemplateElement",_::e_VisitorFunction_InstanciateTemplateElement},
                {"e_VisitorFunction_ResolveTemplateDependencies",_::e_VisitorFunction_ResolveTemplateDependencies},
                {"e_VisitorFunction_BinaryOperator",_::e_VisitorFunction_BinaryOperator},
                {"e_VisitorFunction_UnaryPreOperator",_::e_VisitorFunction_UnaryPreOperator},
                {"e_VisitorFunction_UnaryPostOperator",_::e_VisitorFunction_UnaryPostOperator},
                {"e_VisitorFunction_BuildClass",_::e_VisitorFunction_BuildClass},
                {"e_VisitorFunction_ComputeSizeAndAlignment",_::e_VisitorFunction_ComputeSizeAndAlignment},
                {"e_VisitorFunction_HasSideEffect",_::e_VisitorFunction_HasSideEffect}})
            .end()
            .struct_<NameLookupData>()
                .field("name", &_::NameLookupData::name)
                .field("scope", &_::NameLookupData::scope)
                .field("access", &_::NameLookupData::access)
                .field("outSymbols", &_::NameLookupData::outSymbols)
            .end()
            .struct_<UnqualifiedLookupData>()
                .inherits<::phantom::lang::Semantic::NameLookupData>()
                .field("templateArguments", &_::UnqualifiedLookupData::templateArguments)
                .field("functionArguments", &_::UnqualifiedLookupData::functionArguments)
                .field("result", &_::UnqualifiedLookupData::result)
            .end()
            .struct_<QualifiedLookupData>()
                .inherits<::phantom::lang::Semantic::UnqualifiedLookupData>()
                .field("lhs", &_::QualifiedLookupData::lhs)
            .end()
            .enum_<EError>().values({
                {"e_ErrorOK",_::e_ErrorOK},
                {"e_Error_NoConversion",_::e_Error_NoConversion},
                {"e_Error_NoOverload",_::e_Error_NoOverload},
                {"e_Error_NoMember",_::e_Error_NoMember},
                {"e_Error_NoBuiltinOperator",_::e_Error_NoBuiltinOperator},
                {"e_Error_NoBinaryOperator",_::e_Error_NoBinaryOperator},
                {"e_Error_NoUnaryPrefixOperator",_::e_Error_NoUnaryPrefixOperator},
                {"e_Error_NoUnaryPostfixOperator",_::e_Error_NoUnaryPostfixOperator},
                {"e_Error_NoConstructor",_::e_Error_NoConstructor},
                {"e_Error_NoDefaultConstructor",_::e_Error_NoDefaultConstructor},
                {"e_Error_NoTemplateDeclaration",_::e_Error_NoTemplateDeclaration},
                {"e_Error_NoTemplateSpecialization",_::e_Error_NoTemplateSpecialization},
                {"e_Error_ExpectedTemplateSignature",_::e_Error_ExpectedTemplateSignature},
                {"e_Error_ReferencesCannotBeAllocated",_::e_Error_ReferencesCannotBeAllocated},
                {"e_Error_ReferencesMustBeInitialized",_::e_Error_ReferencesMustBeInitialized},
                {"e_Error_UnknownBuiltinOperator",_::e_Error_UnknownBuiltinOperator},
                {"e_Error_AmbiguousCallToMultipleBuiltinOperator",_::e_Error_AmbiguousCallToMultipleBuiltinOperator},
                {"e_Error_AmbiguousAccessToMultipleMember",_::e_Error_AmbiguousAccessToMultipleMember},
                {"e_Error_AmbiguousAccessToMultipleConstructor",_::e_Error_AmbiguousAccessToMultipleConstructor},
                {"e_Error_AmbiguousCallToMultipleOverloads",_::e_Error_AmbiguousCallToMultipleOverloads},
                {"e_Error_TemplateDependantElementIsNotAnExpression",_::e_Error_TemplateDependantElementIsNotAnExpression},
                {"e_Error_InaccessibleSymbol",_::e_Error_InaccessibleSymbol},
                {"e_Error_UnexpectedMember",_::e_Error_UnexpectedMember},
                {"e_Error_UnexpectedTemplateArgumentList",_::e_Error_UnexpectedTemplateArgumentList},
                {"e_Error_TooManyTemplateArguments",_::e_Error_TooManyTemplateArguments},
                {"e_Error_NotEnoughTemplateArguments",_::e_Error_NotEnoughTemplateArguments},
                {"e_Error_TemplateInstantiationFailure",_::e_Error_TemplateInstantiationFailure},
                {"e_Error_TemplateResolutionFailure",_::e_Error_TemplateResolutionFailure},
                {"e_Error_InstantiationOfUndefinedTemplateSpecialization",_::e_Error_InstantiationOfUndefinedTemplateSpecialization},
                {"e_Error_ExpectedExpression",_::e_Error_ExpectedExpression},
                {"e_Error_StateMachineAlreadyDefinedInBase",_::e_Error_StateMachineAlreadyDefinedInBase},
                {"e_Error_QualifiedLookupFailure",_::e_Error_QualifiedLookupFailure},
                {"e_Error_UnqualifiedLookupFailure",_::e_Error_UnqualifiedLookupFailure},
                {"e_Error_QualifiedSymbolsLookupFailure",_::e_Error_QualifiedSymbolsLookupFailure},
                {"e_Error_UnqualifiedSymbolsLookupFailure",_::e_Error_UnqualifiedSymbolsLookupFailure},
                {"e_Error_TooManyFunctionArguments",_::e_Error_TooManyFunctionArguments},
                {"e_Error_NotEnoughFunctionArguments",_::e_Error_NotEnoughFunctionArguments},
                {"e_Error_MissingMethodCaller",_::e_Error_MissingMethodCaller},
                {"e_Error_ExpectedMemberPointerExpression",_::e_Error_ExpectedMemberPointerExpression},
                {"e_Error_FunctionConflictsWithPreviouslyDeclared",_::e_Error_FunctionConflictsWithPreviouslyDeclared},
                {"e_Error_SymbolAlreadyExistsWithName",_::e_Error_SymbolAlreadyExistsWithName},
                {"e_Error_ConstructorAlreadyDefined",_::e_Error_ConstructorAlreadyDefined},
                {"e_Error_TypeCannotHaveQualifiers",_::e_Error_TypeCannotHaveQualifiers},
                {"e_Error_TypeCannotHavePointer",_::e_Error_TypeCannotHavePointer},
                {"e_Error_TypeCannotHaveLValueReference",_::e_Error_TypeCannotHaveLValueReference},
                {"e_Error_TemplateSubroutineLookupFailure",_::e_Error_TemplateSubroutineLookupFailure},
                {"e_Error_ExpectedType",_::e_Error_ExpectedType},
                {"e_Error_ExpectedClassType",_::e_Error_ExpectedClassType},
                {"e_Error_ExpectedClass",_::e_Error_ExpectedClass},
                {"e_Error_ExpectedConstant",_::e_Error_ExpectedConstant},
                {"e_Error_IncompleteType",_::e_Error_IncompleteType},
                {"e_Error_IllegalAutoArray",_::e_Error_IllegalAutoArray},
                {"e_Error_EmptyInitializerList",_::e_Error_EmptyInitializerList},
                {"e_Error_IllegalExplicitArrayConversion",_::e_Error_IllegalExplicitArrayConversion},
                {"e_Error_IllegalInheritedConstructorInAggregateInitialization",_::e_Error_IllegalInheritedConstructorInAggregateInitialization},
                {"e_Error_IllegalExplicitConstructorInAggregateInitialization",_::e_Error_IllegalExplicitConstructorInAggregateInitialization},
                {"e_Error_IllegalNonImplicitConstructorInAggregateInitialization",_::e_Error_IllegalNonImplicitConstructorInAggregateInitialization},
                {"e_Error_IllegalNonPublicFieldInAggregateInitialization",_::e_Error_IllegalNonPublicFieldInAggregateInitialization},
                {"e_Error_IllegalNonPublicBaseClassInAggregateInitialization",_::e_Error_IllegalNonPublicBaseClassInAggregateInitialization},
                {"e_Error_IllegalVirtualFunctionInAggregateInitialization",_::e_Error_IllegalVirtualFunctionInAggregateInitialization},
                {"e_Error_CannotInstantiateAbstractClass",_::e_Error_CannotInstantiateAbstractClass},
                {"e_Error_StaticAssertFailed",_::e_Error_StaticAssertFailed}})
            .end()
            .staticMethod<const char*(EError)>("DefaultErrorText", &_::DefaultErrorText)
            .method<ConstantExpression*()>("createNullptrExpression", &_::createNullptrExpression)
            .method<Expression*(Type*)>("createZeroInitExpression", &_::createZeroInitExpression)
            .typedef_<ErrorDelegate>("ErrorDelegate")
            .method<Message*() const>("getMessage", &_::getMessage)
            .typedef_<NameFormatDelegate>("NameFormatDelegate")
            .method<void(NameFormatDelegate)>("setNameFormatDelegate", &_::setNameFormatDelegate)
            .method<LocalVariable*(Type*, StringView, Modifiers, Modifiers)>("createLocalVariable", &_::createLocalVariable)["PHANTOM_R_NONE"]["PHANTOM_R_NONE"]
            .method<DeleteExpression*(Class*, Expression*, LanguageElement*)>("createDeleteExpression", &_::createDeleteExpression)
            .method<DeleteExpression*(Expression*, bool, LanguageElement*)>("createDeleteExpression", &_::createDeleteExpression)
            .method<void(Alias*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(AllocateExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(AnonymousSection*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(AnonymousStruct*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(AnonymousUnion*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Application*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ArrayExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Array*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BaseConstructorCallStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Block*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BranchIfNotStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BranchIfStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BranchStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BuiltInConversionExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BuiltInOperator*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BuiltInOperatorExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(CallExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Class*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ClassType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ConditionalExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Constant*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ConstantExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Constructor*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ConstructorCallExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ConstType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ConstVolatileType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ContainerClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Field*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FieldExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FieldInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FieldPointerExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FieldPointer*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Pointer*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(DeallocateExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(DeleteExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Ellipsis*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Enum*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Evaluable*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Expression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ExpressionStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Function*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FunctionPointer*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FunctionType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(IdentityExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(InitializerListType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(InitializerListExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Label*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LanguageElement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LoadExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LocalVariable*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LocalVariableExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LocalVariableInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LValueExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LValueInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LValueReference*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MemberAnonymousSection*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MemberAnonymousStruct*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MemberAnonymousUnion*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Method*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MethodPointerCallExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MethodPointer*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MemberPointer*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Module*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Symbol*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(SymbolReferenceExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Namespace*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(NewExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Package*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PackageFolder*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Parameter*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Placeholder*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlaceholderClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlaceholderClassType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlaceholderConstant*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlaceholderTemplate*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlaceholderType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlacementNewExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PrimitiveType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Property*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PropertyExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Reference*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ReturnStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(RValueReference*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(RValueToConstLValueExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(RValueReferenceExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Scope*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(SequentialContainerClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(SetClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VectorClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MapClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Signature*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Source*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Statement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(StaticAssert*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(StringLiteralExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Structure*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(StructureExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Subroutine*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(SubroutinePointerExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BinaryOperationExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(UnaryPostOperationExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(UnaryPreOperationExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Template*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantArray*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantDeclType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantElement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantTemplateInstance*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantTypeOfExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateParameter*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateSignature*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateSpecialization*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Type*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TypeTraitExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Union*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ValueMember*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VarArgCallExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Variable*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VariableExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VirtualMethodTable*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VolatileType*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(FundamentalTypeT<void>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (std::nullptr_t, phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::phantom::lang::FundamentalTypeT<std::nullptr_t> *, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<bool>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<char>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<uchar>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<schar>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<short>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<ushort>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<int>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<uint>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<long>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<ulong>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<longlong>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<ulonglong>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::FloatingPointTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(FloatingPointTypeT<float>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::FloatingPointTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(FloatingPointTypeT<double>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::FloatingPointTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(FloatingPointTypeT<longdouble>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<wchar_t>*, ::phantom::lang::VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<char16_t>*, ::phantom::lang::VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<char32_t>*, ::phantom::lang::VisitorData), virtual_>("visit", &_::visit)
            .method<void(Class*)>("generateImplicitCopyAssignmentOperatorCode", &_::generateImplicitCopyAssignmentOperatorCode)
            .method<void(Class*)>("generateImplicitCopyConstructorCode", &_::generateImplicitCopyConstructorCode)
            .method<void(Class*)>("generateImplicitMoveConstructorCode", &_::generateImplicitMoveConstructorCode)
            .method<void(Class*)>("generateImplicitMoveAssignmentOperatorCode", &_::generateImplicitMoveAssignmentOperatorCode)
            .method<void(Class*)>("generateImplicitDefaultConstructorCode", &_::generateImplicitDefaultConstructorCode)
            .method<Expression*(Type*, Expression*, LanguageElement*)>("createDestructionExpression", &_::createDestructionExpression)
            .method<Expression*(Field*, Expression*, LanguageElement*)>("createDestructionExpression", &_::createDestructionExpression)
            .method<void(Class*)>("generateImplicitDestructorCode", &_::generateImplicitDestructorCode)
            .method<void(Subroutines&, Symbols&, OptionalArrayView<LanguageElement*>, Expressions&, LanguageElement*&, StringView, LanguageElement*, Type*, Modifiers, uint)>("selectCallCandidate", &_::selectCallCandidate)["nullptr"]["nullptr"]["0"]["0"]
            .method<Expression*(StringView, Expression*, Expression*, LanguageElement*)>("solveBinaryOperator", &_::solveBinaryOperator)["nullptr"]
            .method<Expression*(Operator, Expression*, Expression*, LanguageElement*)>("solveBinaryOperator", &_::solveBinaryOperator)["nullptr"]
            .method<Expression*(StringView, Expression*, LanguageElement*)>("solveUnaryPreOperator", &_::solveUnaryPreOperator)["nullptr"]
            .method<Expression*(Operator, Expression*, LanguageElement*)>("solveUnaryPreOperator", &_::solveUnaryPreOperator)["nullptr"]
            .method<Expression*(StringView, Expression*, LanguageElement*)>("solveUnaryPostOperator", &_::solveUnaryPostOperator)["nullptr"]
            .method<Expression*(Operator, Expression*, LanguageElement*)>("solveUnaryPostOperator", &_::solveUnaryPostOperator)["nullptr"]
            .method<Expression*(Expression*, Expression*, Expression*, LanguageElement*)>("solveConditional", &_::solveConditional)["nullptr"]
            .method<LanguageElement*(LanguageElement*, StringView, OptionalArrayView<LanguageElement*>, OptionalArrayView<Expression*>, LanguageElement*, Type*, int)>("qualifiedLookup", &_::qualifiedLookup)["nullptr"]["nullptr"]["0"]
            .method<LanguageElement*(StringView, OptionalArrayView<LanguageElement*>, OptionalArrayView<Expression*>, LanguageElement*, Type*, int)>("unqualifiedLookup", &_::unqualifiedLookup)["nullptr"]["nullptr"]["0"]
            .method<void(LanguageElement*, StringView, Symbols&, Accesses*, LanguageElement*, Access, int)>("qualifiedSymbolsLookup", &_::qualifiedSymbolsLookup)["nullptr"]["nullptr"]["Access::Public"]["0"]
            .method<void(StringView, Symbols&, Accesses*, LanguageElement*, Access, int)>("unqualifiedSymbolsLookup", &_::unqualifiedSymbolsLookup)["nullptr"]["nullptr"]["Access::Public"]["0"]
            .method<void(StringView, Symbols&, Accesses*, LanguageElement*, Access, int)>("silentUnqualifiedSymbolsLookup", &_::silentUnqualifiedSymbolsLookup)["nullptr"]["Access::Public"]["0"]
            .method<void(LanguageElement*, StringView, Symbols&, Accesses*, LanguageElement*, Access, int)>("silentQualifiedSymbolsLookup", &_::silentQualifiedSymbolsLookup)["nullptr"]["nullptr"]["Access::Public"]["0"]
            .method<LanguageElement*(LanguageElement*, StringView, OptionalArrayView<LanguageElement*>, OptionalArrayView<Expression*>, LanguageElement*, Type*, int)>("silentQualifiedLookup", &_::silentQualifiedLookup)["nullptr"]["nullptr"]["0"]
            .method<LanguageElement*(StringView, OptionalArrayView<LanguageElement*>, OptionalArrayView<Expression*>, LanguageElement*, Type*, int)>("silentUnqualifiedLookup", &_::silentUnqualifiedLookup)["nullptr"]["nullptr"]["0"]
            .method<Subroutine*(LanguageElement*, StringView, OptionalArrayView<LanguageElement*>, TypesView, Modifiers, uint, LanguageElement*)>("subroutineLookup", &_::subroutineLookup)["nullptr"]
            .method<::phantom::lang::Subroutine *(LanguageElement*, StringView, OptionalArrayView<LanguageElement*>, TypesView, Modifiers, uint, LanguageElement*), virtual_>("subroutineLookupCascade", &_::subroutineLookupCascade)["nullptr"]
            .method<LanguageElement*(Template*, const LanguageElements&, LanguageElement*)>("instantiateTemplate", &_::instantiateTemplate)["nullptr"]
            .method<LanguageElement*(LanguageElement*, LanguageElement*, PlaceholderMap&)>("templateArgumentDeduction", &_::templateArgumentDeduction)
            .method<Type*(LanguageElement*, Type*, PlaceholderMap&)>("templateArgumentDeduction", &_::templateArgumentDeduction)
            .method<Type*(Type*, Type*, PlaceholderMap&)>("callTemplateArgumentDeduction", &_::callTemplateArgumentDeduction)
            .method<Type*(Type*, Type*)>("autoDeduction", &_::autoDeduction)
            .method<Expression*(Type*, Expression*)>("autoDeduction", &_::autoDeduction)
            .method<int(TemplateSpecialization*, TemplateSpecialization*)>("compareSpecialized", &_::compareSpecialized)
            .method<int(const LanguageElements&, const LanguageElements&, const LanguageElements&, const LanguageElements&)>("partialOrdering", &_::partialOrdering)
            .method<bool(StringView) const>("isIdentifier", &_::isIdentifier)
            .method<bool(Expression*) const>("hasSideEffects", &_::hasSideEffects)
            .method<Expression*(Subroutine*, ExpressionsView, LanguageElement*)>("createCallExpression", &_::createCallExpression)
            .method<Expression*(Constructor*, ExpressionsView, LanguageElement*)>("createCallExpression", &_::createCallExpression)
            .method<Expression*(Method*, Expression*, ExpressionsView, LanguageElement*)>("createCallExpression", &_::createCallExpression)
            .method<Expression*(Expression*, Expression*, ExpressionsView, LanguageElement*)>("createCallExpression", &_::createCallExpression)
            .method<Expression*(Function*, ExpressionsView, LanguageElement*)>("createCallExpression", &_::createCallExpression)
            .method<Expression*(Expression*, Type*, CastKind, UserDefinedFunctions, LanguageElement*, bool)>("internalConvert", &_::internalConvert)["CastKind::Implicit"]["UserDefinedFunctions::ImplicitsOnly"]["nullptr"]["false"]
            .method<Expression*(Type*, LanguageElement*, bool)>("defaultConstruct", &_::defaultConstruct)["nullptr"]["true"]
            .method<Expression*(Expression*, Type*, CastKind, UserDefinedFunctions, LanguageElement*)>("convert", &_::convert)["CastKind::Implicit"]["UserDefinedFunctions::ImplicitsOnly"]["nullptr"]
            .method<Expression*(Expression*, Type*, LanguageElement*)>("convert", &_::convert)
            .method<Expression*(Expression*, Type*, LanguageElement*)>("convertIfNotEqual", &_::convertIfNotEqual)
            .method<Expression*(Expression*, Type*, UserDefinedFunctions, LanguageElement*)>("convertIfNotEqual", &_::convertIfNotEqual)
            .method<Expression*(Expression*, Type*, UserDefinedFunctions, LanguageElement*)>("initialize", &_::initialize)["UserDefinedFunctions::ImplicitsOnly"]["nullptr"]
            .method<Expression*(Expression*, Type*, LanguageElement*)>("initialize", &_::initialize)
            .method<Expression*(Expression*, Type*, LanguageElement*)>("initializeIfNotEqual", &_::initializeIfNotEqual)
            .method<Expression*(Expression*, Type*, UserDefinedFunctions, LanguageElement*)>("initializeIfNotEqual", &_::initializeIfNotEqual)
            .method<::phantom::lang::Expression *(LanguageElement*, Expression*), virtual_>("toExpression", &_::toExpression)["nullptr"]
            .method<Expression*(Field*, Expression*)>("toExpression", &_::toExpression)
            .method<Expression*(Property*, Expression*)>("toExpression", &_::toExpression)
            .method<::phantom::lang::Conversion *(Type*, Type*, CastKind, UserDefinedFunctions, LanguageElement*, bool), virtual_>("newConversion", &_::newConversion)["CastKind::Implicit"]["UserDefinedFunctions::ImplicitsOnly"]["nullptr"]["false"]
            .method<::phantom::lang::Conversion *(Type*, Type*, LanguageElement*, bool), virtual_>("newConversion", &_::newConversion)["nullptr"]["false"]
            .method<void(Conversion*), virtual_>("deleteConversion", &_::deleteConversion)
            .method<void(ConversionResults&), virtual_>("deleteConversions", &_::deleteConversions)
            .method<void(Type*), virtual_>("computeSizeAndAlignment", &_::computeSizeAndAlignment)
            .method<void(ClassType*, EClassBuildState)>("buildClass", &_::buildClass)
            .method<void(Scope*, EClassBuildState)>("buildScopeClasses", &_::buildScopeClasses)
            .method<void(Block*, EClassBuildState)>("buildScopeClasses", &_::buildScopeClasses)
            .method<void(ClassType*), virtual_>("sizeClass", &_::sizeClass)
            .method<void(ClassType*), virtual_>("edifyClass", &_::edifyClass)
            .method<void(ClassType*), virtual_>("finalizeClass", &_::finalizeClass)
            .method<bool(Symbol*, LanguageElement*) const, virtual_>("isSymbolAccessible", &_::isSymbolAccessible)
            .method<bool(Symbol*, Symbol*) const, virtual_>("isFriendWith", &_::isFriendWith)
            .method<Source*() const>("getSource", &_::getSource)
            .method<LanguageElement*(LanguageElement*, TemplateSpecialization*, EClassBuildState, LanguageElement*, int)>("instantiateTemplateElement", &_::instantiateTemplateElement)
            .method<void(bool)>("setAccessModifiersEnabled", &_::setAccessModifiersEnabled)
            .method<const ::phantom::lang::CodeRangeLocation &() const>("currentCodeRangeLocation", &_::currentCodeRangeLocation)
            .method<void(const CodeRangeLocation&)>("pushCodeRangeLocation", &_::pushCodeRangeLocation)
            .method<void()>("popCodeRangeLocation", &_::popCodeRangeLocation)
            .method<StringBuffer(Expression* const*, size_t)>("FormatArgumentList", &_::FormatArgumentList)
            .method<StringBuffer(LanguageElement* const*, size_t)>("FormatElementList", &_::FormatElementList)
            .method<void(LanguageElement*, StringBuffer&)>("FormatElement", &_::FormatElement)
            .method<StringBuffer(LanguageElement*)>("FormatElement", &_::FormatElement)
            .method<Block*(Method*, bool)>("addBlock", &_::addBlock)["false"]
            .method<Block*(Subroutine*, bool)>("addBlock", &_::addBlock)["false"]
            .staticMethod<::phantom::lang::Block *(Method*)>("AddBlock", &_::AddBlock)
            .staticMethod<::phantom::lang::Block *(Subroutine*)>("AddBlock", &_::AddBlock)
            .method<void(Signature*, TypesView, ConversionResults&, LanguageElement*)>("newImplicitConversions", &_::newImplicitConversions)
            .method<void(Signature*, ExpressionsView, ConversionResults&, LanguageElement*)>("newImplicitConversions", &_::newImplicitConversions)
            .method<void(FunctionType*, TypesView, ConversionResults&, LanguageElement*)>("newImplicitConversions", &_::newImplicitConversions)
            .method<void(FunctionType*, ExpressionsView, ConversionResults&, LanguageElement*)>("newImplicitConversions", &_::newImplicitConversions)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Semantic")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
