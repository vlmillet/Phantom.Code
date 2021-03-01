// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Evaluable.h"
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a statement/instruction inside a function block.

class PHANTOM_EXPORT_PHANTOM_CODE Statement : public Evaluable
{
    friend class Block;
    friend class Subroutine;

    PHANTOM_CANCEL_META_CLASS();

public:
    Statement(uint a_uiFlags = 0);
    ~Statement();

    virtual void eval(ExecutionContext& a_Context) const = 0;

    virtual Statement* asStatement() const { return (Statement*)this; }

    Block* getBlock() const;

    Block* getRootBlock() const;

    Subroutine* getSubroutine() const;

    Statement* getNextStatement() const;

    Evaluable* evaluateExpressionLifeTime(Expression*) const override;

    virtual LocalVariableInitializationStatement* asLocalVariableInitializationStatement() const { return nullptr; }
    virtual BranchIfNotStatement*                 asBranchIfNotStatement() const { return nullptr; }
    virtual BranchIfStatement*                    asBranchIfStatement() const { return nullptr; }
    virtual BranchStatement*                      asBranchStatement() const { return nullptr; }
    virtual ReturnStatement*                      asReturnStatement() const { return nullptr; }
    virtual ExpressionStatement*                  asExpressionStatement() const { return nullptr; }
    virtual FieldInitializationStatement*         asFieldInitializationStatement() const { return nullptr; }
    virtual BaseConstructorCallStatement*         asBaseConstructorCallStatement() const { return nullptr; }
    virtual Label*                                asLabel() const { return nullptr; }

    Statement* getNextNonBlockStatement() const;

protected:
    virtual void onAttachedToBlock(Block*);
    virtual void restore() {}
};

} // namespace lang
} // namespace phantom
