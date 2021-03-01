// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/lang.h>
#include <phantom/lang/CodeLocation.h>
#include <phantom/lang/ExecutionContext.h>
#include <phantom/utils/Flags.h>
#include <phantom/utils/Signal.h>
#include <phantom/utils/SmallMap.h>
#include <phantom/utils/SmallVector.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace detail
{
class Thread;
}
namespace lang
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS

long __stdcall VectoredHandlerBreakPoint(struct _EXCEPTION_POINTERS* ExceptionInfo);

#endif

#define BREAK_INSTRUCTION_SIZE 1
#define CALL_INSTRUCTION_SIZE 5

class Instruction;

class PHANTOM_EXPORT_PHANTOM_CODE DebuggerMutex
{
public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
};

struct CodeLocation;

class PHANTOM_EXPORT_PHANTOM_CODE BreakPoint final
{
public:
    enum EFlag
    {
        e_Flag_User = 0x1,
        e_Flag_StepOverInstruction = 0x2,
        e_Flag_StepOverLine = 0x4,
        e_Flag_StepInto = 0x8,
        e_Flag_StepOut = 0x10,
        e_Flag_Restorer = 0x20
    };

    PHANTOM_DECLARE_FLAGS(Flags, EFlag);

    enum EState
    {
        e_State_Enabled = 0x1,
        e_State_Bypass = 0x2,
    };

    BreakPoint(void* a_pAddress, const CodeLocation& a_CodeLocation)
        : m_pRestoredBreakPoint(0),
          m_Flags(0),
          m_iStates(0),
          m_pMemoryAddress((byte*)a_pAddress),
          m_CodeLocation(a_CodeLocation)
    {
    }

    byte* nextAvailableBreakPointLocation();

    bool operator<(const BreakPoint& other) const { return m_pMemoryAddress < other.m_pMemoryAddress; }

    void setFlag(EFlag f) { m_Flags |= f; }

    void unsetFlag(EFlag f) { m_Flags &= ~f; }

    bool isFlagSet(EFlag f) const { return (m_Flags & f) == f; }

    Flags getFlags() const { return m_Flags; }

    bool isEnabled() const { return (m_iStates & e_State_Enabled) != 0; }
    bool isByPassed() const { return (m_iStates & e_State_Bypass) != 0; }

    void setEnabled(bool value);

    void setByPass(bool value)
    {
        if (value)
            m_iStates |= e_State_Bypass;
        else
            m_iStates &= ~e_State_Bypass;
    }

    void setFlags(Flags flags) { m_Flags |= flags; }

    byte* getMemoryAddress() const { return m_pMemoryAddress; }

    void setMemoryAddress(byte* a_pAddress) { m_pMemoryAddress = a_pAddress; }

    const CodeLocation& getCodeLocation() const { return m_CodeLocation; }
    void                setCodeLocation(const CodeLocation& a_Pos) { m_CodeLocation = a_Pos; }

    bool findStepOverLineInstructions(byte* a_pFrameBasePointer, byte*& a_pNext,
                                      byte*& a_pUnconditionalBranchTarget) const;

    bool findStepIntoInstruction(byte*& a_pCallAddress, void* a_pGenericContext) const;

protected:
    Source*      m_pSource;
    CodeLocation m_CodeLocation;
    byte*        m_pMemoryAddress;
    BreakPoint*  m_pRestoredBreakPoint;
    Flags        m_Flags;
    int          m_iStates;
    byte         m_OriginalOpCode;
};

PHANTOM_DEF_LIST(BreakPoint);

class PHANTOM_EXPORT_PHANTOM_CODE Debugger
{
    enum ERequest
    {
        e_Request_None,
        e_Request_StepOverLine,
        e_Request_StepOverInstruction,
        e_Request_StepOut,
        e_Request_StepInto,
        e_Request_Resume,
    };

public:
    Debugger(void);
    ~Debugger(void);

    bool attach();
    void detach();
    template<class T>
    void debug(T&& functor)
    {
        _debug(std::bind(functor));
    }

    HAUNT_SLOT void resume();
    void            resumeAndExecute(std::function<void()>&&  a_ToExecute,
                                     phantom::detail::Thread* a_pExecutionContextThread = nullptr);
    HAUNT_SLOT void stepOverLine();
    HAUNT_SLOT void stepOverInstruction();
    HAUNT_SLOT void stepInto();
    HAUNT_SLOT void stepOut();

    BreakPoint* addBreakPoint(void* a_pAddress);
    BreakPoint* addBreakPoint(void* a_pAddress, const CodeLocation& a_CodeLocation);
    void        removeBreakPoint(BreakPoint* a_pBreakPoint);

    BreakPoint* getBreakPoint(void* a_pAddress) const
    {
        auto found = m_BreakPointsForAddress.find(a_pAddress);
        return found == m_BreakPointsForAddress.end() ? nullptr : found->second;
    }

    phantom::detail::Thread* getCurrentDebuggedThread() const { return m_pThread; }

    void setCurrentDebuggedThread(phantom::detail::Thread* a_pThread) { m_pThread = a_pThread; }

    void setMutex(DebuggerMutex* a_pMutex) { m_pMutex = a_pMutex; }

    bool isAttachable() const;
    bool isAttached() const { return m_bAttached; }
    bool isPaused() const { return m_bPaused; }

    CodeLocation getFrameCodeLocation(size_t i = 0) const;
    Subroutine*  getFrameSubroutine(size_t i = 0) const;
    SourceFile*  getFrameSourceFile(size_t i = 0) const;
    Source*      getFrameSource(size_t i = 0) const;
    Block*       getFrameBlock(size_t i = 0) const;
    byte*        getFrameBasePointer(size_t i = 0) const;
    byte*        getFrameReturnAddress(size_t i = 0) const;
    byte*        getFrameStackPointer(size_t i = 0) const;
    byte*        getFrameInstructionPointer(size_t i = 0) const;
    byte*        getFrameLocalVariableAddress(LocalVariable* a_pLocalVariable, size_t i = 0) const;
    size_t       getCallStackSize() const { return m_CallStackFrames.size(); }
    void         getFrameCodeLocations(SmallVector<CodeLocation>& a_Frames) const;

protected:
    void         _debug(std::function<void()>&& func);
    virtual void clear();

    virtual void enableBreakPoints();
    virtual void disableBreakPoints();

    virtual bool handleSoftwareBreakPoint(void* a_pGenericContext);
    virtual bool handleHardwareBreakPoint(void* a_pGenericContext);

    BreakPoint*      findBreakPointForAddress(void* start);
    LanguageElement* findElementForAddress(void* a_pAddress) const;
    CodeLocation     findCodeLocationForAddress(void* a_pAddress) const;

public:
    phantom::Signal<void()> paused;
    phantom::Signal<void()> resumed;
    phantom::Signal<void()> aboutToBeAttached;
    phantom::Signal<void()> attached;
    phantom::Signal<void()> aboutToBeDetached;
    phantom::Signal<void()> detached;

protected:
    void updateBreakPoints();

    struct FrameInfo
    {
        Subroutine* func;
        byte*       bp;
        byte*       ip;
        byte*       sp;
        byte*       rp;
    };

    BreakPoint* addBreakPoint(void* a_pAddress, BreakPoint::Flags flags);
    BreakPoint* addBreakPoint(void* a_pAddress, BreakPoint::Flags flags, const CodeLocation& a_CodeLocation);
    void        removeBreakPoint(void* a_pAddress, BreakPoint::EFlag flag);
    void        removeBreakPoint(BreakPoint* a_pBreakPoint, BreakPoint::EFlag flag);
    void        removeBreakPoints(BreakPoint::EFlag flags);
    void        addRestorerBreakPoints(BreakPoint* restored_bp);
    void        removeRestorerBreakPoints();
    bool        doesBreakPointMustBeEnabled(BreakPoint* a_pBreakPoint) const;

    void _resume();
    bool applyStepOverLine();
    bool applyStepOverInstruction();
    bool applyStepOut();
    bool applyStepInto();
    bool removeHardwareBreakpoint(size_t hBrk);
    void fetchDebugInformations(DebugInformations& handlers) const;

private:
    Subroutine* _findSubroutineAtAddress(DebugInformations& handlers, byte* ip);
    void        _StackTrace64();
    void        captureCallStack(DebugInformations& handlers);
    void        _captureCallStack(DebugInformations& handlers, byte* eip, byte* ebp, byte* esp);
    void        clearCurrentBreakpoint();
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    friend long __stdcall VectoredHandlerBreakPoint(struct _EXCEPTION_POINTERS* ExceptionInfo);
#endif
    long _handleBreakException(struct _EXCEPTION_POINTERS* _pointers, unsigned long _code);

private:
    CodeGenerator*               m_pCodeGenerator;
    SmallMap<void*, BreakPoint*> m_BreakPointsForAddress;
    ERequest                     m_Request;
    DebuggerMutex*               m_pMutex;
    BreakPoints                  m_RememberedBreakPoints;
    phantom::detail::Thread*     m_pThread = nullptr;

    // Step by step
    size_t      m_StepByStepBreakPointHandle;
    Subroutine* m_StepByStepSubroutine;
    byte*       m_StepByStepEBP;
    byte*       m_StepByStepEIP;
    byte*       m_pSoftwareBreakAddress;
    byte*       m_pBreakAddress;
    int         m_StepByStepLine;

    BreakPoint*                 m_pCurrentBreakPoint;
    Instruction*                m_pCurrentHardwareBreakpointInstruction;
    size_t                      m_DebuggedThread;
    SmallVector<FrameInfo, 100> m_CallStackFrames;
    int                         m_iEnablingCounter;
    byte*                       m_BasePointer;
    byte*                       m_RootBasePointer;
    void*                       m_pGenericContext;
    bool                        m_bAttached;
    bool                        m_bPaused;
    bool                        m_bStepIntoEnabled;
};

class PHANTOM_EXPORT_PHANTOM_CODE DebuggerFrameContext : public ExecutionContext
{
public:
    DebuggerFrameContext(size_t a_Frame) : m_Frame(a_Frame) {}
    // must be reimplemented
    virtual bool  call(Subroutine* a_pSubroutine, void** a_ppArgs, size_t a_uiCount);
    virtual void  setNextStatement(Statement* ) { PHANTOM_UNREACHABLE(); }
    virtual byte* getStackPointer() const;
    virtual byte* getBasePointer() const;
    virtual void* getReturnAddress() const;
    virtual byte* addressOfThis() const;
    virtual byte* addressOf(LocalVariable* a_pLocalVariable) const;

private:
    size_t m_Frame = 0;
};

} // namespace lang
} // namespace phantom
