// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "LanguageElementVisitorEx.h"
#include "conversions.h"
#include "lang.h"

#include <phantom/lang/Type.h>
#include <phantom/utils/Delegate.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
typedef Delegate<void(void** a_pArgs, void* a_pOutput)> BuiltInOperatorDelegate;

/// \brief  Represents a built-in C++ operator. (see C++ standard).
class PHANTOM_EXPORT_PHANTOM_CODE BuiltInOperator : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

    PHANTOM_DECLARE_META_CLASS(BuiltInOperator);

    friend class CppLite;

public:
    typedef SmallSet<Type*> TypeSet;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets operator type from operator ID.
    ///
    /// \param  a_eOperator The operator ID.
    ///
    /// \return The operator type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static OperatorKind OperatorType(Operator a_eOperator);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets operator name from operator ID.
    ///
    /// \param  a_eOperator The operator ID.
    ///
    /// \return The operator name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static const char* OperatorName(Operator a_eOperator);

    /// \brief  Built-in operator Overload data.
    struct Overload
    {
        Overload() = default;
        Overload(Type* r, Type* o0, Type* o1, BuiltInOperatorDelegate d) : result(r), op0(o0), op1(o1), delegate(d) {}

        Type*                   result = nullptr;
        Type*                   op0 = nullptr;
        Type*                   op1 = nullptr;
        BuiltInOperatorDelegate delegate;

        void newImplicitConversions(Language* a_pLanguage, Type** a_ppTypes, ConversionResults& out) const;

        String toString(StringView a_strName) const;

        bool operator==(const Overload& other) const
        {
            return op0->isSame(other.op0) && (op1 == other.op1 || (op1 && op1->isSame(other.op1)));
        }
        bool operator!=(const Overload& other) const { return !operator==(other); }
    };

    typedef SmallVector<Overload> Overloads;

private:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs from operator id.
    ///
    /// \param  a_eOperator The operator ID.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

public:
    BuiltInOperator(Operator a_eOperatorId);
    virtual BuiltInOperator* asBuiltInOperator() const { return const_cast<BuiltInOperator*>(this); }

    OperatorKind getOperatorType() const { return m_eType; }

    Operator getId() const { return m_eOperator; }

    inline size_t getOperandCount() const { return m_uiOperandCount; }

    virtual void getDecoratedName(StringBuffer& a_Buf) const;
    virtual void getQualifiedName(StringBuffer& a_Buf) const;
    virtual void getQualifiedDecoratedName(StringBuffer& a_Buf) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  For each built in operator Overload, generate implicit conversions from given
    ///         argument types.
    ///
    /// \param  a_ppArgumentTypes       The argument types.
    /// \param [out]    a_Conversions   The conversion lists.
    /// \param [out]    a_Overloads     The overloads matching each conversion list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void newImplicitConversions(Type** a_ppArgumentTypes, SmallVector<ConversionResults>& a_Conversions,
                                Overloads& a_Overloads) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  For each built in operator Overload, generate implicit conversions from given
    ///         argument expressions.
    ///
    /// \param  a_ppArgumentTypes       The argument expressions.
    /// \param [out]    a_Conversions   The conversion lists.
    /// \param [out]    a_Overloads     The overloads matching each conversion list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void newImplicitConversions(Expression** a_ppExpressions, SmallVector<ConversionResults>& a_Conversions,
                                Overloads& a_Overloads) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Release conversions created with newImplicitConversions functions.
    ///
    /// \param [out]    a_Conversions   The conversions list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void deleteConversions(SmallVector<ConversionResults>& a_Conversions) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator poiting to the first Overload.
    ///
    /// \return An iterator.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Overloads::const_iterator beginOverloads() const { return m_Overloads.begin(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator poiting to the end of overloads.
    ///
    /// \return An iterator
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Overloads::const_iterator endOverloads() const { return m_Overloads.end(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets Overload at given position index
    ///
    /// \return An Overload struct
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Overload getOverload(size_t i) const { return m_Overloads[i]; }

private:
    void editOverload(bool a_bAdd, Type* a_pResult, Type* a_pOp0, Type* a_pOp1, BuiltInOperatorDelegate a_Delegate);
    void editOverload(bool a_bAdd, Type* a_pResult, Type* a_pOp0, BuiltInOperatorDelegate a_Delegate);
    void addOverload(Type* a_pResult, Type* a_pOp0, Type* a_pOp1, BuiltInOperatorDelegate a_Delegate);
    void removeOverload(Type* a_pResult, Type* a_pOp0, Type* a_pOp1, BuiltInOperatorDelegate a_Delegate);
    void fetchExtraOverloads(Type* a_pType, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const;
    void fetchExtraOverloads(MemberPointer* a_pMemberPointer, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const;
    void fetchExtraOverloads(Reference* a_pReference, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const;
    void fetchExtraOverloads(Enum* a_pEnum, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const;
    void fetchExtraOverloads(Class* a_pClass, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const;
    void fetchExtraOverloads(ClassType* a_pClassType, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const;
    void fetchExtraOverloads(PointerType* a_pPointerType, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const;
    void fetchExtraOverloads(Pointer* a_pPointer, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const;
    void fetchExtraOverloads(Array* a_pArray, Overloads& a_Overloads, TypeSet& a_AlreadyTreated) const;
    void fetchExtraOverloads(FunctionPointer* a_pFunctionPointer, Overloads& a_Overloads,
                             TypeSet& a_AlreadyTreated) const;
    void fetchExtraNullptrOverloads(Overloads& a_Overloads) const;

protected:
    Operator     m_eOperator;
    Overloads    m_Overloads;
    OperatorKind m_eType;
    size_t       m_uiOperandCount;
};

} // namespace lang
} // namespace phantom
