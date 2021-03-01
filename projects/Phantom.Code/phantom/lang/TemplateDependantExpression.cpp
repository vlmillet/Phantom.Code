// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "TemplateDependantExpression.h"

#include <phantom/lang/TemplateDependantElement.h>
#include <phantom/lang/TemplateDependantType.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
TemplateDependantExpression::TemplateDependantExpression(TemplateDependantElement* a_pElement)
    : Expression(a_pElement->New<TemplateDependantType>(), nullptr, PHANTOM_R_FLAG_TEMPLATE_DEPENDANT),
      m_pElement(a_pElement)
{
}

TemplateDependantExpression* TemplateDependantExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    TemplateDependantElement* pNewElem = a_pOwner->New<TemplateDependantElement>(
    m_pElement->getLeft() ? m_pElement->getLeft()->asExpression()
    ? m_pElement->getLeft()->asExpression()->clone(a_pOwner)
    : m_pElement->getLeft()
                          : nullptr,
    m_pElement->getName(),
    m_pElement->getTemplateArguments() ? MakeOptionalArrayView(&*m_pElement->getTemplateArguments()) : NullOpt,
    m_pElement->getFunctionArguments() ? MakeOptionalArrayView(&*m_pElement->getFunctionArguments()) : NullOpt,
    m_pElement->getModifiers(), m_pElement->getFlags());
    pNewElem->setCodeRange(m_pElement->getCodeRange());
    return a_pOwner->New<TemplateDependantExpression>(pNewElem);
}

} // namespace lang
} // namespace phantom
