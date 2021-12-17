#pragma once

// haunt {

// clang-format off

#include "Message.h"

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

#include <phantom/template-only-push>

#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Message")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Message)
        {
            using Messages = typedef_< phantom::lang::Messages>;
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            using messages_def = typedef_<_::messages_def>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::Object>()
        .public_()
            .struct_<messages_def>()
                .constructor<void(Message*)>()({"a_pMessage"})
            .end()
            .constructor<void(MessageType, StringView)>()({"a_eType","a_strText"})["MessageType::Undefined"]["\"\""]
            .method<Message*() const>("getRootMessage", &_::getRootMessage)
            .method<Message*() const>("getParent", &_::getParent)
            .method<Message*(size_t) const>("getChild", &_::getChild)({"a_uiIndex"})
            .method<size_t() const>("getChildCount", &_::getChildCount)
            .method<Messages const&() const>("getChildren", &_::getChildren)
            .method<const ::phantom::Variant &() const>("getData", &_::getData)
            .method<void(const Variant&)>("setData", &_::setData)({"a_Data"})
            .method<MessageType() const>("getType", &_::getType)
            .method<void(MessageType)>("setType", &_::setType)({"a_eType"})
            .method<MessageType() const>("getMostValuableMessageType", &_::getMostValuableMessageType)
            .method<StringView() const>("getText", &_::getText)
            .method<void(StringView)>("setText", &_::setText)({"a_strText"})
            .method<StringView() const>("getCategory", &_::getCategory)
            .method<void(StringView)>("setCategory", &_::setCategory)({"a_strCategory"})
            .method<void(Message*)>("addChild", &_::addChild)({"a_pChild"})
            .method<void(Message*)>("removeChild", &_::removeChild)({"a_pChild"})
            .method<Message*(bool) const>("clone", &_::clone)({"recursive"})["true"]
            /// missing symbol(s) reflection (va_list) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(const char*, va_list)>("format", &_::format)({"a_format","args"})
            .method<void()>("open", &_::open)
            /// missing symbol(s) reflection (va_list) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Message*(MessageType, const ::phantom::Variant &, const char*, va_list)>("messageD", &_::messageD)({"a_eType","a_Data","a_Format","args"})
            /// missing symbol(s) reflection (va_list) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Message*(MessageType, const char*, va_list)>("message", &_::message)({"a_eType","a_Format","args"})
            .method<messages_def()>("messages", &_::messages)
            .method<void(Message*) const>("copyChildrenTo", &_::copyChildrenTo)({"a_pMessage"})
            .method<void()>("clear", &_::clear)
            .method<Message*()>("removeLast", &_::removeLast)
        
        .protected_()
            .method<void(Message*)>("emitMessageOpenedCascade", &_::emitMessageOpenedCascade)({"a_pMessage"})
        
        .public_()
            .field("childAdded", &_::childAdded)
            .field("childRemoved", &_::childRemoved)
            .field("descendantAdded", &_::descendantAdded)
            .field("descendantRemoved", &_::descendantRemoved)
            .field("opened", &_::opened)
            .field("messageOpened", &_::messageOpened)
        
        .protected_()
            .field("m_pParent", &_::m_pParent)
            .field("m_Children", &_::m_Children)
            .field("m_eType", &_::m_eType)
            .field("m_strCategory", &_::m_strCategory)
            .field("m_strText", &_::m_strText)
            .field("m_Data", &_::m_Data)
            .field("m_Flags", &_::m_Flags)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Message")
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
