#pragma once

// haunt {

// clang-format off

#include "ArrayExpression.h"

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
    PHANTOM_SOURCE("ArrayExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ArrayExpression)
        {
            using Expressions = typedef_< phantom::lang::Expressions>;
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Array*, ExpressionsView)>()({"a_pArray","a_Values"})
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::ArrayExpression *() const, virtual_>("asArrayExpression", &_::asArrayExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::ArrayExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<ExpressionsView() const>("getValues", &_::getValues)
        
        .protected_()
            .field("m_Values", &_::m_Values)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ArrayExpression")
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
