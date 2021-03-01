// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/LanguageElement.h>
#include <phantom/lang/LanguageElementVisitorEx.h>
#include <phantom/lang/lang.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents an evaluable element such as language expressions and statements.
class PHANTOM_EXPORT_PHANTOM_CODE Evaluable : public LanguageElement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX;

public:
    Evaluable(uint a_uiFlags = 0);
    Evaluable(Type* a_pEvalType, uint a_uiFlags = 0);
    ~Evaluable();

    void initialize()
    {
        LanguageElement::initialize();
        if (m_pEvalType)
            addReferencedElement(m_pEvalType);
    }

    virtual Evaluable* asEvaluable() const { return (Evaluable*)this; }

    CompiledSource* getCompiledSource() const;

    Semantic* getSemantic() const;

    Language* getLanguage() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Mark this expression as implicit. This is used to ensure good reverse engineering if
    /// needed.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void setImplicit() { addFlags(PHANTOM_R_FLAG_IMPLICIT); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this expression is implicit.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool isImplicit() const { return testFlags(PHANTOM_R_FLAG_IMPLICIT); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Evaluates this evaluable, creating the context for you.
    /// /!\ REMINDER : Evaluation is not intended to be highly efficient in terms of speed or
    /// performances, but convenient and user-friendly (for debuggers or scripting actions)
    /// => To have close-to-native performances, use a JIT compiler
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void eval() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Evaluates this evaluable to a buffer (the evaluable must be non void and return a
    /// value), creating the context for you.
    /// /!\ REMINDER : Evaluation is not intended to be highly efficient in terms of speed or
    /// performances, but convenient and user-friendly (for debuggers or scripting actions)
    /// => To have close-to-native performances, use a JIT compiler
    ///
    /// \param [in,out] a_pResultAddress The buffer receiving the evaluation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void eval(void* a_pResultAddress) const;

    /// \brief  Evaluates this evaluable, providing a context.
    virtual void eval(ExecutionContext& a_Context) const = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Evaluate a given expression's life time (its life scope).
    ///
    /// \param [in,out] a_pExpression   The expression we want to evaluate the life of.
    ///
    /// \return The statement (scope) in which the expression will be 'alive'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Evaluable* evaluateExpressionLifeTime(Expression* a_pExpression) const;

    void addScopedDestruction(Expression* a_pExpression);

    Expressions const& getScopedDestructions() const;
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets this expression's value type.
    ///
    /// \return This expression's value type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getEvalType() const { return m_pEvalType; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this evaluable has a value storage (means once evaluated the value is
    /// stored in memory somewhere).
    ///
    /// \return true if has value storage, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool hasStorage() const { return false; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this evaluable or any sub expression recursively has a value storage
    /// (means once evaluated the value is stored in memory somewhere).
    ///
    /// \return true if has value storage recursively, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasStorageCascade() const;

    ExpressionsView getSubExpressions() const;

    virtual void dump(std::ostream& _out, int a_Lvl = 0) const;

protected:
    virtual void flush(ExecutionContext&) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \internal
    /// \brief  Pushes a temporary value destruction.
    ///
    /// \param [in,out] a_Context   The execution context.
    /// \param [in,out] a_pType     If non-null, the type.
    /// \param [in,out] a_pBuffer   If non-null, the buffer.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void pushTempDestruction(ExecutionContext& a_Context, Type* a_pType, void* a_pBuffer) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Convenient function for internal arguments evaluation (useful in function call
    ///         expressions).
    ///
    /// \param  a_Arguments The arguments' expressions.
    /// \param  a_Addresses The evaluated argument storage addresses.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void evaluateArguments(ExecutionContext& a_Context, ExpressionsView a_Arguments, VoidPtrs& a_Addresses) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a sub expression and clones it if needed (the given expression is not orphan).
    ///
    /// \param [in,out] a_prExpression  [in,out] If non-null, the expression to add (and maybe
    /// clone).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addSubExpression(Expression*& a_prExpression);
    void removeSubExpression(Expression* a_pExp);

protected:
    void               evalScopedDestructions(ExecutionContext& a_Context) const;
    virtual Evaluable* evaluateLifeTime() const { return nullptr; }

private:
    SmallVector<Expression*, 1> m_SubExpressions;
    Type*                       m_pEvalType = nullptr;
    Expressions*                m_pScopedDestructions = nullptr;
};

} // namespace lang
} // namespace phantom
