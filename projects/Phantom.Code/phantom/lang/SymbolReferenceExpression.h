// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "Expression.h"

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM_CODE SymbolReferenceExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    template<class T>
    static Expression* Create(Semantic* a_pSemantic, LanguageElement* a_pOwner, T* a_pSymbol)
    {
        return Create(a_pSemantic, a_pOwner, a_pSymbol, PHANTOM_CLASSOF(T));
    }
    static Expression* Create(Semantic* a_pSemantic, LanguageElement* a_pOwner, Symbol* a_pSymbol,
                              Class* a_pCastSymbolClass);

public:
    SymbolReferenceExpression(Symbol* a_pSymbol);

    void initialize()
    {
        Expression::initialize();
        addReferencedElement(m_pSymbol);
    }

    Symbol* getSymbol() const { return m_pSymbol; }

    virtual void eval(ExecutionContext& a_Context) const;

protected:
    virtual SymbolReferenceExpression* cloneImpl(LanguageElement* a_pOwner) const;

private:
    Symbol* m_pSymbol;
};
} // namespace lang
} // namespace phantom
