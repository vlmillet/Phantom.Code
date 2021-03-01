// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "VarArgCallExpression.h"

#include <phantom/lang/LValueReference.h>
#include <phantom/lang/Pointer.h>
#include <phantom/lang/RValueReference.h>
#include <phantom/lang/Subroutine.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
VarArgCallExpression::VarArgCallExpression(Subroutine* a_pSubroutine, ExpressionsView a_Arguments)
    : CallExpression(a_pSubroutine, a_Arguments)
{
}

VarArgCallExpression::VarArgCallExpression(Subroutine* a_pSubroutine, Expression* a_pArgument)
    : VarArgCallExpression(a_pSubroutine, Expressions{a_pArgument})
{
}

VarArgCallExpression::VarArgCallExpression(Subroutine* a_pSubroutine)
    : VarArgCallExpression(a_pSubroutine, Expressions{})
{
}

VarArgCallExpression::~VarArgCallExpression() {}

void VarArgCallExpression::eval(ExecutionContext& a_Context) const
{
    Subroutine* pSubroutine = getSubroutine();
    Types       varArgTypes;
    auto        arguments = getArguments();
    for (size_t i = pSubroutine->getParameters().size(); i < getArguments().size(); ++i)
    {
        varArgTypes.push_back(arguments[i]->getValueType());
    }
    VoidPtrs addresses;
    evaluateArguments(a_Context, arguments, addresses);
    if (a_Context.resultPointer() != nullptr)
    {
        pSubroutine->placementCallVarArg(a_Context, addresses.empty() ? nullptr : (addresses.data()),
                                         varArgTypes); // first call
    }
    else // void or non provided result pointer
    {
        if (pSubroutine->getReturnType() != PHANTOM_TYPEOF(void))
        {
            auto  pType = pSubroutine->getReturnType()->removeReference()->removeQualifiers();
            void* temp = pType->allocate();
            a_Context.pushResultPointer(temp);
            pSubroutine->placementCallVarArg(a_Context, addresses.empty() ? nullptr : (addresses.data()), varArgTypes);
            a_Context.popResultPointer();
            pType->destroy(temp);
            pType->deallocate(temp);
        }
        else
        {
            pSubroutine->callVarArg(a_Context, addresses.empty() ? nullptr : (addresses.data()), varArgTypes);
        }
    }
    flush(a_Context);
}

Expression* VarArgCallExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    Expressions clonedExpressions;
    for (auto pArg : getArguments())
    {
        clonedExpressions.push_back(pArg->clone(a_pOwner));
    }
    return a_pOwner->New<VarArgCallExpression>(getSubroutine(), clonedExpressions);
}

} // namespace lang
} // namespace phantom
