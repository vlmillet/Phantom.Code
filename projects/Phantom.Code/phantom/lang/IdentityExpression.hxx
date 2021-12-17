#pragma once

// haunt {

// clang-format off

#include "IdentityExpression.h"

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
    PHANTOM_SOURCE("IdentityExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(IdentityExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Type*, Expression*)>()({"a_pValueType","a_pExpression"})
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::Expression *() const, virtual_>("removeIdentity", &_::removeIdentity)
            .method<::phantom::lang::Expression *() const, virtual_>("removeImplicitIdentity", &_::removeImplicitIdentity)
            .method<::phantom::lang::LanguageElement *() const, virtual_|override_>("removeExpression", &_::removeExpression)
            .method<Expression*() const>("getExpression", &_::getExpression)
            .method<::phantom::lang::IdentityExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<bool() const, virtual_>("hasStorage", &_::hasStorage)
            .method<void*(ExecutionContext&) const, virtual_>("evalStorage", &_::evalStorage)({"a_Context"})
            .method<void*() const, virtual_>("getStorage", &_::getStorage)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)({"a_Context"})
            .method<::phantom::lang::Expression *() const, virtual_>("removeConversions", &_::removeConversions)
            .method<void(CastKind)>("setConversionKind", &_::setConversionKind)({"a_CastKind"})
            .method<CastKind() const>("getConversionKind", &_::getConversionKind)
        
        .protected_()
            .field("m_pExpression", &_::m_pExpression)
            .field("m_CastKind", &_::m_CastKind)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("IdentityExpression")
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
