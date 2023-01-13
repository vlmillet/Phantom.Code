#pragma once

// haunt {

// clang-format off

#include "CppLiteParser.h"

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
#include <phantom/enum>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/Flags.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("CppLiteParser")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(CppLiteParser)
        {
            using EFlag = typedef_<_::EFlag>;
            using EPass = typedef_<_::EPass>;
            using Flags = typedef_<_::Flags>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Parser>()
        
        .public_()
            .enum_<EFlag>().values({
                {"e_Flag_None",_::e_Flag_None},
                {"e_Flag_PrintErrors",_::e_Flag_PrintErrors},
                {"e_Flag_StandardLookup",_::e_Flag_StandardLookup},
                {"e_Flag_SymbolMode",_::e_Flag_SymbolMode},
                {"e_Flag_NoCodeLocation",_::e_Flag_NoCodeLocation}})
            .end()
            .enum_<EPass>().values({
                {"e_Pass_Imports",_::e_Pass_Imports},
                {"e_Pass_Globals",_::e_Pass_Globals},
                {"e_Pass_Inheritance",_::e_Pass_Inheritance},
                {"e_Pass_MemberLocal",_::e_Pass_MemberLocal},
                {"e_Pass_MemberGlobal",_::e_Pass_MemberGlobal},
                {"e_Pass_Sizing",_::e_Pass_Sizing},
                {"e_Pass_FieldDefaults",_::e_Pass_FieldDefaults},
                {"e_Pass_Implicits",_::e_Pass_Implicits},
                {"e_Pass_Blocks",_::e_Pass_Blocks},
                {"e_Pass_Count",_::e_Pass_Count}})
            .end()
            .typedef_<Flags>("Flags")
            .constructor<void(Source*, Message*, Flags)>()({"a_pSource","a_pMessage","a_Flags"})["e_Flag_None"]
            .method<Flags() const>("getFlags", &_::getFlags)
            .method<void(Flags)>("setFlags", &_::setFlags)({"flags"})
            .method<bool(EFlag) const>("hasFlag", &_::hasFlag)({"flag"})
            .method<::phantom::lang::Symbol *(StringView, LanguageElement*, Modifiers, uint), virtual_>("parseSymbolName", &_::parseSymbolName)({"a_strSymbol","a_pScope","a_Modifiers","a_uiFlags"})
            .method<::phantom::lang::Expression *(StringView, LanguageElement*), virtual_>("parseExpression", &_::parseExpression)({"a_strName","a_pScope"})
            .method<CppLite*() const>("getLanguage", &_::getLanguage)
            .method<bool(StringView) const, virtual_>("checkSymbolSyntax", &_::checkSymbolSyntax)({"a_strSymbol"})
            .method<size_t() const>("getErrorCount", &_::getErrorCount)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CppLiteParser")
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
