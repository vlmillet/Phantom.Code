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
/// \brief  Rerpesents a member function pointer call expression.
class PHANTOM_EXPORT_PHANTOM_CODE MethodPointerCallExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    MethodPointerCallExpression(Expression* a_pObjectExpression, Expression* a_pMemberExpression,
                                ExpressionsView a_Arguments);

    void initialize();

    void terminate() override;

    virtual void eval(ExecutionContext& a_Context) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the caller object expression.
    ///
    /// \return The object expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getObjectExpression() const { return m_pObjectExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the member function expression.
    ///
    /// \return The member function expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getMemberExpression() const { return m_pMemberExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the member function pointer type.
    ///
    /// \return Tthe member function pointer type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MethodPointer* getMethodPointer() const { return m_pMethodPointer; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the arguments.
    ///
    /// \return The arguments.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionsView getArguments() const { return m_Arguments; }

    virtual MethodPointerCallExpression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    virtual void flush(ExecutionContext& a_Context) const override;

private:
    void _destroyReturnStorage();

private:
    MethodPointer* m_pMethodPointer;
    Expression*    m_pObjectExpression;
    Expression*    m_pMemberExpression;
    Expressions    m_Arguments;
    void*          m_pReturnStorage;
};

} // namespace lang
} // namespace phantom
