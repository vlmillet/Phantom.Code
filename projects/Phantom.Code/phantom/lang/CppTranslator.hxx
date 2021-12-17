#pragma once

// haunt {

// clang-format off

#include "CppTranslator.h"

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
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include <phantom/utils/Flags.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallSet.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("CppTranslator")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(CppTranslator)
        {
            using Flag = typedef_<_::Flag>;
            using Flags = typedef_<_::Flags>;
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using Operation = typedef_<_::Operation>;
            using Options = typedef_< phantom::lang::Options>;
            using Output = typedef_<_::Output>;
            using Sources = typedef_< phantom::lang::Sources>;
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            using TranslationOption = typedef_<_::TranslationOption>;
            using TranslationType = typedef_<_::TranslationType>;
            this_()
            .inherits<::phantom::lang::LanguageElementVisitorEx>()
        .public_()
            .constructor<void()>()
            .struct_<Output>()
                .field("h", &_::Output::h)
                .field("cpp", &_::Output::cpp)
                .field("hIncludes", &_::Output::hIncludes)
                .field("cppIncludes", &_::Output::cppIncludes)
            .end()
            .method<const Options&() const>("getOptions", &_::getOptions)
            .method<void(const Options&)>("setOptions", &_::setOptions)({"a_Options"})
            .method<void(StringView, StringView)>("setOption", &_::setOption)({"a_strKey","a_strValue"})
            .method<StringView(StringView) const>("getOption", &_::getOption)({"a_Key"})
            .method<void(Symbol*, Symbol*)>("addAlias", &_::addAlias)({"a_pOriginal","a_pAlias"})
            .method<bool(StringView, StringView)>("addAlias", &_::addAlias)({"a_Original","a_Alias"})
            .method<Symbol*(Symbol*) const>("getAlias", &_::getAlias)({"a_pOriginal"})
            .method<Source*() const>("getSource", &_::getSource)
            .method<void(Source*, Output&)>("translate", &_::translate)({"a_pSource","a_Out"})
            .method<void(Symbol*, String&, LanguageElement*), virtual_>("translateSymbol", &_::translateSymbol)({"a_pSymbol","a_strSymbol","a_pContextScope"})
            .method<void(Expression*, String&, LanguageElement*), virtual_>("translateExpression", &_::translateExpression)({"a_pExpression","a_strExpresson","a_pContextScope"})
            .struct_<TranslationType>()
                .enum_<>().values({
                    {"Declaration",_::TranslationType::Declaration},
                    {"Definition",_::TranslationType::Definition},
                    {"Name",_::TranslationType::Name},
                    {"Typed",_::TranslationType::Typed}})
                .end()
            .end()
            .struct_<TranslationOption>()
                .enum_<>().values({
                    {"Declaration",_::TranslationOption::Declaration},
                    {"Definition",_::TranslationOption::Definition},
                    {"FixedSizeIndexes",_::TranslationOption::FixedSizeIndexes}})
                .end()
            .end()
            .enum_<Flag>().values({
                {"None",_::None},
                {"PrintErrors",_::PrintErrors},
                {"NoSourceMode",_::NoSourceMode}})
            .end()
            .typedef_<Flags>("Flags")
            .method<Flags() const>("getFlags", &_::getFlags)
            .method<void(Flags)>("setFlags", &_::setFlags)({"flags"})
            .method<bool(Flag) const>("hasFlag", &_::hasFlag)({"flag"})
            .method<void(StringView, size_t)>("addForceIncludeTemplateArgument", &_::addForceIncludeTemplateArgument)({"_templateQName","_argIdx"})
            .method<void(Alias*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(AllocateExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(AnonymousSection*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(AnonymousStruct*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(AnonymousUnion*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Application*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ArrayExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Array*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BaseConstructorCallStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Block*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BranchIfNotStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BranchIfStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BranchStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BuiltInConversionExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BuiltInOperator*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BuiltInOperatorExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(CallExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Class*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ClassListInitializationExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ClassTypeListInitializationExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ClassType*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(CommaExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ConditionalExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Constant*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ConstantExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Constructor*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ConstructorCallExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ConstType*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ConstVolatileType*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ContainerClass*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Field*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(FieldExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(FieldInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(FieldPointerExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(FieldPointer*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Pointer*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(DeallocateExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(DeleteExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Ellipsis*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Enum*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Evaluable*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Expression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ExpressionStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Function*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(FunctionPointer*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(FunctionType*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(IdentityExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(InitializerListExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Label*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LanguageElement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LoadExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LocalVariable*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LocalVariableExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LocalVariableInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LValueExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LValueInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LValueReference*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(MemberAnonymousSection*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(MemberAnonymousStruct*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(MemberAnonymousUnion*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Method*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(MethodPointerCallExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(MethodPointer*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(MemberPointer*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Module*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Symbol*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Namespace*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(NewExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Package*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PackageFolder*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Parameter*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Placeholder*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PlaceholderClass*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PlaceholderClassType*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PlaceholderConstant*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PlaceholderTemplate*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PlaceholderType*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PlacementNewExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PrimitiveType*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Property*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PropertyExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Reference*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ReturnStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(RValueReference*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(RValueToConstLValueExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(RValueReferenceExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Scope*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Signature*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Source*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Statement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(StaticAssert*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(StringLiteralExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Structure*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(StructureExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Subroutine*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BinaryOperationExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(UnaryPostOperationExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(UnaryPreOperationExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Template*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateDependantArray*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateDependantDeclType*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateDependantElement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateDependantExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateDependantTemplateInstance*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateDependantType*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateDependantTypeOfExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateParameter*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateSignature*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateSpecialization*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Type*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Union*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ValueMember*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Variable*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(VariableExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(VirtualMethodTable*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(VolatileType*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(VirtualMethodTableSetupStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PointerAdjustmentExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Source*, Sources const&)>("appendIncludes", &_::appendIncludes)({"a_pInput","a_IncludedSources"})
        
        .protected_()
            .method<int(uint, String&)>("translate", &_::translate)({"a_uiPass","a_Out"})
            .method<void(const String&, String&, int)>("translateLiteral", &_::translateLiteral)({"a_Value","translation","options"})["0"]
            .method<void(LanguageElement*, int)>("translate", &_::translate)({"a_pLanguageElement","a_TranslationType"})["TranslationType::Name"]
            .method<void(Type*)>("translateTypeName", &_::translateTypeName)({"a_pType"})
            .method<void(SmallMap<String, Source*>&, Source*)>("_addInclude", &_::_addInclude)({"a_Includes","a_pSource"})
            .method<void(Type*, const String&)>("translateTyped", &_::translateTyped)({"a_pType","a_Identifier"})
            .method<Alias*(Type*)>("_findAliasOf", &_::_findAliasOf)({"a_pType"})
            .method<Alias*(Module*, Type*)>("_findAliasOf", &_::_findAliasOf)({"a_pModule","a_pType"})
            .method<void()>("newLine", &_::newLine)
            .method<void()>("indent", &_::indent)
            .method<void(StringView)>("append", &_::append)({"a_strString"})
            .method<void(const char*)>("append", &_::append)({"a_strString"})
            .method<void(char)>("append", &_::append)({"c"})
            .method<void(const String&)>("add", &_::add)({"a_strString"})
            .method<void(const char*)>("add", &_::add)({"a_strString"})
            .method<void(char*)>("add", &_::add)({"c"})
            .method<void()>("beginDecl", &_::beginDecl)
            .method<void(char)>("beginBlock", &_::beginBlock)({"c"})["'{'"]
            .method<void(char)>("endBlock", &_::endBlock)({"c"})["'}'"]
            .method<void(const char*)>("endBlock", &_::endBlock)({"c"})
            .method<void()>("endDecl", &_::endDecl)
            .method<void(const String&)>("label", &_::label)({"l"})
            .method<void(LanguageElement*)>("pushContextScope", &_::pushContextScope)({"a_pScope"})
            .method<::phantom::lang::LanguageElement *() const>("getContextScope", &_::getContextScope)
            .method<void()>("popContextScope", &_::popContextScope)
            .method<void(Symbol*)>("addInclude", &_::addInclude)({"a_pSymbol"})
            .method<void(SmallMap<String, Source*>&, Symbol*)>("addInclude", &_::addInclude)({"a_Includes","a_pSymbol"})
            .method<void(Type*)>("addForwardH", &_::addForwardH)({"a_pType"})
            .method<void(Type*)>("addForwardCPP", &_::addForwardCPP)({"a_pType"})
            .method<void(Type*)>("addForward", &_::addForward)({"a_pType"})
            .method<void(Namespace*)>("appendNamespaceList", &_::appendNamespaceList)({"a_pNamespace"})
            .method<void(Namespace*)>("appendNamespaceScope", &_::appendNamespaceScope)({"a_pNamespace"})
            .method<void(Namespace*)>("appendNamespaceOpening", &_::appendNamespaceOpening)({"a_pNamespace"})
            .method<void(Namespace*)>("appendNamespaceClosing", &_::appendNamespaceClosing)({"a_pNamespace"})
            .method<void(ClassType*)>("appendNestingClassList", &_::appendNestingClassList)({"a_pClassType"})
            .method<void(const String&)>("appendError", &_::appendError)({"a_Error"})
            .method<void()>("beginElseIf", &_::beginElseIf)
            .method<void()>("beginIf", &_::beginIf)
            .method<void()>("endIf", &_::endIf)
            .method<void()>("beginFor", &_::beginFor)
            .method<void()>("endFor", &_::endFor)
            .method<void()>("beginWhile", &_::beginWhile)
            .method<void()>("endWhile", &_::endWhile)
            .method<void()>("beginDo", &_::beginDo)
            .method<void(bool)>("beginParen", &_::beginParen)({"_a"})
            .method<void(bool)>("endParen", &_::endParen)({"_a"})
            .method<void(Expression*, Expression*, VisitorData)>("appendExpression", &_::appendExpression)({"a_pExpression","","a_Data"})
            .struct_<Operation>()
                .constructor<void(), default_>()
                .constructor<void(Operator, bool)>()({"op","lhs"})
                .field("op", &_::Operation::op)
                .field("lhs", &_::Operation::lhs)
            .end()
            .method<bool(Operation, Operator) const>("requiresParenthesis", &_::requiresParenthesis)({"_left","_right"})
            .method<bool(Operator) const>("requiresParenthesis", &_::requiresParenthesis)({"_right"})
        
        .protected_()
            .field("m_uiFile", &_::m_uiFile)
            .field("m_pSource", &_::m_pSource)
            .field("m_Options", &_::m_Options)
            .field("m_pTranslation", &_::m_pTranslation)
            .field("m_IndentationLevel", &_::m_IndentationLevel)
            .field("m_ContextScopes", &_::m_ContextScopes)
            .field("m_MemberAnonymousSections", &_::m_MemberAnonymousSections)
            .field("m_IncludesH", &_::m_IncludesH)
            .field("m_IncludesCPP", &_::m_IncludesCPP)
            .field("m_ForwardsH", &_::m_ForwardsH)
            .field("m_ForwardsCPP", &_::m_ForwardsCPP)
            .field("m_Aliases", &_::m_Aliases)
            .field("m_Flags", &_::m_Flags)
            .field("m_cIndentChar", &_::m_cIndentChar)
            .field("m_iIndentCharCount", &_::m_iIndentCharCount)
            .field("m_ForceIncludeTemplateArguments", &_::m_ForceIncludeTemplateArguments)
            .field("m_bEndif", &_::m_bEndif)
            .field("m_noAlias", &_::m_noAlias)
            .field("m_ForceQualifiedName", &_::m_ForceQualifiedName)
            .field("m_NoKeywordChars", &_::m_NoKeywordChars)
            .field("m_AliasCache", &_::m_AliasCache)
            .field("m_OperatorStack", &_::m_OperatorStack)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CppTranslator")
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

/// This is your old version, just in case :)
// #pragma once
//
// #include "Translator.hxx"
//
// o_classNB((phantom, reflection), CppTranslator, (Translator))();
//
//
