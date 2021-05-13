#pragma once

// haunt {

// clang-format off

#include "CommaExpression.h"

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

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("CommaExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(CommaExpression)
        {
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .constructor<void(Expression*, Expression*)>()({"a_pLeftExpression","a_pRightExpression"})
            .method<void()>("initialize", &_::initialize)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)({"a_Context"})
            .method<::phantom::lang::CommaExpression *() const, virtual_>("asCommaExpression", &_::asCommaExpression)
            .method<::phantom::lang::CommaExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)({"a_pOwner"})
        
        .protected_()
            .field("m_pLeftExpression", &_::m_pLeftExpression)
            .field("m_pRightExpression", &_::m_pRightExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CommaExpression")
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
