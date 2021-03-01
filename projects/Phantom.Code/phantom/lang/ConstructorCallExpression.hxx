#pragma once

// haunt {

// clang-format off

#include "ConstructorCallExpression.h"

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

#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("ConstructorCallExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ConstructorCallExpression)
        {
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Constructor*)>()
            .constructor<void(Constructor*, ExpressionsView)>()
            .constructor<void(Constructor*, Expression*)>()
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<::phantom::lang::ConstructorCallExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
            .method<Constructor*() const>("getConstructor", &_::getConstructor)
            .method<::phantom::SmallVector<Expression *, 4>::const_iterator() const>("beginArguments", &_::beginArguments)
            .method<::phantom::SmallVector<Expression *, 4>::const_iterator() const>("endArguments", &_::endArguments)
            .method<ExpressionsView() const>("getArguments", &_::getArguments)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ConstructorCallExpression")
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
