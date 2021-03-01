// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "TemplateDependantDeclType.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
TemplateDependantDeclType::TemplateDependantDeclType(Expression* a_pExpression, Type* a_pAutoType)
    : Type(TypeKind::Unknown, "", 0, 0, 0, PHANTOM_R_FLAG_TEMPLATE_DEPENDANT),
      m_pExpression(a_pExpression),
      m_pAutoType(a_pAutoType)
{
}

} // namespace lang
} // namespace phantom
