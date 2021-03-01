#pragma once

// haunt {

// clang-format off

#include "WStringLiteralExpression.h"

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
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("WStringLiteralExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(WStringLiteralExpression)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            /// missing symbol(s) reflection (std::wstring) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(StringView, const ::std::wstring &)>()
            .method<void*(ExecutionContext&) const, virtual_>("evalStorage", &_::evalStorage)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<const wchar_t*() const>("getString", &_::getString)
            .method<StringView() const>("getLiteral", &_::getLiteral)
            .method<bool() const, virtual_>("hasStorage", &_::hasStorage)
            .method<::phantom::lang::WStringLiteralExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
        
        .protected_()
            .field("m_Literal", &_::m_Literal)
            /// missing symbol(s) reflection (std::wstring) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("m_Value", &_::m_Value)
            .field("m_Buffer", &_::m_Buffer)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("WStringLiteralExpression")
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
