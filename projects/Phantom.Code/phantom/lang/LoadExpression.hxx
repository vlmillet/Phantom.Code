#pragma once

// haunt {

// clang-format off

#include "LoadExpression.h"

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
    PHANTOM_SOURCE("LoadExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(LoadExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Expression*)>()({"a_pLoadedExpression"})
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::LoadExpression *() const, virtual_>("asLoadExpression", &_::asLoadExpression)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)({"a_Context"})
            .method<Expression*() const>("getLoadedExpression", &_::getLoadedExpression)
            .method<::phantom::lang::LoadExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<bool() const, virtual_>("hasStorage", &_::hasStorage)
            .method<void*(ExecutionContext&) const, virtual_>("evalStorage", &_::evalStorage)({"a_Context"})
            .method<void*() const, virtual_>("getStorage", &_::getStorage)
        
        .protected_()
            .field("m_pLoadedExpression", &_::m_pLoadedExpression)
            .field("m_pArrayAddressingHack", &_::m_pArrayAddressingHack)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LoadExpression")
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
