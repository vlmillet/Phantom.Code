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
            this_()
        
        .public_()
            .constructor<void(Language*, Source*, Message*)>()
            .method<::phantom::lang::Symbol *(StringView, LanguageElement*, Modifiers, uint), pure_virtual>("parseSymbolName", &_::parseSymbolName)["0"]
            .method<::phantom::lang::Expression *(StringView, LanguageElement*), pure_virtual>("parseExpression", &_::parseExpression)
            .method<const Options&() const>("getOptions", &_::getOptions)
            .method<void(StringView, StringView)>("setOption", &_::setOption)
            .method<uint() const>("getPass", &_::getPass)
            .method<uint() const>("getPassCount", &_::getPassCount)
            .method<Language*() const>("getLanguage", &_::getLanguage)
            .method<Message*() const>("getMessage", &_::getMessage)
            .method<StringView(StringView) const>("getOption", &_::getOption)
            .method<Source*() const>("getSource", &_::getSource)
            .method<Semantic*() const>("getSemantic", &_::getSemantic)
            .method<void(BuildSession&), virtual_>("begin", &_::begin)
            .method<void(), virtual_>("end", &_::end)
            .method<int(uint), pure_virtual>("parse", &_::parse)
            .method<int(uint)>("stepParse", &_::stepParse)
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
