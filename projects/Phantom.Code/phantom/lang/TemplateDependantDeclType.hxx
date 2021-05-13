#pragma once

// haunt {

// clang-format off

#include "TemplateDependantDeclType.h"

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
    PHANTOM_SOURCE("TemplateDependantDeclType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateDependantDeclType)
        {
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Type>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Expression*, Type*)>()({"m_pExpression","a_pAutoType"})["nullptr"]
            .method<bool(Type*) const, virtual_>("isA", &_::isA)({""})
            .method<bool() const, virtual_>("isCopyable", &_::isCopyable)
            .method<Type*() const>("getAutoType", &_::getAutoType)
            .method<Expression*() const>("getExpression", &_::getExpression)
        
        .protected_()
            .field("m_pExpression", &_::m_pExpression)
            .field("m_pAutoType", &_::m_pAutoType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateDependantDeclType")
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
