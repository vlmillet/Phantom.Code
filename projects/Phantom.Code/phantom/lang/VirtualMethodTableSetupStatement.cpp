// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "VirtualMethodTableSetupStatement.h"

#include "Expression.h"

#include <phantom/lang/VirtualMethodTable.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
VirtualMethodTableSetupStatement::VirtualMethodTableSetupStatement(VirtualMethodTable* a_pTable) : m_pTable(a_pTable) {}

void VirtualMethodTableSetupStatement::initialize()
{
    Statement::initialize();
    addReferencedElement(m_pTable);
}

void VirtualMethodTableSetupStatement::eval(ExecutionContext& a_Context) const
{
    byte* _this = *(byte**)a_Context.addressOfThis();
    m_pTable->construct(_this);
}

VirtualMethodTable* VirtualMethodTableSetupStatement::getTable() const
{
    return m_pTable;
}

} // namespace lang
} // namespace phantom
