// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "MemSetStatement.h"

#include "Expression.h"
#include "phantom/lang/Type.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
MemSetStatement::MemSetStatement(Expression* a_pAddr, byte a_uValue, size_t a_uiSize)
    : m_pAddr(a_pAddr), m_uiSize(a_uiSize), m_uValue(a_uValue)
{
    PHANTOM_ASSERT(a_pAddr->getValueType()->removeConstReference()->asPointer() || isTemplateDependant());
}

void MemSetStatement::eval(ExecutionContext& a_Context) const
{
    void* pAddress;
    m_pAddr->load(a_Context, &pAddress);
    memset(pAddress, m_uValue, m_uiSize);
    Statement::eval(a_Context);
}

void MemSetStatement::flush(ExecutionContext& a_Context) const
{
    m_pAddr->flush(a_Context);
}

} // namespace lang
} // namespace phantom
