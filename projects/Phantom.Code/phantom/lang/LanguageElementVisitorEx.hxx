#pragma once

// haunt {

// clang-format off

#include "LanguageElementVisitorEx.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/method>
#include <phantom/using>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("LanguageElementVisitorEx")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(LanguageElementVisitorEx)
        {
            this_()
            .inherits<::phantom::lang::LanguageElementVisitor>()
        .public_()
            .using_("LanguageElementVisitor::visit")
            .method<void(AllocateExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ArrayExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BaseConstructorCallStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Block*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BranchIfNotStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BranchIfStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BranchStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BuiltInConversionExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BuiltInOperator*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BuiltInOperatorExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(CallExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ClassListInitializationExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ClassTypeListInitializationExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ConditionalExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ConstantExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ConstructorCallExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ControlStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FieldExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FieldPointerAdjustmentExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FieldPointerExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FieldInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(DeallocateExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(DeleteExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Evaluable*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Expression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ExpressionStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(IdentityExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(InitializerListExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Label*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LoadExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LocalVariableExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LocalVariableInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LValueExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LValueInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MethodPointerCallExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MemCopyStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(NewExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlacementNewExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PointerAdjustmentExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PropertyExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ReturnStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(RValueReferenceExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(RValueToConstLValueExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Statement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(StaticAssert*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(StringLiteralExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(StructureExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(SubroutinePointerExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(SymbolReferenceExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(BinaryOperationExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(UnaryPostOperationExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(UnaryPreOperationExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantDeclType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantTypeOfExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemporaryObjectDestructionExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TypeTraitExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VarArgCallExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VariableExpression*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VirtualMethodTableSetupStatement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(WStringLiteralExpression*, VisitorData), virtual_>("visit", &_::visit)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LanguageElementVisitorEx")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
