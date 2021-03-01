// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "SymbolReferenceExpression.h"

#include "Semantic.h"

namespace phantom
{
namespace lang
{
Expression* SymbolReferenceExpression::Create(Semantic* a_pSemantic, LanguageElement* a_pOwner, Symbol* a_pSymbol,
                                              Class* a_pCastSymbolClass)
{
    SymbolReferenceExpression* pExp = a_pOwner->New<SymbolReferenceExpression>(a_pSymbol);
    return a_pSemantic->convert(pExp, a_pCastSymbolClass->addPointer(), CastKind::Static);
}

SymbolReferenceExpression::SymbolReferenceExpression(Symbol* a_pSymbol)
    : Expression(PHANTOM_CLASSOF(Symbol)->addPointer()), m_pSymbol(a_pSymbol)
{
}

void SymbolReferenceExpression::eval(ExecutionContext& a_Context) const
{
    if (void* pAddr = a_Context.resultPointer())
    {
        *(Symbol**)pAddr = m_pSymbol;
    }
}

SymbolReferenceExpression* SymbolReferenceExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<SymbolReferenceExpression>(m_pSymbol);
}

} // namespace lang
} // namespace phantom
