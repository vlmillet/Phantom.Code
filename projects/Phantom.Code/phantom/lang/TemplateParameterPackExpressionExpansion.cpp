#include "TemplateParameterPackExpressionExpansion.h"

#include "TemplateParameterPackTypeExpansion.h"

#include <phantom/lang/Application.h>

namespace phantom
{
namespace lang
{
TemplateParameterPackExpressionExpansion::TemplateParameterPackExpressionExpansion(Expression* a_pExpandedElement,
                                                                                   Parameter*  a_pExpandedParameter)
    : Expression(Application::Get()->getAuto()),
      m_pExpandedExpression(a_pExpandedElement),
      m_pExpandedParameter(a_pExpandedParameter)
{
    PHANTOM_ASSERT(Object::Cast<TemplateParameterPackTypeExpansion>(a_pExpandedParameter->getValueType()));
}

TemplateParameterPackExpressionExpansion*
TemplateParameterPackExpressionExpansion::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<TemplateParameterPackExpressionExpansion>(m_pExpandedExpression, m_pExpandedParameter);
}

} // namespace lang
} // namespace phantom
