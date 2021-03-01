#pragma once

// haunt {

// clang-format off

#include "ReturnStatement.h"

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

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("ReturnStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ReturnStatement)
        {
            using String = typedef_< phantom::String>;
            this_()
            .inherits<::phantom::lang::ControlStatement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void()>()
            .constructor<void(Expression*)>()
            .method<void()>("initialize", &_::initialize)
            .method<ReturnStatement*() const, virtual_|override_>("asReturnStatement", &_::asReturnStatement)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)
            .method<Expression*() const>("getExpression", &_::getExpression)
            .method<Statement*() const, virtual_|override_>("getTargetStatement", &_::getTargetStatement)
        
        .protected_()
            .method<void()>("checkValidity", &_::checkValidity)
            .method<void()>("_setupExpression", &_::_setupExpression)
            .method<void(Block*), virtual_|override_>("onAttachedToBlock", &_::onAttachedToBlock)
        
        .protected_()
            .field("m_pReturnExpression", &_::m_pReturnExpression)
            .field("m_pConvertedReturnExpression", &_::m_pConvertedReturnExpression)
            .field("m_pExpressionString", &_::m_pExpressionString)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ReturnStatement")
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
