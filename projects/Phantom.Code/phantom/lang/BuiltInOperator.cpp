// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "BuiltInOperator.h"

#include "BuiltInOperatorExpression.h"
#include "BuiltInOperators.h"
#include "CppLite.h"
#include "Semantic.h"
#include "phantom/lang/FieldPointer.h"
#include "phantom/lang/MethodPointer.h"

#include <phantom/lang/FunctionPointer.h>
#include <phantom/lang/LValueReference.h>
#include <phantom/lang/MemberPointer.h>
#include <phantom/lang/Method.h>
#include <phantom/lang/Pointer.h>
#include <phantom/lang/RValueReference.h>
#include <phantom/lang/VolatileType.h>
#include <phantom/utils/SmallSet.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
BuiltInOperator::BuiltInOperator(Operator a_eOperator)
    : Symbol(OperatorName(a_eOperator), 0, PHANTOM_R_FLAG_NATIVE),
      m_eOperator(a_eOperator),
      m_eType(OperatorType(a_eOperator))
{
    switch (m_eType)
    {
    case OperatorKind::Binary:
    case OperatorKind::UnaryPostfixed:
        m_uiOperandCount = 2;
        break;
    case OperatorKind::UnaryPrefixed:
    case OperatorKind::Postfixed:
        m_uiOperandCount = 1;
        break;
    case OperatorKind::Ternary:
        m_uiOperandCount = 3;
        break;
    default:
        m_uiOperandCount = ~size_t(0);
    }
}

void BuiltInOperator::getQualifiedName(StringBuffer& a_Buf) const
{
    getName(a_Buf);
}

void BuiltInOperator::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    getQualifiedName(a_Buf);
}

void BuiltInOperator::getDecoratedName(StringBuffer& a_Buf) const
{
    getName(a_Buf);
}

OperatorKind BuiltInOperator::OperatorType(Operator a_eOperator)
{
    switch (a_eOperator)
    {
    case Operator::Complement:
    case Operator::Not:
    case Operator::Address:
    case Operator::Dereference:
    case Operator::PreDecrement:
    case Operator::PreIncrement:
    case Operator::Minus:
    case Operator::Plus:
        return OperatorKind::UnaryPrefixed;

    case Operator::PostDecrement:
    case Operator::PostIncrement:
        return OperatorKind::UnaryPostfixed;

    case Operator::Arrow:
    case Operator::ArrowStar:
        return OperatorKind::Postfixed;

    case Operator::Parenthesis:
        return OperatorKind::Vararg;
    }
    return OperatorKind::Binary;
}

void BuiltInOperator::fetchExtraOverloads(Type* a_pType, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const
{
    a_pType = a_pType->removeQualifiers();
    if (!a_AlreadyTreated.insert(a_pType).second)
        return;
    switch (a_pType->getTypeKind())
    {
    case TypeKind::Pointer:
        if (a_pType->asPointer())
        {
            fetchExtraOverloads(static_cast<Pointer*>(a_pType), a_Overloads, a_AlreadyTreated);
        }
        else if (a_pType->asFunctionPointer())
        {
            fetchExtraOverloads(static_cast<FunctionPointer*>(a_pType), a_Overloads, a_AlreadyTreated);
        }
        break;
    case TypeKind::FieldPointer:
        fetchExtraOverloads(static_cast<FieldPointer*>(a_pType), a_Overloads, a_AlreadyTreated);
        break;
    case TypeKind::MethodPointer:
        fetchExtraOverloads(static_cast<MethodPointer*>(a_pType), a_Overloads, a_AlreadyTreated);
        break;
    case TypeKind::FunctionPointer:
        fetchExtraOverloads(static_cast<FunctionPointer*>(a_pType), a_Overloads, a_AlreadyTreated);
        break;
    case TypeKind::Class:
        fetchExtraOverloads(static_cast<Class*>(a_pType), a_Overloads, a_AlreadyTreated);
        break;
    case TypeKind::Structure:
        fetchExtraOverloads(static_cast<ClassType*>(a_pType), a_Overloads, a_AlreadyTreated);
        break;
    case TypeKind::LValueReference:
        fetchExtraOverloads(static_cast<LValueReference*>(a_pType), a_Overloads, a_AlreadyTreated);
        break;
    case TypeKind::RValueReference:
        fetchExtraOverloads(static_cast<RValueReference*>(a_pType), a_Overloads, a_AlreadyTreated);
        break;
    case TypeKind::Enum:
        fetchExtraOverloads(static_cast<Enum*>(a_pType), a_Overloads, a_AlreadyTreated);
        break;
    case TypeKind::Array:
        fetchExtraOverloads(static_cast<Array*>(a_pType), a_Overloads, a_AlreadyTreated);
        break;
    default:
        if (a_pType == PHANTOM_TYPEOF(std::nullptr_t))
        {
            fetchExtraNullptrOverloads(a_Overloads);
        }
        break;
    }
}

void BuiltInOperator::fetchExtraOverloads(ClassType* a_pClassType, Overloads& a_Overloads,
                                          TypeSet& a_AlreadyTreated) const
{
    // look for every conversion functions
    for (auto p : a_pClassType->getMethods())
    {
        StringView name = p->getName();
        if (name.size() >= 8 && name[8] == ' ') // TODO : find a faster way to distinguish conversion functions
        {
            fetchExtraOverloads(p->getReturnType(), a_Overloads, a_AlreadyTreated);
        }
    }
}

void BuiltInOperator::fetchExtraNullptrOverloads(Overloads& a_Overloads) const
{
    Type* pBoolType = PHANTOM_TYPEOF(bool);
    Type* pNullptrType = PHANTOM_TYPEOF(std::nullptr_t);
    switch (m_eOperator)
    {
        /// standard 13.6.16
        /// ------------------------
        /// bool operator==(T , T );
    case Operator::Equal:
        a_Overloads.push_back(
        Overload(pBoolType, pNullptrType, pNullptrType,
                 BuiltInOperatorDelegate(static_cast<PointerType*>(PHANTOM_TYPEOF(void*)), &PointerType::equal)));
        break;
    }
}

void BuiltInOperator::fetchExtraOverloads(Class* a_pClass, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const
{
    fetchExtraOverloads(static_cast<ClassType*>(a_pClass), a_Overloads, a_AlreadyTreated);
    // look for every base classes conversion functions
    for (Class* pBC : a_pClass->getBaseClasses())
    {
        if (a_AlreadyTreated.insert(pBC).second)
        {
            fetchExtraOverloads(pBC, a_Overloads, a_AlreadyTreated);
        }
    }
}

void BuiltInOperator::fetchExtraOverloads(MemberPointer* a_pMemberPointer, Overloads& a_Overloads, TypeSet&) const
{
    Type* pBoolType = PHANTOM_TYPEOF(bool);
    Type* pNullptrType = PHANTOM_TYPEOF(std::nullptr_t);
    switch (m_eOperator)
    {
        /// standard 13.6.16
        /// ------------------------
        /// bool operator==(T , T );
    case Operator::Equal:
        a_Overloads.push_back(Overload(pBoolType, a_pMemberPointer, a_pMemberPointer,
                                       BuiltInOperatorDelegate(a_pMemberPointer, &MemberPointer::equal)));
        a_Overloads.push_back(Overload(pBoolType, a_pMemberPointer, pNullptrType,
                                       BuiltInOperatorDelegate(a_pMemberPointer, &MemberPointer::equal)));
        a_Overloads.push_back(Overload(pBoolType, pNullptrType, a_pMemberPointer,
                                       BuiltInOperatorDelegate(a_pMemberPointer, &MemberPointer::equal)));
        break;

        /// bool operator!=(T , T );
    case Operator::NotEqual:
        a_Overloads.push_back(Overload(pBoolType, a_pMemberPointer, a_pMemberPointer,
                                       BuiltInOperatorDelegate(a_pMemberPointer, &MemberPointer::notEqual)));
        a_Overloads.push_back(Overload(pBoolType, a_pMemberPointer, pNullptrType,
                                       BuiltInOperatorDelegate(a_pMemberPointer, &MemberPointer::notEqual)));
        a_Overloads.push_back(Overload(pBoolType, pNullptrType, a_pMemberPointer,
                                       BuiltInOperatorDelegate(a_pMemberPointer, &MemberPointer::notEqual)));
        break;

        /// standard 13.6.20
        /// ------------------------
        /// VQ T & operator=(VQ T &, T );
    case Operator::Assignment:
        a_Overloads.push_back(Overload(a_pMemberPointer->makeLValueReference(), a_pMemberPointer->makeLValueReference(),
                                       a_pMemberPointer,
                                       BuiltInOperatorDelegate(a_pMemberPointer, &MemberPointer::assignment)));
        a_Overloads.push_back(Overload(a_pMemberPointer->makeVolatile()->makeLValueReference(),
                                       a_pMemberPointer->makeVolatile()->makeLValueReference(), a_pMemberPointer,
                                       BuiltInOperatorDelegate(a_pMemberPointer, &MemberPointer::assignment)));
        break;
    }
}

void BuiltInOperator::fetchExtraOverloads(Reference* a_pReference, Overloads& a_Overloads,
                                          TypeSet& a_AlreadyTreated) const
{
    if (m_eOperator == Operator::Address)
    {
        a_Overloads.push_back(Overload(a_pReference->getReferencedType()->makePointer(), a_pReference, nullptr,
                                       BuiltInOperatorDelegate(a_pReference, &Reference::address)));
    }
    else
    {
        Type* pType = a_pReference->removeReference();
        fetchExtraOverloads(pType, a_Overloads, a_AlreadyTreated);
    }
}

void BuiltInOperator::fetchExtraOverloads(Enum* a_pEnum, Overloads& a_Overloads, TypeSet&) const
{
    builtin_operation_delegate_selector selector;
    Type*                               pBoolType = PHANTOM_TYPEOF(bool);
    /// 13.6.15
    /// bool operator<(T , T );
    /// bool operator>(T , T );
    /// bool operator<=(T , T );
    /// bool operator>=(T , T );
    /// bool operator==(T , T );
    /// bool operator!=(T , T );
    Type* pUnderlyingType = a_pEnum->getUnderlyingIntType();

    switch (m_eOperator)
    {
    case Operator::Less:
        a_Overloads.push_back(
        Overload(pBoolType, a_pEnum, a_pEnum, selector.delegate(Operator::Less, pUnderlyingType, pUnderlyingType)));
        break;
    case Operator::Greater:
        a_Overloads.push_back(
        Overload(pBoolType, a_pEnum, a_pEnum, selector.delegate(Operator::Greater, pUnderlyingType, pUnderlyingType)));
        break;
    case Operator::LessEqual:
        a_Overloads.push_back(Overload(pBoolType, a_pEnum, a_pEnum,
                                       selector.delegate(Operator::LessEqual, pUnderlyingType, pUnderlyingType)));
        break;
    case Operator::GreaterEqual:
        a_Overloads.push_back(Overload(pBoolType, a_pEnum, a_pEnum,
                                       selector.delegate(Operator::GreaterEqual, pUnderlyingType, pUnderlyingType)));
        break;
    case Operator::Equal:
        a_Overloads.push_back(
        Overload(pBoolType, a_pEnum, a_pEnum, selector.delegate(Operator::Equal, pUnderlyingType, pUnderlyingType)));
        break;
    case Operator::NotEqual:
        a_Overloads.push_back(
        Overload(pBoolType, a_pEnum, a_pEnum, selector.delegate(Operator::NotEqual, pUnderlyingType, pUnderlyingType)));
        break;
        /// 13.6.19
        /// T *VQ & operator=(T *VQ &, T *);
    case Operator::Assignment:
        a_Overloads.push_back(Overload(a_pEnum->makeLValueReference(), a_pEnum->makeLValueReference(), a_pEnum,
                                       selector.delegate(Operator::Assignment, pUnderlyingType, pUnderlyingType)));
        a_Overloads.push_back(Overload(a_pEnum->makeVolatile()->makeLValueReference(),
                                       a_pEnum->makeVolatile()->makeLValueReference(), a_pEnum,
                                       selector.delegate(Operator::Assignment, pUnderlyingType, pUnderlyingType)));
        break;
    }
}

void BuiltInOperator::fetchExtraOverloads(PointerType* a_pPointerType, Overloads& a_Overloads, TypeSet&) const
{
    Type* pBoolType = PHANTOM_TYPEOF(bool);
    Type* ref = a_pPointerType->makeLValueReference();
    Type* Vref = a_pPointerType->makeVolatile()->makeLValueReference();
    Type* pNullptrType = PHANTOM_TYPEOF(std::nullptr_t);
    switch (m_eOperator)
    {
        /// 13.6.15
        /// ----------------------
        /// bool operator<(T , T );
        /// bool operator>(T , T );
        /// bool operator<=(T , T );
        /// bool operator>=(T , T );
        /// bool operator==(T , T );
        /// bool operator!=(T , T );

    case Operator::Less:
        a_Overloads.push_back(Overload(pBoolType, a_pPointerType, a_pPointerType,
                                       BuiltInOperatorDelegate(a_pPointerType, &PointerType::less)));
        break;
    case Operator::Greater:
        a_Overloads.push_back(Overload(pBoolType, a_pPointerType, a_pPointerType,
                                       BuiltInOperatorDelegate(a_pPointerType, &PointerType::greater)));
        break;
    case Operator::LessEqual:
        a_Overloads.push_back(Overload(pBoolType, a_pPointerType, a_pPointerType,
                                       BuiltInOperatorDelegate(a_pPointerType, &PointerType::lessEqual)));
        break;
    case Operator::GreaterEqual:
        a_Overloads.push_back(Overload(pBoolType, a_pPointerType, a_pPointerType,
                                       BuiltInOperatorDelegate(a_pPointerType, &PointerType::greaterEqual)));
        break;
    case Operator::Equal:
        a_Overloads.push_back(Overload(pBoolType, a_pPointerType, a_pPointerType,
                                       BuiltInOperatorDelegate(a_pPointerType, &PointerType::equal)));
        a_Overloads.push_back(Overload(pBoolType, pNullptrType, a_pPointerType,
                                       BuiltInOperatorDelegate(a_pPointerType, &PointerType::equal)));
        a_Overloads.push_back(Overload(pBoolType, a_pPointerType, pNullptrType,
                                       BuiltInOperatorDelegate(a_pPointerType, &PointerType::equal)));
        break;
    case Operator::NotEqual:
        a_Overloads.push_back(Overload(pBoolType, a_pPointerType, a_pPointerType,
                                       BuiltInOperatorDelegate(a_pPointerType, &PointerType::notEqual)));
        a_Overloads.push_back(Overload(pBoolType, pNullptrType, a_pPointerType,
                                       BuiltInOperatorDelegate(a_pPointerType, &PointerType::notEqual)));
        a_Overloads.push_back(Overload(pBoolType, a_pPointerType, pNullptrType,
                                       BuiltInOperatorDelegate(a_pPointerType, &PointerType::notEqual)));
        break;

        /// 13.6.19
        /// T *VQ & operator=(T *VQ &, T *);
    case Operator::Assignment:
        a_Overloads.push_back(
        Overload(ref, ref, a_pPointerType, BuiltInOperatorDelegate(a_pPointerType, &PointerType::assignment)));
        a_Overloads.push_back(
        Overload(Vref, Vref, a_pPointerType, BuiltInOperatorDelegate(a_pPointerType, &PointerType::assignment)));
        break;
    }
}

void BuiltInOperator::fetchExtraOverloads(Pointer* a_pPointer, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const
{
    Type* std_ptrdiff_t = PHANTOM_TYPEOF(std::ptrdiff_t);
    Type* pIntType = PHANTOM_TYPEOF(int);
    bool  is_void = a_pPointer->getPointeeType()->removeAllQualifiers() == PHANTOM_TYPEOF(void);
    Type* ref = a_pPointer->makeLValueReference();
    Type* Vref = a_pPointer->makeVolatile()->makeLValueReference();
    Type* deref = a_pPointer->removePointer()->makeLValueReference();

    switch (m_eOperator)
    {
        /// 13.6.5
        /// T *VQ & operator++(T *VQ &);
        /// T *VQ & operator--(T *VQ &);
        /// T * operator++(T *VQ &, int);
        /// T * operator--(T *VQ &, int);
    case Operator::PreIncrement:
        if (!is_void)
        {
            a_Overloads.push_back(
            Overload(ref, ref, nullptr, BuiltInOperatorDelegate(a_pPointer, &Pointer::preIncrement)));
            a_Overloads.push_back(
            Overload(Vref, Vref, nullptr, BuiltInOperatorDelegate(a_pPointer, &Pointer::preIncrement)));
        }
        break;
    case Operator::PreDecrement:
        if (!is_void)
        {
            a_Overloads.push_back(
            Overload(ref, ref, nullptr, BuiltInOperatorDelegate(a_pPointer, &Pointer::preDecrement)));
            a_Overloads.push_back(
            Overload(Vref, Vref, nullptr, BuiltInOperatorDelegate(a_pPointer, &Pointer::preDecrement)));
        }
        break;
    case Operator::PostIncrement:
        if (!is_void)
        {
            a_Overloads.push_back(
            Overload(a_pPointer, ref, pIntType, BuiltInOperatorDelegate(a_pPointer, &Pointer::postIncrement)));
            a_Overloads.push_back(
            Overload(a_pPointer, Vref, pIntType, BuiltInOperatorDelegate(a_pPointer, &Pointer::postIncrement)));
        }
        break;
    case Operator::PostDecrement:
        if (!is_void)
        {
            a_Overloads.push_back(
            Overload(a_pPointer, ref, pIntType, BuiltInOperatorDelegate(a_pPointer, &Pointer::postDecrement)));
            a_Overloads.push_back(
            Overload(a_pPointer, Vref, pIntType, BuiltInOperatorDelegate(a_pPointer, &Pointer::postDecrement)));
        }
        break;

        /// 13.6.6
        /// T & operator*(T *);
        //
    case Operator::Dereference:
        if (!is_void)
        {
            a_Overloads.push_back(
            Overload(deref, a_pPointer, nullptr, BuiltInOperatorDelegate(a_pPointer, &Pointer::dereference)));
        }
        break;

        /// 13.6.8
        /// T * operator+(T *);
        //
    case Operator::Plus:
        a_Overloads.push_back(
        Overload(a_pPointer, a_pPointer, nullptr, BuiltInOperatorDelegate(a_pPointer, &Pointer::plus)));
        break;

        /// 13.6.13
        /// T * operator+(T *, std::ptrdiff_t);
        /// T & operator[](T *, std::ptrdiff_t);
        /// T * operator-(T *, std::ptrdiff_t);
        /// T * operator+(std::ptrdiff_t, T *);
        /// T & operator[](std::ptrdiff_t, T *);

    case Operator::Add:
        if (!is_void)
        {
            a_Overloads.push_back(
            Overload(a_pPointer, std_ptrdiff_t, a_pPointer, BuiltInOperatorDelegate(a_pPointer, &Pointer::addRev)));
            a_Overloads.push_back(
            Overload(a_pPointer, a_pPointer, std_ptrdiff_t, BuiltInOperatorDelegate(a_pPointer, &Pointer::add)));
        }
        break;

    case Operator::Subtract:
        if (!is_void)
        {
            a_Overloads.push_back(
            Overload(a_pPointer, a_pPointer, std_ptrdiff_t, BuiltInOperatorDelegate(a_pPointer, &Pointer::subtract)));
        }
        /// 13.6.14
        /// std::ptrdiff_t operator-(T , T );
        a_Overloads.push_back(Overload(std_ptrdiff_t, a_pPointer, a_pPointer,
                                       BuiltInOperatorDelegate(a_pPointer, &Pointer::subtractPointer)));
        break;

    case Operator::Bracket:
        if (!is_void)
        {
            a_Overloads.push_back(
            Overload(deref, std_ptrdiff_t, a_pPointer, BuiltInOperatorDelegate(a_pPointer, &Pointer::bracketRev)));
            a_Overloads.push_back(
            Overload(deref, a_pPointer, std_ptrdiff_t, BuiltInOperatorDelegate(a_pPointer, &Pointer::bracket)));
        }
        break;

        /// 13.6.21
        /// T *VQ & operator+=(T *VQ &, std::ptrdiff_t);
        /// T *VQ & operator-=(T *VQ &, std::ptrdiff_t);
    case Operator::AssignmentAdd:
        if (!is_void)
        {
            a_Overloads.push_back(
            Overload(ref, ref, std_ptrdiff_t, BuiltInOperatorDelegate(a_pPointer, &Pointer::assignmentAdd)));
            a_Overloads.push_back(
            Overload(Vref, Vref, std_ptrdiff_t, BuiltInOperatorDelegate(a_pPointer, &Pointer::assignmentAdd)));
        }
        break;

    case Operator::AssignmentSubtract:
        if (!is_void)
        {
            a_Overloads.push_back(
            Overload(ref, ref, std_ptrdiff_t, BuiltInOperatorDelegate(a_pPointer, &Pointer::assignmentSubtract)));
            a_Overloads.push_back(
            Overload(Vref, Vref, std_ptrdiff_t, BuiltInOperatorDelegate(a_pPointer, &Pointer::assignmentSubtract)));
        }
        break;
    }
    fetchExtraOverloads(static_cast<PointerType*>(a_pPointer), a_Overloads, a_AlreadyTreated);
}

void BuiltInOperator::fetchExtraOverloads(FunctionPointer* a_pFunctionPointer, Overloads& a_Overloads,
                                          TypeSet& a_AlreadyTreated) const
{
    fetchExtraOverloads(static_cast<PointerType*>(a_pFunctionPointer), a_Overloads, a_AlreadyTreated);
}

void BuiltInOperator::fetchExtraOverloads(Array* a_pArray, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const
{
    fetchExtraOverloads(a_pArray->getItemType()->makePointer(), a_Overloads, a_AlreadyTreated);
}

void BuiltInOperator::newImplicitConversions(Type** a_ppTypes, SmallVector<ConversionResults>& a_Conversions,
                                             Overloads& a_Overloads) const
{
    Language* pLanguage = CppLite::Get();
    Overloads extraOverloads;
    TypeSet   treatedTypes;
    for (size_t i = 0; i < m_uiOperandCount; ++i)
    {
        fetchExtraOverloads(a_ppTypes[i], extraOverloads, treatedTypes);
    }
    a_Conversions.resize(extraOverloads.size() + m_Overloads.size());

    size_t c = 0;
    for (size_t i = 0; i < extraOverloads.size(); ++i)
    {
        ConversionResults& conv = a_Conversions[c++];
        extraOverloads[i].newImplicitConversions(pLanguage, a_ppTypes, conv);
        a_Overloads.push_back(extraOverloads[i]);
    }

    for (size_t i = 0; i < m_Overloads.size(); ++i)
    {
        ConversionResults& conv = a_Conversions[c++];
        m_Overloads[i].newImplicitConversions(pLanguage, a_ppTypes, conv);
        a_Overloads.push_back(m_Overloads[i]);
    }
}

void BuiltInOperator::newImplicitConversions(Expression**                    a_ppExpressions,
                                             SmallVector<ConversionResults>& a_Conversions,
                                             Overloads&                      a_Overloads) const
{
    Type* types[3];
    for (size_t i = 0; i < m_uiOperandCount; ++i)
        types[i] = a_ppExpressions[i]->getValueType();
    newImplicitConversions(types, a_Conversions, a_Overloads);
}

void BuiltInOperator::deleteConversions(SmallVector<ConversionResults>& a_Conversions) const
{
    for (auto it = a_Conversions.begin(); it != a_Conversions.end(); ++it)
    {
        it->destroy(getSource());
    }
}

void BuiltInOperator::editOverload(bool a_bAdd, Type* a_pResult, Type* a_pOp0, Type* a_pOp1,
                                   BuiltInOperatorDelegate a_Delegate)
{
    if (a_bAdd)
        addOverload(a_pResult, a_pOp0, a_pOp1, a_Delegate);
    else
        removeOverload(a_pResult, a_pOp0, a_pOp1, a_Delegate);
}

void BuiltInOperator::editOverload(bool a_bAdd, Type* a_pResult, Type* a_pOp0, BuiltInOperatorDelegate a_Delegate)
{
    if (a_bAdd)
        addOverload(a_pResult, a_pOp0, 0, a_Delegate);
    else
        removeOverload(a_pResult, a_pOp0, 0, a_Delegate);
}

void BuiltInOperator::addOverload(Type* a_pResult, Type* a_pOp0, Type* a_pOp1, BuiltInOperatorDelegate a_Delegate)
{
    m_Overloads.push_back(Overload(a_pResult, a_pOp0, a_pOp1, a_Delegate));
}

void BuiltInOperator::removeOverload(Type* a_pResult, Type* a_pOp0, Type* a_pOp1, BuiltInOperatorDelegate a_Delegate)
{
    Overload ov(a_pResult, a_pOp0, a_pOp1, a_Delegate);
    for (auto it = m_Overloads.begin(); it != m_Overloads.end(); ++it)
    {
        if (*it == ov)
        {
            m_Overloads.erase(it);
            return;
        }
    }
    PHANTOM_ASSERT(false);
}

const char* BuiltInOperator::OperatorName(Operator a_eOperator)
{
    static SmallMap<Operator, const char*> s_OperatorNames;
    if (s_OperatorNames.empty())
    {
        s_OperatorNames[Operator::Plus] = "+";
        s_OperatorNames[Operator::Minus] = "-";
        s_OperatorNames[Operator::Add] = "+";
        s_OperatorNames[Operator::Subtract] = "-";
        s_OperatorNames[Operator::Multiply] = "*";
        s_OperatorNames[Operator::Divide] = "/";
        s_OperatorNames[Operator::Address] = "&";
        s_OperatorNames[Operator::Dereference] = "*";
        s_OperatorNames[Operator::Arrow] = "->";
        s_OperatorNames[Operator::ArrowStar] = "->*";
        s_OperatorNames[Operator::PreDecrement] = "--";
        s_OperatorNames[Operator::PreIncrement] = "++";
        s_OperatorNames[Operator::PostDecrement] = "--";
        s_OperatorNames[Operator::PostIncrement] = "++";
        s_OperatorNames[Operator::Equal] = "==";
        s_OperatorNames[Operator::NotEqual] = "!=";
        s_OperatorNames[Operator::Greater] = ">";
        s_OperatorNames[Operator::Less] = "<";
        s_OperatorNames[Operator::GreaterEqual] = ">=";
        s_OperatorNames[Operator::LessEqual] = "<=";
        s_OperatorNames[Operator::LogicalAnd] = "&&";
        s_OperatorNames[Operator::LogicalOr] = "||";
        s_OperatorNames[Operator::XOr] = "^";
        s_OperatorNames[Operator::Not] = "!";
        s_OperatorNames[Operator::BitAnd] = "&";
        s_OperatorNames[Operator::BitOr] = "|";
        s_OperatorNames[Operator::Complement] = "~";
        s_OperatorNames[Operator::Modulo] = "%";
        s_OperatorNames[Operator::ShiftLeft] = "<<";
        s_OperatorNames[Operator::ShiftRight] = ">>";
        s_OperatorNames[Operator::Assignment] = "=";
        s_OperatorNames[Operator::AssignmentAdd] = "+=";
        s_OperatorNames[Operator::AssignmentSubtract] = "-=";
        s_OperatorNames[Operator::AssignmentMultiply] = "*=";
        s_OperatorNames[Operator::AssignmentDivide] = "/=";
        s_OperatorNames[Operator::AssignmentBitAnd] = "&=";
        s_OperatorNames[Operator::AssignmentBitOr] = "|=";
        s_OperatorNames[Operator::AssignmentModulo] = "%=";
        s_OperatorNames[Operator::AssignmentShiftLeft] = "<<=";
        s_OperatorNames[Operator::AssignmentShiftRight] = ">>=";
        s_OperatorNames[Operator::AssignmentXOr] = "^=";
        s_OperatorNames[Operator::Bracket] = "[]";
        s_OperatorNames[Operator::Parenthesis] = "()";
        s_OperatorNames[Operator::Comma] = ",";
        s_OperatorNames[Operator::Conversion] = " ";
        s_OperatorNames[Operator::COUNT] = "";
    }
    return s_OperatorNames[a_eOperator];
}

void BuiltInOperator::Overload::newImplicitConversions(Language*, Type** a_ppTypes, ConversionResults& out) const
{
    out.push_back(CppLite::Get()->getDefaultSemantic()->newConversion(a_ppTypes[0], op0, nullptr));
    if (op1)
        out.push_back(CppLite::Get()->getDefaultSemantic()->newConversion(a_ppTypes[1], op1, nullptr));
}

String BuiltInOperator::Overload::toString(StringView a_strName) const
{
    return result->getQualifiedDecoratedName() + " operator" + a_strName + "( " + op0->getQualifiedDecoratedName() +
    (op1 ? String(", ") + op1->getQualifiedDecoratedName() : "") + " )";
}

} // namespace lang
} // namespace phantom
