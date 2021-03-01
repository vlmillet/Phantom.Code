// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "StructureExpression.h"

#include "phantom/lang/Field.h"
#include "phantom/lang/Structure.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
StructureExpression::StructureExpression(Structure* a_pType, const Fields& a_Fields, ExpressionsView a_Values)
    : Expression(a_pType), m_Values(a_Values), m_Fields(a_Fields)
{
    PHANTOM_ASSERT(a_Fields.size() == a_Values.size());
    for (size_t i = 0; i < m_Values.size(); i++)
    {
        PHANTOM_ASSERT(a_Fields[i]);
        PHANTOM_ASSERT(a_Values[i]);
        PHANTOM_ASSERT(a_Values[i]->hasEvalConversionTo(a_Fields[i]->getValueType()));
    }
}

StructureExpression::~StructureExpression() {}

void StructureExpression::eval(ExecutionContext& a_Context) const
{
    byte* pDest = (byte*)a_Context.resultPointer();
    for (size_t i = 0; i < m_Values.size(); i++)
    {
        m_Values[i]->eval(a_Context, pDest + m_Fields[i]->getOffset());
    }
    flush(a_Context);
}

StructureExpression* StructureExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<StructureExpression>(static_cast<Structure*>(getValueType()), m_Fields, m_Values);
}

} // namespace lang
} // namespace phantom
