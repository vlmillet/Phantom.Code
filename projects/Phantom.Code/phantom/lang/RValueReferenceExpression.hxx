#pragma once

// haunt {

// clang-format off

#include "RValueReferenceExpression.h"

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
    PHANTOM_SOURCE("RValueReferenceExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(RValueReferenceExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Expression*)>()
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)
            .method<void(ExecutionContext&) const, virtual_|override_>("flush", &_::flush)
            .method<Expression*() const>("getRValueExpression", &_::getRValueExpression)
            .method<RValueReferenceExpression*(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
            .method<void(ExecutionContext&) const, virtual_|override_>("load", &_::load)
            .method<Expression*(LanguageElement*), virtual_|override_>("load", &_::load)
            .method<Expression*(), virtual_|override_>("loadRValueReferenceExpression", &_::loadRValueReferenceExpression)
            .method<Expression*(), virtual_|override_>("removeRValueReferenceExpression", &_::removeRValueReferenceExpression)
            .method<Expression*(), virtual_|override_>("removeRValueToLValueExpression", &_::removeRValueToLValueExpression)
            .method<LanguageElement*(), virtual_|override_>("hatchExpression", &_::hatchExpression)
            .method<LanguageElement*() const, virtual_|override_>("removeExpression", &_::removeExpression)
        
        .protected_()
            .field("m_pRValueExpression", &_::m_pRValueExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("RValueReferenceExpression")
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
