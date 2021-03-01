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
/// \brief  Represents a placement construction expression (ex: new (pMyPlace) MyClass(...)) .
class PHANTOM_EXPORT_PHANTOM_CODE PlacementNewExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a placement construction expression with a default constructor and a
    /// place expression.
    ///
    /// \param  a_pPlaceExpresion           The place expression.
    /// \param  a_pConstructionExpression   The construction expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PlacementNewExpression(Expression* a_pPlaceExpresion, Expression* a_pConstructionExpression);

    /// \brief  Destructor.
    void initialize()
    {
        Expression::initialize();
        addSubExpression(m_pConstructionExpression);
        m_pConstructionExpression->setTemporary(false);
        addSubExpression(m_pPlaceExpression);
    }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual PlacementNewExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the place expression which evaluates to an address where to do the placement
    /// construction.
    ///
    /// \return The place expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getPlaceExpression() const { return m_pPlaceExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constructor associated with this expression.
    ///
    /// \return The constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getConstructionExpression() const { return m_pConstructionExpression; }

protected:
    Expression* m_pPlaceExpression;
    Expression* m_pConstructionExpression;
};

} // namespace lang
} // namespace phantom
