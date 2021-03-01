// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Expression.h"

/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM_CODE ConstantExpression : public Expression
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    template<class T>
    static Expression* Create(T a_Val);

public:
    ConstantExpression(Constant* a_pConstant);
    void initialize();
    void terminate() override;

    virtual void setValue(void const* a_pSrc) const;

    virtual void eval(ExecutionContext& a_Context) const;

    Constant* getConstant() const { return m_pConstant; }

    virtual bool hasStorage() const { return true; }

    void* evalStorage(ExecutionContext& a_Context) const override;

    virtual LanguageElement* hatchExpression();

    virtual LanguageElement*    removeExpression() const override;
    virtual ConstantExpression* cloneImpl(LanguageElement* a_pOwner) const override;

    virtual bool isPersistent() const;

    virtual bool isCompileTime() const override { return true; }

protected:
    Constant* m_pConstant;
    void*     m_pTempValue;
};

template<class T>
struct ConstantExpressionH
{
    static ConstantExpression* Create(T val) { return a_pOwner->New<ConstantExpression>(Constant::Create(val)); }
};

template<class T>
struct ConstantExpressionH<T*>
{
    static Expression* Create(T* val)
    {
        return (New<ConstantExpression>(Constant::Create(size_t(val))))
        ->convert(PHANTOM_TYPEOF(T*), CastKind::Reinterpret);
    }
};

template<class T>
Expression* ConstantExpression::Create(T a_Val)
{
    return ConstantExpressionH<T>::Create(a_Val);
}

} // namespace lang
} // namespace phantom
