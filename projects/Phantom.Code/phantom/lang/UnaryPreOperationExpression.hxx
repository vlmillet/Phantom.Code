#pragma once

// haunt {

// clang-format off

#include "UnaryPreOperationExpression.h"

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

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("UnaryPreOperationExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(UnaryPreOperationExpression)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Type*, StringView, Expression*)>()({"a_pType","a_strOperator","a_pExpression"})
            .constructor<void(StringView, Expression*)>()({"a_strOperator","a_pExpression"})
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)({"a_Context"})
            .method<Expression*() const>("getOperandExpression", &_::getOperandExpression)
            .method<StringView() const>("getOperatorName", &_::getOperatorName)
            .method<::phantom::lang::UnaryPreOperationExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
        
        .protected_()
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)({""})
        
        .protected_()
            .field("m_strOperator", &_::m_strOperator)
            .field("m_pOperandExpression", &_::m_pOperandExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("UnaryPreOperationExpression")
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
