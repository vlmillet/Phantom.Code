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
class PHANTOM_EXPORT_PHANTOM_CODE IdentityExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    IdentityExpression(Type* a_pValueType, Expression* a_pExpression);

    void initialize()
    {
        Expression::initialize();
        addSubExpression(m_pExpression);
    }

    virtual void eval(ExecutionContext& a_Context) const { m_pExpression->eval(a_Context); }

    virtual Expression* removeIdentity() const { return m_pExpression->removeIdentity(); }
    virtual Expression* removeImplicitIdentity() const { if (m_CastKind == CastKind::Implicit) return m_pExpression->removeIdentity(); return const_cast<IdentityExpression*>(this); }

    virtual LanguageElement* removeExpression() const override { return m_pExpression->removeExpression(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  The underlying expression wrapped in this one.
    ///
    /// \return The underlying expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getExpression() const { return m_pExpression; }

    virtual IdentityExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    virtual bool hasStorage() const { return m_pExpression->hasStorage(); }

    virtual void* evalStorage(ExecutionContext& a_Context) const
    {
        PHANTOM_ASSERT(m_pExpression);
        return m_pExpression->evalStorage(a_Context);
    }

    virtual void* getStorage() const
    {
        PHANTOM_ASSERT(m_pExpression);
        return m_pExpression->getStorage();
    }

    virtual void flush(ExecutionContext& a_Context) const { m_pExpression->flush(a_Context); }

    virtual Expression* removeConversions() const { return m_pExpression; }

    void     setConversionKind(CastKind a_CastKind) { m_CastKind = a_CastKind; }
    CastKind getConversionKind() const { return m_CastKind; }

protected:
    Expression* m_pExpression;
    CastKind    m_CastKind = CastKind::Implicit;
};

} // namespace lang
} // namespace phantom
