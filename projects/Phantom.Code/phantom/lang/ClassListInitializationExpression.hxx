#pragma once

// haunt {

// clang-format off

#include "ClassListInitializationExpression.h"

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
    PHANTOM_SOURCE("ClassListInitializationExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ClassListInitializationExpression)
        {
            using Expressions = typedef_< phantom::lang::Expressions>;
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Class*, ExpressionsView)>()({"a_pClass","a_Expressions"})
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::ClassListInitializationExpression *() const, virtual_>("asClassListInitializationExpression", &_::asClassListInitializationExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::ClassListInitializationExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<Expressions const&() const>("getExpressions", &_::getExpressions)
            .method<Class*() const>("getClass", &_::getClass)
        
        .protected_()
            .field("m_pClass", &_::m_pClass)
            .field("m_Expressions", &_::m_Expressions)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ClassListInitializationExpression")
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
