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
/// \brief  Represents an array expression (ex: { 0, 2, 5, anInt } ) .
class PHANTOM_EXPORT_PHANTOM_CODE StructureExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a structure expression with a structure, fields and field initialization
    /// expressions.
    ///
    /// \param  a_pStructureType    The structure type matching the given member values.
    /// \param  a_Fields            The fields to initialize
    /// \param  a_Values            The values of the new array
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StructureExpression(Structure* a_pStructureType, const Fields& a_Fields, ExpressionsView a_Values);

    /// \brief  Destructor.
    ~StructureExpression();

    virtual StructureExpression* asStructureExpression() const { return (StructureExpression*)this; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual StructureExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the initialized fields.
    ///
    /// \return The initialized fields.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Fields& getFields() const { return m_Fields; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the field initialization values.
    ///
    /// \return The field initialization values.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExpressionsView getValues() const { return m_Values; }

protected:
    Fields      m_Fields;
    Expressions m_Values;
};

} // namespace lang
} // namespace phantom
