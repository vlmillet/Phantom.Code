#include "ParameterPackExpressionExpansion.h"
#include "TemplateParameterPackTypeExpansion.h"

#include <phantom/lang/Application.h>

namespace phantom
{
namespace lang
{
ParameterPackExpressionExpansion::ParameterPackExpressionExpansion(Expression* a_pExpandedElement,
                                                                   Parameter*  a_pExpandedParameter)
    : Expression(Application::Get()->getAuto()),
      m_pExpandedExpression(a_pExpandedElement),
      m_pExpandedParameter(a_pExpandedParameter)
{
    PHANTOM_ASSERT(Object::Cast<TemplateParameterPackTypeExpansion>(a_pExpandedParameter->getValueType()));
}

ParameterPackExpressionExpansion* ParameterPackExpressionExpansion::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<ParameterPackExpressionExpansion>(m_pExpandedExpression, m_pExpandedParameter);
}

} // namespace lang
} // namespace phantom
