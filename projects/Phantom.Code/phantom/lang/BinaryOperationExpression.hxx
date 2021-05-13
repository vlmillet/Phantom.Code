#pragma once

// haunt {

// clang-format off

#include "BinaryOperationExpression.h"

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
    PHANTOM_SOURCE("BinaryOperationExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(BinaryOperationExpression)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(StringView, Expression*, Expression*)>()({"a_strOperator","a_pLeftExpression","a_pRightExpression"})
            .constructor<void(Type*, StringView, Expression*, Expression*)>()({"a_pValueType","a_strOperator","a_pLeftExpression","a_pRightExpression"})
            .method<void()>("initialize", &_::initialize)
            .method<StringView() const>("getOperatorName", &_::getOperatorName)
            .method<Expression*() const>("getLeftOperandExpression", &_::getLeftOperandExpression)
            .method<Expression*() const>("getRightOperandExpression", &_::getRightOperandExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({""})
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)({"a_Context"})
            .method<bool() const, virtual_>("hasStorage", &_::hasStorage)
            .method<::phantom::lang::BinaryOperationExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
        
        .protected_()
            .field("m_strOperator", &_::m_strOperator)
            .field("m_pLeftOperandExpression", &_::m_pLeftOperandExpression)
            .field("m_pRightOperandExpression", &_::m_pRightOperandExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BinaryOperationExpression")
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
