// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "LanguageElementVisitorEx.h"

#include "AllocateExpression.h"
#include "ArrayExpression.h"
#include "BaseConstructorCallStatement.h"
#include "BinaryOperationExpression.h"
#include "Block.h"
#include "BranchIfNotStatement.h"
#include "BranchIfStatement.h"
#include "BranchStatement.h"
#include "BuiltInConversionExpression.h"
#include "BuiltInOperator.h"
#include "BuiltInOperatorExpression.h"
#include "CallExpression.h"
#include "ClassListInitializationExpression.h"
#include "ClassTypeListInitializationExpression.h"
#include "ConditionalExpression.h"
#include "ConstantExpression.h"
#include "ConstructorCallExpression.h"
#include "DeallocateExpression.h"
#include "DeleteExpression.h"
#include "Evaluable.h"
#include "Expression.h"
#include "ExpressionStatement.h"
#include "FieldExpression.h"
#include "FieldInitializationStatement.h"
#include "FieldPointerAdjustmentExpression.h"
#include "FieldPointerExpression.h"
#include "IdentityExpression.h"
#include "InitializerListExpression.h"
#include "LValueExpression.h"
#include "LValueInitializationStatement.h"
#include "Label.h"
#include "LoadExpression.h"
#include "LocalVariableExpression.h"
#include "LocalVariableInitializationStatement.h"
#include "MemCopyStatement.h"
#include "MethodPointerCallExpression.h"
#include "NewExpression.h"
#include "PlacementNewExpression.h"
#include "PointerAdjustmentExpression.h"
#include "PropertyExpression.h"
#include "RValueReferenceExpression.h"
#include "RValueToConstLValueExpression.h"
#include "ReturnStatement.h"
#include "Statement.h"
#include "StaticAssert.h"
#include "StringLiteralExpression.h"
#include "StructureExpression.h"
#include "SubroutinePointerExpression.h"
#include "SymbolReferenceExpression.h"
#include "TemplateDependantDeclType.h"
#include "TemplateDependantExpression.h"
#include "TemplateDependantTypeOfExpression.h"
#include "TemporaryObjectDestructionExpression.h"
#include "TypeTraitExpression.h"
#include "UnaryPostOperationExpression.h"
#include "UnaryPreOperationExpression.h"
#include "VarArgCallExpression.h"
#include "VariableExpression.h"
#include "VirtualMethodTableSetupStatement.h"
#include "WStringLiteralExpression.h"

namespace phantom
{
namespace lang
{
void LanguageElementVisitorEx::visit(AllocateExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(ArrayExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(BaseConstructorCallStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(Block* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(BranchIfNotStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<BranchStatement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(BranchIfStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<BranchStatement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(BranchStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(BuiltInConversionExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(BuiltInOperator* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(BuiltInOperatorExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(CallExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(ClassListInitializationExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(ClassTypeListInitializationExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(ConditionalExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(ConstantExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(ConstructorCallExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(ControlStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(FieldExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(FieldPointerAdjustmentExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(FieldPointerExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(FieldInitializationStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(DeallocateExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(DeleteExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(Evaluable* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(Expression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Evaluable*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(ExpressionStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(IdentityExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(InitializerListExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(Label* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(LoadExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(LocalVariableExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(LocalVariableInitializationStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(LValueExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(LValueInitializationStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(MethodPointerCallExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(MemCopyStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(NewExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(PlacementNewExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(PointerAdjustmentExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(PropertyExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(ReturnStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(RValueReferenceExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(RValueToConstLValueExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(Statement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Evaluable*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(StaticAssert* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(StringLiteralExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(StructureExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(SubroutinePointerExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(SymbolReferenceExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(BinaryOperationExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(UnaryPostOperationExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(UnaryPreOperationExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(TemplateDependantDeclType* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(TemplateDependantExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(TemplateDependantTypeOfExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(TemporaryObjectDestructionExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(TypeTraitExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(VarArgCallExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(VariableExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(VirtualMethodTableSetupStatement* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Statement*>(a_pInput), a_Data);
}
void LanguageElementVisitorEx::visit(WStringLiteralExpression* a_pInput, VisitorData a_Data)
{
    return visit(static_cast<Expression*>(a_pInput), a_Data);
}

} // namespace lang
} // namespace phantom
