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
/// \brief  Represents a return statement.
/// \code{.cpp}
///     ex: return 0.f / return;
/// \endcode
class PHANTOM_EXPORT_PHANTOM_CODE ControlStatement : public Statement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

    friend class Block;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator pointing to the first RAII destruction statement.
    ///
    /// \return An iterator.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expressions const& getRAIIDestructions() const
    {
        const_cast<ControlStatement*>(this)->stripRAIIDestructions();
        return m_RAIIDestructions;
    }

    virtual Statement* getTargetStatement() const = 0;

protected:
    void eval(ExecutionContext& a_Context) const override;
    void finish();
    void stripRAIIDestructions();

protected:
    Expressions m_RAIIDestructions;
};

} // namespace lang
} // namespace phantom
