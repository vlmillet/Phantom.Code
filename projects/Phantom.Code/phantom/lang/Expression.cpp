// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "Expression.h"

#include "BinaryOperationExpression.h"
#include "Block.h"
#include "BuiltInOperatorExpression.h"
#include "CallExpression.h"
#include "CompiledSource.h"
#include "ConstantExpression.h"
#include "CppLite.h"
#include "ExpressionStatement.h"
#include "IdentityExpression.h"
#include "Interpreter.h"
#include "Language.h"
#include "LoadExpression.h"
#include "PointerAdjustmentExpression.h"
#include "Semantic.h"
#include "TemporaryObjectDestructionExpression.h"
#include "UnaryPreOperationExpression.h"
#include "phantom/lang/Application.h"
#include "phantom/lang/Constant.h"
#include "phantom/lang/Destructor.h"
#include "phantom/lang/LValueReference.h"
#include "phantom/lang/Pointer.h"

#include <ostream>
/* *********************************************** */

namespace phantom
{
namespace lang
{
Expression::Expression(Type* a_pType, Type* a_pEvalType, uint a_uiFlags /*= 0*/)
    : Evaluable(a_pEvalType ? a_pEvalType : a_pType, a_uiFlags), m_pValueType(a_pType)
{
    PHANTOM_ASSERT(m_pValueType);
}

void Expression::load(void* a_pDest) const
{
    InterpreterContext context;
    load(context, a_pDest);
}

Expression* Expression::load(LanguageElement* a_pOwner)
{
    if (m_pValueType->asReference())
        return a_pOwner->New<LoadExpression>(const_cast<Expression*>(this));
    return const_cast<Expression*>(this);
}

void Expression::load(ExecutionContext& a_Context) const
{
    Reference* pReference = m_pValueType->asReference();
    if (pReference)
    {
        PHANTOM_ASSERT(pReference->getReferencedType()->isCopyable(), "non-copyable expression value cannot be loaded");
        void* pBuffer;
        eval(a_Context, &pBuffer);
        pReference->getReferencedType()->copy(a_Context.resultPointer(), pBuffer);
    }
    else
        eval(a_Context);
}

void Expression::setValue(void const*) const
{
    PHANTOM_ASSERT(false, "Expression is not an l-value");
}

void Expression::store(void const* a_pSrc) const
{
    if (m_pValueType->asConstLValueReference())
    {
        PHANTOM_ASSERT(false, "Expression is const and cannot be modified");
    }
    InterpreterContext context;
    Reference*         pReference = m_pValueType->asReference();
    if (pReference)
    {
        void* pBuffer;
        eval(context, &pBuffer);
        pReference->getReferencedType()->copy(pBuffer, a_pSrc);
    }
    else
        setValue(a_pSrc);
    flush(context);
}

bool Expression::hasEffectiveAddress() const
{
    return (m_pValueType->asReference() != nullptr) || hasStorage();
}

void* Expression::loadEffectiveAddress(ExecutionContext& a_Context) const
{
    auto typeID = getEvalType()->getTypeKind();
    if (typeID == TypeKind::LValueReference || typeID == TypeKind::RValueReference)
    {
        void* pAddress = nullptr;
        eval(a_Context, &pAddress);
        return pAddress;
    }
    else if (hasStorage())
    {
        return evalStorage(a_Context);
    }
    else
    {
        /// If no effective address, allocate a temporary
        Type* pTypeNoRef = getEvalType()->removeRValueReference();
        void* pTemp = pTypeNoRef->allocate();
        eval(a_Context, pTemp);
        pushTempDestruction(a_Context, pTypeNoRef, pTemp);
        return pTemp;
    }
}

void* Expression::loadEffectiveAddress() const
{
    InterpreterContext ctx;
    return loadEffectiveAddress(ctx);
}

void Expression::flush(ExecutionContext&) const {}

Type* Expression::storageType(Type* a_pType) const
{
    Type* pStorageType = a_pType;
    if (pStorageType == PHANTOM_TYPEOF(void))
        return nullptr;
    if (pStorageType->asLValueReference())
    {
        pStorageType = pStorageType->asLValueReference()->getReferencedType()->makePointer();
    }
    return pStorageType;
}

Variant Expression::get() const
{
    Variant v;
    if (m_pValueType->removeReference()->removeQualifiers() == PHANTOM_TYPEOF(const char*))
    {
        const char* pStr;
        load(&pStr);
        return Variant(pStr);
    }
    v.setType(m_pValueType->removeReference()->removeQualifiers());
    load((void*)v.data());
    return v;
}

void Expression::set(const Variant& v)
{
    if (v.type() == m_pValueType->removeReference())
    {
        store(v.data());
    }
    else
    {
        Variant casted = v.as(m_pValueType->removeReference());
        if (casted.isNull())
        {
            PHANTOM_ASSERT(false, "Cannot convert Variant to expression value type");
        }
        store(casted.data());
    }
}

Expression* Expression::toExpression(Expression* /*= nullptr*/) const
{
    return (Expression*)this;
}

bool Expression::isPersistent() const
{
    auto exps = getSubExpressions();
    for (auto exp : exps)
    {
        if (!(exp->isPersistent()))
            return false;
    }
    return m_pValueType->isNative();
}

void Expression::onAttachedToBlock(Block* a_pBlock)
{
    size_t subExCnt = getSubExpressions().size();
    while (subExCnt--)
    {
        getSubExpressions()[subExCnt]->onAttachedToBlock(a_pBlock);
    }
    if (isTemporary())
    {
        Class* pClass = getValueType()->removeQualifiers()->asClass(); /// X&&
        if (pClass)
        {
            Evaluable* pLifeTimeScope = evaluateLifeTime();
            PHANTOM_ASSERT(pLifeTimeScope);
            pLifeTimeScope->addScopedDestruction(pLifeTimeScope->New<TemporaryObjectDestructionExpression>(this));
        }
    }
    //     for (auto pSym : m_SymbolDependencies)
    //     {
    //         CompiledSource::Get(getSource())->ensureStrongDependency(this, pSym);
    //     }
}

bool Expression::hasEvalConversionTo(Type* a_pOutputType) const
{
    Type* pInputNoQualifiers = getValueType()->removeAllQualifiers();
    Type* pInputEvalNoQualifiers = getEvalType()->removeAllQualifiers();
    Type* pOutputNoQualifiers = a_pOutputType->removeAllQualifiers();
    if (pInputNoQualifiers->isSame(pOutputNoQualifiers))
        return true;
    if (pInputEvalNoQualifiers->isSame(pOutputNoQualifiers))
        return true;
    if (pInputEvalNoQualifiers == PHANTOM_TYPEOF(void*) && pOutputNoQualifiers->asPointer())
        return true;
    if (pOutputNoQualifiers == PHANTOM_TYPEOF(void*) && pInputEvalNoQualifiers->asPointer())
        return true;
    if (pInputEvalNoQualifiers->asAddressType() && pOutputNoQualifiers->asAddressType() &&
        pInputEvalNoQualifiers->removeAddress()->getPointerAdjustmentOffset(pOutputNoQualifiers->removeAddress()) == 0)
        return true;
    Conversion* conv = getSemantic()->newConversion(getEvalType()->removeAllQualifiers(), a_pOutputType,
                                                    CastKind::Implicit, UserDefinedFunctions::ImplicitsOnly, nullptr);
    if (conv == nullptr)
        return false;
    bool canonical = conv->isCanonical();
    PHANTOM_ASSERT(conv->semantic);
    return canonical;
}

Evaluable* Expression::evaluateExpressionLifeTime(Expression*) const
{
    /// by default life time of a sub expression is the life time of its owner
    return evaluateLifeTime();
}

Evaluable* Expression::evaluateLifeTime() const
{
    PHANTOM_ASSERT(getOwner()); // ASSERT_DEBUG
    return static_cast<Evaluable*>(getOwner())->evaluateExpressionLifeTime(const_cast<Expression*>(this));
}

Expression* Expression::dereference(LanguageElement* a_pOwner)
{
    Reference* pReference = m_pValueType->asReference();
    if (pReference)
    {
        return load(a_pOwner)->dereference(a_pOwner);
    }

    Expression* noIdentity = removeIdentity();
    if (noIdentity->getMetaClass() == PHANTOM_CLASSOF(BuiltInOperatorExpression))
    {
        if (static_cast<BuiltInOperatorExpression*>(noIdentity)->getOperator()->getId() == Operator::Address)
        {
            Expression* result = static_cast<BuiltInOperatorExpression*>(noIdentity)->getEvalArgument(0);
            result->detach();
            // we are dereferencing an addressing => cancel each others
            if (noIdentity->getValueType()->removeAllQualifiers() == getValueType()->removeAllQualifiers())
            {
                // identity is just a const adjustement
                return result;
            }
            else
            {
                return New<IdentityExpression>(m_pValueType->removePointer()->addLValueReference(), result);
            }
        }
    }

    Expression* args[2] = {(Expression*)this, 0};
    if (isTemplateDependant())
    {
        return a_pOwner->New<UnaryPreOperationExpression>(
        m_pValueType->removeQualifiers()->removePointer()->addLValueReference(), "*", args[0]);
    }
    Pointer* pPointerType = m_pValueType->removeReference()->removeQualifiers()->asPointer();
    PHANTOM_ASSERT(pPointerType);
    return a_pOwner->New<BuiltInOperatorExpression>(CppLite::Get()->getBuiltInOperator(Operator::Dereference),
                                                    pPointerType->removePointer()->addLValueReference(), args, args,
                                                    BuiltInOperatorDelegate(pPointerType, &Pointer::dereference));
}

Expression* Expression::address(LanguageElement* a_pOwner)
{
    Expression* args[2] = {(Expression*)this, 0};
    if (isTemplateDependant())
    {
        return a_pOwner->New<UnaryPreOperationExpression>(m_pValueType->removeReference()->addPointer(), "&", args[0]);
    }
    Expression* noIdentity = removeIdentity();
    if (noIdentity->getMetaClass() == PHANTOM_CLASSOF(BuiltInOperatorExpression))
    {
        if (static_cast<BuiltInOperatorExpression*>(noIdentity)->getOperator()->getId() == Operator::Dereference)
        {
            Expression* result = static_cast<BuiltInOperatorExpression*>(noIdentity)->getEvalArgument(0);
            result->detach();
            // we are addressing a dereferencement => cancel each others
            if (noIdentity->getValueType()->removeAllQualifiers() == getValueType()->removeAllQualifiers())
            {
                // identity is just a const adjustement
                return result;
            }
            else
            {
                return New<IdentityExpression>(m_pValueType->removeReference()->addPointer(), result);
            }
        }
    }

    Reference* pReference = m_pValueType->asReference();
    PHANTOM_ASSERT(pReference);
    return a_pOwner->New<BuiltInOperatorExpression>(CppLite::Get()->getBuiltInOperator(Operator::Address),
                                                    m_pValueType->removeReference()->addPointer(), args, args,
                                                    BuiltInOperatorDelegate(pReference, &Reference::address));
}

Expression* Expression::arrayAccess(LanguageElement* a_pOwner, Expression* a_pIndexExpression)
{
    Reference* pReference = m_pValueType->asReference();
    if (pReference)
    {
        return load(a_pOwner)->arrayAccess(a_pOwner, a_pIndexExpression);
    }
    Expression* args[2] = {(Expression*)this, a_pIndexExpression};
    if (isTemplateDependant())
    {
        return a_pOwner->New<BinaryOperationExpression>(m_pValueType->removePointerOrArray()->addLValueReference(),
                                                        "[]", args[0], args[1]);
    }
    PHANTOM_ASSERT(m_pValueType->asArray() || m_pValueType->asPointer());
    Type* pElementType = m_pValueType->removePointerOrArray();
    Type* pResultType = pElementType->makeLValueReference();
    Type* pPointer = pElementType->makePointer();
    PHANTOM_ASSERT(pPointer->asPointer());
    return a_pOwner->New<BuiltInOperatorExpression>(
    CppLite::Get()->getBuiltInOperator(Operator::Bracket), pResultType, args, args,
    BuiltInOperatorDelegate(static_cast<Pointer*>(pPointer), &Pointer::bracket));
}

Expression* Expression::arrayAccess(LanguageElement* a_pOwner, size_t a_uiIndex)
{
    return arrayAccess(a_pOwner, New<ConstantExpression>(lang::Constant::Create<size_t>(a_pOwner, a_uiIndex)));
}

Expression* Expression::adjust(LanguageElement* a_pOwner, ptrdiff_t a_iOffset, Type* a_pOutputType /*= nullptr*/)
{
    PHANTOM_ASSERT(getValueType()->asReference() || getValueType()->asPointer() || getValueType()->asArray());
    PHANTOM_ASSERT(a_pOutputType->asReference() || a_pOutputType->asPointer() || a_pOutputType->asArray());
    PHANTOM_ASSERT(a_pOutputType->asReference() == nullptr || getValueType()->asReference() != nullptr,
                   "if output type is a reference type, this expression must be a reference expression");
    Expression* pThis = this;
    if (a_pOutputType)
    {
        if (a_pOutputType->asPointer() && getValueType()->asReference())
            pThis = load(a_pOwner);
    }
    if (a_iOffset == 0)
    {
        return a_pOutputType ? a_pOwner->New<IdentityExpression>(a_pOutputType, pThis) : pThis;
    }
    return a_pOwner->New<PointerAdjustmentExpression>(a_pOutputType ? a_pOutputType : getValueType(), pThis, a_iOffset);
}

Expression* Expression::memberAccess(LanguageElement* /*a_pOwner*/, StringView a_Name,
                                     LanguageElements const* a_pTemplateArguments,
                                     Expressions const*      a_pFunctionArguments,
                                     LanguageElement*        a_pContextScope /*= nullptr*/)
{
    LanguageElement* pElement;
    if (Semantic* pSema = getSemantic())
        pElement = pSema->qualifiedLookup(this, a_Name, MakeOptionalArrayView(a_pTemplateArguments),
                                          MakeOptionalArrayView(a_pFunctionArguments), a_pContextScope, nullptr, 0);
    else
        pElement = CppLite::Get()->getDefaultSemantic()->qualifiedLookup(
        this, a_Name, MakeOptionalArrayView(a_pTemplateArguments), MakeOptionalArrayView(a_pFunctionArguments),
        a_pContextScope, nullptr, 0);
    return pElement ? getSemantic()->toExpression(pElement) : nullptr;
}

Expression* Expression::assign(LanguageElement* a_pOwner, Expression* a_pRightExpression)
{
    if (a_pRightExpression->isTemplateDependant() || isTemplateDependant())
    {
        return a_pOwner->New<BinaryOperationExpression>("=", this, a_pRightExpression);
    }
    Reference*  pReference = m_pValueType->asReference();
    Expression* args[2] = {(Expression*)this, a_pRightExpression};
    PHANTOM_ASSERT(pReference && pReference->getReferencedType()->asClassType() == nullptr);
    Type* pElementType = pReference->getReferencedType()->getUnderlyingType();
    Type* pResultType =
    pReference->asRValueReference() ? (Type*)pElementType->makeRValueReference() : pElementType->makeLValueReference();
    return a_pOwner->New<BuiltInOperatorExpression>(
    CppLite::Get()->getBuiltInOperator(Operator::Assignment), pResultType, args, args,
    BuiltInOperatorDelegate(pReference, &Reference::memcpyStoreDelegate));
}

bool Expression::isCompileTime() const
{
    auto exps = getSubExpressions();
    if (exps.size())
    {
        for (auto exp : exps)
        {
            if (!(exp->isCompileTime()))
                return false;
        }
        return true;
    }
    return false;
}

Expression* Expression::clone(LanguageElement* a_pOwner) const
{
    Expression* pClone = cloneImpl(a_pOwner);
    pClone->setFlags(getFlags() & ~PHANTOM_R_FLAG_NATIVE); // on cloning, native should disappear
    pClone->setCodeRange(getCodeRange());
    return pClone;
}

void Expression::setTemporary(bool a_bValue)
{
    if (a_bValue)
        setFlags(getFlags() | PHANTOM_R_FLAG_TEMP);
    else
        setFlags(getFlags() & ~PHANTOM_R_FLAG_TEMP);
}

bool Expression::isTemporary() const
{
    return testFlags(PHANTOM_R_FLAG_TEMP);
}

void Expression::dump(std::ostream& a_Out, int a_Lvl) const
{
    int l = a_Lvl;
    while (l--)
        a_Out << "  ";
    a_Out << rtti.metaClass->getName() << " (" << getValueType()->getDecoratedName() << ')' << std::endl;

    auto exps = getSubExpressions();
    for (auto exp : exps)
        exp->dump(a_Out, a_Lvl + 1);
}

void* Expression::evalStorage(ExecutionContext&) const
{
    return nullptr;
}

void* Expression::getStorage() const
{
    return nullptr;
}

} // namespace lang
} // namespace phantom
