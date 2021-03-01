// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "Block.h"
#include "Compiler.h"
#include "DebugInformation.h"
#include "Debugger.h"

#include <phantom/lang/Application.h>
#include <phantom/lang/Source.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
BreakPoint* DebugInformation::addBreakPoint(const CodeLocation& a_Loc)
{
    void* pAddress = getAddressForCodeLocation(a_Loc);
    if (pAddress)
        return addBreakPoint(pAddress, a_Loc);
    return nullptr;
}

BreakPoint* DebugInformation::addBreakPoint(void* a_pAddress, const CodeLocation& a_CandidateLoc)
{
    BreakPoint* pNewBreakPoint =
    Compiler::Get()->getDebugger()->addBreakPoint(a_pAddress, a_CandidateLoc);
    return pNewBreakPoint;
}

void DebugInformation::removeBreakPoint(BreakPoint* a_pBreakPoint)
{
    Compiler::Get()->getDebugger()->removeBreakPoint(a_pBreakPoint);
}

phantom::lang::CodeLocation DebugInformation::getCodeLocationForAddress(void*) const
{
	return CodeLocation();
}

Subroutine* DebugInformation::getSubroutineAtAddress(byte* a_pAddress) const
{
    CodeLocation l = getCodeLocationForAddress(a_pAddress);
    if (l.isValid())
    {
        LanguageElement* pElem = l.source->getElementAtCodePosition(l.position);
        if (Block* pBlock = pElem->getEnclosingBlock())
        {
            return pBlock->getSubroutine();
        }
    }
    return nullptr;
}

phantom::byte* DebugInformation::getLocalVariableAddress(LocalVariable* , size_t ) const
{
	return nullptr;
}

void* DebugInformation::getAddressForCodeLocation(CodeLocation ) const
{
	return nullptr;
}

bool DebugInformation::findStepOverInstructions(byte* , byte* , byte*& , byte*& ) const
{
	return false;
}

bool DebugInformation::findStepIntoInstruction(byte* , byte*& , void* ) const
{
	return false;
}

} // namespace lang
} // namespace phantom
