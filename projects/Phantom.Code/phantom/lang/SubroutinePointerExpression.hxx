#pragma once

// haunt {

// clang-format off

#include "SubroutinePointerExpression.h"

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

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("SubroutinePointerExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(SubroutinePointerExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Source*, Subroutine*)>()
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<::phantom::lang::SubroutinePointerExpression *() const, virtual_>("asSubroutinePointerExpression", &_::asSubroutinePointerExpression)
            .method<Subroutine*() const>("getSubroutine", &_::getSubroutine)
        
        .protected_()
            .method<::phantom::lang::SubroutinePointerExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
        
        .protected_()
            .field("m_pSubroutine", &_::m_pSubroutine)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SubroutinePointerExpression")
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
