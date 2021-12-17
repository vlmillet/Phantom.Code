#pragma once

// haunt {

// clang-format off

#include "DeleteExpression.h"

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
    PHANTOM_SOURCE("DeleteExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(DeleteExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Class*, Expression*)>()({"a_pClass","a_pExpression"})
            .constructor<void(Expression*, bool)>()({"a_pExpression","a_bPhantom"})["false"]
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::DeleteExpression *() const, virtual_>("asDeleteExpression", &_::asDeleteExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::DeleteExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<Class*() const>("getClass", &_::getClass)
            .method<Expression*() const>("getExpression", &_::getExpression)
        
        .protected_()
            .field("m_pInitClass", &_::m_pInitClass)
            .field("m_pClass", &_::m_pClass)
            .field("m_pExpression", &_::m_pExpression)
            .field("m_bPhantom", &_::m_bPhantom)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("DeleteExpression")
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
