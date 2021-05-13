#pragma once

// haunt {

// clang-format off

#include "CppLite.h"

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
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("CppLite")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(CppLite)
        {
            using EFlag = typedef_<_::EFlag>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            this_()("name", "CppLite Language")(phantom::lang::Modifiers(PHANTOM_R_SINGLETON))(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Language>()
        
        .public_()
        
        .public_()
            .constructor<void()>()
            .staticMethod<void()>("Load", &_::Load)
            .enum_<EFlag>().values({
                {"e_Flag_CppCompatible",_::e_Flag_CppCompatible}})
            .end()
            .method<bool(StringView) const, virtual_>("checkSymbolSyntax", &_::checkSymbolSyntax)({"a_strSymbol"})
            .method<bool(StringView) const>("isIdentifier", &_::isIdentifier)({"a_strName"})
            .method<::phantom::lang::Symbol *(StringView, LanguageElement*, Modifiers, uint), virtual_|override_>("findSymbol", &_::findSymbol)({"a_strSymbol","a_pContextScope","a_Modifiers","a_uiFlags"})["nullptr"]["0"]["0"]
            .method<void(StringView, Symbols&, LanguageElement*), virtual_|override_>("findSymbols", &_::findSymbols)({"a_strSymbolsName","a_Symbols","a_pContextScope"})["nullptr"]
            .method<::phantom::lang::Symbol *(StringView, LanguageElement*, Modifiers, uint), virtual_|override_>("parseSymbol", &_::parseSymbol)({"a_strSymbol","a_pContextScope","a_Modifiers","a_uiFlags"})["nullptr"]["0"]["0"]
            .method<::phantom::lang::Expression *(StringView, LanguageElement*), virtual_|override_>("expression", &_::expression)({"a_strExpression","a_pContextScope"})["nullptr"]
            .method<Expression*(StringView, Message*, LanguageElement*)>("expression", &_::expression)({"a_strExpression","a_pMessage","a_pContextScope"})["nullptr"]
            .method<::phantom::lang::Expression *(StringView, LanguageElement*), virtual_|override_>("parseExpression", &_::parseExpression)({"a_strExpression","a_pContextScope"})["nullptr"]
            .method<Expression*(StringView, Message*, LanguageElement*)>("parseExpression", &_::parseExpression)({"a_strExpression","a_pMessage","a_pContextScope"})["nullptr"]
            .method<Semantic*() const>("getDefaultSemantic", &_::getDefaultSemantic)
            .method<Operator(StringView) const>("getBuiltInBinaryOperatorId", &_::getBuiltInBinaryOperatorId)({"a_strName"})
            .method<Operator(StringView) const>("getBuiltInPreUnaryOperatorId", &_::getBuiltInPreUnaryOperatorId)({"a_strName"})
            .method<Operator(StringView) const>("getBuiltInPostUnaryOperatorId", &_::getBuiltInPostUnaryOperatorId)({"a_strName"})
            .method<BuiltInOperator*(StringView) const>("getBuiltInBinaryOperator", &_::getBuiltInBinaryOperator)({"a_strName"})
            .method<BuiltInOperator*(StringView) const>("getBuiltInPreUnaryOperator", &_::getBuiltInPreUnaryOperator)({"a_strName"})
            .method<BuiltInOperator*(StringView) const>("getBuiltInPostUnaryOperator", &_::getBuiltInPostUnaryOperator)({"a_strName"})
            .method<BuiltInOperator*(Operator) const>("getBuiltInOperator", &_::getBuiltInOperator)({"a_Op"})
            .method<Source*() const>("getDefaultSource", &_::getDefaultSource)
        
        .protected_()
            .method<Parser*(Source*, Message*) const, virtual_|override_>("createParser", &_::createParser)({"a_pSource","a_pMessage"})
            .method<void(Parser*) const, virtual_|override_>("destroyParser", &_::destroyParser)({"a_pParser"})
            .method<Semantic*(Source*, Message*) const, virtual_|override_>("createSemantic", &_::createSemantic)({"a_pSource","a_pMessage"})
            .method<void(Semantic*) const, virtual_|override_>("destroySemantic", &_::destroySemantic)({"a_pSemantic"})
        
        .public_()
            .staticMethod<::phantom::lang::CppLite *()>("Get", &_::Get)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CppLite")
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
