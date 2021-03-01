#pragma once

// haunt {

// clang-format off

#include "UnaryPostOperationExpression.h"

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
    PHANTOM_SOURCE("UnaryPostOperationExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(UnaryPostOperationExpression)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Type*, StringView, Expression*)>()
            .constructor<void(StringView, Expression*)>()
            .method<void()>("initialize", &_::initialize)
            .method<Expression*() const>("getOperandExpression", &_::getOperandExpression)
            .method<StringView() const>("getOperatorName", &_::getOperatorName)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)
            .method<::phantom::lang::UnaryPostOperationExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
        
        .protected_()
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)
        
        .protected_()
            .field("m_strOperator", &_::m_strOperator)
            .field("m_pOperandExpression", &_::m_pOperandExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("UnaryPostOperationExpression")
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
