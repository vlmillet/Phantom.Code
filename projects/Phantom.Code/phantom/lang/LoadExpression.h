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
/// \brief  Represent a 'load' instruction (opposite to 'store').
class PHANTOM_EXPORT_PHANTOM_CODE LoadExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    LoadExpression(Expression* a_pLoadedExpression);

    void         initialize();
    virtual void eval(ExecutionContext& a_Context) const;

    virtual LoadExpression* asLoadExpression() const { return const_cast<LoadExpression*>(this); }

    virtual void flush(ExecutionContext& a_Context) const { m_pLoadedExpression->flush(a_Context); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  The expression to 'load'.
    ///
    /// \return null if it fails, else the expression to 'load'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getLoadedExpression() const { return m_pLoadedExpression; }

    virtual LoadExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    virtual bool hasStorage() const { return true; }

    virtual void* evalStorage(ExecutionContext& a_Context) const;

    virtual void* getStorage() const;

protected:
    Expression*   m_pLoadedExpression;
    mutable void* m_pArrayAddressingHack;
};

} // namespace lang
} // namespace phantom
