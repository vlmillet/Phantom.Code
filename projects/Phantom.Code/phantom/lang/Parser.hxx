#pragma once

// haunt {

// clang-format off

#include "Parser.h"

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
#include <phantom/constructor>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Parser")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Parser)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using Options = typedef_< phantom::lang::Options>;
            using StringView = typedef_< phantom::StringView>;
            using Strings = typedef_< phantom::Strings>;
            this_()
        
        .public_()
            .constructor<void(Language*, Source*, Message*)>()({"a_pLanguage","a_pSource","a_pMessage"})
            .method<::phantom::lang::Symbol *(StringView, LanguageElement*, Modifiers, uint), pure_virtual>("parseSymbolName", &_::parseSymbolName)({"a_strSymbol","a_pContextScope","a_Modifiers","a_uiFlags"})["0"]
            .method<::phantom::lang::Expression *(StringView, LanguageElement*), pure_virtual>("parseExpression", &_::parseExpression)({"a_strExpression","a_pContextScope"})
            .method<const Options&() const>("getOptions", &_::getOptions)
            .method<void(StringView, StringView)>("setOption", &_::setOption)({"a_strKey","a_strValue"})
            .method<uint() const>("getPass", &_::getPass)
            .method<uint() const>("getPassCount", &_::getPassCount)
            .method<Language*() const>("getLanguage", &_::getLanguage)
            .method<Message*() const>("getMessage", &_::getMessage)
            .method<StringView(StringView) const>("getOption", &_::getOption)({"a_Key"})
            .method<Source*() const>("getSource", &_::getSource)
            .method<Semantic*() const>("getSemantic", &_::getSemantic)
            .method<void(BuildSessionId), virtual_>("begin", &_::begin)({""})
            .method<int(), virtual_>("lex", &_::lex)
            .method<int(Strings&), virtual_>("fetchModules", &_::fetchModules)({"_modules"})
            .method<int(Strings&), virtual_>("fetchImports", &_::fetchImports)({"_modules"})
            .method<void(BuildSessionId), virtual_>("beginParse", &_::beginParse)({""})
            .method<int(uint), pure_virtual>("parse", &_::parse)({"a_uiPass"})
            .method<int(uint)>("stepParse", &_::stepParse)({"a_uiPass"})
            .method<void(), virtual_>("endParse", &_::endParse)
            .method<void(), virtual_>("end", &_::end)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Parser")
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
