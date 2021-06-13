// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "plugin.h"

#include <phantom/fwd.h>
#include <phantom/lang/reflection_fwd.h>
#include <phantom/utils/String.h>

HAUNT_PACKAGE("phantom.lang")
namespace phantom
{
namespace lang
{
PHANTOM_DECL_LISTABLE(BuiltInOperator);
PHANTOM_DECL_LISTABLE(ExpressionStatement);
PHANTOM_DECL_LISTABLE(Expression);
PHANTOM_DECL_LISTABLE(Statement, 20);
typedef ArrayView<Expression*> ExpressionsView;
PHANTOM_DECL_LISTABLE(DebugInformation);
PHANTOM_DECL_LISTABLE(Evaluable);
PHANTOM_DECL_LISTABLE(Block);
PHANTOM_DECL_LISTABLE(Label);
class CppLite;

class Expression;
class ExpressionStatement;
class Evaluable;
class Statement;
class Block;
class BuiltInOperator;
struct Conversion;
class ConversionResults;

// Specialized Statements
class Label;
class BranchIfStatement;
class BranchIfNotStatement;
class BranchStatement;
class MemCopyStatement;
class ControlStatement;

// Specialized Expressions
class InitializerListExpression;
class ClassListInitializationExpression;
class ClassTypeListInitializationExpression;
class IdentityExpression;
class BuiltInOperatorExpression;
class LValueExpression;
class FieldPointerAdjustmentExpression;
class FieldInitializationStatement;
class PointerAdjustmentExpression;
class TypeTraitExpression;
class LoadExpression;
class ReturnStatement;
class CallExpression;
class BuiltInConversionExpression;
class CommaExpression;
class ConstantExpression;
class BinaryOperationExpression;
class UnaryPreOperationExpression;
class UnaryPostOperationExpression;
class SubroutinePointerExpression;
class SymbolReferenceExpression;
class LocalVariableExpression;
class LocalVariableInitializationStatement;
class LValueInitializationStatement;
class BaseConstructorCallStatement;
class VirtualMethodTableSetupStatement;
class FieldExpression;
class PropertyExpression;
class VariableExpression;
class StringLiteralExpression;
class WStringLiteralExpression;
class ConstructorCallExpression;
class PlacementNewExpression;
class DataExpression;
class ConditionalExpression;
class MethodPointerCallExpression;
class FieldPointerExpression;
class RValueToConstLValueExpression;
class RValueReferenceExpression;
class NewExpression;
class DeleteExpression;
class AllocateExpression;
class DeallocateExpression;
class ArrayExpression;
class StructureExpression;
class StaticAssert;
class VarArgCallExpression;
class TemporaryObjectDestructionExpression;

// Template Dependant
class TemplateDependantArray;
class TemplateDependantDeclType;
class TemplateDependantElement;
class TemplateDependantExpression;
class TemplateDependantTemplateInstance;
class TemplateDependantType;
class TemplateDependantTypeOfExpression;
class TemplateParameterPackExpansion;
class TemplateParameterPackTypeExpansion;
class TemplateParameterPackExpressionExpansion;
} // namespace lang

namespace lang
{
PHANTOM_DECL_LISTABLE(CompiledSource);
PHANTOM_DECL_LISTABLE(Project);
PHANTOM_DECL_LISTABLE(Solution);
PHANTOM_DECL_LISTABLE(Message);
class Compiler;
class Debugger;
struct SolutionData;
struct ProjectData;
PHANTOM_DECL_LISTABLE(Language);

using Options = SmallMap<String, String>;

using BuildSessionId = int;

} // namespace lang
} // namespace phantom
HAUNT_END("phantom.lang")
