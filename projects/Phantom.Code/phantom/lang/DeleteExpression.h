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
/// \brief  Represents a new expression (ex: delete ...) .
class PHANTOM_EXPORT_PHANTOM_CODE DeleteExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a static delete expression with given deleted class and
    /// expression.
    ///
    /// \param  a_pClass    The deleted class type.
    /// \param  a_pExpression   The expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    DeleteExpression(Class* a_pClass, Expression* a_pExpression);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a dynamic delete expression with given delete expression but without
    /// class.
    ///
    /// \param  a_pExpression   The expression.
    /// \param  a_bPhantom      Create an equivalent to phantom Delete(instead) of delete
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    DeleteExpression(Expression* a_pExpression, bool a_bPhantom = false);

    void initialize();

    virtual DeleteExpression* asDeleteExpression() const { return (DeleteExpression*)this; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual DeleteExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the deleted class type.
    ///
    /// \return The deleted class type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Class* getClass() const { return m_pClass; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the deleted expression.
    ///
    /// \return The deleted expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getExpression() const { return m_pExpression; }

protected:
    Class*      m_pInitClass;
    Class*      m_pClass;
    Expression* m_pExpression;
    bool        m_bPhantom;
};

} // namespace lang
} // namespace phantom
