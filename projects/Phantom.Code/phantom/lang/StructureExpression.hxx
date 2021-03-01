#pragma once

// haunt {

// clang-format off

#include "StructureExpression.h"

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
    PHANTOM_SOURCE("StructureExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(StructureExpression)
        {
            using Expressions = typedef_< phantom::lang::Expressions>;
            using ExpressionsView = typedef_< phantom::lang::ExpressionsView>;
            using Fields = typedef_< phantom::lang::Fields>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void(Structure*, const Fields&, ExpressionsView)>()
            .method<::phantom::lang::StructureExpression *() const, virtual_>("asStructureExpression", &_::asStructureExpression)
            .method<void(ExecutionContext&) const, virtual_>("eval", &_::eval)
            .method<::phantom::lang::StructureExpression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
            .method<const Fields&() const>("getFields", &_::getFields)
            .method<ExpressionsView() const>("getValues", &_::getValues)
        
        .protected_()
            .field("m_Fields", &_::m_Fields)
            .field("m_Values", &_::m_Values)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StructureExpression")
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
