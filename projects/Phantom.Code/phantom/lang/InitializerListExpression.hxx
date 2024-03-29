#pragma once

// haunt {

// clang-format off

#include "InitializerListExpression.h"

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

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("InitializerListExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(InitializerListExpression)
        {
            using Expressions = typedef_< phantom::lang::Expressions>;
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(InitializerListType*, ExpressionsView)>()({"a_pType","a_Expressions"})
            .constructor<void(Class*, ExpressionsView)>()({"a_pInitializerListClass","a_Expressions"})
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<::phantom::lang::InitializerListExpression *() const, virtual_>("asInitializerListExpression", &_::asInitializerListExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::InitializerListExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<Expressions const&() const>("getExpressions", &_::getExpressions)
            .method<Type*() const>("getContentType", &_::getContentType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("InitializerListExpression")
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
