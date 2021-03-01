// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/ExecutionContext.h>
#include <phantom/lang/lang.h>
#include <phantom/utils/SmallVector.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM_CODE Interpreter
{
public:
    bool call(ExecutionContext& a_Context, Subroutine* a_pSubroutine, void** a_ppArgs, size_t a_uiCount);

    void setNextStatement(Statement* a_pStatement);

    byte*& stackPointer() { return m_pStackPointer; }

    byte* getStackPointer() const { return m_pStackPointer; }

    byte* getBasePointer() const { return m_pBasePointer; }

    void* getReturnAddress() const { return m_ReturnAddresses.back(); }

    void setDefaultThisPointer(void* a_pPointer) { m_pThisPointer = a_pPointer; }

    void* const* getDefaultThisPointerAddress() const { return &m_pThisPointer; }

    void* getDefaultThisPointer() const { return m_pThisPointer; }

private:
    SmallVector<byte, 1> m_Stack;
    byte*                m_pBasePointer{};
    byte*                m_pStackPointer{};
    Statements           m_StatementStack;
    VoidPtrs             m_ReturnAddresses;
    void*                m_pThisPointer{};
};

class PHANTOM_EXPORT_PHANTOM_CODE InterpreterContext : public ExecutionContext
{
public:
    Interpreter* getInterpreter() { return &m_Interpreter; }

    virtual bool call(Subroutine* a_pSubroutine, void** a_ppArgs, size_t a_uiCount) override
    {
        return m_Interpreter.call(*this, a_pSubroutine, a_ppArgs, a_uiCount);
    }
    virtual void  setNextStatement(Statement* a_pStatement) override { m_Interpreter.setNextStatement(a_pStatement); }
    virtual byte* getStackPointer() const override { return m_Interpreter.getStackPointer(); }
    virtual byte* getBasePointer() const override { return m_Interpreter.getBasePointer(); }
    virtual void* getReturnAddress() const override { return m_Interpreter.getReturnAddress(); }
    virtual byte* addressOfThis() const override;
    virtual byte* addressOf(LocalVariable* a_pLocalVariable) const override;

private:
    Interpreter m_Interpreter;
};

} // namespace lang
} // namespace phantom
