#pragma once

// haunt {

// clang-format off

#include "TypeTraitExpression.h"

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
#include <phantom/enum>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/friend>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("TypeTraitExpression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TypeTraitExpression)
        {
            using ETypeTrait = typedef_<_::ETypeTrait>;
            using Types = typedef_< phantom::lang::Types>;
            this_()
            .inherits<::phantom::lang::Expression>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .enum_<ETypeTrait>().values({
                {"e_TypeTait_Undefined",_::e_TypeTait_Undefined},
                {"e_TypeTait_IsSame",_::e_TypeTait_IsSame}})
            .end()
        
        .public_()
            .constructor<void(ETypeTrait, Types const&)>()
            .method<::phantom::lang::TypeTraitExpression *() const, virtual_>("asTypeTraitExpression", &_::asTypeTraitExpression)
            .method<ETypeTrait() const>("getTypeTrait", &_::getTypeTrait)
            .staticMethod<bool(ETypeTrait, Types const&)>("Resolve", &_::Resolve)
            .method<Types const&() const>("getTypes", &_::getTypes)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)
            .method<bool() const, virtual_|override_>("isCompileTime", &_::isCompileTime)
        
        .protected_()
            .method<::phantom::lang::Expression *(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
        
        .protected_()
            .field("m_eTypeTrait", &_::m_eTypeTrait)
            .field("m_Types", &_::m_Types)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TypeTraitExpression")
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
