#include "TemplateParameterPackExpressionExpansion.h"

#include "TemplateParameterPackExpansion.h"

#include <phantom/lang/Placeholder.h>
#include <phantom/lang/TemplateDependantType.h>

namespace phantom
{
namespace lang
{
TemplateParameterPackExpressionExpansion::TemplateParameterPackExpressionExpansion(
TemplateParameterPackExpansion* a_pExpansion)
    : Expression(a_pExpansion->New<TemplateDependantType>()), m_pExpansion(a_pExpansion)
{
}

Placeholder* TemplateParameterPackExpressionExpansion::getPackArgument() const
{
    return m_pExpansion->getPackArgument();
}

Expression* TemplateParameterPackExpressionExpansion::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<TemplateParameterPackExpressionExpansion>(m_pExpansion);
}

} // namespace lang
} // namespace phantom
