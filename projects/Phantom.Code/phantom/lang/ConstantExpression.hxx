#pragma once

// haunt {

// clang-format off

#include "ConstantExpression.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/friend>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("ConstantExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ConstantExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
        
        .public_()
            .constructor<void(Constant*)>()({"a_pConstant"})
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<void(void const*) const, virtual_>("setValue", &_::setValue)({"a_pSrc"})
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<Constant*() const>("getConstant", &_::getConstant)
            .method<bool() const, virtual_>("hasStorage", &_::hasStorage)
            .method<void*(ExecutionContext&) const, virtual_|override_>("evalStorage", &_::evalStorage)({"a_Context"})
            .method<::phantom::lang::LanguageElement *(), virtual_>("hatchExpression", &_::hatchExpression)
            .method<::phantom::lang::LanguageElement *() const, virtual_|override_>("removeExpression", &_::removeExpression)
            .method<::phantom::lang::ConstantExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<bool() const, virtual_>("isPersistent", &_::isPersistent)
            .method<bool() const, virtual_|override_>("isCompileTime", &_::isCompileTime)
        
        .protected_()
            .field("m_pConstant", &_::m_pConstant)
            .field("m_pTempValue", &_::m_pTempValue)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT_T((class), (T), ConstantExpressionH)
        {
            this_()
            .PHANTOM_T staticMethod<::phantom::lang::ConstantExpression *(T)>("Create", &_::Create)({"val"})
            ;
        }
        PHANTOM_STRUCT_TS((class), (T), (T*), ConstantExpressionH)
        {
            this_()
            .PHANTOM_T staticMethod<::phantom::lang::Expression *(T*)>("Create", &_::Create)({"val"})
            ;
        }
    PHANTOM_END("ConstantExpression")
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
