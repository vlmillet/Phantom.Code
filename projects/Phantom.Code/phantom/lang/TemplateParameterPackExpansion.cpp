#include "TemplateParameterPackExpansion.h"

namespace phantom
{
namespace lang
{
TemplateParameterPackExpansion::TemplateParameterPackExpansion(LanguageElement* a_pExpandedElement,
                                                               Placeholder*     a_pPackArgument)
    : m_pExpandedElement(a_pExpandedElement), m_pPackArgument(a_pPackArgument)
{
}

} // namespace lang
} // namespace phantom
