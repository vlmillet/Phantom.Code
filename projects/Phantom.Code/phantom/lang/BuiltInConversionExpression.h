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
/// \brief  Represents a C++ built-in conversion. (see C++ standard).
class PHANTOM_EXPORT_PHANTOM_CODE BuiltInConversionExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor.
    ///
    /// \param [in,out] a_pInputExpression  If non-null, the input expression.
    /// \param [in,out] a_pConversion       If non-null, the conversion.
    /// \param  a_eConversionType           Type of the conversion (ex: reinterpret_cast,
    /// static_cast, ...).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    BuiltInConversionExpression(Expression* a_pInputExpression, Conversion* a_pConversion, CastKind a_eConversionType);
    ~BuiltInConversionExpression();

    void initialize()
    {
        Expression::initialize();
        addSubExpression(m_pInputExpression);
    }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext& a_Context) const { m_pInputExpression->flush(a_Context); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets input expression.
    ///
    /// \return null if it fails, else the input expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getInputExpression() const { return m_pInputExpression; }

    virtual BuiltInConversionExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the conversion.
    ///
    /// \return null if it fails, else the conversion.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Conversion* getConversion() const { return m_pConversion; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets conversion type.
    ///
    /// \return The conversion type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CastKind getConversionType() const { return m_eConversionType; }

    virtual Expression* removeConversions() const { return m_pInputExpression; }

    virtual LanguageElement* removeExpression() const override;

private:
    Expression* m_pInputExpression;
    Conversion* m_pConversion;
    CastKind    m_eConversionType;
};

} // namespace lang
} // namespace phantom
