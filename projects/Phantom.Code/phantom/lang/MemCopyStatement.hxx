#pragma once

// haunt {

// clang-format off

#include "MemCopyStatement.h"

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
    PHANTOM_SOURCE("MemCopyStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(MemCopyStatement)
        {
            this_()
            .inherits<::phantom::lang::Statement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Expression*, Expression*, size_t)>()
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::MemCopyStatement *() const, virtual_>("asMemCopyStatement", &_::asMemCopyStatement)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<void(ExecutionContext&) const, virtual_>("flush", &_::flush)
            .method<Expression*() const>("getDestinationAddressExpression", &_::getDestinationAddressExpression)
            .method<Expression*() const>("getSourceAddressExpression", &_::getSourceAddressExpression)
            .method<size_t() const>("getSize", &_::getSize)
        
        .protected_()
            .field("m_pDst", &_::m_pDst)
            .field("m_pSrc", &_::m_pSrc)
            .field("m_uiSize", &_::m_uiSize)
            .field("m_uValue", &_::m_uValue)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MemCopyStatement")
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
