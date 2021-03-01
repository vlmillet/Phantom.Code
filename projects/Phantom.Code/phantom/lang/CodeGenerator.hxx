#pragma once

// haunt {

// clang-format off

#include "CodeGenerator.h"

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
#include <phantom/enum_class>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("CodeGenerator")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(CodeGenerator)
        {
            using Options = typedef_< phantom::lang::Options>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::Object>()
        
        .public_()
            .enum_<Error>().values({
                {"OK",_::Error::OK},
                {"UnresolvedSymbol",_::Error::UnresolvedSymbol},
                {"MemoryNotReady",_::Error::MemoryNotReady},
                {"OutOfMemory",_::Error::OutOfMemory}})
            .end()
            .constructor<void(int)>()
            .method<const Options&() const>("getOptions", &_::getOptions)
            .method<void(const Options&)>("setOptions", &_::setOptions)
            .method<void(StringView, StringView)>("setOption", &_::setOption)
            .method<uint() const>("getPassCount", &_::getPassCount)
            .method<StringView(StringView) const>("getOption", &_::getOption)
            .method<bool(StringView, StringView&) const>("getOption", &_::getOption)
            .method<Module*() const>("getModule", &_::getModule)
            .method<void()>("outdate", &_::outdate)
            .method<bool() const>("isOutdated", &_::isOutdated)
            .method<bool(Method*, void*, void**, void*) const>("invoke", &_::invoke)
            .method<bool(Subroutine*, void**, void*) const>("call", &_::call)
            .method<::phantom::lang::ExecutionContext *() const, pure_virtual>("createExecutionContext", &_::createExecutionContext)
            .method<void(ExecutionContext*) const, pure_virtual>("destroyExecutionContext", &_::destroyExecutionContext)
        
        .protected_()
            .method<::phantom::lang::DebugInformation *(), virtual_>("createDebugInformation", &_::createDebugInformation)
            .method<void(DebugInformation*), virtual_>("destroyDebugInformation", &_::destroyDebugInformation)
        
        .protected_()
            .method<void(), virtual_>("begin", &_::begin)
            .method<void(), virtual_>("abort", &_::abort)
            .method<void(), virtual_>("end", &_::end)
            .method<::phantom::lang::CodeGenerator::Error(uint), pure_virtual>("compile", &_::compile)
            .method<void(), virtual_>("onOutdated", &_::onOutdated)
        
        .protected_()
            .field("m_pModule", &_::m_pModule)
            .field("m_Options", &_::m_Options)
            .field("m_uiPassCount", &_::m_uiPassCount)
            .field("m_bOutdated", &_::m_bOutdated)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CodeGenerator")
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
