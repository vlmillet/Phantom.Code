// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "BuiltInOperator.h"
#include "Expression.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class BuiltInOperator;

/// \brief  Represents a built-in operator operation.
class PHANTOM_EXPORT_PHANTOM_CODE BuiltInOperatorExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor.
    ///
    /// \param [in] a_pOperator     The built in operator.
    /// \param [in] a_pResultType   The built in operation result type.
    /// \param [in] a_Arguments     The built-in operation arguments.
    /// \param  a_Delegate          The native operation delegate.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    BuiltInOperatorExpression(BuiltInOperator* a_pOperator, Type* a_pResultType, Expression* const* a_InputArguments,
                              Expression* const* a_EvalArguments, BuiltInOperatorDelegate a_Delegate);

    BuiltInOperatorExpression(BuiltInOperator* a_pOperator, Type* a_pResultType, Expression* const* a_InputArguments);

    void initialize();

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext& a_Context) const;

    virtual BuiltInOperatorExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the built-in operator associated with this built-in operation.
    ///
    /// \return null if it fails, else the operator.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    BuiltInOperator* getOperator() const { return m_pOperator; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the argument converted for interpreter evaluation at given index position.
    ///
    /// \param  i   Zero-based index of the argument.
    ///
    /// \return The argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getEvalArgument(size_t i) const { return m_EvalArguments[i]; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the original operator input argument at given index position.
    ///
    /// \param  i   Zero-based index of the argument.
    ///
    /// \return The argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getInputArgument(size_t i) const { return m_InputArguments[i]; }

    virtual bool hasStorage() const { return true; }

    virtual void* evalStorage(ExecutionContext& a_Context) const;

    virtual Expression* dereference(LanguageElement* a_pOwner) override;

    virtual Expression* address(LanguageElement* a_pOwner) override;

    virtual LanguageElement* removeExpression() const override;

    virtual void dump(std::ostream& a_Out, int a_Lvl) const override;

protected:
    Evaluable* evaluateExpressionLifeTime(Expression* a_pExpression) const override;

protected:
    BuiltInOperator*        m_pOperator{};
    Expression*             m_EvalArguments[3] = {nullptr, nullptr, nullptr};
    Expression*             m_InputArguments[3] = {nullptr, nullptr, nullptr};
    BuiltInOperatorDelegate m_Delegate;
    mutable byte            m_Buffer[16]; /// big enough to receive any fundamental type
    bool                    m_bRequestFlush{};
};

} // namespace lang
} // namespace phantom
