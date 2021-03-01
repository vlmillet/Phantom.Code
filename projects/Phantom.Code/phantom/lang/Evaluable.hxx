#pragma once

// haunt {

// clang-format off

#include "Evaluable.h"

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
    PHANTOM_SOURCE("Evaluable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Evaluable)
        {
            using Expressions = typedef_< phantom::lang::Expressions>;
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            using VoidPtrs = typedef_< phantom::VoidPtrs>;
            this_()
            .inherits<::phantom::lang::LanguageElement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(uint)>()["0"]
            .constructor<void(Type*, uint)>()["0"]
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::Evaluable *() const, virtual_>("asEvaluable", &_::asEvaluable)
            .method<CompiledSource*() const>("getCompiledSource", &_::getCompiledSource)
            .method<Semantic*() const>("getSemantic", &_::getSemantic)
            .method<Language*() const>("getLanguage", &_::getLanguage)
            .method<void()>("setImplicit", &_::setImplicit)
            .method<bool() const>("isImplicit", &_::isImplicit)
            .method<void() const>("eval", &_::eval)
            .method<void(void*) const>("eval", &_::eval)
            .method<void(ExecutionContext&) const, pure_virtual>("eval", &_::eval)
            .method<::phantom::lang::Evaluable *(Expression*) const, virtual_>("evaluateExpressionLifeTime", &_::evaluateExpressionLifeTime)
            .method<void(Expression*)>("addScopedDestruction", &_::addScopedDestruction)
            .method<Expressions const&() const>("getScopedDestructions", &_::getScopedDestructions)
            .method<Type*() const>("getEvalType", &_::getEvalType)
            .method<bool() const, virtual_>("hasStorage", &_::hasStorage)
            .method<bool() const>("hasStorageCascade", &_::hasStorageCascade)
            .method<ExpressionsView() const>("getSubExpressions", &_::getSubExpressions)
            /// missing symbol(s) reflection (std::ostream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::ostream &, int) const, virtual_>("dump", &_::dump)["0"]
        
        .protected_()
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)
            .method<void(ExecutionContext&, Type*, void*) const>("pushTempDestruction", &_::pushTempDestruction)
            .method<void(ExecutionContext&, ExpressionsView, VoidPtrs&) const>("evaluateArguments", &_::evaluateArguments)
            .method<void(Expression*&)>("addSubExpression", &_::addSubExpression)
            .method<void(Expression*)>("removeSubExpression", &_::removeSubExpression)
        
        .protected_()
            .method<void(ExecutionContext&) const>("evalScopedDestructions", &_::evalScopedDestructions)
            .method<::phantom::lang::Evaluable *() const, virtual_>("evaluateLifeTime", &_::evaluateLifeTime)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Evaluable")
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
