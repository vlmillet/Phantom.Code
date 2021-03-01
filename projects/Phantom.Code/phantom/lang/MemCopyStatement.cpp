// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "MemCopyStatement.h"

#include "Expression.h"
#include "phantom/lang/Type.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
MemCopyStatement::MemCopyStatement(Expression* a_pDst, Expression* a_pSrc, size_t a_uiSize)
    : m_pDst(a_pDst), m_pSrc(a_pSrc), m_uiSize(a_uiSize)
{
    PHANTOM_ASSERT(a_pDst);
    PHANTOM_ASSERT(a_pSrc);
    PHANTOM_ASSERT(a_uiSize);
    PHANTOM_ASSERT((a_pDst->getValueType()->removeConstReference()->asPointer() &&
                    a_pSrc->getValueType()->removeConstReference()->asPointer()) ||
                   isTemplateDependant());
}

void MemCopyStatement::eval(ExecutionContext& a_Context) const
{
    void* pDstAddress;
    void* pSrcAddress;
    m_pDst->load(a_Context, &pDstAddress);
    m_pSrc->load(a_Context, &pSrcAddress);
    memcpy(pDstAddress, pSrcAddress, m_uiSize);
    Statement::eval(a_Context);
}

void MemCopyStatement::flush(ExecutionContext& a_Context) const
{
    m_pSrc->flush(a_Context);
    m_pDst->flush(a_Context);
}

} // namespace lang
} // namespace phantom
