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
class PHANTOM_EXPORT_PHANTOM_CODE TypeTraitExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    enum ETypeTrait
    {
        e_TypeTait_Undefined,
        e_TypeTait_IsSame,
    };

public:
    TypeTraitExpression(ETypeTrait a_eTypeTrait, Types const& a_Types);
    PHANTOM_DTOR ~TypeTraitExpression(void) {}

    virtual TypeTraitExpression* asTypeTraitExpression() const { return (TypeTraitExpression*)this; }

    ETypeTrait getTypeTrait() const { return m_eTypeTrait; }

    static bool Resolve(ETypeTrait a_eTypeTrait, Types const& a_Types);

    Types const& getTypes() const { return m_Types; }

    virtual void eval(ExecutionContext& a_Context) const override;

    virtual bool isCompileTime() const override { return true; }

protected:
    virtual Expression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    ETypeTrait m_eTypeTrait = e_TypeTait_Undefined;
    Types      m_Types;
};

} // namespace lang
} // namespace phantom
