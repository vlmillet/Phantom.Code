#pragma once

// haunt {

// clang-format off

#include "conversions.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("conversions")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(ConversionVal)
        {
            this_()
            .constructor<void()>()
            .constructor<void(Type*)>()({"a_pType"})
            .constructor<void(Type*, int)>()({"a_pType","cat"})
            .field("type", &_::type)
            .field("category", &_::category)
            .method<Type*() const>("operator->", &_::operator->)
            .method<::phantom::lang::Type *() const>("operator ::phantom::lang::Type *", &_::operator notypedef<::phantom::lang::Type *>)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT_T((class), (t_Type), ConversionValT)
        {
            this_()
            .PHANTOM_T inherits<::phantom::lang::ConversionVal>()
            .PHANTOM_T method<t_Type*() const>("operator->", &_::operator->)
            .PHANTOM_T method<t_Type*()>("operator t_Type*", &_::operator notypedef<t_Type*>)
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(Conversion)
        {
            this_()
            .constructor<void(LanguageElement*, ConversionVal, ConversionVal)>()({"a_pOwner","i","o"})
            .method<void(const void*, void*) const, pure_virtual>("apply", &_::apply)({"a_pInput","a_pOutput"})
            .method<::phantom::lang::Conversion *(LanguageElement*) const, pure_virtual>("clone", &_::clone)({"a_pOwner"})
            .method<int(const Conversion&) const, virtual_>("compare", &_::compare)({"a_Other"})
            .method<::phantom::lang::Expression *(Semantic*, Expression*) const, pure_virtual>("convert", &_::convert)({"a_pSemantic","a_pExpression"})
            .method<bool() const>("isIdentity", &_::isIdentity)
            .method<bool() const, virtual_>("isCanonical", &_::isCanonical)
            .method<void()>("delete_", &_::delete_)
            .field("input", &_::input)
            .field("output", &_::output)
            .field("semantic", &_::semantic)
            .field("kind", &_::kind)
            .field("m_pOwner", &_::m_pOwner)
            ;
        }
        PHANTOM_STRUCT(TemplateDependantConversion)
        {
            this_()
            .inherits<::phantom::lang::Conversion>()
            .constructor<void(LanguageElement*, ConversionVal, ConversionVal)>()({"a_pOwner","i","o"})
            .method<void(const void*, void*) const, virtual_>("apply", &_::apply)({"",""})
            .method<::phantom::lang::Conversion *(LanguageElement*) const, virtual_|override_>("clone", &_::clone)({"a_pOwner"})
            .method<int(const Conversion&) const, virtual_>("compare", &_::compare)({""})
            .method<::phantom::lang::Expression *(Semantic*, Expression*) const, virtual_>("convert", &_::convert)({"",""})
            .method<bool() const, virtual_>("isCanonical", &_::isCanonical)
            ;
        }
        PHANTOM_CLASS(ConversionResults)
        {
            using ConversionList = typedef_<_::ConversionList>;
            this_()
        .public_()
            .typedef_<ConversionList>("ConversionList")
            .constructor<void()>()
            .constructor<void(ConversionResults&&)>()({"rvalue"})
            .constructor<void(const ConversionResults&)>()({"rvalue"})
            .method<ConversionResults&(ConversionResults&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"rvalue"})
            .method<ConversionResults&(const ConversionResults&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"rvalue"})
            .method<int(const ConversionResults&) const>("compare", &_::compare)({"other"})
            .method<void(Conversion*)>("push_back", &_::push_back)({"conv"})
            .method<bool() const>("hasNull", &_::hasNull)
            .method<Conversion*(size_t*)>("takeBest", &_::takeBest)({"a_pIndex"})
            .method<Conversion* const&(size_t) const>("operator[]", &_::operator[])({"index"})
            .method<size_t() const>("size", &_::size)
            .method<::phantom::SmallVector<Conversion *, 4>::const_iterator() const>("begin", &_::begin)
            .method<::phantom::SmallVector<Conversion *, 4>::const_iterator() const>("end", &_::end)
            .method<void(Source*)>("destroy", &_::destroy)({"a_pSource"})
        
        .protected_()
            .field("content", &_::content)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("conversions")
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
