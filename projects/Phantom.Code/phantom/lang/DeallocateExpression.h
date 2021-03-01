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
class PHANTOM_EXPORT_PHANTOM_CODE DeallocateExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a deallocate expression with a given type, size, and heap address
    /// expression.
    ///
    /// \param  a_pType                 The type to deallocate.
    /// \param  a_pSizeExpression       The number of contiguous instance to deallocate.
    /// \param  a_pAddressExpression    The address to deallocate.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    DeallocateExpression(Type* a_pType, Expression* a_pSizeExpression, Expression* a_pAddressExpression);
    DeallocateExpression(Type* a_pType, Expression* a_pAddressExpression);

    void initialize();

    virtual DeallocateExpression* asDeallocateExpression() const { return (DeallocateExpression*)this; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual DeallocateExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the address expression associated with this expression.
    ///
    /// \return The address expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getAddressExpression() const { return m_pAddressExpression; }
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

protected:
    Type*       m_pType;
    Expression* m_pSizeExpression;
    Expression* m_pAddressExpression;
};

} // namespace lang
} // namespace phantom
