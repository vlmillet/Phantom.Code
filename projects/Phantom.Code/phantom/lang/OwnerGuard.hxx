#pragma once

// haunt {

// clang-format off

#include "OwnerGuard.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("OwnerGuard")

        PHANTOM_STRUCT_T((class), (T), OwnerGuard)
        {
            this_()
            .PHANTOM_T constructor<void(T*)>()
            .PHANTOM_T method<T*()>("take", &_::take)
            .PHANTOM_T method<T*&()>("element", &_::element)
            .PHANTOM_T method<T*()>("operator->", &_::operator->)
            .PHANTOM_T field("m_pElement", &_::m_pElement)
            .PHANTOM_T method<T*() const>("operator T*", &_::operator notypedef<T*>)
            .PHANTOM_T method<OwnerGuard<T>&(T*)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            ;
        }
        PHANTOM_STRUCT_T((class), (Ts), OwnersGuard)
        {
            this_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void()>()
            )
            .PHANTOM_T constructor<void(const Ts&)>()
            .PHANTOM_T method<void()>("cancel", &_::cancel)
            .PHANTOM_T method<void(const typename Ts::value_type &)>("push_back", &_::push_back)
            .PHANTOM_T method<size_t() const>("size", &_::size)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            .PHANTOM_T method<typename Ts::const_iterator() const>("begin", &_::begin)
            .PHANTOM_T method<typename Ts::const_iterator() const>("end", &_::end)
            .PHANTOM_T method<Ts&()>("elements", &_::elements)
            .PHANTOM_T method<Ts&()>("operator*", &_::operator*)
            .PHANTOM_T method<Ts*()>("operator->", &_::operator->)
            .PHANTOM_T field("m_Elements", &_::m_Elements)
            .PHANTOM_T method<typename Ts::ArrayViewType() const>("operator typename Ts::ArrayViewType", &_::operator notypedef<typename Ts::ArrayViewType>)
            .PHANTOM_T method<const Ts&() const>("operator const Ts&", &_::operator notypedef<const Ts&>)
            .PHANTOM_T method<Ts&()>("operator Ts&", &_::operator notypedef<Ts&>)
            ;
        }
    PHANTOM_END("OwnerGuard")
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
