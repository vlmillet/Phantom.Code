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
            .method<void(AllocateExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ArrayExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BaseConstructorCallStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Block*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BranchIfNotStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BranchIfStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BranchStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BuiltInConversionExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BuiltInOperator*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BuiltInOperatorExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(CallExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ClassListInitializationExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ClassTypeListInitializationExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(CommaExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ConditionalExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ConstantExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ConstructorCallExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ControlStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(FieldExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(FieldPointerAdjustmentExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(FieldPointerExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(FieldInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(DeallocateExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(DeleteExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Evaluable*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Expression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ExpressionStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(IdentityExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(InitializerListExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Label*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LoadExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LocalVariableExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LocalVariableInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LValueExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(LValueInitializationStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(MethodPointerCallExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(MemCopyStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(NewExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PlacementNewExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PointerAdjustmentExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(PropertyExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(ReturnStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(RValueReferenceExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(RValueToConstLValueExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(Statement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(StaticAssert*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(StringLiteralExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(StructureExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(SubroutinePointerExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(SymbolReferenceExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(BinaryOperationExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(UnaryPostOperationExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(UnaryPreOperationExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateDependantDeclType*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateDependantExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateDependantTypeOfExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateParameterPackExpansion*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateParameterPackTypeExpansion*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemplateParameterPackExpressionExpansion*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TemporaryObjectDestructionExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(TypeTraitExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(VarArgCallExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(VariableExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(VirtualMethodTableSetupStatement*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
            .method<void(WStringLiteralExpression*, VisitorData), virtual_>("visit", &_::visit)({"a_pInput","a_Data"})
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
