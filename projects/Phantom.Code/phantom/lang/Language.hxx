#pragma once

// haunt {

// clang-format off

#include "Language.h"

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
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Language")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Language)
        {
            using Languages = typedef_< phantom::lang::Languages>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            this_()
            .inherits<::phantom::Object>()
        
        .public_()
            .constructor<void(StringView, StringView, uint)>()({"a_strName","a_strExt","a_uiParserPassCount"})
            .staticMethod<::phantom::lang::Language *()>("GetDefault", &_::GetDefault)
            .staticMethod<void(Language*)>("SetDefault", &_::SetDefault)({"a_pLanguage"})
            .staticMethod<::phantom::lang::Language *(StringView)>("GetLanguage", &_::GetLanguage)({"a_Name"})
            .staticMethod<const Languages&()>("GetLanguages", &_::GetLanguages)
            .staticMethod<::phantom::lang::Language *(StringView)>("GetFromFileExtension", &_::GetFromFileExtension)({"a_Extension"})
            .method<StringView() const>("getName", &_::getName)
            .method<bool(StringView) const, pure_virtual>("checkSymbolSyntax", &_::checkSymbolSyntax)({"a_strSymbol"})
            .method<::phantom::lang::Symbol *(StringView, LanguageElement*, Modifiers, uint), pure_virtual>("findSymbol", &_::findSymbol)({"a_strSymbol","a_pContextScope","a_Modifiers","a_uiFlags"})["nullptr"]["0"]["0"]
            .method<void(StringView, Symbols&, LanguageElement*), pure_virtual>("findSymbols", &_::findSymbols)({"a_strSymbolsName","a_Symbols","a_pContextScope"})["nullptr"]
            .method<::phantom::lang::Symbol *(StringView, LanguageElement*, Modifiers, uint), virtual_>("parseSymbol", &_::parseSymbol)({"a_strSymbol","a_pContextScope","a_Modifiers","a_uiFlags"})["nullptr"]["0"]["0"]
            .method<::phantom::lang::Type *(StringView, LanguageElement*, Modifiers, uint), virtual_>("findType", &_::findType)({"a_strType","a_pContextScope","a_Modifiers","a_uiFlags"})["nullptr"]["0"]["0"]
            .method<::phantom::lang::Class *(StringView, LanguageElement*, Modifiers, uint), virtual_>("findClass", &_::findClass)({"a_strClass","a_pContextScope","a_Modifiers","a_uiFlags"})["nullptr"]["0"]["0"]
            .method<::phantom::lang::Function *(StringView, LanguageElement*, Modifiers, uint), virtual_>("findFunction", &_::findFunction)({"a_strFunction","a_pContextScope","a_Modifiers","a_uiFlags"})["nullptr"]["0"]["0"]
            .method<::phantom::lang::Expression *(StringView, LanguageElement*), virtual_>("parseExpression", &_::parseExpression)({"a_strExpression","a_pContextScope"})["nullptr"]
            .method<::phantom::lang::Expression *(StringView, LanguageElement*), pure_virtual>("expression", &_::expression)({"a_strExpression","a_pContextScope"})["nullptr"]
            .method<void(Expression*), virtual_>("release", &_::release)({"a_pExpression"})
            .method<StringView() const>("getSourceFileExtension", &_::getSourceFileExtension)
            .method<uint() const>("getParserPassCount", &_::getParserPassCount)
            .method<uint(uint) const, virtual_>("getParserPassPriority", &_::getParserPassPriority)({"a_uiPass"})
            .method<bool(StringView, Variant const&)>("setOption", &_::setOption)({"a_OptionName","a_Value"})
            .method<Variant const&(StringView) const>("getOption", &_::getOption)({"a_OptionName"})
            .method<::phantom::lang::Parser *(Source*, Message*) const, pure_virtual>("createParser", &_::createParser)({"a_pSource","a_pMessage"})
            .method<void(Parser*) const, pure_virtual>("destroyParser", &_::destroyParser)({"a_pParser"})
            .method<::phantom::lang::Semantic *(Source*, Message*) const, pure_virtual>("createSemantic", &_::createSemantic)({"a_pSource","a_pMessage"})
            .method<void(Semantic*) const, pure_virtual>("destroySemantic", &_::destroySemantic)({"a_pSemantic"})
        
        .protected_()
            .method<void(StringView, Variant const&)>("addOption", &_::addOption)({"a_OptionName","a_DefaultValue"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Language")
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
