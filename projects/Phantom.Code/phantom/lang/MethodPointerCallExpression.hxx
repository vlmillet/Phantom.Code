#pragma once

// haunt {

// clang-format off

#include "MethodPointerCallExpression.h"

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
    PHANTOM_SOURCE("MethodPointerCallExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(MethodPointerCallExpression)
        {
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Expression*, Expression*, ExpressionsView)>()
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)
            .method<Expression*() const>("getObjectExpression", &_::getObjectExpression)
            .method<Expression*() const>("getMemberExpression", &_::getMemberExpression)
            .method<MethodPointer*() const>("getMethodPointer", &_::getMethodPointer)
            .method<ExpressionsView() const>("getArguments", &_::getArguments)
            .method<::phantom::lang::MethodPointerCallExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
        
        .protected_()
            .method<void(ExecutionContext&) const, virtual_|override_>("flush", &_::flush)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MethodPointerCallExpression")
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
