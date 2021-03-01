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
/// \brief  Represents a subroutine call expression.
class PHANTOM_EXPORT_PHANTOM_CODE CallExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a call expression with a subroutine, an argument list and an optional
    /// special return type (which differs from subroutine return type).
    ///
    /// \param [in] a_pSubroutine The subroutine.
    /// \param [in] a_Arguments         The arguments.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CallExpression(Subroutine* a_pSubroutine, ExpressionsView a_Arguments);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a call expression with a subroutine and a single argument.
    ///
    /// \param [in] a_pSubroutine   The subroutine.
    /// \param [in] a_pArgument     The argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CallExpression(Subroutine* a_pSubroutine, Expression* a_pArgument);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a call expression with a subroutine and no argument.
    ///
    /// \param [in] a_pSubroutine   The subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CallExpression(Subroutine* a_pSubroutine);

    void initialize();
    void terminate() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the arguments of this call.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionsView getArguments() const { return m_Arguments; }

    bool transformArgument(Semantic* a_pSemantic, size_t i, Functor<Expression*(Expression*)> a_Transform);

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext& a_Context) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the subroutine associated with this call.
    ///
    /// \return The subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* getSubroutine() const { return m_pSubroutine; }

    /// \brief  Sets this call as 'final', i.e. forces non-virtual call.
    void setFinal();

    /// \brief  Query if this call is final', i.e. forces non-virtual call.
    bool isFinal() const;

    virtual LanguageElement* hatchExpression();

    virtual Expression* cloneImpl(LanguageElement* a_pOwner) const override;

    virtual LanguageElement* removeExpression() const override;

    virtual bool isPersistent() const;

    virtual void dump(std::ostream& a_Out, int a_Lvl) const override;

private:
    Subroutine* m_pSubroutine = nullptr;
    Expressions m_Arguments;
};

} // namespace lang
} // namespace phantom
