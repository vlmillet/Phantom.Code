// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "fwd.h"

#include <phantom/lang/LanguageElementVisitor.h>
/* **************** Declarations ***************** */
/* *********************************************** */

#define PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT_EX PHANTOM_REGISTER_FOR_VISIT(phantom::lang::LanguageElementVisitorEx)

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM_CODE LanguageElementVisitorEx : public LanguageElementVisitor
{
public:
    using LanguageElementVisitor::visit;
    virtual void visit(AllocateExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ArrayExpression* a_pInput, VisitorData a_Data);
    virtual void visit(BaseConstructorCallStatement* a_pInput, VisitorData a_Data);
    virtual void visit(Block* a_pInput, VisitorData a_Data);
    virtual void visit(BranchIfNotStatement* a_pInput, VisitorData a_Data);
    virtual void visit(BranchIfStatement* a_pInput, VisitorData a_Data);
    virtual void visit(BranchStatement* a_pInput, VisitorData a_Data);
    virtual void visit(BuiltInConversionExpression* a_pInput, VisitorData a_Data);
    virtual void visit(BuiltInOperator* a_pInput, VisitorData a_Data);
    virtual void visit(BuiltInOperatorExpression* a_pInput, VisitorData a_Data);
    virtual void visit(CallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ClassListInitializationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ClassTypeListInitializationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(CommaExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ConditionalExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ConstantExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ConstructorCallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ControlStatement* a_pInput, VisitorData a_Data);
    virtual void visit(FieldExpression* a_pInput, VisitorData a_Data);
    virtual void visit(FieldPointerAdjustmentExpression* a_pInput, VisitorData a_Data);
    virtual void visit(FieldPointerExpression* a_pInput, VisitorData a_Data);
    virtual void visit(FieldInitializationStatement* a_pInput, VisitorData a_Data);
    virtual void visit(DeallocateExpression* a_pInput, VisitorData a_Data);
    virtual void visit(DeleteExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Evaluable* a_pInput, VisitorData a_Data);
    virtual void visit(Expression* a_pInput, VisitorData a_Data);
    virtual void visit(ExpressionStatement* a_pInput, VisitorData a_Data);
    virtual void visit(IdentityExpression* a_pInput, VisitorData a_Data);
    virtual void visit(InitializerListExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Label* a_pInput, VisitorData a_Data);
    virtual void visit(LoadExpression* a_pInput, VisitorData a_Data);
    virtual void visit(LocalVariableExpression* a_pInput, VisitorData a_Data);
    virtual void visit(LocalVariableInitializationStatement* a_pInput, VisitorData a_Data);
    virtual void visit(LValueExpression* a_pInput, VisitorData a_Data);
    virtual void visit(LValueInitializationStatement* a_pInput, VisitorData a_Data);
    virtual void visit(MethodPointerCallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(MemCopyStatement* a_pInput, VisitorData a_Data);
    virtual void visit(NewExpression* a_pInput, VisitorData a_Data);
    virtual void visit(PlacementNewExpression* a_pInput, VisitorData a_Data);
    virtual void visit(PointerAdjustmentExpression* a_pInput, VisitorData a_Data);
    virtual void visit(PropertyExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ReturnStatement* a_pInput, VisitorData a_Data);
    virtual void visit(RValueReferenceExpression* a_pInput, VisitorData a_Data);
    virtual void visit(RValueToConstLValueExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Statement* a_pInput, VisitorData a_Data);
    virtual void visit(StaticAssert* a_pInput, VisitorData a_Data);
    virtual void visit(StringLiteralExpression* a_pInput, VisitorData a_Data);
    virtual void visit(StructureExpression* a_pInput, VisitorData a_Data);
    virtual void visit(SubroutinePointerExpression* a_pInput, VisitorData a_Data);
    virtual void visit(SymbolReferenceExpression* a_pInput, VisitorData a_Data);
    virtual void visit(BinaryOperationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(UnaryPostOperationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(UnaryPreOperationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantDeclType* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantExpression* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantTypeOfExpression* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateParameterPackExpansion* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateParameterPackTypeExpansion* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateParameterPackExpressionExpansion* a_pInput, VisitorData a_Data);
    virtual void visit(ParameterPackExpressionExpansion* a_pInput, VisitorData a_Data);
    virtual void visit(TemporaryObjectDestructionExpression* a_pInput, VisitorData a_Data);
    virtual void visit(TypeTraitExpression* a_pInput, VisitorData a_Data);
    virtual void visit(VarArgCallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(VariableExpression* a_pInput, VisitorData a_Data);
    virtual void visit(VirtualMethodTableSetupStatement* a_pInput, VisitorData a_Data);
    virtual void visit(WStringLiteralExpression* a_pInput, VisitorData a_Data);
};

} // namespace lang
} // namespace phantom
