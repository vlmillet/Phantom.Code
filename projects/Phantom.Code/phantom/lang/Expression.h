// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Evaluable.h"

#include <phantom/lang/ExecutionContext.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents the base of a programming language expression (aithmetic/bool    operations,
/// function calls, etc...).
class PHANTOM_EXPORT_PHANTOM_CODE Expression : public Evaluable
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

    PHANTOM_CANCEL_META_CLASS();

    friend class Evaluable;
    friend class Statement;

public:
    Expression(Type* a_pValueType, Type* a_pEvalType = nullptr, uint a_uiFlags = 0);

    void initialize()
    {
        Evaluable::initialize();
        if (m_pValueType != getEvalType())
            addReferencedElement(m_pValueType);
    }

    using Evaluable::eval;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  if 'this' is an IdentityExpression, return the underlying expression, else
    /// return'this'
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* removeIdentity() const { return const_cast<Expression*>(this); }
    virtual Expression* removeImplicitIdentity() const { return const_cast<Expression*>(this); }

    template<class ExpDerived>
    ExpDerived const* as() const
    {
        if (this == nullptr)
            return nullptr;
        return ::phantom::Object::Cast<ExpDerived>(this->removeIdentity());
    }

    template<class ExpDerived>
    ExpDerived* as()
    {
        if (this == nullptr)
            return nullptr;
        return ::phantom::Object::Cast<ExpDerived>(this->removeIdentity());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Retrieces underlying symbol or element this expressions refers to (for example a
    /// Field inside a FieldExpression).
    ///
    /// \return nullptr if no underlying symbol or element exists
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual LanguageElement* removeExpression() const { return nullptr; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Context evaluation. The effective one, the one to override to implement your own
    ///         expressions. Call this function whenever you want to eval other expressions in your
    ///         own implementation instead of 'eval(void*)' which is provided for external calls
    ///         purpose.
    ///
    /// \param [in,out] a_Context The execution context.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void eval(ExecutionContext& a_Context) const = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets this expression's value type.
    ///
    /// \return This expression's value type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getValueType() const { return m_pValueType; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Loads the value of this expression (if this expression has a reference type, load
    /// its content
    ///         and not the reference's address).
    ///
    /// \param [in,out] a_pOutputBuffer The output buffer where to set the value to.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void load(void* a_pOutputBuffer) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Loads the value of this expression (if this expression has a reference type, load
    /// its content
    ///         and not the reference's address).
    ///
    /// \param [in,out] a_pOutputBuffer The output buffer where to set the value to.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Loads the value of this expression (if this expression has a reference type, load
    /// its content
    ///         and not the reference's address).
    ///
    /// \param [in,out] a_Context The current evaluation context.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void load(ExecutionContext& a_Context) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Loads the value of this expression to a custom address (if this expression has a
    /// reference type, load its content
    ///         and not the reference's address).
    ///
    /// \param [in,out] a_Context The current evaluation context.
    /// \param [in,out] a_pResultPointer The pointer where to set the evaluation result to.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void load(ExecutionContext& a_Context, void* a_pResultPointer) const
    {
        a_Context.pushResultPointer(a_pResultPointer);
        load(a_Context);
        a_Context.popResultPointer();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Evaluates this expression to a custom address.
    ///
    /// \param [in,out] a_Context The current evaluation context.
    /// \param [in,out] a_pResultPointer The pointer where to set the evaluation result to.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void eval(ExecutionContext& a_Context, void* a_pResultPointer) const
    {
        a_Context.pushResultPointer(a_pResultPointer);
        eval(a_Context);
        a_Context.popResultPointer();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Stores a value to this expression (only if this expression has a reference type).
    ///
    /// \param [in,out] a_pInputBuffer The input buffer where to get the value from.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void store(void const* a_pInputBuffer) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Evaluates the expression and get its value embedded in a Variant object.
    ///
    /// \return .
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Variant get() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the given expression value from a Variant object (only if this expression has a
    /// reference type, else assert).
    ///
    /// \param  v   The Variant containing the new value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void set(const Variant& v);

    virtual Expression* asExpression() const { return (Expression*)this; }

    virtual Expression* toExpression(Expression* a_pOptExpression = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this expression is persistent, i.e if the expression is ensured to be
    /// always
    ///         valid no matter if code change occured at runtime. An expression is generally
    ///         persistent if all of its referenced elements are natives (obviously not changed at
    ///         runtime) and every sub expression persistent.
    ///
    /// \return true if persistent, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool isPersistent() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Evals the expression and returns the storage address (if has storage, see
    /// hasStorage()).
    ///
    /// \return null if it fails, else the storage address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* evalStorage(ExecutionContext& a_Context) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Returns the storage address (if has storage, see hasStorage()).
    ///
    /// \return null if it fails, else storage address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* getStorage() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this expression has an effective address (if expression type is a lvalue
    /// reference type or if expression has a value storage).
    ///
    /// \return true if has effective address, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasEffectiveAddress() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Loads effective address of this expresion, i.e. evaluates its storage address (see
    /// hasEffectiveAddress())
    ///
    /// \return null if it fails, else the effective address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void* loadEffectiveAddress(ExecutionContext& a_Context) const;
    void* loadEffectiveAddress() const;

    virtual void flush(ExecutionContext& a_Context) const;

    virtual bool isCompileTime() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs a C++ compliant dereference operation(*X).
    ///
    /// \return null if it fails, else the dereference expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* dereference(LanguageElement* a_pOwner);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs a C++ compliant address operation (&amp;X).
    ///
    /// \return null if it fails, else the address expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* address(LanguageElement* a_pOwner);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs a C++ compliant array access operation with a constant index (X[0-N]).
    ///
    /// \param  a_uiIndex   The index.
    ///
    /// \return null if it fails, else the address expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* arrayAccess(LanguageElement* a_pOwner, size_t a_uiIndex);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs a C++ compliant array access operation with an expression index (X[expr]).
    ///
    /// \param [in,out] a_pIndexExpression  The index expression.
    ///
    /// \return null if it fails, else the address expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* adjust(LanguageElement* a_pOwner, ptrdiff_t a_iOffset, Type* a_pOutputType = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs a C++ compliant array access operation with an expression index (X[expr]).
    ///
    /// \param [in,out] a_pIndexExpression  The index expression.
    ///
    /// \return null if it fails, else the address expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* arrayAccess(LanguageElement* a_pOwner, Expression* a_pIndexExpression);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs a C++ compliant member access operation.
    ///
    /// \param [in,out] a_pIndexExpression  The index expression.
    ///
    /// \return null if it fails, else the address expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* memberAccess(LanguageElement* a_pOwner, StringView a_Name,
                                     LanguageElements const* a_pTemplateArguments,
                                     Expressions const*      a_pFunctionArguments,
                                     LanguageElement*        a_pContextScope = nullptr);
    virtual Expression* memberAccess(LanguageElement* a_pOwner, StringView a_Name,
                                     Expressions const* a_pFunctionArguments,
                                     LanguageElement*   a_pContextScope = nullptr)
    {
        return memberAccess(a_pOwner, a_Name, nullptr, a_pFunctionArguments, a_pContextScope);
    }
    virtual Expression* memberAccess(LanguageElement* a_pOwner, StringView a_Name,
                                     LanguageElement* a_pContextScope = nullptr)
    {
        return memberAccess(a_pOwner, a_Name, nullptr, nullptr, a_pContextScope);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs a C++ compliant assignment  (X = expr).
    ///
    /// \param [in,out] a_pExpression   The expression to assign.
    ///
    /// \return null if it fails, else the address expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* assign(LanguageElement* a_pOwner, Expression* a_pExpression);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Loads the value as an expression (must be a reference type expression).
    ///
    /// \param [in,out] a_pOutputBuffer The output buffer where to set the value to.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* load(LanguageElement* a_pOwner);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the conversion to the given output type is canonical.
    ///
    /// \param [in,out] a_pOutputType   The output type.
    ///
    /// \return true if a canonical conversion exists (which means reference and qualifiers are
    /// ignored,
    ///         no base offsetting and no numeric conversion. This means the expression can be
    ///         evaluated as argument even if another type was expected).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool hasEvalConversionTo(Type* a_pOutputType) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs an expression type conversion ((Y)X)..
    ///
    /// \param [in,out] a_pOutputType           The output type.
    /// \param  a_eConversionType               (optional) type of the conversion (implicit,
    /// explicit, reinterpret_case, static_cast, const_cast). \param a_eUserDefinedConversionsPolicy
    /// (optional) user defined functions available for conversion (none, implicits, explicits).
    /// \param  a_pContextScope                 (optional) The context scope where the conversion
    /// occurs.
    ///
    /// \return null if it fails, else the converted expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression*         clone(LanguageElement* a_pOwner) const;
    virtual Expression* cloneImpl(LanguageElement* a_pOwner) const = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes any intermediate conversion from this expression to retrieve the original
    /// one.
    ///
    /// \return this expression if no conversion, else the original expression without conversions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* removeConversions() const { return (Expression*)this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Erases any r-value reference expression which embeds the expression
    ///         (for example 'MyClass(5.f)' has implicitely a 'MyClass&&' type, this method will
    ///         erase the '&&' and keep only the underlying construction expression).
    ///
    /// \return this expression if no r-value reference expression embeds the expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* loadRValueReferenceExpression() { return this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes any r-value reference expression which embeds the expression
    ///         (for example 'MyClass(5.f)' has implicitely a 'MyClass&&' type, this method will
    ///         remove the '&&' and keep only the underlying construction expression).
    ///
    /// \return this expression if no r-value reference expression embeds the expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* removeRValueReferenceExpression() { return this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes any l-value const reference expression which embeds an r-value expression
    ///         (for example 'const MyClass& = MyClass(5.f)').
    ///
    /// \return this expression if no r-value reference expression embeds the expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* removeRValueToLValueExpression() { return this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Combines removeRValueReferenceExpression and removeRValueToLValueExpression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Expression* removeRValueStorageExpression() { return this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets/unsets this expression as temporary (a temporary with class will generate a
    /// destruction at the end of its life time).
    ///
    /// \param  a_bValue    true to set as temporary, false to unset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setTemporary(bool a_bValue);

    bool isTemporary() const;

    virtual void dump(std::ostream& a_Out, int a_Lvl) const override;

protected:
    virtual Evaluable*       evaluateExpressionLifeTime(Expression* a_pExpression) const override;
    virtual Evaluable*       evaluateLifeTime() const;
    PHANTOM_FORCEINLINE void addDependencyOn(Symbol*)
    { /* reserved for later use */
    }

protected:
    Type* storageType(Type* a_pType) const;

private:
    void setValue(void const* a_pSrc) const;
    void onAttachedToBlock(Block* a_pBlock);

private:
    Type* m_pValueType;
};

} // namespace lang
} // namespace phantom
