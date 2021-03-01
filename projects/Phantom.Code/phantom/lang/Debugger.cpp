// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "Debugger.h"

#include "CodeGenerator.h"
#include "DebugInformation.h"
#include "phantom/lang/Method.h"

#include <phantom/lang/Application.h>
#include <phantom/lang/Module.h>
#include <phantom/lang/Source.h>
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    include "windows.h"

#    include <dbghelp.h>
#elif PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_LINUX && PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86
#    include <asm/cachectl.h>
#endif
#include "Block.h"
#include "Compiler.h"
#include "Thread.h"

#include <phantom/lang/LocalVariable.h>
#include <phantom/lang/SourceFile.h>
/* *********************************************** */
static const int MAX_STEP_INSTRUCTION = 20;

enum EHardwareBreakpointType
{
    e_HardwareBreakpointType_Code,
    e_HardwareBreakpointType_ReadWrite,
    e_HardwareBreakpointType_Write,
};

enum EHardwareBreakpointSize
{
    e_HardwareBreakpointSize_1,
    e_HardwareBreakpointSize_2,
    e_HardwareBreakpointSize_4,
    e_HardwareBreakpointSize_8,
};

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS

//
// __declspec(naked) void Debugger_builtInBreak()
// {
//     // Astuce to go back exactly where we came from instead of the classic return address
//     __asm
//     {
//         PUSH        EBP; Save ebp
//         MOV         EBP, ESP; Set stack frame pointer
//             SUB         ESP, 32; Allocate space for locals
//             PUSH EAX;
//         PUSH EBX;
//         PUSH ECX;
//         PUSH EDX;
//     }
//     DWORD rebp;
//     DWORD eip;
//     rebp = 0;
//     __asm
//     {
//         MOV rebp, EBP
//     }
//     eip = *((DWORD*)((unsigned char*)rebp + 4)) - BREAK_INSTRUCTION_SIZE;
//     phantom::moon::script::DebugDataBase::Instance()->handleSoftwareBreakPoint(0);
//     *((DWORD*)((unsigned char*)rebp + 4)) = eip;
//
//     __asm
//     {
//         POP EDX;
//         POP ECX;
//         POP EBX;
//         POP EAX;
//         MOV         ESP, EBP; Restore stack pointer
//             POP         EBP; Restore ebp
//             RET; Return from function
//     }
// }

HANDLE SetHardwareBreakpoint(HANDLE hThread, EHardwareBreakpointType Type, EHardwareBreakpointSize Size, void* s);

class HWBRK
{
public:
    void*                   a;
    HANDLE                  hT;
    EHardwareBreakpointType Type;
    EHardwareBreakpointSize Size;
    HANDLE                  hEv;
    int                     iReg;
    int                     Opr;
    bool                    SUCC;

    HWBRK()
    {
        Opr = 0;
        a = 0;
        hT = 0;
        hEv = 0;
        iReg = 0;
        SUCC = false;
    }
};

void SetBits(DWORD_PTR& dw, int lowBit, int bits, int newValue)
{
    DWORD_PTR mask = (1 << bits) - 1;
    dw = (dw & ~(mask << lowBit)) | (newValue << lowBit);
}

static DWORD WINAPI th(LPVOID lpParameter)
{
    HWBRK* h = (HWBRK*)lpParameter;
    int    j = 0;
    int    y = 0;

    j = SuspendThread(h->hT);
    y = GetLastError();

    CONTEXT ct = {0};
    ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    j = GetThreadContext(h->hT, &ct);
    y = GetLastError();

    int FlagBit = 0;

#    if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X64 || PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86

    bool Dr0Busy = false;
    bool Dr1Busy = false;
    bool Dr2Busy = false;
    bool Dr3Busy = false;
    if (ct.Dr7 & 1)
        Dr0Busy = true;
    if (ct.Dr7 & 4)
        Dr1Busy = true;
    if (ct.Dr7 & 16)
        Dr2Busy = true;
    if (ct.Dr7 & 64)
        Dr3Busy = true;

    if (h->Opr == 1)
    {
        // Remove
        if (h->iReg == 0)
        {
            FlagBit = 0;
            ct.Dr0 = 0;
            Dr0Busy = false;
        }
        if (h->iReg == 1)
        {
            FlagBit = 2;
            ct.Dr1 = 0;
            Dr1Busy = false;
        }
        if (h->iReg == 2)
        {
            FlagBit = 4;
            ct.Dr2 = 0;
            Dr2Busy = false;
        }
        if (h->iReg == 3)
        {
            FlagBit = 6;
            ct.Dr3 = 0;
            Dr3Busy = false;
        }

        ct.Dr7 &= ~(1 << FlagBit);
    }
    else
    {
        if (!Dr0Busy)
        {
            h->iReg = 0;
            ct.Dr0 = (DWORD_PTR)h->a;
            Dr0Busy = true;
        }
        else if (!Dr1Busy)
        {
            h->iReg = 1;
            ct.Dr1 = (DWORD_PTR)h->a;
            Dr1Busy = true;
        }
        else if (!Dr2Busy)
        {
            h->iReg = 2;
            ct.Dr2 = (DWORD_PTR)h->a;
            Dr2Busy = true;
        }
        else if (!Dr3Busy)
        {
            h->iReg = 3;
            ct.Dr3 = (DWORD_PTR)h->a;
            Dr3Busy = true;
        }
        else
        {
            h->SUCC = false;
            j = ResumeThread(h->hT);
            y = GetLastError();
            SetEvent(h->hEv);
            return 0;
        }
        ct.Dr6 = 0;
        int st = 0;
        if (h->Type == e_HardwareBreakpointType_Code)
            st = 0;
        if (h->Type == e_HardwareBreakpointType_ReadWrite)
            st = 3;
        if (h->Type == e_HardwareBreakpointType_Write)
            st = 1;
        int le = 0;
        if (h->Size == e_HardwareBreakpointSize_1)
            le = 0;
        if (h->Size == e_HardwareBreakpointSize_2)
            le = 1;
        if (h->Size == e_HardwareBreakpointSize_4)
            le = 3;
        if (h->Size == e_HardwareBreakpointSize_8)
            le = 2;

        SetBits(ct.Dr7, 16 + h->iReg * 4, 2, st);
        SetBits(ct.Dr7, 18 + h->iReg * 4, 2, le);
        SetBits(ct.Dr7, h->iReg * 2, 1, 1);
    }

#    endif

    ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    j = SetThreadContext(h->hT, &ct);
    y = GetLastError();

    ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    j = GetThreadContext(h->hT, &ct);
    y = GetLastError();

    j = ResumeThread(h->hT);
    y = GetLastError();

    h->SUCC = true;

    SetEvent(h->hEv);
    return 0;
}

HANDLE SetHardwareBreakpoint(HANDLE hThread, EHardwareBreakpointType Type, EHardwareBreakpointSize Size, void* s)
{
    HWBRK* h = phantom::new_<HWBRK>();
    h->a = s;
    h->Size = Size;
    h->Type = Type;
    h->hT = hThread;

    if (hThread == GetCurrentThread())
    {
        DWORD pid = GetCurrentThreadId();
        h->hT = OpenThread(THREAD_ALL_ACCESS, 0, pid);
    }

    h->hEv = CreateEvent(0, 0, 0, 0);
    h->Opr = 0; // Set Break
    HANDLE hY = CreateThread(0, 0, th, (LPVOID)h, 0, 0);
    WaitForSingleObject(h->hEv, INFINITE);
    CloseHandle(h->hEv);
    h->hEv = 0;

    if (hThread == GetCurrentThread())
    {
        CloseHandle(h->hT);
    }
    h->hT = hThread;

    if (!h->SUCC)
    {
        phantom::delete_<HWBRK>(h);
        return 0;
    }

    return (HANDLE)h;
}

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG || PHANTOM_COMPILER == PHANTOM_COMPILER_GCC

struct custom_unix_clang_x86_context
{
    uint32_t ip;
    uint32_t bp;
    uint32_t sp;
    uint32_t ax;
    uint32_t bx;
    uint32_t cx;
    uint32_t dx;
};

struct custom_unix_clang_x86_64_context
{
    uint64_t ip;
    uint64_t bp;
    uint64_t sp;
    uint64_t ax;
    uint64_t bx;
    uint64_t cx;
    uint64_t dx;
};

#endif // PHANTOM_OPERATING_SYSTEM_WINDOWS

namespace phantom
{
namespace lang
{
Debugger::Debugger()
    : m_pCurrentBreakPoint(nullptr),
      m_bAttached(false),
      m_BasePointer(nullptr),
      m_RootBasePointer(nullptr),
      m_iEnablingCounter(0),
      m_StepByStepBreakPointHandle(0),
      m_StepByStepEIP(nullptr),
      m_StepByStepSubroutine(nullptr),
      m_StepByStepLine(0),
      m_StepByStepEBP(0),
      m_Request(e_Request_None),
      m_pMutex(nullptr),
      m_pGenericContext(nullptr),
      m_bPaused(false)
{
}

Debugger::~Debugger(void) {}

bool Debugger::isAttachable() const
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    return !IsDebuggerPresent();
#else
    return false;
#endif
}

bool Debugger::removeHardwareBreakpoint(size_t hBrk)
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    HWBRK* h = (HWBRK*)hBrk;
    if (!h)
        return false;

    bool C = false;
    if (h->hT == GetCurrentThread())
    {
        DWORD pid = GetCurrentThreadId();
        h->hT = OpenThread(THREAD_ALL_ACCESS, 0, pid);
        C = true;
    }

    h->hEv = CreateEvent(0, 0, 0, 0);
    h->Opr = 1; // Remove Break
    HANDLE hY = CreateThread(0, 0, th, (LPVOID)h, 0, 0);
    WaitForSingleObject(h->hEv, INFINITE);
    CloseHandle(h->hEv);
    h->hEv = 0;

    if (C)
    {
        CloseHandle(h->hT);
    }

    phantom::delete_<HWBRK>(h);
    return true;
#else
    return false;
#endif
}

void Debugger::fetchDebugInformations(DebugInformations& dis) const
{
    for (Module* pModule : Application::Get()->getModules())
    {
        dis.push_back(Compiler::Get()->getDebugInformation(pModule));
    }
}

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS

#    if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86

#        define GENERIC_PROGRAM_COUNTER ((unsigned char*)((LPCONTEXT)m_pGenericContext)->Eip)
#        define GENERIC_FRAME_POINTER ((unsigned char*)((LPCONTEXT)m_pGenericContext)->Ebp)
#        define GENERIC_STACK_POINTER ((unsigned char*)((LPCONTEXT)m_pGenericContext)->Esp)

#    elif PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86_64
#        define GENERIC_PROGRAM_COUNTER ((unsigned char*)((LPCONTEXT)m_pGenericContext)->Rip)
#        define GENERIC_FRAME_POINTER ((unsigned char*)((LPCONTEXT)m_pGenericContext)->Rbp)
#        define GENERIC_STACK_POINTER ((unsigned char*)((LPCONTEXT)m_pGenericContext)->Rsp)

#    elif PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_ARM
#        define GENERIC_PROGRAM_COUNTER ((unsigned char*)((LPCONTEXT)m_pGenericContext)->Pc)
#        define GENERIC_FRAME_POINTER ((unsigned char*)((LPCONTEXT)m_pGenericContext)->R11)
#        define GENERIC_STACK_POINTER ((unsigned char*)((LPCONTEXT)m_pGenericContext)->Rsp)

#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG || PHANTOM_COMPILER == PHANTOM_COMPILER_GCC

#    if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86
#        define GENERIC_PROGRAM_COUNTER ((unsigned char*)((custom_unix_clang_x86_context*)m_pGenericContext)->ip)
#        define GENERIC_FRAME_POINTER ((unsigned char*)((custom_unix_clang_x86_context*)m_pGenericContext)->bp)
#        define GENERIC_STACK_POINTER ((unsigned char*)((custom_unix_clang_x86_context*)m_pGenericContext)->sp)

#    elif PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86_64
#        define GENERIC_PROGRAM_COUNTER ((unsigned char*)((custom_unix_clang_x86_64_context*)m_pGenericContext)->ip)
#        define GENERIC_FRAME_POINTER ((unsigned char*)((custom_unix_clang_x86_64_context*)m_pGenericContext)->bp)
#        define GENERIC_STACK_POINTER ((unsigned char*)((custom_unix_clang_x86_64_context*)m_pGenericContext)->sp)

#    endif

#else
#    error generic stack frame context must be defined for your platform
#endif

bool Debugger::handleHardwareBreakPoint(void* a_pGenericContext)
{
    PHANTOM_LOG(Information, "-------------------");
    PHANTOM_LOG(Information, "HARDWARE BREAKPOINT");
    PHANTOM_LOG(Information, "-------------------");
    //     m_pGenericContext = a_pGenericContext;
    //     m_bBroke = false;
    //     Instruction* pInstruction = findInstructionAtMemoryAddress(GENERIC_PROGRAM_COUNTER);
    //     if(m_StepByStepBreakPointHandle != 0 && (GENERIC_PROGRAM_COUNTER != m_StepByStepEIP))
    //     {
    //         // Step by step
    //         if(pInstruction != nullptr)
    //         {
    //             if((m_StepByStepSubroutine != pInstruction->getSubroutine()) || (m_StepByStepLine
    //             != pInstruction->getCodeRangeLocation().range.begin.line))
    //             {
    //                 if(m_bStepIntoEnabled || (m_StepByStepEBP != *((unsigned
    //                 char**)(GENERIC_FRAME_POINTER+4))))
    //                 {
    //                     m_pCurrentHardwareBreakpointInstruction = pInstruction;
    //                     m_bBroke = true;
    //                 }
    //             }
    //         }
    //         removeHardwareBreakpoint(m_StepByStepBreakPointHandle);
    //     }
    //     else
    //     {
    //         // Data break point
    //
    //     }
    //
    //     if(m_bBroke)
    //     {
    //         PHANTOM_EMIT debuggerBroke();
    //         while(m_bBroke) sleepCurrentThread(10);
    //         PHANTOM_EMIT debuggerResumed();
    //     }
    //
    //     m_pCurrentHardwareBreakpointInstruction = nullptr;
    return true;
}

bool Debugger::handleSoftwareBreakPoint(void* a_pGenericContext)
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    bool result = true;

    // if (pThreadGroup) pThreadGroup->lock();

    PHANTOM_ASSERT(m_iEnablingCounter == 0, "break points should have been disabled");
    m_pGenericContext = a_pGenericContext;
    PHANTOM_LOG(Information, "BREAKING at %x EBP:%x THIS:%x", GENERIC_PROGRAM_COUNTER, GENERIC_FRAME_POINTER,
                *((void**)(GENERIC_FRAME_POINTER - 4)));
    m_bPaused = true;
    m_pSoftwareBreakAddress = GENERIC_PROGRAM_COUNTER;
    BreakPoint* pBreakPoint = findBreakPointForAddress(m_pSoftwareBreakAddress);
    if (pBreakPoint == nullptr)
    {
        PHANTOM_LOG(Information, "BREAKPOINT ! FOUND at %x", m_pSoftwareBreakAddress);
        m_pSoftwareBreakAddress = nullptr;
        // if (pThreadGroup) pThreadGroup->unlock();
        return false;
    }

    PHANTOM_LOG(Information, "BREAKPOINT FOUND with flags = %x", (int)pBreakPoint->getFlags());
    DebugInformations handlers;
    fetchDebugInformations(handlers);
    captureCallStack(handlers);
    for (auto& frame : m_CallStackFrames)
    {
        PHANTOM_LOG(Information, "frame PC:%p BP:%p RETURN:%p", frame.ip, frame.bp, frame.rp);
    }
    m_pCurrentBreakPoint = pBreakPoint;
    PHANTOM_ASSERT(m_pCurrentBreakPoint);

    // disable the breakpoint by restoring original assembly code (overwrites 0xCC interruption
    // instruction)
    m_pCurrentBreakPoint->setEnabled(false);

    m_pThread = phantom::detail::Thread::GetCurrent();
    PHANTOM_ASSERT(m_pThread, "no Thread object defined for the one currently debugged");
    phantom::detail::ThreadGroup* pThreadGroup = m_pThread ? m_pThread->getGroup() : nullptr;

    // If bypass set, we continue execution unless stronger flag is set
    if (m_pCurrentBreakPoint->isByPassed())
    {
        PHANTOM_LOG(Information, "BY PASS");
        m_bPaused = false;
    }

    if (m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_Restorer))
    {
        PHANTOM_LOG(Information, "RESTORER BREAKPOINT");
        m_bPaused = false;
    }

    // Step flags stronger than any other break point flag => we pause
    if ((m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_StepOverLine) ||
         m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_StepOverInstruction) ||
         m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_StepInto) ||
         m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_StepOut)))
    {
        m_bPaused = true;
    }

    // if the breakpoint is an user break point
    if (m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_User))
    {
        m_bPaused = true;
    }

    updateBreakPoints();

    if (m_bPaused)
    {
        PHANTOM_LOG(Information, "m_bBroke");
        PHANTOM_EMIT paused();
        while (m_bPaused)
        {
            if (e_Request_None != m_Request)
            {
                PHANTOM_LOG(Information, "=============================================");
                PHANTOM_LOG(Information, "=============================================");
                PHANTOM_LOG(Information, "=============================================");
            }

            switch (m_Request)
            {
            case e_Request_StepInto:
                PHANTOM_LOG(Information, "e_Request_StepInto");
                applyStepInto();
                break;
            case e_Request_StepOverLine:
                PHANTOM_LOG(Information, "e_Request_StepOverLine");
                applyStepOverLine();
                break;
            case e_Request_StepOverInstruction:
                PHANTOM_LOG(Information, "e_Request_StepOverInstruction");
                applyStepOverInstruction();
                break;
            case e_Request_StepOut:
                PHANTOM_LOG(Information, "e_Request_StepOut");
                applyStepOut();
                break;
            case e_Request_Resume:
                PHANTOM_LOG(Information, "e_Request_Resume");
                _resume();
                break;
            }
            m_Request = e_Request_None;
            if (pThreadGroup)
                pThreadGroup->unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (pThreadGroup)
                pThreadGroup->lock();
        }
        PHANTOM_EMIT resumed();
    }
    else
    {
        clearCurrentBreakpoint();
    }

    PHANTOM_LOG(Information, "RESUMED");
    m_CallStackFrames.clear();
    m_BasePointer = nullptr;
    m_pGenericContext = nullptr;

    byte   mem;
    size_t nbyte;
    PHANTOM_ASSERT(m_pSoftwareBreakAddress);
    ReadProcessMemory(GetCurrentProcess(), m_pSoftwareBreakAddress, &mem, 1, &nbyte);
    PHANTOM_ASSERT(mem != 0xCC);
    m_pSoftwareBreakAddress = nullptr;

    m_pThread = nullptr;
    return true;
#else
    return false;
#endif
}

void Debugger::addRestorerBreakPoints(BreakPoint* a_pBreakPoint)
{
    // a restorer breakpoint is an internel break point which will restore the "int 3" on the break
    // point address once the original code is executed
    byte* pAddress = a_pBreakPoint->getMemoryAddress();

    byte* pNextInstructionAddress = nullptr;
    byte* pUnconditionalBranchAddress = nullptr;
    PHANTOM_LOG(Information, "TRY RESTORER BREAKPOINT BY STEP INTO %x", pAddress);
    Compiler::Get()
    ->getDebugInformation(a_pBreakPoint->getCodeLocation().source->getModule())
    ->findStepIntoInstruction(pAddress, pNextInstructionAddress, m_pGenericContext);
    if (pNextInstructionAddress == nullptr)
    {
        PHANTOM_LOG(Information, "TRY RESTORER BREAKPOINT BY STEP OVER %x", pAddress);

        PHANTOM_ASSERT(m_CallStackFrames.size() > 0);
        Compiler::Get()
        ->getDebugInformation(a_pBreakPoint->getCodeLocation().source->getModule())
        ->findStepOverInstructions(m_CallStackFrames[0].rp, pAddress, pNextInstructionAddress,
                                   pUnconditionalBranchAddress);
    }

    if (pNextInstructionAddress)
    {
        PHANTOM_LOG(Information, "ADD RESTORER BREAKPOINT at %x", pNextInstructionAddress);
        addBreakPoint(pNextInstructionAddress, BreakPoint::e_Flag_Restorer)->setEnabled(true);
    }
    if (pUnconditionalBranchAddress)
    {
        PHANTOM_LOG(Information, "ADD RESTORER BREAKPOINT(branch) at %x", pUnconditionalBranchAddress);
        addBreakPoint(pUnconditionalBranchAddress, BreakPoint::e_Flag_Restorer)->setEnabled(true);
    }
}

void Debugger::removeRestorerBreakPoints()
{
    PHANTOM_LOG(Information, "REMOVE RESTORER BREAKPOINTS");
    bool found = true;
    while (found)
    {
        found = false;
        for (auto& pair : m_BreakPointsForAddress)
        {
            BreakPoint* pBP = pair.second;
            if (pBP->isFlagSet(BreakPoint::e_Flag_Restorer))
            {
                removeBreakPoint(pBP->getMemoryAddress(), BreakPoint::e_Flag_Restorer);
                found = true;
                break;
            }
        }
    }
}

void Debugger::_resume()
{
    PHANTOM_LOG(Information, "RESUME");
    m_bPaused = false;
    clearCurrentBreakpoint();
}

HAUNT_SLOT void Debugger::resume()
{
    m_Request = e_Request_Resume;
}

HAUNT_SLOT void Debugger::stepOverLine()
{
    m_Request = e_Request_StepOverLine;
}

HAUNT_SLOT void Debugger::stepOverInstruction()
{
    m_Request = e_Request_StepOverInstruction;
}

HAUNT_SLOT void Debugger::stepInto()
{
    m_Request = e_Request_StepInto;
}

HAUNT_SLOT void Debugger::stepOut()
{
    m_Request = e_Request_StepOut;
}

bool Debugger::applyStepOut()
{
    PHANTOM_ASSERT(m_bAttached);
    PHANTOM_ASSERT(m_bPaused);
    PHANTOM_ASSERT(m_pCurrentBreakPoint && m_pGenericContext && m_pThread);

    if (m_CallStackFrames.size() > 0)
    {
        byte*        next = m_CallStackFrames[0].rp;
        CodeLocation loc = findCodeLocationForAddress(next);
        if (loc.isValid())
        {
            PHANTOM_LOG(Information, "STEP OUT : inserted at %x", next);
            _resume();
            addBreakPoint(next, BreakPoint::e_Flag_StepOut, loc)->setEnabled(true);
            return true;
        }
        else
        {
            PHANTOM_LOG(Information, "STEP OUT : no code location found at %x", next);
        }
    }
    else
    {
        PHANTOM_LOG(Information, "STEP OUT : no previous stack frame found");
    }
    _resume();
    return false;
}

static bool FindNextInstruction(const CodeLocation& a_CodeLocation, byte* a_pReturnAddress, byte* a_pAddress,
                                byte*& a_pNext, byte*& a_pUnconditionalBranchTarget)
{
    if (Source* pSource = a_CodeLocation.source)
    {
        if (DebugInformation* dbgHandler = Compiler::Get()->getDebugInformation(pSource->getModule()))
        {
            return dbgHandler->findStepOverInstructions(a_pReturnAddress, a_pAddress, a_pNext,
                                                        a_pUnconditionalBranchTarget);
        }
    }
    return false;
}

static bool FindStepIntoInstruction(const CodeLocation& a_CodeLocation, byte* a_pAddress, byte*& a_pNext,
                                    void* a_pGenericContext)
{
    if (Source* pSource = a_CodeLocation.source)
    {
        if (DebugInformation* dbgHandler = Compiler::Get()->getDebugInformation(pSource->getModule()))
        {
            return dbgHandler->findStepIntoInstruction(a_pAddress, a_pNext, a_pGenericContext);
        }
    }
    return false;
}

bool Debugger::applyStepOverLine()
{
    PHANTOM_ASSERT(m_bAttached);
    PHANTOM_ASSERT(m_bPaused);
    PHANTOM_ASSERT(m_pCurrentBreakPoint && m_pGenericContext && m_pThread);

    byte*                 next = m_pCurrentBreakPoint->getMemoryAddress();
    SmallVector<byte*, 8> jumps;
    CodeLocation          breakLoc = m_pCurrentBreakPoint->getCodeLocation();
    CodeLocation          loc = breakLoc;
    bool                  hasJump = false;

    int maxStepInstruction = MAX_STEP_INSTRUCTION;
    while (maxStepInstruction--)
    {
        jumps.push_back(nullptr);
        PHANTOM_LOG(Information, "STEP OVER LINE : looking for next instruction at %x", next);
        PHANTOM_ASSERT(m_CallStackFrames.size() > 0);
        if (FindNextInstruction(loc, m_CallStackFrames[0].rp, next, next, jumps.back()))
        {
            hasJump = hasJump || jumps.back() != nullptr;
            PHANTOM_ASSERT(next);
            CodeLocation nextLoc = findCodeLocationForAddress(next);
            PHANTOM_LOG(Information, "STEP OVER LINE : instruction found at %x (%d|%d)", next, nextLoc.position.line,
                        nextLoc.position.column);
            if (!nextLoc.isValid())
                continue;
            bool notOnSameLine =
            (nextLoc.position.line != loc.position.line); // the next instruction is after this line => OK
            loc = nextLoc;
            if (notOnSameLine)
            {
                PHANTOM_LOG(Information, "STEP OVER LINE : instruction is on the next line : OK", next);
                _resume(); // we prepare for resume the debugger before adding breakpoints
                if (loc.isValid())
                {
                    PHANTOM_LOG(Information, "STEP OVER LINE : inserted at %x ", next);
                    addBreakPoint(next, BreakPoint::e_Flag_StepOverLine, loc)->setEnabled(true);
                }

                // handle all jumps we could encounter before the next line instruction
                PHANTOM_LOG(Information, "STEP OVER LINE : jumps %d", jumps.size());
                for (byte* jump : jumps)
                {
                    if (jump)
                    {
                        loc = findCodeLocationForAddress(jump);
                        while (!loc.isValid())
                        {
                            byte* dummy = nullptr;
                            FindNextInstruction(breakLoc, nullptr, jump, jump, dummy);
                            loc = findCodeLocationForAddress(jump);
                        }
                        if (loc.position.line != breakLoc.position.line)
                        {
                            PHANTOM_LOG(Information, "STEP OVER LINE : inserted at branch:%x", jump);
                            addBreakPoint(jump, BreakPoint::e_Flag_StepOverLine, loc)->setEnabled(true);
                        }
                        else
                        {
                            PHANTOM_LOG(Information, "STEP OVER LINE : jump at branch:%x has no valid code position",
                                        jump);
                        }
                    }
                    else
                    {
                        PHANTOM_LOG(Information, "STEP OVER LINE : no jump", jump);
                    }
                }
                return true;
            }
        }
        else
        {
            PHANTOM_LOG(Information, "STEP OVER LINE FAILED TO EVALUATE TARGET INSTRUCTION ADDRESS");
            _resume();
            // handle all jumps we could encounter before the next line instruction
            for (byte* jump : jumps)
            {
                if (jump)
                {
                    loc = findCodeLocationForAddress(jump);
                    if (loc.isValid() && loc.position.line != m_pCurrentBreakPoint->getCodeLocation().position.line)
                    {
                        PHANTOM_LOG(Information, "STEP INTO : inserted at branch:%x", jump);
                        addBreakPoint(jump, BreakPoint::e_Flag_StepOverLine, loc)->setEnabled(true);
                    }
                }
            }
            return false;
        }
    }
    PHANTOM_LOG(Information, "STEP OVER LINE FAILED TO EVALUATE TARGET INSTRUCTION ADDRESS");
    _resume(); // we prepare for resume the debugger before adding breakpoints
    return false;
}

bool Debugger::applyStepOverInstruction()
{
    PHANTOM_ASSERT(m_bAttached);
    PHANTOM_ASSERT(m_bPaused);
    PHANTOM_ASSERT(m_pCurrentBreakPoint && m_pGenericContext && m_pThread);

    byte*        next = m_pCurrentBreakPoint->getMemoryAddress();
    byte*        jump = nullptr;
    CodeLocation loc = m_pCurrentBreakPoint->getCodeLocation();
    bool         success = FindNextInstruction(loc, m_CallStackFrames[0].rp, next, next, jump);
    if (success)
    {
        loc = findCodeLocationForAddress(next);
        PHANTOM_LOG(Information, "STEP OVER INSTRUCTION : inserted at %x [branch:%x]", next, jump);
        PHANTOM_ASSERT(next);
        _resume();
        if (loc.isValid())
            addBreakPoint(next, BreakPoint::e_Flag_StepOverInstruction, loc)->setEnabled(true);
        if (jump)
        {
            loc = findCodeLocationForAddress(jump);
            if (loc.isValid())
                addBreakPoint(jump, BreakPoint::e_Flag_StepOverInstruction, loc)->setEnabled(true);
        }
        return true;
    }
    else
    {
        PHANTOM_LOG(Information, "STEP OVER INSTRUCTION FAILED TO EVALUATE TARGET INSTRUCTION ADDRESS");
        _resume();
        return false;
    }
}

bool Debugger::applyStepInto()
{
    PHANTOM_ASSERT(m_bAttached);
    PHANTOM_ASSERT(m_bPaused);
    PHANTOM_ASSERT(m_pCurrentBreakPoint && m_pGenericContext && m_pThread);

    byte*                 next = m_pCurrentBreakPoint->getMemoryAddress();
    SmallVector<byte*, 8> jumps;
    SmallVector<byte*, 8> stepIntos;
    CodeLocation          breakLoc = m_pCurrentBreakPoint->getCodeLocation();
    CodeLocation          loc = breakLoc;
    int                   maxStepInstruction = MAX_STEP_INSTRUCTION;
    while (maxStepInstruction--)
    {
        jumps.push_back(nullptr);
        stepIntos.push_back(nullptr);

        // first look for STEP INTO
        PHANTOM_LOG(Information, "STEP INTO : looking for step into instruction at %x", next);
        FindStepIntoInstruction(loc, next, stepIntos.back(), m_pGenericContext);
        if (FindNextInstruction(loc, m_CallStackFrames[0].rp, next, next, jumps.back()))
        {
            PHANTOM_LOG(Information, "STEP INTO : instruction found at %x", next);
            PHANTOM_ASSERT(next);
            CodeLocation nextLoc = findCodeLocationForAddress(next);
            if (!nextLoc.isValid())
                continue;
            bool notOnSameLine =
            (nextLoc.position.line != loc.position.line); // the next instruction is after this line => OK
            loc = nextLoc;
            if (notOnSameLine)
            {
                PHANTOM_LOG(Information, "STEP INTO : instruction is on the next line : OK", next);
                _resume(); // we prepare for resume the debugger before adding breakpoints
                if (loc.isValid())
                {
                    PHANTOM_LOG(Information, "STEP INTO : inserted at next line %x ", next);
                    addBreakPoint(next, BreakPoint::e_Flag_StepOverLine, loc)->setEnabled(true);
                }
                goto end_step_into;
            }
        }
        else
        {
            break;
        }
    }

    _resume(); // we prepare for resume the debugger before adding breakpoints
end_step_into:
    // handle all step intos we could encounter before the next line instruction
    for (byte* stepIntoAddress : stepIntos)
    {
        if (stepIntoAddress)
        {
            // LITTLE HACK : we want to skip function prologue and get into the real block
            byte* dummy = nullptr;
            loc = findCodeLocationForAddress(stepIntoAddress);
            if (loc.isValid())
            {
                PHANTOM_VERIFY(FindNextInstruction(loc, nullptr, stepIntoAddress, stepIntoAddress, dummy));
                PHANTOM_ASSERT(!dummy);
                PHANTOM_VERIFY(FindNextInstruction(loc, nullptr, stepIntoAddress, stepIntoAddress, dummy));
                PHANTOM_ASSERT(!dummy);

                loc = findCodeLocationForAddress(stepIntoAddress);
                if (loc.isValid())
                {
                    PHANTOM_LOG(Information, "STEP INTO : inserted at step into:%x", stepIntoAddress);
                    addBreakPoint(stepIntoAddress, BreakPoint::e_Flag_StepInto, loc)->setEnabled(true);
                }
            }
        }
    }
    // handle all jumps we could encounter before the next line instruction
    for (byte* jump : jumps)
    {
        if (jump)
        {
            loc = findCodeLocationForAddress(jump);
            if (loc.isValid() && loc.position.line != breakLoc.position.line)
            {
                PHANTOM_LOG(Information, "STEP INTO : inserted at branch:%x", jump);
                addBreakPoint(jump, BreakPoint::e_Flag_StepOverLine, loc)->setEnabled(true);
            }
        }
    }
    return true;
}

BreakPoint* Debugger::addBreakPoint(void* a_pAddress, const CodeLocation& a_CodeLocation)
{
    if (m_pCurrentBreakPoint && m_pCurrentBreakPoint->getMemoryAddress() == a_pAddress)
    {
        PHANTOM_LOG(Information, "ADDING BREAK POINT ON THE BROKE ONE : add restorer instead");
        m_pCurrentBreakPoint->setFlag(BreakPoint::e_Flag_User);
        addRestorerBreakPoints(m_pCurrentBreakPoint);
        return m_pCurrentBreakPoint;
    }
    else
    {
        BreakPoint* pBP = addBreakPoint(a_pAddress, BreakPoint::e_Flag_User, a_CodeLocation);
        if (m_iEnablingCounter == 0 && m_bAttached)
        {
            pBP->setEnabled(true);
        }
        return pBP;
    }
}

BreakPoint* Debugger::addBreakPoint(void* a_pAddress)
{
    CodeLocation loc = findCodeLocationForAddress(a_pAddress);
    if (loc.isValid())
        return addBreakPoint(a_pAddress, loc);
    return nullptr;
}

BreakPoint* Debugger::addBreakPoint(void* a_pAddress, BreakPoint::Flags flags)
{
    return addBreakPoint(a_pAddress, flags, findCodeLocationForAddress(a_pAddress));
}

BreakPoint* Debugger::addBreakPoint(void* a_pAddress, BreakPoint::Flags flags, const CodeLocation& a_CodeLocation)
{
    BreakPoint*& pBreakPoint = m_BreakPointsForAddress[a_pAddress];
    if (pBreakPoint == nullptr)
    {
        pBreakPoint = phantom::new_<BreakPoint>(a_pAddress, a_CodeLocation);
    }
    pBreakPoint->setFlags(flags);
    return pBreakPoint;
}

void Debugger::removeBreakPoint(BreakPoint* a_pBreakPoint, BreakPoint::EFlag flag)
{
    auto found = m_BreakPointsForAddress.find(a_pBreakPoint->getMemoryAddress());
    PHANTOM_ASSERT(found != m_BreakPointsForAddress.end() && found->second == a_pBreakPoint);
    a_pBreakPoint->unsetFlag(flag);
    if (a_pBreakPoint->getFlags() == 0)
    {
        a_pBreakPoint->setEnabled(false);

        if (m_pCurrentBreakPoint == a_pBreakPoint)
        {
            PHANTOM_LOG(Information, "CURRENT BREAKPOINT REMOVED/NULLIFIED");
            m_pCurrentBreakPoint = nullptr;
        }

        PHANTOM_LOG(Information, "m_BreakPoints.erase(found);");
        m_BreakPointsForAddress.erase(found);

        PHANTOM_LOG(Information, "delete a_pBreakPoint (%x); ", a_pBreakPoint);
        phantom::delete_<BreakPoint>(a_pBreakPoint);
    }
}

void Debugger::removeBreakPoint(BreakPoint* a_pBreakPoint)
{
    if (m_pMutex)
        m_pMutex->lock();
    PHANTOM_ASSERT(a_pBreakPoint->isFlagSet(BreakPoint::e_Flag_User));
    if (m_pCurrentBreakPoint == a_pBreakPoint)
    {
        removeRestorerBreakPoints(); // instead of removing the break point, we remove the restorer
                                     // break point
        m_pCurrentBreakPoint->unsetFlag(BreakPoint::e_Flag_User);
    }
    else
        removeBreakPoint(a_pBreakPoint, BreakPoint::e_Flag_User);
    if (m_pMutex)
        m_pMutex->unlock();
}

void Debugger::removeBreakPoint(void* a_pAddress, BreakPoint::EFlag flag)
{
    BreakPoint* pBP = getBreakPoint(a_pAddress);
    PHANTOM_ASSERT(pBP);
    removeBreakPoint(pBP, flag);
}

byte* BreakPoint::nextAvailableBreakPointLocation()
{
    return nullptr;
    //     return (getMemoryEnd() < m_pInstruction->getMemoryEnd())
    //         ? m_pInstruction->getMemoryEnd()
    //         : getMemoryEnd();
}

void BreakPoint::setEnabled(bool value)
{
    if (isEnabled() == value)
        return;
    size_t written;

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    if (value)
    {
        // PHANTOM_LOG(Information, "BP ENABLED with %x", m_Flags);
        // enable
        m_iStates |= e_State_Enabled;

        // save instruction code
        m_OriginalOpCode = *m_pMemoryAddress;

        // replace by "int 3" trap op code
        byte int3 = 0xCC;
        PHANTOM_LOG(Information, "BreakPoint enabled at %x : replace %x by %x", m_pMemoryAddress, *m_pMemoryAddress,
                    int3);
        PHANTOM_VERIFY(WriteProcessMemory(GetCurrentProcess(), m_pMemoryAddress, &int3, sizeof(byte), &written));
    }
    else
    {
        PHANTOM_LOG(Information, "BreakPoint disabled at %x : replace %x by %x", m_pMemoryAddress, *m_pMemoryAddress,
                    m_OriginalOpCode);
        // PHANTOM_LOG(Information, "BP DISABLED with %x", m_Flags);
        m_iStates &= ~e_State_Enabled;
        // restore instruction code
        PHANTOM_VERIFY(
        WriteProcessMemory(GetCurrentProcess(), m_pMemoryAddress, &m_OriginalOpCode, sizeof(byte), &written));
    }
    PHANTOM_VERIFY(FlushInstructionCache(GetCurrentProcess(), m_pMemoryAddress, BREAK_INSTRUCTION_SIZE));
#elif PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_LINUX && PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86
    PHANTOM_VERIFY(cacheflush(m_pMemoryAddress, BREAK_INSTRUCTION_SIZE, ICACHE) == 0);
#else
#    pragma message "cache instruction flushing won't be available to use the Debugger break points"
#endif
}

bool BreakPoint::findStepOverLineInstructions(byte* a_pReturnAddress, byte*& a_pNext,
                                              byte*& a_pUnconditionalBranchTarget) const
{
    if (Source* pSource = m_CodeLocation.source)
    {
        if (DebugInformation* dbgHandler = Compiler::Get()->getDebugInformation(pSource->getModule()))
        {
            return dbgHandler->findStepOverInstructions(a_pReturnAddress, m_pMemoryAddress, a_pNext,
                                                        a_pUnconditionalBranchTarget);
        }
    }
    return false;
}

bool BreakPoint::findStepIntoInstruction(byte*& a_pCallAddress, void* a_pGenericContext) const
{
    if (Source* pSource = m_CodeLocation.source)
    {
        if (DebugInformation* dbgHandler = Compiler::Get()->getDebugInformation(pSource->getModule()))
        {
            return dbgHandler->findStepIntoInstruction(m_pMemoryAddress, a_pCallAddress, a_pGenericContext);
        }
    }
    return false;
}

BreakPoint* Debugger::findBreakPointForAddress(void* start)
{
    auto found = m_BreakPointsForAddress.find(start);
    return found == m_BreakPointsForAddress.end() ? nullptr : found->second;
}

bool Debugger::attach()
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    if (IsDebuggerPresent())
    {
        return false;
    }
#endif
    PHANTOM_EMIT aboutToBeAttached();
    PHANTOM_LOG(Information, "attach");
    PHANTOM_ASSERT(!m_bAttached);
    m_bAttached = true;
    updateBreakPoints();
    PHANTOM_EMIT attached();
    return true;
}

void Debugger::detach()
{
    PHANTOM_ASSERT(m_bAttached);
    PHANTOM_EMIT aboutToBeDetached();
    PHANTOM_LOG(Information, "detach");
    if (m_bPaused)
    {
        _resume();
    }
    PHANTOM_LOG(Information, "waiting ");

    while (m_pGenericContext)
    {
        std::this_thread::yield(); // wait for _resume completed
    }

    removeRestorerBreakPoints();

    PHANTOM_ASSERT(m_bAttached);
    auto it = m_BreakPointsForAddress.begin();
    auto end = m_BreakPointsForAddress.end();
    for (; it != end; ++it)
    {
        PHANTOM_ASSERT(it->second->getFlags() &
                       (BreakPoint::e_Flag_Restorer | BreakPoint::e_Flag_StepInto | BreakPoint::e_Flag_StepOverLine |
                        BreakPoint::e_Flag_StepOverInstruction | BreakPoint::e_Flag_StepOut),
                       "only user break points should remain in the list at this point, no more step/restorer");
        it->second->setEnabled(false);
    }
    m_bAttached = false;
    PHANTOM_EMIT detached();
}

// AddVectoredExceptionHandler constants:
// CALL_FIRST means call this exception handler first;
// CALL_LAST means call this exception handler last
#define CALL_FIRST 1
#define CALL_LAST 0

long Debugger::_handleBreakException(struct _EXCEPTION_POINTERS* info, unsigned long code)
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    switch (code)
    {
    case EXCEPTION_BREAKPOINT:
    {
        if (!(this->handleSoftwareBreakPoint(info->ContextRecord)))
        {
            DWORD pid = GetCurrentProcessId();
            char  buffer[256] = "";
            sprintf(buffer, "VsJITDebugger.exe -p %d", pid);
            system(buffer);
            return EXCEPTION_CONTINUE_SEARCH;
        }
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    case STATUS_SINGLE_STEP:
    {
        if (!(this->handleHardwareBreakPoint(info->ContextRecord)))
        {
            DWORD pid = GetCurrentProcessId();
            char  buffer[256] = "";
            sprintf(buffer, "VsJITDebugger.exe -p %d", pid);
            system(buffer);
            return EXCEPTION_CONTINUE_SEARCH;
        }
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    default:
        break;
    }
    DWORD pid = GetCurrentProcessId();
    char  buffer[256] = "";
    sprintf(buffer, "VsJITDebugger.exe -p %d", pid);
    system(buffer);
    return EXCEPTION_CONTINUE_SEARCH;
#endif
}

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
long __stdcall VectoredHandlerBreakPoint(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
    return phantom::lang::Compiler::Get()->getDebugger()->_handleBreakException(
    ExceptionInfo, ExceptionInfo->ExceptionRecord->ExceptionCode);
}
#endif

void Debugger::_debug(std::function<void()>&& func)
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS

    CONTEXT ctx;
    if (GetThreadContext(NULL, &ctx))
    {
#    if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86
        m_RootBasePointer = (byte*)ctx.Ebp;
#    else
        m_RootBasePointer = (byte*)ctx.Rbp;
#    endif
    }

    auto handler = [this](LPEXCEPTION_POINTERS info, unsigned long code) -> LONG {
        return _handleBreakException(info, code);
    };

#    if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X64

    PVOID hVeh = AddVectoredExceptionHandler(CALL_FIRST, VectoredHandlerBreakPoint);
    func();
    if (hVeh)
        RemoveVectoredExceptionHandler(hVeh);

#    else
    __try
    {
        func();
    }
    __except (handler(GetExceptionInformation(), GetExceptionCode()))
    {
    }
#    endif

#else
    PHANTOM_LOG(Error, "Debugger::debug() not implemented in the current platform");
    func();
#endif
}

Subroutine* Debugger::_findSubroutineAtAddress(DebugInformations& handlers, byte* ip)
{
    for (DebugInformation* pDbgHandler : handlers)
    {
        if (Subroutine* pSubroutine = pDbgHandler->getSubroutineAtAddress(ip))
            return pSubroutine;
    }
    return nullptr;
}

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS

void Debugger::_StackTrace64()
{
    CONTEXT                       Context;
    KNONVOLATILE_CONTEXT_POINTERS NvContext;
    UNWIND_HISTORY_TABLE          UnwindHistoryTable;
    PRUNTIME_FUNCTION             RuntimeFunction;
    PVOID                         HandlerData;
    ULONG64                       EstablisherFrame;
    ULONG64                       ImageBase;

    //
    // First, we'll get the caller's context.
    //

    size_t frameIndex = 0;

    memcpy(&Context, (LPCONTEXT)m_pGenericContext, sizeof(CONTEXT));
    FrameInfo fi;
    fi.bp = (byte*)Context.Rbp;
    fi.ip = (byte*)Context.Rip;
    fi.rp = (byte*)0;
    fi.sp = (byte*)Context.Rsp;
    m_CallStackFrames.push_back(fi);
    FrameInfo* prevInfo = &m_CallStackFrames.back();

    //
    // Initialize the (optional) unwind history table.
    //

    RtlZeroMemory(&UnwindHistoryTable, sizeof(UNWIND_HISTORY_TABLE));

    //
    // This unwind loop intentionally skips the first call frame, as it shall
    // correspond to the call to StackTrace64, which we aren't interested in.
    //

    for (ULONG Frame = 0;; Frame++)
    {
        //
        // Try to look up unwind metadata for the current function.
        //

        RuntimeFunction = RtlLookupFunctionEntry(Context.Rip, &ImageBase, &UnwindHistoryTable);

        RtlZeroMemory(&NvContext, sizeof(KNONVOLATILE_CONTEXT_POINTERS));

        if (!RuntimeFunction)
        {
            PHANTOM_LOG(Error, "FRAME %02x: we don't have a RUNTIME_FUNCTION", Frame);
            //
            // If we don't have a RUNTIME_FUNCTION, then we've encountered
            // a leaf function.  Adjust the stack approprately.
            //

            Context.Rip = (ULONG64)(*(PULONG64)Context.Rsp);
            Context.Rsp += 8;
        }
        else
        {
            //
            // Otherwise, call upon RtlVirtualUnwind to execute the unwind for
            // us.
            //

            RtlVirtualUnwind(UNW_FLAG_NHANDLER, ImageBase, Context.Rip, RuntimeFunction, &Context, &HandlerData,
                             &EstablisherFrame, &NvContext);
            FrameInfo fi;
            fi.bp = (byte*)Context.Rbp;
            fi.ip = (byte*)Context.Rip;
            fi.rp = (byte*)0;
            fi.sp = (byte*)Context.Rsp;
            prevInfo->rp = fi.ip;
            m_CallStackFrames.push_back(fi);
            prevInfo = &m_CallStackFrames.back();
        }
        //
        // If we reach an RIP of zero, this means that we've walked off the end
        // of the call stack and are done.
        //

        if (!Context.Rip)
            break;

        //
        // Display the context.  Note that we don't bother showing the XMM
        // context, although we have the nonvolatile portion of it.
        //

        PHANTOM_LOG(Error, "FRAME %02x: Rip=%p Rsp=%p Rbp=%p", Frame, Context.Rip, Context.Rsp, Context.Rsp);
        PHANTOM_LOG(Error,
                    "r12=%p r13=%p r14=%p\n"
                    "rdi=%p rsi=%p rbx=%p\n"
                    "rbp=%p rsp=%p",
                    Context.R12, Context.R13, Context.R14, Context.Rdi, Context.Rsi, Context.Rbx, Context.Rbp,
                    Context.Rsp);

        static const CHAR* RegNames[16] = {"Rax", "Rcx", "Rdx", "Rbx", "Rsp", "Rbp", "Rsi", "Rdi",
                                           "R8",  "R9",  "R10", "R11", "R12", "R13", "R14", "R15"};

        //
        // If we have stack-based register stores, then display them here.
        //
        for (ULONG i = 0; i < 16; i++)
        {
            if (NvContext.IntegerContext[i])
            {
                PHANTOM_LOG(Error, " -> Saved register '%s' on stack at %p (=> %p)", RegNames[i],
                            NvContext.IntegerContext[i], *NvContext.IntegerContext[i]);
            }
        }
    }

    return;
}
#endif

void Debugger::captureCallStack(DebugInformations& handlers)
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS

#    if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86
    DWORD machineType = IMAGE_FILE_MACHINE_I386;
#    elif PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X64
    DWORD machineType = IMAGE_FILE_MACHINE_AMD64;
    _StackTrace64();
    return;
#    else
#        error unsupported architecture
#    endif // #error unsupported architecture
    PHANTOM_ASSERT(false);
    CONTEXT context;
    memcpy(&context, ((LPCONTEXT)m_pGenericContext), sizeof(CONTEXT));

    STACKFRAME stack;
    ZeroMemory(&stack, sizeof(STACKFRAME));
    stack.AddrPC.Offset = context.Rip;
    stack.AddrPC.Mode = AddrModeFlat;
    stack.AddrStack.Offset = context.Rsp;
    stack.AddrStack.Mode = AddrModeFlat;
    stack.AddrFrame.Offset = context.Rdi;
    stack.AddrFrame.Mode = AddrModeFlat;
    stack.AddrReturn.Mode = AddrModeFlat;
    PHANTOM_LOG(Error, "STACK FRAME ==========================");
    for (int frame = 0;; frame++)
    {
        if (StackWalk64(machineType, GetCurrentProcess(), GetCurrentThread(), &stack, &context, NULL,
                        SymFunctionTableAccess64, SymGetModuleBase64, NULL))
        {
            FrameInfo info;
            info.func = 0;
            info.ip = (byte*)stack.AddrPC.Offset;
            info.bp = (byte*)stack.AddrFrame.Offset;
            info.sp = (byte*)stack.AddrStack.Offset;
            info.rp = (byte*)stack.AddrReturn.Offset;
            PHANTOM_LOG(Error, "%d : %p", info.bp);
            m_CallStackFrames.push_back(info);
        }
        else
            break;
    }
#    if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X64
    _StackTrace64();
#    endif

#else
    _captureCallStack(handlers, GENERIC_PROGRAM_COUNTER, GENERIC_FRAME_POINTER, GENERIC_STACK_POINTER);
#endif
}

void Debugger::_captureCallStack(DebugInformations& handlers, byte* ip, byte* bp, byte* sp)
{
    PHANTOM_UNREACHABLE();
    //     bool currentFrame = false;
    //     if (m_BasePointer == nullptr)
    //     {
    //         m_BasePointer = bp;
    //         currentFrame = true;
    //     }
    //     byte* up_ip = *((byte**)(bp + PHANTOM_SIZE_OF_VOID_P)) - CALL_INSTRUCTION_SIZE;
    //     byte* up_bp = *((byte**)bp);
    //
    //     Subroutine* pSubroutine = _findSubroutineAtAddress(handlers, bp);
    //
    //     m_CallStackFrames.push_back(FrameInfo{ pSubroutine, ip, bp, sp, 0 });
    //     if (up_bp == m_RootBasePointer) return;
    //     _captureCallStack(handlers, up_ip, up_bp);
}

void Debugger::clearCurrentBreakpoint()
{
    // Step over flag stronger than bypass
    PHANTOM_LOG(Information, "REMOVE STEP BREAKPOINTS");
    PHANTOM_ASSERT(m_pCurrentBreakPoint);
    // if the breakpoint is an user break point
    if (m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_User))
    {
        PHANTOM_LOG(Information, "addRestorerBreakPoints(m_pCurrentBreakPoint)");
        // we add a restorer break point, or more precisely a silent breakpoint which on break will
        // restore the user breakpoint (we can't restore now because we need to first execute what
        // replaced the breakpoint) we insert this breakpoint on the next executed instruction (step
        // into)
        addRestorerBreakPoints(m_pCurrentBreakPoint);
    }
    if (m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_StepOverLine) ||
        m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_StepOverInstruction) ||
        m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_StepInto) ||
        m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_StepOut))
    {
        removeBreakPoints(BreakPoint::e_Flag_StepOverLine);
        removeBreakPoints(BreakPoint::e_Flag_StepOverInstruction);
        removeBreakPoints(BreakPoint::e_Flag_StepInto);
        removeBreakPoints(BreakPoint::e_Flag_StepOut);
    }
    if (m_pCurrentBreakPoint && m_pCurrentBreakPoint->isFlagSet(BreakPoint::e_Flag_Restorer))
    {
        PHANTOM_LOG(Information, "REMOVE RESTORER BREAKPOINT");
        removeBreakPoints(BreakPoint::e_Flag_Restorer);
    }
    if (m_pCurrentBreakPoint && m_pCurrentBreakPoint->getFlags() == 0)
    {
        PHANTOM_LOG(Information, "REMOVE EMPTY CURRENT BREAKPOINT");
        PHANTOM_ASSERT(!m_pCurrentBreakPoint->isEnabled());
        m_BreakPointsForAddress.erase(m_pCurrentBreakPoint->getMemoryAddress());
        phantom::delete_<BreakPoint>(m_pCurrentBreakPoint);
    }
    m_pCurrentBreakPoint = nullptr;
}

Source* Debugger::getFrameSource(size_t i) const
{
    return getFrameCodeLocation(i).source;
}

SourceFile* Debugger::getFrameSourceFile(size_t i) const
{
    Source* pSource = getFrameSource(i);
    return (pSource && pSource->getSourceStream()) ? pSource->getSourceStream()->asFile() : nullptr;
}

bool Debugger::doesBreakPointMustBeEnabled(BreakPoint* a_pBreakPoint) const
{
    return (a_pBreakPoint != m_pCurrentBreakPoint) &&
    (a_pBreakPoint->isFlagSet(BreakPoint::e_Flag_User) || a_pBreakPoint->isFlagSet(BreakPoint::e_Flag_Restorer));
}

void Debugger::updateBreakPoints()
{
    PHANTOM_LOG(Information, "UPDATE BREAKPOINTS:");
    // Enabling/Disabling pass
    {
        auto it = m_BreakPointsForAddress.begin();
        auto end = m_BreakPointsForAddress.end();
        for (; it != end; ++it)
        {
            BreakPoint* a_pBreakPoint = it->second;
            if (a_pBreakPoint != m_pCurrentBreakPoint && a_pBreakPoint->isFlagSet(BreakPoint::e_Flag_User))
            {
                a_pBreakPoint->setEnabled(true);
            }
        }
    }
}

LanguageElement* Debugger::findElementForAddress(void* a_pAddress) const
{
    DebugInformations handlers;
    fetchDebugInformations(handlers);

    for (DebugInformation* pHandler : handlers)
    {
        CodeLocation loc = pHandler->getCodeLocationForAddress(a_pAddress);
        if (loc.isValid())
        {
            return loc.source->getElementAtCodePosition(loc.position);
        }
    }
    return nullptr;
}

CodeLocation Debugger::findCodeLocationForAddress(void* a_pAddress) const
{
    DebugInformations handlers;
    fetchDebugInformations(handlers);

    for (DebugInformation* pHandler : handlers)
    {
        CodeLocation loc = pHandler->getCodeLocationForAddress(a_pAddress);
        if (loc.isValid())
        {
            return loc;
        }
    }
    return CodeLocation();
}

CodeLocation Debugger::getFrameCodeLocation(size_t i) const
{
    byte* address = m_CallStackFrames[i].ip;
    byte* jmpDummy;
    int   maxLoop = MAX_STEP_INSTRUCTION;
    while (maxLoop--)
    {
        CodeLocation loc = findCodeLocationForAddress(address);
        if (loc.isValid())
            return loc;
        if (!FindNextInstruction(loc, m_CallStackFrames[i].rp, address, address, jmpDummy))
            break;
    }
    return CodeLocation();
}

Subroutine* Debugger::getFrameSubroutine(size_t i) const
{
    Block* pBlock = getFrameBlock(i);
    return pBlock ? pBlock->getSubroutine() : nullptr;
}

Block* Debugger::getFrameBlock(size_t i) const
{
    CodeLocation loc = getFrameCodeLocation(i);
    if (loc.source)
    {
        LanguageElement* pElement = loc.source->getElementAtCodePosition(loc.position);
        while (pElement)
        {
            if (Block* pBlock = pElement->asBlock())
            {
                return pBlock;
            }
            pElement = pElement->getOwner();
        }
    }
    return nullptr;
}

void Debugger::clear()
{
    PHANTOM_LOG(Information, "CLEAR DEBUGGER");
    PHANTOM_ASSERT(!m_bAttached);

    // m_BreakPoints.clear();
    m_CallStackFrames.clear();
    m_pCurrentBreakPoint = nullptr;
    m_BasePointer = nullptr;
    m_RootBasePointer = nullptr;
    m_bPaused = false;
}

byte* Debugger::getFrameBasePointer(size_t i) const
{
    return m_CallStackFrames[i].bp;
}

byte* Debugger::getFrameStackPointer(size_t i /*= 0*/) const
{
    return m_CallStackFrames[i].sp;
}

byte* Debugger::getFrameReturnAddress(size_t i /*= 0*/) const
{
    return m_CallStackFrames[i].rp;
}

byte* Debugger::getFrameInstructionPointer(size_t i) const
{
    return m_CallStackFrames[i].ip;
}

byte* Debugger::getFrameLocalVariableAddress(LocalVariable* a_pLocalVariable, size_t i /*= 0*/) const
{
    return Compiler::Get()
    ->getDebugInformation(a_pLocalVariable->getModule())
    ->getLocalVariableAddress(a_pLocalVariable, i);
}

void Debugger::getFrameCodeLocations(SmallVector<CodeLocation>& a_Frames) const
{
    for (size_t i = 0; i < m_CallStackFrames.size(); ++i)
    {
        a_Frames.push_back(getFrameCodeLocation(i));
    }
}

void Debugger::resumeAndExecute(std::function<void()>&& a_ToExecute, phantom::detail::Thread* a_pExecutionContextThread)
{
    PHANTOM_ASSERT(m_bPaused);
    PHANTOM_ASSERT(m_pThread);
    if (a_pExecutionContextThread == nullptr)
        a_pExecutionContextThread = phantom::detail::Thread::GetCurrent();
    PHANTOM_ASSERT(m_pThread != a_pExecutionContextThread);
    m_pThread->pushCall(
    [a_ToExecute, a_pExecutionContextThread]() { a_pExecutionContextThread->pushCallAndWait(a_ToExecute); });
    resume();
}

void Debugger::enableBreakPoints()
{
    // PHANTOM_LOG(Information, "ENABLE BREAKPOINTS");
    PHANTOM_ASSERT(m_iEnablingCounter > 0);
    m_iEnablingCounter--;
    if (m_iEnablingCounter == 0)
    {
        auto it = m_RememberedBreakPoints.begin();
        auto end = m_RememberedBreakPoints.end();
        for (; it != end; ++it)
        {
            (*it)->setEnabled(true);
        }
        m_RememberedBreakPoints.clear();
    }
}

void Debugger::disableBreakPoints()
{
    // PHANTOM_LOG(Information, "DISABLE BREAKPOINTS");
    PHANTOM_ASSERT(m_iEnablingCounter >= 0);
    if (m_iEnablingCounter == 0)
    {
        PHANTOM_ASSERT(m_RememberedBreakPoints.empty());
        auto it = m_BreakPointsForAddress.begin();
        auto end = m_BreakPointsForAddress.end();
        for (; it != end; ++it)
        {
            BreakPoint* a_pBreakPoint = it->second;
            if (a_pBreakPoint->isEnabled())
            {
                a_pBreakPoint->setEnabled(false);
                m_RememberedBreakPoints.push_back(a_pBreakPoint);
            }
        }
    }
    m_iEnablingCounter++;
}

void Debugger::removeBreakPoints(BreakPoint::EFlag flag)
{
    BreakPoints to_remove;
    for (auto it = m_BreakPointsForAddress.begin(); it != m_BreakPointsForAddress.end(); ++it)
    {
        if (it->second->isFlagSet(flag))
            to_remove.push_back(it->second);
    }
    PHANTOM_LOG(Information, "START REMOVING");
    for (auto it = to_remove.begin(); it != to_remove.end(); ++it)
    {
        removeBreakPoint(*it, flag);
    }
}

bool DebuggerFrameContext::call(Subroutine* a_pSubroutine, void** a_ppArgs, size_t a_uiCount)
{
    a_pSubroutine->call(*this, a_ppArgs, a_uiCount);
    return true;
}

byte* DebuggerFrameContext::getStackPointer() const
{
    return Compiler::Get()->getDebugger()->getFrameStackPointer(m_Frame);
}

byte* DebuggerFrameContext::getBasePointer() const
{
    return Compiler::Get()->getDebugger()->getFrameBasePointer(m_Frame);
}

void* DebuggerFrameContext::getReturnAddress() const
{
    return Compiler::Get()->getDebugger()->getFrameReturnAddress(m_Frame);
}

byte* DebuggerFrameContext::addressOfThis() const
{
    Subroutine* pSubroutine = Compiler::Get()->getDebugger()->getFrameSubroutine(m_Frame);
    PHANTOM_ASSERT(pSubroutine);
    PHANTOM_ASSERT(pSubroutine->asMethod());
    return addressOf(static_cast<Method*>(pSubroutine)->getThis());
}

byte* DebuggerFrameContext::addressOf(LocalVariable* a_pLocalVariable) const
{
    return Compiler::Get()->getDebugger()->getFrameLocalVariableAddress(a_pLocalVariable, m_Frame);
}

#if 0

// DEBUGGER PIPELINE

DebuggerPipelineLoop()
{
	Sentry sentry;
	DebugEngine debugEngine;
	NativeDebugEngineProxy nativeDebugEngineProxy;
	game.connectTo(sentry);

	while(true)
	{
		if (game.debugger.break)
		{
			if (game.debugger.debugInformation)
			{
				if (!debugEngine)
				{
					debugEngine = new DebugEngine;
				}
				debugEngine->attach(game);
				debugEngine->breakAt(game.debugger.debugInformation);

				if (debugEngine.play)
				{
					game.debugger.resume();
				}
			}
			else
			{
				nativeDebugEngineProxy.attach()
			}
		}

}

#endif

} // namespace lang
} // namespace phantom
