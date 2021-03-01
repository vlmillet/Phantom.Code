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
/// \brief  Represents a new expression (ex: New<MyClass>(...)) .
class PHANTOM_EXPORT_PHANTOM_CODE AllocateExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an allocate expression with a given size expression and if it is on stack
    /// or heap.
    ///
    /// \param  a_pType                 The allocated type.
    /// \param  a_pSizeExpression       The size to allocate.
    /// \param  a_bStack                Allocate on stack ? (else on heap)
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    AllocateExpression(Type* a_pType, bool a_bOnStack = false);
    AllocateExpression(Type* a_pType, Expression* a_pSizeExpression, bool a_bOnStack = false);

    void initialize();

    virtual void eval(ExecutionContext& a_Context) const;

    virtual AllocateExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the size expression associated with this expression.
    ///
    /// \return The size expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getSizeExpression() const { return m_pSizeExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the allocated type.
    ///
    /// \return The allocated type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getType() const { return m_pType; }

    bool isOnStack() const { return m_bStack; }

protected:
    Type*       m_pType;
    Expression* m_pSizeExpression;
    bool        m_bStack;
};

} // namespace lang
} // namespace phantom
