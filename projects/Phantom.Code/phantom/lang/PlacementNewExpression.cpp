// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "PlacementNewExpression.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
PlacementNewExpression::PlacementNewExpression(Expression* a_pPlaceExpresion, Expression* a_pConstructionExpression)
    : Expression(a_pConstructionExpression->getValueType()->addPointer()),
      m_pPlaceExpression(a_pPlaceExpresion),
      m_pConstructionExpression(a_pConstructionExpression)
{
    PHANTOM_ASSERT(m_pPlaceExpression);
    PHANTOM_ASSERT(m_pPlaceExpression->isTemplateDependant() || m_pPlaceExpression->getValueType()->asPointer());
    PHANTOM_ASSERT(
    isTemplateDependant() ||
    m_pConstructionExpression->getValueType()->isSame(m_pPlaceExpression->getValueType()->removePointer()));
}

void PlacementNewExpression::eval(ExecutionContext& a_Context) const
{
    void* pPlace;
    m_pPlaceExpression->load(a_Context, &pPlace);
    m_pConstructionExpression->eval(a_Context, pPlace);
    if (a_Context.resultPointer())
    {
        *((void**)a_Context.resultPointer()) = pPlace;
    }
}

PlacementNewExpression* PlacementNewExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<PlacementNewExpression>(m_pPlaceExpression, m_pConstructionExpression);
}

} // namespace lang
} // namespace phantom
