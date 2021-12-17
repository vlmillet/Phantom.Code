#pragma once

// haunt {

// clang-format off

#include "DebugInformation.h"

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
#include <phantom/friend>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("DebugInformation")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(DebugInformation)
        {
            this_()
        
        .public_()
            .constructor<void()>()
            .method<Module*() const>("getModule", &_::getModule)
            .method<BreakPoint*(const CodeLocation&)>("addBreakPoint", &_::addBreakPoint)({"a_Position"})
            .method<BreakPoint*(void*, const CodeLocation&)>("addBreakPoint", &_::addBreakPoint)({"a_pAddress","a_CandidatePosition"})["CodeLocation()"]
            .method<void(BreakPoint*)>("removeBreakPoint", &_::removeBreakPoint)({"a_pBreakPoint"})
            .method<::phantom::lang::CodeLocation(void*) const, virtual_>("getCodeLocationForAddress", &_::getCodeLocationForAddress)({"a_pAddress"})
            .method<void*(CodeLocation) const, virtual_>("getAddressForCodeLocation", &_::getAddressForCodeLocation)({"a_CodeLocation"})
            .method<bool(byte*, byte*, byte*&, byte*&) const, virtual_>("findStepOverInstructions", &_::findStepOverInstructions)({"a_pReturnAddress","a_pAddress","a_pNext","a_pUnconditionalBranchTarget"})
            .method<bool(byte*, byte*&, void*) const, virtual_>("findStepIntoInstruction", &_::findStepIntoInstruction)({"a_pAddress","a_pCallAddress","a_pGenericThreadContext"})
            .method<::phantom::lang::Subroutine *(byte*) const, virtual_>("getSubroutineAtAddress", &_::getSubroutineAtAddress)({"a_pAddress"})
            .method<byte*(LocalVariable*, size_t) const, virtual_>("getLocalVariableAddress", &_::getLocalVariableAddress)({"a_pLocalVariable","a_iDebuggerFrameIndex"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("DebugInformation")
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
