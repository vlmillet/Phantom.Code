// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "LValueExpression.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents an access to a Data member. (ex: object.data_member)
class PHANTOM_EXPORT_PHANTOM_CODE FieldExpression : public LValueExpression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a Data member expression from an object reference expression and a Data
    /// member.
    ///
    /// \param [in,out] a_pObjectExpression The object expression.
    /// \param [in,out] a_pField       The Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FieldExpression(Expression* a_pObjectExpression, Field* a_pField);

    void initialize();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the object expression associated with this expression.
    ///
    /// \return The object expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getObjectExpression() const { return m_pObjectExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member associated with this expression.
    ///
    /// \return The Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* getField() const { return m_pField; }

    virtual void flush(ExecutionContext& a_Context) const { m_pObjectExpression->flush(a_Context); }

    virtual LanguageElement* removeExpression() const override;

    virtual bool isPersistent() const;

    virtual FieldExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    virtual void* internalEvalAddress(ExecutionContext& a_Context) const;

protected:
    Expression* m_pObjectExpression;
    Field*      m_pField;
};

} // namespace lang
} // namespace phantom
