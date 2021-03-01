#pragma once

// haunt {

// clang-format off

#include "StringLiteralExpression.h"

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
    PHANTOM_SOURCE("StringLiteralExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(StringLiteralExpression)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(StringView, StringView)>()
            .constructor<void(StringView)>()
            .method<void*(ExecutionContext&) const, virtual_|override_>("evalStorage", &_::evalStorage)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)
            .method<const char*const*() const>("getAddress", &_::getAddress)
            .method<const char*() const>("getString", &_::getString)
            .method<StringView() const>("getLiteral", &_::getLiteral)
            .method<bool() const, virtual_|override_>("hasStorage", &_::hasStorage)
            .method<StringLiteralExpression*(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
            .method<bool() const, virtual_|override_>("isCompileTime", &_::isCompileTime)
        
        .protected_()
            .field("m_Literal", &_::m_Literal)
            .field("m_Value", &_::m_Value)
            .field("m_Buffer", &_::m_Buffer)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StringLiteralExpression")
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
