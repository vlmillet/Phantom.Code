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
/// \brief  Represents a data member initialization statement
///         \code{.cpp}
///                     MyClass(int valueA) : a(valueA) {} // data member 'a' initialized with
///                     'valueA'
///         \endcode

class PHANTOM_EXPORT_PHANTOM_CODE VirtualMethodTableSetupStatement : public Statement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor.
    ///
    /// \param [in,out] a_pTable    The virtual member function table to setup.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VirtualMethodTableSetupStatement(VirtualMethodTable* a_pTable);

    void initialize();

    virtual VirtualMethodTableSetupStatement* asVirtualMethodTableSetupStatement() const
    {
        return (VirtualMethodTableSetupStatement*)this;
    }

    virtual void eval(ExecutionContext& a_Context) const;

    virtual void flush(ExecutionContext&) const {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the table associated with this statement.
    ///
    /// \return The constructed element or null.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VirtualMethodTable* getTable() const;

protected:
    VirtualMethodTable* m_pTable;
};

} // namespace lang
} // namespace phantom
