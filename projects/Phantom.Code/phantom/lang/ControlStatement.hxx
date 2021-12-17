#pragma once

// haunt {

// clang-format off

#include "ControlStatement.h"

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
#include <phantom/field>
#include <phantom/friend>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("ControlStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ControlStatement)
        {
            using Expressions = typedef_< phantom::lang::Expressions>;
            this_()
            .inherits<::phantom::lang::Statement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .method<Expressions const&() const>("getRAIIDestructions", &_::getRAIIDestructions)
            .method<::phantom::lang::Statement *() const, pure_virtual>("getTargetStatement", &_::getTargetStatement)
        
        .protected_()
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)({"a_Context"})
            .method<void()>("finish", &_::finish)
            .method<void()>("stripRAIIDestructions", &_::stripRAIIDestructions)
        
        .protected_()
            .field("m_RAIIDestructions", &_::m_RAIIDestructions)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ControlStatement")
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
