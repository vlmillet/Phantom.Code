// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "BuiltInConversionExpression.h"

#include "conversions.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
BuiltInConversionExpression::BuiltInConversionExpression(Expression* a_pInputExpression, Conversion* a_pConversion,
                                                         CastKind a_eConversionType)
    : Expression(a_pConversion ? a_pConversion->output.type : nullptr),
      m_pInputExpression(a_pInputExpression),
      m_pConversion(a_pConversion),
      m_eConversionType(a_eConversionType)
{
    PHANTOM_ASSERT(m_pInputExpression);
    PHANTOM_ASSERT(m_pInputExpression->getValueType()->removeQualifiers()->asClassType() == nullptr ||
                   m_pInputExpression->isTemplateDependant() || a_pConversion->output.type->isTemplateDependant());
    PHANTOM_ASSERT(a_pConversion->output.type->removeQualifiers()->asClassType() == nullptr ||
                   m_pInputExpression->isTemplateDependant() || a_pConversion->output.type->isTemplateDependant());
}

BuiltInConversionExpression::~BuiltInConversionExpression()
{
    deleteVirtual(m_pConversion);
}

void BuiltInConversionExpression::eval(ExecutionContext& a_Context) const
{
    m_pConversion->apply(m_pInputExpression->loadEffectiveAddress(a_Context), a_Context.resultPointer());
}

BuiltInConversionExpression* BuiltInConversionExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<BuiltInConversionExpression>(
    m_pInputExpression, m_pConversion ? m_pConversion->clone(a_pOwner) : nullptr, m_eConversionType);
}

LanguageElement* BuiltInConversionExpression::removeExpression() const
{
    return m_pInputExpression->removeExpression();
}

} // namespace lang
} // namespace phantom
