#pragma once

// haunt {

// clang-format off

#include "TemplateDependantTypeOfExpression.h"

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
    PHANTOM_SOURCE("TemplateDependantTypeOfExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateDependantTypeOfExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Type*, Type*)>()({"a_pType","a_pMetaTypePtrCast"})
            .method<::phantom::lang::TemplateDependantTypeOfExpression *() const, virtual_>("asTemplateDependantTypeOfExpression", &_::asTemplateDependantTypeOfExpression)
            .method<Type*() const>("getType", &_::getType)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)({"a_Context"})
            .method<bool() const, virtual_|override_>("isCompileTime", &_::isCompileTime)
        
        .protected_()
            .method<::phantom::lang::Expression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
        
        .protected_()
            .field("m_pType", &_::m_pType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateDependantTypeOfExpression")
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
