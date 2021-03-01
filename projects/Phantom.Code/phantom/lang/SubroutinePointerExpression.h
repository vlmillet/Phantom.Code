// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Expression.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM_CODE SubroutinePointerExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    SubroutinePointerExpression(Source* a_pSource, Subroutine* a_pSubroutine);

    void initialize()
    {
        Expression::initialize();
        addReferencedElement(m_pSubroutine);
    }

    virtual void eval(ExecutionContext& a_Context) const
    {
        if (a_Context.resultPointer())
            *(void**)a_Context.resultPointer() = m_pSubroutine;
    }

    virtual SubroutinePointerExpression* asSubroutinePointerExpression() const
    {
        return const_cast<SubroutinePointerExpression*>(this);
    }

    Subroutine* getSubroutine() const { return m_pSubroutine; }

protected:
    virtual SubroutinePointerExpression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    Subroutine* m_pSubroutine{};
};

} // namespace lang
} // namespace phantom
