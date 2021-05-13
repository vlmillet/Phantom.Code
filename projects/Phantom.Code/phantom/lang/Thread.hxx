#pragma once

// haunt {

// clang-format off

#include "Thread.h"

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
#include <phantom/field>
#include <phantom/property>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace detail {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Thread")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ThreadGroup)
        {
            this_()
        
        .public_()
            .method<void(), pure_virtual>("lock", &_::lock)
            .method<void(), pure_virtual>("unlock", &_::unlock)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS_T((class), (t_Mutex), ThreadGroupT)
        {
            this_()
            .PHANTOM_T inherits<::phantom::detail::ThreadGroup>()
        .public_()
            .PHANTOM_T method<void(), virtual_>("lock", &_::lock)
            .PHANTOM_T method<void(), virtual_>("unlock", &_::unlock)
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Thread)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
        
        .protected_()
            .constructor<void()>()
            /// missing symbol(s) reflection (std::thread::id) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(StringView, ::std::thread::id)>()({"a_strName","id"})
        
        .public_()
            .constructor<void(const char*)>()({"a_strName"})
            .constructor<void(StringView)>()({"a_strName"})
            /// missing symbol(s) reflection (std::function) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(const ::std::function<bool ()> &)>()({"a_TickCallback"})
            .staticMethod<::phantom::detail::Thread *()>("GetCurrent", &_::GetCurrent)
            .staticMethod<::phantom::detail::Thread *()>("GetMain", &_::GetMain)
            /// missing symbol(s) reflection (std::thread::id) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<::phantom::detail::Thread *(::std::thread::id)>("Get", &_::Get)({"id"})
            /// missing symbol(s) reflection (std::thread::id) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<::std::thread::id()>("GetMainId", &_::GetMainId)
            /// missing symbol(s) reflection (std::thread::id) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<::std::thread::id()>("GetCurrentId", &_::GetCurrentId)
            /// missing symbol(s) reflection (std::thread::id) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::std::thread::id() const>("getId", &_::getId)
            /// missing symbol(s) reflection (std::function) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(const ::std::function<bool ()> &)>("setTickCallback", &_::setTickCallback)({"a_TickCallback"})
            .method<void()>("execute", &_::execute)
            .method<void()>("start", &_::start)
            .method<void()>("interrupt", &_::interrupt)
            .method<bool() const>("isLocked", &_::isLocked)
            .method<int()>("incrementLock", &_::incrementLock)
            .method<int()>("decrementLock", &_::decrementLock)
            .method<void()>("lock", &_::lock)
            .method<void()>("unlock", &_::unlock)
            .method<StringView() const>("getName", &_::getName)
            .method<void(StringView)>("setName", &_::setName)({"a_Name"})
            /// missing symbol(s) reflection (std::future) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(const ::std::future<void> &)>("pushCall", &_::pushCall)({"a_Future"})
            /// missing symbol(s) reflection (std::function) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(const ::std::function<void ()> &)>("pushCall", &_::pushCall)({"a_Call"})
            /// missing symbol(s) reflection (std::function) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(const ::std::function<void ()> &)>("pushCallAndWait", &_::pushCallAndWait)({"a_Call"})
            .method<void(ThreadGroup*)>("setGroup", &_::setGroup)({"a_pGroup"})
            .method<ThreadGroup*() const>("getGroup", &_::getGroup)
            .method<void(int)>("sleepMs", &_::sleepMs)({"ms"})
        
        .protected_()
            .property<StringView>("Name", &_::getName, &_::setName)
        
        .protected_()
            .field("m_strName", &_::m_strName)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Thread")
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
