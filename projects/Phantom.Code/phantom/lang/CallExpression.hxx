#pragma once

// haunt {

// clang-format off

#include "CallExpression.h"

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

#include <phantom/utils/Functor.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("CallExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(CallExpression)
        {
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Subroutine*, ExpressionsView)>()
            .constructor<void(Subroutine*, Expression*)>()
            .constructor<void(Subroutine*)>()
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<ExpressionsView() const>("getArguments", &_::getArguments)
            .method<bool(Semantic*, size_t, ::phantom::Functor<Expression *(Expression *)>)>("transformArgument", &_::transformArgument)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)
            .method<Subroutine*() const>("getSubroutine", &_::getSubroutine)
            .method<void()>("setFinal", &_::setFinal)
            .method<bool() const>("isFinal", &_::isFinal)
            .method<::phantom::lang::LanguageElement *(), virtual_>("hatchExpression", &_::hatchExpression)
            .method<::phantom::lang::Expression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
            .method<::phantom::lang::LanguageElement *() const, virtual_|override_>("removeExpression", &_::removeExpression)
            .method<bool() const, virtual_>("isPersistent", &_::isPersistent)
            /// missing symbol(s) reflection (std::ostream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::ostream &, int) const, virtual_|override_>("dump", &_::dump)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CallExpression")
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
