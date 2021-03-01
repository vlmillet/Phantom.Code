#pragma once

// haunt {

// clang-format off

#include "Label.h"

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
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Label")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Label)
        {
            using ETag = typedef_<_::ETag>;
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Statement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .enum_<ETag>().values({
                {"e_Tag_None",_::e_Tag_None},
                {"e_Tag_Else",_::e_Tag_Else},
                {"e_Tag_Endif",_::e_Tag_Endif},
                {"e_Tag_Break",_::e_Tag_Break},
                {"e_Tag_Continue",_::e_Tag_Continue},
                {"e_Tag_Case",_::e_Tag_Case},
                {"e_Tag_Default",_::e_Tag_Default}})
            .end()
            .constructor<void(int)>()["e_Tag_None"]
            .constructor<void(StringView, int)>()["e_Tag_None"]
            .method<Label*() const, virtual_|override_>("asLabel", &_::asLabel)
            .method<void(ExecutionContext&) const, virtual_|override_>("eval", &_::eval)
            .method<void(ExecutionContext&) const, virtual_|override_>("flush", &_::flush)
            .method<StringView() const>("getLabelName", &_::getLabelName)
            .method<void(StringView)>("setLabelName", &_::setLabelName)
            .method<int() const>("getTag", &_::getTag)
            .method<void(int)>("setTag", &_::setTag)
            .method<size_t() const>("getIndex", &_::getIndex)
        
        .protected_()
            .method<void(Block*), virtual_|override_>("onAttachedToBlock", &_::onAttachedToBlock)
            .method<void(BranchStatement*)>("_registerBranchStatement", &_::_registerBranchStatement)
        
        .protected_()
            .field("m_LabelName", &_::m_LabelName)
            .field("m_uiIndex", &_::m_uiIndex)
            .field("m_BranchStatements", &_::m_BranchStatements)
            .field("m_iTag", &_::m_iTag)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Label")
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
