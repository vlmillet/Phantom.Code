// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "Interpreter.h"

#include "Block.h"
#include "Message.h"

#include <phantom/lang/LocalVariable.h>
#include <phantom/lang/Subroutine.h>
#include <phantom/utils/Placement.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
bool Interpreter::call(ExecutionContext& a_Context, Subroutine* a_pSubroutine, void** a_ppArgs, size_t a_uiCount)
{
    if (m_Stack.empty())
    {
        m_Stack.resize(8192);
    }
    if (m_pStackPointer == 0)
    {
        PHANTOM_ASSERT(m_pBasePointer == 0);
        m_pStackPointer = m_Stack.data();
    }
    byte* pPrevBasePointer = m_pBasePointer;
    byte* pPrevStackPointer = m_pStackPointer;
    m_pBasePointer = m_pStackPointer;
    Block* pBlock = a_pSubroutine->getBlock();
    PHANTOM_ASSERT(pBlock, "interpreter call of an undefined subroutine");
    size_t count = a_pSubroutine->getParameters().size() + (a_pSubroutine->getABI() == ABI::MethodCall);
    PHANTOM_ASSERT(a_uiCount <= count, "too many arguments");
    PHANTOM_ASSERT(a_uiCount >= count, "not enough arguments");
    while (count--)
    {
        LocalVariable* pLocalVariable = pBlock->getLocalVariable(count);
        byte*          address = m_pStackPointer + pLocalVariable->getEvalFrameOffset();
        Type*          pValueType = pLocalVariable->getValueType();
        if (auto pClassType = pValueType->asClassType())
        {
            if (auto pCCtor = pClassType->getCopyConstructor())
            {
                void* arg[1] = {a_ppArgs[count]};
                pCCtor->construct(address, arg);
            }
        }
        else
        {
            if (pValueType->asReference())
                memcpy(address, &a_ppArgs[count], sizeof(void*));
            else
                memcpy(address, a_ppArgs[count], pValueType->getSize());
        }
    }
    m_pStackPointer += a_pSubroutine->getFrameSize();
    Statement* pFirstStatement = pBlock->getFirstNonBlockStatementCascade();
    if (pFirstStatement)
    {
        m_StatementStack.push_back(pFirstStatement);
        m_ReturnAddresses.push_back(a_Context.resultPointer());
        while (m_StatementStack.back())
        {
            Statement* pCurrentStatement = m_StatementStack.back();
            pCurrentStatement->eval(a_Context);
            a_Context.releaseTemporaries(pCurrentStatement);
            if (pCurrentStatement == m_StatementStack.back()) // no branch done
            {
                m_StatementStack.back() = m_StatementStack.back()->getNextStatement();
            }
        }
        m_StatementStack.pop_back();
        m_ReturnAddresses.pop_back();
    }
    m_pBasePointer = pPrevBasePointer;
    m_pStackPointer = pPrevStackPointer;
    for (size_t i = 0; i < a_uiCount; ++i)
    {
        LocalVariable* pLocalVariable = pBlock->getLocalVariable(i);
        byte*          address = m_pStackPointer + pLocalVariable->getEvalFrameOffset();
        Type*          pValueType = pLocalVariable->getValueType();
        pValueType->destroy(address);
    }
    a_Context.releaseTemporaries(a_pSubroutine->getBlock());
    return true;
}

void Interpreter::setNextStatement(Statement* a_pStatement)
{
    m_StatementStack.back() = a_pStatement;
}

byte* InterpreterContext::addressOfThis() const
{
    if (byte* pBasePointer = m_Interpreter.getBasePointer())
        return pBasePointer;
    PHANTOM_ASSERT(m_Interpreter.getDefaultThisPointer());
    return (byte*)m_Interpreter.getDefaultThisPointerAddress();
}

byte* InterpreterContext::addressOf(LocalVariable* a_pLocalVariable) const
{
    byte* pBasePointer = m_Interpreter.getBasePointer();
    PHANTOM_ASSERT(pBasePointer);
    return pBasePointer + a_pLocalVariable->getEvalFrameOffset();
}

} // namespace lang
} // namespace phantom
