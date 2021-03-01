#pragma once

// haunt {

// clang-format off

#include "BuiltInConversionExpression.h"

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

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("BuiltInConversionExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(BuiltInConversionExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Expression*, Conversion*, CastKind)>()
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)
            .method<Expression*() const>("getInputExpression", &_::getInputExpression)
            .method<::phantom::lang::BuiltInConversionExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
            .method<const ::phantom::lang::Conversion *() const>("getConversion", &_::getConversion)
            .method<CastKind() const>("getConversionType", &_::getConversionType)
            .method<::phantom::lang::Expression *() const, virtual_>("removeConversions", &_::removeConversions)
            .method<::phantom::lang::LanguageElement *() const, virtual_|override_>("removeExpression", &_::removeExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BuiltInConversionExpression")
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
