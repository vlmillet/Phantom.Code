#pragma once

// haunt {

// clang-format off

#include "VirtualMethodTableSetupStatement.h"

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
    PHANTOM_SOURCE("VirtualMethodTableSetupStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(VirtualMethodTableSetupStatement)
        {
            this_()
            .inherits<::phantom::lang::Statement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(VirtualMethodTable*)>()({"a_pTable"})
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::VirtualMethodTableSetupStatement *() const, virtual_>("asVirtualMethodTableSetupStatement", &_::asVirtualMethodTableSetupStatement)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)({""})
            .method<VirtualMethodTable*() const>("getTable", &_::getTable)
        
        .protected_()
            .field("m_pTable", &_::m_pTable)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("VirtualMethodTableSetupStatement")
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
