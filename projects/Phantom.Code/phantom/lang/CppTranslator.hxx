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
            .method<void(const Options&)>("setOptions", &_::setOptions)
            .method<void(StringView, StringView)>("setOption", &_::setOption)
            .method<StringView(StringView) const>("getOption", &_::getOption)
            .method<Source*() const>("getSource", &_::getSource)
            .method<void(Source*, Output&)>("translate", &_::translate)
            .method<void(Symbol*, String&, LanguageElement*), virtual_>("translateSymbol", &_::translateSymbol)
            .method<void(Expression*, String&, LanguageElement*), virtual_>("translateExpression", &_::translateExpression)
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
            .method<void(Flags)>("setFlags", &_::setFlags)
            .method<bool(Flag) const>("hasFlag", &_::hasFlag)
            .method<void(StringView, size_t)>("addForceIncludeTemplateArgument", &_::addForceIncludeTemplateArgument)
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
            .method<void(ClassListInitializationExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ClassTypeListInitializationExpression*, VisitorData), virtual_>("visit", &_::visit)
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
            .method<void(Signature*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Source*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Statement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(StaticAssert*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(StringLiteralExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Structure*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(StructureExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Subroutine*, VisitorData), virtual_>("visit", &_::visit)
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
            .method<void(Union*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ValueMember*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Variable*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VariableExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VirtualMethodTable*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VolatileType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VirtualMethodTableSetupStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PointerAdjustmentExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Source*, Sources const&)>("appendIncludes", &_::appendIncludes)
        
        .protected_()
            .method<int(uint, String&)>("translate", &_::translate)
            .method<void(const String&, String&, int)>("translateLiteral", &_::translateLiteral)["0"]
            .method<void(LanguageElement*, int)>("translate", &_::translate)["TranslationType::Name"]
            .method<void(Type*)>("translateTypeName", &_::translateTypeName)
            .method<void(SmallMap<String, Source*>&, Source*)>("_addInclude", &_::_addInclude)
            .method<void(Type*, const String&)>("translateTyped", &_::translateTyped)
            .method<void()>("newLine", &_::newLine)
            .method<void()>("indent", &_::indent)
            .method<void(StringView)>("append", &_::append)
            .method<void(const char*)>("append", &_::append)
            .method<void(char)>("append", &_::append)
            .method<void(const String&)>("add", &_::add)
            .method<void(const char*)>("add", &_::add)
            .method<void(char*)>("add", &_::add)
            .method<void()>("beginDecl", &_::beginDecl)
            .method<void(char)>("beginBlock", &_::beginBlock)["'{'"]
            .method<void(char)>("endBlock", &_::endBlock)["'}'"]
            .method<void(const char*)>("endBlock", &_::endBlock)
            .method<void()>("endDecl", &_::endDecl)
            .method<void(const String&)>("label", &_::label)
            .method<void(LanguageElement*)>("pushContextScope", &_::pushContextScope)
            .method<::phantom::lang::LanguageElement *() const>("getContextScope", &_::getContextScope)
            .method<void()>("popContextScope", &_::popContextScope)
            .method<void(Symbol*)>("addInclude", &_::addInclude)
            .method<void(SmallMap<String, Source*>&, Symbol*)>("addInclude", &_::addInclude)
            .method<void(Type*)>("addForwardH", &_::addForwardH)
            .method<void(Type*)>("addForwardCPP", &_::addForwardCPP)
            .method<void(Type*)>("addForward", &_::addForward)
            .method<void(Namespace*)>("appendNamespaceList", &_::appendNamespaceList)
            .method<void(Namespace*)>("appendNamespaceScope", &_::appendNamespaceScope)
            .method<void(Namespace*)>("appendNamespaceOpening", &_::appendNamespaceOpening)
            .method<void(Namespace*)>("appendNamespaceClosing", &_::appendNamespaceClosing)
            .method<void(ClassType*)>("appendNestingClassList", &_::appendNestingClassList)
            .method<void(const String&)>("appendError", &_::appendError)
            .method<void()>("beginElseIf", &_::beginElseIf)
            .method<void()>("beginIf", &_::beginIf)
            .method<void()>("endIf", &_::endIf)
            .method<void()>("beginFor", &_::beginFor)
            .method<void()>("endFor", &_::endFor)
            .method<void()>("beginWhile", &_::beginWhile)
            .method<void()>("endWhile", &_::endWhile)
            .method<void()>("beginDo", &_::beginDo)
            .method<void(Expression*, Expression*, VisitorData)>("appendExpressionWithPrecedenceSafety", &_::appendExpressionWithPrecedenceSafety)
        
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
            .field("m_Flags", &_::m_Flags)
            .field("m_cIndentChar", &_::m_cIndentChar)
            .field("m_iIndentCharCount", &_::m_iIndentCharCount)
            .field("m_ForceIncludeTemplateArguments", &_::m_ForceIncludeTemplateArguments)
            .field("m_bEndif", &_::m_bEndif)
            .field("m_noAlias", &_::m_noAlias)
            .field("m_ForceQualifiedName", &_::m_ForceQualifiedName)
            .field("m_NoKeywordChars", &_::m_NoKeywordChars)
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
