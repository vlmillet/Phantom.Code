#pragma once

// haunt {

// clang-format off

#include "BuiltInOperatorExpression.h"

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
    PHANTOM_SOURCE("BuiltInOperatorExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(BuiltInOperatorExpression)
        {
            using BuiltInOperatorDelegate = typedef_< phantom::lang::BuiltInOperatorDelegate>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(BuiltInOperator*, Type*, Expression* const*, Expression* const*, BuiltInOperatorDelegate)>()
            .constructor<void(BuiltInOperator*, Type*, Expression* const*)>()
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)
            .method<::phantom::lang::BuiltInOperatorExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
            .method<BuiltInOperator*() const>("getOperator", &_::getOperator)
            .method<Expression*(size_t) const>("getEvalArgument", &_::getEvalArgument)
            .method<Expression*(size_t) const>("getInputArgument", &_::getInputArgument)
            .method<bool() const, virtual_>("hasStorage", &_::hasStorage)
            .method<void*(ExecutionContext&) const, virtual_>("evalStorage", &_::evalStorage)
            .method<::phantom::lang::Expression *(LanguageElement*), virtual_|override_>("dereference", &_::dereference)
            .method<::phantom::lang::Expression *(LanguageElement*), virtual_|override_>("address", &_::address)
            .method<::phantom::lang::LanguageElement *() const, virtual_|override_>("removeExpression", &_::removeExpression)
            /// missing symbol(s) reflection (std::ostream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::ostream &, int) const, virtual_|override_>("dump", &_::dump)
        
        .protected_()
            .method<Evaluable*(Expression*) const, virtual_|override_>("evaluateExpressionLifeTime", &_::evaluateExpressionLifeTime)
        
        .protected_()
            .field("m_pOperator", &_::m_pOperator)
            .field("m_EvalArguments", &_::m_EvalArguments)
            .field("m_InputArguments", &_::m_InputArguments)
            .field("m_Delegate", &_::m_Delegate)
            .field("m_Buffer", &_::m_Buffer)
            .field("m_bRequestFlush", &_::m_bRequestFlush)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BuiltInOperatorExpression")
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
