#pragma once

// haunt {

// clang-format off

#include "Debugger.h"

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
#include <phantom/enum>
#include <phantom/function>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/Flags.hxx>
#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Debugger")

        #if PHANTOM_NOT_TEMPLATE
        /// missing symbol(s) reflection (_EXCEPTION_POINTERS) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Functions) { this_().function<long(_EXCEPTION_POINTERS*)>("VectoredHandlerBreakPoint", VectoredHandlerBreakPoint)({"ExceptionInfo"});}
        PHANTOM_CLASS(DebuggerMutex)
        {
            this_()
        .public_()
            .method<void(), pure_virtual>("lock", &_::lock)
            .method<void(), pure_virtual>("unlock", &_::unlock)
            ;
        }
        PHANTOM_CLASS(BreakPoint)
        {
            using EFlag = typedef_<_::EFlag>;
            using EState = typedef_<_::EState>;
            using Flags = typedef_<_::Flags>;
            this_()
        .public_()
            .enum_<EFlag>().values({
                {"e_Flag_User",_::e_Flag_User},
                {"e_Flag_StepOverInstruction",_::e_Flag_StepOverInstruction},
                {"e_Flag_StepOverLine",_::e_Flag_StepOverLine},
                {"e_Flag_StepInto",_::e_Flag_StepInto},
                {"e_Flag_StepOut",_::e_Flag_StepOut},
                {"e_Flag_Restorer",_::e_Flag_Restorer}})
            .end()
            .typedef_<Flags>("Flags")
            .enum_<EState>().values({
                {"e_State_Enabled",_::e_State_Enabled},
                {"e_State_Bypass",_::e_State_Bypass}})
            .end()
            .constructor<void(void *, const ::phantom::lang::CodeLocation &)>()({"a_pAddress","a_CodeLocation"})
            .method<phantom::byte *()>("nextAvailableBreakPointLocation", &_::nextAvailableBreakPointLocation)
            .method<bool(const ::phantom::lang::BreakPoint &) const>("operator<", &_::operator<)({"other"})
            .method<void(::phantom::lang::BreakPoint::EFlag)>("setFlag", &_::setFlag)({"f"})
            .method<void(::phantom::lang::BreakPoint::EFlag)>("unsetFlag", &_::unsetFlag)({"f"})
            .method<bool(::phantom::lang::BreakPoint::EFlag) const>("isFlagSet", &_::isFlagSet)({"f"})
            .method<phantom::lang::BreakPoint::Flags() const>("getFlags", &_::getFlags)
            .method<bool() const>("isEnabled", &_::isEnabled)
            .method<bool() const>("isByPassed", &_::isByPassed)
            .method<void(bool)>("setEnabled", &_::setEnabled)({"value"})
            .method<void(bool)>("setByPass", &_::setByPass)({"value"})
            .method<void(::phantom::lang::BreakPoint::Flags)>("setFlags", &_::setFlags)({"flags"})
            .method<phantom::byte *() const>("getMemoryAddress", &_::getMemoryAddress)
            .method<void(::phantom::byte *)>("setMemoryAddress", &_::setMemoryAddress)({"a_pAddress"})
            .method<const phantom::lang::CodeLocation &() const>("getCodeLocation", &_::getCodeLocation)
            .method<void(const ::phantom::lang::CodeLocation &)>("setCodeLocation", &_::setCodeLocation)({"a_Pos"})
            .method<bool(::phantom::byte *, ::phantom::byte *&, ::phantom::byte *&) const>("findStepOverLineInstructions", &_::findStepOverLineInstructions)({"a_pFrameBasePointer","a_pNext","a_pUnconditionalBranchTarget"})
            .method<bool(::phantom::byte *&, void *) const>("findStepIntoInstruction", &_::findStepIntoInstruction)({"a_pCallAddress","a_pGenericContext"})
            ;
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<BreakPoints>("BreakPoints"); }
        PHANTOM_CLASS(Debugger)
        {
            using DebugInformations = typedef_< phantom::lang::DebugInformations>;
            using FrameInfo = typedef_<_::FrameInfo>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
        .public_()
            .constructor<void()>()
            .method<bool()>("attach", &_::attach)
            .method<void()>("detach", &_::detach)
            .method<void()>("resume", &_::resume)
            /// missing symbol(s) reflection (std::function) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::function<void ()> &&, ::phantom::detail::Thread *)>("resumeAndExecute", &_::resumeAndExecute)({"a_ToExecute","a_pExecutionContextThread"})["nullptr"]
            .method<void()>("stepOverLine", &_::stepOverLine)
            .method<void()>("stepOverInstruction", &_::stepOverInstruction)
            .method<void()>("stepInto", &_::stepInto)
            .method<void()>("stepOut", &_::stepOut)
            .method<BreakPoint*(void*)>("addBreakPoint", &_::addBreakPoint)({"a_pAddress"})
            .method<BreakPoint*(void*, const CodeLocation&)>("addBreakPoint", &_::addBreakPoint)({"a_pAddress","a_CodeLocation"})
            .method<void(BreakPoint*)>("removeBreakPoint", &_::removeBreakPoint)({"a_pBreakPoint"})
            .method<BreakPoint*(void*) const>("getBreakPoint", &_::getBreakPoint)({"a_pAddress"})
            .method<::phantom::detail::Thread *() const>("getCurrentDebuggedThread", &_::getCurrentDebuggedThread)
            .method<void(::phantom::detail::Thread *)>("setCurrentDebuggedThread", &_::setCurrentDebuggedThread)({"a_pThread"})
            .method<void(DebuggerMutex*)>("setMutex", &_::setMutex)({"a_pMutex"})
            .method<bool() const>("isAttachable", &_::isAttachable)
            .method<bool() const>("isAttached", &_::isAttached)
            .method<bool() const>("isPaused", &_::isPaused)
            .method<CodeLocation(size_t) const>("getFrameCodeLocation", &_::getFrameCodeLocation)({"i"})["0"]
            .method<Subroutine*(size_t) const>("getFrameSubroutine", &_::getFrameSubroutine)({"i"})["0"]
            .method<SourceFile*(size_t) const>("getFrameSourceFile", &_::getFrameSourceFile)({"i"})["0"]
            .method<Source*(size_t) const>("getFrameSource", &_::getFrameSource)({"i"})["0"]
            .method<Block*(size_t) const>("getFrameBlock", &_::getFrameBlock)({"i"})["0"]
            .method<byte*(size_t) const>("getFrameBasePointer", &_::getFrameBasePointer)({"i"})["0"]
            .method<byte*(size_t) const>("getFrameReturnAddress", &_::getFrameReturnAddress)({"i"})["0"]
            .method<byte*(size_t) const>("getFrameStackPointer", &_::getFrameStackPointer)({"i"})["0"]
            .method<byte*(size_t) const>("getFrameInstructionPointer", &_::getFrameInstructionPointer)({"i"})["0"]
            .method<byte*(LocalVariable*, size_t) const>("getFrameLocalVariableAddress", &_::getFrameLocalVariableAddress)({"a_pLocalVariable","i"})["0"]
            .method<size_t() const>("getCallStackSize", &_::getCallStackSize)
            .method<void(SmallVector<CodeLocation>&) const>("getFrameCodeLocations", &_::getFrameCodeLocations)({"a_Frames"})
        
        .protected_()
            /// missing symbol(s) reflection (std::function) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::function<void ()> &&)>("_debug", &_::_debug)({"func"})
            .method<void(), virtual_>("clear", &_::clear)
            .method<void(), virtual_>("enableBreakPoints", &_::enableBreakPoints)
            .method<void(), virtual_>("disableBreakPoints", &_::disableBreakPoints)
            .method<bool(void*), virtual_>("handleSoftwareBreakPoint", &_::handleSoftwareBreakPoint)({"a_pGenericContext"})
            .method<bool(void*), virtual_>("handleHardwareBreakPoint", &_::handleHardwareBreakPoint)({"a_pGenericContext"})
            .method<BreakPoint*(void*)>("findBreakPointForAddress", &_::findBreakPointForAddress)({"start"})
            .method<LanguageElement*(void*) const>("findElementForAddress", &_::findElementForAddress)({"a_pAddress"})
            .method<CodeLocation(void*) const>("findCodeLocationForAddress", &_::findCodeLocationForAddress)({"a_pAddress"})
        
        .public_()
            .field("paused", &_::paused)
            .field("resumed", &_::resumed)
            .field("aboutToBeAttached", &_::aboutToBeAttached)
            .field("attached", &_::attached)
            .field("aboutToBeDetached", &_::aboutToBeDetached)
            .field("detached", &_::detached)
        
        .protected_()
            .method<void()>("updateBreakPoints", &_::updateBreakPoints)
            .struct_<FrameInfo>()
                .field("func", &_::FrameInfo::func)
                .field("bp", &_::FrameInfo::bp)
                .field("ip", &_::FrameInfo::ip)
                .field("sp", &_::FrameInfo::sp)
                .field("rp", &_::FrameInfo::rp)
            .end()
            .method<BreakPoint*(void*, ::phantom::lang::BreakPoint::Flags)>("addBreakPoint", &_::addBreakPoint)({"a_pAddress","flags"})
            .method<BreakPoint*(void*, ::phantom::lang::BreakPoint::Flags, const CodeLocation&)>("addBreakPoint", &_::addBreakPoint)({"a_pAddress","flags","a_CodeLocation"})
            .method<void(void*, ::phantom::lang::BreakPoint::EFlag)>("removeBreakPoint", &_::removeBreakPoint)({"a_pAddress","flag"})
            .method<void(BreakPoint*, ::phantom::lang::BreakPoint::EFlag)>("removeBreakPoint", &_::removeBreakPoint)({"a_pBreakPoint","flag"})
            .method<void(::phantom::lang::BreakPoint::EFlag)>("removeBreakPoints", &_::removeBreakPoints)({"flags"})
            .method<void(BreakPoint*)>("addRestorerBreakPoints", &_::addRestorerBreakPoints)({"restored_bp"})
            .method<void()>("removeRestorerBreakPoints", &_::removeRestorerBreakPoints)
            .method<bool(BreakPoint*) const>("doesBreakPointMustBeEnabled", &_::doesBreakPointMustBeEnabled)({"a_pBreakPoint"})
            .method<void()>("_resume", &_::_resume)
            .method<bool()>("applyStepOverLine", &_::applyStepOverLine)
            .method<bool()>("applyStepOverInstruction", &_::applyStepOverInstruction)
            .method<bool()>("applyStepOut", &_::applyStepOut)
            .method<bool()>("applyStepInto", &_::applyStepInto)
            .method<bool(size_t)>("removeHardwareBreakpoint", &_::removeHardwareBreakpoint)({"hBrk"})
            .method<void(DebugInformations&) const>("fetchDebugInformations", &_::fetchDebugInformations)({"handlers"})
            ;
        }
        PHANTOM_CLASS(DebuggerFrameContext)
        {
            this_()
            .inherits<::phantom::lang::ExecutionContext>()
        .public_()
            .constructor<void(size_t)>()({"a_Frame"})
            .method<bool(Subroutine*, void**, size_t), virtual_>("call", &_::call)({"a_pSubroutine","a_ppArgs","a_uiCount"})
            .method<void(Statement*), virtual_>("setNextStatement", &_::setNextStatement)({""})
            .method<byte*() const, virtual_>("getStackPointer", &_::getStackPointer)
            .method<byte*() const, virtual_>("getBasePointer", &_::getBasePointer)
            .method<void*() const, virtual_>("getReturnAddress", &_::getReturnAddress)
            .method<byte*() const, virtual_>("addressOfThis", &_::addressOfThis)
            .method<byte*(LocalVariable*) const, virtual_>("addressOf", &_::addressOf)({"a_pLocalVariable"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Debugger")
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
