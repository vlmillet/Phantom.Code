#pragma once

// haunt {

// clang-format off

#include "TemplateParameterPackExpansion.h"

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
    PHANTOM_SOURCE("TemplateParameterPackExpansion")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateParameterPackExpansion)
        {
            this_()
            .inherits<::phantom::lang::LanguageElement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(LanguageElement*, Placeholder*)>()({"a_pExpandedElement","a_pPackArgument"})
            .method<LanguageElement*() const>("getExpandedElement", &_::getExpandedElement)
            .method<Placeholder*() const>("getPackArgument", &_::getPackArgument)
        
        .protected_()
            .field("m_pExpandedElement", &_::m_pExpandedElement)
            .field("m_pPackArgument", &_::m_pPackArgument)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateParameterPackExpansion")
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
