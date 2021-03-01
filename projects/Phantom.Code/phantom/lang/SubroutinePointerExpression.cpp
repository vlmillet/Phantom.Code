// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "SubroutinePointerExpression.h"

#include <phantom/lang/FunctionPointer.h>
#include <phantom/lang/Method.h>
#include <phantom/lang/MethodPointer.h>
#include <phantom/lang/Source.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
static Type* DeduceType(Source* a_pSource, Subroutine* a_pSubroutine)
{
    // TODO : implement a makePointer(Source*) in Subroutine to make this cleaner
    if (Method* pMethod = a_pSubroutine->asMethod())
    {
        return a_pSource->methodPointerType(pMethod->getOwnerClassType(), a_pSubroutine->getReturnType(),
                                            a_pSubroutine->getParameterTypes());
    }
    else if (Function* pFunction = a_pSubroutine->asFunction())
    {
        return a_pSource->functionPointerType(a_pSubroutine->getReturnType(), ABI::CDecl,
                                              a_pSubroutine->getParameterTypes());
    }
    else
    {
        PHANTOM_ASSERT(false, "unknown subroutine type");
    }
    return PHANTOM_TYPEOF(void*);
}

SubroutinePointerExpression::SubroutinePointerExpression(Source* a_pSource, Subroutine* a_pSubroutine)
    : Expression(DeduceType(a_pSource, a_pSubroutine)), m_pSubroutine(a_pSubroutine)
{
}

SubroutinePointerExpression* SubroutinePointerExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<SubroutinePointerExpression>(getValueType()->getSource(), m_pSubroutine);
}

} // namespace lang
} // namespace phantom
