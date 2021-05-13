#pragma once

// haunt {

// clang-format off

#include "MemSetStatement.h"

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
    PHANTOM_SOURCE("MemSetStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(MemSetStatement)
        {
            this_()
            .inherits<::phantom::lang::Statement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Expression*, byte, size_t)>()({"a_pAddressExpression","a_uValue","a_uiSize"})
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::MemSetStatement *() const, virtual_>("asMemSetStatement", &_::asMemSetStatement)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)({"a_Context"})
            .method<Expression*() const>("getAddressExpression", &_::getAddressExpression)
            .method<size_t() const>("getSize", &_::getSize)
            .method<byte() const>("getValue", &_::getValue)
        
        .protected_()
            .field("m_pAddr", &_::m_pAddr)
            .field("m_uiSize", &_::m_uiSize)
            .field("m_uValue", &_::m_uValue)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MemSetStatement")
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
