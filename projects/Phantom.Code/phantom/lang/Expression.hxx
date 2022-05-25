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
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
        
        .public_()
            .constructor<void(Type*, Type*, uint)>()({"a_pValueType","a_pEvalType","a_uiFlags"})["nullptr"]["0"]
            .method<void()>("initialize", &_::initialize)
            .using_("Evaluable::eval")
            .method<::phantom::lang::Expression *() const, virtual_>("removeIdentity", &_::removeIdentity)
            .method<::phantom::lang::Expression *() const, virtual_>("removeImplicitIdentity", &_::removeImplicitIdentity)
            .method<::phantom::lang::LanguageElement *() const, virtual_>("removeExpression", &_::removeExpression)
            .method<void(ExecutionContext&) const, pure_virtual>("eval", &_::eval)({"a_Context"})
            .method<Type*() const>("getValueType", &_::getValueType)
            .method<void(void*) const>("load", &_::load)({"a_pOutputBuffer"})
            .method<void(ExecutionContext&) const, virtual_>("load", &_::load)({"a_Context"})
            .method<void(ExecutionContext&, void*) const>("load", &_::load)({"a_Context","a_pResultPointer"})
            .method<void(ExecutionContext&, void*) const>("eval", &_::eval)({"a_Context","a_pResultPointer"})
            .method<void(void const*) const>("store", &_::store)({"a_pInputBuffer"})
            .method<Variant() const>("get", &_::get)
            .method<void(const Variant&)>("set", &_::set)({"v"})
            .method<::phantom::lang::Expression *() const, virtual_>("asExpression", &_::asExpression)
            .method<::phantom::lang::Expression *(Expression*) const, virtual_>("toExpression", &_::toExpression)({"a_pOptExpression"})["nullptr"]
            .method<bool() const, virtual_>("isPersistent", &_::isPersistent)
            .method<void*(ExecutionContext&) const, virtual_>("evalStorage", &_::evalStorage)({"a_Context"})
            .method<void*() const, virtual_>("getStorage", &_::getStorage)
            .method<bool() const>("hasEffectiveAddress", &_::hasEffectiveAddress)
            .method<void*(ExecutionContext&) const>("loadEffectiveAddress", &_::loadEffectiveAddress)({"a_Context"})
            .method<void*() const>("loadEffectiveAddress", &_::loadEffectiveAddress)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)({"a_Context"})
            .method<bool() const, virtual_>("isCompileTime", &_::isCompileTime)
            .method<::phantom::lang::Expression *(LanguageElement*), virtual_>("dereference", &_::dereference)({"a_pOwner"})
            .method<::phantom::lang::Expression *(LanguageElement*), virtual_>("address", &_::address)({"a_pOwner"})
            .method<::phantom::lang::Expression *(LanguageElement*, size_t), virtual_>("arrayAccess", &_::arrayAccess)({"a_pOwner","a_uiIndex"})
            .method<::phantom::lang::Expression *(LanguageElement*, ptrdiff_t, Type*), virtual_>("adjust", &_::adjust)({"a_pOwner","a_iOffset","a_pOutputType"})["nullptr"]
            .method<::phantom::lang::Expression *(LanguageElement*, Expression*), virtual_>("arrayAccess", &_::arrayAccess)({"a_pOwner","a_pIndexExpression"})
            .method<::phantom::lang::Expression *(LanguageElement*, StringView, LanguageElements const*, Expressions const*, LanguageElement*), virtual_>("memberAccess", &_::memberAccess)({"a_pOwner","a_Name","a_pTemplateArguments","a_pFunctionArguments","a_pContextScope"})["nullptr"]
            .method<::phantom::lang::Expression *(LanguageElement*, StringView, Expressions const*, LanguageElement*), virtual_>("memberAccess", &_::memberAccess)({"a_pOwner","a_Name","a_pFunctionArguments","a_pContextScope"})["nullptr"]
            .method<::phantom::lang::Expression *(LanguageElement*, StringView, LanguageElement*), virtual_>("memberAccess", &_::memberAccess)({"a_pOwner","a_Name","a_pContextScope"})["nullptr"]
            .method<::phantom::lang::Expression *(LanguageElement*, Expression*), virtual_>("assign", &_::assign)({"a_pOwner","a_pExpression"})
            .method<::phantom::lang::Expression *(LanguageElement*), virtual_>("load", &_::load)({"a_pOwner"})
            .method<bool(Type*) const, virtual_>("hasEvalConversionTo", &_::hasEvalConversionTo)({"a_pOutputType"})
            .method<Expression*(LanguageElement*) const>("clone", &_::clone)({"a_pOwner"})
            .method<::phantom::lang::Expression *(LanguageElement*) const, pure_virtual>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<::phantom::lang::Expression *() const, virtual_>("removeConversions", &_::removeConversions)
            .method<::phantom::lang::Expression *(), virtual_>("loadRValueReferenceExpression", &_::loadRValueReferenceExpression)
            .method<::phantom::lang::Expression *(), virtual_>("removeRValueReferenceExpression", &_::removeRValueReferenceExpression)
            .method<::phantom::lang::Expression *(), virtual_>("removeRValueToLValueExpression", &_::removeRValueToLValueExpression)
            .method<::phantom::lang::Expression *(), virtual_>("removeRValueStorageExpression", &_::removeRValueStorageExpression)
            .method<void(bool)>("setTemporary", &_::setTemporary)({"a_bValue"})
            .method<bool() const>("isTemporary", &_::isTemporary)
            /// missing symbol(s) reflection (std::ostream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::ostream &, int) const, virtual_|override_>("dump", &_::dump)({"a_Out","a_Lvl"})
        
        .protected_()
            .method<::phantom::lang::Evaluable *(Expression*) const, virtual_|override_>("evaluateExpressionLifeTime", &_::evaluateExpressionLifeTime)({"a_pExpression"})
            .method<::phantom::lang::Evaluable *() const, virtual_>("evaluateLifeTime", &_::evaluateLifeTime)
            .method<void(Symbol*)>("addDependencyOn", &_::addDependencyOn)({""})
        
        .protected_()
            .method<Type*(Type*) const>("storageType", &_::storageType)({"a_pType"})
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
