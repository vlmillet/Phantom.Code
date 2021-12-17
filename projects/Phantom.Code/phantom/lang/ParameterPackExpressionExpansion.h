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
class TemplateParameterPackExpansion;
class PHANTOM_EXPORT_PHANTOM_CODE ParameterPackExpressionExpansion : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ParameterPackExpressionExpansion(Expression* a_pExpandedExp, Parameter* a_pExpandedParam);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the expanded expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getExpandedExpression() const { return m_pExpandedExpression; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the expanded function parameter.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Parameter* getExpandedParameter() const { return m_pExpandedParameter; }

    void eval(ExecutionContext&) const override { PHANTOM_ASSERT(false, "Invalid pack expansion evaluation"); }

    void flush(ExecutionContext& a_Context) const override
    {
        PHANTOM_ASSERT(false, "Invalid pack expansion evaluation");
    }

    bool hasStorage() const override { return false; }

private:
    ParameterPackExpressionExpansion* cloneImpl(LanguageElement* a_pOwner) const override;

protected:
    Expression* m_pExpandedExpression{};
    Parameter*  m_pExpandedParameter{};
};

} // namespace lang
} // namespace phantom
