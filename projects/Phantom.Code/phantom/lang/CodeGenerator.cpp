// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "CodeGenerator.h"

#include "DebugInformation.h"
#include "Message.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
static String emptyOption;

CodeGenerator::CodeGenerator(int a_iPassCount) : m_pModule(nullptr), m_uiPassCount(a_iPassCount) {}

CodeGenerator::~CodeGenerator(void)
{
    PHANTOM_ASSERT(m_pModule == nullptr);
}

StringView CodeGenerator::getOption(StringView a_Key) const
{
    auto found = m_Options.find(a_Key);
    return found == m_Options.end() ? emptyOption : found->second;
}

bool CodeGenerator::getOption(StringView _option, StringView& _value) const
{
    auto found = m_Options.find(_option);
    if (found != m_Options.end())
    {
        _value = found->second;
        return true;
    }
    return false;
}

void CodeGenerator::outdate()
{
    if (m_bOutdated)
        return;
    m_bOutdated = true;
    onOutdated();
}

DebugInformation* CodeGenerator::createDebugInformation()
{
    return new_<DebugInformation>();
}

void CodeGenerator::destroyDebugInformation(DebugInformation* a_pDebugInformationHandler)
{
    delete_<DebugInformation>(a_pDebugInformationHandler);
}

uint CodeGenerator::getPassCount() const
{
    return m_uiPassCount;
}

bool CodeGenerator::invoke(Method* a_pMethod, void* a_pObject, void** a_ppArgs, void* a_pRet) const
{
    ExecutionContext*     context = createExecutionContext();
    size_t                argCount = a_pMethod->getSignature()->getParameters().size() + 1;
    SmallVector<void*, 7> newArgs;
    newArgs.resize(argCount);
    newArgs[0] = &a_pObject;
    memcpy(&newArgs[1], a_ppArgs, (argCount - 1) * sizeof(void*));
    if (a_pRet)
        context->pushResultPointer(a_pRet);
    bool result = context->call(a_pMethod, newArgs.data(), argCount);
    if (a_pRet)
        context->popResultPointer();
    destroyExecutionContext(context);
    return result;
}

bool CodeGenerator::call(Subroutine* a_pSubroutine, void** a_ppArgs, void* a_pRet) const
{
    ExecutionContext* context = createExecutionContext();
    size_t argCount = a_pSubroutine->getSignature()->getParameters().size() + a_pSubroutine->asMethod() != nullptr;
    if (a_pRet)
        context->pushResultPointer(a_pRet);
    bool result = context->call(a_pSubroutine, a_ppArgs, argCount);
    if (a_pRet)
        context->popResultPointer();
    destroyExecutionContext(context);
    return result;
}

} // namespace lang
} // namespace phantom
