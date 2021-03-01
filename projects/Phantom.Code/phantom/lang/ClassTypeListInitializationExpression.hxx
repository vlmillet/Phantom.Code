#pragma once

// haunt {

// clang-format off

#include "ClassTypeListInitializationExpression.h"

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
    PHANTOM_SOURCE("ClassTypeListInitializationExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ClassTypeListInitializationExpression)
        {
            using Expressions = typedef_< phantom::lang::Expressions>;
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(ClassType*, ExpressionsView)>()
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::ClassTypeListInitializationExpression *() const, virtual_>("asClassTypeListInitializationExpression", &_::asClassTypeListInitializationExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<::phantom::lang::ClassTypeListInitializationExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
            .method<Expressions const&() const>("getExpressions", &_::getExpressions)
            .method<ClassType*() const>("getClassType", &_::getClassType)
        
        .protected_()
            .field("m_pClassType", &_::m_pClassType)
            .field("m_Expressions", &_::m_Expressions)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ClassTypeListInitializationExpression")
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
