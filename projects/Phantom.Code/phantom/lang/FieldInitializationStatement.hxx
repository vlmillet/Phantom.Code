#pragma once

// haunt {

// clang-format off

#include "FieldInitializationStatement.h"

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
    PHANTOM_SOURCE("FieldInitializationStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(FieldInitializationStatement)
        {
            this_()
            .inherits<::phantom::lang::Statement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Field*, Expression*, size_t)>()({"a_pField","a_pValueExpression","a_uiArrayIndex"})["0"]
            .method<void()>("initialize", &_::initialize)
            .method<::phantom::lang::FieldInitializationStatement *() const, virtual_>("asFieldInitializationStatement", &_::asFieldInitializationStatement)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)({"a_Context"})
            .method<Field*() const>("getField", &_::getField)
            .method<Expression*() const>("getInitializationExpression", &_::getInitializationExpression)
            .method<size_t() const>("getArrayIndex", &_::getArrayIndex)
        
        .protected_()
            .field("m_pField", &_::m_pField)
            .field("m_pInitializationExpression", &_::m_pInitializationExpression)
            .field("m_uiArrayIndex", &_::m_uiArrayIndex)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FieldInitializationStatement")
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
