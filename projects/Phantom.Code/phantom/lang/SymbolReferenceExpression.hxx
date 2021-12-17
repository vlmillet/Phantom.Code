#pragma once

// haunt {

// clang-format off

#include "SymbolReferenceExpression.h"

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
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/friend>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("SymbolReferenceExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(SymbolReferenceExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Expression *(Semantic*, LanguageElement*, Symbol*, Class*)>("Create", &_::Create)({"a_pSemantic","a_pOwner","a_pSymbol","a_pCastSymbolClass"})
        
        .public_()
            .constructor<void(Symbol*)>()({"a_pSymbol"})
            .method<void()>("initialize", &_::initialize)
            .method<Symbol*() const>("getSymbol", &_::getSymbol)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
        
        .protected_()
            .method<::phantom::lang::SymbolReferenceExpression *(LanguageElement*) const, virtual_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SymbolReferenceExpression")
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
