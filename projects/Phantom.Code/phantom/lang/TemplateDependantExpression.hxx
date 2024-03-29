#pragma once

// haunt {

// clang-format off

#include "TemplateDependantExpression.h"

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
    PHANTOM_SOURCE("TemplateDependantExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateDependantExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(TemplateDependantElement*)>()({"a_pElement"})
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({""})
            .method<::phantom::lang::TemplateDependantExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            .method<TemplateDependantElement*() const>("getTemplateDependantElement", &_::getTemplateDependantElement)
        
        .protected_()
            .field("m_pElement", &_::m_pElement)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateDependantExpression")
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
