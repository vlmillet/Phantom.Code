// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "lang.h"

#include <phantom/lang/CodeLocation.h>

namespace phantom
{
namespace lang
{
class BreakPoint;

class PHANTOM_EXPORT_PHANTOM_CODE DebugInformation
{
    friend class Compiler;

public:
    DebugInformation() : m_pModule(nullptr)
    {
    }

    Module* getModule() const
    {
        return m_pModule;
    }

    BreakPoint* addBreakPoint(const CodeLocation& a_Position);
    BreakPoint* addBreakPoint(void*               a_pAddress,
                              const CodeLocation& a_CandidatePosition = CodeLocation());

    void removeBreakPoint(BreakPoint* a_pBreakPoint);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the code position in the source matching the given program memory address
    /// (available for JIT compilers debugging).
    ///
    /// \param a_pAddress  The address in memory.
    ///
    /// \return the code position matching the given address, else an invalid CodePosition().
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual CodeLocation getCodeLocationForAddress(void* a_pAddress) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the address matching the given code position in the source (available for JIT
    /// assembly).
    ///
    /// \param a_CodePosition  The code position in the source associated with this compiler.
    ///
    /// \return the address matching the code position, else nullptr.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* getAddressForCodeLocation(CodeLocation a_CodeLocation) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the next instruction pointer which will be executed after the given instruction
    /// pointer
    ///         in the current frame (generally called a "step over") (available for JIT assembly).
    ///
    /// \param a_pAddress  The current instruction pointer (ex: the program counter).
    ///
    /// \return the address the processor will go to after the current instruction in the same
    /// frame.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool findStepOverInstructions(byte* a_pReturnAddress, byte* a_pAddress, byte*& a_pNext,
                                          byte*& a_pUnconditionalBranchTarget) const;
    virtual bool findStepIntoInstruction(byte* a_pAddress, byte*& a_pCallAddress,
                                         void* a_pGenericThreadContext) const;
    virtual Subroutine* getSubroutineAtAddress(byte* a_pAddress) const;

    virtual byte* getLocalVariableAddress(LocalVariable* a_pLocalVariable,
                                          size_t         a_iDebuggerFrameIndex) const;

private:
    Module* m_pModule;
};
} // namespace lang
} // namespace phantom
