#pragma once

// haunt {

// clang-format off

#include "Expression.h"

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
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Expression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Expression)
        {
            using Expressions = typedef_< phantom::lang::Expressions>;
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Evaluable>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
        
        .public_()
            .constructor<void(Type*, Type*, uint)>()["nullptr"]["0"]
            .method<void()>("initialize", &_::initialize)
            .using_("Evaluable::eval")
            .method<::phantom::lang::Expression *() const, virtual_>("removeIdentity", &_::removeIdentity)
            .method<::phantom::lang::Expression *() const, virtual_>("removeImplicitIdentity", &_::removeImplicitIdentity)
            .method<::phantom::lang::LanguageElement *() const, virtual_>("removeExpression", &_::removeExpression)
            .method<void(ExecutionContext&) const, pure_virtual>("eval", &_::eval)
            .method<Type*() const>("getValueType", &_::getValueType)
            .method<void(void*) const>("load", &_::load)
            .method<void(ExecutionContext&) const, virtual_>("load", &_::load)
            .method<void(ExecutionContext&, void*) const>("load", &_::load)
            .method<void(ExecutionContext&, void*) const>("eval", &_::eval)
            .method<void(void const*) const>("store", &_::store)
            .method<Variant() const>("get", &_::get)
            .method<void(const Variant&)>("set", &_::set)
            .method<::phantom::lang::Expression *() const, virtual_>("asExpression", &_::asExpression)
            .method<::phantom::lang::Expression *(Expression*) const, virtual_>("toExpression", &_::toExpression)["nullptr"]
            .method<bool() const, virtual_>("isPersistent", &_::isPersistent)
            .method<void*(ExecutionContext&) const, virtual_>("evalStorage", &_::evalStorage)
            .method<void*() const, virtual_>("getStorage", &_::getStorage)
            .method<bool() const>("hasEffectiveAddress", &_::hasEffectiveAddress)
            .method<void*(ExecutionContext&) const>("loadEffectiveAddress", &_::loadEffectiveAddress)
            .method<void*() const>("loadEffectiveAddress", &_::loadEffectiveAddress)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)
            .method<bool() const, virtual_>("isCompileTime", &_::isCompileTime)
            .method<::phantom::lang::Expression *(LanguageElement*), virtual_>("dereference", &_::dereference)
            .method<::phantom::lang::Expression *(LanguageElement*), virtual_>("address", &_::address)
            .method<::phantom::lang::Expression *(LanguageElement*, size_t), virtual_>("arrayAccess", &_::arrayAccess)
            .method<::phantom::lang::Expression *(LanguageElement*, ptrdiff_t, Type*), virtual_>("adjust", &_::adjust)["nullptr"]
            .method<::phantom::lang::Expression *(LanguageElement*, Expression*), virtual_>("arrayAccess", &_::arrayAccess)
            .method<::phantom::lang::Expression *(LanguageElement*, StringView, LanguageElements const*, Expressions const*, LanguageElement*), virtual_>("memberAccess", &_::memberAccess)["nullptr"]
            .method<::phantom::lang::Expression *(LanguageElement*, StringView, Expressions const*, LanguageElement*), virtual_>("memberAccess", &_::memberAccess)["nullptr"]
            .method<::phantom::lang::Expression *(LanguageElement*, StringView, LanguageElement*), virtual_>("memberAccess", &_::memberAccess)["nullptr"]
            .method<::phantom::lang::Expression *(LanguageElement*, Expression*), virtual_>("assign", &_::assign)
            .method<::phantom::lang::Expression *(LanguageElement*), virtual_>("load", &_::load)
            .method<bool(Type*) const, virtual_>("hasEvalConversionTo", &_::hasEvalConversionTo)
            .method<Expression*(LanguageElement*) const>("clone", &_::clone)
            .method<::phantom::lang::Expression *(LanguageElement*) const, pure_virtual>("cloneImpl", &_::cloneImpl)
            .method<::phantom::lang::Expression *() const, virtual_>("removeConversions", &_::removeConversions)
            .method<::phantom::lang::Expression *(), virtual_>("loadRValueReferenceExpression", &_::loadRValueReferenceExpression)
            .method<::phantom::lang::Expression *(), virtual_>("removeRValueReferenceExpression", &_::removeRValueReferenceExpression)
            .method<::phantom::lang::Expression *(), virtual_>("removeRValueToLValueExpression", &_::removeRValueToLValueExpression)
            .method<void(bool)>("setTemporary", &_::setTemporary)
            .method<bool() const>("isTemporary", &_::isTemporary)
            /// missing symbol(s) reflection (std::ostream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::ostream &, int) const, virtual_|override_>("dump", &_::dump)
        
        .protected_()
            .method<::phantom::lang::Evaluable *(Expression*) const, virtual_|override_>("evaluateExpressionLifeTime", &_::evaluateExpressionLifeTime)
            .method<::phantom::lang::Evaluable *() const, virtual_>("evaluateLifeTime", &_::evaluateLifeTime)
            .method<void(Symbol*)>("addDependencyOn", &_::addDependencyOn)
        
        .protected_()
            .method<Type*(Type*) const>("storageType", &_::storageType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Expression")
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
