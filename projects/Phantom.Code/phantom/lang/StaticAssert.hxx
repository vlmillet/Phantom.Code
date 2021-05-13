#pragma once

// haunt {

// clang-format off

#include "StaticAssert.h"

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
    PHANTOM_SOURCE("StaticAssert")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(StaticAssert)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::LanguageElement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Expression*, StringView)>()({"a_pExpression","a_Text"})
            .method<::phantom::lang::StaticAssert *() const, virtual_>("asStaticAssert", &_::asStaticAssert)
            .method<Expression*() const>("getExpression", &_::getExpression)
            .method<StringView() const>("getText", &_::getText)
        
        .protected_()
            .field("m_pExpression", &_::m_pExpression)
            .field("m_Text", &_::m_Text)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StaticAssert")
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
