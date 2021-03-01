// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Statement.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM_CODE MemSetStatement : public Statement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a 'memset' statement from an address expression, a value and a size in
    ///         bytes.
    ///
    /// \param [in,out] a_pAddressExpression    The address expression.
    /// \param  a_uValue                        The init value.
    /// \param  a_uiSize                        The init size (in bytes).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemSetStatement(Expression* a_pAddressExpression, byte a_uValue, size_t a_uiSize);

    void initialize()
    {
        Statement::initialize();
        addSubExpression(m_pAddr);
    }

    virtual MemSetStatement* asMemSetStatement() const { return (MemSetStatement*)this; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext& a_Context) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the address expression.
    ///
    /// \return The address expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getAddressExpression() const { return m_pAddr; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the init size in bytes.
    ///
    /// \return The init size.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getSize() const { return m_uiSize; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the init value.
    ///
    /// \return The init value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    byte getValue() const { return m_uValue; }

protected:
    Expression* m_pAddr;
    size_t      m_uiSize;
    byte        m_uValue;
};

} // namespace lang
} // namespace phantom
