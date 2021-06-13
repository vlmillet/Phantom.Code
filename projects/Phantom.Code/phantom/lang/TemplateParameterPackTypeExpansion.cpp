#include "TemplateParameterPackTypeExpansion.h"

#include "TemplateParameterPackExpansion.h"

#include <phantom/lang/PlaceholderType.h>

namespace phantom
{
namespace lang
{
TemplateParameterPackTypeExpansion::TemplateParameterPackTypeExpansion(TemplateParameterPackExpansion* a_pExpansion)
    : Type(TypeKind::Unknown, a_pExpansion->getPackArgument()->asSymbol()->getName(), 0, 0,
           a_pExpansion->getPackArgument()->asSymbol()->getModifiers(), PHANTOM_R_FLAG_TEMPLATE_DEPENDANT),
      m_pExpansion(a_pExpansion)
{
    PHANTOM_ASSERT(m_pExpansion->getPackArgument()->asPlaceholderType());
}

phantom::lang::PlaceholderType* TemplateParameterPackTypeExpansion::getPackArgument() const
{
    return static_cast<PlaceholderType*>(m_pExpansion->getPackArgument());
}

} // namespace lang
} // namespace phantom
