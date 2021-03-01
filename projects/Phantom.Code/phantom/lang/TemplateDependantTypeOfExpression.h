// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Expression.h"
/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM_CODE TemplateDependantTypeOfExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    TemplateDependantTypeOfExpression(Type* a_pType, Type* a_pMetaTypePtrCast);
    PHANTOM_DTOR ~TemplateDependantTypeOfExpression(void) {}

    virtual TemplateDependantTypeOfExpression* asTemplateDependantTypeOfExpression() const
    {
        return (TemplateDependantTypeOfExpression*)this;
    }

    Type* getType() const { return m_pType; }

    virtual void eval(ExecutionContext& a_Context) const override;

    virtual bool isCompileTime() const override { return true; }

protected:
    virtual Expression* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    Type* m_pType;
};

} // namespace lang
} // namespace phantom
