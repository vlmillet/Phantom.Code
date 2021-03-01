// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Expression.h"
#include "LanguageElementVisitorEx.h"
#include "lang.h"

#include <phantom/lang/LanguageElement.h>
#include <phantom/lang/Source.h>
#include <phantom/lang/TypeIndex.h>
#include <phantom/utils/ArrayView.h>
#include <phantom/utils/Delegate.h>
#include <phantom/utils/OptionalArrayView.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
struct NumericConversion;

enum EValueCategory
{
    e_prvalue,
    e_rvalue,
    e_glvalue,
    e_lvalue,
    e_xvalue,
};

class PHANTOM_EXPORT_PHANTOM_CODE Semantic : public Object, public LanguageElementVisitorEx
{
    friend class LanguageElement;
    friend class Source;
    friend class SemanticPrivate;

public:
    enum EClassBuildState
    {
        e_ClassBuildState_None,
        e_ClassBuildState_Inheritance,
        e_ClassBuildState_Members,
        e_ClassBuildState_Sized,
        e_ClassBuildState_States,
        e_ClassBuildState_ImplicitsAndDefaults,
        e_ClassBuildState_Blocks
    };
    enum
    {
        e_ClassBuildState_Count = e_ClassBuildState_Blocks + 1
    };

    enum ERefType
    {
        e_RefType_None,
        e_RefType_LValue,
        e_RefType_RValue,
    };

    enum
    {
        e_VisitorFlag_None = 0x0,
        e_VisitorFlag_KeepAliases = 0x1,
    };

    Semantic(Source* a_pSource, Message* a_pMessage);
    ~Semantic(void);

    template<class T, class... Args>
    T* New(Args&&... a_Args)
    {
        return m_pSource->New<T>(std::forward<Args>(a_Args)...);
    }

    void Delete(LanguageElement* a_pElement) { m_pSource->Delete(a_pElement); }

    template<class T, class... Args>
    T* new_(Args&&... a_Args)
    {
        return m_pSource->new_<T>(std::forward<Args>(a_Args)...);
    }
    template<class T, class... Args>
    T* newConv(Args&&... a_Args)
    {
        return m_pSource->new_<T>(m_pSource, std::forward<Args>(a_Args)...);
    }

    template<class T>
    Constant* newConstant(T v)
    {
        return Constant::Create<T>(getSource(), v);
    }

    template<class T>
    void delete_(TypeIndentityT<T*> a_pPtr)
    {
        m_pSource->delete_<T>(a_pPtr);
    }
    template<class T>
    void deleteVirtual(T* a_pPtr)
    {
        m_pSource->deleteVirtual(a_pPtr);
    }

    void setStandardLookup(bool a_bStandardLookup) { m_bStandardLookup = a_bStandardLookup; }
    bool isStandardLookup() const { return m_bStandardLookup; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a built in binary operator from its name (ex : +=, %, +, ...).
    ///
    /// \param  a_strName   The operator' name.
    ///
    /// \return null if it fails, else the built in binary operator.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    BuiltInOperator* getBuiltInBinaryOperator(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a built in pre unary operator from its name (ex : ++, -, *, & ...).
    ///
    /// \param  a_strName   The operator's name.
    ///
    /// \return null if it fails, else the built in pre unary operator.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    BuiltInOperator* getBuiltInPreUnaryOperator(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a built in post unary operator from its name (ex : ++, --, ...).
    ///
    /// \param  a_strName   The operator's name.
    ///
    /// \return null if it fails, else the built in post unary operator.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    BuiltInOperator* getBuiltInPostUnaryOperator(StringView a_strName) const;

    /// \brief  Visitor functions identifiers.
    enum EVisitorFunction
    {
        e_VisitorFunction_TypeConversion,
        e_VisitorFunction_ExpressionConversion,
        e_VisitorFunction_ToExpression,
        e_VisitorFunction_CreateCallExpression,
        e_VisitorFunction_SubroutineResolution,
        e_VisitorFunction_UnqualifiedLookup,
        e_VisitorFunction_TemplateArgumentDeduction,
        e_VisitorFunction_CallTemplateArgumentDeduction,
        e_VisitorFunction_QualifiedLookup,
        e_VisitorFunction_UnqualifiedNameLookup,
        e_VisitorFunction_QualifiedNameLookup,
        e_VisitorFunction_Access,
        e_VisitorFunction_IsViableCallCandidate,
        e_VisitorFunction_IsViableSignatureCandidate,
        e_VisitorFunction_InstanciateTemplateElement,
        e_VisitorFunction_ResolveTemplateDependencies,
        e_VisitorFunction_BinaryOperator,
        e_VisitorFunction_UnaryPreOperator,
        e_VisitorFunction_UnaryPostOperator,
        e_VisitorFunction_BuildClass,
        e_VisitorFunction_ComputeSizeAndAlignment,
        e_VisitorFunction_HasSideEffect,
    };

    struct NameLookupData
    {
        StringView       name;
        LanguageElement* scope = nullptr;
        Access           access;
        Symbols          outSymbols;
    };

    struct UnqualifiedLookupData : NameLookupData
    {
        OptionalArrayView<LanguageElement*> templateArguments;
        OptionalArrayView<Expression*>      functionArguments;
        LanguageElement**                   result = nullptr;
    };

    struct QualifiedLookupData : UnqualifiedLookupData
    {
        LanguageElement* lhs = nullptr;
    };

    enum EError
    {
        e_ErrorOK,
        e_Error_NoConversion,
        e_Error_NoOverload,
        e_Error_NoMember,
        e_Error_NoBuiltinOperator,
        e_Error_NoBinaryOperator,
        e_Error_NoUnaryPrefixOperator,
        e_Error_NoUnaryPostfixOperator,
        e_Error_NoConstructor,
        e_Error_NoDefaultConstructor,
        e_Error_NoTemplateDeclaration,
        e_Error_NoTemplateSpecialization,
        e_Error_ExpectedTemplateSignature,
        e_Error_ReferencesCannotBeAllocated,
        e_Error_ReferencesMustBeInitialized,
        e_Error_UnknownBuiltinOperator,
        e_Error_AmbiguousCallToMultipleBuiltinOperator,
        e_Error_AmbiguousAccessToMultipleMember,
        e_Error_AmbiguousAccessToMultipleConstructor,
        e_Error_AmbiguousCallToMultipleOverloads,
        e_Error_TemplateDependantElementIsNotAnExpression,
        e_Error_InaccessibleSymbol,
        e_Error_UnexpectedMember,
        e_Error_UnexpectedTemplateArgumentList,
        e_Error_TooManyTemplateArguments,
        e_Error_NotEnoughTemplateArguments,
        e_Error_TemplateInstantiationFailure,
        e_Error_TemplateResolutionFailure,
        e_Error_InstantiationOfUndefinedTemplateSpecialization,
        e_Error_ExpectedExpression,
        e_Error_StateMachineAlreadyDefinedInBase,
        e_Error_QualifiedLookupFailure,
        e_Error_UnqualifiedLookupFailure,
        e_Error_QualifiedSymbolsLookupFailure,
        e_Error_UnqualifiedSymbolsLookupFailure,
        e_Error_TooManyFunctionArguments,
        e_Error_NotEnoughFunctionArguments,
        e_Error_MissingMethodCaller,
        e_Error_ExpectedMemberPointerExpression,
        e_Error_FunctionConflictsWithPreviouslyDeclared,
        e_Error_SymbolAlreadyExistsWithName,
        e_Error_ConstructorAlreadyDefined,
        e_Error_TypeCannotHaveQualifiers,
        e_Error_TypeCannotHavePointer,
        e_Error_TypeCannotHaveLValueReference,
        e_Error_TemplateSubroutineLookupFailure,
        e_Error_ExpectedType,
        e_Error_ExpectedClassType,
        e_Error_ExpectedClass,
        e_Error_ExpectedConstant,
        e_Error_IncompleteType,
        e_Error_IllegalAutoArray,
        e_Error_EmptyInitializerList,
        e_Error_IllegalExplicitArrayConversion,
        e_Error_IllegalInheritedConstructorInAggregateInitialization,
        e_Error_IllegalExplicitConstructorInAggregateInitialization,
        e_Error_IllegalNonImplicitConstructorInAggregateInitialization,
        e_Error_IllegalNonPublicFieldInAggregateInitialization,
        e_Error_IllegalNonPublicBaseClassInAggregateInitialization,
        e_Error_IllegalVirtualFunctionInAggregateInitialization,
        e_Error_CannotInstantiateAbstractClass,
        e_Error_StaticAssertFailed
    };

    static const char* DefaultErrorText(EError a_eError);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates a 'nullptr' constant expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ConstantExpression* createNullptrExpression();

    Expression* createZeroInitExpression(Type* pType);

    typedef Delegate<void(CodeRangeLocation const&, const char*)> ErrorDelegate;

    Message* getMessage() const { return m_pMessage; }

    typedef Delegate<void(LanguageElement*, StringBuffer& a_Buffer)> NameFormatDelegate;

    void setNameFormatDelegate(NameFormatDelegate a_Delegate) { m_NameFormatDelegate = a_Delegate; }

    LocalVariable*    createLocalVariable(Type* a_pValueType, StringView a_Name, Modifiers a_Modifiers = PHANTOM_R_NONE,
                                          Modifiers a_Flags = PHANTOM_R_NONE);
    DeleteExpression* createDeleteExpression(Class* a_pClass, Expression* a_pDeletedExpression,
                                             LanguageElement* a_pScope);
    DeleteExpression* createDeleteExpression(Expression* a_pDeletedExpression, bool a_bRTTIBasedDelete,
                                             LanguageElement* a_pScope);

    /// ------------------------------- VISIT -----------------------------------

    virtual void visit(Alias* a_pInput, VisitorData a_Data);
    virtual void visit(AllocateExpression* a_pInput, VisitorData a_Data);
    virtual void visit(AnonymousSection* a_pInput, VisitorData a_Data);
    virtual void visit(AnonymousStruct* a_pInput, VisitorData a_Data);
    virtual void visit(AnonymousUnion* a_pInput, VisitorData a_Data);
    virtual void visit(Application* a_pInput, VisitorData a_Data);
    virtual void visit(ArrayExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Array* a_pInput, VisitorData a_Data);
    virtual void visit(BaseConstructorCallStatement* a_pInput, VisitorData a_Data);
    virtual void visit(Block* a_pInput, VisitorData a_Data);
    virtual void visit(BranchIfNotStatement* a_pInput, VisitorData a_Data);
    virtual void visit(BranchIfStatement* a_pInput, VisitorData a_Data);
    virtual void visit(BranchStatement* a_pInput, VisitorData a_Data);
    virtual void visit(BuiltInConversionExpression* a_pInput, VisitorData a_Data);
    virtual void visit(BuiltInOperator* a_pInput, VisitorData a_Data);
    virtual void visit(BuiltInOperatorExpression* a_pInput, VisitorData a_Data);
    virtual void visit(CallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Class* a_pInput, VisitorData a_Data);
    virtual void visit(ClassType* a_pInput, VisitorData a_Data);
    virtual void visit(ConditionalExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Constant* a_pInput, VisitorData a_Data);
    virtual void visit(ConstantExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Constructor* a_pInput, VisitorData a_Data);
    virtual void visit(ConstructorCallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(ConstType* a_pInput, VisitorData a_Data);
    virtual void visit(ConstVolatileType* a_pInput, VisitorData a_Data);
    virtual void visit(ContainerClass* a_pInput, VisitorData a_Data);
    virtual void visit(Field* a_pInput, VisitorData a_Data);
    virtual void visit(FieldExpression* a_pInput, VisitorData a_Data);
    virtual void visit(FieldInitializationStatement* a_pInput, VisitorData a_Data);
    virtual void visit(FieldPointerExpression* a_pInput, VisitorData a_Data);
    virtual void visit(FieldPointer* a_pInput, VisitorData a_Data);
    virtual void visit(Pointer* a_pInput, VisitorData a_Data);
    virtual void visit(DeallocateExpression* a_pInput, VisitorData a_Data);
    virtual void visit(DeleteExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Ellipsis* a_pInput, VisitorData a_Data);
    virtual void visit(Enum* a_pInput, VisitorData a_Data);
    virtual void visit(Evaluable* a_pInput, VisitorData a_Data);
    virtual void visit(Expression* a_pInput, VisitorData a_Data);
    virtual void visit(ExpressionStatement* a_pInput, VisitorData a_Data);
    virtual void visit(Function* a_pInput, VisitorData a_Data);
    virtual void visit(FunctionPointer* a_pInput, VisitorData a_Data);
    virtual void visit(FunctionType* a_pInput, VisitorData a_Data);
    virtual void visit(IdentityExpression* a_pInput, VisitorData a_Data);
    virtual void visit(InitializerListType* a_pInput, VisitorData a_Data);
    virtual void visit(InitializerListExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Label* a_pInput, VisitorData a_Data);
    virtual void visit(LanguageElement* a_pInput, VisitorData a_Data);
    virtual void visit(LoadExpression* a_pInput, VisitorData a_Data);
    virtual void visit(LocalVariable* a_pInput, VisitorData a_Data);
    virtual void visit(LocalVariableExpression* a_pInput, VisitorData a_Data);
    virtual void visit(LocalVariableInitializationStatement* a_pInput, VisitorData a_Data);
    virtual void visit(LValueExpression* a_pInput, VisitorData a_Data);
    virtual void visit(LValueInitializationStatement* a_pInput, VisitorData a_Data);
    virtual void visit(LValueReference* a_pInput, VisitorData a_Data);
    virtual void visit(MemberAnonymousSection* a_pInput, VisitorData a_Data);
    virtual void visit(MemberAnonymousStruct* a_pInput, VisitorData a_Data);
    virtual void visit(MemberAnonymousUnion* a_pInput, VisitorData a_Data);
    virtual void visit(Method* a_pInput, VisitorData a_Data);
    virtual void visit(MethodPointerCallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(MethodPointer* a_pInput, VisitorData a_Data);
    virtual void visit(MemberPointer* a_pInput, VisitorData a_Data);
    virtual void visit(Module* a_pInput, VisitorData a_Data);
    virtual void visit(Symbol* a_pInput, VisitorData a_Data);
    virtual void visit(SymbolReferenceExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Namespace* a_pInput, VisitorData a_Data);
    virtual void visit(NewExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Package* a_pInput, VisitorData a_Data);
    virtual void visit(PackageFolder* a_pInput, VisitorData a_Data);
    virtual void visit(Parameter* a_pInput, VisitorData a_Data);
    virtual void visit(Placeholder* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderClass* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderClassType* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderConstant* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderTemplate* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderType* a_pInput, VisitorData a_Data);
    virtual void visit(PlacementNewExpression* a_pInput, VisitorData a_Data);
    virtual void visit(PrimitiveType* a_pInput, VisitorData a_Data);
    virtual void visit(Property* a_pInput, VisitorData a_Data);
    virtual void visit(PropertyExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Reference* a_pInput, VisitorData a_Data);
    virtual void visit(ReturnStatement* a_pInput, VisitorData a_Data);
    virtual void visit(RValueReference* a_pInput, VisitorData a_Data);
    virtual void visit(RValueToConstLValueExpression* a_pInput, VisitorData a_Data);
    virtual void visit(RValueReferenceExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Scope* a_pInput, VisitorData a_Data);
    virtual void visit(SequentialContainerClass* a_pInput, VisitorData a_Data);
    virtual void visit(SetClass* a_pInput, VisitorData a_Data);
    virtual void visit(VectorClass* a_pInput, VisitorData a_Data);
    virtual void visit(MapClass* a_pInput, VisitorData a_Data);
    virtual void visit(Signature* a_pInput, VisitorData a_Data);
    virtual void visit(Source* a_pInput, VisitorData a_Data);
    virtual void visit(Statement* a_pInput, VisitorData a_Data);
    virtual void visit(StaticAssert* a_pInput, VisitorData a_Data);
    virtual void visit(StringLiteralExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Structure* a_pInput, VisitorData a_Data);
    virtual void visit(StructureExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Subroutine* a_pInput, VisitorData a_Data);
    virtual void visit(SubroutinePointerExpression* a_pInput, VisitorData a_Data);
    virtual void visit(BinaryOperationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(UnaryPostOperationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(UnaryPreOperationExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Template* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantArray* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantDeclType* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantElement* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantExpression* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantTemplateInstance* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantType* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantTypeOfExpression* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateParameter* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateSignature* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateSpecialization* a_pInput, VisitorData a_Data);
    virtual void visit(Type* a_pInput, VisitorData a_Data);
    virtual void visit(TypeTraitExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Union* a_pInput, VisitorData a_Data);
    virtual void visit(ValueMember* a_pInput, VisitorData a_Data);
    virtual void visit(VarArgCallExpression* a_pInput, VisitorData a_Data);
    virtual void visit(Variable* a_pInput, VisitorData a_Data);
    virtual void visit(VariableExpression* a_pInput, VisitorData a_Data);
    virtual void visit(VirtualMethodTable* a_pInput, VisitorData a_Data);
    virtual void visit(VolatileType* a_pInput, VisitorData a_Data);

    virtual void visit(FundamentalTypeT<void>* a_pInput, VisitorData a_Data);
    virtual void visit(FundamentalTypeT<std::nullptr_t>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<bool>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<char>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<uchar>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<schar>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<short>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<ushort>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<int>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<uint>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<long>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<ulong>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<longlong>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<ulonglong>* a_pInput, VisitorData a_Data);
    virtual void visit(FloatingPointTypeT<float>* a_pInput, VisitorData a_Data);
    virtual void visit(FloatingPointTypeT<double>* a_pInput, VisitorData a_Data);
    virtual void visit(FloatingPointTypeT<longdouble>* a_pInput, VisitorData a_Data);

    PHANTOM_IF_WCHAR_T(virtual void visit(IntegralTypeT<wchar_t>* a_pInput, VisitorData a_Data););
    PHANTOM_IF_CHAR16_T(virtual void visit(IntegralTypeT<char16_t>* a_pInput, VisitorData a_Data););
    PHANTOM_IF_CHAR32_T(virtual void visit(IntegralTypeT<char32_t>* a_pInput, VisitorData a_Data););

    void        generateImplicitCopyAssignmentOperatorCode(Class* a_pClass);
    void        generateImplicitCopyConstructorCode(Class* a_pClass);
    void        generateImplicitMoveConstructorCode(Class* a_pClass);
    void        generateImplicitMoveAssignmentOperatorCode(Class* a_pClass);
    void        generateImplicitDefaultConstructorCode(Class* a_pClass);
    Expression* createDestructionExpression(Type* a_pType, Expression* a_pObjExpression, LanguageElement* a_pScope);
    Expression* createDestructionExpression(Field* a_pDM, Expression* a_pObjExpression, LanguageElement* a_pScope);
    void        generateImplicitDestructorCode(Class* a_pClass);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant function call candidates selection.
    ///
    /// \param  inoutViableCandidates           Receives viable candidates (if any found during selection).
    /// \param  candidates                      The candidates.
    /// \param  a_pTemplateArguments            The optional template arguments.
    /// \param  a_Arguments                     The arguments.
    /// \param [in,out] a_pSelectedCandidate    [in,out] The selected candidate, or null if none or more than one.
    /// \param  a_strName                       The function name.
    /// \param [in,out] a_pContextScope         (optional) The call context scope.
    /// \param [in,out] a_pInitializationType   (optional) The type of the value initialized by this
    ///                                         call.
    /// \param  a_Modifiers                     (optional) The call modifiers (0 or
    /// PHANTOM_R_VIRTUAL). \param  a_uiFlags                       (optional) the flags.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void selectCallCandidate(Subroutines& outViableCandidates, Symbols& candidates,
                             OptionalArrayView<LanguageElement*> a_pTemplateArguments, Expressions& a_Arguments,
                             LanguageElement*& a_pSelectedCandidate, StringView a_strName,
                             LanguageElement* a_pContextScope = nullptr, Type* a_pInitializationType = nullptr,
                             Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Solve C++ binary operator.
    ///
    ///         \code{.cpp}
    ///                    (a &lt; b)
    ///                    (a += (a + b))
    ///                    ...
    ///         \endcode.
    ///
    /// \param  a_strOp         The operator id.
    /// \param  a_pLeftOperand  The left operand.
    /// \param  a_pRightOperand The right operand.
    /// \param  a_pContextScope (optional) The context scope.
    ///
    /// \return null if it fails, else an expression representing the operation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* solveBinaryOperator(StringView a_strOp, Expression* a_pLeftOperand, Expression* a_pRightOperand,
                                    LanguageElement* a_pContextScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Solve C++ binary operator.
    ///
    ///         \code{.cpp}
    ///                    (a &lt; b)
    ///                    (a += (a + b))
    ///                    ...
    ///         \endcode.
    ///
    /// \param  a_eOp               The operator id.
    /// \param [in,out] a_pLeft     The left operand.
    /// \param [in,out] a_pRight    The right operand.
    /// \param  a_pContextScope     (optional) The context scope.
    ///
    /// \return null if it fails, else an expression representing the operation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* solveBinaryOperator(Operator a_eOp, Expression* a_pLeft, Expression* a_pRight,
                                    LanguageElement* a_pContextScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Solve C++ unary pre operator.
    ///
    ///         \code{.cpp}
    ///                    ++a;
    ///                    &amp;a
    ///                    ...
    ///         \endcode.
    ///
    /// \param  a_strOp         The operator name.
    /// \param  a_pOperand      The operand.
    /// \param  a_pContextScope (optional) [in,out] If non-null, the context scope.
    ///
    /// \return null if it fails, else an expression representing the operation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* solveUnaryPreOperator(StringView a_strOp, Expression* a_pOperand,
                                      LanguageElement* a_pContextScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Solve C++ unary pre operator.
    ///
    ///         \code{.cpp}
    ///                    ++a;
    ///                    &amp;a
    ///                    ...
    ///         \endcode.
    ///
    /// \param  a_eOp       The operator id.
    /// \param  a_pOperand  The operand.
    /// \param  a_pScope    (optional) The context scope.
    ///
    /// \return null if it fails, else an expression representing the operation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* solveUnaryPreOperator(Operator a_eOp, Expression* a_pOperand, LanguageElement* a_pScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Solve C++ unary post operator.
    ///
    ///         \code{.cpp}
    ///                    a--;
    ///                    ...
    ///         \endcode.
    ///
    /// \param  a_strOp         The operator name.
    /// \param  a_pOperand      The operand.
    /// \param  a_pContextScope (optional) The context scope.
    ///
    /// \return null if it fails, else an expression representing the operation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* solveUnaryPostOperator(StringView a_strOp, Expression* a_pOperand,
                                       LanguageElement* a_pContextScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Solve C++ unary post operator.
    ///
    ///         \code{.cpp}
    ///                    a--;
    ///                    ...
    ///         \endcode.
    ///
    /// \param  a_eOp       The operator id.
    /// \param  a_pOperand  The operand.
    /// \param  a_pScope    (optional) The context scope.
    ///
    /// \return null if it fails, else an expression representing the operation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* solveUnaryPostOperator(Operator a_eOp, Expression* a_pOperand, LanguageElement* a_pScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Solve C++ conditional expression.
    ///
    /// \param [in,out] a_pCond          The condition.
    /// \param  a_pThen               The then expression.
    /// \param  a_pElse    The else expression.
    /// \param  a_pScope                (optional) [in,out] If non-null, the scope.
    ///
    /// \return null if it fails, else a conditional expression representation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* solveConditional(Expression* a_pCond, Expression* a_pThen, Expression* a_pElse,
                                 LanguageElement* a_pScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant qualified lookup.
    ///
    /// \param [in,out] a_pLHS          The left hand side.
    /// \param  a_strName               The name.
    /// \param  a_pTemplateArguments    The template arguments.
    /// \param  a_pFunctionArguments    The function arguments.
    /// \param  a_pScope                (optional) [in,out] If non-null, the scope.
    /// \param  a_pInitializationType   (optional) [in,out] If non-null, type of the initialization.
    ///
    /// \return null if it fails, else the lookup result.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* qualifiedLookup(LanguageElement* a_pLHS, StringView a_strName,
                                     OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                     OptionalArrayView<Expression*>      a_pFunctionArguments,
                                     LanguageElement* a_pScope = nullptr, Type* a_pInitializationType = nullptr,
                                     int flags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant unqualified lookup.
    ///
    /// \param  a_strName               The name.
    /// \param  a_pTemplateArguments    The template arguments.
    /// \param  a_pFunctionArguments    The function arguments.
    /// \param  a_pScope                (optional) [in,out] If non-null, the scope.
    /// \param  a_pInitializationType   (optional) [in,out] If non-null, type of the initialization.
    ///
    /// \return null if it fails, else.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* unqualifiedLookup(StringView a_strName, OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                       OptionalArrayView<Expression*> a_pFunctionArguments,
                                       LanguageElement* a_pScope = nullptr, Type* a_pInitializationType = nullptr,
                                       int flags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant qualified symbols lookup.
    ///
    /// \param [in,out] a_pLHS          The left hand side.
    /// \param  a_strName               The symbols name.
    /// \param  a_Symbols               The result symbols.
    /// \param  a_pScope                (optional) [in,out] If non-null, the context scope.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void qualifiedSymbolsLookup(LanguageElement* a_pLHS, StringView a_strName, Symbols& a_Symbols,
                                Accesses* a_Accesses = nullptr, LanguageElement* a_pScope = nullptr,
                                Access a_Access = Access::Public, int flags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant unqualified symbols lookup.
    ///
    /// \param  a_strName               The name.
    /// \param  a_Symbols               The result symbols.
    /// \param  a_Symbols               (optional) The result accesses.
    /// \param  a_pScope                (optional) [in,out] If non-null, the scope.
    /// \param  a_InitAccess            (optional) The initial access.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void unqualifiedSymbolsLookup(StringView a_strName, Symbols& a_Symbols, Accesses* a_pAccesses = nullptr,
                                  LanguageElement* a_pScope = nullptr, Access a_Access = Access::Public, int flags = 0);

    void silentUnqualifiedSymbolsLookup(StringView a_strName, Symbols& a_Symbols, Accesses* a_pAccesses,
                                        LanguageElement* a_pScope = nullptr, Access a_Access = Access::Public,
                                        int flags = 0)
    {
        Accesses dummy;
        internalUnqualifiedSymbolsLookup(a_strName, a_Symbols, a_pAccesses ? *a_pAccesses : dummy, a_pScope, a_Access,
                                         flags);
    }

    void silentQualifiedSymbolsLookup(LanguageElement* a_pLHS, StringView a_strName, Symbols& a_Symbols,
                                      Accesses* a_pAccesses = nullptr, LanguageElement* a_pScope = nullptr,
                                      Access a_Access = Access::Public, int flags = 0)
    {
        Accesses dummy;
        internalQualifiedSymbolsLookup(a_pLHS, a_strName, a_Symbols, a_pAccesses ? *a_pAccesses : dummy, a_pScope,
                                       a_Access, flags);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant qualified symbols lookup (no error emitted).
    ///
    /// \param [in,out] a_pLHS          The left hand side.
    /// \param  a_strName               The symbols name.
    /// \param  a_Symbols               The result symbols.
    /// \param  a_pScope                (optional) [in,out] If non-null, the context scope.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* silentQualifiedLookup(LanguageElement* a_pLHS, StringView a_strName,
                                           OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                           OptionalArrayView<Expression*>      a_pFunctionArguments,
                                           LanguageElement* a_pScope = nullptr, Type* a_pInitializationType = nullptr,
                                           int flags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant unqualified symbols lookup (no error emitted).
    ///
    /// \param  a_strName               The name.
    /// \param  a_Symbols               The result symbols.
    /// \param  a_Symbols               (optional) The result accesses.
    /// \param  a_pScope                (optional) [in,out] If non-null, the scope.
    /// \param  a_InitAccess            (optional) The initial access.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* silentUnqualifiedLookup(StringView                          a_strName,
                                             OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                             OptionalArrayView<Expression*>      a_pFunctionArguments,
                                             LanguageElement* a_pScope = nullptr, Type* a_pInitializationType = nullptr,
                                             int flags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs a C++ compliant subroutine lookup.
    ///
    /// \param [in,out] a_pLHS          The left hand side.
    /// \param  a_strName               The subroutine name.
    /// \param  a_pTemplateArguments    The subroutine template arguments.
    /// \param  a_ParameterTypes        The subroutine parameters' types.
    /// \param  a_Modifiers             The subroutine qualifiers.
    /// \param  a_uiFlags               The flags.
    /// \param  a_pScope                (optional) If non-null, the scope, else use 'a_pLHS' as
    /// scope.
    ///
    /// \return null if it fails, else.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* subroutineLookup(LanguageElement* a_pLHS, StringView a_strName,
                                 OptionalArrayView<LanguageElement*> a_pTemplateArguments, TypesView a_ParameterTypes,
                                 Modifiers a_Modifiers, uint a_uiFlags, LanguageElement* a_pScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs a C++ compliant subroutine lookup recursively through all left hand side
    ///         owners.
    ///
    /// \param [in,out] a_pLHS          If non-null, the left hand side.
    /// \param  a_strName               The subroutine name.
    /// \param  a_pTemplateArguments    The subroutine template arguments.
    /// \param  a_ParameterTypes        The subroutine parameters' types.
    /// \param  a_Modifiers             The subroutine qualifiers.
    /// \param  a_uiFlags               The flags.
    /// \param  a_pScope                (optional) If non-null, the scope, else use 'a_pLHS' as
    /// scope.
    ///
    /// \return null if it fails, else.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Subroutine* subroutineLookupCascade(LanguageElement* a_pLHS, StringView a_strName,
                                                OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                                TypesView a_ParameterTypes, Modifiers a_Modifiers, uint a_uiFlags,
                                                LanguageElement* a_pScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Instanciate a template element.
    ///
    /// \param [in,out] a_pElement              The template element who have to be instantiated.
    /// \param [in,out] a_pTemplateSignature    The template signature.
    /// \param  a_Arguments                     The template arguments.
    /// \param  a_pScope                        (optional) If non-null, the scope.
    /// \param  a_Flags                         (optional) The option flags.
    ///
    /// \return null if it fails, else the instantiated template dependant element.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* instantiateTemplate(Template* a_pTemplate, const LanguageElements& a_Arguments,
                                         LanguageElement* a_pScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant template argument deduction (see C++ standard)
    ///
    /// \param [in,out] a_pParameter    If non-null, the parameter.
    /// \param [in,out] a_pArgument     If non-null, the argument.
    /// \param [in,out] deductions      [in,out] The explicit/implicit placeholders arguments
    /// deductions.
    ///
    /// \return null if it fails, else the deduced element.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* templateArgumentDeduction(LanguageElement* a_pParameter, LanguageElement* a_pArgument,
                                               PlaceholderMap& deductions);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant template type argument deduction (see C++ standard)
    ///
    /// \param [in,out] a_pParameter    If non-null, the parameter.
    /// \param [in,out] a_pArgument     If non-null, the argument type.
    /// \param [in,out] deductions      [in,out] The explicit/implicit placeholders arguments
    /// deductions.
    ///
    /// \return null if it fails, else the deduced type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* templateArgumentDeduction(LanguageElement* a_pParameter, Type* a_pArgument, PlaceholderMap& deductions);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant template type argument deduction as function call (see C++
    /// standard)
    ///
    /// \param [in,out] a_pParameter    If non-null, the parameter type.
    /// \param [in,out] a_pArgument     If non-null, the argument type.
    /// \param [in,out] deductions      [in,out] The explicit/implicit placeholders arguments
    /// deductions.
    ///
    /// \return null if it fails, else the deduced type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* callTemplateArgumentDeduction(Type* a_pParameter, Type* a_pArgument, PlaceholderMap& deductions);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant 'auto' keyword deduction and choose final type
    ///
    /// \param [in,out] a_pParameter    If non-null, the parameter type.
    /// \param [in,out] a_pArgument     If non-null, the argument type.
    ///
    /// \return null if it fails, else the deduced type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* autoDeduction(Type* a_pAutoBasedType, Type* a_pCandidate);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant 'auto' keyword deduction and convert final expression
    ///
    /// \param [in,out] a_pParameter    If non-null, the parameter type.
    /// \param [in,out] a_pArgument     If non-null, the argument expression.
    ///
    /// \return null if it fails, else the deduced type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* autoDeduction(Type* a_pParameter, Expression* a_pArgument);

    ////////////ço////////////////////////////////////////////////////////////////////////////////////////
    /// $!moi)i)\brief  Performs C++ compliant specialization comparison.
    ///
    /// \param a_pT0   The first specialization.
    /// \param a_pT1   The second specialization.
    ///
    /// \return -1 if T0 less specialized than T1, 0 if T0 and T1 are as mush specialized and 1 if
    /// T0 is more specialized than T1.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    int compareSpecialized(TemplateSpecialization* a_pT0, TemplateSpecialization* a_pT1);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs C++ compliant partial ordering.
    ///
    /// \param  P0  The first parameters list.
    /// \param  A0  The first arguments list.
    /// \param  P1  The second parameters list;
    /// \param  A1  The second argument list;
    ///
    /// \return -1 if P1/A1 couple is better than P0/A0 couple, 0 if neither P0/A0 or P1/A0 couples
    /// are better than the other, and 1 if P0/A0 couple is better than P1/A1 couple.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    int partialOrdering(const LanguageElements& P0, const LanguageElements& A0, const LanguageElements& P1,
                        const LanguageElements& A1);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if given name is a C++ identifier.
    ///
    /// \param  a_strName   The name.
    ///+
    /// \return true if C++ identifier, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isIdentifier(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the given expression has side effects.
    ///
    /// \param  a_pExp   The expression to test.
    ///
    /// \return true if the given expression has side effects, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasSideEffects(Expression* a_pExp) const;

    Expression* createCallExpression(Subroutine* a_pSubroutine, ExpressionsView a_Arguments, LanguageElement* a_pScope);
    Expression* createCallExpression(Constructor* a_pSubroutine, ExpressionsView a_Arguments,
                                     LanguageElement* a_pScope);
    Expression* createCallExpression(Method* a_pMethod, Expression* a_pCaller, ExpressionsView a_Arguments,
                                     LanguageElement* a_pScope);
    Expression* createCallExpression(Expression* a_pObjectExpression, Expression* a_pMethodPointerExpression,
                                     ExpressionsView a_Arguments, LanguageElement* a_pScope);
    Expression* createCallExpression(Function* a_pFunction, ExpressionsView a_Arguments, LanguageElement* a_pScope);

    Expression*
    internalConvert(Expression* a_pExpression, Type* a_pOutputType, CastKind a_eConversionType = CastKind::Implicit,
                    UserDefinedFunctions a_eUserDefinedAllowedConversions = UserDefinedFunctions::ImplicitsOnly,
                    LanguageElement* a_pContextScope = nullptr, bool a_bInitialize = false);

    Expression* defaultConstruct(Type* a_pType, LanguageElement* a_pContextScope = nullptr, bool a_bZeroMem = true);

    Expression* convert(Expression* a_pExpression, Type* a_pOutputType, CastKind a_eConversionType = CastKind::Implicit,
                        UserDefinedFunctions a_eUserDefinedAllowedConversions = UserDefinedFunctions::ImplicitsOnly,
                        LanguageElement*     a_pContextScope = nullptr);
    Expression* convert(Expression* a_pExpression, Type* a_pOutputType, LanguageElement* a_pContextScope);
    Expression* convertIfNotEqual(Expression* a_pExpression, Type* a_pOutputType, LanguageElement* a_pContextScope);
    Expression* convertIfNotEqual(Expression* a_pExpression, Type* a_pOutputType,
                                  UserDefinedFunctions a_eUserDefinedAllowedConversions,
                                  LanguageElement*     a_pContextScope);

    Expression* initialize(Expression* a_pExpression, Type* a_pOutputType,
                           UserDefinedFunctions a_eUserDefinedAllowedConversions = UserDefinedFunctions::ImplicitsOnly,
                           LanguageElement*     a_pContextScope = nullptr);
    Expression* initialize(Expression* a_pExpression, Type* a_pOutputType, LanguageElement* a_pContextScope);
    Expression* initializeIfNotEqual(Expression* a_pExpression, Type* a_pOutputType, LanguageElement* a_pContextScope);
    Expression* initializeIfNotEqual(Expression* a_pExpression, Type* a_pOutputType,
                                     UserDefinedFunctions a_eUserDefinedAllowedConversions,
                                     LanguageElement*     a_pContextScope);

    virtual Expression* toExpression(LanguageElement* a_pElement, Expression* a_pObjectExpression = nullptr);

    Expression* toExpression(Field* a_pField, Expression* a_pObjectExpression);
    Expression* toExpression(Property* a_pProperty, Expression* a_pObjectExpression);

    virtual Conversion*
    newConversion(Type* a_pInput, Type* a_pOutput, CastKind a_iConversionType = CastKind::Implicit,
                  UserDefinedFunctions a_eUserDefinedConversions = UserDefinedFunctions::ImplicitsOnly,
                  LanguageElement* a_pContextScope = nullptr, bool a_bInitialize = false);

    virtual Conversion* newConversion(Type* a_pInputType, Type* a_pOutputType,
                                      LanguageElement* a_pContextScope = nullptr, bool a_bInitialize = false)
    {
        return newConversion(a_pInputType, a_pOutputType, CastKind::Implicit, UserDefinedFunctions::ImplicitsOnly,
                             a_pContextScope, a_bInitialize);
    }

    virtual void deleteConversion(Conversion* a_pConversion);
    virtual void deleteConversions(ConversionResults& a_Conversions);

    virtual void computeSizeAndAlignment(Type* a_pType);

    void         buildClass(ClassType* a_pClassType, EClassBuildState a_eBuildState);
    void         buildScopeClasses(Scope* a_pScope, EClassBuildState a_eBuildState);
    void         buildScopeClasses(Block* a_pBlock, EClassBuildState a_eBuildState);
    virtual void sizeClass(ClassType* a_pClassType);
    virtual void edifyClass(ClassType* a_pClassType);
    virtual void finalizeClass(ClassType* a_pClassType);

    virtual bool isSymbolAccessible(Symbol* a_pSymbol, LanguageElement* a_pScope) const;
    virtual bool isFriendWith(Symbol* a_pWith, Symbol* a_pIsFriend) const;

    Source* getSource() const { return m_pSource; }

    LanguageElement* instantiateTemplateElement(LanguageElement* a_pPrimary, TemplateSpecialization* a_pInstantiation,
                                                EClassBuildState a_Pass, LanguageElement* a_pScope, int a_Flags);

    void setAccessModifiersEnabled(bool a_bEnabled);

    const CodeRangeLocation& currentCodeRangeLocation() const { return m_CodeRangeLocationStack.back(); }
    void                     pushCodeRangeLocation(const CodeRangeLocation& a_Loc);
    void                     popCodeRangeLocation();

    StringBuffer FormatArgumentList(Expression* const* a_ppArguments, size_t a_Count);

    StringBuffer FormatElementList(LanguageElement* const* a_ppLanguageElements, size_t a_Count);
    void         FormatElement(LanguageElement* a_pSymbol, StringBuffer& a_Txt);
    StringBuffer FormatElement(LanguageElement* a_pSymbol);

    Block* addBlock(Method* a_pMethod, bool a_EnsureTemplateInstanceComplete = false);
    Block* addBlock(Subroutine* a_pSubroutine, bool a_EnsureTemplateInstanceComplete = false);

    static Block* AddBlock(Method* a_pMethod);
    static Block* AddBlock(Subroutine* a_pSubroutine);

    void newImplicitConversions(Signature* a_pSignature, TypesView a_ArgTypes, ConversionResults& a_Out,
                                LanguageElement* a_pContextScope);
    void newImplicitConversions(Signature* a_pSignature, ExpressionsView a_Args, ConversionResults& a_Out,
                                LanguageElement* a_pContextScope);
    void newImplicitConversions(FunctionType* a_pFuncType, TypesView a_ArgTypes, ConversionResults& a_Out,
                                LanguageElement* a_pContextScope);
    void newImplicitConversions(FunctionType* a_pFuncType, ExpressionsView a_Args, ConversionResults& a_Out,
                                LanguageElement* a_pContextScope);

private:
    template<typename t_Ty>
    void visitIntegral(IntegralTypeT<t_Ty>* a_pInput, VisitorData a_Data);
    template<typename t_Ty>
    void visitFloatingPoint(FloatingPointTypeT<t_Ty>* a_pInput, VisitorData a_Data);

    ERefType         baseConversion(Type* a_pInput, Type*& a_pOutput, CastKind a_ConversionType);
    Conversion*      userDefinedConversion(Type* a_pInput, Type* a_pOutput, bool a_bExplicit, bool a_bInitialize,
                                           LanguageElement* a_pContextScope = nullptr, ERefType a_RefType = e_RefType_None);
    Conversion*      baseUserDefinedConversion(Type* a_pInput, Type* a_pOutput, CastKind a_ConversionType,
                                               LanguageElement* a_pContextScope);
    Conversion*      userDefinedConversionByConstruction(Type* a_pInput, ClassType* a_pOutput, bool a_bExplicit,
                                                         bool a_bInitialize, LanguageElement* a_pContextScope,
                                                         ERefType a_RefType);
    Conversion*      userDefinedConversionByConversionFunction(ClassType* a_pInput, Type* a_pOutput, bool a_bExplicit,
                                                               LanguageElement* a_pContextScope, ERefType a_RefType);
    Conversion*      userDefinedConversionByConversionFunction(Class* a_pInput, Type* a_pOutput, bool a_bExplicit,
                                                               LanguageElement* a_pContextScope, ERefType a_RefType);
    Conversion*      baseStandardConversion(Type* a_pInput, Type*& a_pOutput, CastKind a_ConversionType,
                                            ERefType& a_RefType);
    void             internalUnqualifiedSymbolsLookup(StringView a_strName, Symbols& a_Symbols, Accesses& a_Accesses,
                                                      LanguageElement* a_pScope = nullptr, Access a_Access = Access::Public,
                                                      int flags = 0);
    void             internalQualifiedSymbolsLookup(LanguageElement* a_pLHS, StringView a_strName, Symbols& a_Symbols,
                                                    Accesses& a_Accesses, LanguageElement* a_pScope = nullptr,
                                                    Access a_Access = Access::Public, int flags = 0);
    LanguageElement* internalQualifiedLookup(LanguageElement* a_pLHS, StringView a_strName,
                                             OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                             OptionalArrayView<Expression*>      a_pFunctionArguments,
                                             LanguageElement* a_pScope = nullptr, Type* a_pInitializationType = nullptr,
                                             int flags = 0);
    LanguageElement* internalUnqualifiedLookup(StringView                          a_strName,
                                               OptionalArrayView<LanguageElement*> a_pTemplateArguments,
                                               OptionalArrayView<Expression*>      a_pFunctionArguments,
                                               LanguageElement*                    a_pScope = nullptr,
                                               Type* a_pInitializationType = nullptr, int flags = 0);

    template<typename t_Ty>
    Conversion* baseIntegralConversion(IntegralTypeT<t_Ty>* a_pInput, Type*& pOutput, CastKind iConversionType,
                                       ERefType& refType);
    template<typename t_Ty, CastKind>
    Conversion* integralConversion(IntegralTypeT<t_Ty>* a_pInput, Type* pOutput, bool a_bPromotion, ERefType refType);
    template<typename t_Ty, CastKind>
    NumericConversion* integralConversion(IntegralTypeT<t_Ty>* a_pInput, Type* pOutput, bool a_bPromotion);
    template<typename t_Ty, CastKind>
    NumericConversion* arithmeticConversion(ArithmeticTypeT<t_Ty>* a_pInput, Type* pOutput, bool a_bPromotion = false);
    template<typename t_Ty, CastKind>
    NumericConversion* floatingPointConversion(FloatingPointTypeT<t_Ty>* a_pInput, Type* pOutput,
                                               bool a_bPromotion = false);
    template<typename t_Ty, CastKind>
    Conversion* floatingPointConversion(FloatingPointTypeT<t_Ty>* a_pInput, Type* pOutput, bool a_bPromotion,
                                        ERefType refType);
    void        generateArgumentsText(const Expressions& arguments, String& argumentsText);

    SmallMap<TemplateSpecialization*, SmallMap<LanguageElement*, LanguageElement*>>& templateInstantiations();

    LanguageElement* resolveTemplateDependency(LanguageElement*                  a_pElement,
                                               const class TemplateSubstitution& a_TemplateSubstitution,
                                               EClassBuildState a_uiPass, LanguageElement* a_pScope, int flags);
    LanguageElement* instantiateTemplateElement(LanguageElement*                  a_pPrimary,
                                                const class TemplateSubstitution& a_TemplateSubstitution,
                                                EClassBuildState a_Pass, LanguageElement* a_pScope, int a_Flags);

    void ErrorTemplateInstantiationFailure(TemplateDependantElement* a_pInput, LanguageElements* pTemplateArguments,
                                           Expressions* pFunctionArguments);

    void ConversionError(Expression* a_pFrom, Type* a_pTo);
    void ConversionError(Type* a_pFrom, Type* a_pTo);

    void _selectCallCandidateError(StringView name, Symbols const& candidates, Subroutines const& viableCandidates,
                                   Expressions const& arguments);

private:
    void        _errorToMessage(const CodeRangeLocation& loc, const char* txt);
    Expression* _createFieldCopyConstruction(Class* a_pThis, Class* a_pClass, Expression* a_pWhere);
    Expression* _createFieldCopyConstruction(Class* a_pThis, Array* a_pArray, Expression* a_pWhere);
    Expression* _createFieldMoveConstruction(Class* a_pThis, Block* a_pBlock, Field* a_pDM, Class* a_pClass,
                                             Expression* a_pWhere);
    Expression* _createFieldMoveConstruction(Class* a_pThis, Block* a_pBlock, Field* a_pDM, Array* a_pArray,
                                             Expression* a_pWhere);
    void _copyAssignField(Class* a_pThis, Block* a_pBlock, Expression* a_pDME, Class* a_pClass, Expression* a_pWhere);
    void _copyAssignField(Class* a_pThis, Block* a_pBlock, Expression* a_pDME, Array* a_pArray, Expression* a_pWhere);

    void _moveAssignField(Class* a_pThis, Block* a_pBlock, Expression* a_pDME, Class* a_pClass, Expression* a_pWhere);
    void _moveAssignField(Class* a_pThis, Block* a_pBlock, Expression* a_pDME, Array* a_pArray, Expression* a_pWhere);

private:
    BuiltInOperator*   m_BuiltInOperators[int(Operator::COUNT)];
    const char*        m_ScopeDelimiter = "::";
    Message*           m_pMessage = nullptr;
    Source*            m_pSource = nullptr;
    NameFormatDelegate m_NameFormatDelegate;
    SmallMap<TemplateSpecialization*, SmallMap<LanguageElement*, LanguageElement*>>* m_pTemplateInstantiations =
    nullptr;
    CodeRangeLocations m_CodeRangeLocationStack;
    bool               m_bAccessModifiersEnabled = true;
    struct ConversionParams
    {
        LanguageElement* scope{};
        uint32_t         castKind{};
        uint16_t         udConv{};
        uint16_t         init{};
        bool operator==(ConversionParams const& o) const { return memcmp(this, &o, sizeof(ConversionParams)) == 0; };
    };

    struct ConvHash
    {
        bool      operator<(ConvHash const& _other) const { return memcmp(data, _other.data, sizeof(ConvHash)) < 0; }
        uintptr_t data[4] = {0, 0, 0, 0};
    };

    SmallMap<ConvHash, Pair<ConversionParams, Conversion*>, 256> m_CachedConversions;
    bool                                                         m_bStandardLookup = false;

private:
    enum
    {
        e_VisitorFlag_Unqualified = 0x2,
    };
};

} // namespace lang
} // namespace phantom

#include "detail/conversions.h"
