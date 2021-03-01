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
/// \brief  Represents a 'memcpy' statement.
class PHANTOM_EXPORT_PHANTOM_CODE MemCopyStatement : public Statement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a 'memcpy' statement from a destination address expression, a source
    ///         address expression and a size in bytes.
    ///
    /// \param [in,out] a_pDstAddressExpression The destination address expression.
    /// \param [in,out] a_pSrcAddressExpression The source address expression.
    /// \param  a_uiSize                        The copy size (in bytes).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemCopyStatement(Expression* a_pDstAddressExpression, Expression* a_pSrcAddressExpression, size_t a_uiSize);

    void initialize()
    {
        Statement::initialize();
        addSubExpression(m_pDst);
        addSubExpression(m_pSrc);
    }

    virtual MemCopyStatement* asMemCopyStatement() const { return (MemCopyStatement*)this; }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext& a_Context) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the destination address expression.
    ///
    /// \return The destination address expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getDestinationAddressExpression() const { return m_pDst; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the source address expression.
    ///
    /// \return The source address expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getSourceAddressExpression() const { return m_pSrc; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the copy size in bytes.
    ///
    /// \return The copy size in bytes.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getSize() const { return m_uiSize; }

protected:
    Expression* m_pDst;
    Expression* m_pSrc;
    size_t      m_uiSize;
    byte        m_uValue;
};

} // namespace lang
} // namespace phantom
