#include <phantom/plugin.h>
#include <iostream>
#include <functional>
#include "Lexer.hpp"
#include <phantom/utils/SmallVector.h>
#include <phantom/utils/Functor.h>

namespace phantom
{
    namespace lang
    {
        namespace ast
        {
            using phantom::SmallVector;
            enum EToken : short
            {
                tokenERROR = -1
                , token_SCOPE = 1 // must starts to 1, 0 is reserved by spell::Lexer
                , token_BLOCK_START
                , token_BLOCK_END
                , token_PAREN_START
                , token_PAREN_END
                , token_BRACKET_START
                , token_BRACKET_END
                , token_CLASS
                , token_UNION
                , token_STRUCT
                , token_ENUM
                , token_TEMPLATE
                , token_TYPENAME
                , token_THIS
                , token_NULLPTR
                , token_OPERATOR
                , token_OVERRIDE
                , token_BREAK
                , token_CONTINUE
                , token_SEMI_COLON
                , token_COLON
                , token_COMMA
                , token_ASS_ADD
                , token_ASS_AND
                , token_ASS_DIV
                , token_ASS_MOD
                , token_ASS_MUL
                , token_ASS_OR
                , token_ASS_SHL
                , token_ASS_SUB
                , token_ASS_XOR
                , token_LE
                , token_GE
                , token_ARROW
                , token_LT
                , token_GT
                , token_EQ
                , token_NE
                , token_NOT
                , token_QUESTION
                , token_MUL
                , token_DIV
                , token_INC
                , token_DEC
                , token_PLUS
                , token_MINUS
                , token_COMPL
                , token_DOT
                , token_ATAT
                , token_LOG_AND
                , token_BIN_AND
                , token_LOG_OR
                , token_BIN_OR
                , token_XOR
                , token_MOD
                , token_SHL
                , token_PUBLIC
                , token_PROTECTED
                , token_PRIVATE
                , token_ASSIGN
                , token_ELLIPSE
                , token_AUTO
                , token_UNSIGNED
                , token_SIGNED
                , token_LONG
                , token_BOOL
                , token_CHAR
                , token_SHORT
                , token_INT
                , token_FLOATING_POINT_TYPE
                , token_VOID
                , token_CASE
                , token_FOR
                , token_WHILE
                , token_SWITCH
                , token_DEFAULT
                , token_DO
                , token_IF
                , token_ELSE
                , token_RETURN
                , token_GOTO
                , token_NEW
                , token_DELETE
                , token_EXPLICIT
                , token_TYPEDEF
                , token_USING
                , token_REINTERPRET_CAST
                , token_STATIC_CAST
                , token_CONST_CAST
                , token_SIZEOF
                , token_ALIGNOF
                , token_ALIGNAS
                , token_FRIEND
                , token_TYPEOF
                , token_CLASSOF
                , token_ENUMOF
                , token_UNIONOF
                , token_IMPORT
                , token_INCLUDE
                , token_MODULE
                , token_FUNCTION
                , token_FUNCTION_PTR
                , token_DELEGATE
                , token_ASSERT
                , token_STATIC_ASSERT
                , token_IS_SAME
                , token_CONST
                , token_VOLATILE
                , token_DECLTYPE
                , token_FINAL
                , token_VIRTUAL
                , token_STATIC
                , token_HEX_VALUE
                , token_FLOAT_VALUE
                , token_DOUBLE_VALUE
                , token_ULONGLONG_VALUE
                , token_ULONG_VALUE
                , token_UINT_VALUE
                , token_LONGLONG_VALUE
                , token_LONG_VALUE
                , token_INT_VALUE
                , token_BOOL_VALUE
                , token_STRING_LIT
                , token_CHAR_LIT
                , token_RESERVED_PHANTOM_KEYWORD
                , token_IDENTIFIER
                , tokenEOF
            };
            typedef phantom::SmallVector<EToken> ETokens;
            enum EError
            {
                e_ErrorOK,
                e_Error_ExpectedToken,
                e_Error_UnexpectedToken,
                e_Error_UnknownToken,
                e_Error_CannotReadInput,
                e_Error_Count,
            };
            typedef phantom::SmallVector<EError> EErrors;
            enum ERuleType
            {
                e_RuleTypeUndefined,
                e_RuleType_Source,
                e_RuleType_ModuleDeclaration,
                e_RuleType_ImportDeclaration,
                e_RuleType_Import,
                e_RuleType_ImportBinding,
                e_RuleType_ImportBase,
                e_RuleType_Meta,
                e_RuleType_MetaDecl,
                e_RuleType_Annotation,
                e_RuleType_MetaOrAnnotation,
                e_RuleType_SourceDeclaration,
                e_RuleType_Declaration,
                e_RuleType_MixinStatementDefinition,
                e_RuleType_BlockDeclaration,
                e_RuleType_ConversionFunction,
                e_RuleType_Constructor,
                e_RuleType_Destructor,
                e_RuleType_ConstructorInitializer,
                e_RuleType_ArrowReturn,
                e_RuleType_FunctionEnd,
                e_RuleType_Operator,
                e_RuleType_OperatorEnd,
                e_RuleType_FunctionOrVariableEnd,
                e_RuleType_MethodSpecifier,
                e_RuleType_Specifier,
                e_RuleType_FunctionOrVariable,
                e_RuleType_DeclType,
                e_RuleType_TemplateArgument,
                e_RuleType_LastTemplateArgumentAssign,
                e_RuleType_LastTemplateArgumentNoAssign,
                e_RuleType_LastTemplateArgument,
                e_RuleType_NotLastTemplateArgumentAssign,
                e_RuleType_NotLastTemplateArgumentNoAssign,
                e_RuleType_NotLastTemplateArgument,
                e_RuleType_TemplateArgumentListSplit,
                e_RuleType_TemplateArgumentNoAssign,
                e_RuleType_TemplateArgumentAssign,
                e_RuleType_TemplateArgumentList,
                e_RuleType_Name,
                e_RuleType_FundamentalTypeBase,
                e_RuleType_UnsignedType,
                e_RuleType_SignedType,
                e_RuleType_LongType,
                e_RuleType_VoidType,
                e_RuleType_FundamentalType,
                e_RuleType_QualifiedType,
                e_RuleType_ExplicitType,
                e_RuleType_TypeNoFunctionType,
                e_RuleType_ExplicitTypeNoFunctionType,
                e_RuleType_ExpressionType,
                e_RuleType_Type,
                e_RuleType_ExplicitTypeOrAuto,
                e_RuleType_TypeOrAuto,
                e_RuleType_AutoType,
                e_RuleType_BasicType,
                e_RuleType_Auto,
                e_RuleType_BasicTypeOrAuto,
                e_RuleType_IntegralConstant,
                e_RuleType_ArrayExtentExpression,
                e_RuleType_ArrayExtent,
                e_RuleType_FunctionTypeExtent,
                e_RuleType_DerivedExtent,
                e_RuleType_TypeExtentNoFunction,
                e_RuleType_TypeExtent,
                e_RuleType_Enum,
                e_RuleType_EnumConstant,
                e_RuleType_Using,
                e_RuleType_Typedef,
                e_RuleType_Alias,
                e_RuleType_TemplateParameterDefault,
                e_RuleType_TemplateValueParameter,
                e_RuleType_TemplateTypeParameter,
                e_RuleType_TemplateParameter,
                e_RuleType_TemplateSignature,
                e_RuleType_Template,
                e_RuleType_TemplateFunction,
                e_RuleType_FunctionBlock,
                e_RuleType_QualifiedName,
                e_RuleType_QualifiedDotName,
                e_RuleType_QualifiedScopedName,
                e_RuleType_BaseClass,
                e_RuleType_Class,
                e_RuleType_Union,
                e_RuleType_Method,
                e_RuleType_Parameter,
                e_RuleType_AccessSpecifier,
                e_RuleType_FieldOverride,
                e_RuleType_Symbol,
                e_RuleType_Member,
                e_RuleType_MemberWithMetaOrAnnotation,
                e_RuleType_BasicMemberWithMetaOrAnnotation,
                e_RuleType_Friend,
                e_RuleType_BasicMember,
                e_RuleType_StatementOrCase,
                e_RuleType_IfOrWhileLocalVariableDecl,
                e_RuleType_IfOrWhileCondition,
                e_RuleType_IfStatement,
                e_RuleType_CaseStatement,
                e_RuleType_DefaultStatement,
                e_RuleType_LabelStatement,
                e_RuleType_SwitchStatement,
                e_RuleType_ForeachSignature,
                e_RuleType_ForInit,
                e_RuleType_ForStatement,
                e_RuleType_WhileStatement,
                e_RuleType_BlockStatement,
                e_RuleType_DoWhileStatement,
                e_RuleType_Statement,
                e_RuleType_MixinStatement,
                e_RuleType_ReturnStatement,
                e_RuleType_GotoStatement,
                e_RuleType_Declarator,
                e_RuleType_ExplicitLocalVariableDecl,
                e_RuleType_AutoDeclarator,
                e_RuleType_AutoLocalVariableDecl,
                e_RuleType_LocalVariableDecl,
                e_RuleType_LocalVariableStatement,
                e_RuleType_ExpressionStatement,
                e_RuleType_Expression,
                e_RuleType_AssignExpression,
                e_RuleType_ConditionalExpression,
                e_RuleType_LogicalOrExpressionRight,
                e_RuleType_LogicalOrExpression,
                e_RuleType_LogicalAndExpressionRight,
                e_RuleType_LogicalAndExpression,
                e_RuleType_BinOrExpressionRight,
                e_RuleType_BinOrExpression,
                e_RuleType_XOrExpressionRight,
                e_RuleType_XOrExpression,
                e_RuleType_BinAndExpressionRight,
                e_RuleType_BinAndExpression,
                e_RuleType_EqualityExpressionRight,
                e_RuleType_EqualityExpression,
                e_RuleType_RelationalExpressionRight,
                e_RuleType_RelationalExpression,
                e_RuleType_ShiftExpressionRight,
                e_RuleType_ShiftExpression,
                e_RuleType_AddExpressionRight,
                e_RuleType_AddExpression,
                e_RuleType_MulExpressionRight,
                e_RuleType_MulExpression,
                e_RuleType_SpecialCastExpression,
                e_RuleType_FunctionPtrExpression,
                e_RuleType_UnaryExpression,
                e_RuleType_AssignExpressionOrInitializerList,
                e_RuleType_ExpressionOrInitializerList,
                e_RuleType_InitializerList,
                e_RuleType_CallList,
                e_RuleType_ArrayAccess,
                e_RuleType_KeywordExpression,
                e_RuleType_NullptrExpression,
                e_RuleType_PostFixExpression,
                e_RuleType_PostFixEnd,
                e_RuleType_CStyleCastExpression,
                e_RuleType_TypeTraitBinary,
                e_RuleType_StaticAssert,
                e_RuleType_AssertStatement,
                e_RuleType_BreakStatement,
                e_RuleType_ContinueStatement,
                e_RuleType_SizeofExpression,
                e_RuleType_TypeofExpression,
                e_RuleType_StringLiteralChain,
                e_RuleType_FundamentalTypeFunctionCast,
                e_RuleType_LambdaCapture,
                e_RuleType_LambdaExpression,
                e_RuleType_PrimaryExpression,
            };
            struct _BaseRule;
            struct Source;
            struct ModuleDeclaration;
            struct ImportDeclaration;
            struct Import;
            struct ImportBinding;
            struct ImportBase;
            struct Meta;
            struct MetaDecl;
            struct Annotation;
            struct MetaOrAnnotation;
            struct SourceDeclaration;
            struct Declaration;
            struct MixinStatementDefinition;
            struct BlockDeclaration;
            struct ConversionFunction;
            struct Constructor;
            struct Destructor;
            struct ConstructorInitializer;
            struct ArrowReturn;
            struct FunctionEnd;
            struct Operator;
            struct OperatorEnd;
            struct FunctionOrVariableEnd;
            struct MethodSpecifier;
            struct Specifier;
            struct FunctionOrVariable;
            struct DeclType;
            struct TemplateArgument;
            struct LastTemplateArgumentAssign;
            struct LastTemplateArgumentNoAssign;
            struct LastTemplateArgument;
            struct NotLastTemplateArgumentAssign;
            struct NotLastTemplateArgumentNoAssign;
            struct NotLastTemplateArgument;
            struct TemplateArgumentListSplit;
            struct TemplateArgumentNoAssign;
            struct TemplateArgumentAssign;
            struct TemplateArgumentList;
            struct Name;
            struct FundamentalTypeBase;
            struct UnsignedType;
            struct SignedType;
            struct LongType;
            struct VoidType;
            struct FundamentalType;
            struct QualifiedType;
            struct ExplicitType;
            struct TypeNoFunctionType;
            struct ExplicitTypeNoFunctionType;
            struct ExpressionType;
            struct Type;
            struct ExplicitTypeOrAuto;
            struct TypeOrAuto;
            struct AutoType;
            struct BasicType;
            struct Auto;
            struct BasicTypeOrAuto;
            struct IntegralConstant;
            struct ArrayExtentExpression;
            struct ArrayExtent;
            struct FunctionTypeExtent;
            struct DerivedExtent;
            struct TypeExtentNoFunction;
            struct TypeExtent;
            struct Enum;
            struct EnumConstant;
            struct Using;
            struct Typedef;
            struct Alias;
            struct TemplateParameterDefault;
            struct TemplateValueParameter;
            struct TemplateTypeParameter;
            struct TemplateParameter;
            struct TemplateSignature;
            struct Template;
            struct TemplateFunction;
            struct FunctionBlock;
            struct QualifiedName;
            struct QualifiedDotName;
            struct QualifiedScopedName;
            struct BaseClass;
            struct Class;
            struct Union;
            struct Method;
            struct Parameter;
            struct AccessSpecifier;
            struct FieldOverride;
            struct Symbol;
            struct Member;
            struct MemberWithMetaOrAnnotation;
            struct BasicMemberWithMetaOrAnnotation;
            struct Friend;
            struct BasicMember;
            struct StatementOrCase;
            struct IfOrWhileLocalVariableDecl;
            struct IfOrWhileCondition;
            struct IfStatement;
            struct CaseStatement;
            struct DefaultStatement;
            struct LabelStatement;
            struct SwitchStatement;
            struct ForeachSignature;
            struct ForInit;
            struct ForStatement;
            struct WhileStatement;
            struct BlockStatement;
            struct DoWhileStatement;
            struct Statement;
            struct MixinStatement;
            struct ReturnStatement;
            struct GotoStatement;
            struct Declarator;
            struct ExplicitLocalVariableDecl;
            struct AutoDeclarator;
            struct AutoLocalVariableDecl;
            struct LocalVariableDecl;
            struct LocalVariableStatement;
            struct ExpressionStatement;
            struct Expression;
            struct AssignExpression;
            struct ConditionalExpression;
            struct LogicalOrExpressionRight;
            struct LogicalOrExpression;
            struct LogicalAndExpressionRight;
            struct LogicalAndExpression;
            struct BinOrExpressionRight;
            struct BinOrExpression;
            struct XOrExpressionRight;
            struct XOrExpression;
            struct BinAndExpressionRight;
            struct BinAndExpression;
            struct EqualityExpressionRight;
            struct EqualityExpression;
            struct RelationalExpressionRight;
            struct RelationalExpression;
            struct ShiftExpressionRight;
            struct ShiftExpression;
            struct AddExpressionRight;
            struct AddExpression;
            struct MulExpressionRight;
            struct MulExpression;
            struct SpecialCastExpression;
            struct FunctionPtrExpression;
            struct UnaryExpression;
            struct AssignExpressionOrInitializerList;
            struct ExpressionOrInitializerList;
            struct InitializerList;
            struct CallList;
            struct ArrayAccess;
            struct KeywordExpression;
            struct NullptrExpression;
            struct PostFixExpression;
            struct PostFixEnd;
            struct CStyleCastExpression;
            struct TypeTraitBinary;
            struct StaticAssert;
            struct AssertStatement;
            struct BreakStatement;
            struct ContinueStatement;
            struct SizeofExpression;
            struct TypeofExpression;
            struct StringLiteralChain;
            struct FundamentalTypeFunctionCast;
            struct LambdaCapture;
            struct LambdaExpression;
            struct PrimaryExpression;
            typedef phantom::SmallVector<Source*> Sources;
            typedef phantom::SmallVector<ModuleDeclaration*> ModuleDeclarations;
            typedef phantom::SmallVector<ImportDeclaration*> ImportDeclarations;
            typedef phantom::SmallVector<Import*> Imports;
            typedef phantom::SmallVector<ImportBinding*> ImportBindings;
            typedef phantom::SmallVector<ImportBase*> ImportBases;
            typedef phantom::SmallVector<Meta*> Metas;
            typedef phantom::SmallVector<MetaDecl*> MetaDecls;
            typedef phantom::SmallVector<Annotation*> Annotations;
            typedef phantom::SmallVector<MetaOrAnnotation*> MetaOrAnnotations;
            typedef phantom::SmallVector<SourceDeclaration*> SourceDeclarations;
            typedef phantom::SmallVector<Declaration*> Declarations;
            typedef phantom::SmallVector<MixinStatementDefinition*> MixinStatementDefinitions;
            typedef phantom::SmallVector<BlockDeclaration*> BlockDeclarations;
            typedef phantom::SmallVector<ConversionFunction*> ConversionFunctions;
            typedef phantom::SmallVector<Constructor*> Constructors;
            typedef phantom::SmallVector<Destructor*> Destructors;
            typedef phantom::SmallVector<ConstructorInitializer*> ConstructorInitializers;
            typedef phantom::SmallVector<ArrowReturn*> ArrowReturns;
            typedef phantom::SmallVector<FunctionEnd*> FunctionEnds;
            typedef phantom::SmallVector<Operator*> Operators;
            typedef phantom::SmallVector<OperatorEnd*> OperatorEnds;
            typedef phantom::SmallVector<FunctionOrVariableEnd*> FunctionOrVariableEnds;
            typedef phantom::SmallVector<MethodSpecifier*> MethodSpecifiers;
            typedef phantom::SmallVector<Specifier*> Specifiers;
            typedef phantom::SmallVector<FunctionOrVariable*> FunctionOrVariables;
            typedef phantom::SmallVector<DeclType*> DeclTypes;
            typedef phantom::SmallVector<TemplateArgument*> TemplateArguments;
            typedef phantom::SmallVector<LastTemplateArgumentAssign*> LastTemplateArgumentAssigns;
            typedef phantom::SmallVector<LastTemplateArgumentNoAssign*> LastTemplateArgumentNoAssigns;
            typedef phantom::SmallVector<LastTemplateArgument*> LastTemplateArguments;
            typedef phantom::SmallVector<NotLastTemplateArgumentAssign*> NotLastTemplateArgumentAssigns;
            typedef phantom::SmallVector<NotLastTemplateArgumentNoAssign*> NotLastTemplateArgumentNoAssigns;
            typedef phantom::SmallVector<NotLastTemplateArgument*> NotLastTemplateArguments;
            typedef phantom::SmallVector<TemplateArgumentListSplit*> TemplateArgumentListSplits;
            typedef phantom::SmallVector<TemplateArgumentNoAssign*> TemplateArgumentNoAssigns;
            typedef phantom::SmallVector<TemplateArgumentAssign*> TemplateArgumentAssigns;
            typedef phantom::SmallVector<TemplateArgumentList*> TemplateArgumentLists;
            typedef phantom::SmallVector<Name*> Names;
            typedef phantom::SmallVector<FundamentalTypeBase*> FundamentalTypeBases;
            typedef phantom::SmallVector<UnsignedType*> UnsignedTypes;
            typedef phantom::SmallVector<SignedType*> SignedTypes;
            typedef phantom::SmallVector<LongType*> LongTypes;
            typedef phantom::SmallVector<VoidType*> VoidTypes;
            typedef phantom::SmallVector<FundamentalType*> FundamentalTypes;
            typedef phantom::SmallVector<QualifiedType*> QualifiedTypes;
            typedef phantom::SmallVector<ExplicitType*> ExplicitTypes;
            typedef phantom::SmallVector<TypeNoFunctionType*> TypeNoFunctionTypes;
            typedef phantom::SmallVector<ExplicitTypeNoFunctionType*> ExplicitTypeNoFunctionTypes;
            typedef phantom::SmallVector<ExpressionType*> ExpressionTypes;
            typedef phantom::SmallVector<Type*> Types;
            typedef phantom::SmallVector<ExplicitTypeOrAuto*> ExplicitTypeOrAutos;
            typedef phantom::SmallVector<TypeOrAuto*> TypeOrAutos;
            typedef phantom::SmallVector<AutoType*> AutoTypes;
            typedef phantom::SmallVector<BasicType*> BasicTypes;
            typedef phantom::SmallVector<Auto*> Autos;
            typedef phantom::SmallVector<BasicTypeOrAuto*> BasicTypeOrAutos;
            typedef phantom::SmallVector<IntegralConstant*> IntegralConstants;
            typedef phantom::SmallVector<ArrayExtentExpression*> ArrayExtentExpressions;
            typedef phantom::SmallVector<ArrayExtent*> ArrayExtents;
            typedef phantom::SmallVector<FunctionTypeExtent*> FunctionTypeExtents;
            typedef phantom::SmallVector<DerivedExtent*> DerivedExtents;
            typedef phantom::SmallVector<TypeExtentNoFunction*> TypeExtentNoFunctions;
            typedef phantom::SmallVector<TypeExtent*> TypeExtents;
            typedef phantom::SmallVector<Enum*> Enums;
            typedef phantom::SmallVector<EnumConstant*> EnumConstants;
            typedef phantom::SmallVector<Using*> Usings;
            typedef phantom::SmallVector<Typedef*> Typedefs;
            typedef phantom::SmallVector<Alias*> Aliases;
            typedef phantom::SmallVector<TemplateParameterDefault*> TemplateParameterDefaults;
            typedef phantom::SmallVector<TemplateValueParameter*> TemplateValueParameters;
            typedef phantom::SmallVector<TemplateTypeParameter*> TemplateTypeParameters;
            typedef phantom::SmallVector<TemplateParameter*> TemplateParameters;
            typedef phantom::SmallVector<TemplateSignature*> TemplateSignatures;
            typedef phantom::SmallVector<Template*> Templates;
            typedef phantom::SmallVector<TemplateFunction*> TemplateFunctions;
            typedef phantom::SmallVector<FunctionBlock*> FunctionBlocks;
            typedef phantom::SmallVector<QualifiedName*> QualifiedNames;
            typedef phantom::SmallVector<QualifiedDotName*> QualifiedDotNames;
            typedef phantom::SmallVector<QualifiedScopedName*> QualifiedScopedNames;
            typedef phantom::SmallVector<BaseClass*> BaseClasses;
            typedef phantom::SmallVector<Class*> Classes;
            typedef phantom::SmallVector<Union*> Unions;
            typedef phantom::SmallVector<Method*> Methods;
            typedef phantom::SmallVector<Parameter*> Parameters;
            typedef phantom::SmallVector<AccessSpecifier*> AccessSpecifiers;
            typedef phantom::SmallVector<FieldOverride*> FieldOverrides;
            typedef phantom::SmallVector<Symbol*> Symbols;
            typedef phantom::SmallVector<Member*> Members;
            typedef phantom::SmallVector<MemberWithMetaOrAnnotation*> MemberWithMetaOrAnnotations;
            typedef phantom::SmallVector<BasicMemberWithMetaOrAnnotation*> BasicMemberWithMetaOrAnnotations;
            typedef phantom::SmallVector<Friend*> Friends;
            typedef phantom::SmallVector<BasicMember*> BasicMembers;
            typedef phantom::SmallVector<StatementOrCase*> StatementOrCases;
            typedef phantom::SmallVector<IfOrWhileLocalVariableDecl*> IfOrWhileLocalVariableDecls;
            typedef phantom::SmallVector<IfOrWhileCondition*> IfOrWhileConditions;
            typedef phantom::SmallVector<IfStatement*> IfStatements;
            typedef phantom::SmallVector<CaseStatement*> CaseStatements;
            typedef phantom::SmallVector<DefaultStatement*> DefaultStatements;
            typedef phantom::SmallVector<LabelStatement*> LabelStatements;
            typedef phantom::SmallVector<SwitchStatement*> SwitchStatements;
            typedef phantom::SmallVector<ForeachSignature*> ForeachSignatures;
            typedef phantom::SmallVector<ForInit*> ForInits;
            typedef phantom::SmallVector<ForStatement*> ForStatements;
            typedef phantom::SmallVector<WhileStatement*> WhileStatements;
            typedef phantom::SmallVector<BlockStatement*> BlockStatements;
            typedef phantom::SmallVector<DoWhileStatement*> DoWhileStatements;
            typedef phantom::SmallVector<Statement*> Statements;
            typedef phantom::SmallVector<MixinStatement*> MixinStatements;
            typedef phantom::SmallVector<ReturnStatement*> ReturnStatements;
            typedef phantom::SmallVector<GotoStatement*> GotoStatements;
            typedef phantom::SmallVector<Declarator*> Declarators;
            typedef phantom::SmallVector<ExplicitLocalVariableDecl*> ExplicitLocalVariableDecls;
            typedef phantom::SmallVector<AutoDeclarator*> AutoDeclarators;
            typedef phantom::SmallVector<AutoLocalVariableDecl*> AutoLocalVariableDecls;
            typedef phantom::SmallVector<LocalVariableDecl*> LocalVariableDecls;
            typedef phantom::SmallVector<LocalVariableStatement*> LocalVariableStatements;
            typedef phantom::SmallVector<ExpressionStatement*> ExpressionStatements;
            typedef phantom::SmallVector<Expression*> Expressions;
            typedef phantom::SmallVector<AssignExpression*> AssignExpressions;
            typedef phantom::SmallVector<ConditionalExpression*> ConditionalExpressions;
            typedef phantom::SmallVector<LogicalOrExpressionRight*> LogicalOrExpressionRights;
            typedef phantom::SmallVector<LogicalOrExpression*> LogicalOrExpressions;
            typedef phantom::SmallVector<LogicalAndExpressionRight*> LogicalAndExpressionRights;
            typedef phantom::SmallVector<LogicalAndExpression*> LogicalAndExpressions;
            typedef phantom::SmallVector<BinOrExpressionRight*> BinOrExpressionRights;
            typedef phantom::SmallVector<BinOrExpression*> BinOrExpressions;
            typedef phantom::SmallVector<XOrExpressionRight*> XOrExpressionRights;
            typedef phantom::SmallVector<XOrExpression*> XOrExpressions;
            typedef phantom::SmallVector<BinAndExpressionRight*> BinAndExpressionRights;
            typedef phantom::SmallVector<BinAndExpression*> BinAndExpressions;
            typedef phantom::SmallVector<EqualityExpressionRight*> EqualityExpressionRights;
            typedef phantom::SmallVector<EqualityExpression*> EqualityExpressions;
            typedef phantom::SmallVector<RelationalExpressionRight*> RelationalExpressionRights;
            typedef phantom::SmallVector<RelationalExpression*> RelationalExpressions;
            typedef phantom::SmallVector<ShiftExpressionRight*> ShiftExpressionRights;
            typedef phantom::SmallVector<ShiftExpression*> ShiftExpressions;
            typedef phantom::SmallVector<AddExpressionRight*> AddExpressionRights;
            typedef phantom::SmallVector<AddExpression*> AddExpressions;
            typedef phantom::SmallVector<MulExpressionRight*> MulExpressionRights;
            typedef phantom::SmallVector<MulExpression*> MulExpressions;
            typedef phantom::SmallVector<SpecialCastExpression*> SpecialCastExpressions;
            typedef phantom::SmallVector<FunctionPtrExpression*> FunctionPtrExpressions;
            typedef phantom::SmallVector<UnaryExpression*> UnaryExpressions;
            typedef phantom::SmallVector<AssignExpressionOrInitializerList*> AssignExpressionOrInitializerLists;
            typedef phantom::SmallVector<ExpressionOrInitializerList*> ExpressionOrInitializerLists;
            typedef phantom::SmallVector<InitializerList*> InitializerLists;
            typedef phantom::SmallVector<CallList*> CallLists;
            typedef phantom::SmallVector<ArrayAccess*> ArrayAccesses;
            typedef phantom::SmallVector<KeywordExpression*> KeywordExpressions;
            typedef phantom::SmallVector<NullptrExpression*> NullptrExpressions;
            typedef phantom::SmallVector<PostFixExpression*> PostFixExpressions;
            typedef phantom::SmallVector<PostFixEnd*> PostFixEnds;
            typedef phantom::SmallVector<CStyleCastExpression*> CStyleCastExpressions;
            typedef phantom::SmallVector<TypeTraitBinary*> TypeTraitBinaries;
            typedef phantom::SmallVector<StaticAssert*> StaticAsserts;
            typedef phantom::SmallVector<AssertStatement*> AssertStatements;
            typedef phantom::SmallVector<BreakStatement*> BreakStatements;
            typedef phantom::SmallVector<ContinueStatement*> ContinueStatements;
            typedef phantom::SmallVector<SizeofExpression*> SizeofExpressions;
            typedef phantom::SmallVector<TypeofExpression*> TypeofExpressions;
            typedef phantom::SmallVector<StringLiteralChain*> StringLiteralChains;
            typedef phantom::SmallVector<FundamentalTypeFunctionCast*> FundamentalTypeFunctionCasts;
            typedef phantom::SmallVector<LambdaCapture*> LambdaCaptures;
            typedef phantom::SmallVector<LambdaExpression*> LambdaExpressions;
            typedef phantom::SmallVector<PrimaryExpression*> PrimaryExpressions;
            struct _BaseRule
            {
                _BaseRule(ERuleType t = e_RuleTypeUndefined) : __type(t) {}
                spell::Location location() { return __location; }
                ERuleType type() { return __type; }
                spell::Location __location;
                ERuleType __type;
            };
            struct Source : public _BaseRule
            {
                Source(ERuleType t = e_RuleType_Source)
                : _BaseRule(t)
                , m_ModuleDeclarations(nullptr)
                , m_ImportDeclarations(nullptr)
                , m_SourceDeclarations(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::ModuleDeclaration*,4ull>* m_ModuleDeclarations;
                phantom::SmallVector<phantom::lang::ast::ImportDeclaration*,4ull>* m_ImportDeclarations;
                phantom::SmallVector<phantom::lang::ast::SourceDeclaration*,4ull>* m_SourceDeclarations;
            };
            struct ModuleDeclaration : public _BaseRule
            {
                ModuleDeclaration(ERuleType t = e_RuleType_ModuleDeclaration)
                : _BaseRule(t)
                , m_Names(nullptr)
                {
                }
                phantom::SmallVector<char const*,4ull>* m_Names;
            };
            struct ImportDeclaration : public _BaseRule
            {
                ImportDeclaration(ERuleType t = e_RuleType_ImportDeclaration)
                : _BaseRule(t)
                , m_Imports(nullptr)
                , m_Names(nullptr)
                {
                }
                spell::any m_STATIC;
                spell::any m_PUBLIC;
                phantom::SmallVector<phantom::lang::ast::Import*,4ull>* m_Imports;
                spell::any m_LT;
                phantom::SmallVector<spell::any,4ull>* m_Names;
                spell::any m_GT;
                spell::any m_STRING_LIT;
            };
            struct Import : public _BaseRule
            {
                Import(ERuleType t = e_RuleType_Import)
                : _BaseRule(t)
                , m_ImportBase(nullptr)
                , m_ImportBindings(nullptr)
                {
                }
                phantom::lang::ast::ImportBase* m_ImportBase;
                phantom::SmallVector<phantom::lang::ast::ImportBinding*,4ull>* m_ImportBindings;
            };
            struct ImportBinding : public _BaseRule
            {
                ImportBinding(ERuleType t = e_RuleType_ImportBinding)
                : _BaseRule(t)
                , m_IDENTIFIER(nullptr)
                {
                }
                char const* m_IDENTIFIER;
                spell::any m_ASSIGN;
                spell::any m_IDENTIFIER0;
            };
            struct ImportBase : public _BaseRule
            {
                ImportBase(ERuleType t = e_RuleType_ImportBase)
                : _BaseRule(t)
                , m_Names(nullptr)
                {
                }
                spell::any m_Alias;
                spell::any m_ASSIGN;
                phantom::SmallVector<spell::any,4ull>* m_Names;
            };
            struct Meta : public _BaseRule
            {
                Meta(ERuleType t = e_RuleType_Meta)
                : _BaseRule(t)
                , m_MetaDecls(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::MetaDecl*,4ull>* m_MetaDecls;
            };
            struct MetaDecl : public _BaseRule
            {
                MetaDecl(ERuleType t = e_RuleType_MetaDecl)
                : _BaseRule(t)
                , m_Key(nullptr)
                , m_Value(nullptr)
                {
                }
                char const* m_Key;
                spell::any m_ASSIGN;
                phantom::lang::ast::Expression* m_Value;
            };
            struct Annotation : public _BaseRule
            {
                Annotation(ERuleType t = e_RuleType_Annotation)
                : _BaseRule(t)
                , m_IDENTIFIER(nullptr)
                {
                }
                char const* m_IDENTIFIER;
            };
            struct MetaOrAnnotation : public _BaseRule
            {
                MetaOrAnnotation(ERuleType t = e_RuleType_MetaOrAnnotation)
                : _BaseRule(t)
                , m_Meta(nullptr)
                , m_Annotation(nullptr)
                {
                }
                phantom::lang::ast::Meta* m_Meta;
                phantom::lang::ast::Annotation* m_Annotation;
            };
            struct SourceDeclaration : public _BaseRule
            {
                SourceDeclaration(ERuleType t = e_RuleType_SourceDeclaration)
                : _BaseRule(t)
                , m_MetaOrAnnotations(nullptr)
                , m_Declaration(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::MetaOrAnnotation*,4ull>* m_MetaOrAnnotations;
                phantom::lang::ast::Declaration* m_Declaration;
            };
            struct Declaration : public _BaseRule
            {
                Declaration(ERuleType t = e_RuleType_Declaration)
                : _BaseRule(t)
                , m_Class(nullptr)
                , m_Enum(nullptr)
                , m_Alias(nullptr)
                , m_Using(nullptr)
                , m_Typedef(nullptr)
                , m_Template(nullptr)
                , m_MixinStatementDefinition(nullptr)
                , m_StaticAssert(nullptr)
                , m_FunctionOrVariable(nullptr)
                {
                }
                phantom::lang::ast::Class* m_Class;
                phantom::lang::ast::Enum* m_Enum;
                phantom::lang::ast::Alias* m_Alias;
                phantom::lang::ast::Using* m_Using;
                phantom::lang::ast::Typedef* m_Typedef;
                phantom::lang::ast::Template* m_Template;
                phantom::lang::ast::MixinStatementDefinition* m_MixinStatementDefinition;
                phantom::lang::ast::StaticAssert* m_StaticAssert;
                phantom::lang::ast::FunctionOrVariable* m_FunctionOrVariable;
            };
            struct MixinStatementDefinition : public _BaseRule
            {
                MixinStatementDefinition(ERuleType t = e_RuleType_MixinStatementDefinition)
                : _BaseRule(t)
                , m_ATAT(nullptr)
                , m_IDENTIFIER(nullptr)
                , m_PAREN_START(nullptr)
                , m_IDENTIFIERs(nullptr)
                , m_Statements(nullptr)
                {
                }
                char const* m_ATAT;
                char const* m_IDENTIFIER;
                char const* m_PAREN_START;
                phantom::SmallVector<spell::any,4ull>* m_IDENTIFIERs;
                phantom::SmallVector<phantom::lang::ast::Statement*,4ull>* m_Statements;
            };
            struct ForeachSignature : public _BaseRule
            {
                ForeachSignature(ERuleType t = e_RuleType_ForeachSignature)
                : _BaseRule(t)
                , m_TypeOrAuto(nullptr)
                , m_IDENTIFIER(nullptr)
                , m_Expression(nullptr)
                {
                }
                phantom::lang::ast::Type* m_TypeOrAuto;
                char const* m_IDENTIFIER;
                phantom::lang::ast::Expression* m_Expression;
            };
            struct ConversionFunction : public _BaseRule
            {
                ConversionFunction(ERuleType t = e_RuleType_ConversionFunction)
                : _BaseRule(t)
                , m_TypeNoFunctionType(nullptr)
                , m_PAREN_START(nullptr)
                , m_CONSTs(nullptr)
                , m_FunctionBlock(nullptr)
                {
                }
                phantom::lang::ast::Type* m_TypeNoFunctionType;
                char const* m_PAREN_START;
                phantom::SmallVector<spell::any,4ull>* m_CONSTs;
                phantom::lang::ast::FunctionBlock* m_FunctionBlock;
            };
            struct Constructor : public _BaseRule
            {
                Constructor(ERuleType t = e_RuleType_Constructor)
                : _BaseRule(t)
                , m_IDENTIFIER(nullptr)
                , m_TemplateArgumentList(nullptr)
                , m_PAREN_START(nullptr)
                , m_Parameters(nullptr)
                , m_ConstructorInitializers(nullptr)
                , m_FunctionBlock(nullptr)
                {
                }
                char const* m_IDENTIFIER;
                phantom::lang::ast::TemplateArgumentList* m_TemplateArgumentList;
                char const* m_PAREN_START;
                phantom::SmallVector<phantom::lang::ast::Parameter*,4ull>* m_Parameters;
                spell::any m_Suffix;
                phantom::SmallVector<phantom::lang::ast::ConstructorInitializer*,4ull>* m_ConstructorInitializers;
                phantom::lang::ast::FunctionBlock* m_FunctionBlock;
            };
            struct Destructor : public _BaseRule
            {
                Destructor(ERuleType t = e_RuleType_Destructor)
                : _BaseRule(t)
                , m_COMPL(nullptr)
                , m_IDENTIFIER(nullptr)
                , m_PAREN_START(nullptr)
                , m_OVERRIDEs(nullptr)
                , m_FunctionBlock(nullptr)
                {
                }
                spell::any m_VIRTUAL;
                char const* m_COMPL;
                char const* m_IDENTIFIER;
                char const* m_PAREN_START;
                phantom::SmallVector<spell::any,4ull>* m_OVERRIDEs;
                phantom::lang::ast::FunctionBlock* m_FunctionBlock;
            };
            struct ConstructorInitializer : public _BaseRule
            {
                ConstructorInitializer(ERuleType t = e_RuleType_ConstructorInitializer)
                : _BaseRule(t)
                , m_BasicType(nullptr)
                , m_CallList(nullptr)
                , m_InitializerList(nullptr)
                {
                }
                phantom::lang::ast::Type* m_BasicType;
                phantom::lang::ast::CallList* m_CallList;
                phantom::lang::ast::Expression* m_InitializerList;
            };
            struct ArrowReturn : public _BaseRule
            {
                ArrowReturn(ERuleType t = e_RuleType_ArrowReturn)
                : _BaseRule(t)
                , m_ARROW(nullptr)
                , m_Type(nullptr)
                {
                }
                char const* m_ARROW;
                phantom::lang::ast::Type* m_Type;
            };
            struct FunctionEnd : public _BaseRule
            {
                FunctionEnd(ERuleType t = e_RuleType_FunctionEnd)
                : _BaseRule(t)
                , m_PAREN_START(nullptr)
                , m_Parameters(nullptr)
                , m_CONSTs(nullptr)
                , m_ArrowReturn(nullptr)
                , m_OVERRIDEs(nullptr)
                , m_FunctionBlock(nullptr)
                {
                }
                char const* m_PAREN_START;
                phantom::SmallVector<phantom::lang::ast::Parameter*,4ull>* m_Parameters;
                phantom::SmallVector<spell::any,4ull>* m_CONSTs;
                phantom::lang::ast::ArrowReturn* m_ArrowReturn;
                phantom::SmallVector<spell::any,4ull>* m_OVERRIDEs;
                phantom::lang::ast::FunctionBlock* m_FunctionBlock;
                spell::any m_Suffix;
            };
            struct Operator : public _BaseRule
            {
                Operator(ERuleType t = e_RuleType_Operator)
                : _BaseRule(t)
                , m_OpParen(nullptr)
                , m_OpBracket(nullptr)
                , m_RightShiftAssign(nullptr)
                , m_RightShift(nullptr)
                {
                }
                phantom::SmallVector<spell::any,4ull>* m_OpParen;
                phantom::SmallVector<spell::any,4ull>* m_OpBracket;
                phantom::SmallVector<spell::any,4ull>* m_RightShiftAssign;
                phantom::SmallVector<spell::any,4ull>* m_RightShift;
                spell::any m_GreaterThan;
                spell::any m_ASSIGN;
            };
            struct OperatorEnd : public _BaseRule
            {
                OperatorEnd(ERuleType t = e_RuleType_OperatorEnd)
                : _BaseRule(t)
                , m_Operator(nullptr)
                , m_FunctionEnd(nullptr)
                {
                }
                phantom::lang::ast::Operator* m_Operator;
                phantom::lang::ast::FunctionEnd* m_FunctionEnd;
            };
            struct FunctionOrVariableEnd : public _BaseRule
            {
                FunctionOrVariableEnd(ERuleType t = e_RuleType_FunctionOrVariableEnd)
                : _BaseRule(t)
                , m_IDENTIFIER(nullptr)
                , m_InitializerList(nullptr)
                , m_ArrayExtents(nullptr)
                , m_AssignExpressionOrInitializerList(nullptr)
                , m_Declarators(nullptr)
                , m_TemplateArgumentList(nullptr)
                , m_FunctionEnd(nullptr)
                , m_CallList(nullptr)
                {
                }
                char const* m_IDENTIFIER;
                phantom::lang::ast::Expression* m_InitializerList;
                phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>* m_ArrayExtents;
                phantom::lang::ast::Expression* m_AssignExpressionOrInitializerList;
                phantom::SmallVector<phantom::lang::ast::Declarator*,4ull>* m_Declarators;
                phantom::lang::ast::TemplateArgumentList* m_TemplateArgumentList;
                phantom::lang::ast::FunctionEnd* m_FunctionEnd;
                phantom::lang::ast::CallList* m_CallList;
            };
            struct MethodSpecifier : public _BaseRule
            {
                MethodSpecifier(ERuleType t = e_RuleType_MethodSpecifier)
                : _BaseRule(t)
                , m_STATIC(nullptr)
                {
                }
                char const* m_STATIC;
            };
            struct Specifier : public _BaseRule
            {
                Specifier(ERuleType t = e_RuleType_Specifier)
                : _BaseRule(t)
                , m_MethodSpecifier(nullptr)
                {
                }
                phantom::lang::ast::MethodSpecifier* m_MethodSpecifier;
            };
            struct FunctionOrVariable : public _BaseRule
            {
                FunctionOrVariable(ERuleType t = e_RuleType_FunctionOrVariable)
                : _BaseRule(t)
                , m_Specifiers(nullptr)
                , m_TypeOrAuto(nullptr)
                , m_OperatorEnd(nullptr)
                , m_FunctionOrVariableEnd(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::Specifier*,4ull>* m_Specifiers;
                spell::any m_PAREN_START;
                spell::any m_AlignAs;
                phantom::lang::ast::Type* m_TypeOrAuto;
                phantom::lang::ast::OperatorEnd* m_OperatorEnd;
                phantom::lang::ast::FunctionOrVariableEnd* m_FunctionOrVariableEnd;
            };
            struct DeclType : public _BaseRule
            {
                DeclType(ERuleType t = e_RuleType_DeclType)
                : _BaseRule(t)
                , m_PAREN_START(nullptr)
                , m_Expression(nullptr)
                {
                }
                char const* m_PAREN_START;
                phantom::lang::ast::Expression* m_Expression;
            };
            struct TemplateArgument : public _BaseRule
            {
                TemplateArgument(ERuleType t = e_RuleType_TemplateArgument)
                : _BaseRule(t)
                {
                }
            };
            struct ForInit : public _BaseRule
            {
                ForInit(ERuleType t = e_RuleType_ForInit)
                : _BaseRule(t)
                , m_LocalVariableDecl(nullptr)
                , m_Expression(nullptr)
                {
                }
                phantom::lang::ast::LocalVariableDecl* m_LocalVariableDecl;
                phantom::lang::ast::Expression* m_Expression;
            };
            struct Statement : public _BaseRule
            {
                Statement(ERuleType t = e_RuleType_Statement)
                : _BaseRule(t)
                , m_IfStatement(nullptr)
                , m_ForStatement(nullptr)
                , m_SwitchStatement(nullptr)
                , m_WhileStatement(nullptr)
                , m_AssertStatement(nullptr)
                , m_BreakStatement(nullptr)
                , m_ContinueStatement(nullptr)
                , m_DoWhileStatement(nullptr)
                , m_BlockStatement(nullptr)
                , m_ReturnStatement(nullptr)
                , m_GotoStatement(nullptr)
                , m_BlockDeclaration(nullptr)
                , m_MixinStatement(nullptr)
                , m_LabelStatement(nullptr)
                , m_LocalVariableStatement(nullptr)
                , m_ExpressionStatement(nullptr)
                {
                }
                phantom::lang::ast::Statement* m_IfStatement;
                phantom::lang::ast::Statement* m_ForStatement;
                phantom::lang::ast::Statement* m_SwitchStatement;
                phantom::lang::ast::Statement* m_WhileStatement;
                phantom::lang::ast::Statement* m_AssertStatement;
                phantom::lang::ast::Statement* m_BreakStatement;
                phantom::lang::ast::Statement* m_ContinueStatement;
                phantom::lang::ast::Statement* m_DoWhileStatement;
                phantom::lang::ast::Statement* m_BlockStatement;
                phantom::lang::ast::ReturnStatement* m_ReturnStatement;
                phantom::lang::ast::GotoStatement* m_GotoStatement;
                phantom::lang::ast::Statement* m_BlockDeclaration;
                phantom::lang::ast::MixinStatement* m_MixinStatement;
                phantom::lang::ast::Statement* m_LabelStatement;
                phantom::lang::ast::Statement* m_LocalVariableStatement;
                phantom::lang::ast::Statement* m_ExpressionStatement;
            };
            struct MixinStatement : public _BaseRule
            {
                MixinStatement(ERuleType t = e_RuleType_MixinStatement)
                : _BaseRule(t)
                , m_ATAT(nullptr)
                , m_IDENTIFIER(nullptr)
                , m_PAREN_START(nullptr)
                , m_Expressions(nullptr)
                {
                }
                char const* m_ATAT;
                char const* m_IDENTIFIER;
                char const* m_PAREN_START;
                phantom::SmallVector<phantom::lang::ast::Expression*,4ull>* m_Expressions;
            };
            struct ReturnStatement : public _BaseRule
            {
                ReturnStatement(ERuleType t = e_RuleType_ReturnStatement)
                : _BaseRule(t)
                , m_pExpression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_pExpression;
            };
            struct GotoStatement : public _BaseRule
            {
                GotoStatement(ERuleType t = e_RuleType_GotoStatement)
                : _BaseRule(t)
                , m_IDENTIFIER(nullptr)
                {
                }
                char const* m_IDENTIFIER;
            };
            struct Declarator : public _BaseRule
            {
                Declarator(ERuleType t = e_RuleType_Declarator)
                : _BaseRule(t)
                , m_TypeExtents(nullptr)
                , m_IDENTIFIER(nullptr)
                , m_ArrayExtents(nullptr)
                , m_AssignExpressionOrInitializerList(nullptr)
                , m_CallList(nullptr)
                , m_InitializerList(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>* m_TypeExtents;
                char const* m_IDENTIFIER;
                phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>* m_ArrayExtents;
                phantom::lang::ast::Expression* m_AssignExpressionOrInitializerList;
                phantom::lang::ast::CallList* m_CallList;
                phantom::lang::ast::Expression* m_InitializerList;
            };
            struct TemplateArgumentListSplit : public _BaseRule
            {
                TemplateArgumentListSplit(ERuleType t = e_RuleType_TemplateArgumentListSplit)
                : _BaseRule(t)
                , m_NotLastTemplateArguments(nullptr)
                , m_LastTemplateArgument(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::TemplateArgument*,4ull>* m_NotLastTemplateArguments;
                phantom::lang::ast::TemplateArgument* m_LastTemplateArgument;
            };
            struct TemplateArgumentNoAssign : public _BaseRule
            {
                TemplateArgumentNoAssign(ERuleType t = e_RuleType_TemplateArgumentNoAssign)
                : _BaseRule(t)
                , m_PostFixExpression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_PostFixExpression;
            };
            struct TemplateArgumentAssign : public _BaseRule
            {
                TemplateArgumentAssign(ERuleType t = e_RuleType_TemplateArgumentAssign)
                : _BaseRule(t)
                , m_Type(nullptr)
                {
                }
                spell::any m_INT_VALUE;
                phantom::lang::ast::Type* m_Type;
            };
            struct TemplateArgumentList : public _BaseRule
            {
                TemplateArgumentList(ERuleType t = e_RuleType_TemplateArgumentList)
                : _BaseRule(t)
                , m_TemplateArgumentListSplit(nullptr)
                {
                }
                spell::any m_GT;
                phantom::lang::ast::TemplateArgumentListSplit* m_TemplateArgumentListSplit;
            };
            struct Name : public _BaseRule
            {
                Name(ERuleType t = e_RuleType_Name)
                : _BaseRule(t)
                , m_Operator(nullptr)
                , m_TemplateArgumentList(nullptr)
                {
                }
                phantom::lang::ast::Operator* m_Operator;
                spell::any m_COMPL;
                spell::any m_IDENTIFIER;
                phantom::lang::ast::TemplateArgumentList* m_TemplateArgumentList;
            };
            struct FundamentalTypeBase : public _BaseRule
            {
                FundamentalTypeBase(ERuleType t = e_RuleType_FundamentalTypeBase)
                : _BaseRule(t)
                , m_Typename(nullptr)
                {
                }
                char const* m_Typename;
            };
            struct AutoDeclarator : public _BaseRule
            {
                AutoDeclarator(ERuleType t = e_RuleType_AutoDeclarator)
                : _BaseRule(t)
                , m_TypeExtents(nullptr)
                , m_IDENTIFIER(nullptr)
                , m_CallList(nullptr)
                , m_InitializerList(nullptr)
                , m_ArrayExtents(nullptr)
                , m_AssignExpressionOrInitializerList(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>* m_TypeExtents;
                char const* m_IDENTIFIER;
                phantom::lang::ast::CallList* m_CallList;
                phantom::lang::ast::Expression* m_InitializerList;
                phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>* m_ArrayExtents;
                phantom::lang::ast::Expression* m_AssignExpressionOrInitializerList;
            };
            struct LocalVariableDecl : public _BaseRule
            {
                LocalVariableDecl(ERuleType t = e_RuleType_LocalVariableDecl)
                : _BaseRule(t)
                , m_AutoLocalVariableDecl(nullptr)
                , m_ExplicitLocalVariableDecl(nullptr)
                {
                }
                phantom::lang::ast::LocalVariableDecl* m_AutoLocalVariableDecl;
                phantom::lang::ast::LocalVariableDecl* m_ExplicitLocalVariableDecl;
            };
            struct Expression : public _BaseRule
            {
                Expression(ERuleType t = e_RuleType_Expression)
                : _BaseRule(t)
                , m_Expression_AssignExpression(nullptr)
                , m_Expression_Expression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_Expression_AssignExpression;
                phantom::lang::ast::Expression* m_Expression_Expression;
            };
            struct LogicalOrExpressionRight : public _BaseRule
            {
                LogicalOrExpressionRight(ERuleType t = e_RuleType_LogicalOrExpressionRight)
                : _BaseRule(t)
                , m_Op(nullptr)
                , m_RightExpression(nullptr)
                {
                }
                char const* m_Op;
                phantom::lang::ast::Expression* m_RightExpression;
            };
            struct LogicalAndExpressionRight : public _BaseRule
            {
                LogicalAndExpressionRight(ERuleType t = e_RuleType_LogicalAndExpressionRight)
                : _BaseRule(t)
                , m_Op(nullptr)
                , m_RightExpression(nullptr)
                {
                }
                char const* m_Op;
                phantom::lang::ast::Expression* m_RightExpression;
            };
            struct BinOrExpressionRight : public _BaseRule
            {
                BinOrExpressionRight(ERuleType t = e_RuleType_BinOrExpressionRight)
                : _BaseRule(t)
                , m_Op(nullptr)
                , m_RightExpression(nullptr)
                {
                }
                char const* m_Op;
                phantom::lang::ast::Expression* m_RightExpression;
            };
            struct XOrExpressionRight : public _BaseRule
            {
                XOrExpressionRight(ERuleType t = e_RuleType_XOrExpressionRight)
                : _BaseRule(t)
                , m_Op(nullptr)
                , m_RightExpression(nullptr)
                {
                }
                char const* m_Op;
                phantom::lang::ast::Expression* m_RightExpression;
            };
            struct BinAndExpressionRight : public _BaseRule
            {
                BinAndExpressionRight(ERuleType t = e_RuleType_BinAndExpressionRight)
                : _BaseRule(t)
                , m_Op(nullptr)
                , m_RightExpression(nullptr)
                {
                }
                char const* m_Op;
                phantom::lang::ast::Expression* m_RightExpression;
            };
            struct EqualityExpressionRight : public _BaseRule
            {
                EqualityExpressionRight(ERuleType t = e_RuleType_EqualityExpressionRight)
                : _BaseRule(t)
                , m_Op(nullptr)
                , m_RightExpression(nullptr)
                {
                }
                char const* m_Op;
                phantom::lang::ast::Expression* m_RightExpression;
            };
            struct RelationalExpressionRight : public _BaseRule
            {
                RelationalExpressionRight(ERuleType t = e_RuleType_RelationalExpressionRight)
                : _BaseRule(t)
                , m_OpMul(nullptr)
                , m_RightExpression(nullptr)
                {
                }
                phantom::SmallVector<spell::any,4ull>* m_OpMul;
                phantom::lang::ast::Expression* m_RightExpression;
                spell::any m_Op;
            };
            struct Type : public _BaseRule
            {
                Type(ERuleType t = e_RuleType_Type)
                : _BaseRule(t)
                , m_ExplicitType(nullptr)
                , m_ExpressionType(nullptr)
                {
                }
                phantom::lang::ast::Type* m_ExplicitType;
                phantom::lang::ast::Type* m_ExpressionType;
            };
            struct ShiftExpressionRight : public _BaseRule
            {
                ShiftExpressionRight(ERuleType t = e_RuleType_ShiftExpressionRight)
                : _BaseRule(t)
                , m_OpMul(nullptr)
                , m_RightExpression(nullptr)
                {
                }
                phantom::SmallVector<spell::any,4ull>* m_OpMul;
                phantom::lang::ast::Expression* m_RightExpression;
                spell::any m_Op;
            };
            struct AddExpressionRight : public _BaseRule
            {
                AddExpressionRight(ERuleType t = e_RuleType_AddExpressionRight)
                : _BaseRule(t)
                , m_Op(nullptr)
                , m_RightExpression(nullptr)
                {
                }
                char const* m_Op;
                phantom::lang::ast::Expression* m_RightExpression;
            };
            struct MulExpressionRight : public _BaseRule
            {
                MulExpressionRight(ERuleType t = e_RuleType_MulExpressionRight)
                : _BaseRule(t)
                , m_Op(nullptr)
                , m_RightExpression(nullptr)
                {
                }
                char const* m_Op;
                phantom::lang::ast::Expression* m_RightExpression;
            };
            struct SpecialCastExpression : public _BaseRule
            {
                SpecialCastExpression(ERuleType t = e_RuleType_SpecialCastExpression)
                : _BaseRule(t)
                , m_CastKind(nullptr)
                , m_Type(nullptr)
                , m_PAREN_START(nullptr)
                , m_Expression(nullptr)
                {
                }
                char const* m_CastKind;
                phantom::lang::ast::Type* m_Type;
                char const* m_PAREN_START;
                phantom::lang::ast::Expression* m_Expression;
            };
            struct FunctionPtrExpression : public _BaseRule
            {
                FunctionPtrExpression(ERuleType t = e_RuleType_FunctionPtrExpression)
                : _BaseRule(t)
                , m_Name(nullptr)
                {
                }
                phantom::lang::ast::Name* m_Name;
            };
            struct CallList : public _BaseRule
            {
                CallList(ERuleType t = e_RuleType_CallList)
                : _BaseRule(t)
                , m_PAREN_START(nullptr)
                , m_AssignExpressionOrInitializerLists(nullptr)
                {
                }
                char const* m_PAREN_START;
                phantom::SmallVector<phantom::lang::ast::Expression*,4ull>* m_AssignExpressionOrInitializerLists;
            };
            struct PostFixEnd : public _BaseRule
            {
                PostFixEnd(ERuleType t = e_RuleType_PostFixEnd)
                : _BaseRule(t)
                , m_CallList(nullptr)
                , m_ArrayAccess(nullptr)
                , m_InitializerList(nullptr)
                , m_Name(nullptr)
                {
                }
                phantom::lang::ast::CallList* m_CallList;
                phantom::lang::ast::Expression* m_ArrayAccess;
                phantom::lang::ast::Expression* m_InitializerList;
                spell::any m_DOT;
                phantom::lang::ast::Name* m_Name;
                spell::any m_ELLIPSE;
                spell::any m_INC;
            };
            struct ArrayExtentExpression : public _BaseRule
            {
                ArrayExtentExpression(ERuleType t = e_RuleType_ArrayExtentExpression)
                : _BaseRule(t)
                , m_IntegralConstant(nullptr)
                , m_AssignExpression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_IntegralConstant;
                phantom::lang::ast::Expression* m_AssignExpression;
            };
            struct StaticAssert : public _BaseRule
            {
                StaticAssert(ERuleType t = e_RuleType_StaticAssert)
                : _BaseRule(t)
                , m_PAREN_START(nullptr)
                , m_AssignExpression(nullptr)
                {
                }
                char const* m_PAREN_START;
                phantom::lang::ast::Expression* m_AssignExpression;
                spell::any m_STRING_LIT;
            };
            struct StringLiteralChain : public _BaseRule
            {
                StringLiteralChain(ERuleType t = e_RuleType_StringLiteralChain)
                : _BaseRule(t)
                , m_STRING_LIT(nullptr)
                , m_STRING_LITs(nullptr)
                {
                }
                char const* m_STRING_LIT;
                phantom::SmallVector<spell::any,4ull>* m_STRING_LITs;
            };
            struct LambdaCapture : public _BaseRule
            {
                LambdaCapture(ERuleType t = e_RuleType_LambdaCapture)
                : _BaseRule(t)
                {
                }
                spell::any m_IDENTIFIER;
                spell::any m_ASSIGN;
                spell::any m_BIN_AND;
                spell::any m_THIS;
            };
            struct LambdaExpression : public _BaseRule
            {
                LambdaExpression(ERuleType t = e_RuleType_LambdaExpression)
                : _BaseRule(t)
                , m_LambdaCaptures(nullptr)
                , m_ArrowReturn(nullptr)
                , m_PAREN_START(nullptr)
                , m_Parameters(nullptr)
                , m_FunctionBlock(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::LambdaCapture*,4ull>* m_LambdaCaptures;
                phantom::lang::ast::ArrowReturn* m_ArrowReturn;
                char const* m_PAREN_START;
                phantom::SmallVector<phantom::lang::ast::Parameter*,4ull>* m_Parameters;
                phantom::lang::ast::FunctionBlock* m_FunctionBlock;
            };
            struct TypeExtent : public _BaseRule
            {
                TypeExtent(ERuleType t = e_RuleType_TypeExtent)
                : _BaseRule(t)
                , m_ArrayExtent(nullptr)
                , m_FunctionTypeExtent(nullptr)
                , m_DerivedExtent(nullptr)
                {
                }
                phantom::lang::ast::TypeExtent* m_ArrayExtent;
                phantom::lang::ast::TypeExtent* m_FunctionTypeExtent;
                phantom::lang::ast::TypeExtent* m_DerivedExtent;
            };
            struct Enum : public _BaseRule
            {
                Enum(ERuleType t = e_RuleType_Enum)
                : _BaseRule(t)
                , m_IDENTIFIER(nullptr)
                , m_EnumConstants(nullptr)
                {
                }
                spell::any m_CLASS;
                char const* m_IDENTIFIER;
                phantom::SmallVector<phantom::lang::ast::EnumConstant*,4ull>* m_EnumConstants;
            };
            struct EnumConstant : public _BaseRule
            {
                EnumConstant(ERuleType t = e_RuleType_EnumConstant)
                : _BaseRule(t)
                , m_IDENTIFIER(nullptr)
                , m_AssignExpression(nullptr)
                {
                }
                char const* m_IDENTIFIER;
                phantom::lang::ast::Expression* m_AssignExpression;
            };
            struct Using : public _BaseRule
            {
                Using(ERuleType t = e_RuleType_Using)
                : _BaseRule(t)
                , m_QualifiedName(nullptr)
                {
                }
                phantom::lang::ast::QualifiedName* m_QualifiedName;
            };
            struct Typedef : public _BaseRule
            {
                Typedef(ERuleType t = e_RuleType_Typedef)
                : _BaseRule(t)
                , m_Type(nullptr)
                , m_IDENTIFIER(nullptr)
                {
                }
                phantom::lang::ast::Type* m_Type;
                char const* m_IDENTIFIER;
            };
            struct Alias : public _BaseRule
            {
                Alias(ERuleType t = e_RuleType_Alias)
                : _BaseRule(t)
                , m_IDENTIFIER(nullptr)
                , m_ASSIGN(nullptr)
                , m_Type(nullptr)
                {
                }
                char const* m_IDENTIFIER;
                char const* m_ASSIGN;
                phantom::lang::ast::Type* m_Type;
            };
            struct TemplateParameterDefault : public _BaseRule
            {
                TemplateParameterDefault(ERuleType t = e_RuleType_TemplateParameterDefault)
                : _BaseRule(t)
                , m_TemplateArgumentAssign(nullptr)
                , m_TemplateArgumentNoAssign(nullptr)
                {
                }
                phantom::lang::ast::TemplateArgumentAssign* m_TemplateArgumentAssign;
                phantom::lang::ast::TemplateArgumentNoAssign* m_TemplateArgumentNoAssign;
            };
            struct TemplateValueParameter : public _BaseRule
            {
                TemplateValueParameter(ERuleType t = e_RuleType_TemplateValueParameter)
                : _BaseRule(t)
                , m_QualifiedName(nullptr)
                , m_FundamentalType(nullptr)
                , m_IDENTIFIER(nullptr)
                {
                }
                phantom::lang::ast::QualifiedName* m_QualifiedName;
                phantom::lang::ast::Type* m_FundamentalType;
                spell::any m_ELLIPSE;
                char const* m_IDENTIFIER;
            };
            struct TemplateTypeParameter : public _BaseRule
            {
                TemplateTypeParameter(ERuleType t = e_RuleType_TemplateTypeParameter)
                : _BaseRule(t)
                , m_IDENTIFIER(nullptr)
                {
                }
                spell::any m_ELLIPSE;
                char const* m_IDENTIFIER;
            };
            struct TemplateParameter : public _BaseRule
            {
                TemplateParameter(ERuleType t = e_RuleType_TemplateParameter)
                : _BaseRule(t)
                , m_TemplateValueParameter(nullptr)
                , m_TemplateTypeParameter(nullptr)
                , m_TemplateParameterDefault(nullptr)
                {
                }
                phantom::lang::ast::TemplateValueParameter* m_TemplateValueParameter;
                phantom::lang::ast::TemplateTypeParameter* m_TemplateTypeParameter;
                phantom::lang::ast::TemplateParameterDefault* m_TemplateParameterDefault;
            };
            struct TemplateSignature : public _BaseRule
            {
                TemplateSignature(ERuleType t = e_RuleType_TemplateSignature)
                : _BaseRule(t)
                , m_TemplateParameters(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::TemplateParameter*,4ull>* m_TemplateParameters;
            };
            struct Template : public _BaseRule
            {
                Template(ERuleType t = e_RuleType_Template)
                : _BaseRule(t)
                , m_TemplateSignature(nullptr)
                , m_Class(nullptr)
                , m_Union(nullptr)
                , m_Alias(nullptr)
                , m_TemplateFunction(nullptr)
                , m_Constructor(nullptr)
                {
                }
                phantom::lang::ast::TemplateSignature* m_TemplateSignature;
                phantom::lang::ast::Class* m_Class;
                phantom::lang::ast::Union* m_Union;
                phantom::lang::ast::Alias* m_Alias;
                phantom::lang::ast::TemplateFunction* m_TemplateFunction;
                phantom::lang::ast::Constructor* m_Constructor;
            };
            struct TemplateFunction : public _BaseRule
            {
                TemplateFunction(ERuleType t = e_RuleType_TemplateFunction)
                : _BaseRule(t)
                , m_TypeOrAuto(nullptr)
                , m_IDENTIFIER(nullptr)
                , m_FunctionEnd(nullptr)
                {
                }
                spell::any m_STATIC;
                phantom::lang::ast::Type* m_TypeOrAuto;
                char const* m_IDENTIFIER;
                phantom::lang::ast::FunctionEnd* m_FunctionEnd;
            };
            struct FunctionBlock : public _BaseRule
            {
                FunctionBlock(ERuleType t = e_RuleType_FunctionBlock)
                : _BaseRule(t)
                , m_Statements(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::Statement*,4ull>* m_Statements;
            };
            struct QualifiedName : public _BaseRule
            {
                QualifiedName(ERuleType t = e_RuleType_QualifiedName)
                : _BaseRule(t)
                , m_Scope(nullptr)
                , m_Dot(nullptr)
                {
                }
                spell::any m_TYPENAME;
                phantom::lang::ast::QualifiedScopedName* m_Scope;
                phantom::lang::ast::QualifiedDotName* m_Dot;
            };
            struct QualifiedDotName : public _BaseRule
            {
                QualifiedDotName(ERuleType t = e_RuleType_QualifiedDotName)
                : _BaseRule(t)
                , m_Names(nullptr)
                {
                }
                spell::any m_DOT;
                phantom::SmallVector<phantom::lang::ast::Name*,4ull>* m_Names;
            };
            struct QualifiedScopedName : public _BaseRule
            {
                QualifiedScopedName(ERuleType t = e_RuleType_QualifiedScopedName)
                : _BaseRule(t)
                , m_Names(nullptr)
                , m_Name(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::Name*,4ull>* m_Names;
                phantom::lang::ast::Name* m_Name;
                spell::any m_SCOPE;
            };
            struct BaseClass : public _BaseRule
            {
                BaseClass(ERuleType t = e_RuleType_BaseClass)
                : _BaseRule(t)
                , m_BasicType(nullptr)
                {
                }
                spell::any m_PUBLIC;
                phantom::lang::ast::Type* m_BasicType;
            };
            struct Class : public _BaseRule
            {
                Class(ERuleType t = e_RuleType_Class)
                : _BaseRule(t)
                , m_CLASS(nullptr)
                , m_MetaClassConstructionExpression(nullptr)
                , m_IDENTIFIER(nullptr)
                , m_TemplateArgumentList(nullptr)
                , m_BaseClasses(nullptr)
                , m_Members(nullptr)
                {
                }
                char const* m_CLASS;
                spell::any m_PAREN_START;
                phantom::lang::ast::Expression* m_MetaClassConstructionExpression;
                spell::any m_PAREN_START0;
                spell::any m_AlignAs;
                char const* m_IDENTIFIER;
                phantom::lang::ast::TemplateArgumentList* m_TemplateArgumentList;
                phantom::SmallVector<phantom::lang::ast::BaseClass*,4ull>* m_BaseClasses;
                phantom::SmallVector<phantom::lang::ast::Member*,4ull>* m_Members;
            };
            struct Union : public _BaseRule
            {
                Union(ERuleType t = e_RuleType_Union)
                : _BaseRule(t)
                , m_IDENTIFIER(nullptr)
                , m_TemplateArgumentList(nullptr)
                , m_BasicMembers(nullptr)
                {
                }
                char const* m_IDENTIFIER;
                phantom::lang::ast::TemplateArgumentList* m_TemplateArgumentList;
                phantom::SmallVector<phantom::lang::ast::Member*,4ull>* m_BasicMembers;
            };
            struct Method : public _BaseRule
            {
                Method(ERuleType t = e_RuleType_Method)
                : _BaseRule(t)
                , m_MethodSpecifiers(nullptr)
                , m_TypeOrAuto(nullptr)
                , m_IDENTIFIER(nullptr)
                , m_FunctionEnd(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::MethodSpecifier*,4ull>* m_MethodSpecifiers;
                phantom::lang::ast::Type* m_TypeOrAuto;
                char const* m_IDENTIFIER;
                phantom::lang::ast::FunctionEnd* m_FunctionEnd;
            };
            struct Parameter : public _BaseRule
            {
                Parameter(ERuleType t = e_RuleType_Parameter)
                : _BaseRule(t)
                , m_Type(nullptr)
                , m_ArrayExtents(nullptr)
                , m_AssignExpressionOrInitializerList(nullptr)
                {
                }
                phantom::lang::ast::Type* m_Type;
                spell::any m_IDENTIFIER;
                phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>* m_ArrayExtents;
                spell::any m_ASSIGN;
                phantom::lang::ast::Expression* m_AssignExpressionOrInitializerList;
            };
            struct AccessSpecifier : public _BaseRule
            {
                AccessSpecifier(ERuleType t = e_RuleType_AccessSpecifier)
                : _BaseRule(t)
                , m_PUBLIC(nullptr)
                {
                }
                char const* m_PUBLIC;
            };
            struct FieldOverride : public _BaseRule
            {
                FieldOverride(ERuleType t = e_RuleType_FieldOverride)
                : _BaseRule(t)
                , m_DEFAULT(nullptr)
                , m_IDENTIFIER(nullptr)
                , m_ASSIGN(nullptr)
                , m_AssignExpression(nullptr)
                {
                }
                char const* m_DEFAULT;
                char const* m_IDENTIFIER;
                char const* m_ASSIGN;
                phantom::lang::ast::Expression* m_AssignExpression;
            };
            struct Symbol : public _BaseRule
            {
                Symbol(ERuleType t = e_RuleType_Symbol)
                : _BaseRule(t)
                , m_ExplicitType(nullptr)
                , m_PostFixExpression(nullptr)
                {
                }
                phantom::lang::ast::Type* m_ExplicitType;
                phantom::lang::ast::Expression* m_PostFixExpression;
            };
            struct Member : public _BaseRule
            {
                Member(ERuleType t = e_RuleType_Member)
                : _BaseRule(t)
                , m_MemberWithMetaOrAnnotation(nullptr)
                , m_BasicMember(nullptr)
                {
                }
                phantom::lang::ast::Member* m_MemberWithMetaOrAnnotation;
                phantom::lang::ast::Member* m_BasicMember;
            };
            struct IfOrWhileLocalVariableDecl : public _BaseRule
            {
                IfOrWhileLocalVariableDecl(ERuleType t = e_RuleType_IfOrWhileLocalVariableDecl)
                : _BaseRule(t)
                , m_TypeOrAuto(nullptr)
                , m_IDENTIFIER(nullptr)
                , m_Expression(nullptr)
                {
                }
                phantom::lang::ast::Type* m_TypeOrAuto;
                char const* m_IDENTIFIER;
                phantom::lang::ast::Expression* m_Expression;
            };
            struct IfOrWhileCondition : public _BaseRule
            {
                IfOrWhileCondition(ERuleType t = e_RuleType_IfOrWhileCondition)
                : _BaseRule(t)
                , m_IfOrWhileLocalVariableDecl(nullptr)
                , m_Expression(nullptr)
                {
                }
                phantom::lang::ast::IfOrWhileLocalVariableDecl* m_IfOrWhileLocalVariableDecl;
                phantom::lang::ast::Expression* m_Expression;
            };
            struct BlockDeclaration : public Statement
            {
                BlockDeclaration(ERuleType t = e_RuleType_BlockDeclaration)
                : Statement(t)
                , m_Class(nullptr)
                , m_Enum(nullptr)
                , m_Typedef(nullptr)
                , m_Alias(nullptr)
                , m_Using(nullptr)
                , m_StaticAssert(nullptr)
                , m_Alias0(nullptr)
                {
                }
                phantom::lang::ast::Class* m_Class;
                phantom::lang::ast::Enum* m_Enum;
                phantom::lang::ast::Typedef* m_Typedef;
                phantom::lang::ast::Alias* m_Alias;
                phantom::lang::ast::Using* m_Using;
                phantom::lang::ast::StaticAssert* m_StaticAssert;
                phantom::lang::ast::Alias* m_Alias0;
            };
            struct FundamentalTypeFunctionCast : public Expression
            {
                FundamentalTypeFunctionCast(ERuleType t = e_RuleType_FundamentalTypeFunctionCast)
                : Expression(t)
                , m_FundamentalType(nullptr)
                , m_PAREN_START(nullptr)
                , m_Expression(nullptr)
                {
                }
                phantom::lang::ast::Type* m_FundamentalType;
                char const* m_PAREN_START;
                phantom::lang::ast::Expression* m_Expression;
            };
            struct ExpressionStatement : public Statement
            {
                ExpressionStatement(ERuleType t = e_RuleType_ExpressionStatement)
                : Statement(t)
                , m_Expression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_Expression;
            };
            struct LastTemplateArgument : public TemplateArgument
            {
                LastTemplateArgument(ERuleType t = e_RuleType_LastTemplateArgument)
                : TemplateArgument(t)
                , m_LastTemplateArgumentAssign(nullptr)
                , m_LastTemplateArgumentNoAssign(nullptr)
                {
                }
                phantom::lang::ast::LastTemplateArgument* m_LastTemplateArgumentAssign;
                phantom::lang::ast::LastTemplateArgument* m_LastTemplateArgumentNoAssign;
            };
            struct SwitchStatement : public Statement
            {
                SwitchStatement(ERuleType t = e_RuleType_SwitchStatement)
                : Statement(t)
                , m_PAREN_START(nullptr)
                , m_Expression(nullptr)
                , m_StatementOrCases(nullptr)
                {
                }
                char const* m_PAREN_START;
                phantom::lang::ast::Expression* m_Expression;
                phantom::SmallVector<phantom::lang::ast::Statement*,4ull>* m_StatementOrCases;
            };
            struct PrimaryExpression : public Expression
            {
                PrimaryExpression(ERuleType t = e_RuleType_PrimaryExpression)
                : Expression(t)
                , m_KeywordExpression(nullptr)
                , m_SizeofExpression(nullptr)
                , m_TypeofExpression(nullptr)
                , m_NullptrExpression(nullptr)
                , m_Name(nullptr)
                , m_CStyleCastExpression(nullptr)
                , m_TypeTraitBinary(nullptr)
                , m_StringLiteralChain(nullptr)
                , m_SpecialCastExpression(nullptr)
                , m_FunctionPtrExpression(nullptr)
                , m_FundamentalTypeFunctionCast(nullptr)
                , m_LambdaExpression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_KeywordExpression;
                phantom::lang::ast::Expression* m_SizeofExpression;
                phantom::lang::ast::Expression* m_TypeofExpression;
                phantom::lang::ast::Expression* m_NullptrExpression;
                spell::any m_DOT;
                phantom::lang::ast::Name* m_Name;
                phantom::lang::ast::Expression* m_CStyleCastExpression;
                phantom::lang::ast::Expression* m_TypeTraitBinary;
                phantom::lang::ast::StringLiteralChain* m_StringLiteralChain;
                phantom::lang::ast::SpecialCastExpression* m_SpecialCastExpression;
                phantom::lang::ast::FunctionPtrExpression* m_FunctionPtrExpression;
                phantom::lang::ast::Expression* m_FundamentalTypeFunctionCast;
                phantom::lang::ast::LambdaExpression* m_LambdaExpression;
                spell::any m_INT_VALUE;
            };
            struct NotLastTemplateArgument : public TemplateArgument
            {
                NotLastTemplateArgument(ERuleType t = e_RuleType_NotLastTemplateArgument)
                : TemplateArgument(t)
                , m_NotLastTemplateArgumentAssign(nullptr)
                , m_NotLastTemplateArgumentNoAssign(nullptr)
                {
                }
                phantom::lang::ast::NotLastTemplateArgument* m_NotLastTemplateArgumentAssign;
                phantom::lang::ast::NotLastTemplateArgument* m_NotLastTemplateArgumentNoAssign;
            };
            struct UnsignedType : public Type
            {
                UnsignedType(ERuleType t = e_RuleType_UnsignedType)
                : Type(t)
                , m_UNSIGNED(nullptr)
                , m_Type(nullptr)
                {
                }
                char const* m_UNSIGNED;
                phantom::lang::ast::Type* m_Type;
            };
            struct SignedType : public Type
            {
                SignedType(ERuleType t = e_RuleType_SignedType)
                : Type(t)
                , m_SIGNED(nullptr)
                , m_Type(nullptr)
                {
                }
                char const* m_SIGNED;
                phantom::lang::ast::Type* m_Type;
            };
            struct LongType : public Type
            {
                LongType(ERuleType t = e_RuleType_LongType)
                : Type(t)
                , m_LONG(nullptr)
                , m_Type(nullptr)
                {
                }
                char const* m_LONG;
                phantom::lang::ast::Type* m_Type;
            };
            struct VoidType : public Type
            {
                VoidType(ERuleType t = e_RuleType_VoidType)
                : Type(t)
                , m_VOID(nullptr)
                {
                }
                char const* m_VOID;
            };
            struct FundamentalType : public Type
            {
                FundamentalType(ERuleType t = e_RuleType_FundamentalType)
                : Type(t)
                , m_UnsignedType(nullptr)
                , m_SignedType(nullptr)
                , m_LongType(nullptr)
                , m_VoidType(nullptr)
                , m_FundamentalTypeBase(nullptr)
                {
                }
                phantom::lang::ast::Type* m_UnsignedType;
                phantom::lang::ast::Type* m_SignedType;
                phantom::lang::ast::Type* m_LongType;
                phantom::lang::ast::Type* m_VoidType;
                phantom::lang::ast::FundamentalTypeBase* m_FundamentalTypeBase;
            };
            struct QualifiedType : public Type
            {
                QualifiedType(ERuleType t = e_RuleType_QualifiedType)
                : Type(t)
                , m_QualifiedName(nullptr)
                {
                }
                phantom::lang::ast::QualifiedName* m_QualifiedName;
            };
            struct ExplicitType : public Type
            {
                ExplicitType(ERuleType t = e_RuleType_ExplicitType)
                : Type(t)
                , m_CONSTs(nullptr)
                , m_BasicType(nullptr)
                , m_TypeExtents(nullptr)
                {
                }
                phantom::SmallVector<spell::any,4ull>* m_CONSTs;
                phantom::lang::ast::Type* m_BasicType;
                phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>* m_TypeExtents;
                spell::any m_ELLIPSE;
            };
            struct TypeNoFunctionType : public Type
            {
                TypeNoFunctionType(ERuleType t = e_RuleType_TypeNoFunctionType)
                : Type(t)
                , m_ExplicitTypeNoFunctionType(nullptr)
                , m_ExpressionType(nullptr)
                {
                }
                phantom::lang::ast::Type* m_ExplicitTypeNoFunctionType;
                phantom::lang::ast::Type* m_ExpressionType;
            };
            struct ExplicitTypeNoFunctionType : public Type
            {
                ExplicitTypeNoFunctionType(ERuleType t = e_RuleType_ExplicitTypeNoFunctionType)
                : Type(t)
                , m_CONSTs(nullptr)
                , m_BasicType(nullptr)
                , m_TypeExtentNoFunctions(nullptr)
                {
                }
                phantom::SmallVector<spell::any,4ull>* m_CONSTs;
                phantom::lang::ast::Type* m_BasicType;
                phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>* m_TypeExtentNoFunctions;
                spell::any m_ELLIPSE;
            };
            struct ExpressionType : public Type
            {
                ExpressionType(ERuleType t = e_RuleType_ExpressionType)
                : Type(t)
                , m_AssignExpression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_AssignExpression;
            };
            struct ExplicitTypeOrAuto : public Type
            {
                ExplicitTypeOrAuto(ERuleType t = e_RuleType_ExplicitTypeOrAuto)
                : Type(t)
                , m_CONSTs(nullptr)
                , m_BasicTypeOrAuto(nullptr)
                , m_TypeExtents(nullptr)
                {
                }
                phantom::SmallVector<spell::any,4ull>* m_CONSTs;
                phantom::lang::ast::Type* m_BasicTypeOrAuto;
                phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>* m_TypeExtents;
            };
            struct TypeOrAuto : public Type
            {
                TypeOrAuto(ERuleType t = e_RuleType_TypeOrAuto)
                : Type(t)
                , m_ExplicitTypeOrAuto(nullptr)
                , m_ExpressionType(nullptr)
                {
                }
                phantom::lang::ast::Type* m_ExplicitTypeOrAuto;
                phantom::lang::ast::Type* m_ExpressionType;
            };
            struct AutoType : public Type
            {
                AutoType(ERuleType t = e_RuleType_AutoType)
                : Type(t)
                , m_CONSTs(nullptr)
                , m_Auto(nullptr)
                , m_TypeExtents(nullptr)
                {
                }
                phantom::SmallVector<spell::any,4ull>* m_CONSTs;
                phantom::lang::ast::Type* m_Auto;
                phantom::SmallVector<phantom::lang::ast::TypeExtent*,4ull>* m_TypeExtents;
            };
            struct BasicType : public Type
            {
                BasicType(ERuleType t = e_RuleType_BasicType)
                : Type(t)
                , m_FundamentalType(nullptr)
                , m_DeclType(nullptr)
                , m_QualifiedType(nullptr)
                {
                }
                phantom::lang::ast::Type* m_FundamentalType;
                phantom::lang::ast::DeclType* m_DeclType;
                phantom::lang::ast::Type* m_QualifiedType;
            };
            struct Auto : public Type
            {
                Auto(ERuleType t = e_RuleType_Auto)
                : Type(t)
                , m_AUTO(nullptr)
                {
                }
                char const* m_AUTO;
            };
            struct BasicTypeOrAuto : public Type
            {
                BasicTypeOrAuto(ERuleType t = e_RuleType_BasicTypeOrAuto)
                : Type(t)
                , m_FundamentalType(nullptr)
                , m_Auto(nullptr)
                , m_DeclType(nullptr)
                , m_QualifiedType(nullptr)
                {
                }
                phantom::lang::ast::Type* m_FundamentalType;
                phantom::lang::ast::Type* m_Auto;
                phantom::lang::ast::DeclType* m_DeclType;
                phantom::lang::ast::Type* m_QualifiedType;
            };
            struct IntegralConstant : public Expression
            {
                IntegralConstant(ERuleType t = e_RuleType_IntegralConstant)
                : Expression(t)
                {
                }
                spell::any m_INT_VALUE;
            };
            struct ArrayExtent : public TypeExtent
            {
                ArrayExtent(ERuleType t = e_RuleType_ArrayExtent)
                : TypeExtent(t)
                , m_ArrayExtentExpression(nullptr)
                {
                }
                phantom::lang::ast::ArrayExtentExpression* m_ArrayExtentExpression;
            };
            struct FunctionTypeExtent : public TypeExtent
            {
                FunctionTypeExtent(ERuleType t = e_RuleType_FunctionTypeExtent)
                : TypeExtent(t)
                , m_PAREN_START(nullptr)
                , m_Types(nullptr)
                {
                }
                char const* m_PAREN_START;
                phantom::SmallVector<phantom::lang::ast::Type*,4ull>* m_Types;
            };
            struct DerivedExtent : public TypeExtent
            {
                DerivedExtent(ERuleType t = e_RuleType_DerivedExtent)
                : TypeExtent(t)
                , m_BIN_AND(nullptr)
                {
                }
                char const* m_BIN_AND;
            };
            struct TypeExtentNoFunction : public TypeExtent
            {
                TypeExtentNoFunction(ERuleType t = e_RuleType_TypeExtentNoFunction)
                : TypeExtent(t)
                , m_ArrayExtent(nullptr)
                , m_DerivedExtent(nullptr)
                {
                }
                phantom::lang::ast::TypeExtent* m_ArrayExtent;
                phantom::lang::ast::TypeExtent* m_DerivedExtent;
            };
            struct AssignExpression : public Expression
            {
                AssignExpression(ERuleType t = e_RuleType_AssignExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_RightShiftAssign(nullptr)
                , m_RightShiftAssign_RightExpression(nullptr)
                , m_RightShift(nullptr)
                , m_RightShift_RightExpression(nullptr)
                , m_Greater_RightExpression(nullptr)
                , m_RightExpression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::SmallVector<spell::any,4ull>* m_RightShiftAssign;
                phantom::lang::ast::Expression* m_RightShiftAssign_RightExpression;
                phantom::SmallVector<spell::any,4ull>* m_RightShift;
                phantom::lang::ast::Expression* m_RightShift_RightExpression;
                spell::any m_GT;
                phantom::lang::ast::Expression* m_Greater_RightExpression;
                spell::any m_Op;
                phantom::lang::ast::Expression* m_RightExpression;
            };
            struct ConditionalExpression : public Expression
            {
                ConditionalExpression(ERuleType t = e_RuleType_ConditionalExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_Then(nullptr)
                , m_Else(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::lang::ast::Expression* m_Then;
                phantom::lang::ast::Expression* m_Else;
            };
            struct Friend : public Member
            {
                Friend(ERuleType t = e_RuleType_Friend)
                : Member(t)
                , m_QualifiedName(nullptr)
                {
                }
                spell::any m_CLASS;
                phantom::lang::ast::QualifiedName* m_QualifiedName;
            };
            struct LogicalOrExpression : public Expression
            {
                LogicalOrExpression(ERuleType t = e_RuleType_LogicalOrExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_LogicalOrExpressionRights(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::SmallVector<phantom::lang::ast::LogicalOrExpressionRight*,4ull>* m_LogicalOrExpressionRights;
            };
            struct BasicMember : public Member
            {
                BasicMember(ERuleType t = e_RuleType_BasicMember)
                : Member(t)
                , m_AccessSpecifier(nullptr)
                , m_Destructor(nullptr)
                , m_Method(nullptr)
                , m_Method0(nullptr)
                , m_FunctionOrVariable(nullptr)
                , m_ConversionFunction(nullptr)
                , m_Constructor(nullptr)
                , m_Destructor0(nullptr)
                , m_Friend(nullptr)
                , m_BasicMemberWithMetaOrAnnotation(nullptr)
                , m_Declaration(nullptr)
                {
                }
                phantom::lang::ast::AccessSpecifier* m_AccessSpecifier;
                phantom::lang::ast::Destructor* m_Destructor;
                phantom::lang::ast::Method* m_Method;
                phantom::lang::ast::Method* m_Method0;
                phantom::lang::ast::FunctionOrVariable* m_FunctionOrVariable;
                phantom::lang::ast::ConversionFunction* m_ConversionFunction;
                phantom::lang::ast::Constructor* m_Constructor;
                phantom::lang::ast::Destructor* m_Destructor0;
                phantom::lang::ast::Member* m_Friend;
                phantom::lang::ast::Member* m_BasicMemberWithMetaOrAnnotation;
                phantom::lang::ast::Declaration* m_Declaration;
            };
            struct LogicalAndExpression : public Expression
            {
                LogicalAndExpression(ERuleType t = e_RuleType_LogicalAndExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_LogicalAndExpressionRights(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::SmallVector<phantom::lang::ast::LogicalAndExpressionRight*,4ull>* m_LogicalAndExpressionRights;
            };
            struct ForStatement : public Statement
            {
                ForStatement(ERuleType t = e_RuleType_ForStatement)
                : Statement(t)
                , m_PAREN_START(nullptr)
                , m_ForeachSignature(nullptr)
                , m_Init(nullptr)
                , m_Test(nullptr)
                , m_Update(nullptr)
                , m_Statement(nullptr)
                {
                }
                char const* m_PAREN_START;
                phantom::lang::ast::ForeachSignature* m_ForeachSignature;
                phantom::lang::ast::ForInit* m_Init;
                phantom::lang::ast::Expression* m_Test;
                phantom::lang::ast::Expression* m_Update;
                phantom::lang::ast::Statement* m_Statement;
            };
            struct BinOrExpression : public Expression
            {
                BinOrExpression(ERuleType t = e_RuleType_BinOrExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_BinOrExpressionRights(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::SmallVector<phantom::lang::ast::BinOrExpressionRight*,4ull>* m_BinOrExpressionRights;
            };
            struct WhileStatement : public Statement
            {
                WhileStatement(ERuleType t = e_RuleType_WhileStatement)
                : Statement(t)
                , m_PAREN_START(nullptr)
                , m_Condition(nullptr)
                , m_Statement(nullptr)
                {
                }
                char const* m_PAREN_START;
                phantom::lang::ast::IfOrWhileCondition* m_Condition;
                phantom::lang::ast::Statement* m_Statement;
            };
            struct XOrExpression : public Expression
            {
                XOrExpression(ERuleType t = e_RuleType_XOrExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_XOrExpressionRights(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::SmallVector<phantom::lang::ast::XOrExpressionRight*,4ull>* m_XOrExpressionRights;
            };
            struct BlockStatement : public Statement
            {
                BlockStatement(ERuleType t = e_RuleType_BlockStatement)
                : Statement(t)
                , m_Statements(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::Statement*,4ull>* m_Statements;
            };
            struct BinAndExpression : public Expression
            {
                BinAndExpression(ERuleType t = e_RuleType_BinAndExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_BinAndExpressionRights(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::SmallVector<phantom::lang::ast::BinAndExpressionRight*,4ull>* m_BinAndExpressionRights;
            };
            struct DoWhileStatement : public Statement
            {
                DoWhileStatement(ERuleType t = e_RuleType_DoWhileStatement)
                : Statement(t)
                , m_BlockStatement(nullptr)
                , m_PAREN_START(nullptr)
                , m_Expression(nullptr)
                {
                }
                phantom::lang::ast::Statement* m_BlockStatement;
                char const* m_PAREN_START;
                phantom::lang::ast::Expression* m_Expression;
            };
            struct EqualityExpression : public Expression
            {
                EqualityExpression(ERuleType t = e_RuleType_EqualityExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_EqualityExpressionRights(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::SmallVector<phantom::lang::ast::EqualityExpressionRight*,4ull>* m_EqualityExpressionRights;
            };
            struct StatementOrCase : public Statement
            {
                StatementOrCase(ERuleType t = e_RuleType_StatementOrCase)
                : Statement(t)
                , m_CaseStatement(nullptr)
                , m_DefaultStatement(nullptr)
                , m_Statement(nullptr)
                {
                }
                phantom::lang::ast::Statement* m_CaseStatement;
                phantom::lang::ast::Statement* m_DefaultStatement;
                phantom::lang::ast::Statement* m_Statement;
            };
            struct RelationalExpression : public Expression
            {
                RelationalExpression(ERuleType t = e_RuleType_RelationalExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_RelationalExpressionRights(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::SmallVector<phantom::lang::ast::RelationalExpressionRight*,4ull>* m_RelationalExpressionRights;
            };
            struct MemberWithMetaOrAnnotation : public Member
            {
                MemberWithMetaOrAnnotation(ERuleType t = e_RuleType_MemberWithMetaOrAnnotation)
                : Member(t)
                , m_MetaOrAnnotations(nullptr)
                , m_Member(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::MetaOrAnnotation*,4ull>* m_MetaOrAnnotations;
                phantom::lang::ast::Member* m_Member;
            };
            struct ShiftExpression : public Expression
            {
                ShiftExpression(ERuleType t = e_RuleType_ShiftExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_ShiftExpressionRights(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::SmallVector<phantom::lang::ast::ShiftExpressionRight*,4ull>* m_ShiftExpressionRights;
            };
            struct BasicMemberWithMetaOrAnnotation : public Member
            {
                BasicMemberWithMetaOrAnnotation(ERuleType t = e_RuleType_BasicMemberWithMetaOrAnnotation)
                : Member(t)
                , m_MetaOrAnnotations(nullptr)
                , m_BasicMember(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::MetaOrAnnotation*,4ull>* m_MetaOrAnnotations;
                phantom::lang::ast::Member* m_BasicMember;
            };
            struct AddExpression : public Expression
            {
                AddExpression(ERuleType t = e_RuleType_AddExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_AddExpressionRights(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::SmallVector<phantom::lang::ast::AddExpressionRight*,4ull>* m_AddExpressionRights;
            };
            struct IfStatement : public Statement
            {
                IfStatement(ERuleType t = e_RuleType_IfStatement)
                : Statement(t)
                , m_PAREN_START(nullptr)
                , m_Condition(nullptr)
                , m_Then(nullptr)
                , m_Else(nullptr)
                {
                }
                char const* m_PAREN_START;
                phantom::lang::ast::IfOrWhileCondition* m_Condition;
                phantom::lang::ast::Statement* m_Then;
                phantom::lang::ast::Statement* m_Else;
            };
            struct MulExpression : public Expression
            {
                MulExpression(ERuleType t = e_RuleType_MulExpression)
                : Expression(t)
                , m_LeftExpression(nullptr)
                , m_MulExpressionRights(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_LeftExpression;
                phantom::SmallVector<phantom::lang::ast::MulExpressionRight*,4ull>* m_MulExpressionRights;
            };
            struct CaseStatement : public Statement
            {
                CaseStatement(ERuleType t = e_RuleType_CaseStatement)
                : Statement(t)
                , m_Expression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_Expression;
            };
            struct ExplicitLocalVariableDecl : public LocalVariableDecl
            {
                ExplicitLocalVariableDecl(ERuleType t = e_RuleType_ExplicitLocalVariableDecl)
                : LocalVariableDecl(t)
                , m_CONSTs(nullptr)
                , m_BasicType(nullptr)
                , m_Declarators(nullptr)
                {
                }
                phantom::SmallVector<spell::any,4ull>* m_CONSTs;
                phantom::lang::ast::Type* m_BasicType;
                phantom::SmallVector<phantom::lang::ast::Declarator*,4ull>* m_Declarators;
            };
            struct UnaryExpression : public Expression
            {
                UnaryExpression(ERuleType t = e_RuleType_UnaryExpression)
                : Expression(t)
                , m_Expression(nullptr)
                , m_PostFixExpression(nullptr)
                {
                }
                spell::any m_Op;
                phantom::lang::ast::Expression* m_Expression;
                phantom::lang::ast::Expression* m_PostFixExpression;
            };
            struct AssignExpressionOrInitializerList : public Expression
            {
                AssignExpressionOrInitializerList(ERuleType t = e_RuleType_AssignExpressionOrInitializerList)
                : Expression(t)
                , m_InitializerList(nullptr)
                , m_AssignExpression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_InitializerList;
                phantom::lang::ast::Expression* m_AssignExpression;
            };
            struct ExpressionOrInitializerList : public Expression
            {
                ExpressionOrInitializerList(ERuleType t = e_RuleType_ExpressionOrInitializerList)
                : Expression(t)
                , m_InitializerList(nullptr)
                , m_Expression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_InitializerList;
                phantom::lang::ast::Expression* m_Expression;
            };
            struct InitializerList : public Expression
            {
                InitializerList(ERuleType t = e_RuleType_InitializerList)
                : Expression(t)
                , m_AssignExpressionOrInitializerLists(nullptr)
                {
                }
                phantom::SmallVector<phantom::lang::ast::Expression*,4ull>* m_AssignExpressionOrInitializerLists;
            };
            struct DefaultStatement : public Statement
            {
                DefaultStatement(ERuleType t = e_RuleType_DefaultStatement)
                : Statement(t)
                {
                }
            };
            struct ArrayAccess : public Expression
            {
                ArrayAccess(ERuleType t = e_RuleType_ArrayAccess)
                : Expression(t)
                , m_Expression(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_Expression;
            };
            struct KeywordExpression : public Expression
            {
                KeywordExpression(ERuleType t = e_RuleType_KeywordExpression)
                : Expression(t)
                , m_Keyword(nullptr)
                {
                }
                char const* m_Keyword;
            };
            struct NullptrExpression : public Expression
            {
                NullptrExpression(ERuleType t = e_RuleType_NullptrExpression)
                : Expression(t)
                {
                }
            };
            struct PostFixExpression : public Expression
            {
                PostFixExpression(ERuleType t = e_RuleType_PostFixExpression)
                : Expression(t)
                , m_Expression(nullptr)
                , m_PostFixEnds(nullptr)
                {
                }
                phantom::lang::ast::Expression* m_Expression;
                phantom::SmallVector<phantom::lang::ast::PostFixEnd*,4ull>* m_PostFixEnds;
            };
            struct AutoLocalVariableDecl : public LocalVariableDecl
            {
                AutoLocalVariableDecl(ERuleType t = e_RuleType_AutoLocalVariableDecl)
                : LocalVariableDecl(t)
                , m_CONSTs(nullptr)
                , m_Auto(nullptr)
                , m_AutoDeclarators(nullptr)
                {
                }
                phantom::SmallVector<spell::any,4ull>* m_CONSTs;
                phantom::lang::ast::Type* m_Auto;
                phantom::SmallVector<phantom::lang::ast::AutoDeclarator*,4ull>* m_AutoDeclarators;
            };
            struct CStyleCastExpression : public Expression
            {
                CStyleCastExpression(ERuleType t = e_RuleType_CStyleCastExpression)
                : Expression(t)
                , m_PAREN_START(nullptr)
                , m_Expression(nullptr)
                , m_Type(nullptr)
                , m_Casted(nullptr)
                {
                }
                char const* m_PAREN_START;
                phantom::lang::ast::Expression* m_Expression;
                phantom::lang::ast::Type* m_Type;
                phantom::lang::ast::Expression* m_Casted;
            };
            struct TypeTraitBinary : public Expression
            {
                TypeTraitBinary(ERuleType t = e_RuleType_TypeTraitBinary)
                : Expression(t)
                , m_IS_SAME(nullptr)
                , m_PAREN_START(nullptr)
                , m_Type(nullptr)
                , m_Type0(nullptr)
                {
                }
                char const* m_IS_SAME;
                char const* m_PAREN_START;
                phantom::lang::ast::Type* m_Type;
                phantom::lang::ast::Type* m_Type0;
            };
            struct LabelStatement : public Statement
            {
                LabelStatement(ERuleType t = e_RuleType_LabelStatement)
                : Statement(t)
                , m_IDENTIFIER(nullptr)
                {
                }
                char const* m_IDENTIFIER;
            };
            struct AssertStatement : public Statement
            {
                AssertStatement(ERuleType t = e_RuleType_AssertStatement)
                : Statement(t)
                , m_PAREN_START(nullptr)
                , m_AssignExpression(nullptr)
                , m_AssignExpressions(nullptr)
                {
                }
                char const* m_PAREN_START;
                phantom::lang::ast::Expression* m_AssignExpression;
                spell::any m_STRING_LIT;
                phantom::SmallVector<phantom::lang::ast::Expression*,4ull>* m_AssignExpressions;
            };
            struct BreakStatement : public Statement
            {
                BreakStatement(ERuleType t = e_RuleType_BreakStatement)
                : Statement(t)
                , m_BREAK(nullptr)
                {
                }
                char const* m_BREAK;
            };
            struct ContinueStatement : public Statement
            {
                ContinueStatement(ERuleType t = e_RuleType_ContinueStatement)
                : Statement(t)
                , m_CONTINUE(nullptr)
                {
                }
                char const* m_CONTINUE;
            };
            struct SizeofExpression : public Expression
            {
                SizeofExpression(ERuleType t = e_RuleType_SizeofExpression)
                : Expression(t)
                , m_SIZEOF(nullptr)
                , m_PAREN_START(nullptr)
                , m_Type(nullptr)
                {
                }
                char const* m_SIZEOF;
                char const* m_PAREN_START;
                phantom::lang::ast::Type* m_Type;
            };
            struct TypeofExpression : public Expression
            {
                TypeofExpression(ERuleType t = e_RuleType_TypeofExpression)
                : Expression(t)
                , m_TYPEOF(nullptr)
                , m_PAREN_START(nullptr)
                , m_Type(nullptr)
                {
                }
                char const* m_TYPEOF;
                char const* m_PAREN_START;
                phantom::lang::ast::Type* m_Type;
            };
            struct LocalVariableStatement : public Statement
            {
                LocalVariableStatement(ERuleType t = e_RuleType_LocalVariableStatement)
                : Statement(t)
                , m_LocalVariableDecl(nullptr)
                {
                }
                phantom::lang::ast::LocalVariableDecl* m_LocalVariableDecl;
            };
            struct LastTemplateArgumentAssign : public LastTemplateArgument
            {
                LastTemplateArgumentAssign(ERuleType t = e_RuleType_LastTemplateArgumentAssign)
                : LastTemplateArgument(t)
                , m_TemplateArgumentAssign(nullptr)
                {
                }
                phantom::lang::ast::TemplateArgumentAssign* m_TemplateArgumentAssign;
            };
            struct LastTemplateArgumentNoAssign : public LastTemplateArgument
            {
                LastTemplateArgumentNoAssign(ERuleType t = e_RuleType_LastTemplateArgumentNoAssign)
                : LastTemplateArgument(t)
                , m_TemplateArgumentNoAssign(nullptr)
                {
                }
                phantom::lang::ast::TemplateArgumentNoAssign* m_TemplateArgumentNoAssign;
            };
            struct NotLastTemplateArgumentAssign : public NotLastTemplateArgument
            {
                NotLastTemplateArgumentAssign(ERuleType t = e_RuleType_NotLastTemplateArgumentAssign)
                : NotLastTemplateArgument(t)
                , m_TemplateArgumentAssign(nullptr)
                {
                }
                phantom::lang::ast::TemplateArgumentAssign* m_TemplateArgumentAssign;
            };
            struct NotLastTemplateArgumentNoAssign : public NotLastTemplateArgument
            {
                NotLastTemplateArgumentNoAssign(ERuleType t = e_RuleType_NotLastTemplateArgumentNoAssign)
                : NotLastTemplateArgument(t)
                , m_TemplateArgumentNoAssign(nullptr)
                {
                }
                phantom::lang::ast::TemplateArgumentNoAssign* m_TemplateArgumentNoAssign;
            };
            struct CppLiteGrammarPrivate;
            namespace visitor
            {
                template<class T>
                struct Recursive
                {
                    inline bool visit(Sources* input) { return static_cast<T*>(this)->visitSources(input); }
                    inline bool end(Sources* input) { return static_cast<T*>(this)->endSources(input); }
                    inline bool traverse(Sources* input) { return static_cast<T*>(this)->traverseSources(input); }
                    inline bool visitSources(Sources* input) { return true; }
                    inline bool endSources(Sources* input) { return true; }
                    inline bool visit(Source* input) { return static_cast<T*>(this)->visitSource(input); }
                    inline bool end(Source* input) { return static_cast<T*>(this)->endSource(input); }
                    inline bool traverse(Source* input) { return static_cast<T*>(this)->traverseSource(input); }
                    inline bool visitSource(Source* input) { return true; }
                    inline bool endSource(Source* input) { return true; }
                    bool traverseSources(Sources* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseSource(Source* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_ModuleDeclarations) if(!traverse(input->m_ModuleDeclarations)) return false;
                        if (input->m_ImportDeclarations) if(!traverse(input->m_ImportDeclarations)) return false;
                        if (input->m_SourceDeclarations) if(!traverse(input->m_SourceDeclarations)) return false;
                        return end(input);
                    }
                    inline bool visit(ModuleDeclarations* input) { return static_cast<T*>(this)->visitModuleDeclarations(input); }
                    inline bool end(ModuleDeclarations* input) { return static_cast<T*>(this)->endModuleDeclarations(input); }
                    inline bool traverse(ModuleDeclarations* input) { return static_cast<T*>(this)->traverseModuleDeclarations(input); }
                    inline bool visitModuleDeclarations(ModuleDeclarations* input) { return true; }
                    inline bool endModuleDeclarations(ModuleDeclarations* input) { return true; }
                    inline bool visit(ModuleDeclaration* input) { return static_cast<T*>(this)->visitModuleDeclaration(input); }
                    inline bool end(ModuleDeclaration* input) { return static_cast<T*>(this)->endModuleDeclaration(input); }
                    inline bool traverse(ModuleDeclaration* input) { return static_cast<T*>(this)->traverseModuleDeclaration(input); }
                    inline bool visitModuleDeclaration(ModuleDeclaration* input) { return true; }
                    inline bool endModuleDeclaration(ModuleDeclaration* input) { return true; }
                    bool traverseModuleDeclarations(ModuleDeclarations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseModuleDeclaration(ModuleDeclaration* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(ImportDeclarations* input) { return static_cast<T*>(this)->visitImportDeclarations(input); }
                    inline bool end(ImportDeclarations* input) { return static_cast<T*>(this)->endImportDeclarations(input); }
                    inline bool traverse(ImportDeclarations* input) { return static_cast<T*>(this)->traverseImportDeclarations(input); }
                    inline bool visitImportDeclarations(ImportDeclarations* input) { return true; }
                    inline bool endImportDeclarations(ImportDeclarations* input) { return true; }
                    inline bool visit(ImportDeclaration* input) { return static_cast<T*>(this)->visitImportDeclaration(input); }
                    inline bool end(ImportDeclaration* input) { return static_cast<T*>(this)->endImportDeclaration(input); }
                    inline bool traverse(ImportDeclaration* input) { return static_cast<T*>(this)->traverseImportDeclaration(input); }
                    inline bool visitImportDeclaration(ImportDeclaration* input) { return true; }
                    inline bool endImportDeclaration(ImportDeclaration* input) { return true; }
                    bool traverseImportDeclarations(ImportDeclarations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseImportDeclaration(ImportDeclaration* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Imports) if(!traverse(input->m_Imports)) return false;
                        return end(input);
                    }
                    inline bool visit(Imports* input) { return static_cast<T*>(this)->visitImports(input); }
                    inline bool end(Imports* input) { return static_cast<T*>(this)->endImports(input); }
                    inline bool traverse(Imports* input) { return static_cast<T*>(this)->traverseImports(input); }
                    inline bool visitImports(Imports* input) { return true; }
                    inline bool endImports(Imports* input) { return true; }
                    inline bool visit(Import* input) { return static_cast<T*>(this)->visitImport(input); }
                    inline bool end(Import* input) { return static_cast<T*>(this)->endImport(input); }
                    inline bool traverse(Import* input) { return static_cast<T*>(this)->traverseImport(input); }
                    inline bool visitImport(Import* input) { return true; }
                    inline bool endImport(Import* input) { return true; }
                    bool traverseImports(Imports* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseImport(Import* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_ImportBase) if(!traverse(input->m_ImportBase)) return false;
                        if (input->m_ImportBindings) if(!traverse(input->m_ImportBindings)) return false;
                        return end(input);
                    }
                    inline bool visit(ImportBindings* input) { return static_cast<T*>(this)->visitImportBindings(input); }
                    inline bool end(ImportBindings* input) { return static_cast<T*>(this)->endImportBindings(input); }
                    inline bool traverse(ImportBindings* input) { return static_cast<T*>(this)->traverseImportBindings(input); }
                    inline bool visitImportBindings(ImportBindings* input) { return true; }
                    inline bool endImportBindings(ImportBindings* input) { return true; }
                    inline bool visit(ImportBinding* input) { return static_cast<T*>(this)->visitImportBinding(input); }
                    inline bool end(ImportBinding* input) { return static_cast<T*>(this)->endImportBinding(input); }
                    inline bool traverse(ImportBinding* input) { return static_cast<T*>(this)->traverseImportBinding(input); }
                    inline bool visitImportBinding(ImportBinding* input) { return true; }
                    inline bool endImportBinding(ImportBinding* input) { return true; }
                    bool traverseImportBindings(ImportBindings* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseImportBinding(ImportBinding* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(ImportBases* input) { return static_cast<T*>(this)->visitImportBases(input); }
                    inline bool end(ImportBases* input) { return static_cast<T*>(this)->endImportBases(input); }
                    inline bool traverse(ImportBases* input) { return static_cast<T*>(this)->traverseImportBases(input); }
                    inline bool visitImportBases(ImportBases* input) { return true; }
                    inline bool endImportBases(ImportBases* input) { return true; }
                    inline bool visit(ImportBase* input) { return static_cast<T*>(this)->visitImportBase(input); }
                    inline bool end(ImportBase* input) { return static_cast<T*>(this)->endImportBase(input); }
                    inline bool traverse(ImportBase* input) { return static_cast<T*>(this)->traverseImportBase(input); }
                    inline bool visitImportBase(ImportBase* input) { return true; }
                    inline bool endImportBase(ImportBase* input) { return true; }
                    bool traverseImportBases(ImportBases* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseImportBase(ImportBase* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(Metas* input) { return static_cast<T*>(this)->visitMetas(input); }
                    inline bool end(Metas* input) { return static_cast<T*>(this)->endMetas(input); }
                    inline bool traverse(Metas* input) { return static_cast<T*>(this)->traverseMetas(input); }
                    inline bool visitMetas(Metas* input) { return true; }
                    inline bool endMetas(Metas* input) { return true; }
                    inline bool visit(Meta* input) { return static_cast<T*>(this)->visitMeta(input); }
                    inline bool end(Meta* input) { return static_cast<T*>(this)->endMeta(input); }
                    inline bool traverse(Meta* input) { return static_cast<T*>(this)->traverseMeta(input); }
                    inline bool visitMeta(Meta* input) { return true; }
                    inline bool endMeta(Meta* input) { return true; }
                    bool traverseMetas(Metas* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseMeta(Meta* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_MetaDecls) if(!traverse(input->m_MetaDecls)) return false;
                        return end(input);
                    }
                    inline bool visit(MetaDecls* input) { return static_cast<T*>(this)->visitMetaDecls(input); }
                    inline bool end(MetaDecls* input) { return static_cast<T*>(this)->endMetaDecls(input); }
                    inline bool traverse(MetaDecls* input) { return static_cast<T*>(this)->traverseMetaDecls(input); }
                    inline bool visitMetaDecls(MetaDecls* input) { return true; }
                    inline bool endMetaDecls(MetaDecls* input) { return true; }
                    inline bool visit(MetaDecl* input) { return static_cast<T*>(this)->visitMetaDecl(input); }
                    inline bool end(MetaDecl* input) { return static_cast<T*>(this)->endMetaDecl(input); }
                    inline bool traverse(MetaDecl* input) { return static_cast<T*>(this)->traverseMetaDecl(input); }
                    inline bool visitMetaDecl(MetaDecl* input) { return true; }
                    inline bool endMetaDecl(MetaDecl* input) { return true; }
                    bool traverseMetaDecls(MetaDecls* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseMetaDecl(MetaDecl* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Value) if(!traverse(input->m_Value)) return false;
                        return end(input);
                    }
                    inline bool visit(Annotations* input) { return static_cast<T*>(this)->visitAnnotations(input); }
                    inline bool end(Annotations* input) { return static_cast<T*>(this)->endAnnotations(input); }
                    inline bool traverse(Annotations* input) { return static_cast<T*>(this)->traverseAnnotations(input); }
                    inline bool visitAnnotations(Annotations* input) { return true; }
                    inline bool endAnnotations(Annotations* input) { return true; }
                    inline bool visit(Annotation* input) { return static_cast<T*>(this)->visitAnnotation(input); }
                    inline bool end(Annotation* input) { return static_cast<T*>(this)->endAnnotation(input); }
                    inline bool traverse(Annotation* input) { return static_cast<T*>(this)->traverseAnnotation(input); }
                    inline bool visitAnnotation(Annotation* input) { return true; }
                    inline bool endAnnotation(Annotation* input) { return true; }
                    bool traverseAnnotations(Annotations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAnnotation(Annotation* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(MetaOrAnnotations* input) { return static_cast<T*>(this)->visitMetaOrAnnotations(input); }
                    inline bool end(MetaOrAnnotations* input) { return static_cast<T*>(this)->endMetaOrAnnotations(input); }
                    inline bool traverse(MetaOrAnnotations* input) { return static_cast<T*>(this)->traverseMetaOrAnnotations(input); }
                    inline bool visitMetaOrAnnotations(MetaOrAnnotations* input) { return true; }
                    inline bool endMetaOrAnnotations(MetaOrAnnotations* input) { return true; }
                    inline bool visit(MetaOrAnnotation* input) { return static_cast<T*>(this)->visitMetaOrAnnotation(input); }
                    inline bool end(MetaOrAnnotation* input) { return static_cast<T*>(this)->endMetaOrAnnotation(input); }
                    inline bool traverse(MetaOrAnnotation* input) { return static_cast<T*>(this)->traverseMetaOrAnnotation(input); }
                    inline bool visitMetaOrAnnotation(MetaOrAnnotation* input) { return true; }
                    inline bool endMetaOrAnnotation(MetaOrAnnotation* input) { return true; }
                    bool traverseMetaOrAnnotations(MetaOrAnnotations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseMetaOrAnnotation(MetaOrAnnotation* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Meta) if(!traverse(input->m_Meta)) return false;
                        if (input->m_Annotation) if(!traverse(input->m_Annotation)) return false;
                        return end(input);
                    }
                    inline bool visit(SourceDeclarations* input) { return static_cast<T*>(this)->visitSourceDeclarations(input); }
                    inline bool end(SourceDeclarations* input) { return static_cast<T*>(this)->endSourceDeclarations(input); }
                    inline bool traverse(SourceDeclarations* input) { return static_cast<T*>(this)->traverseSourceDeclarations(input); }
                    inline bool visitSourceDeclarations(SourceDeclarations* input) { return true; }
                    inline bool endSourceDeclarations(SourceDeclarations* input) { return true; }
                    inline bool visit(SourceDeclaration* input) { return static_cast<T*>(this)->visitSourceDeclaration(input); }
                    inline bool end(SourceDeclaration* input) { return static_cast<T*>(this)->endSourceDeclaration(input); }
                    inline bool traverse(SourceDeclaration* input) { return static_cast<T*>(this)->traverseSourceDeclaration(input); }
                    inline bool visitSourceDeclaration(SourceDeclaration* input) { return true; }
                    inline bool endSourceDeclaration(SourceDeclaration* input) { return true; }
                    bool traverseSourceDeclarations(SourceDeclarations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseSourceDeclaration(SourceDeclaration* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_MetaOrAnnotations) if(!traverse(input->m_MetaOrAnnotations)) return false;
                        if (input->m_Declaration) if(!traverse(input->m_Declaration)) return false;
                        return end(input);
                    }
                    inline bool visit(Declarations* input) { return static_cast<T*>(this)->visitDeclarations(input); }
                    inline bool end(Declarations* input) { return static_cast<T*>(this)->endDeclarations(input); }
                    inline bool traverse(Declarations* input) { return static_cast<T*>(this)->traverseDeclarations(input); }
                    inline bool visitDeclarations(Declarations* input) { return true; }
                    inline bool endDeclarations(Declarations* input) { return true; }
                    inline bool visit(Declaration* input) { return static_cast<T*>(this)->visitDeclaration(input); }
                    inline bool end(Declaration* input) { return static_cast<T*>(this)->endDeclaration(input); }
                    inline bool traverse(Declaration* input) { return static_cast<T*>(this)->traverseDeclaration(input); }
                    inline bool visitDeclaration(Declaration* input) { return true; }
                    inline bool endDeclaration(Declaration* input) { return true; }
                    bool traverseDeclarations(Declarations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseDeclaration(Declaration* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Class) if(!traverse(input->m_Class)) return false;
                        if (input->m_Enum) if(!traverse(input->m_Enum)) return false;
                        if (input->m_Alias) if(!traverse(input->m_Alias)) return false;
                        if (input->m_Using) if(!traverse(input->m_Using)) return false;
                        if (input->m_Typedef) if(!traverse(input->m_Typedef)) return false;
                        if (input->m_Template) if(!traverse(input->m_Template)) return false;
                        if (input->m_MixinStatementDefinition) if(!traverse(input->m_MixinStatementDefinition)) return false;
                        if (input->m_StaticAssert) if(!traverse(input->m_StaticAssert)) return false;
                        if (input->m_FunctionOrVariable) if(!traverse(input->m_FunctionOrVariable)) return false;
                        return end(input);
                    }
                    inline bool visit(MixinStatementDefinitions* input) { return static_cast<T*>(this)->visitMixinStatementDefinitions(input); }
                    inline bool end(MixinStatementDefinitions* input) { return static_cast<T*>(this)->endMixinStatementDefinitions(input); }
                    inline bool traverse(MixinStatementDefinitions* input) { return static_cast<T*>(this)->traverseMixinStatementDefinitions(input); }
                    inline bool visitMixinStatementDefinitions(MixinStatementDefinitions* input) { return true; }
                    inline bool endMixinStatementDefinitions(MixinStatementDefinitions* input) { return true; }
                    inline bool visit(MixinStatementDefinition* input) { return static_cast<T*>(this)->visitMixinStatementDefinition(input); }
                    inline bool end(MixinStatementDefinition* input) { return static_cast<T*>(this)->endMixinStatementDefinition(input); }
                    inline bool traverse(MixinStatementDefinition* input) { return static_cast<T*>(this)->traverseMixinStatementDefinition(input); }
                    inline bool visitMixinStatementDefinition(MixinStatementDefinition* input) { return true; }
                    inline bool endMixinStatementDefinition(MixinStatementDefinition* input) { return true; }
                    bool traverseMixinStatementDefinitions(MixinStatementDefinitions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseMixinStatementDefinition(MixinStatementDefinition* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Statements) if(!traverse(input->m_Statements)) return false;
                        return end(input);
                    }
                    inline bool visit(ForeachSignatures* input) { return static_cast<T*>(this)->visitForeachSignatures(input); }
                    inline bool end(ForeachSignatures* input) { return static_cast<T*>(this)->endForeachSignatures(input); }
                    inline bool traverse(ForeachSignatures* input) { return static_cast<T*>(this)->traverseForeachSignatures(input); }
                    inline bool visitForeachSignatures(ForeachSignatures* input) { return true; }
                    inline bool endForeachSignatures(ForeachSignatures* input) { return true; }
                    inline bool visit(ForeachSignature* input) { return static_cast<T*>(this)->visitForeachSignature(input); }
                    inline bool end(ForeachSignature* input) { return static_cast<T*>(this)->endForeachSignature(input); }
                    inline bool traverse(ForeachSignature* input) { return static_cast<T*>(this)->traverseForeachSignature(input); }
                    inline bool visitForeachSignature(ForeachSignature* input) { return true; }
                    inline bool endForeachSignature(ForeachSignature* input) { return true; }
                    bool traverseForeachSignatures(ForeachSignatures* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseForeachSignature(ForeachSignature* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TypeOrAuto) if(!traverse(input->m_TypeOrAuto)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(ConversionFunctions* input) { return static_cast<T*>(this)->visitConversionFunctions(input); }
                    inline bool end(ConversionFunctions* input) { return static_cast<T*>(this)->endConversionFunctions(input); }
                    inline bool traverse(ConversionFunctions* input) { return static_cast<T*>(this)->traverseConversionFunctions(input); }
                    inline bool visitConversionFunctions(ConversionFunctions* input) { return true; }
                    inline bool endConversionFunctions(ConversionFunctions* input) { return true; }
                    inline bool visit(ConversionFunction* input) { return static_cast<T*>(this)->visitConversionFunction(input); }
                    inline bool end(ConversionFunction* input) { return static_cast<T*>(this)->endConversionFunction(input); }
                    inline bool traverse(ConversionFunction* input) { return static_cast<T*>(this)->traverseConversionFunction(input); }
                    inline bool visitConversionFunction(ConversionFunction* input) { return true; }
                    inline bool endConversionFunction(ConversionFunction* input) { return true; }
                    bool traverseConversionFunctions(ConversionFunctions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseConversionFunction(ConversionFunction* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TypeNoFunctionType) if(!traverse(input->m_TypeNoFunctionType)) return false;
                        if (input->m_FunctionBlock) if(!traverse(input->m_FunctionBlock)) return false;
                        return end(input);
                    }
                    inline bool visit(Constructors* input) { return static_cast<T*>(this)->visitConstructors(input); }
                    inline bool end(Constructors* input) { return static_cast<T*>(this)->endConstructors(input); }
                    inline bool traverse(Constructors* input) { return static_cast<T*>(this)->traverseConstructors(input); }
                    inline bool visitConstructors(Constructors* input) { return true; }
                    inline bool endConstructors(Constructors* input) { return true; }
                    inline bool visit(Constructor* input) { return static_cast<T*>(this)->visitConstructor(input); }
                    inline bool end(Constructor* input) { return static_cast<T*>(this)->endConstructor(input); }
                    inline bool traverse(Constructor* input) { return static_cast<T*>(this)->traverseConstructor(input); }
                    inline bool visitConstructor(Constructor* input) { return true; }
                    inline bool endConstructor(Constructor* input) { return true; }
                    bool traverseConstructors(Constructors* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseConstructor(Constructor* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TemplateArgumentList) if(!traverse(input->m_TemplateArgumentList)) return false;
                        if (input->m_Parameters) if(!traverse(input->m_Parameters)) return false;
                        if (input->m_ConstructorInitializers) if(!traverse(input->m_ConstructorInitializers)) return false;
                        if (input->m_FunctionBlock) if(!traverse(input->m_FunctionBlock)) return false;
                        return end(input);
                    }
                    inline bool visit(Destructors* input) { return static_cast<T*>(this)->visitDestructors(input); }
                    inline bool end(Destructors* input) { return static_cast<T*>(this)->endDestructors(input); }
                    inline bool traverse(Destructors* input) { return static_cast<T*>(this)->traverseDestructors(input); }
                    inline bool visitDestructors(Destructors* input) { return true; }
                    inline bool endDestructors(Destructors* input) { return true; }
                    inline bool visit(Destructor* input) { return static_cast<T*>(this)->visitDestructor(input); }
                    inline bool end(Destructor* input) { return static_cast<T*>(this)->endDestructor(input); }
                    inline bool traverse(Destructor* input) { return static_cast<T*>(this)->traverseDestructor(input); }
                    inline bool visitDestructor(Destructor* input) { return true; }
                    inline bool endDestructor(Destructor* input) { return true; }
                    bool traverseDestructors(Destructors* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseDestructor(Destructor* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_FunctionBlock) if(!traverse(input->m_FunctionBlock)) return false;
                        return end(input);
                    }
                    inline bool visit(ConstructorInitializers* input) { return static_cast<T*>(this)->visitConstructorInitializers(input); }
                    inline bool end(ConstructorInitializers* input) { return static_cast<T*>(this)->endConstructorInitializers(input); }
                    inline bool traverse(ConstructorInitializers* input) { return static_cast<T*>(this)->traverseConstructorInitializers(input); }
                    inline bool visitConstructorInitializers(ConstructorInitializers* input) { return true; }
                    inline bool endConstructorInitializers(ConstructorInitializers* input) { return true; }
                    inline bool visit(ConstructorInitializer* input) { return static_cast<T*>(this)->visitConstructorInitializer(input); }
                    inline bool end(ConstructorInitializer* input) { return static_cast<T*>(this)->endConstructorInitializer(input); }
                    inline bool traverse(ConstructorInitializer* input) { return static_cast<T*>(this)->traverseConstructorInitializer(input); }
                    inline bool visitConstructorInitializer(ConstructorInitializer* input) { return true; }
                    inline bool endConstructorInitializer(ConstructorInitializer* input) { return true; }
                    bool traverseConstructorInitializers(ConstructorInitializers* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseConstructorInitializer(ConstructorInitializer* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_BasicType) if(!traverse(input->m_BasicType)) return false;
                        if (input->m_CallList) if(!traverse(input->m_CallList)) return false;
                        if (input->m_InitializerList) if(!traverse(input->m_InitializerList)) return false;
                        return end(input);
                    }
                    inline bool visit(ArrowReturns* input) { return static_cast<T*>(this)->visitArrowReturns(input); }
                    inline bool end(ArrowReturns* input) { return static_cast<T*>(this)->endArrowReturns(input); }
                    inline bool traverse(ArrowReturns* input) { return static_cast<T*>(this)->traverseArrowReturns(input); }
                    inline bool visitArrowReturns(ArrowReturns* input) { return true; }
                    inline bool endArrowReturns(ArrowReturns* input) { return true; }
                    inline bool visit(ArrowReturn* input) { return static_cast<T*>(this)->visitArrowReturn(input); }
                    inline bool end(ArrowReturn* input) { return static_cast<T*>(this)->endArrowReturn(input); }
                    inline bool traverse(ArrowReturn* input) { return static_cast<T*>(this)->traverseArrowReturn(input); }
                    inline bool visitArrowReturn(ArrowReturn* input) { return true; }
                    inline bool endArrowReturn(ArrowReturn* input) { return true; }
                    bool traverseArrowReturns(ArrowReturns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseArrowReturn(ArrowReturn* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        return end(input);
                    }
                    inline bool visit(FunctionEnds* input) { return static_cast<T*>(this)->visitFunctionEnds(input); }
                    inline bool end(FunctionEnds* input) { return static_cast<T*>(this)->endFunctionEnds(input); }
                    inline bool traverse(FunctionEnds* input) { return static_cast<T*>(this)->traverseFunctionEnds(input); }
                    inline bool visitFunctionEnds(FunctionEnds* input) { return true; }
                    inline bool endFunctionEnds(FunctionEnds* input) { return true; }
                    inline bool visit(FunctionEnd* input) { return static_cast<T*>(this)->visitFunctionEnd(input); }
                    inline bool end(FunctionEnd* input) { return static_cast<T*>(this)->endFunctionEnd(input); }
                    inline bool traverse(FunctionEnd* input) { return static_cast<T*>(this)->traverseFunctionEnd(input); }
                    inline bool visitFunctionEnd(FunctionEnd* input) { return true; }
                    inline bool endFunctionEnd(FunctionEnd* input) { return true; }
                    bool traverseFunctionEnds(FunctionEnds* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseFunctionEnd(FunctionEnd* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Parameters) if(!traverse(input->m_Parameters)) return false;
                        if (input->m_ArrowReturn) if(!traverse(input->m_ArrowReturn)) return false;
                        if (input->m_FunctionBlock) if(!traverse(input->m_FunctionBlock)) return false;
                        return end(input);
                    }
                    inline bool visit(Operators* input) { return static_cast<T*>(this)->visitOperators(input); }
                    inline bool end(Operators* input) { return static_cast<T*>(this)->endOperators(input); }
                    inline bool traverse(Operators* input) { return static_cast<T*>(this)->traverseOperators(input); }
                    inline bool visitOperators(Operators* input) { return true; }
                    inline bool endOperators(Operators* input) { return true; }
                    inline bool visit(Operator* input) { return static_cast<T*>(this)->visitOperator(input); }
                    inline bool end(Operator* input) { return static_cast<T*>(this)->endOperator(input); }
                    inline bool traverse(Operator* input) { return static_cast<T*>(this)->traverseOperator(input); }
                    inline bool visitOperator(Operator* input) { return true; }
                    inline bool endOperator(Operator* input) { return true; }
                    bool traverseOperators(Operators* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseOperator(Operator* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(OperatorEnds* input) { return static_cast<T*>(this)->visitOperatorEnds(input); }
                    inline bool end(OperatorEnds* input) { return static_cast<T*>(this)->endOperatorEnds(input); }
                    inline bool traverse(OperatorEnds* input) { return static_cast<T*>(this)->traverseOperatorEnds(input); }
                    inline bool visitOperatorEnds(OperatorEnds* input) { return true; }
                    inline bool endOperatorEnds(OperatorEnds* input) { return true; }
                    inline bool visit(OperatorEnd* input) { return static_cast<T*>(this)->visitOperatorEnd(input); }
                    inline bool end(OperatorEnd* input) { return static_cast<T*>(this)->endOperatorEnd(input); }
                    inline bool traverse(OperatorEnd* input) { return static_cast<T*>(this)->traverseOperatorEnd(input); }
                    inline bool visitOperatorEnd(OperatorEnd* input) { return true; }
                    inline bool endOperatorEnd(OperatorEnd* input) { return true; }
                    bool traverseOperatorEnds(OperatorEnds* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseOperatorEnd(OperatorEnd* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Operator) if(!traverse(input->m_Operator)) return false;
                        if (input->m_FunctionEnd) if(!traverse(input->m_FunctionEnd)) return false;
                        return end(input);
                    }
                    inline bool visit(FunctionOrVariableEnds* input) { return static_cast<T*>(this)->visitFunctionOrVariableEnds(input); }
                    inline bool end(FunctionOrVariableEnds* input) { return static_cast<T*>(this)->endFunctionOrVariableEnds(input); }
                    inline bool traverse(FunctionOrVariableEnds* input) { return static_cast<T*>(this)->traverseFunctionOrVariableEnds(input); }
                    inline bool visitFunctionOrVariableEnds(FunctionOrVariableEnds* input) { return true; }
                    inline bool endFunctionOrVariableEnds(FunctionOrVariableEnds* input) { return true; }
                    inline bool visit(FunctionOrVariableEnd* input) { return static_cast<T*>(this)->visitFunctionOrVariableEnd(input); }
                    inline bool end(FunctionOrVariableEnd* input) { return static_cast<T*>(this)->endFunctionOrVariableEnd(input); }
                    inline bool traverse(FunctionOrVariableEnd* input) { return static_cast<T*>(this)->traverseFunctionOrVariableEnd(input); }
                    inline bool visitFunctionOrVariableEnd(FunctionOrVariableEnd* input) { return true; }
                    inline bool endFunctionOrVariableEnd(FunctionOrVariableEnd* input) { return true; }
                    bool traverseFunctionOrVariableEnds(FunctionOrVariableEnds* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseFunctionOrVariableEnd(FunctionOrVariableEnd* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_InitializerList) if(!traverse(input->m_InitializerList)) return false;
                        if (input->m_ArrayExtents) if(!traverse(input->m_ArrayExtents)) return false;
                        if (input->m_AssignExpressionOrInitializerList) if(!traverse(input->m_AssignExpressionOrInitializerList)) return false;
                        if (input->m_Declarators) if(!traverse(input->m_Declarators)) return false;
                        if (input->m_TemplateArgumentList) if(!traverse(input->m_TemplateArgumentList)) return false;
                        if (input->m_FunctionEnd) if(!traverse(input->m_FunctionEnd)) return false;
                        if (input->m_CallList) if(!traverse(input->m_CallList)) return false;
                        return end(input);
                    }
                    inline bool visit(MethodSpecifiers* input) { return static_cast<T*>(this)->visitMethodSpecifiers(input); }
                    inline bool end(MethodSpecifiers* input) { return static_cast<T*>(this)->endMethodSpecifiers(input); }
                    inline bool traverse(MethodSpecifiers* input) { return static_cast<T*>(this)->traverseMethodSpecifiers(input); }
                    inline bool visitMethodSpecifiers(MethodSpecifiers* input) { return true; }
                    inline bool endMethodSpecifiers(MethodSpecifiers* input) { return true; }
                    inline bool visit(MethodSpecifier* input) { return static_cast<T*>(this)->visitMethodSpecifier(input); }
                    inline bool end(MethodSpecifier* input) { return static_cast<T*>(this)->endMethodSpecifier(input); }
                    inline bool traverse(MethodSpecifier* input) { return static_cast<T*>(this)->traverseMethodSpecifier(input); }
                    inline bool visitMethodSpecifier(MethodSpecifier* input) { return true; }
                    inline bool endMethodSpecifier(MethodSpecifier* input) { return true; }
                    bool traverseMethodSpecifiers(MethodSpecifiers* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseMethodSpecifier(MethodSpecifier* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(Specifiers* input) { return static_cast<T*>(this)->visitSpecifiers(input); }
                    inline bool end(Specifiers* input) { return static_cast<T*>(this)->endSpecifiers(input); }
                    inline bool traverse(Specifiers* input) { return static_cast<T*>(this)->traverseSpecifiers(input); }
                    inline bool visitSpecifiers(Specifiers* input) { return true; }
                    inline bool endSpecifiers(Specifiers* input) { return true; }
                    inline bool visit(Specifier* input) { return static_cast<T*>(this)->visitSpecifier(input); }
                    inline bool end(Specifier* input) { return static_cast<T*>(this)->endSpecifier(input); }
                    inline bool traverse(Specifier* input) { return static_cast<T*>(this)->traverseSpecifier(input); }
                    inline bool visitSpecifier(Specifier* input) { return true; }
                    inline bool endSpecifier(Specifier* input) { return true; }
                    bool traverseSpecifiers(Specifiers* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseSpecifier(Specifier* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_MethodSpecifier) if(!traverse(input->m_MethodSpecifier)) return false;
                        return end(input);
                    }
                    inline bool visit(FunctionOrVariables* input) { return static_cast<T*>(this)->visitFunctionOrVariables(input); }
                    inline bool end(FunctionOrVariables* input) { return static_cast<T*>(this)->endFunctionOrVariables(input); }
                    inline bool traverse(FunctionOrVariables* input) { return static_cast<T*>(this)->traverseFunctionOrVariables(input); }
                    inline bool visitFunctionOrVariables(FunctionOrVariables* input) { return true; }
                    inline bool endFunctionOrVariables(FunctionOrVariables* input) { return true; }
                    inline bool visit(FunctionOrVariable* input) { return static_cast<T*>(this)->visitFunctionOrVariable(input); }
                    inline bool end(FunctionOrVariable* input) { return static_cast<T*>(this)->endFunctionOrVariable(input); }
                    inline bool traverse(FunctionOrVariable* input) { return static_cast<T*>(this)->traverseFunctionOrVariable(input); }
                    inline bool visitFunctionOrVariable(FunctionOrVariable* input) { return true; }
                    inline bool endFunctionOrVariable(FunctionOrVariable* input) { return true; }
                    bool traverseFunctionOrVariables(FunctionOrVariables* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseFunctionOrVariable(FunctionOrVariable* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Specifiers) if(!traverse(input->m_Specifiers)) return false;
                        if (input->m_TypeOrAuto) if(!traverse(input->m_TypeOrAuto)) return false;
                        if (input->m_OperatorEnd) if(!traverse(input->m_OperatorEnd)) return false;
                        if (input->m_FunctionOrVariableEnd) if(!traverse(input->m_FunctionOrVariableEnd)) return false;
                        return end(input);
                    }
                    inline bool visit(DeclTypes* input) { return static_cast<T*>(this)->visitDeclTypes(input); }
                    inline bool end(DeclTypes* input) { return static_cast<T*>(this)->endDeclTypes(input); }
                    inline bool traverse(DeclTypes* input) { return static_cast<T*>(this)->traverseDeclTypes(input); }
                    inline bool visitDeclTypes(DeclTypes* input) { return true; }
                    inline bool endDeclTypes(DeclTypes* input) { return true; }
                    inline bool visit(DeclType* input) { return static_cast<T*>(this)->visitDeclType(input); }
                    inline bool end(DeclType* input) { return static_cast<T*>(this)->endDeclType(input); }
                    inline bool traverse(DeclType* input) { return static_cast<T*>(this)->traverseDeclType(input); }
                    inline bool visitDeclType(DeclType* input) { return true; }
                    inline bool endDeclType(DeclType* input) { return true; }
                    bool traverseDeclTypes(DeclTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseDeclType(DeclType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(TemplateArguments* input) { return static_cast<T*>(this)->visitTemplateArguments(input); }
                    inline bool end(TemplateArguments* input) { return static_cast<T*>(this)->endTemplateArguments(input); }
                    inline bool traverse(TemplateArguments* input) { return static_cast<T*>(this)->traverseTemplateArguments(input); }
                    inline bool visitTemplateArguments(TemplateArguments* input) { return true; }
                    inline bool endTemplateArguments(TemplateArguments* input) { return true; }
                    inline bool visit(TemplateArgument* input) { return static_cast<T*>(this)->visitTemplateArgument(input); }
                    inline bool end(TemplateArgument* input) { return static_cast<T*>(this)->endTemplateArgument(input); }
                    inline bool traverse(TemplateArgument* input) { return static_cast<T*>(this)->traverseTemplateArgument(input); }
                    inline bool visitTemplateArgument(TemplateArgument* input) { return true; }
                    inline bool endTemplateArgument(TemplateArgument* input) { return true; }
                    bool traverseTemplateArguments(TemplateArguments* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplateArgument(TemplateArgument* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_LastTemplateArgument: return traverse(static_cast<LastTemplateArgument*>(input));
                            case e_RuleType_NotLastTemplateArgument: return traverse(static_cast<NotLastTemplateArgument*>(input));
                            default:
                            {
                                return visit(input) && end(input);
                            }
                        }
                    }
                    inline bool visit(ForInits* input) { return static_cast<T*>(this)->visitForInits(input); }
                    inline bool end(ForInits* input) { return static_cast<T*>(this)->endForInits(input); }
                    inline bool traverse(ForInits* input) { return static_cast<T*>(this)->traverseForInits(input); }
                    inline bool visitForInits(ForInits* input) { return true; }
                    inline bool endForInits(ForInits* input) { return true; }
                    inline bool visit(ForInit* input) { return static_cast<T*>(this)->visitForInit(input); }
                    inline bool end(ForInit* input) { return static_cast<T*>(this)->endForInit(input); }
                    inline bool traverse(ForInit* input) { return static_cast<T*>(this)->traverseForInit(input); }
                    inline bool visitForInit(ForInit* input) { return true; }
                    inline bool endForInit(ForInit* input) { return true; }
                    bool traverseForInits(ForInits* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseForInit(ForInit* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LocalVariableDecl) if(!traverse(input->m_LocalVariableDecl)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(Statements* input) { return static_cast<T*>(this)->visitStatements(input); }
                    inline bool end(Statements* input) { return static_cast<T*>(this)->endStatements(input); }
                    inline bool traverse(Statements* input) { return static_cast<T*>(this)->traverseStatements(input); }
                    inline bool visitStatements(Statements* input) { return true; }
                    inline bool endStatements(Statements* input) { return true; }
                    inline bool visit(Statement* input) { return static_cast<T*>(this)->visitStatement(input); }
                    inline bool end(Statement* input) { return static_cast<T*>(this)->endStatement(input); }
                    inline bool traverse(Statement* input) { return static_cast<T*>(this)->traverseStatement(input); }
                    inline bool visitStatement(Statement* input) { return true; }
                    inline bool endStatement(Statement* input) { return true; }
                    bool traverseStatements(Statements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseStatement(Statement* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_BlockDeclaration: return traverse(static_cast<BlockDeclaration*>(input));
                            case e_RuleType_StatementOrCase: return traverse(static_cast<StatementOrCase*>(input));
                            case e_RuleType_IfStatement: return traverse(static_cast<IfStatement*>(input));
                            case e_RuleType_CaseStatement: return traverse(static_cast<CaseStatement*>(input));
                            case e_RuleType_DefaultStatement: return traverse(static_cast<DefaultStatement*>(input));
                            case e_RuleType_LabelStatement: return traverse(static_cast<LabelStatement*>(input));
                            case e_RuleType_SwitchStatement: return traverse(static_cast<SwitchStatement*>(input));
                            case e_RuleType_ForStatement: return traverse(static_cast<ForStatement*>(input));
                            case e_RuleType_WhileStatement: return traverse(static_cast<WhileStatement*>(input));
                            case e_RuleType_BlockStatement: return traverse(static_cast<BlockStatement*>(input));
                            case e_RuleType_DoWhileStatement: return traverse(static_cast<DoWhileStatement*>(input));
                            case e_RuleType_LocalVariableStatement: return traverse(static_cast<LocalVariableStatement*>(input));
                            case e_RuleType_ExpressionStatement: return traverse(static_cast<ExpressionStatement*>(input));
                            case e_RuleType_AssertStatement: return traverse(static_cast<AssertStatement*>(input));
                            case e_RuleType_BreakStatement: return traverse(static_cast<BreakStatement*>(input));
                            case e_RuleType_ContinueStatement: return traverse(static_cast<ContinueStatement*>(input));
                            default:
                            {
                                if (!visit(input)) return false;
                                if (input->m_IfStatement) if(!traverse(input->m_IfStatement)) return false;
                                if (input->m_ForStatement) if(!traverse(input->m_ForStatement)) return false;
                                if (input->m_SwitchStatement) if(!traverse(input->m_SwitchStatement)) return false;
                                if (input->m_WhileStatement) if(!traverse(input->m_WhileStatement)) return false;
                                if (input->m_AssertStatement) if(!traverse(input->m_AssertStatement)) return false;
                                if (input->m_BreakStatement) if(!traverse(input->m_BreakStatement)) return false;
                                if (input->m_ContinueStatement) if(!traverse(input->m_ContinueStatement)) return false;
                                if (input->m_DoWhileStatement) if(!traverse(input->m_DoWhileStatement)) return false;
                                if (input->m_BlockStatement) if(!traverse(input->m_BlockStatement)) return false;
                                if (input->m_ReturnStatement) if(!traverse(input->m_ReturnStatement)) return false;
                                if (input->m_GotoStatement) if(!traverse(input->m_GotoStatement)) return false;
                                if (input->m_BlockDeclaration) if(!traverse(input->m_BlockDeclaration)) return false;
                                if (input->m_MixinStatement) if(!traverse(input->m_MixinStatement)) return false;
                                if (input->m_LabelStatement) if(!traverse(input->m_LabelStatement)) return false;
                                if (input->m_LocalVariableStatement) if(!traverse(input->m_LocalVariableStatement)) return false;
                                if (input->m_ExpressionStatement) if(!traverse(input->m_ExpressionStatement)) return false;
                                return end(input);
                            }
                        }
                    }
                    inline bool visit(MixinStatements* input) { return static_cast<T*>(this)->visitMixinStatements(input); }
                    inline bool end(MixinStatements* input) { return static_cast<T*>(this)->endMixinStatements(input); }
                    inline bool traverse(MixinStatements* input) { return static_cast<T*>(this)->traverseMixinStatements(input); }
                    inline bool visitMixinStatements(MixinStatements* input) { return true; }
                    inline bool endMixinStatements(MixinStatements* input) { return true; }
                    inline bool visit(MixinStatement* input) { return static_cast<T*>(this)->visitMixinStatement(input); }
                    inline bool end(MixinStatement* input) { return static_cast<T*>(this)->endMixinStatement(input); }
                    inline bool traverse(MixinStatement* input) { return static_cast<T*>(this)->traverseMixinStatement(input); }
                    inline bool visitMixinStatement(MixinStatement* input) { return true; }
                    inline bool endMixinStatement(MixinStatement* input) { return true; }
                    bool traverseMixinStatements(MixinStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseMixinStatement(MixinStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Expressions) if(!traverse(input->m_Expressions)) return false;
                        return end(input);
                    }
                    inline bool visit(ReturnStatements* input) { return static_cast<T*>(this)->visitReturnStatements(input); }
                    inline bool end(ReturnStatements* input) { return static_cast<T*>(this)->endReturnStatements(input); }
                    inline bool traverse(ReturnStatements* input) { return static_cast<T*>(this)->traverseReturnStatements(input); }
                    inline bool visitReturnStatements(ReturnStatements* input) { return true; }
                    inline bool endReturnStatements(ReturnStatements* input) { return true; }
                    inline bool visit(ReturnStatement* input) { return static_cast<T*>(this)->visitReturnStatement(input); }
                    inline bool end(ReturnStatement* input) { return static_cast<T*>(this)->endReturnStatement(input); }
                    inline bool traverse(ReturnStatement* input) { return static_cast<T*>(this)->traverseReturnStatement(input); }
                    inline bool visitReturnStatement(ReturnStatement* input) { return true; }
                    inline bool endReturnStatement(ReturnStatement* input) { return true; }
                    bool traverseReturnStatements(ReturnStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseReturnStatement(ReturnStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_pExpression) if(!traverse(input->m_pExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(GotoStatements* input) { return static_cast<T*>(this)->visitGotoStatements(input); }
                    inline bool end(GotoStatements* input) { return static_cast<T*>(this)->endGotoStatements(input); }
                    inline bool traverse(GotoStatements* input) { return static_cast<T*>(this)->traverseGotoStatements(input); }
                    inline bool visitGotoStatements(GotoStatements* input) { return true; }
                    inline bool endGotoStatements(GotoStatements* input) { return true; }
                    inline bool visit(GotoStatement* input) { return static_cast<T*>(this)->visitGotoStatement(input); }
                    inline bool end(GotoStatement* input) { return static_cast<T*>(this)->endGotoStatement(input); }
                    inline bool traverse(GotoStatement* input) { return static_cast<T*>(this)->traverseGotoStatement(input); }
                    inline bool visitGotoStatement(GotoStatement* input) { return true; }
                    inline bool endGotoStatement(GotoStatement* input) { return true; }
                    bool traverseGotoStatements(GotoStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseGotoStatement(GotoStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(Declarators* input) { return static_cast<T*>(this)->visitDeclarators(input); }
                    inline bool end(Declarators* input) { return static_cast<T*>(this)->endDeclarators(input); }
                    inline bool traverse(Declarators* input) { return static_cast<T*>(this)->traverseDeclarators(input); }
                    inline bool visitDeclarators(Declarators* input) { return true; }
                    inline bool endDeclarators(Declarators* input) { return true; }
                    inline bool visit(Declarator* input) { return static_cast<T*>(this)->visitDeclarator(input); }
                    inline bool end(Declarator* input) { return static_cast<T*>(this)->endDeclarator(input); }
                    inline bool traverse(Declarator* input) { return static_cast<T*>(this)->traverseDeclarator(input); }
                    inline bool visitDeclarator(Declarator* input) { return true; }
                    inline bool endDeclarator(Declarator* input) { return true; }
                    bool traverseDeclarators(Declarators* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseDeclarator(Declarator* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TypeExtents) if(!traverse(input->m_TypeExtents)) return false;
                        if (input->m_ArrayExtents) if(!traverse(input->m_ArrayExtents)) return false;
                        if (input->m_AssignExpressionOrInitializerList) if(!traverse(input->m_AssignExpressionOrInitializerList)) return false;
                        if (input->m_CallList) if(!traverse(input->m_CallList)) return false;
                        if (input->m_InitializerList) if(!traverse(input->m_InitializerList)) return false;
                        return end(input);
                    }
                    inline bool visit(TemplateArgumentListSplits* input) { return static_cast<T*>(this)->visitTemplateArgumentListSplits(input); }
                    inline bool end(TemplateArgumentListSplits* input) { return static_cast<T*>(this)->endTemplateArgumentListSplits(input); }
                    inline bool traverse(TemplateArgumentListSplits* input) { return static_cast<T*>(this)->traverseTemplateArgumentListSplits(input); }
                    inline bool visitTemplateArgumentListSplits(TemplateArgumentListSplits* input) { return true; }
                    inline bool endTemplateArgumentListSplits(TemplateArgumentListSplits* input) { return true; }
                    inline bool visit(TemplateArgumentListSplit* input) { return static_cast<T*>(this)->visitTemplateArgumentListSplit(input); }
                    inline bool end(TemplateArgumentListSplit* input) { return static_cast<T*>(this)->endTemplateArgumentListSplit(input); }
                    inline bool traverse(TemplateArgumentListSplit* input) { return static_cast<T*>(this)->traverseTemplateArgumentListSplit(input); }
                    inline bool visitTemplateArgumentListSplit(TemplateArgumentListSplit* input) { return true; }
                    inline bool endTemplateArgumentListSplit(TemplateArgumentListSplit* input) { return true; }
                    bool traverseTemplateArgumentListSplits(TemplateArgumentListSplits* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplateArgumentListSplit(TemplateArgumentListSplit* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_NotLastTemplateArguments) if(!traverse(input->m_NotLastTemplateArguments)) return false;
                        if (input->m_LastTemplateArgument) if(!traverse(input->m_LastTemplateArgument)) return false;
                        return end(input);
                    }
                    inline bool visit(TemplateArgumentNoAssigns* input) { return static_cast<T*>(this)->visitTemplateArgumentNoAssigns(input); }
                    inline bool end(TemplateArgumentNoAssigns* input) { return static_cast<T*>(this)->endTemplateArgumentNoAssigns(input); }
                    inline bool traverse(TemplateArgumentNoAssigns* input) { return static_cast<T*>(this)->traverseTemplateArgumentNoAssigns(input); }
                    inline bool visitTemplateArgumentNoAssigns(TemplateArgumentNoAssigns* input) { return true; }
                    inline bool endTemplateArgumentNoAssigns(TemplateArgumentNoAssigns* input) { return true; }
                    inline bool visit(TemplateArgumentNoAssign* input) { return static_cast<T*>(this)->visitTemplateArgumentNoAssign(input); }
                    inline bool end(TemplateArgumentNoAssign* input) { return static_cast<T*>(this)->endTemplateArgumentNoAssign(input); }
                    inline bool traverse(TemplateArgumentNoAssign* input) { return static_cast<T*>(this)->traverseTemplateArgumentNoAssign(input); }
                    inline bool visitTemplateArgumentNoAssign(TemplateArgumentNoAssign* input) { return true; }
                    inline bool endTemplateArgumentNoAssign(TemplateArgumentNoAssign* input) { return true; }
                    bool traverseTemplateArgumentNoAssigns(TemplateArgumentNoAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplateArgumentNoAssign(TemplateArgumentNoAssign* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_PostFixExpression) if(!traverse(input->m_PostFixExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(TemplateArgumentAssigns* input) { return static_cast<T*>(this)->visitTemplateArgumentAssigns(input); }
                    inline bool end(TemplateArgumentAssigns* input) { return static_cast<T*>(this)->endTemplateArgumentAssigns(input); }
                    inline bool traverse(TemplateArgumentAssigns* input) { return static_cast<T*>(this)->traverseTemplateArgumentAssigns(input); }
                    inline bool visitTemplateArgumentAssigns(TemplateArgumentAssigns* input) { return true; }
                    inline bool endTemplateArgumentAssigns(TemplateArgumentAssigns* input) { return true; }
                    inline bool visit(TemplateArgumentAssign* input) { return static_cast<T*>(this)->visitTemplateArgumentAssign(input); }
                    inline bool end(TemplateArgumentAssign* input) { return static_cast<T*>(this)->endTemplateArgumentAssign(input); }
                    inline bool traverse(TemplateArgumentAssign* input) { return static_cast<T*>(this)->traverseTemplateArgumentAssign(input); }
                    inline bool visitTemplateArgumentAssign(TemplateArgumentAssign* input) { return true; }
                    inline bool endTemplateArgumentAssign(TemplateArgumentAssign* input) { return true; }
                    bool traverseTemplateArgumentAssigns(TemplateArgumentAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplateArgumentAssign(TemplateArgumentAssign* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        return end(input);
                    }
                    inline bool visit(TemplateArgumentLists* input) { return static_cast<T*>(this)->visitTemplateArgumentLists(input); }
                    inline bool end(TemplateArgumentLists* input) { return static_cast<T*>(this)->endTemplateArgumentLists(input); }
                    inline bool traverse(TemplateArgumentLists* input) { return static_cast<T*>(this)->traverseTemplateArgumentLists(input); }
                    inline bool visitTemplateArgumentLists(TemplateArgumentLists* input) { return true; }
                    inline bool endTemplateArgumentLists(TemplateArgumentLists* input) { return true; }
                    inline bool visit(TemplateArgumentList* input) { return static_cast<T*>(this)->visitTemplateArgumentList(input); }
                    inline bool end(TemplateArgumentList* input) { return static_cast<T*>(this)->endTemplateArgumentList(input); }
                    inline bool traverse(TemplateArgumentList* input) { return static_cast<T*>(this)->traverseTemplateArgumentList(input); }
                    inline bool visitTemplateArgumentList(TemplateArgumentList* input) { return true; }
                    inline bool endTemplateArgumentList(TemplateArgumentList* input) { return true; }
                    bool traverseTemplateArgumentLists(TemplateArgumentLists* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplateArgumentList(TemplateArgumentList* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TemplateArgumentListSplit) if(!traverse(input->m_TemplateArgumentListSplit)) return false;
                        return end(input);
                    }
                    inline bool visit(Names* input) { return static_cast<T*>(this)->visitNames(input); }
                    inline bool end(Names* input) { return static_cast<T*>(this)->endNames(input); }
                    inline bool traverse(Names* input) { return static_cast<T*>(this)->traverseNames(input); }
                    inline bool visitNames(Names* input) { return true; }
                    inline bool endNames(Names* input) { return true; }
                    inline bool visit(Name* input) { return static_cast<T*>(this)->visitName(input); }
                    inline bool end(Name* input) { return static_cast<T*>(this)->endName(input); }
                    inline bool traverse(Name* input) { return static_cast<T*>(this)->traverseName(input); }
                    inline bool visitName(Name* input) { return true; }
                    inline bool endName(Name* input) { return true; }
                    bool traverseNames(Names* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseName(Name* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Operator) if(!traverse(input->m_Operator)) return false;
                        if (input->m_TemplateArgumentList) if(!traverse(input->m_TemplateArgumentList)) return false;
                        return end(input);
                    }
                    inline bool visit(FundamentalTypeBases* input) { return static_cast<T*>(this)->visitFundamentalTypeBases(input); }
                    inline bool end(FundamentalTypeBases* input) { return static_cast<T*>(this)->endFundamentalTypeBases(input); }
                    inline bool traverse(FundamentalTypeBases* input) { return static_cast<T*>(this)->traverseFundamentalTypeBases(input); }
                    inline bool visitFundamentalTypeBases(FundamentalTypeBases* input) { return true; }
                    inline bool endFundamentalTypeBases(FundamentalTypeBases* input) { return true; }
                    inline bool visit(FundamentalTypeBase* input) { return static_cast<T*>(this)->visitFundamentalTypeBase(input); }
                    inline bool end(FundamentalTypeBase* input) { return static_cast<T*>(this)->endFundamentalTypeBase(input); }
                    inline bool traverse(FundamentalTypeBase* input) { return static_cast<T*>(this)->traverseFundamentalTypeBase(input); }
                    inline bool visitFundamentalTypeBase(FundamentalTypeBase* input) { return true; }
                    inline bool endFundamentalTypeBase(FundamentalTypeBase* input) { return true; }
                    bool traverseFundamentalTypeBases(FundamentalTypeBases* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseFundamentalTypeBase(FundamentalTypeBase* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(AutoDeclarators* input) { return static_cast<T*>(this)->visitAutoDeclarators(input); }
                    inline bool end(AutoDeclarators* input) { return static_cast<T*>(this)->endAutoDeclarators(input); }
                    inline bool traverse(AutoDeclarators* input) { return static_cast<T*>(this)->traverseAutoDeclarators(input); }
                    inline bool visitAutoDeclarators(AutoDeclarators* input) { return true; }
                    inline bool endAutoDeclarators(AutoDeclarators* input) { return true; }
                    inline bool visit(AutoDeclarator* input) { return static_cast<T*>(this)->visitAutoDeclarator(input); }
                    inline bool end(AutoDeclarator* input) { return static_cast<T*>(this)->endAutoDeclarator(input); }
                    inline bool traverse(AutoDeclarator* input) { return static_cast<T*>(this)->traverseAutoDeclarator(input); }
                    inline bool visitAutoDeclarator(AutoDeclarator* input) { return true; }
                    inline bool endAutoDeclarator(AutoDeclarator* input) { return true; }
                    bool traverseAutoDeclarators(AutoDeclarators* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAutoDeclarator(AutoDeclarator* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TypeExtents) if(!traverse(input->m_TypeExtents)) return false;
                        if (input->m_CallList) if(!traverse(input->m_CallList)) return false;
                        if (input->m_InitializerList) if(!traverse(input->m_InitializerList)) return false;
                        if (input->m_ArrayExtents) if(!traverse(input->m_ArrayExtents)) return false;
                        if (input->m_AssignExpressionOrInitializerList) if(!traverse(input->m_AssignExpressionOrInitializerList)) return false;
                        return end(input);
                    }
                    inline bool visit(LocalVariableDecls* input) { return static_cast<T*>(this)->visitLocalVariableDecls(input); }
                    inline bool end(LocalVariableDecls* input) { return static_cast<T*>(this)->endLocalVariableDecls(input); }
                    inline bool traverse(LocalVariableDecls* input) { return static_cast<T*>(this)->traverseLocalVariableDecls(input); }
                    inline bool visitLocalVariableDecls(LocalVariableDecls* input) { return true; }
                    inline bool endLocalVariableDecls(LocalVariableDecls* input) { return true; }
                    inline bool visit(LocalVariableDecl* input) { return static_cast<T*>(this)->visitLocalVariableDecl(input); }
                    inline bool end(LocalVariableDecl* input) { return static_cast<T*>(this)->endLocalVariableDecl(input); }
                    inline bool traverse(LocalVariableDecl* input) { return static_cast<T*>(this)->traverseLocalVariableDecl(input); }
                    inline bool visitLocalVariableDecl(LocalVariableDecl* input) { return true; }
                    inline bool endLocalVariableDecl(LocalVariableDecl* input) { return true; }
                    bool traverseLocalVariableDecls(LocalVariableDecls* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLocalVariableDecl(LocalVariableDecl* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_ExplicitLocalVariableDecl: return traverse(static_cast<ExplicitLocalVariableDecl*>(input));
                            case e_RuleType_AutoLocalVariableDecl: return traverse(static_cast<AutoLocalVariableDecl*>(input));
                            default:
                            {
                                if (!visit(input)) return false;
                                if (input->m_AutoLocalVariableDecl) if(!traverse(input->m_AutoLocalVariableDecl)) return false;
                                if (input->m_ExplicitLocalVariableDecl) if(!traverse(input->m_ExplicitLocalVariableDecl)) return false;
                                return end(input);
                            }
                        }
                    }
                    inline bool visit(Expressions* input) { return static_cast<T*>(this)->visitExpressions(input); }
                    inline bool end(Expressions* input) { return static_cast<T*>(this)->endExpressions(input); }
                    inline bool traverse(Expressions* input) { return static_cast<T*>(this)->traverseExpressions(input); }
                    inline bool visitExpressions(Expressions* input) { return true; }
                    inline bool endExpressions(Expressions* input) { return true; }
                    inline bool visit(Expression* input) { return static_cast<T*>(this)->visitExpression(input); }
                    inline bool end(Expression* input) { return static_cast<T*>(this)->endExpression(input); }
                    inline bool traverse(Expression* input) { return static_cast<T*>(this)->traverseExpression(input); }
                    inline bool visitExpression(Expression* input) { return true; }
                    inline bool endExpression(Expression* input) { return true; }
                    bool traverseExpressions(Expressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseExpression(Expression* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_IntegralConstant: return traverse(static_cast<IntegralConstant*>(input));
                            case e_RuleType_AssignExpression: return traverse(static_cast<AssignExpression*>(input));
                            case e_RuleType_ConditionalExpression: return traverse(static_cast<ConditionalExpression*>(input));
                            case e_RuleType_LogicalOrExpression: return traverse(static_cast<LogicalOrExpression*>(input));
                            case e_RuleType_LogicalAndExpression: return traverse(static_cast<LogicalAndExpression*>(input));
                            case e_RuleType_BinOrExpression: return traverse(static_cast<BinOrExpression*>(input));
                            case e_RuleType_XOrExpression: return traverse(static_cast<XOrExpression*>(input));
                            case e_RuleType_BinAndExpression: return traverse(static_cast<BinAndExpression*>(input));
                            case e_RuleType_EqualityExpression: return traverse(static_cast<EqualityExpression*>(input));
                            case e_RuleType_RelationalExpression: return traverse(static_cast<RelationalExpression*>(input));
                            case e_RuleType_ShiftExpression: return traverse(static_cast<ShiftExpression*>(input));
                            case e_RuleType_AddExpression: return traverse(static_cast<AddExpression*>(input));
                            case e_RuleType_MulExpression: return traverse(static_cast<MulExpression*>(input));
                            case e_RuleType_UnaryExpression: return traverse(static_cast<UnaryExpression*>(input));
                            case e_RuleType_AssignExpressionOrInitializerList: return traverse(static_cast<AssignExpressionOrInitializerList*>(input));
                            case e_RuleType_ExpressionOrInitializerList: return traverse(static_cast<ExpressionOrInitializerList*>(input));
                            case e_RuleType_InitializerList: return traverse(static_cast<InitializerList*>(input));
                            case e_RuleType_ArrayAccess: return traverse(static_cast<ArrayAccess*>(input));
                            case e_RuleType_KeywordExpression: return traverse(static_cast<KeywordExpression*>(input));
                            case e_RuleType_NullptrExpression: return traverse(static_cast<NullptrExpression*>(input));
                            case e_RuleType_PostFixExpression: return traverse(static_cast<PostFixExpression*>(input));
                            case e_RuleType_CStyleCastExpression: return traverse(static_cast<CStyleCastExpression*>(input));
                            case e_RuleType_TypeTraitBinary: return traverse(static_cast<TypeTraitBinary*>(input));
                            case e_RuleType_SizeofExpression: return traverse(static_cast<SizeofExpression*>(input));
                            case e_RuleType_TypeofExpression: return traverse(static_cast<TypeofExpression*>(input));
                            case e_RuleType_FundamentalTypeFunctionCast: return traverse(static_cast<FundamentalTypeFunctionCast*>(input));
                            case e_RuleType_PrimaryExpression: return traverse(static_cast<PrimaryExpression*>(input));
                            default:
                            {
                                if (!visit(input)) return false;
                                if (input->m_Expression_AssignExpression) if(!traverse(input->m_Expression_AssignExpression)) return false;
                                if (input->m_Expression_Expression) if(!traverse(input->m_Expression_Expression)) return false;
                                return end(input);
                            }
                        }
                    }
                    inline bool visit(LogicalOrExpressionRights* input) { return static_cast<T*>(this)->visitLogicalOrExpressionRights(input); }
                    inline bool end(LogicalOrExpressionRights* input) { return static_cast<T*>(this)->endLogicalOrExpressionRights(input); }
                    inline bool traverse(LogicalOrExpressionRights* input) { return static_cast<T*>(this)->traverseLogicalOrExpressionRights(input); }
                    inline bool visitLogicalOrExpressionRights(LogicalOrExpressionRights* input) { return true; }
                    inline bool endLogicalOrExpressionRights(LogicalOrExpressionRights* input) { return true; }
                    inline bool visit(LogicalOrExpressionRight* input) { return static_cast<T*>(this)->visitLogicalOrExpressionRight(input); }
                    inline bool end(LogicalOrExpressionRight* input) { return static_cast<T*>(this)->endLogicalOrExpressionRight(input); }
                    inline bool traverse(LogicalOrExpressionRight* input) { return static_cast<T*>(this)->traverseLogicalOrExpressionRight(input); }
                    inline bool visitLogicalOrExpressionRight(LogicalOrExpressionRight* input) { return true; }
                    inline bool endLogicalOrExpressionRight(LogicalOrExpressionRight* input) { return true; }
                    bool traverseLogicalOrExpressionRights(LogicalOrExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLogicalOrExpressionRight(LogicalOrExpressionRight* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_RightExpression) if(!traverse(input->m_RightExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(LogicalAndExpressionRights* input) { return static_cast<T*>(this)->visitLogicalAndExpressionRights(input); }
                    inline bool end(LogicalAndExpressionRights* input) { return static_cast<T*>(this)->endLogicalAndExpressionRights(input); }
                    inline bool traverse(LogicalAndExpressionRights* input) { return static_cast<T*>(this)->traverseLogicalAndExpressionRights(input); }
                    inline bool visitLogicalAndExpressionRights(LogicalAndExpressionRights* input) { return true; }
                    inline bool endLogicalAndExpressionRights(LogicalAndExpressionRights* input) { return true; }
                    inline bool visit(LogicalAndExpressionRight* input) { return static_cast<T*>(this)->visitLogicalAndExpressionRight(input); }
                    inline bool end(LogicalAndExpressionRight* input) { return static_cast<T*>(this)->endLogicalAndExpressionRight(input); }
                    inline bool traverse(LogicalAndExpressionRight* input) { return static_cast<T*>(this)->traverseLogicalAndExpressionRight(input); }
                    inline bool visitLogicalAndExpressionRight(LogicalAndExpressionRight* input) { return true; }
                    inline bool endLogicalAndExpressionRight(LogicalAndExpressionRight* input) { return true; }
                    bool traverseLogicalAndExpressionRights(LogicalAndExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLogicalAndExpressionRight(LogicalAndExpressionRight* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_RightExpression) if(!traverse(input->m_RightExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(BinOrExpressionRights* input) { return static_cast<T*>(this)->visitBinOrExpressionRights(input); }
                    inline bool end(BinOrExpressionRights* input) { return static_cast<T*>(this)->endBinOrExpressionRights(input); }
                    inline bool traverse(BinOrExpressionRights* input) { return static_cast<T*>(this)->traverseBinOrExpressionRights(input); }
                    inline bool visitBinOrExpressionRights(BinOrExpressionRights* input) { return true; }
                    inline bool endBinOrExpressionRights(BinOrExpressionRights* input) { return true; }
                    inline bool visit(BinOrExpressionRight* input) { return static_cast<T*>(this)->visitBinOrExpressionRight(input); }
                    inline bool end(BinOrExpressionRight* input) { return static_cast<T*>(this)->endBinOrExpressionRight(input); }
                    inline bool traverse(BinOrExpressionRight* input) { return static_cast<T*>(this)->traverseBinOrExpressionRight(input); }
                    inline bool visitBinOrExpressionRight(BinOrExpressionRight* input) { return true; }
                    inline bool endBinOrExpressionRight(BinOrExpressionRight* input) { return true; }
                    bool traverseBinOrExpressionRights(BinOrExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBinOrExpressionRight(BinOrExpressionRight* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_RightExpression) if(!traverse(input->m_RightExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(XOrExpressionRights* input) { return static_cast<T*>(this)->visitXOrExpressionRights(input); }
                    inline bool end(XOrExpressionRights* input) { return static_cast<T*>(this)->endXOrExpressionRights(input); }
                    inline bool traverse(XOrExpressionRights* input) { return static_cast<T*>(this)->traverseXOrExpressionRights(input); }
                    inline bool visitXOrExpressionRights(XOrExpressionRights* input) { return true; }
                    inline bool endXOrExpressionRights(XOrExpressionRights* input) { return true; }
                    inline bool visit(XOrExpressionRight* input) { return static_cast<T*>(this)->visitXOrExpressionRight(input); }
                    inline bool end(XOrExpressionRight* input) { return static_cast<T*>(this)->endXOrExpressionRight(input); }
                    inline bool traverse(XOrExpressionRight* input) { return static_cast<T*>(this)->traverseXOrExpressionRight(input); }
                    inline bool visitXOrExpressionRight(XOrExpressionRight* input) { return true; }
                    inline bool endXOrExpressionRight(XOrExpressionRight* input) { return true; }
                    bool traverseXOrExpressionRights(XOrExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseXOrExpressionRight(XOrExpressionRight* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_RightExpression) if(!traverse(input->m_RightExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(BinAndExpressionRights* input) { return static_cast<T*>(this)->visitBinAndExpressionRights(input); }
                    inline bool end(BinAndExpressionRights* input) { return static_cast<T*>(this)->endBinAndExpressionRights(input); }
                    inline bool traverse(BinAndExpressionRights* input) { return static_cast<T*>(this)->traverseBinAndExpressionRights(input); }
                    inline bool visitBinAndExpressionRights(BinAndExpressionRights* input) { return true; }
                    inline bool endBinAndExpressionRights(BinAndExpressionRights* input) { return true; }
                    inline bool visit(BinAndExpressionRight* input) { return static_cast<T*>(this)->visitBinAndExpressionRight(input); }
                    inline bool end(BinAndExpressionRight* input) { return static_cast<T*>(this)->endBinAndExpressionRight(input); }
                    inline bool traverse(BinAndExpressionRight* input) { return static_cast<T*>(this)->traverseBinAndExpressionRight(input); }
                    inline bool visitBinAndExpressionRight(BinAndExpressionRight* input) { return true; }
                    inline bool endBinAndExpressionRight(BinAndExpressionRight* input) { return true; }
                    bool traverseBinAndExpressionRights(BinAndExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBinAndExpressionRight(BinAndExpressionRight* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_RightExpression) if(!traverse(input->m_RightExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(EqualityExpressionRights* input) { return static_cast<T*>(this)->visitEqualityExpressionRights(input); }
                    inline bool end(EqualityExpressionRights* input) { return static_cast<T*>(this)->endEqualityExpressionRights(input); }
                    inline bool traverse(EqualityExpressionRights* input) { return static_cast<T*>(this)->traverseEqualityExpressionRights(input); }
                    inline bool visitEqualityExpressionRights(EqualityExpressionRights* input) { return true; }
                    inline bool endEqualityExpressionRights(EqualityExpressionRights* input) { return true; }
                    inline bool visit(EqualityExpressionRight* input) { return static_cast<T*>(this)->visitEqualityExpressionRight(input); }
                    inline bool end(EqualityExpressionRight* input) { return static_cast<T*>(this)->endEqualityExpressionRight(input); }
                    inline bool traverse(EqualityExpressionRight* input) { return static_cast<T*>(this)->traverseEqualityExpressionRight(input); }
                    inline bool visitEqualityExpressionRight(EqualityExpressionRight* input) { return true; }
                    inline bool endEqualityExpressionRight(EqualityExpressionRight* input) { return true; }
                    bool traverseEqualityExpressionRights(EqualityExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseEqualityExpressionRight(EqualityExpressionRight* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_RightExpression) if(!traverse(input->m_RightExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(RelationalExpressionRights* input) { return static_cast<T*>(this)->visitRelationalExpressionRights(input); }
                    inline bool end(RelationalExpressionRights* input) { return static_cast<T*>(this)->endRelationalExpressionRights(input); }
                    inline bool traverse(RelationalExpressionRights* input) { return static_cast<T*>(this)->traverseRelationalExpressionRights(input); }
                    inline bool visitRelationalExpressionRights(RelationalExpressionRights* input) { return true; }
                    inline bool endRelationalExpressionRights(RelationalExpressionRights* input) { return true; }
                    inline bool visit(RelationalExpressionRight* input) { return static_cast<T*>(this)->visitRelationalExpressionRight(input); }
                    inline bool end(RelationalExpressionRight* input) { return static_cast<T*>(this)->endRelationalExpressionRight(input); }
                    inline bool traverse(RelationalExpressionRight* input) { return static_cast<T*>(this)->traverseRelationalExpressionRight(input); }
                    inline bool visitRelationalExpressionRight(RelationalExpressionRight* input) { return true; }
                    inline bool endRelationalExpressionRight(RelationalExpressionRight* input) { return true; }
                    bool traverseRelationalExpressionRights(RelationalExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseRelationalExpressionRight(RelationalExpressionRight* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_RightExpression) if(!traverse(input->m_RightExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(Types* input) { return static_cast<T*>(this)->visitTypes(input); }
                    inline bool end(Types* input) { return static_cast<T*>(this)->endTypes(input); }
                    inline bool traverse(Types* input) { return static_cast<T*>(this)->traverseTypes(input); }
                    inline bool visitTypes(Types* input) { return true; }
                    inline bool endTypes(Types* input) { return true; }
                    inline bool visit(Type* input) { return static_cast<T*>(this)->visitType(input); }
                    inline bool end(Type* input) { return static_cast<T*>(this)->endType(input); }
                    inline bool traverse(Type* input) { return static_cast<T*>(this)->traverseType(input); }
                    inline bool visitType(Type* input) { return true; }
                    inline bool endType(Type* input) { return true; }
                    bool traverseTypes(Types* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseType(Type* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_UnsignedType: return traverse(static_cast<UnsignedType*>(input));
                            case e_RuleType_SignedType: return traverse(static_cast<SignedType*>(input));
                            case e_RuleType_LongType: return traverse(static_cast<LongType*>(input));
                            case e_RuleType_VoidType: return traverse(static_cast<VoidType*>(input));
                            case e_RuleType_FundamentalType: return traverse(static_cast<FundamentalType*>(input));
                            case e_RuleType_QualifiedType: return traverse(static_cast<QualifiedType*>(input));
                            case e_RuleType_ExplicitType: return traverse(static_cast<ExplicitType*>(input));
                            case e_RuleType_TypeNoFunctionType: return traverse(static_cast<TypeNoFunctionType*>(input));
                            case e_RuleType_ExplicitTypeNoFunctionType: return traverse(static_cast<ExplicitTypeNoFunctionType*>(input));
                            case e_RuleType_ExpressionType: return traverse(static_cast<ExpressionType*>(input));
                            case e_RuleType_ExplicitTypeOrAuto: return traverse(static_cast<ExplicitTypeOrAuto*>(input));
                            case e_RuleType_TypeOrAuto: return traverse(static_cast<TypeOrAuto*>(input));
                            case e_RuleType_AutoType: return traverse(static_cast<AutoType*>(input));
                            case e_RuleType_BasicType: return traverse(static_cast<BasicType*>(input));
                            case e_RuleType_Auto: return traverse(static_cast<Auto*>(input));
                            case e_RuleType_BasicTypeOrAuto: return traverse(static_cast<BasicTypeOrAuto*>(input));
                            default:
                            {
                                if (!visit(input)) return false;
                                if (input->m_ExplicitType) if(!traverse(input->m_ExplicitType)) return false;
                                if (input->m_ExpressionType) if(!traverse(input->m_ExpressionType)) return false;
                                return end(input);
                            }
                        }
                    }
                    inline bool visit(ShiftExpressionRights* input) { return static_cast<T*>(this)->visitShiftExpressionRights(input); }
                    inline bool end(ShiftExpressionRights* input) { return static_cast<T*>(this)->endShiftExpressionRights(input); }
                    inline bool traverse(ShiftExpressionRights* input) { return static_cast<T*>(this)->traverseShiftExpressionRights(input); }
                    inline bool visitShiftExpressionRights(ShiftExpressionRights* input) { return true; }
                    inline bool endShiftExpressionRights(ShiftExpressionRights* input) { return true; }
                    inline bool visit(ShiftExpressionRight* input) { return static_cast<T*>(this)->visitShiftExpressionRight(input); }
                    inline bool end(ShiftExpressionRight* input) { return static_cast<T*>(this)->endShiftExpressionRight(input); }
                    inline bool traverse(ShiftExpressionRight* input) { return static_cast<T*>(this)->traverseShiftExpressionRight(input); }
                    inline bool visitShiftExpressionRight(ShiftExpressionRight* input) { return true; }
                    inline bool endShiftExpressionRight(ShiftExpressionRight* input) { return true; }
                    bool traverseShiftExpressionRights(ShiftExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseShiftExpressionRight(ShiftExpressionRight* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_RightExpression) if(!traverse(input->m_RightExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(AddExpressionRights* input) { return static_cast<T*>(this)->visitAddExpressionRights(input); }
                    inline bool end(AddExpressionRights* input) { return static_cast<T*>(this)->endAddExpressionRights(input); }
                    inline bool traverse(AddExpressionRights* input) { return static_cast<T*>(this)->traverseAddExpressionRights(input); }
                    inline bool visitAddExpressionRights(AddExpressionRights* input) { return true; }
                    inline bool endAddExpressionRights(AddExpressionRights* input) { return true; }
                    inline bool visit(AddExpressionRight* input) { return static_cast<T*>(this)->visitAddExpressionRight(input); }
                    inline bool end(AddExpressionRight* input) { return static_cast<T*>(this)->endAddExpressionRight(input); }
                    inline bool traverse(AddExpressionRight* input) { return static_cast<T*>(this)->traverseAddExpressionRight(input); }
                    inline bool visitAddExpressionRight(AddExpressionRight* input) { return true; }
                    inline bool endAddExpressionRight(AddExpressionRight* input) { return true; }
                    bool traverseAddExpressionRights(AddExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAddExpressionRight(AddExpressionRight* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_RightExpression) if(!traverse(input->m_RightExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(MulExpressionRights* input) { return static_cast<T*>(this)->visitMulExpressionRights(input); }
                    inline bool end(MulExpressionRights* input) { return static_cast<T*>(this)->endMulExpressionRights(input); }
                    inline bool traverse(MulExpressionRights* input) { return static_cast<T*>(this)->traverseMulExpressionRights(input); }
                    inline bool visitMulExpressionRights(MulExpressionRights* input) { return true; }
                    inline bool endMulExpressionRights(MulExpressionRights* input) { return true; }
                    inline bool visit(MulExpressionRight* input) { return static_cast<T*>(this)->visitMulExpressionRight(input); }
                    inline bool end(MulExpressionRight* input) { return static_cast<T*>(this)->endMulExpressionRight(input); }
                    inline bool traverse(MulExpressionRight* input) { return static_cast<T*>(this)->traverseMulExpressionRight(input); }
                    inline bool visitMulExpressionRight(MulExpressionRight* input) { return true; }
                    inline bool endMulExpressionRight(MulExpressionRight* input) { return true; }
                    bool traverseMulExpressionRights(MulExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseMulExpressionRight(MulExpressionRight* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_RightExpression) if(!traverse(input->m_RightExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(SpecialCastExpressions* input) { return static_cast<T*>(this)->visitSpecialCastExpressions(input); }
                    inline bool end(SpecialCastExpressions* input) { return static_cast<T*>(this)->endSpecialCastExpressions(input); }
                    inline bool traverse(SpecialCastExpressions* input) { return static_cast<T*>(this)->traverseSpecialCastExpressions(input); }
                    inline bool visitSpecialCastExpressions(SpecialCastExpressions* input) { return true; }
                    inline bool endSpecialCastExpressions(SpecialCastExpressions* input) { return true; }
                    inline bool visit(SpecialCastExpression* input) { return static_cast<T*>(this)->visitSpecialCastExpression(input); }
                    inline bool end(SpecialCastExpression* input) { return static_cast<T*>(this)->endSpecialCastExpression(input); }
                    inline bool traverse(SpecialCastExpression* input) { return static_cast<T*>(this)->traverseSpecialCastExpression(input); }
                    inline bool visitSpecialCastExpression(SpecialCastExpression* input) { return true; }
                    inline bool endSpecialCastExpression(SpecialCastExpression* input) { return true; }
                    bool traverseSpecialCastExpressions(SpecialCastExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseSpecialCastExpression(SpecialCastExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(FunctionPtrExpressions* input) { return static_cast<T*>(this)->visitFunctionPtrExpressions(input); }
                    inline bool end(FunctionPtrExpressions* input) { return static_cast<T*>(this)->endFunctionPtrExpressions(input); }
                    inline bool traverse(FunctionPtrExpressions* input) { return static_cast<T*>(this)->traverseFunctionPtrExpressions(input); }
                    inline bool visitFunctionPtrExpressions(FunctionPtrExpressions* input) { return true; }
                    inline bool endFunctionPtrExpressions(FunctionPtrExpressions* input) { return true; }
                    inline bool visit(FunctionPtrExpression* input) { return static_cast<T*>(this)->visitFunctionPtrExpression(input); }
                    inline bool end(FunctionPtrExpression* input) { return static_cast<T*>(this)->endFunctionPtrExpression(input); }
                    inline bool traverse(FunctionPtrExpression* input) { return static_cast<T*>(this)->traverseFunctionPtrExpression(input); }
                    inline bool visitFunctionPtrExpression(FunctionPtrExpression* input) { return true; }
                    inline bool endFunctionPtrExpression(FunctionPtrExpression* input) { return true; }
                    bool traverseFunctionPtrExpressions(FunctionPtrExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseFunctionPtrExpression(FunctionPtrExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Name) if(!traverse(input->m_Name)) return false;
                        return end(input);
                    }
                    inline bool visit(CallLists* input) { return static_cast<T*>(this)->visitCallLists(input); }
                    inline bool end(CallLists* input) { return static_cast<T*>(this)->endCallLists(input); }
                    inline bool traverse(CallLists* input) { return static_cast<T*>(this)->traverseCallLists(input); }
                    inline bool visitCallLists(CallLists* input) { return true; }
                    inline bool endCallLists(CallLists* input) { return true; }
                    inline bool visit(CallList* input) { return static_cast<T*>(this)->visitCallList(input); }
                    inline bool end(CallList* input) { return static_cast<T*>(this)->endCallList(input); }
                    inline bool traverse(CallList* input) { return static_cast<T*>(this)->traverseCallList(input); }
                    inline bool visitCallList(CallList* input) { return true; }
                    inline bool endCallList(CallList* input) { return true; }
                    bool traverseCallLists(CallLists* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseCallList(CallList* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_AssignExpressionOrInitializerLists) if(!traverse(input->m_AssignExpressionOrInitializerLists)) return false;
                        return end(input);
                    }
                    inline bool visit(PostFixEnds* input) { return static_cast<T*>(this)->visitPostFixEnds(input); }
                    inline bool end(PostFixEnds* input) { return static_cast<T*>(this)->endPostFixEnds(input); }
                    inline bool traverse(PostFixEnds* input) { return static_cast<T*>(this)->traversePostFixEnds(input); }
                    inline bool visitPostFixEnds(PostFixEnds* input) { return true; }
                    inline bool endPostFixEnds(PostFixEnds* input) { return true; }
                    inline bool visit(PostFixEnd* input) { return static_cast<T*>(this)->visitPostFixEnd(input); }
                    inline bool end(PostFixEnd* input) { return static_cast<T*>(this)->endPostFixEnd(input); }
                    inline bool traverse(PostFixEnd* input) { return static_cast<T*>(this)->traversePostFixEnd(input); }
                    inline bool visitPostFixEnd(PostFixEnd* input) { return true; }
                    inline bool endPostFixEnd(PostFixEnd* input) { return true; }
                    bool traversePostFixEnds(PostFixEnds* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traversePostFixEnd(PostFixEnd* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_CallList) if(!traverse(input->m_CallList)) return false;
                        if (input->m_ArrayAccess) if(!traverse(input->m_ArrayAccess)) return false;
                        if (input->m_InitializerList) if(!traverse(input->m_InitializerList)) return false;
                        if (input->m_Name) if(!traverse(input->m_Name)) return false;
                        return end(input);
                    }
                    inline bool visit(ArrayExtentExpressions* input) { return static_cast<T*>(this)->visitArrayExtentExpressions(input); }
                    inline bool end(ArrayExtentExpressions* input) { return static_cast<T*>(this)->endArrayExtentExpressions(input); }
                    inline bool traverse(ArrayExtentExpressions* input) { return static_cast<T*>(this)->traverseArrayExtentExpressions(input); }
                    inline bool visitArrayExtentExpressions(ArrayExtentExpressions* input) { return true; }
                    inline bool endArrayExtentExpressions(ArrayExtentExpressions* input) { return true; }
                    inline bool visit(ArrayExtentExpression* input) { return static_cast<T*>(this)->visitArrayExtentExpression(input); }
                    inline bool end(ArrayExtentExpression* input) { return static_cast<T*>(this)->endArrayExtentExpression(input); }
                    inline bool traverse(ArrayExtentExpression* input) { return static_cast<T*>(this)->traverseArrayExtentExpression(input); }
                    inline bool visitArrayExtentExpression(ArrayExtentExpression* input) { return true; }
                    inline bool endArrayExtentExpression(ArrayExtentExpression* input) { return true; }
                    bool traverseArrayExtentExpressions(ArrayExtentExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseArrayExtentExpression(ArrayExtentExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_IntegralConstant) if(!traverse(input->m_IntegralConstant)) return false;
                        if (input->m_AssignExpression) if(!traverse(input->m_AssignExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(StaticAsserts* input) { return static_cast<T*>(this)->visitStaticAsserts(input); }
                    inline bool end(StaticAsserts* input) { return static_cast<T*>(this)->endStaticAsserts(input); }
                    inline bool traverse(StaticAsserts* input) { return static_cast<T*>(this)->traverseStaticAsserts(input); }
                    inline bool visitStaticAsserts(StaticAsserts* input) { return true; }
                    inline bool endStaticAsserts(StaticAsserts* input) { return true; }
                    inline bool visit(StaticAssert* input) { return static_cast<T*>(this)->visitStaticAssert(input); }
                    inline bool end(StaticAssert* input) { return static_cast<T*>(this)->endStaticAssert(input); }
                    inline bool traverse(StaticAssert* input) { return static_cast<T*>(this)->traverseStaticAssert(input); }
                    inline bool visitStaticAssert(StaticAssert* input) { return true; }
                    inline bool endStaticAssert(StaticAssert* input) { return true; }
                    bool traverseStaticAsserts(StaticAsserts* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseStaticAssert(StaticAssert* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_AssignExpression) if(!traverse(input->m_AssignExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(StringLiteralChains* input) { return static_cast<T*>(this)->visitStringLiteralChains(input); }
                    inline bool end(StringLiteralChains* input) { return static_cast<T*>(this)->endStringLiteralChains(input); }
                    inline bool traverse(StringLiteralChains* input) { return static_cast<T*>(this)->traverseStringLiteralChains(input); }
                    inline bool visitStringLiteralChains(StringLiteralChains* input) { return true; }
                    inline bool endStringLiteralChains(StringLiteralChains* input) { return true; }
                    inline bool visit(StringLiteralChain* input) { return static_cast<T*>(this)->visitStringLiteralChain(input); }
                    inline bool end(StringLiteralChain* input) { return static_cast<T*>(this)->endStringLiteralChain(input); }
                    inline bool traverse(StringLiteralChain* input) { return static_cast<T*>(this)->traverseStringLiteralChain(input); }
                    inline bool visitStringLiteralChain(StringLiteralChain* input) { return true; }
                    inline bool endStringLiteralChain(StringLiteralChain* input) { return true; }
                    bool traverseStringLiteralChains(StringLiteralChains* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseStringLiteralChain(StringLiteralChain* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(LambdaCaptures* input) { return static_cast<T*>(this)->visitLambdaCaptures(input); }
                    inline bool end(LambdaCaptures* input) { return static_cast<T*>(this)->endLambdaCaptures(input); }
                    inline bool traverse(LambdaCaptures* input) { return static_cast<T*>(this)->traverseLambdaCaptures(input); }
                    inline bool visitLambdaCaptures(LambdaCaptures* input) { return true; }
                    inline bool endLambdaCaptures(LambdaCaptures* input) { return true; }
                    inline bool visit(LambdaCapture* input) { return static_cast<T*>(this)->visitLambdaCapture(input); }
                    inline bool end(LambdaCapture* input) { return static_cast<T*>(this)->endLambdaCapture(input); }
                    inline bool traverse(LambdaCapture* input) { return static_cast<T*>(this)->traverseLambdaCapture(input); }
                    inline bool visitLambdaCapture(LambdaCapture* input) { return true; }
                    inline bool endLambdaCapture(LambdaCapture* input) { return true; }
                    bool traverseLambdaCaptures(LambdaCaptures* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLambdaCapture(LambdaCapture* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(LambdaExpressions* input) { return static_cast<T*>(this)->visitLambdaExpressions(input); }
                    inline bool end(LambdaExpressions* input) { return static_cast<T*>(this)->endLambdaExpressions(input); }
                    inline bool traverse(LambdaExpressions* input) { return static_cast<T*>(this)->traverseLambdaExpressions(input); }
                    inline bool visitLambdaExpressions(LambdaExpressions* input) { return true; }
                    inline bool endLambdaExpressions(LambdaExpressions* input) { return true; }
                    inline bool visit(LambdaExpression* input) { return static_cast<T*>(this)->visitLambdaExpression(input); }
                    inline bool end(LambdaExpression* input) { return static_cast<T*>(this)->endLambdaExpression(input); }
                    inline bool traverse(LambdaExpression* input) { return static_cast<T*>(this)->traverseLambdaExpression(input); }
                    inline bool visitLambdaExpression(LambdaExpression* input) { return true; }
                    inline bool endLambdaExpression(LambdaExpression* input) { return true; }
                    bool traverseLambdaExpressions(LambdaExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLambdaExpression(LambdaExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LambdaCaptures) if(!traverse(input->m_LambdaCaptures)) return false;
                        if (input->m_ArrowReturn) if(!traverse(input->m_ArrowReturn)) return false;
                        if (input->m_Parameters) if(!traverse(input->m_Parameters)) return false;
                        if (input->m_FunctionBlock) if(!traverse(input->m_FunctionBlock)) return false;
                        return end(input);
                    }
                    inline bool visit(TypeExtents* input) { return static_cast<T*>(this)->visitTypeExtents(input); }
                    inline bool end(TypeExtents* input) { return static_cast<T*>(this)->endTypeExtents(input); }
                    inline bool traverse(TypeExtents* input) { return static_cast<T*>(this)->traverseTypeExtents(input); }
                    inline bool visitTypeExtents(TypeExtents* input) { return true; }
                    inline bool endTypeExtents(TypeExtents* input) { return true; }
                    inline bool visit(TypeExtent* input) { return static_cast<T*>(this)->visitTypeExtent(input); }
                    inline bool end(TypeExtent* input) { return static_cast<T*>(this)->endTypeExtent(input); }
                    inline bool traverse(TypeExtent* input) { return static_cast<T*>(this)->traverseTypeExtent(input); }
                    inline bool visitTypeExtent(TypeExtent* input) { return true; }
                    inline bool endTypeExtent(TypeExtent* input) { return true; }
                    bool traverseTypeExtents(TypeExtents* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTypeExtent(TypeExtent* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_ArrayExtent: return traverse(static_cast<ArrayExtent*>(input));
                            case e_RuleType_FunctionTypeExtent: return traverse(static_cast<FunctionTypeExtent*>(input));
                            case e_RuleType_DerivedExtent: return traverse(static_cast<DerivedExtent*>(input));
                            case e_RuleType_TypeExtentNoFunction: return traverse(static_cast<TypeExtentNoFunction*>(input));
                            default:
                            {
                                if (!visit(input)) return false;
                                if (input->m_ArrayExtent) if(!traverse(input->m_ArrayExtent)) return false;
                                if (input->m_FunctionTypeExtent) if(!traverse(input->m_FunctionTypeExtent)) return false;
                                if (input->m_DerivedExtent) if(!traverse(input->m_DerivedExtent)) return false;
                                return end(input);
                            }
                        }
                    }
                    inline bool visit(Enums* input) { return static_cast<T*>(this)->visitEnums(input); }
                    inline bool end(Enums* input) { return static_cast<T*>(this)->endEnums(input); }
                    inline bool traverse(Enums* input) { return static_cast<T*>(this)->traverseEnums(input); }
                    inline bool visitEnums(Enums* input) { return true; }
                    inline bool endEnums(Enums* input) { return true; }
                    inline bool visit(Enum* input) { return static_cast<T*>(this)->visitEnum(input); }
                    inline bool end(Enum* input) { return static_cast<T*>(this)->endEnum(input); }
                    inline bool traverse(Enum* input) { return static_cast<T*>(this)->traverseEnum(input); }
                    inline bool visitEnum(Enum* input) { return true; }
                    inline bool endEnum(Enum* input) { return true; }
                    bool traverseEnums(Enums* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseEnum(Enum* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_EnumConstants) if(!traverse(input->m_EnumConstants)) return false;
                        return end(input);
                    }
                    inline bool visit(EnumConstants* input) { return static_cast<T*>(this)->visitEnumConstants(input); }
                    inline bool end(EnumConstants* input) { return static_cast<T*>(this)->endEnumConstants(input); }
                    inline bool traverse(EnumConstants* input) { return static_cast<T*>(this)->traverseEnumConstants(input); }
                    inline bool visitEnumConstants(EnumConstants* input) { return true; }
                    inline bool endEnumConstants(EnumConstants* input) { return true; }
                    inline bool visit(EnumConstant* input) { return static_cast<T*>(this)->visitEnumConstant(input); }
                    inline bool end(EnumConstant* input) { return static_cast<T*>(this)->endEnumConstant(input); }
                    inline bool traverse(EnumConstant* input) { return static_cast<T*>(this)->traverseEnumConstant(input); }
                    inline bool visitEnumConstant(EnumConstant* input) { return true; }
                    inline bool endEnumConstant(EnumConstant* input) { return true; }
                    bool traverseEnumConstants(EnumConstants* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseEnumConstant(EnumConstant* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_AssignExpression) if(!traverse(input->m_AssignExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(Usings* input) { return static_cast<T*>(this)->visitUsings(input); }
                    inline bool end(Usings* input) { return static_cast<T*>(this)->endUsings(input); }
                    inline bool traverse(Usings* input) { return static_cast<T*>(this)->traverseUsings(input); }
                    inline bool visitUsings(Usings* input) { return true; }
                    inline bool endUsings(Usings* input) { return true; }
                    inline bool visit(Using* input) { return static_cast<T*>(this)->visitUsing(input); }
                    inline bool end(Using* input) { return static_cast<T*>(this)->endUsing(input); }
                    inline bool traverse(Using* input) { return static_cast<T*>(this)->traverseUsing(input); }
                    inline bool visitUsing(Using* input) { return true; }
                    inline bool endUsing(Using* input) { return true; }
                    bool traverseUsings(Usings* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseUsing(Using* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_QualifiedName) if(!traverse(input->m_QualifiedName)) return false;
                        return end(input);
                    }
                    inline bool visit(Typedefs* input) { return static_cast<T*>(this)->visitTypedefs(input); }
                    inline bool end(Typedefs* input) { return static_cast<T*>(this)->endTypedefs(input); }
                    inline bool traverse(Typedefs* input) { return static_cast<T*>(this)->traverseTypedefs(input); }
                    inline bool visitTypedefs(Typedefs* input) { return true; }
                    inline bool endTypedefs(Typedefs* input) { return true; }
                    inline bool visit(Typedef* input) { return static_cast<T*>(this)->visitTypedef(input); }
                    inline bool end(Typedef* input) { return static_cast<T*>(this)->endTypedef(input); }
                    inline bool traverse(Typedef* input) { return static_cast<T*>(this)->traverseTypedef(input); }
                    inline bool visitTypedef(Typedef* input) { return true; }
                    inline bool endTypedef(Typedef* input) { return true; }
                    bool traverseTypedefs(Typedefs* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTypedef(Typedef* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        return end(input);
                    }
                    inline bool visit(Aliases* input) { return static_cast<T*>(this)->visitAliases(input); }
                    inline bool end(Aliases* input) { return static_cast<T*>(this)->endAliases(input); }
                    inline bool traverse(Aliases* input) { return static_cast<T*>(this)->traverseAliases(input); }
                    inline bool visitAliases(Aliases* input) { return true; }
                    inline bool endAliases(Aliases* input) { return true; }
                    inline bool visit(Alias* input) { return static_cast<T*>(this)->visitAlias(input); }
                    inline bool end(Alias* input) { return static_cast<T*>(this)->endAlias(input); }
                    inline bool traverse(Alias* input) { return static_cast<T*>(this)->traverseAlias(input); }
                    inline bool visitAlias(Alias* input) { return true; }
                    inline bool endAlias(Alias* input) { return true; }
                    bool traverseAliases(Aliases* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAlias(Alias* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        return end(input);
                    }
                    inline bool visit(TemplateParameterDefaults* input) { return static_cast<T*>(this)->visitTemplateParameterDefaults(input); }
                    inline bool end(TemplateParameterDefaults* input) { return static_cast<T*>(this)->endTemplateParameterDefaults(input); }
                    inline bool traverse(TemplateParameterDefaults* input) { return static_cast<T*>(this)->traverseTemplateParameterDefaults(input); }
                    inline bool visitTemplateParameterDefaults(TemplateParameterDefaults* input) { return true; }
                    inline bool endTemplateParameterDefaults(TemplateParameterDefaults* input) { return true; }
                    inline bool visit(TemplateParameterDefault* input) { return static_cast<T*>(this)->visitTemplateParameterDefault(input); }
                    inline bool end(TemplateParameterDefault* input) { return static_cast<T*>(this)->endTemplateParameterDefault(input); }
                    inline bool traverse(TemplateParameterDefault* input) { return static_cast<T*>(this)->traverseTemplateParameterDefault(input); }
                    inline bool visitTemplateParameterDefault(TemplateParameterDefault* input) { return true; }
                    inline bool endTemplateParameterDefault(TemplateParameterDefault* input) { return true; }
                    bool traverseTemplateParameterDefaults(TemplateParameterDefaults* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplateParameterDefault(TemplateParameterDefault* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TemplateArgumentAssign) if(!traverse(input->m_TemplateArgumentAssign)) return false;
                        if (input->m_TemplateArgumentNoAssign) if(!traverse(input->m_TemplateArgumentNoAssign)) return false;
                        return end(input);
                    }
                    inline bool visit(TemplateValueParameters* input) { return static_cast<T*>(this)->visitTemplateValueParameters(input); }
                    inline bool end(TemplateValueParameters* input) { return static_cast<T*>(this)->endTemplateValueParameters(input); }
                    inline bool traverse(TemplateValueParameters* input) { return static_cast<T*>(this)->traverseTemplateValueParameters(input); }
                    inline bool visitTemplateValueParameters(TemplateValueParameters* input) { return true; }
                    inline bool endTemplateValueParameters(TemplateValueParameters* input) { return true; }
                    inline bool visit(TemplateValueParameter* input) { return static_cast<T*>(this)->visitTemplateValueParameter(input); }
                    inline bool end(TemplateValueParameter* input) { return static_cast<T*>(this)->endTemplateValueParameter(input); }
                    inline bool traverse(TemplateValueParameter* input) { return static_cast<T*>(this)->traverseTemplateValueParameter(input); }
                    inline bool visitTemplateValueParameter(TemplateValueParameter* input) { return true; }
                    inline bool endTemplateValueParameter(TemplateValueParameter* input) { return true; }
                    bool traverseTemplateValueParameters(TemplateValueParameters* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplateValueParameter(TemplateValueParameter* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_QualifiedName) if(!traverse(input->m_QualifiedName)) return false;
                        if (input->m_FundamentalType) if(!traverse(input->m_FundamentalType)) return false;
                        return end(input);
                    }
                    inline bool visit(TemplateTypeParameters* input) { return static_cast<T*>(this)->visitTemplateTypeParameters(input); }
                    inline bool end(TemplateTypeParameters* input) { return static_cast<T*>(this)->endTemplateTypeParameters(input); }
                    inline bool traverse(TemplateTypeParameters* input) { return static_cast<T*>(this)->traverseTemplateTypeParameters(input); }
                    inline bool visitTemplateTypeParameters(TemplateTypeParameters* input) { return true; }
                    inline bool endTemplateTypeParameters(TemplateTypeParameters* input) { return true; }
                    inline bool visit(TemplateTypeParameter* input) { return static_cast<T*>(this)->visitTemplateTypeParameter(input); }
                    inline bool end(TemplateTypeParameter* input) { return static_cast<T*>(this)->endTemplateTypeParameter(input); }
                    inline bool traverse(TemplateTypeParameter* input) { return static_cast<T*>(this)->traverseTemplateTypeParameter(input); }
                    inline bool visitTemplateTypeParameter(TemplateTypeParameter* input) { return true; }
                    inline bool endTemplateTypeParameter(TemplateTypeParameter* input) { return true; }
                    bool traverseTemplateTypeParameters(TemplateTypeParameters* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplateTypeParameter(TemplateTypeParameter* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(TemplateParameters* input) { return static_cast<T*>(this)->visitTemplateParameters(input); }
                    inline bool end(TemplateParameters* input) { return static_cast<T*>(this)->endTemplateParameters(input); }
                    inline bool traverse(TemplateParameters* input) { return static_cast<T*>(this)->traverseTemplateParameters(input); }
                    inline bool visitTemplateParameters(TemplateParameters* input) { return true; }
                    inline bool endTemplateParameters(TemplateParameters* input) { return true; }
                    inline bool visit(TemplateParameter* input) { return static_cast<T*>(this)->visitTemplateParameter(input); }
                    inline bool end(TemplateParameter* input) { return static_cast<T*>(this)->endTemplateParameter(input); }
                    inline bool traverse(TemplateParameter* input) { return static_cast<T*>(this)->traverseTemplateParameter(input); }
                    inline bool visitTemplateParameter(TemplateParameter* input) { return true; }
                    inline bool endTemplateParameter(TemplateParameter* input) { return true; }
                    bool traverseTemplateParameters(TemplateParameters* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplateParameter(TemplateParameter* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TemplateValueParameter) if(!traverse(input->m_TemplateValueParameter)) return false;
                        if (input->m_TemplateTypeParameter) if(!traverse(input->m_TemplateTypeParameter)) return false;
                        if (input->m_TemplateParameterDefault) if(!traverse(input->m_TemplateParameterDefault)) return false;
                        return end(input);
                    }
                    inline bool visit(TemplateSignatures* input) { return static_cast<T*>(this)->visitTemplateSignatures(input); }
                    inline bool end(TemplateSignatures* input) { return static_cast<T*>(this)->endTemplateSignatures(input); }
                    inline bool traverse(TemplateSignatures* input) { return static_cast<T*>(this)->traverseTemplateSignatures(input); }
                    inline bool visitTemplateSignatures(TemplateSignatures* input) { return true; }
                    inline bool endTemplateSignatures(TemplateSignatures* input) { return true; }
                    inline bool visit(TemplateSignature* input) { return static_cast<T*>(this)->visitTemplateSignature(input); }
                    inline bool end(TemplateSignature* input) { return static_cast<T*>(this)->endTemplateSignature(input); }
                    inline bool traverse(TemplateSignature* input) { return static_cast<T*>(this)->traverseTemplateSignature(input); }
                    inline bool visitTemplateSignature(TemplateSignature* input) { return true; }
                    inline bool endTemplateSignature(TemplateSignature* input) { return true; }
                    bool traverseTemplateSignatures(TemplateSignatures* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplateSignature(TemplateSignature* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TemplateParameters) if(!traverse(input->m_TemplateParameters)) return false;
                        return end(input);
                    }
                    inline bool visit(Templates* input) { return static_cast<T*>(this)->visitTemplates(input); }
                    inline bool end(Templates* input) { return static_cast<T*>(this)->endTemplates(input); }
                    inline bool traverse(Templates* input) { return static_cast<T*>(this)->traverseTemplates(input); }
                    inline bool visitTemplates(Templates* input) { return true; }
                    inline bool endTemplates(Templates* input) { return true; }
                    inline bool visit(Template* input) { return static_cast<T*>(this)->visitTemplate(input); }
                    inline bool end(Template* input) { return static_cast<T*>(this)->endTemplate(input); }
                    inline bool traverse(Template* input) { return static_cast<T*>(this)->traverseTemplate(input); }
                    inline bool visitTemplate(Template* input) { return true; }
                    inline bool endTemplate(Template* input) { return true; }
                    bool traverseTemplates(Templates* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplate(Template* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TemplateSignature) if(!traverse(input->m_TemplateSignature)) return false;
                        if (input->m_Class) if(!traverse(input->m_Class)) return false;
                        if (input->m_Union) if(!traverse(input->m_Union)) return false;
                        if (input->m_Alias) if(!traverse(input->m_Alias)) return false;
                        if (input->m_TemplateFunction) if(!traverse(input->m_TemplateFunction)) return false;
                        if (input->m_Constructor) if(!traverse(input->m_Constructor)) return false;
                        return end(input);
                    }
                    inline bool visit(TemplateFunctions* input) { return static_cast<T*>(this)->visitTemplateFunctions(input); }
                    inline bool end(TemplateFunctions* input) { return static_cast<T*>(this)->endTemplateFunctions(input); }
                    inline bool traverse(TemplateFunctions* input) { return static_cast<T*>(this)->traverseTemplateFunctions(input); }
                    inline bool visitTemplateFunctions(TemplateFunctions* input) { return true; }
                    inline bool endTemplateFunctions(TemplateFunctions* input) { return true; }
                    inline bool visit(TemplateFunction* input) { return static_cast<T*>(this)->visitTemplateFunction(input); }
                    inline bool end(TemplateFunction* input) { return static_cast<T*>(this)->endTemplateFunction(input); }
                    inline bool traverse(TemplateFunction* input) { return static_cast<T*>(this)->traverseTemplateFunction(input); }
                    inline bool visitTemplateFunction(TemplateFunction* input) { return true; }
                    inline bool endTemplateFunction(TemplateFunction* input) { return true; }
                    bool traverseTemplateFunctions(TemplateFunctions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTemplateFunction(TemplateFunction* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TypeOrAuto) if(!traverse(input->m_TypeOrAuto)) return false;
                        if (input->m_FunctionEnd) if(!traverse(input->m_FunctionEnd)) return false;
                        return end(input);
                    }
                    inline bool visit(FunctionBlocks* input) { return static_cast<T*>(this)->visitFunctionBlocks(input); }
                    inline bool end(FunctionBlocks* input) { return static_cast<T*>(this)->endFunctionBlocks(input); }
                    inline bool traverse(FunctionBlocks* input) { return static_cast<T*>(this)->traverseFunctionBlocks(input); }
                    inline bool visitFunctionBlocks(FunctionBlocks* input) { return true; }
                    inline bool endFunctionBlocks(FunctionBlocks* input) { return true; }
                    inline bool visit(FunctionBlock* input) { return static_cast<T*>(this)->visitFunctionBlock(input); }
                    inline bool end(FunctionBlock* input) { return static_cast<T*>(this)->endFunctionBlock(input); }
                    inline bool traverse(FunctionBlock* input) { return static_cast<T*>(this)->traverseFunctionBlock(input); }
                    inline bool visitFunctionBlock(FunctionBlock* input) { return true; }
                    inline bool endFunctionBlock(FunctionBlock* input) { return true; }
                    bool traverseFunctionBlocks(FunctionBlocks* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseFunctionBlock(FunctionBlock* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Statements) if(!traverse(input->m_Statements)) return false;
                        return end(input);
                    }
                    inline bool visit(QualifiedNames* input) { return static_cast<T*>(this)->visitQualifiedNames(input); }
                    inline bool end(QualifiedNames* input) { return static_cast<T*>(this)->endQualifiedNames(input); }
                    inline bool traverse(QualifiedNames* input) { return static_cast<T*>(this)->traverseQualifiedNames(input); }
                    inline bool visitQualifiedNames(QualifiedNames* input) { return true; }
                    inline bool endQualifiedNames(QualifiedNames* input) { return true; }
                    inline bool visit(QualifiedName* input) { return static_cast<T*>(this)->visitQualifiedName(input); }
                    inline bool end(QualifiedName* input) { return static_cast<T*>(this)->endQualifiedName(input); }
                    inline bool traverse(QualifiedName* input) { return static_cast<T*>(this)->traverseQualifiedName(input); }
                    inline bool visitQualifiedName(QualifiedName* input) { return true; }
                    inline bool endQualifiedName(QualifiedName* input) { return true; }
                    bool traverseQualifiedNames(QualifiedNames* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseQualifiedName(QualifiedName* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Scope) if(!traverse(input->m_Scope)) return false;
                        if (input->m_Dot) if(!traverse(input->m_Dot)) return false;
                        return end(input);
                    }
                    inline bool visit(QualifiedDotNames* input) { return static_cast<T*>(this)->visitQualifiedDotNames(input); }
                    inline bool end(QualifiedDotNames* input) { return static_cast<T*>(this)->endQualifiedDotNames(input); }
                    inline bool traverse(QualifiedDotNames* input) { return static_cast<T*>(this)->traverseQualifiedDotNames(input); }
                    inline bool visitQualifiedDotNames(QualifiedDotNames* input) { return true; }
                    inline bool endQualifiedDotNames(QualifiedDotNames* input) { return true; }
                    inline bool visit(QualifiedDotName* input) { return static_cast<T*>(this)->visitQualifiedDotName(input); }
                    inline bool end(QualifiedDotName* input) { return static_cast<T*>(this)->endQualifiedDotName(input); }
                    inline bool traverse(QualifiedDotName* input) { return static_cast<T*>(this)->traverseQualifiedDotName(input); }
                    inline bool visitQualifiedDotName(QualifiedDotName* input) { return true; }
                    inline bool endQualifiedDotName(QualifiedDotName* input) { return true; }
                    bool traverseQualifiedDotNames(QualifiedDotNames* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseQualifiedDotName(QualifiedDotName* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Names) if(!traverse(input->m_Names)) return false;
                        return end(input);
                    }
                    inline bool visit(QualifiedScopedNames* input) { return static_cast<T*>(this)->visitQualifiedScopedNames(input); }
                    inline bool end(QualifiedScopedNames* input) { return static_cast<T*>(this)->endQualifiedScopedNames(input); }
                    inline bool traverse(QualifiedScopedNames* input) { return static_cast<T*>(this)->traverseQualifiedScopedNames(input); }
                    inline bool visitQualifiedScopedNames(QualifiedScopedNames* input) { return true; }
                    inline bool endQualifiedScopedNames(QualifiedScopedNames* input) { return true; }
                    inline bool visit(QualifiedScopedName* input) { return static_cast<T*>(this)->visitQualifiedScopedName(input); }
                    inline bool end(QualifiedScopedName* input) { return static_cast<T*>(this)->endQualifiedScopedName(input); }
                    inline bool traverse(QualifiedScopedName* input) { return static_cast<T*>(this)->traverseQualifiedScopedName(input); }
                    inline bool visitQualifiedScopedName(QualifiedScopedName* input) { return true; }
                    inline bool endQualifiedScopedName(QualifiedScopedName* input) { return true; }
                    bool traverseQualifiedScopedNames(QualifiedScopedNames* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseQualifiedScopedName(QualifiedScopedName* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Names) if(!traverse(input->m_Names)) return false;
                        if (input->m_Name) if(!traverse(input->m_Name)) return false;
                        return end(input);
                    }
                    inline bool visit(BaseClasses* input) { return static_cast<T*>(this)->visitBaseClasses(input); }
                    inline bool end(BaseClasses* input) { return static_cast<T*>(this)->endBaseClasses(input); }
                    inline bool traverse(BaseClasses* input) { return static_cast<T*>(this)->traverseBaseClasses(input); }
                    inline bool visitBaseClasses(BaseClasses* input) { return true; }
                    inline bool endBaseClasses(BaseClasses* input) { return true; }
                    inline bool visit(BaseClass* input) { return static_cast<T*>(this)->visitBaseClass(input); }
                    inline bool end(BaseClass* input) { return static_cast<T*>(this)->endBaseClass(input); }
                    inline bool traverse(BaseClass* input) { return static_cast<T*>(this)->traverseBaseClass(input); }
                    inline bool visitBaseClass(BaseClass* input) { return true; }
                    inline bool endBaseClass(BaseClass* input) { return true; }
                    bool traverseBaseClasses(BaseClasses* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBaseClass(BaseClass* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_BasicType) if(!traverse(input->m_BasicType)) return false;
                        return end(input);
                    }
                    inline bool visit(Classes* input) { return static_cast<T*>(this)->visitClasses(input); }
                    inline bool end(Classes* input) { return static_cast<T*>(this)->endClasses(input); }
                    inline bool traverse(Classes* input) { return static_cast<T*>(this)->traverseClasses(input); }
                    inline bool visitClasses(Classes* input) { return true; }
                    inline bool endClasses(Classes* input) { return true; }
                    inline bool visit(Class* input) { return static_cast<T*>(this)->visitClass(input); }
                    inline bool end(Class* input) { return static_cast<T*>(this)->endClass(input); }
                    inline bool traverse(Class* input) { return static_cast<T*>(this)->traverseClass(input); }
                    inline bool visitClass(Class* input) { return true; }
                    inline bool endClass(Class* input) { return true; }
                    bool traverseClasses(Classes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseClass(Class* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_MetaClassConstructionExpression) if(!traverse(input->m_MetaClassConstructionExpression)) return false;
                        if (input->m_TemplateArgumentList) if(!traverse(input->m_TemplateArgumentList)) return false;
                        if (input->m_BaseClasses) if(!traverse(input->m_BaseClasses)) return false;
                        if (input->m_Members) if(!traverse(input->m_Members)) return false;
                        return end(input);
                    }
                    inline bool visit(Unions* input) { return static_cast<T*>(this)->visitUnions(input); }
                    inline bool end(Unions* input) { return static_cast<T*>(this)->endUnions(input); }
                    inline bool traverse(Unions* input) { return static_cast<T*>(this)->traverseUnions(input); }
                    inline bool visitUnions(Unions* input) { return true; }
                    inline bool endUnions(Unions* input) { return true; }
                    inline bool visit(Union* input) { return static_cast<T*>(this)->visitUnion(input); }
                    inline bool end(Union* input) { return static_cast<T*>(this)->endUnion(input); }
                    inline bool traverse(Union* input) { return static_cast<T*>(this)->traverseUnion(input); }
                    inline bool visitUnion(Union* input) { return true; }
                    inline bool endUnion(Union* input) { return true; }
                    bool traverseUnions(Unions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseUnion(Union* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TemplateArgumentList) if(!traverse(input->m_TemplateArgumentList)) return false;
                        if (input->m_BasicMembers) if(!traverse(input->m_BasicMembers)) return false;
                        return end(input);
                    }
                    inline bool visit(Methods* input) { return static_cast<T*>(this)->visitMethods(input); }
                    inline bool end(Methods* input) { return static_cast<T*>(this)->endMethods(input); }
                    inline bool traverse(Methods* input) { return static_cast<T*>(this)->traverseMethods(input); }
                    inline bool visitMethods(Methods* input) { return true; }
                    inline bool endMethods(Methods* input) { return true; }
                    inline bool visit(Method* input) { return static_cast<T*>(this)->visitMethod(input); }
                    inline bool end(Method* input) { return static_cast<T*>(this)->endMethod(input); }
                    inline bool traverse(Method* input) { return static_cast<T*>(this)->traverseMethod(input); }
                    inline bool visitMethod(Method* input) { return true; }
                    inline bool endMethod(Method* input) { return true; }
                    bool traverseMethods(Methods* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseMethod(Method* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_MethodSpecifiers) if(!traverse(input->m_MethodSpecifiers)) return false;
                        if (input->m_TypeOrAuto) if(!traverse(input->m_TypeOrAuto)) return false;
                        if (input->m_FunctionEnd) if(!traverse(input->m_FunctionEnd)) return false;
                        return end(input);
                    }
                    inline bool visit(Parameters* input) { return static_cast<T*>(this)->visitParameters(input); }
                    inline bool end(Parameters* input) { return static_cast<T*>(this)->endParameters(input); }
                    inline bool traverse(Parameters* input) { return static_cast<T*>(this)->traverseParameters(input); }
                    inline bool visitParameters(Parameters* input) { return true; }
                    inline bool endParameters(Parameters* input) { return true; }
                    inline bool visit(Parameter* input) { return static_cast<T*>(this)->visitParameter(input); }
                    inline bool end(Parameter* input) { return static_cast<T*>(this)->endParameter(input); }
                    inline bool traverse(Parameter* input) { return static_cast<T*>(this)->traverseParameter(input); }
                    inline bool visitParameter(Parameter* input) { return true; }
                    inline bool endParameter(Parameter* input) { return true; }
                    bool traverseParameters(Parameters* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseParameter(Parameter* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        if (input->m_ArrayExtents) if(!traverse(input->m_ArrayExtents)) return false;
                        if (input->m_AssignExpressionOrInitializerList) if(!traverse(input->m_AssignExpressionOrInitializerList)) return false;
                        return end(input);
                    }
                    inline bool visit(AccessSpecifiers* input) { return static_cast<T*>(this)->visitAccessSpecifiers(input); }
                    inline bool end(AccessSpecifiers* input) { return static_cast<T*>(this)->endAccessSpecifiers(input); }
                    inline bool traverse(AccessSpecifiers* input) { return static_cast<T*>(this)->traverseAccessSpecifiers(input); }
                    inline bool visitAccessSpecifiers(AccessSpecifiers* input) { return true; }
                    inline bool endAccessSpecifiers(AccessSpecifiers* input) { return true; }
                    inline bool visit(AccessSpecifier* input) { return static_cast<T*>(this)->visitAccessSpecifier(input); }
                    inline bool end(AccessSpecifier* input) { return static_cast<T*>(this)->endAccessSpecifier(input); }
                    inline bool traverse(AccessSpecifier* input) { return static_cast<T*>(this)->traverseAccessSpecifier(input); }
                    inline bool visitAccessSpecifier(AccessSpecifier* input) { return true; }
                    inline bool endAccessSpecifier(AccessSpecifier* input) { return true; }
                    bool traverseAccessSpecifiers(AccessSpecifiers* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAccessSpecifier(AccessSpecifier* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(FieldOverrides* input) { return static_cast<T*>(this)->visitFieldOverrides(input); }
                    inline bool end(FieldOverrides* input) { return static_cast<T*>(this)->endFieldOverrides(input); }
                    inline bool traverse(FieldOverrides* input) { return static_cast<T*>(this)->traverseFieldOverrides(input); }
                    inline bool visitFieldOverrides(FieldOverrides* input) { return true; }
                    inline bool endFieldOverrides(FieldOverrides* input) { return true; }
                    inline bool visit(FieldOverride* input) { return static_cast<T*>(this)->visitFieldOverride(input); }
                    inline bool end(FieldOverride* input) { return static_cast<T*>(this)->endFieldOverride(input); }
                    inline bool traverse(FieldOverride* input) { return static_cast<T*>(this)->traverseFieldOverride(input); }
                    inline bool visitFieldOverride(FieldOverride* input) { return true; }
                    inline bool endFieldOverride(FieldOverride* input) { return true; }
                    bool traverseFieldOverrides(FieldOverrides* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseFieldOverride(FieldOverride* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_AssignExpression) if(!traverse(input->m_AssignExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(Symbols* input) { return static_cast<T*>(this)->visitSymbols(input); }
                    inline bool end(Symbols* input) { return static_cast<T*>(this)->endSymbols(input); }
                    inline bool traverse(Symbols* input) { return static_cast<T*>(this)->traverseSymbols(input); }
                    inline bool visitSymbols(Symbols* input) { return true; }
                    inline bool endSymbols(Symbols* input) { return true; }
                    inline bool visit(Symbol* input) { return static_cast<T*>(this)->visitSymbol(input); }
                    inline bool end(Symbol* input) { return static_cast<T*>(this)->endSymbol(input); }
                    inline bool traverse(Symbol* input) { return static_cast<T*>(this)->traverseSymbol(input); }
                    inline bool visitSymbol(Symbol* input) { return true; }
                    inline bool endSymbol(Symbol* input) { return true; }
                    bool traverseSymbols(Symbols* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseSymbol(Symbol* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_ExplicitType) if(!traverse(input->m_ExplicitType)) return false;
                        if (input->m_PostFixExpression) if(!traverse(input->m_PostFixExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(Members* input) { return static_cast<T*>(this)->visitMembers(input); }
                    inline bool end(Members* input) { return static_cast<T*>(this)->endMembers(input); }
                    inline bool traverse(Members* input) { return static_cast<T*>(this)->traverseMembers(input); }
                    inline bool visitMembers(Members* input) { return true; }
                    inline bool endMembers(Members* input) { return true; }
                    inline bool visit(Member* input) { return static_cast<T*>(this)->visitMember(input); }
                    inline bool end(Member* input) { return static_cast<T*>(this)->endMember(input); }
                    inline bool traverse(Member* input) { return static_cast<T*>(this)->traverseMember(input); }
                    inline bool visitMember(Member* input) { return true; }
                    inline bool endMember(Member* input) { return true; }
                    bool traverseMembers(Members* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseMember(Member* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_MemberWithMetaOrAnnotation: return traverse(static_cast<MemberWithMetaOrAnnotation*>(input));
                            case e_RuleType_BasicMemberWithMetaOrAnnotation: return traverse(static_cast<BasicMemberWithMetaOrAnnotation*>(input));
                            case e_RuleType_Friend: return traverse(static_cast<Friend*>(input));
                            case e_RuleType_BasicMember: return traverse(static_cast<BasicMember*>(input));
                            default:
                            {
                                if (!visit(input)) return false;
                                if (input->m_MemberWithMetaOrAnnotation) if(!traverse(input->m_MemberWithMetaOrAnnotation)) return false;
                                if (input->m_BasicMember) if(!traverse(input->m_BasicMember)) return false;
                                return end(input);
                            }
                        }
                    }
                    inline bool visit(IfOrWhileLocalVariableDecls* input) { return static_cast<T*>(this)->visitIfOrWhileLocalVariableDecls(input); }
                    inline bool end(IfOrWhileLocalVariableDecls* input) { return static_cast<T*>(this)->endIfOrWhileLocalVariableDecls(input); }
                    inline bool traverse(IfOrWhileLocalVariableDecls* input) { return static_cast<T*>(this)->traverseIfOrWhileLocalVariableDecls(input); }
                    inline bool visitIfOrWhileLocalVariableDecls(IfOrWhileLocalVariableDecls* input) { return true; }
                    inline bool endIfOrWhileLocalVariableDecls(IfOrWhileLocalVariableDecls* input) { return true; }
                    inline bool visit(IfOrWhileLocalVariableDecl* input) { return static_cast<T*>(this)->visitIfOrWhileLocalVariableDecl(input); }
                    inline bool end(IfOrWhileLocalVariableDecl* input) { return static_cast<T*>(this)->endIfOrWhileLocalVariableDecl(input); }
                    inline bool traverse(IfOrWhileLocalVariableDecl* input) { return static_cast<T*>(this)->traverseIfOrWhileLocalVariableDecl(input); }
                    inline bool visitIfOrWhileLocalVariableDecl(IfOrWhileLocalVariableDecl* input) { return true; }
                    inline bool endIfOrWhileLocalVariableDecl(IfOrWhileLocalVariableDecl* input) { return true; }
                    bool traverseIfOrWhileLocalVariableDecls(IfOrWhileLocalVariableDecls* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseIfOrWhileLocalVariableDecl(IfOrWhileLocalVariableDecl* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TypeOrAuto) if(!traverse(input->m_TypeOrAuto)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(IfOrWhileConditions* input) { return static_cast<T*>(this)->visitIfOrWhileConditions(input); }
                    inline bool end(IfOrWhileConditions* input) { return static_cast<T*>(this)->endIfOrWhileConditions(input); }
                    inline bool traverse(IfOrWhileConditions* input) { return static_cast<T*>(this)->traverseIfOrWhileConditions(input); }
                    inline bool visitIfOrWhileConditions(IfOrWhileConditions* input) { return true; }
                    inline bool endIfOrWhileConditions(IfOrWhileConditions* input) { return true; }
                    inline bool visit(IfOrWhileCondition* input) { return static_cast<T*>(this)->visitIfOrWhileCondition(input); }
                    inline bool end(IfOrWhileCondition* input) { return static_cast<T*>(this)->endIfOrWhileCondition(input); }
                    inline bool traverse(IfOrWhileCondition* input) { return static_cast<T*>(this)->traverseIfOrWhileCondition(input); }
                    inline bool visitIfOrWhileCondition(IfOrWhileCondition* input) { return true; }
                    inline bool endIfOrWhileCondition(IfOrWhileCondition* input) { return true; }
                    bool traverseIfOrWhileConditions(IfOrWhileConditions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseIfOrWhileCondition(IfOrWhileCondition* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_IfOrWhileLocalVariableDecl) if(!traverse(input->m_IfOrWhileLocalVariableDecl)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(BlockDeclarations* input) { return static_cast<T*>(this)->visitBlockDeclarations(input); }
                    inline bool end(BlockDeclarations* input) { return static_cast<T*>(this)->endBlockDeclarations(input); }
                    inline bool traverse(BlockDeclarations* input) { return static_cast<T*>(this)->traverseBlockDeclarations(input); }
                    inline bool visitBlockDeclarations(BlockDeclarations* input) { return true; }
                    inline bool endBlockDeclarations(BlockDeclarations* input) { return true; }
                    inline bool visit(BlockDeclaration* input) { return static_cast<T*>(this)->visitBlockDeclaration(input); }
                    inline bool end(BlockDeclaration* input) { return static_cast<T*>(this)->endBlockDeclaration(input); }
                    inline bool traverse(BlockDeclaration* input) { return static_cast<T*>(this)->traverseBlockDeclaration(input); }
                    inline bool visitBlockDeclaration(BlockDeclaration* input) { return true; }
                    inline bool endBlockDeclaration(BlockDeclaration* input) { return true; }
                    bool traverseBlockDeclarations(BlockDeclarations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBlockDeclaration(BlockDeclaration* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Class) if(!traverse(input->m_Class)) return false;
                        if (input->m_Enum) if(!traverse(input->m_Enum)) return false;
                        if (input->m_Typedef) if(!traverse(input->m_Typedef)) return false;
                        if (input->m_Alias) if(!traverse(input->m_Alias)) return false;
                        if (input->m_Using) if(!traverse(input->m_Using)) return false;
                        if (input->m_StaticAssert) if(!traverse(input->m_StaticAssert)) return false;
                        if (input->m_Alias0) if(!traverse(input->m_Alias0)) return false;
                        return end(input);
                    }
                    inline bool visit(FundamentalTypeFunctionCasts* input) { return static_cast<T*>(this)->visitFundamentalTypeFunctionCasts(input); }
                    inline bool end(FundamentalTypeFunctionCasts* input) { return static_cast<T*>(this)->endFundamentalTypeFunctionCasts(input); }
                    inline bool traverse(FundamentalTypeFunctionCasts* input) { return static_cast<T*>(this)->traverseFundamentalTypeFunctionCasts(input); }
                    inline bool visitFundamentalTypeFunctionCasts(FundamentalTypeFunctionCasts* input) { return true; }
                    inline bool endFundamentalTypeFunctionCasts(FundamentalTypeFunctionCasts* input) { return true; }
                    inline bool visit(FundamentalTypeFunctionCast* input) { return static_cast<T*>(this)->visitFundamentalTypeFunctionCast(input); }
                    inline bool end(FundamentalTypeFunctionCast* input) { return static_cast<T*>(this)->endFundamentalTypeFunctionCast(input); }
                    inline bool traverse(FundamentalTypeFunctionCast* input) { return static_cast<T*>(this)->traverseFundamentalTypeFunctionCast(input); }
                    inline bool visitFundamentalTypeFunctionCast(FundamentalTypeFunctionCast* input) { return true; }
                    inline bool endFundamentalTypeFunctionCast(FundamentalTypeFunctionCast* input) { return true; }
                    bool traverseFundamentalTypeFunctionCasts(FundamentalTypeFunctionCasts* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseFundamentalTypeFunctionCast(FundamentalTypeFunctionCast* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_FundamentalType) if(!traverse(input->m_FundamentalType)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(ExpressionStatements* input) { return static_cast<T*>(this)->visitExpressionStatements(input); }
                    inline bool end(ExpressionStatements* input) { return static_cast<T*>(this)->endExpressionStatements(input); }
                    inline bool traverse(ExpressionStatements* input) { return static_cast<T*>(this)->traverseExpressionStatements(input); }
                    inline bool visitExpressionStatements(ExpressionStatements* input) { return true; }
                    inline bool endExpressionStatements(ExpressionStatements* input) { return true; }
                    inline bool visit(ExpressionStatement* input) { return static_cast<T*>(this)->visitExpressionStatement(input); }
                    inline bool end(ExpressionStatement* input) { return static_cast<T*>(this)->endExpressionStatement(input); }
                    inline bool traverse(ExpressionStatement* input) { return static_cast<T*>(this)->traverseExpressionStatement(input); }
                    inline bool visitExpressionStatement(ExpressionStatement* input) { return true; }
                    inline bool endExpressionStatement(ExpressionStatement* input) { return true; }
                    bool traverseExpressionStatements(ExpressionStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseExpressionStatement(ExpressionStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(LastTemplateArguments* input) { return static_cast<T*>(this)->visitLastTemplateArguments(input); }
                    inline bool end(LastTemplateArguments* input) { return static_cast<T*>(this)->endLastTemplateArguments(input); }
                    inline bool traverse(LastTemplateArguments* input) { return static_cast<T*>(this)->traverseLastTemplateArguments(input); }
                    inline bool visitLastTemplateArguments(LastTemplateArguments* input) { return true; }
                    inline bool endLastTemplateArguments(LastTemplateArguments* input) { return true; }
                    inline bool visit(LastTemplateArgument* input) { return static_cast<T*>(this)->visitLastTemplateArgument(input); }
                    inline bool end(LastTemplateArgument* input) { return static_cast<T*>(this)->endLastTemplateArgument(input); }
                    inline bool traverse(LastTemplateArgument* input) { return static_cast<T*>(this)->traverseLastTemplateArgument(input); }
                    inline bool visitLastTemplateArgument(LastTemplateArgument* input) { return true; }
                    inline bool endLastTemplateArgument(LastTemplateArgument* input) { return true; }
                    bool traverseLastTemplateArguments(LastTemplateArguments* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLastTemplateArgument(LastTemplateArgument* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_LastTemplateArgumentAssign: return traverse(static_cast<LastTemplateArgumentAssign*>(input));
                            case e_RuleType_LastTemplateArgumentNoAssign: return traverse(static_cast<LastTemplateArgumentNoAssign*>(input));
                            default:
                            {
                                if (!visit(input)) return false;
                                if (input->m_LastTemplateArgumentAssign) if(!traverse(input->m_LastTemplateArgumentAssign)) return false;
                                if (input->m_LastTemplateArgumentNoAssign) if(!traverse(input->m_LastTemplateArgumentNoAssign)) return false;
                                return end(input);
                            }
                        }
                    }
                    inline bool visit(SwitchStatements* input) { return static_cast<T*>(this)->visitSwitchStatements(input); }
                    inline bool end(SwitchStatements* input) { return static_cast<T*>(this)->endSwitchStatements(input); }
                    inline bool traverse(SwitchStatements* input) { return static_cast<T*>(this)->traverseSwitchStatements(input); }
                    inline bool visitSwitchStatements(SwitchStatements* input) { return true; }
                    inline bool endSwitchStatements(SwitchStatements* input) { return true; }
                    inline bool visit(SwitchStatement* input) { return static_cast<T*>(this)->visitSwitchStatement(input); }
                    inline bool end(SwitchStatement* input) { return static_cast<T*>(this)->endSwitchStatement(input); }
                    inline bool traverse(SwitchStatement* input) { return static_cast<T*>(this)->traverseSwitchStatement(input); }
                    inline bool visitSwitchStatement(SwitchStatement* input) { return true; }
                    inline bool endSwitchStatement(SwitchStatement* input) { return true; }
                    bool traverseSwitchStatements(SwitchStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseSwitchStatement(SwitchStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        if (input->m_StatementOrCases) if(!traverse(input->m_StatementOrCases)) return false;
                        return end(input);
                    }
                    inline bool visit(PrimaryExpressions* input) { return static_cast<T*>(this)->visitPrimaryExpressions(input); }
                    inline bool end(PrimaryExpressions* input) { return static_cast<T*>(this)->endPrimaryExpressions(input); }
                    inline bool traverse(PrimaryExpressions* input) { return static_cast<T*>(this)->traversePrimaryExpressions(input); }
                    inline bool visitPrimaryExpressions(PrimaryExpressions* input) { return true; }
                    inline bool endPrimaryExpressions(PrimaryExpressions* input) { return true; }
                    inline bool visit(PrimaryExpression* input) { return static_cast<T*>(this)->visitPrimaryExpression(input); }
                    inline bool end(PrimaryExpression* input) { return static_cast<T*>(this)->endPrimaryExpression(input); }
                    inline bool traverse(PrimaryExpression* input) { return static_cast<T*>(this)->traversePrimaryExpression(input); }
                    inline bool visitPrimaryExpression(PrimaryExpression* input) { return true; }
                    inline bool endPrimaryExpression(PrimaryExpression* input) { return true; }
                    bool traversePrimaryExpressions(PrimaryExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traversePrimaryExpression(PrimaryExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_KeywordExpression) if(!traverse(input->m_KeywordExpression)) return false;
                        if (input->m_SizeofExpression) if(!traverse(input->m_SizeofExpression)) return false;
                        if (input->m_TypeofExpression) if(!traverse(input->m_TypeofExpression)) return false;
                        if (input->m_NullptrExpression) if(!traverse(input->m_NullptrExpression)) return false;
                        if (input->m_Name) if(!traverse(input->m_Name)) return false;
                        if (input->m_CStyleCastExpression) if(!traverse(input->m_CStyleCastExpression)) return false;
                        if (input->m_TypeTraitBinary) if(!traverse(input->m_TypeTraitBinary)) return false;
                        if (input->m_StringLiteralChain) if(!traverse(input->m_StringLiteralChain)) return false;
                        if (input->m_SpecialCastExpression) if(!traverse(input->m_SpecialCastExpression)) return false;
                        if (input->m_FunctionPtrExpression) if(!traverse(input->m_FunctionPtrExpression)) return false;
                        if (input->m_FundamentalTypeFunctionCast) if(!traverse(input->m_FundamentalTypeFunctionCast)) return false;
                        if (input->m_LambdaExpression) if(!traverse(input->m_LambdaExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(NotLastTemplateArguments* input) { return static_cast<T*>(this)->visitNotLastTemplateArguments(input); }
                    inline bool end(NotLastTemplateArguments* input) { return static_cast<T*>(this)->endNotLastTemplateArguments(input); }
                    inline bool traverse(NotLastTemplateArguments* input) { return static_cast<T*>(this)->traverseNotLastTemplateArguments(input); }
                    inline bool visitNotLastTemplateArguments(NotLastTemplateArguments* input) { return true; }
                    inline bool endNotLastTemplateArguments(NotLastTemplateArguments* input) { return true; }
                    inline bool visit(NotLastTemplateArgument* input) { return static_cast<T*>(this)->visitNotLastTemplateArgument(input); }
                    inline bool end(NotLastTemplateArgument* input) { return static_cast<T*>(this)->endNotLastTemplateArgument(input); }
                    inline bool traverse(NotLastTemplateArgument* input) { return static_cast<T*>(this)->traverseNotLastTemplateArgument(input); }
                    inline bool visitNotLastTemplateArgument(NotLastTemplateArgument* input) { return true; }
                    inline bool endNotLastTemplateArgument(NotLastTemplateArgument* input) { return true; }
                    bool traverseNotLastTemplateArguments(NotLastTemplateArguments* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseNotLastTemplateArgument(NotLastTemplateArgument* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_NotLastTemplateArgumentAssign: return traverse(static_cast<NotLastTemplateArgumentAssign*>(input));
                            case e_RuleType_NotLastTemplateArgumentNoAssign: return traverse(static_cast<NotLastTemplateArgumentNoAssign*>(input));
                            default:
                            {
                                if (!visit(input)) return false;
                                if (input->m_NotLastTemplateArgumentAssign) if(!traverse(input->m_NotLastTemplateArgumentAssign)) return false;
                                if (input->m_NotLastTemplateArgumentNoAssign) if(!traverse(input->m_NotLastTemplateArgumentNoAssign)) return false;
                                return end(input);
                            }
                        }
                    }
                    inline bool visit(UnsignedTypes* input) { return static_cast<T*>(this)->visitUnsignedTypes(input); }
                    inline bool end(UnsignedTypes* input) { return static_cast<T*>(this)->endUnsignedTypes(input); }
                    inline bool traverse(UnsignedTypes* input) { return static_cast<T*>(this)->traverseUnsignedTypes(input); }
                    inline bool visitUnsignedTypes(UnsignedTypes* input) { return true; }
                    inline bool endUnsignedTypes(UnsignedTypes* input) { return true; }
                    inline bool visit(UnsignedType* input) { return static_cast<T*>(this)->visitUnsignedType(input); }
                    inline bool end(UnsignedType* input) { return static_cast<T*>(this)->endUnsignedType(input); }
                    inline bool traverse(UnsignedType* input) { return static_cast<T*>(this)->traverseUnsignedType(input); }
                    inline bool visitUnsignedType(UnsignedType* input) { return true; }
                    inline bool endUnsignedType(UnsignedType* input) { return true; }
                    bool traverseUnsignedTypes(UnsignedTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseUnsignedType(UnsignedType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        return end(input);
                    }
                    inline bool visit(SignedTypes* input) { return static_cast<T*>(this)->visitSignedTypes(input); }
                    inline bool end(SignedTypes* input) { return static_cast<T*>(this)->endSignedTypes(input); }
                    inline bool traverse(SignedTypes* input) { return static_cast<T*>(this)->traverseSignedTypes(input); }
                    inline bool visitSignedTypes(SignedTypes* input) { return true; }
                    inline bool endSignedTypes(SignedTypes* input) { return true; }
                    inline bool visit(SignedType* input) { return static_cast<T*>(this)->visitSignedType(input); }
                    inline bool end(SignedType* input) { return static_cast<T*>(this)->endSignedType(input); }
                    inline bool traverse(SignedType* input) { return static_cast<T*>(this)->traverseSignedType(input); }
                    inline bool visitSignedType(SignedType* input) { return true; }
                    inline bool endSignedType(SignedType* input) { return true; }
                    bool traverseSignedTypes(SignedTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseSignedType(SignedType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        return end(input);
                    }
                    inline bool visit(LongTypes* input) { return static_cast<T*>(this)->visitLongTypes(input); }
                    inline bool end(LongTypes* input) { return static_cast<T*>(this)->endLongTypes(input); }
                    inline bool traverse(LongTypes* input) { return static_cast<T*>(this)->traverseLongTypes(input); }
                    inline bool visitLongTypes(LongTypes* input) { return true; }
                    inline bool endLongTypes(LongTypes* input) { return true; }
                    inline bool visit(LongType* input) { return static_cast<T*>(this)->visitLongType(input); }
                    inline bool end(LongType* input) { return static_cast<T*>(this)->endLongType(input); }
                    inline bool traverse(LongType* input) { return static_cast<T*>(this)->traverseLongType(input); }
                    inline bool visitLongType(LongType* input) { return true; }
                    inline bool endLongType(LongType* input) { return true; }
                    bool traverseLongTypes(LongTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLongType(LongType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        return end(input);
                    }
                    inline bool visit(VoidTypes* input) { return static_cast<T*>(this)->visitVoidTypes(input); }
                    inline bool end(VoidTypes* input) { return static_cast<T*>(this)->endVoidTypes(input); }
                    inline bool traverse(VoidTypes* input) { return static_cast<T*>(this)->traverseVoidTypes(input); }
                    inline bool visitVoidTypes(VoidTypes* input) { return true; }
                    inline bool endVoidTypes(VoidTypes* input) { return true; }
                    inline bool visit(VoidType* input) { return static_cast<T*>(this)->visitVoidType(input); }
                    inline bool end(VoidType* input) { return static_cast<T*>(this)->endVoidType(input); }
                    inline bool traverse(VoidType* input) { return static_cast<T*>(this)->traverseVoidType(input); }
                    inline bool visitVoidType(VoidType* input) { return true; }
                    inline bool endVoidType(VoidType* input) { return true; }
                    bool traverseVoidTypes(VoidTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseVoidType(VoidType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(FundamentalTypes* input) { return static_cast<T*>(this)->visitFundamentalTypes(input); }
                    inline bool end(FundamentalTypes* input) { return static_cast<T*>(this)->endFundamentalTypes(input); }
                    inline bool traverse(FundamentalTypes* input) { return static_cast<T*>(this)->traverseFundamentalTypes(input); }
                    inline bool visitFundamentalTypes(FundamentalTypes* input) { return true; }
                    inline bool endFundamentalTypes(FundamentalTypes* input) { return true; }
                    inline bool visit(FundamentalType* input) { return static_cast<T*>(this)->visitFundamentalType(input); }
                    inline bool end(FundamentalType* input) { return static_cast<T*>(this)->endFundamentalType(input); }
                    inline bool traverse(FundamentalType* input) { return static_cast<T*>(this)->traverseFundamentalType(input); }
                    inline bool visitFundamentalType(FundamentalType* input) { return true; }
                    inline bool endFundamentalType(FundamentalType* input) { return true; }
                    bool traverseFundamentalTypes(FundamentalTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseFundamentalType(FundamentalType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_UnsignedType) if(!traverse(input->m_UnsignedType)) return false;
                        if (input->m_SignedType) if(!traverse(input->m_SignedType)) return false;
                        if (input->m_LongType) if(!traverse(input->m_LongType)) return false;
                        if (input->m_VoidType) if(!traverse(input->m_VoidType)) return false;
                        if (input->m_FundamentalTypeBase) if(!traverse(input->m_FundamentalTypeBase)) return false;
                        return end(input);
                    }
                    inline bool visit(QualifiedTypes* input) { return static_cast<T*>(this)->visitQualifiedTypes(input); }
                    inline bool end(QualifiedTypes* input) { return static_cast<T*>(this)->endQualifiedTypes(input); }
                    inline bool traverse(QualifiedTypes* input) { return static_cast<T*>(this)->traverseQualifiedTypes(input); }
                    inline bool visitQualifiedTypes(QualifiedTypes* input) { return true; }
                    inline bool endQualifiedTypes(QualifiedTypes* input) { return true; }
                    inline bool visit(QualifiedType* input) { return static_cast<T*>(this)->visitQualifiedType(input); }
                    inline bool end(QualifiedType* input) { return static_cast<T*>(this)->endQualifiedType(input); }
                    inline bool traverse(QualifiedType* input) { return static_cast<T*>(this)->traverseQualifiedType(input); }
                    inline bool visitQualifiedType(QualifiedType* input) { return true; }
                    inline bool endQualifiedType(QualifiedType* input) { return true; }
                    bool traverseQualifiedTypes(QualifiedTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseQualifiedType(QualifiedType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_QualifiedName) if(!traverse(input->m_QualifiedName)) return false;
                        return end(input);
                    }
                    inline bool visit(ExplicitTypes* input) { return static_cast<T*>(this)->visitExplicitTypes(input); }
                    inline bool end(ExplicitTypes* input) { return static_cast<T*>(this)->endExplicitTypes(input); }
                    inline bool traverse(ExplicitTypes* input) { return static_cast<T*>(this)->traverseExplicitTypes(input); }
                    inline bool visitExplicitTypes(ExplicitTypes* input) { return true; }
                    inline bool endExplicitTypes(ExplicitTypes* input) { return true; }
                    inline bool visit(ExplicitType* input) { return static_cast<T*>(this)->visitExplicitType(input); }
                    inline bool end(ExplicitType* input) { return static_cast<T*>(this)->endExplicitType(input); }
                    inline bool traverse(ExplicitType* input) { return static_cast<T*>(this)->traverseExplicitType(input); }
                    inline bool visitExplicitType(ExplicitType* input) { return true; }
                    inline bool endExplicitType(ExplicitType* input) { return true; }
                    bool traverseExplicitTypes(ExplicitTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseExplicitType(ExplicitType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_BasicType) if(!traverse(input->m_BasicType)) return false;
                        if (input->m_TypeExtents) if(!traverse(input->m_TypeExtents)) return false;
                        return end(input);
                    }
                    inline bool visit(TypeNoFunctionTypes* input) { return static_cast<T*>(this)->visitTypeNoFunctionTypes(input); }
                    inline bool end(TypeNoFunctionTypes* input) { return static_cast<T*>(this)->endTypeNoFunctionTypes(input); }
                    inline bool traverse(TypeNoFunctionTypes* input) { return static_cast<T*>(this)->traverseTypeNoFunctionTypes(input); }
                    inline bool visitTypeNoFunctionTypes(TypeNoFunctionTypes* input) { return true; }
                    inline bool endTypeNoFunctionTypes(TypeNoFunctionTypes* input) { return true; }
                    inline bool visit(TypeNoFunctionType* input) { return static_cast<T*>(this)->visitTypeNoFunctionType(input); }
                    inline bool end(TypeNoFunctionType* input) { return static_cast<T*>(this)->endTypeNoFunctionType(input); }
                    inline bool traverse(TypeNoFunctionType* input) { return static_cast<T*>(this)->traverseTypeNoFunctionType(input); }
                    inline bool visitTypeNoFunctionType(TypeNoFunctionType* input) { return true; }
                    inline bool endTypeNoFunctionType(TypeNoFunctionType* input) { return true; }
                    bool traverseTypeNoFunctionTypes(TypeNoFunctionTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTypeNoFunctionType(TypeNoFunctionType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_ExplicitTypeNoFunctionType) if(!traverse(input->m_ExplicitTypeNoFunctionType)) return false;
                        if (input->m_ExpressionType) if(!traverse(input->m_ExpressionType)) return false;
                        return end(input);
                    }
                    inline bool visit(ExplicitTypeNoFunctionTypes* input) { return static_cast<T*>(this)->visitExplicitTypeNoFunctionTypes(input); }
                    inline bool end(ExplicitTypeNoFunctionTypes* input) { return static_cast<T*>(this)->endExplicitTypeNoFunctionTypes(input); }
                    inline bool traverse(ExplicitTypeNoFunctionTypes* input) { return static_cast<T*>(this)->traverseExplicitTypeNoFunctionTypes(input); }
                    inline bool visitExplicitTypeNoFunctionTypes(ExplicitTypeNoFunctionTypes* input) { return true; }
                    inline bool endExplicitTypeNoFunctionTypes(ExplicitTypeNoFunctionTypes* input) { return true; }
                    inline bool visit(ExplicitTypeNoFunctionType* input) { return static_cast<T*>(this)->visitExplicitTypeNoFunctionType(input); }
                    inline bool end(ExplicitTypeNoFunctionType* input) { return static_cast<T*>(this)->endExplicitTypeNoFunctionType(input); }
                    inline bool traverse(ExplicitTypeNoFunctionType* input) { return static_cast<T*>(this)->traverseExplicitTypeNoFunctionType(input); }
                    inline bool visitExplicitTypeNoFunctionType(ExplicitTypeNoFunctionType* input) { return true; }
                    inline bool endExplicitTypeNoFunctionType(ExplicitTypeNoFunctionType* input) { return true; }
                    bool traverseExplicitTypeNoFunctionTypes(ExplicitTypeNoFunctionTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseExplicitTypeNoFunctionType(ExplicitTypeNoFunctionType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_BasicType) if(!traverse(input->m_BasicType)) return false;
                        if (input->m_TypeExtentNoFunctions) if(!traverse(input->m_TypeExtentNoFunctions)) return false;
                        return end(input);
                    }
                    inline bool visit(ExpressionTypes* input) { return static_cast<T*>(this)->visitExpressionTypes(input); }
                    inline bool end(ExpressionTypes* input) { return static_cast<T*>(this)->endExpressionTypes(input); }
                    inline bool traverse(ExpressionTypes* input) { return static_cast<T*>(this)->traverseExpressionTypes(input); }
                    inline bool visitExpressionTypes(ExpressionTypes* input) { return true; }
                    inline bool endExpressionTypes(ExpressionTypes* input) { return true; }
                    inline bool visit(ExpressionType* input) { return static_cast<T*>(this)->visitExpressionType(input); }
                    inline bool end(ExpressionType* input) { return static_cast<T*>(this)->endExpressionType(input); }
                    inline bool traverse(ExpressionType* input) { return static_cast<T*>(this)->traverseExpressionType(input); }
                    inline bool visitExpressionType(ExpressionType* input) { return true; }
                    inline bool endExpressionType(ExpressionType* input) { return true; }
                    bool traverseExpressionTypes(ExpressionTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseExpressionType(ExpressionType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_AssignExpression) if(!traverse(input->m_AssignExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(ExplicitTypeOrAutos* input) { return static_cast<T*>(this)->visitExplicitTypeOrAutos(input); }
                    inline bool end(ExplicitTypeOrAutos* input) { return static_cast<T*>(this)->endExplicitTypeOrAutos(input); }
                    inline bool traverse(ExplicitTypeOrAutos* input) { return static_cast<T*>(this)->traverseExplicitTypeOrAutos(input); }
                    inline bool visitExplicitTypeOrAutos(ExplicitTypeOrAutos* input) { return true; }
                    inline bool endExplicitTypeOrAutos(ExplicitTypeOrAutos* input) { return true; }
                    inline bool visit(ExplicitTypeOrAuto* input) { return static_cast<T*>(this)->visitExplicitTypeOrAuto(input); }
                    inline bool end(ExplicitTypeOrAuto* input) { return static_cast<T*>(this)->endExplicitTypeOrAuto(input); }
                    inline bool traverse(ExplicitTypeOrAuto* input) { return static_cast<T*>(this)->traverseExplicitTypeOrAuto(input); }
                    inline bool visitExplicitTypeOrAuto(ExplicitTypeOrAuto* input) { return true; }
                    inline bool endExplicitTypeOrAuto(ExplicitTypeOrAuto* input) { return true; }
                    bool traverseExplicitTypeOrAutos(ExplicitTypeOrAutos* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseExplicitTypeOrAuto(ExplicitTypeOrAuto* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_BasicTypeOrAuto) if(!traverse(input->m_BasicTypeOrAuto)) return false;
                        if (input->m_TypeExtents) if(!traverse(input->m_TypeExtents)) return false;
                        return end(input);
                    }
                    inline bool visit(TypeOrAutos* input) { return static_cast<T*>(this)->visitTypeOrAutos(input); }
                    inline bool end(TypeOrAutos* input) { return static_cast<T*>(this)->endTypeOrAutos(input); }
                    inline bool traverse(TypeOrAutos* input) { return static_cast<T*>(this)->traverseTypeOrAutos(input); }
                    inline bool visitTypeOrAutos(TypeOrAutos* input) { return true; }
                    inline bool endTypeOrAutos(TypeOrAutos* input) { return true; }
                    inline bool visit(TypeOrAuto* input) { return static_cast<T*>(this)->visitTypeOrAuto(input); }
                    inline bool end(TypeOrAuto* input) { return static_cast<T*>(this)->endTypeOrAuto(input); }
                    inline bool traverse(TypeOrAuto* input) { return static_cast<T*>(this)->traverseTypeOrAuto(input); }
                    inline bool visitTypeOrAuto(TypeOrAuto* input) { return true; }
                    inline bool endTypeOrAuto(TypeOrAuto* input) { return true; }
                    bool traverseTypeOrAutos(TypeOrAutos* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTypeOrAuto(TypeOrAuto* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_ExplicitTypeOrAuto) if(!traverse(input->m_ExplicitTypeOrAuto)) return false;
                        if (input->m_ExpressionType) if(!traverse(input->m_ExpressionType)) return false;
                        return end(input);
                    }
                    inline bool visit(AutoTypes* input) { return static_cast<T*>(this)->visitAutoTypes(input); }
                    inline bool end(AutoTypes* input) { return static_cast<T*>(this)->endAutoTypes(input); }
                    inline bool traverse(AutoTypes* input) { return static_cast<T*>(this)->traverseAutoTypes(input); }
                    inline bool visitAutoTypes(AutoTypes* input) { return true; }
                    inline bool endAutoTypes(AutoTypes* input) { return true; }
                    inline bool visit(AutoType* input) { return static_cast<T*>(this)->visitAutoType(input); }
                    inline bool end(AutoType* input) { return static_cast<T*>(this)->endAutoType(input); }
                    inline bool traverse(AutoType* input) { return static_cast<T*>(this)->traverseAutoType(input); }
                    inline bool visitAutoType(AutoType* input) { return true; }
                    inline bool endAutoType(AutoType* input) { return true; }
                    bool traverseAutoTypes(AutoTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAutoType(AutoType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Auto) if(!traverse(input->m_Auto)) return false;
                        if (input->m_TypeExtents) if(!traverse(input->m_TypeExtents)) return false;
                        return end(input);
                    }
                    inline bool visit(BasicTypes* input) { return static_cast<T*>(this)->visitBasicTypes(input); }
                    inline bool end(BasicTypes* input) { return static_cast<T*>(this)->endBasicTypes(input); }
                    inline bool traverse(BasicTypes* input) { return static_cast<T*>(this)->traverseBasicTypes(input); }
                    inline bool visitBasicTypes(BasicTypes* input) { return true; }
                    inline bool endBasicTypes(BasicTypes* input) { return true; }
                    inline bool visit(BasicType* input) { return static_cast<T*>(this)->visitBasicType(input); }
                    inline bool end(BasicType* input) { return static_cast<T*>(this)->endBasicType(input); }
                    inline bool traverse(BasicType* input) { return static_cast<T*>(this)->traverseBasicType(input); }
                    inline bool visitBasicType(BasicType* input) { return true; }
                    inline bool endBasicType(BasicType* input) { return true; }
                    bool traverseBasicTypes(BasicTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBasicType(BasicType* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_FundamentalType) if(!traverse(input->m_FundamentalType)) return false;
                        if (input->m_DeclType) if(!traverse(input->m_DeclType)) return false;
                        if (input->m_QualifiedType) if(!traverse(input->m_QualifiedType)) return false;
                        return end(input);
                    }
                    inline bool visit(Autos* input) { return static_cast<T*>(this)->visitAutos(input); }
                    inline bool end(Autos* input) { return static_cast<T*>(this)->endAutos(input); }
                    inline bool traverse(Autos* input) { return static_cast<T*>(this)->traverseAutos(input); }
                    inline bool visitAutos(Autos* input) { return true; }
                    inline bool endAutos(Autos* input) { return true; }
                    inline bool visit(Auto* input) { return static_cast<T*>(this)->visitAuto(input); }
                    inline bool end(Auto* input) { return static_cast<T*>(this)->endAuto(input); }
                    inline bool traverse(Auto* input) { return static_cast<T*>(this)->traverseAuto(input); }
                    inline bool visitAuto(Auto* input) { return true; }
                    inline bool endAuto(Auto* input) { return true; }
                    bool traverseAutos(Autos* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAuto(Auto* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(BasicTypeOrAutos* input) { return static_cast<T*>(this)->visitBasicTypeOrAutos(input); }
                    inline bool end(BasicTypeOrAutos* input) { return static_cast<T*>(this)->endBasicTypeOrAutos(input); }
                    inline bool traverse(BasicTypeOrAutos* input) { return static_cast<T*>(this)->traverseBasicTypeOrAutos(input); }
                    inline bool visitBasicTypeOrAutos(BasicTypeOrAutos* input) { return true; }
                    inline bool endBasicTypeOrAutos(BasicTypeOrAutos* input) { return true; }
                    inline bool visit(BasicTypeOrAuto* input) { return static_cast<T*>(this)->visitBasicTypeOrAuto(input); }
                    inline bool end(BasicTypeOrAuto* input) { return static_cast<T*>(this)->endBasicTypeOrAuto(input); }
                    inline bool traverse(BasicTypeOrAuto* input) { return static_cast<T*>(this)->traverseBasicTypeOrAuto(input); }
                    inline bool visitBasicTypeOrAuto(BasicTypeOrAuto* input) { return true; }
                    inline bool endBasicTypeOrAuto(BasicTypeOrAuto* input) { return true; }
                    bool traverseBasicTypeOrAutos(BasicTypeOrAutos* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBasicTypeOrAuto(BasicTypeOrAuto* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_FundamentalType) if(!traverse(input->m_FundamentalType)) return false;
                        if (input->m_Auto) if(!traverse(input->m_Auto)) return false;
                        if (input->m_DeclType) if(!traverse(input->m_DeclType)) return false;
                        if (input->m_QualifiedType) if(!traverse(input->m_QualifiedType)) return false;
                        return end(input);
                    }
                    inline bool visit(IntegralConstants* input) { return static_cast<T*>(this)->visitIntegralConstants(input); }
                    inline bool end(IntegralConstants* input) { return static_cast<T*>(this)->endIntegralConstants(input); }
                    inline bool traverse(IntegralConstants* input) { return static_cast<T*>(this)->traverseIntegralConstants(input); }
                    inline bool visitIntegralConstants(IntegralConstants* input) { return true; }
                    inline bool endIntegralConstants(IntegralConstants* input) { return true; }
                    inline bool visit(IntegralConstant* input) { return static_cast<T*>(this)->visitIntegralConstant(input); }
                    inline bool end(IntegralConstant* input) { return static_cast<T*>(this)->endIntegralConstant(input); }
                    inline bool traverse(IntegralConstant* input) { return static_cast<T*>(this)->traverseIntegralConstant(input); }
                    inline bool visitIntegralConstant(IntegralConstant* input) { return true; }
                    inline bool endIntegralConstant(IntegralConstant* input) { return true; }
                    bool traverseIntegralConstants(IntegralConstants* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseIntegralConstant(IntegralConstant* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(ArrayExtents* input) { return static_cast<T*>(this)->visitArrayExtents(input); }
                    inline bool end(ArrayExtents* input) { return static_cast<T*>(this)->endArrayExtents(input); }
                    inline bool traverse(ArrayExtents* input) { return static_cast<T*>(this)->traverseArrayExtents(input); }
                    inline bool visitArrayExtents(ArrayExtents* input) { return true; }
                    inline bool endArrayExtents(ArrayExtents* input) { return true; }
                    inline bool visit(ArrayExtent* input) { return static_cast<T*>(this)->visitArrayExtent(input); }
                    inline bool end(ArrayExtent* input) { return static_cast<T*>(this)->endArrayExtent(input); }
                    inline bool traverse(ArrayExtent* input) { return static_cast<T*>(this)->traverseArrayExtent(input); }
                    inline bool visitArrayExtent(ArrayExtent* input) { return true; }
                    inline bool endArrayExtent(ArrayExtent* input) { return true; }
                    bool traverseArrayExtents(ArrayExtents* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseArrayExtent(ArrayExtent* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_ArrayExtentExpression) if(!traverse(input->m_ArrayExtentExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(FunctionTypeExtents* input) { return static_cast<T*>(this)->visitFunctionTypeExtents(input); }
                    inline bool end(FunctionTypeExtents* input) { return static_cast<T*>(this)->endFunctionTypeExtents(input); }
                    inline bool traverse(FunctionTypeExtents* input) { return static_cast<T*>(this)->traverseFunctionTypeExtents(input); }
                    inline bool visitFunctionTypeExtents(FunctionTypeExtents* input) { return true; }
                    inline bool endFunctionTypeExtents(FunctionTypeExtents* input) { return true; }
                    inline bool visit(FunctionTypeExtent* input) { return static_cast<T*>(this)->visitFunctionTypeExtent(input); }
                    inline bool end(FunctionTypeExtent* input) { return static_cast<T*>(this)->endFunctionTypeExtent(input); }
                    inline bool traverse(FunctionTypeExtent* input) { return static_cast<T*>(this)->traverseFunctionTypeExtent(input); }
                    inline bool visitFunctionTypeExtent(FunctionTypeExtent* input) { return true; }
                    inline bool endFunctionTypeExtent(FunctionTypeExtent* input) { return true; }
                    bool traverseFunctionTypeExtents(FunctionTypeExtents* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseFunctionTypeExtent(FunctionTypeExtent* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Types) if(!traverse(input->m_Types)) return false;
                        return end(input);
                    }
                    inline bool visit(DerivedExtents* input) { return static_cast<T*>(this)->visitDerivedExtents(input); }
                    inline bool end(DerivedExtents* input) { return static_cast<T*>(this)->endDerivedExtents(input); }
                    inline bool traverse(DerivedExtents* input) { return static_cast<T*>(this)->traverseDerivedExtents(input); }
                    inline bool visitDerivedExtents(DerivedExtents* input) { return true; }
                    inline bool endDerivedExtents(DerivedExtents* input) { return true; }
                    inline bool visit(DerivedExtent* input) { return static_cast<T*>(this)->visitDerivedExtent(input); }
                    inline bool end(DerivedExtent* input) { return static_cast<T*>(this)->endDerivedExtent(input); }
                    inline bool traverse(DerivedExtent* input) { return static_cast<T*>(this)->traverseDerivedExtent(input); }
                    inline bool visitDerivedExtent(DerivedExtent* input) { return true; }
                    inline bool endDerivedExtent(DerivedExtent* input) { return true; }
                    bool traverseDerivedExtents(DerivedExtents* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseDerivedExtent(DerivedExtent* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(TypeExtentNoFunctions* input) { return static_cast<T*>(this)->visitTypeExtentNoFunctions(input); }
                    inline bool end(TypeExtentNoFunctions* input) { return static_cast<T*>(this)->endTypeExtentNoFunctions(input); }
                    inline bool traverse(TypeExtentNoFunctions* input) { return static_cast<T*>(this)->traverseTypeExtentNoFunctions(input); }
                    inline bool visitTypeExtentNoFunctions(TypeExtentNoFunctions* input) { return true; }
                    inline bool endTypeExtentNoFunctions(TypeExtentNoFunctions* input) { return true; }
                    inline bool visit(TypeExtentNoFunction* input) { return static_cast<T*>(this)->visitTypeExtentNoFunction(input); }
                    inline bool end(TypeExtentNoFunction* input) { return static_cast<T*>(this)->endTypeExtentNoFunction(input); }
                    inline bool traverse(TypeExtentNoFunction* input) { return static_cast<T*>(this)->traverseTypeExtentNoFunction(input); }
                    inline bool visitTypeExtentNoFunction(TypeExtentNoFunction* input) { return true; }
                    inline bool endTypeExtentNoFunction(TypeExtentNoFunction* input) { return true; }
                    bool traverseTypeExtentNoFunctions(TypeExtentNoFunctions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTypeExtentNoFunction(TypeExtentNoFunction* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_ArrayExtent) if(!traverse(input->m_ArrayExtent)) return false;
                        if (input->m_DerivedExtent) if(!traverse(input->m_DerivedExtent)) return false;
                        return end(input);
                    }
                    inline bool visit(AssignExpressions* input) { return static_cast<T*>(this)->visitAssignExpressions(input); }
                    inline bool end(AssignExpressions* input) { return static_cast<T*>(this)->endAssignExpressions(input); }
                    inline bool traverse(AssignExpressions* input) { return static_cast<T*>(this)->traverseAssignExpressions(input); }
                    inline bool visitAssignExpressions(AssignExpressions* input) { return true; }
                    inline bool endAssignExpressions(AssignExpressions* input) { return true; }
                    inline bool visit(AssignExpression* input) { return static_cast<T*>(this)->visitAssignExpression(input); }
                    inline bool end(AssignExpression* input) { return static_cast<T*>(this)->endAssignExpression(input); }
                    inline bool traverse(AssignExpression* input) { return static_cast<T*>(this)->traverseAssignExpression(input); }
                    inline bool visitAssignExpression(AssignExpression* input) { return true; }
                    inline bool endAssignExpression(AssignExpression* input) { return true; }
                    bool traverseAssignExpressions(AssignExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAssignExpression(AssignExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_RightShiftAssign_RightExpression) if(!traverse(input->m_RightShiftAssign_RightExpression)) return false;
                        if (input->m_RightShift_RightExpression) if(!traverse(input->m_RightShift_RightExpression)) return false;
                        if (input->m_Greater_RightExpression) if(!traverse(input->m_Greater_RightExpression)) return false;
                        if (input->m_RightExpression) if(!traverse(input->m_RightExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(ConditionalExpressions* input) { return static_cast<T*>(this)->visitConditionalExpressions(input); }
                    inline bool end(ConditionalExpressions* input) { return static_cast<T*>(this)->endConditionalExpressions(input); }
                    inline bool traverse(ConditionalExpressions* input) { return static_cast<T*>(this)->traverseConditionalExpressions(input); }
                    inline bool visitConditionalExpressions(ConditionalExpressions* input) { return true; }
                    inline bool endConditionalExpressions(ConditionalExpressions* input) { return true; }
                    inline bool visit(ConditionalExpression* input) { return static_cast<T*>(this)->visitConditionalExpression(input); }
                    inline bool end(ConditionalExpression* input) { return static_cast<T*>(this)->endConditionalExpression(input); }
                    inline bool traverse(ConditionalExpression* input) { return static_cast<T*>(this)->traverseConditionalExpression(input); }
                    inline bool visitConditionalExpression(ConditionalExpression* input) { return true; }
                    inline bool endConditionalExpression(ConditionalExpression* input) { return true; }
                    bool traverseConditionalExpressions(ConditionalExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseConditionalExpression(ConditionalExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_Then) if(!traverse(input->m_Then)) return false;
                        if (input->m_Else) if(!traverse(input->m_Else)) return false;
                        return end(input);
                    }
                    inline bool visit(Friends* input) { return static_cast<T*>(this)->visitFriends(input); }
                    inline bool end(Friends* input) { return static_cast<T*>(this)->endFriends(input); }
                    inline bool traverse(Friends* input) { return static_cast<T*>(this)->traverseFriends(input); }
                    inline bool visitFriends(Friends* input) { return true; }
                    inline bool endFriends(Friends* input) { return true; }
                    inline bool visit(Friend* input) { return static_cast<T*>(this)->visitFriend(input); }
                    inline bool end(Friend* input) { return static_cast<T*>(this)->endFriend(input); }
                    inline bool traverse(Friend* input) { return static_cast<T*>(this)->traverseFriend(input); }
                    inline bool visitFriend(Friend* input) { return true; }
                    inline bool endFriend(Friend* input) { return true; }
                    bool traverseFriends(Friends* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseFriend(Friend* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_QualifiedName) if(!traverse(input->m_QualifiedName)) return false;
                        return end(input);
                    }
                    inline bool visit(LogicalOrExpressions* input) { return static_cast<T*>(this)->visitLogicalOrExpressions(input); }
                    inline bool end(LogicalOrExpressions* input) { return static_cast<T*>(this)->endLogicalOrExpressions(input); }
                    inline bool traverse(LogicalOrExpressions* input) { return static_cast<T*>(this)->traverseLogicalOrExpressions(input); }
                    inline bool visitLogicalOrExpressions(LogicalOrExpressions* input) { return true; }
                    inline bool endLogicalOrExpressions(LogicalOrExpressions* input) { return true; }
                    inline bool visit(LogicalOrExpression* input) { return static_cast<T*>(this)->visitLogicalOrExpression(input); }
                    inline bool end(LogicalOrExpression* input) { return static_cast<T*>(this)->endLogicalOrExpression(input); }
                    inline bool traverse(LogicalOrExpression* input) { return static_cast<T*>(this)->traverseLogicalOrExpression(input); }
                    inline bool visitLogicalOrExpression(LogicalOrExpression* input) { return true; }
                    inline bool endLogicalOrExpression(LogicalOrExpression* input) { return true; }
                    bool traverseLogicalOrExpressions(LogicalOrExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLogicalOrExpression(LogicalOrExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_LogicalOrExpressionRights) if(!traverse(input->m_LogicalOrExpressionRights)) return false;
                        return end(input);
                    }
                    inline bool visit(BasicMembers* input) { return static_cast<T*>(this)->visitBasicMembers(input); }
                    inline bool end(BasicMembers* input) { return static_cast<T*>(this)->endBasicMembers(input); }
                    inline bool traverse(BasicMembers* input) { return static_cast<T*>(this)->traverseBasicMembers(input); }
                    inline bool visitBasicMembers(BasicMembers* input) { return true; }
                    inline bool endBasicMembers(BasicMembers* input) { return true; }
                    inline bool visit(BasicMember* input) { return static_cast<T*>(this)->visitBasicMember(input); }
                    inline bool end(BasicMember* input) { return static_cast<T*>(this)->endBasicMember(input); }
                    inline bool traverse(BasicMember* input) { return static_cast<T*>(this)->traverseBasicMember(input); }
                    inline bool visitBasicMember(BasicMember* input) { return true; }
                    inline bool endBasicMember(BasicMember* input) { return true; }
                    bool traverseBasicMembers(BasicMembers* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBasicMember(BasicMember* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_AccessSpecifier) if(!traverse(input->m_AccessSpecifier)) return false;
                        if (input->m_Destructor) if(!traverse(input->m_Destructor)) return false;
                        if (input->m_Method) if(!traverse(input->m_Method)) return false;
                        if (input->m_Method0) if(!traverse(input->m_Method0)) return false;
                        if (input->m_FunctionOrVariable) if(!traverse(input->m_FunctionOrVariable)) return false;
                        if (input->m_ConversionFunction) if(!traverse(input->m_ConversionFunction)) return false;
                        if (input->m_Constructor) if(!traverse(input->m_Constructor)) return false;
                        if (input->m_Destructor0) if(!traverse(input->m_Destructor0)) return false;
                        if (input->m_Friend) if(!traverse(input->m_Friend)) return false;
                        if (input->m_BasicMemberWithMetaOrAnnotation) if(!traverse(input->m_BasicMemberWithMetaOrAnnotation)) return false;
                        if (input->m_Declaration) if(!traverse(input->m_Declaration)) return false;
                        return end(input);
                    }
                    inline bool visit(LogicalAndExpressions* input) { return static_cast<T*>(this)->visitLogicalAndExpressions(input); }
                    inline bool end(LogicalAndExpressions* input) { return static_cast<T*>(this)->endLogicalAndExpressions(input); }
                    inline bool traverse(LogicalAndExpressions* input) { return static_cast<T*>(this)->traverseLogicalAndExpressions(input); }
                    inline bool visitLogicalAndExpressions(LogicalAndExpressions* input) { return true; }
                    inline bool endLogicalAndExpressions(LogicalAndExpressions* input) { return true; }
                    inline bool visit(LogicalAndExpression* input) { return static_cast<T*>(this)->visitLogicalAndExpression(input); }
                    inline bool end(LogicalAndExpression* input) { return static_cast<T*>(this)->endLogicalAndExpression(input); }
                    inline bool traverse(LogicalAndExpression* input) { return static_cast<T*>(this)->traverseLogicalAndExpression(input); }
                    inline bool visitLogicalAndExpression(LogicalAndExpression* input) { return true; }
                    inline bool endLogicalAndExpression(LogicalAndExpression* input) { return true; }
                    bool traverseLogicalAndExpressions(LogicalAndExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLogicalAndExpression(LogicalAndExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_LogicalAndExpressionRights) if(!traverse(input->m_LogicalAndExpressionRights)) return false;
                        return end(input);
                    }
                    inline bool visit(ForStatements* input) { return static_cast<T*>(this)->visitForStatements(input); }
                    inline bool end(ForStatements* input) { return static_cast<T*>(this)->endForStatements(input); }
                    inline bool traverse(ForStatements* input) { return static_cast<T*>(this)->traverseForStatements(input); }
                    inline bool visitForStatements(ForStatements* input) { return true; }
                    inline bool endForStatements(ForStatements* input) { return true; }
                    inline bool visit(ForStatement* input) { return static_cast<T*>(this)->visitForStatement(input); }
                    inline bool end(ForStatement* input) { return static_cast<T*>(this)->endForStatement(input); }
                    inline bool traverse(ForStatement* input) { return static_cast<T*>(this)->traverseForStatement(input); }
                    inline bool visitForStatement(ForStatement* input) { return true; }
                    inline bool endForStatement(ForStatement* input) { return true; }
                    bool traverseForStatements(ForStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseForStatement(ForStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_ForeachSignature) if(!traverse(input->m_ForeachSignature)) return false;
                        if (input->m_Init) if(!traverse(input->m_Init)) return false;
                        if (input->m_Test) if(!traverse(input->m_Test)) return false;
                        if (input->m_Update) if(!traverse(input->m_Update)) return false;
                        if (input->m_Statement) if(!traverse(input->m_Statement)) return false;
                        return end(input);
                    }
                    inline bool visit(BinOrExpressions* input) { return static_cast<T*>(this)->visitBinOrExpressions(input); }
                    inline bool end(BinOrExpressions* input) { return static_cast<T*>(this)->endBinOrExpressions(input); }
                    inline bool traverse(BinOrExpressions* input) { return static_cast<T*>(this)->traverseBinOrExpressions(input); }
                    inline bool visitBinOrExpressions(BinOrExpressions* input) { return true; }
                    inline bool endBinOrExpressions(BinOrExpressions* input) { return true; }
                    inline bool visit(BinOrExpression* input) { return static_cast<T*>(this)->visitBinOrExpression(input); }
                    inline bool end(BinOrExpression* input) { return static_cast<T*>(this)->endBinOrExpression(input); }
                    inline bool traverse(BinOrExpression* input) { return static_cast<T*>(this)->traverseBinOrExpression(input); }
                    inline bool visitBinOrExpression(BinOrExpression* input) { return true; }
                    inline bool endBinOrExpression(BinOrExpression* input) { return true; }
                    bool traverseBinOrExpressions(BinOrExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBinOrExpression(BinOrExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_BinOrExpressionRights) if(!traverse(input->m_BinOrExpressionRights)) return false;
                        return end(input);
                    }
                    inline bool visit(WhileStatements* input) { return static_cast<T*>(this)->visitWhileStatements(input); }
                    inline bool end(WhileStatements* input) { return static_cast<T*>(this)->endWhileStatements(input); }
                    inline bool traverse(WhileStatements* input) { return static_cast<T*>(this)->traverseWhileStatements(input); }
                    inline bool visitWhileStatements(WhileStatements* input) { return true; }
                    inline bool endWhileStatements(WhileStatements* input) { return true; }
                    inline bool visit(WhileStatement* input) { return static_cast<T*>(this)->visitWhileStatement(input); }
                    inline bool end(WhileStatement* input) { return static_cast<T*>(this)->endWhileStatement(input); }
                    inline bool traverse(WhileStatement* input) { return static_cast<T*>(this)->traverseWhileStatement(input); }
                    inline bool visitWhileStatement(WhileStatement* input) { return true; }
                    inline bool endWhileStatement(WhileStatement* input) { return true; }
                    bool traverseWhileStatements(WhileStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseWhileStatement(WhileStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Condition) if(!traverse(input->m_Condition)) return false;
                        if (input->m_Statement) if(!traverse(input->m_Statement)) return false;
                        return end(input);
                    }
                    inline bool visit(XOrExpressions* input) { return static_cast<T*>(this)->visitXOrExpressions(input); }
                    inline bool end(XOrExpressions* input) { return static_cast<T*>(this)->endXOrExpressions(input); }
                    inline bool traverse(XOrExpressions* input) { return static_cast<T*>(this)->traverseXOrExpressions(input); }
                    inline bool visitXOrExpressions(XOrExpressions* input) { return true; }
                    inline bool endXOrExpressions(XOrExpressions* input) { return true; }
                    inline bool visit(XOrExpression* input) { return static_cast<T*>(this)->visitXOrExpression(input); }
                    inline bool end(XOrExpression* input) { return static_cast<T*>(this)->endXOrExpression(input); }
                    inline bool traverse(XOrExpression* input) { return static_cast<T*>(this)->traverseXOrExpression(input); }
                    inline bool visitXOrExpression(XOrExpression* input) { return true; }
                    inline bool endXOrExpression(XOrExpression* input) { return true; }
                    bool traverseXOrExpressions(XOrExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseXOrExpression(XOrExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_XOrExpressionRights) if(!traverse(input->m_XOrExpressionRights)) return false;
                        return end(input);
                    }
                    inline bool visit(BlockStatements* input) { return static_cast<T*>(this)->visitBlockStatements(input); }
                    inline bool end(BlockStatements* input) { return static_cast<T*>(this)->endBlockStatements(input); }
                    inline bool traverse(BlockStatements* input) { return static_cast<T*>(this)->traverseBlockStatements(input); }
                    inline bool visitBlockStatements(BlockStatements* input) { return true; }
                    inline bool endBlockStatements(BlockStatements* input) { return true; }
                    inline bool visit(BlockStatement* input) { return static_cast<T*>(this)->visitBlockStatement(input); }
                    inline bool end(BlockStatement* input) { return static_cast<T*>(this)->endBlockStatement(input); }
                    inline bool traverse(BlockStatement* input) { return static_cast<T*>(this)->traverseBlockStatement(input); }
                    inline bool visitBlockStatement(BlockStatement* input) { return true; }
                    inline bool endBlockStatement(BlockStatement* input) { return true; }
                    bool traverseBlockStatements(BlockStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBlockStatement(BlockStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Statements) if(!traverse(input->m_Statements)) return false;
                        return end(input);
                    }
                    inline bool visit(BinAndExpressions* input) { return static_cast<T*>(this)->visitBinAndExpressions(input); }
                    inline bool end(BinAndExpressions* input) { return static_cast<T*>(this)->endBinAndExpressions(input); }
                    inline bool traverse(BinAndExpressions* input) { return static_cast<T*>(this)->traverseBinAndExpressions(input); }
                    inline bool visitBinAndExpressions(BinAndExpressions* input) { return true; }
                    inline bool endBinAndExpressions(BinAndExpressions* input) { return true; }
                    inline bool visit(BinAndExpression* input) { return static_cast<T*>(this)->visitBinAndExpression(input); }
                    inline bool end(BinAndExpression* input) { return static_cast<T*>(this)->endBinAndExpression(input); }
                    inline bool traverse(BinAndExpression* input) { return static_cast<T*>(this)->traverseBinAndExpression(input); }
                    inline bool visitBinAndExpression(BinAndExpression* input) { return true; }
                    inline bool endBinAndExpression(BinAndExpression* input) { return true; }
                    bool traverseBinAndExpressions(BinAndExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBinAndExpression(BinAndExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_BinAndExpressionRights) if(!traverse(input->m_BinAndExpressionRights)) return false;
                        return end(input);
                    }
                    inline bool visit(DoWhileStatements* input) { return static_cast<T*>(this)->visitDoWhileStatements(input); }
                    inline bool end(DoWhileStatements* input) { return static_cast<T*>(this)->endDoWhileStatements(input); }
                    inline bool traverse(DoWhileStatements* input) { return static_cast<T*>(this)->traverseDoWhileStatements(input); }
                    inline bool visitDoWhileStatements(DoWhileStatements* input) { return true; }
                    inline bool endDoWhileStatements(DoWhileStatements* input) { return true; }
                    inline bool visit(DoWhileStatement* input) { return static_cast<T*>(this)->visitDoWhileStatement(input); }
                    inline bool end(DoWhileStatement* input) { return static_cast<T*>(this)->endDoWhileStatement(input); }
                    inline bool traverse(DoWhileStatement* input) { return static_cast<T*>(this)->traverseDoWhileStatement(input); }
                    inline bool visitDoWhileStatement(DoWhileStatement* input) { return true; }
                    inline bool endDoWhileStatement(DoWhileStatement* input) { return true; }
                    bool traverseDoWhileStatements(DoWhileStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseDoWhileStatement(DoWhileStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_BlockStatement) if(!traverse(input->m_BlockStatement)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(EqualityExpressions* input) { return static_cast<T*>(this)->visitEqualityExpressions(input); }
                    inline bool end(EqualityExpressions* input) { return static_cast<T*>(this)->endEqualityExpressions(input); }
                    inline bool traverse(EqualityExpressions* input) { return static_cast<T*>(this)->traverseEqualityExpressions(input); }
                    inline bool visitEqualityExpressions(EqualityExpressions* input) { return true; }
                    inline bool endEqualityExpressions(EqualityExpressions* input) { return true; }
                    inline bool visit(EqualityExpression* input) { return static_cast<T*>(this)->visitEqualityExpression(input); }
                    inline bool end(EqualityExpression* input) { return static_cast<T*>(this)->endEqualityExpression(input); }
                    inline bool traverse(EqualityExpression* input) { return static_cast<T*>(this)->traverseEqualityExpression(input); }
                    inline bool visitEqualityExpression(EqualityExpression* input) { return true; }
                    inline bool endEqualityExpression(EqualityExpression* input) { return true; }
                    bool traverseEqualityExpressions(EqualityExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseEqualityExpression(EqualityExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_EqualityExpressionRights) if(!traverse(input->m_EqualityExpressionRights)) return false;
                        return end(input);
                    }
                    inline bool visit(StatementOrCases* input) { return static_cast<T*>(this)->visitStatementOrCases(input); }
                    inline bool end(StatementOrCases* input) { return static_cast<T*>(this)->endStatementOrCases(input); }
                    inline bool traverse(StatementOrCases* input) { return static_cast<T*>(this)->traverseStatementOrCases(input); }
                    inline bool visitStatementOrCases(StatementOrCases* input) { return true; }
                    inline bool endStatementOrCases(StatementOrCases* input) { return true; }
                    inline bool visit(StatementOrCase* input) { return static_cast<T*>(this)->visitStatementOrCase(input); }
                    inline bool end(StatementOrCase* input) { return static_cast<T*>(this)->endStatementOrCase(input); }
                    inline bool traverse(StatementOrCase* input) { return static_cast<T*>(this)->traverseStatementOrCase(input); }
                    inline bool visitStatementOrCase(StatementOrCase* input) { return true; }
                    inline bool endStatementOrCase(StatementOrCase* input) { return true; }
                    bool traverseStatementOrCases(StatementOrCases* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseStatementOrCase(StatementOrCase* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_CaseStatement) if(!traverse(input->m_CaseStatement)) return false;
                        if (input->m_DefaultStatement) if(!traverse(input->m_DefaultStatement)) return false;
                        if (input->m_Statement) if(!traverse(input->m_Statement)) return false;
                        return end(input);
                    }
                    inline bool visit(RelationalExpressions* input) { return static_cast<T*>(this)->visitRelationalExpressions(input); }
                    inline bool end(RelationalExpressions* input) { return static_cast<T*>(this)->endRelationalExpressions(input); }
                    inline bool traverse(RelationalExpressions* input) { return static_cast<T*>(this)->traverseRelationalExpressions(input); }
                    inline bool visitRelationalExpressions(RelationalExpressions* input) { return true; }
                    inline bool endRelationalExpressions(RelationalExpressions* input) { return true; }
                    inline bool visit(RelationalExpression* input) { return static_cast<T*>(this)->visitRelationalExpression(input); }
                    inline bool end(RelationalExpression* input) { return static_cast<T*>(this)->endRelationalExpression(input); }
                    inline bool traverse(RelationalExpression* input) { return static_cast<T*>(this)->traverseRelationalExpression(input); }
                    inline bool visitRelationalExpression(RelationalExpression* input) { return true; }
                    inline bool endRelationalExpression(RelationalExpression* input) { return true; }
                    bool traverseRelationalExpressions(RelationalExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseRelationalExpression(RelationalExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_RelationalExpressionRights) if(!traverse(input->m_RelationalExpressionRights)) return false;
                        return end(input);
                    }
                    inline bool visit(MemberWithMetaOrAnnotations* input) { return static_cast<T*>(this)->visitMemberWithMetaOrAnnotations(input); }
                    inline bool end(MemberWithMetaOrAnnotations* input) { return static_cast<T*>(this)->endMemberWithMetaOrAnnotations(input); }
                    inline bool traverse(MemberWithMetaOrAnnotations* input) { return static_cast<T*>(this)->traverseMemberWithMetaOrAnnotations(input); }
                    inline bool visitMemberWithMetaOrAnnotations(MemberWithMetaOrAnnotations* input) { return true; }
                    inline bool endMemberWithMetaOrAnnotations(MemberWithMetaOrAnnotations* input) { return true; }
                    inline bool visit(MemberWithMetaOrAnnotation* input) { return static_cast<T*>(this)->visitMemberWithMetaOrAnnotation(input); }
                    inline bool end(MemberWithMetaOrAnnotation* input) { return static_cast<T*>(this)->endMemberWithMetaOrAnnotation(input); }
                    inline bool traverse(MemberWithMetaOrAnnotation* input) { return static_cast<T*>(this)->traverseMemberWithMetaOrAnnotation(input); }
                    inline bool visitMemberWithMetaOrAnnotation(MemberWithMetaOrAnnotation* input) { return true; }
                    inline bool endMemberWithMetaOrAnnotation(MemberWithMetaOrAnnotation* input) { return true; }
                    bool traverseMemberWithMetaOrAnnotations(MemberWithMetaOrAnnotations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseMemberWithMetaOrAnnotation(MemberWithMetaOrAnnotation* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_MetaOrAnnotations) if(!traverse(input->m_MetaOrAnnotations)) return false;
                        if (input->m_Member) if(!traverse(input->m_Member)) return false;
                        return end(input);
                    }
                    inline bool visit(ShiftExpressions* input) { return static_cast<T*>(this)->visitShiftExpressions(input); }
                    inline bool end(ShiftExpressions* input) { return static_cast<T*>(this)->endShiftExpressions(input); }
                    inline bool traverse(ShiftExpressions* input) { return static_cast<T*>(this)->traverseShiftExpressions(input); }
                    inline bool visitShiftExpressions(ShiftExpressions* input) { return true; }
                    inline bool endShiftExpressions(ShiftExpressions* input) { return true; }
                    inline bool visit(ShiftExpression* input) { return static_cast<T*>(this)->visitShiftExpression(input); }
                    inline bool end(ShiftExpression* input) { return static_cast<T*>(this)->endShiftExpression(input); }
                    inline bool traverse(ShiftExpression* input) { return static_cast<T*>(this)->traverseShiftExpression(input); }
                    inline bool visitShiftExpression(ShiftExpression* input) { return true; }
                    inline bool endShiftExpression(ShiftExpression* input) { return true; }
                    bool traverseShiftExpressions(ShiftExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseShiftExpression(ShiftExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_ShiftExpressionRights) if(!traverse(input->m_ShiftExpressionRights)) return false;
                        return end(input);
                    }
                    inline bool visit(BasicMemberWithMetaOrAnnotations* input) { return static_cast<T*>(this)->visitBasicMemberWithMetaOrAnnotations(input); }
                    inline bool end(BasicMemberWithMetaOrAnnotations* input) { return static_cast<T*>(this)->endBasicMemberWithMetaOrAnnotations(input); }
                    inline bool traverse(BasicMemberWithMetaOrAnnotations* input) { return static_cast<T*>(this)->traverseBasicMemberWithMetaOrAnnotations(input); }
                    inline bool visitBasicMemberWithMetaOrAnnotations(BasicMemberWithMetaOrAnnotations* input) { return true; }
                    inline bool endBasicMemberWithMetaOrAnnotations(BasicMemberWithMetaOrAnnotations* input) { return true; }
                    inline bool visit(BasicMemberWithMetaOrAnnotation* input) { return static_cast<T*>(this)->visitBasicMemberWithMetaOrAnnotation(input); }
                    inline bool end(BasicMemberWithMetaOrAnnotation* input) { return static_cast<T*>(this)->endBasicMemberWithMetaOrAnnotation(input); }
                    inline bool traverse(BasicMemberWithMetaOrAnnotation* input) { return static_cast<T*>(this)->traverseBasicMemberWithMetaOrAnnotation(input); }
                    inline bool visitBasicMemberWithMetaOrAnnotation(BasicMemberWithMetaOrAnnotation* input) { return true; }
                    inline bool endBasicMemberWithMetaOrAnnotation(BasicMemberWithMetaOrAnnotation* input) { return true; }
                    bool traverseBasicMemberWithMetaOrAnnotations(BasicMemberWithMetaOrAnnotations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBasicMemberWithMetaOrAnnotation(BasicMemberWithMetaOrAnnotation* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_MetaOrAnnotations) if(!traverse(input->m_MetaOrAnnotations)) return false;
                        if (input->m_BasicMember) if(!traverse(input->m_BasicMember)) return false;
                        return end(input);
                    }
                    inline bool visit(AddExpressions* input) { return static_cast<T*>(this)->visitAddExpressions(input); }
                    inline bool end(AddExpressions* input) { return static_cast<T*>(this)->endAddExpressions(input); }
                    inline bool traverse(AddExpressions* input) { return static_cast<T*>(this)->traverseAddExpressions(input); }
                    inline bool visitAddExpressions(AddExpressions* input) { return true; }
                    inline bool endAddExpressions(AddExpressions* input) { return true; }
                    inline bool visit(AddExpression* input) { return static_cast<T*>(this)->visitAddExpression(input); }
                    inline bool end(AddExpression* input) { return static_cast<T*>(this)->endAddExpression(input); }
                    inline bool traverse(AddExpression* input) { return static_cast<T*>(this)->traverseAddExpression(input); }
                    inline bool visitAddExpression(AddExpression* input) { return true; }
                    inline bool endAddExpression(AddExpression* input) { return true; }
                    bool traverseAddExpressions(AddExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAddExpression(AddExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_AddExpressionRights) if(!traverse(input->m_AddExpressionRights)) return false;
                        return end(input);
                    }
                    inline bool visit(IfStatements* input) { return static_cast<T*>(this)->visitIfStatements(input); }
                    inline bool end(IfStatements* input) { return static_cast<T*>(this)->endIfStatements(input); }
                    inline bool traverse(IfStatements* input) { return static_cast<T*>(this)->traverseIfStatements(input); }
                    inline bool visitIfStatements(IfStatements* input) { return true; }
                    inline bool endIfStatements(IfStatements* input) { return true; }
                    inline bool visit(IfStatement* input) { return static_cast<T*>(this)->visitIfStatement(input); }
                    inline bool end(IfStatement* input) { return static_cast<T*>(this)->endIfStatement(input); }
                    inline bool traverse(IfStatement* input) { return static_cast<T*>(this)->traverseIfStatement(input); }
                    inline bool visitIfStatement(IfStatement* input) { return true; }
                    inline bool endIfStatement(IfStatement* input) { return true; }
                    bool traverseIfStatements(IfStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseIfStatement(IfStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Condition) if(!traverse(input->m_Condition)) return false;
                        if (input->m_Then) if(!traverse(input->m_Then)) return false;
                        if (input->m_Else) if(!traverse(input->m_Else)) return false;
                        return end(input);
                    }
                    inline bool visit(MulExpressions* input) { return static_cast<T*>(this)->visitMulExpressions(input); }
                    inline bool end(MulExpressions* input) { return static_cast<T*>(this)->endMulExpressions(input); }
                    inline bool traverse(MulExpressions* input) { return static_cast<T*>(this)->traverseMulExpressions(input); }
                    inline bool visitMulExpressions(MulExpressions* input) { return true; }
                    inline bool endMulExpressions(MulExpressions* input) { return true; }
                    inline bool visit(MulExpression* input) { return static_cast<T*>(this)->visitMulExpression(input); }
                    inline bool end(MulExpression* input) { return static_cast<T*>(this)->endMulExpression(input); }
                    inline bool traverse(MulExpression* input) { return static_cast<T*>(this)->traverseMulExpression(input); }
                    inline bool visitMulExpression(MulExpression* input) { return true; }
                    inline bool endMulExpression(MulExpression* input) { return true; }
                    bool traverseMulExpressions(MulExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseMulExpression(MulExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LeftExpression) if(!traverse(input->m_LeftExpression)) return false;
                        if (input->m_MulExpressionRights) if(!traverse(input->m_MulExpressionRights)) return false;
                        return end(input);
                    }
                    inline bool visit(CaseStatements* input) { return static_cast<T*>(this)->visitCaseStatements(input); }
                    inline bool end(CaseStatements* input) { return static_cast<T*>(this)->endCaseStatements(input); }
                    inline bool traverse(CaseStatements* input) { return static_cast<T*>(this)->traverseCaseStatements(input); }
                    inline bool visitCaseStatements(CaseStatements* input) { return true; }
                    inline bool endCaseStatements(CaseStatements* input) { return true; }
                    inline bool visit(CaseStatement* input) { return static_cast<T*>(this)->visitCaseStatement(input); }
                    inline bool end(CaseStatement* input) { return static_cast<T*>(this)->endCaseStatement(input); }
                    inline bool traverse(CaseStatement* input) { return static_cast<T*>(this)->traverseCaseStatement(input); }
                    inline bool visitCaseStatement(CaseStatement* input) { return true; }
                    inline bool endCaseStatement(CaseStatement* input) { return true; }
                    bool traverseCaseStatements(CaseStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseCaseStatement(CaseStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(ExplicitLocalVariableDecls* input) { return static_cast<T*>(this)->visitExplicitLocalVariableDecls(input); }
                    inline bool end(ExplicitLocalVariableDecls* input) { return static_cast<T*>(this)->endExplicitLocalVariableDecls(input); }
                    inline bool traverse(ExplicitLocalVariableDecls* input) { return static_cast<T*>(this)->traverseExplicitLocalVariableDecls(input); }
                    inline bool visitExplicitLocalVariableDecls(ExplicitLocalVariableDecls* input) { return true; }
                    inline bool endExplicitLocalVariableDecls(ExplicitLocalVariableDecls* input) { return true; }
                    inline bool visit(ExplicitLocalVariableDecl* input) { return static_cast<T*>(this)->visitExplicitLocalVariableDecl(input); }
                    inline bool end(ExplicitLocalVariableDecl* input) { return static_cast<T*>(this)->endExplicitLocalVariableDecl(input); }
                    inline bool traverse(ExplicitLocalVariableDecl* input) { return static_cast<T*>(this)->traverseExplicitLocalVariableDecl(input); }
                    inline bool visitExplicitLocalVariableDecl(ExplicitLocalVariableDecl* input) { return true; }
                    inline bool endExplicitLocalVariableDecl(ExplicitLocalVariableDecl* input) { return true; }
                    bool traverseExplicitLocalVariableDecls(ExplicitLocalVariableDecls* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseExplicitLocalVariableDecl(ExplicitLocalVariableDecl* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_BasicType) if(!traverse(input->m_BasicType)) return false;
                        if (input->m_Declarators) if(!traverse(input->m_Declarators)) return false;
                        return end(input);
                    }
                    inline bool visit(UnaryExpressions* input) { return static_cast<T*>(this)->visitUnaryExpressions(input); }
                    inline bool end(UnaryExpressions* input) { return static_cast<T*>(this)->endUnaryExpressions(input); }
                    inline bool traverse(UnaryExpressions* input) { return static_cast<T*>(this)->traverseUnaryExpressions(input); }
                    inline bool visitUnaryExpressions(UnaryExpressions* input) { return true; }
                    inline bool endUnaryExpressions(UnaryExpressions* input) { return true; }
                    inline bool visit(UnaryExpression* input) { return static_cast<T*>(this)->visitUnaryExpression(input); }
                    inline bool end(UnaryExpression* input) { return static_cast<T*>(this)->endUnaryExpression(input); }
                    inline bool traverse(UnaryExpression* input) { return static_cast<T*>(this)->traverseUnaryExpression(input); }
                    inline bool visitUnaryExpression(UnaryExpression* input) { return true; }
                    inline bool endUnaryExpression(UnaryExpression* input) { return true; }
                    bool traverseUnaryExpressions(UnaryExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseUnaryExpression(UnaryExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        if (input->m_PostFixExpression) if(!traverse(input->m_PostFixExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(AssignExpressionOrInitializerLists* input) { return static_cast<T*>(this)->visitAssignExpressionOrInitializerLists(input); }
                    inline bool end(AssignExpressionOrInitializerLists* input) { return static_cast<T*>(this)->endAssignExpressionOrInitializerLists(input); }
                    inline bool traverse(AssignExpressionOrInitializerLists* input) { return static_cast<T*>(this)->traverseAssignExpressionOrInitializerLists(input); }
                    inline bool visitAssignExpressionOrInitializerLists(AssignExpressionOrInitializerLists* input) { return true; }
                    inline bool endAssignExpressionOrInitializerLists(AssignExpressionOrInitializerLists* input) { return true; }
                    inline bool visit(AssignExpressionOrInitializerList* input) { return static_cast<T*>(this)->visitAssignExpressionOrInitializerList(input); }
                    inline bool end(AssignExpressionOrInitializerList* input) { return static_cast<T*>(this)->endAssignExpressionOrInitializerList(input); }
                    inline bool traverse(AssignExpressionOrInitializerList* input) { return static_cast<T*>(this)->traverseAssignExpressionOrInitializerList(input); }
                    inline bool visitAssignExpressionOrInitializerList(AssignExpressionOrInitializerList* input) { return true; }
                    inline bool endAssignExpressionOrInitializerList(AssignExpressionOrInitializerList* input) { return true; }
                    bool traverseAssignExpressionOrInitializerLists(AssignExpressionOrInitializerLists* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAssignExpressionOrInitializerList(AssignExpressionOrInitializerList* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_InitializerList) if(!traverse(input->m_InitializerList)) return false;
                        if (input->m_AssignExpression) if(!traverse(input->m_AssignExpression)) return false;
                        return end(input);
                    }
                    inline bool visit(ExpressionOrInitializerLists* input) { return static_cast<T*>(this)->visitExpressionOrInitializerLists(input); }
                    inline bool end(ExpressionOrInitializerLists* input) { return static_cast<T*>(this)->endExpressionOrInitializerLists(input); }
                    inline bool traverse(ExpressionOrInitializerLists* input) { return static_cast<T*>(this)->traverseExpressionOrInitializerLists(input); }
                    inline bool visitExpressionOrInitializerLists(ExpressionOrInitializerLists* input) { return true; }
                    inline bool endExpressionOrInitializerLists(ExpressionOrInitializerLists* input) { return true; }
                    inline bool visit(ExpressionOrInitializerList* input) { return static_cast<T*>(this)->visitExpressionOrInitializerList(input); }
                    inline bool end(ExpressionOrInitializerList* input) { return static_cast<T*>(this)->endExpressionOrInitializerList(input); }
                    inline bool traverse(ExpressionOrInitializerList* input) { return static_cast<T*>(this)->traverseExpressionOrInitializerList(input); }
                    inline bool visitExpressionOrInitializerList(ExpressionOrInitializerList* input) { return true; }
                    inline bool endExpressionOrInitializerList(ExpressionOrInitializerList* input) { return true; }
                    bool traverseExpressionOrInitializerLists(ExpressionOrInitializerLists* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseExpressionOrInitializerList(ExpressionOrInitializerList* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_InitializerList) if(!traverse(input->m_InitializerList)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(InitializerLists* input) { return static_cast<T*>(this)->visitInitializerLists(input); }
                    inline bool end(InitializerLists* input) { return static_cast<T*>(this)->endInitializerLists(input); }
                    inline bool traverse(InitializerLists* input) { return static_cast<T*>(this)->traverseInitializerLists(input); }
                    inline bool visitInitializerLists(InitializerLists* input) { return true; }
                    inline bool endInitializerLists(InitializerLists* input) { return true; }
                    inline bool visit(InitializerList* input) { return static_cast<T*>(this)->visitInitializerList(input); }
                    inline bool end(InitializerList* input) { return static_cast<T*>(this)->endInitializerList(input); }
                    inline bool traverse(InitializerList* input) { return static_cast<T*>(this)->traverseInitializerList(input); }
                    inline bool visitInitializerList(InitializerList* input) { return true; }
                    inline bool endInitializerList(InitializerList* input) { return true; }
                    bool traverseInitializerLists(InitializerLists* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseInitializerList(InitializerList* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_AssignExpressionOrInitializerLists) if(!traverse(input->m_AssignExpressionOrInitializerLists)) return false;
                        return end(input);
                    }
                    inline bool visit(DefaultStatements* input) { return static_cast<T*>(this)->visitDefaultStatements(input); }
                    inline bool end(DefaultStatements* input) { return static_cast<T*>(this)->endDefaultStatements(input); }
                    inline bool traverse(DefaultStatements* input) { return static_cast<T*>(this)->traverseDefaultStatements(input); }
                    inline bool visitDefaultStatements(DefaultStatements* input) { return true; }
                    inline bool endDefaultStatements(DefaultStatements* input) { return true; }
                    inline bool visit(DefaultStatement* input) { return static_cast<T*>(this)->visitDefaultStatement(input); }
                    inline bool end(DefaultStatement* input) { return static_cast<T*>(this)->endDefaultStatement(input); }
                    inline bool traverse(DefaultStatement* input) { return static_cast<T*>(this)->traverseDefaultStatement(input); }
                    inline bool visitDefaultStatement(DefaultStatement* input) { return true; }
                    inline bool endDefaultStatement(DefaultStatement* input) { return true; }
                    bool traverseDefaultStatements(DefaultStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseDefaultStatement(DefaultStatement* input)
                    {
                        assert(input); 
                        return visit(input) && end(input);
                    }
                    inline bool visit(ArrayAccesses* input) { return static_cast<T*>(this)->visitArrayAccesses(input); }
                    inline bool end(ArrayAccesses* input) { return static_cast<T*>(this)->endArrayAccesses(input); }
                    inline bool traverse(ArrayAccesses* input) { return static_cast<T*>(this)->traverseArrayAccesses(input); }
                    inline bool visitArrayAccesses(ArrayAccesses* input) { return true; }
                    inline bool endArrayAccesses(ArrayAccesses* input) { return true; }
                    inline bool visit(ArrayAccess* input) { return static_cast<T*>(this)->visitArrayAccess(input); }
                    inline bool end(ArrayAccess* input) { return static_cast<T*>(this)->endArrayAccess(input); }
                    inline bool traverse(ArrayAccess* input) { return static_cast<T*>(this)->traverseArrayAccess(input); }
                    inline bool visitArrayAccess(ArrayAccess* input) { return true; }
                    inline bool endArrayAccess(ArrayAccess* input) { return true; }
                    bool traverseArrayAccesses(ArrayAccesses* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseArrayAccess(ArrayAccess* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        return end(input);
                    }
                    inline bool visit(KeywordExpressions* input) { return static_cast<T*>(this)->visitKeywordExpressions(input); }
                    inline bool end(KeywordExpressions* input) { return static_cast<T*>(this)->endKeywordExpressions(input); }
                    inline bool traverse(KeywordExpressions* input) { return static_cast<T*>(this)->traverseKeywordExpressions(input); }
                    inline bool visitKeywordExpressions(KeywordExpressions* input) { return true; }
                    inline bool endKeywordExpressions(KeywordExpressions* input) { return true; }
                    inline bool visit(KeywordExpression* input) { return static_cast<T*>(this)->visitKeywordExpression(input); }
                    inline bool end(KeywordExpression* input) { return static_cast<T*>(this)->endKeywordExpression(input); }
                    inline bool traverse(KeywordExpression* input) { return static_cast<T*>(this)->traverseKeywordExpression(input); }
                    inline bool visitKeywordExpression(KeywordExpression* input) { return true; }
                    inline bool endKeywordExpression(KeywordExpression* input) { return true; }
                    bool traverseKeywordExpressions(KeywordExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseKeywordExpression(KeywordExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(NullptrExpressions* input) { return static_cast<T*>(this)->visitNullptrExpressions(input); }
                    inline bool end(NullptrExpressions* input) { return static_cast<T*>(this)->endNullptrExpressions(input); }
                    inline bool traverse(NullptrExpressions* input) { return static_cast<T*>(this)->traverseNullptrExpressions(input); }
                    inline bool visitNullptrExpressions(NullptrExpressions* input) { return true; }
                    inline bool endNullptrExpressions(NullptrExpressions* input) { return true; }
                    inline bool visit(NullptrExpression* input) { return static_cast<T*>(this)->visitNullptrExpression(input); }
                    inline bool end(NullptrExpression* input) { return static_cast<T*>(this)->endNullptrExpression(input); }
                    inline bool traverse(NullptrExpression* input) { return static_cast<T*>(this)->traverseNullptrExpression(input); }
                    inline bool visitNullptrExpression(NullptrExpression* input) { return true; }
                    inline bool endNullptrExpression(NullptrExpression* input) { return true; }
                    bool traverseNullptrExpressions(NullptrExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseNullptrExpression(NullptrExpression* input)
                    {
                        assert(input); 
                        return visit(input) && end(input);
                    }
                    inline bool visit(PostFixExpressions* input) { return static_cast<T*>(this)->visitPostFixExpressions(input); }
                    inline bool end(PostFixExpressions* input) { return static_cast<T*>(this)->endPostFixExpressions(input); }
                    inline bool traverse(PostFixExpressions* input) { return static_cast<T*>(this)->traversePostFixExpressions(input); }
                    inline bool visitPostFixExpressions(PostFixExpressions* input) { return true; }
                    inline bool endPostFixExpressions(PostFixExpressions* input) { return true; }
                    inline bool visit(PostFixExpression* input) { return static_cast<T*>(this)->visitPostFixExpression(input); }
                    inline bool end(PostFixExpression* input) { return static_cast<T*>(this)->endPostFixExpression(input); }
                    inline bool traverse(PostFixExpression* input) { return static_cast<T*>(this)->traversePostFixExpression(input); }
                    inline bool visitPostFixExpression(PostFixExpression* input) { return true; }
                    inline bool endPostFixExpression(PostFixExpression* input) { return true; }
                    bool traversePostFixExpressions(PostFixExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traversePostFixExpression(PostFixExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        if (input->m_PostFixEnds) if(!traverse(input->m_PostFixEnds)) return false;
                        return end(input);
                    }
                    inline bool visit(AutoLocalVariableDecls* input) { return static_cast<T*>(this)->visitAutoLocalVariableDecls(input); }
                    inline bool end(AutoLocalVariableDecls* input) { return static_cast<T*>(this)->endAutoLocalVariableDecls(input); }
                    inline bool traverse(AutoLocalVariableDecls* input) { return static_cast<T*>(this)->traverseAutoLocalVariableDecls(input); }
                    inline bool visitAutoLocalVariableDecls(AutoLocalVariableDecls* input) { return true; }
                    inline bool endAutoLocalVariableDecls(AutoLocalVariableDecls* input) { return true; }
                    inline bool visit(AutoLocalVariableDecl* input) { return static_cast<T*>(this)->visitAutoLocalVariableDecl(input); }
                    inline bool end(AutoLocalVariableDecl* input) { return static_cast<T*>(this)->endAutoLocalVariableDecl(input); }
                    inline bool traverse(AutoLocalVariableDecl* input) { return static_cast<T*>(this)->traverseAutoLocalVariableDecl(input); }
                    inline bool visitAutoLocalVariableDecl(AutoLocalVariableDecl* input) { return true; }
                    inline bool endAutoLocalVariableDecl(AutoLocalVariableDecl* input) { return true; }
                    bool traverseAutoLocalVariableDecls(AutoLocalVariableDecls* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAutoLocalVariableDecl(AutoLocalVariableDecl* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Auto) if(!traverse(input->m_Auto)) return false;
                        if (input->m_AutoDeclarators) if(!traverse(input->m_AutoDeclarators)) return false;
                        return end(input);
                    }
                    inline bool visit(CStyleCastExpressions* input) { return static_cast<T*>(this)->visitCStyleCastExpressions(input); }
                    inline bool end(CStyleCastExpressions* input) { return static_cast<T*>(this)->endCStyleCastExpressions(input); }
                    inline bool traverse(CStyleCastExpressions* input) { return static_cast<T*>(this)->traverseCStyleCastExpressions(input); }
                    inline bool visitCStyleCastExpressions(CStyleCastExpressions* input) { return true; }
                    inline bool endCStyleCastExpressions(CStyleCastExpressions* input) { return true; }
                    inline bool visit(CStyleCastExpression* input) { return static_cast<T*>(this)->visitCStyleCastExpression(input); }
                    inline bool end(CStyleCastExpression* input) { return static_cast<T*>(this)->endCStyleCastExpression(input); }
                    inline bool traverse(CStyleCastExpression* input) { return static_cast<T*>(this)->traverseCStyleCastExpression(input); }
                    inline bool visitCStyleCastExpression(CStyleCastExpression* input) { return true; }
                    inline bool endCStyleCastExpression(CStyleCastExpression* input) { return true; }
                    bool traverseCStyleCastExpressions(CStyleCastExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseCStyleCastExpression(CStyleCastExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Expression) if(!traverse(input->m_Expression)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        if (input->m_Casted) if(!traverse(input->m_Casted)) return false;
                        return end(input);
                    }
                    inline bool visit(TypeTraitBinaries* input) { return static_cast<T*>(this)->visitTypeTraitBinaries(input); }
                    inline bool end(TypeTraitBinaries* input) { return static_cast<T*>(this)->endTypeTraitBinaries(input); }
                    inline bool traverse(TypeTraitBinaries* input) { return static_cast<T*>(this)->traverseTypeTraitBinaries(input); }
                    inline bool visitTypeTraitBinaries(TypeTraitBinaries* input) { return true; }
                    inline bool endTypeTraitBinaries(TypeTraitBinaries* input) { return true; }
                    inline bool visit(TypeTraitBinary* input) { return static_cast<T*>(this)->visitTypeTraitBinary(input); }
                    inline bool end(TypeTraitBinary* input) { return static_cast<T*>(this)->endTypeTraitBinary(input); }
                    inline bool traverse(TypeTraitBinary* input) { return static_cast<T*>(this)->traverseTypeTraitBinary(input); }
                    inline bool visitTypeTraitBinary(TypeTraitBinary* input) { return true; }
                    inline bool endTypeTraitBinary(TypeTraitBinary* input) { return true; }
                    bool traverseTypeTraitBinaries(TypeTraitBinaries* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTypeTraitBinary(TypeTraitBinary* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        if (input->m_Type0) if(!traverse(input->m_Type0)) return false;
                        return end(input);
                    }
                    inline bool visit(LabelStatements* input) { return static_cast<T*>(this)->visitLabelStatements(input); }
                    inline bool end(LabelStatements* input) { return static_cast<T*>(this)->endLabelStatements(input); }
                    inline bool traverse(LabelStatements* input) { return static_cast<T*>(this)->traverseLabelStatements(input); }
                    inline bool visitLabelStatements(LabelStatements* input) { return true; }
                    inline bool endLabelStatements(LabelStatements* input) { return true; }
                    inline bool visit(LabelStatement* input) { return static_cast<T*>(this)->visitLabelStatement(input); }
                    inline bool end(LabelStatement* input) { return static_cast<T*>(this)->endLabelStatement(input); }
                    inline bool traverse(LabelStatement* input) { return static_cast<T*>(this)->traverseLabelStatement(input); }
                    inline bool visitLabelStatement(LabelStatement* input) { return true; }
                    inline bool endLabelStatement(LabelStatement* input) { return true; }
                    bool traverseLabelStatements(LabelStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLabelStatement(LabelStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(AssertStatements* input) { return static_cast<T*>(this)->visitAssertStatements(input); }
                    inline bool end(AssertStatements* input) { return static_cast<T*>(this)->endAssertStatements(input); }
                    inline bool traverse(AssertStatements* input) { return static_cast<T*>(this)->traverseAssertStatements(input); }
                    inline bool visitAssertStatements(AssertStatements* input) { return true; }
                    inline bool endAssertStatements(AssertStatements* input) { return true; }
                    inline bool visit(AssertStatement* input) { return static_cast<T*>(this)->visitAssertStatement(input); }
                    inline bool end(AssertStatement* input) { return static_cast<T*>(this)->endAssertStatement(input); }
                    inline bool traverse(AssertStatement* input) { return static_cast<T*>(this)->traverseAssertStatement(input); }
                    inline bool visitAssertStatement(AssertStatement* input) { return true; }
                    inline bool endAssertStatement(AssertStatement* input) { return true; }
                    bool traverseAssertStatements(AssertStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseAssertStatement(AssertStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_AssignExpression) if(!traverse(input->m_AssignExpression)) return false;
                        if (input->m_AssignExpressions) if(!traverse(input->m_AssignExpressions)) return false;
                        return end(input);
                    }
                    inline bool visit(BreakStatements* input) { return static_cast<T*>(this)->visitBreakStatements(input); }
                    inline bool end(BreakStatements* input) { return static_cast<T*>(this)->endBreakStatements(input); }
                    inline bool traverse(BreakStatements* input) { return static_cast<T*>(this)->traverseBreakStatements(input); }
                    inline bool visitBreakStatements(BreakStatements* input) { return true; }
                    inline bool endBreakStatements(BreakStatements* input) { return true; }
                    inline bool visit(BreakStatement* input) { return static_cast<T*>(this)->visitBreakStatement(input); }
                    inline bool end(BreakStatement* input) { return static_cast<T*>(this)->endBreakStatement(input); }
                    inline bool traverse(BreakStatement* input) { return static_cast<T*>(this)->traverseBreakStatement(input); }
                    inline bool visitBreakStatement(BreakStatement* input) { return true; }
                    inline bool endBreakStatement(BreakStatement* input) { return true; }
                    bool traverseBreakStatements(BreakStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseBreakStatement(BreakStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(ContinueStatements* input) { return static_cast<T*>(this)->visitContinueStatements(input); }
                    inline bool end(ContinueStatements* input) { return static_cast<T*>(this)->endContinueStatements(input); }
                    inline bool traverse(ContinueStatements* input) { return static_cast<T*>(this)->traverseContinueStatements(input); }
                    inline bool visitContinueStatements(ContinueStatements* input) { return true; }
                    inline bool endContinueStatements(ContinueStatements* input) { return true; }
                    inline bool visit(ContinueStatement* input) { return static_cast<T*>(this)->visitContinueStatement(input); }
                    inline bool end(ContinueStatement* input) { return static_cast<T*>(this)->endContinueStatement(input); }
                    inline bool traverse(ContinueStatement* input) { return static_cast<T*>(this)->traverseContinueStatement(input); }
                    inline bool visitContinueStatement(ContinueStatement* input) { return true; }
                    inline bool endContinueStatement(ContinueStatement* input) { return true; }
                    bool traverseContinueStatements(ContinueStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseContinueStatement(ContinueStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        return end(input);
                    }
                    inline bool visit(SizeofExpressions* input) { return static_cast<T*>(this)->visitSizeofExpressions(input); }
                    inline bool end(SizeofExpressions* input) { return static_cast<T*>(this)->endSizeofExpressions(input); }
                    inline bool traverse(SizeofExpressions* input) { return static_cast<T*>(this)->traverseSizeofExpressions(input); }
                    inline bool visitSizeofExpressions(SizeofExpressions* input) { return true; }
                    inline bool endSizeofExpressions(SizeofExpressions* input) { return true; }
                    inline bool visit(SizeofExpression* input) { return static_cast<T*>(this)->visitSizeofExpression(input); }
                    inline bool end(SizeofExpression* input) { return static_cast<T*>(this)->endSizeofExpression(input); }
                    inline bool traverse(SizeofExpression* input) { return static_cast<T*>(this)->traverseSizeofExpression(input); }
                    inline bool visitSizeofExpression(SizeofExpression* input) { return true; }
                    inline bool endSizeofExpression(SizeofExpression* input) { return true; }
                    bool traverseSizeofExpressions(SizeofExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseSizeofExpression(SizeofExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        return end(input);
                    }
                    inline bool visit(TypeofExpressions* input) { return static_cast<T*>(this)->visitTypeofExpressions(input); }
                    inline bool end(TypeofExpressions* input) { return static_cast<T*>(this)->endTypeofExpressions(input); }
                    inline bool traverse(TypeofExpressions* input) { return static_cast<T*>(this)->traverseTypeofExpressions(input); }
                    inline bool visitTypeofExpressions(TypeofExpressions* input) { return true; }
                    inline bool endTypeofExpressions(TypeofExpressions* input) { return true; }
                    inline bool visit(TypeofExpression* input) { return static_cast<T*>(this)->visitTypeofExpression(input); }
                    inline bool end(TypeofExpression* input) { return static_cast<T*>(this)->endTypeofExpression(input); }
                    inline bool traverse(TypeofExpression* input) { return static_cast<T*>(this)->traverseTypeofExpression(input); }
                    inline bool visitTypeofExpression(TypeofExpression* input) { return true; }
                    inline bool endTypeofExpression(TypeofExpression* input) { return true; }
                    bool traverseTypeofExpressions(TypeofExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseTypeofExpression(TypeofExpression* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_Type) if(!traverse(input->m_Type)) return false;
                        return end(input);
                    }
                    inline bool visit(LocalVariableStatements* input) { return static_cast<T*>(this)->visitLocalVariableStatements(input); }
                    inline bool end(LocalVariableStatements* input) { return static_cast<T*>(this)->endLocalVariableStatements(input); }
                    inline bool traverse(LocalVariableStatements* input) { return static_cast<T*>(this)->traverseLocalVariableStatements(input); }
                    inline bool visitLocalVariableStatements(LocalVariableStatements* input) { return true; }
                    inline bool endLocalVariableStatements(LocalVariableStatements* input) { return true; }
                    inline bool visit(LocalVariableStatement* input) { return static_cast<T*>(this)->visitLocalVariableStatement(input); }
                    inline bool end(LocalVariableStatement* input) { return static_cast<T*>(this)->endLocalVariableStatement(input); }
                    inline bool traverse(LocalVariableStatement* input) { return static_cast<T*>(this)->traverseLocalVariableStatement(input); }
                    inline bool visitLocalVariableStatement(LocalVariableStatement* input) { return true; }
                    inline bool endLocalVariableStatement(LocalVariableStatement* input) { return true; }
                    bool traverseLocalVariableStatements(LocalVariableStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLocalVariableStatement(LocalVariableStatement* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_LocalVariableDecl) if(!traverse(input->m_LocalVariableDecl)) return false;
                        return end(input);
                    }
                    inline bool visit(LastTemplateArgumentAssigns* input) { return static_cast<T*>(this)->visitLastTemplateArgumentAssigns(input); }
                    inline bool end(LastTemplateArgumentAssigns* input) { return static_cast<T*>(this)->endLastTemplateArgumentAssigns(input); }
                    inline bool traverse(LastTemplateArgumentAssigns* input) { return static_cast<T*>(this)->traverseLastTemplateArgumentAssigns(input); }
                    inline bool visitLastTemplateArgumentAssigns(LastTemplateArgumentAssigns* input) { return true; }
                    inline bool endLastTemplateArgumentAssigns(LastTemplateArgumentAssigns* input) { return true; }
                    inline bool visit(LastTemplateArgumentAssign* input) { return static_cast<T*>(this)->visitLastTemplateArgumentAssign(input); }
                    inline bool end(LastTemplateArgumentAssign* input) { return static_cast<T*>(this)->endLastTemplateArgumentAssign(input); }
                    inline bool traverse(LastTemplateArgumentAssign* input) { return static_cast<T*>(this)->traverseLastTemplateArgumentAssign(input); }
                    inline bool visitLastTemplateArgumentAssign(LastTemplateArgumentAssign* input) { return true; }
                    inline bool endLastTemplateArgumentAssign(LastTemplateArgumentAssign* input) { return true; }
                    bool traverseLastTemplateArgumentAssigns(LastTemplateArgumentAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLastTemplateArgumentAssign(LastTemplateArgumentAssign* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TemplateArgumentAssign) if(!traverse(input->m_TemplateArgumentAssign)) return false;
                        return end(input);
                    }
                    inline bool visit(LastTemplateArgumentNoAssigns* input) { return static_cast<T*>(this)->visitLastTemplateArgumentNoAssigns(input); }
                    inline bool end(LastTemplateArgumentNoAssigns* input) { return static_cast<T*>(this)->endLastTemplateArgumentNoAssigns(input); }
                    inline bool traverse(LastTemplateArgumentNoAssigns* input) { return static_cast<T*>(this)->traverseLastTemplateArgumentNoAssigns(input); }
                    inline bool visitLastTemplateArgumentNoAssigns(LastTemplateArgumentNoAssigns* input) { return true; }
                    inline bool endLastTemplateArgumentNoAssigns(LastTemplateArgumentNoAssigns* input) { return true; }
                    inline bool visit(LastTemplateArgumentNoAssign* input) { return static_cast<T*>(this)->visitLastTemplateArgumentNoAssign(input); }
                    inline bool end(LastTemplateArgumentNoAssign* input) { return static_cast<T*>(this)->endLastTemplateArgumentNoAssign(input); }
                    inline bool traverse(LastTemplateArgumentNoAssign* input) { return static_cast<T*>(this)->traverseLastTemplateArgumentNoAssign(input); }
                    inline bool visitLastTemplateArgumentNoAssign(LastTemplateArgumentNoAssign* input) { return true; }
                    inline bool endLastTemplateArgumentNoAssign(LastTemplateArgumentNoAssign* input) { return true; }
                    bool traverseLastTemplateArgumentNoAssigns(LastTemplateArgumentNoAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseLastTemplateArgumentNoAssign(LastTemplateArgumentNoAssign* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TemplateArgumentNoAssign) if(!traverse(input->m_TemplateArgumentNoAssign)) return false;
                        return end(input);
                    }
                    inline bool visit(NotLastTemplateArgumentAssigns* input) { return static_cast<T*>(this)->visitNotLastTemplateArgumentAssigns(input); }
                    inline bool end(NotLastTemplateArgumentAssigns* input) { return static_cast<T*>(this)->endNotLastTemplateArgumentAssigns(input); }
                    inline bool traverse(NotLastTemplateArgumentAssigns* input) { return static_cast<T*>(this)->traverseNotLastTemplateArgumentAssigns(input); }
                    inline bool visitNotLastTemplateArgumentAssigns(NotLastTemplateArgumentAssigns* input) { return true; }
                    inline bool endNotLastTemplateArgumentAssigns(NotLastTemplateArgumentAssigns* input) { return true; }
                    inline bool visit(NotLastTemplateArgumentAssign* input) { return static_cast<T*>(this)->visitNotLastTemplateArgumentAssign(input); }
                    inline bool end(NotLastTemplateArgumentAssign* input) { return static_cast<T*>(this)->endNotLastTemplateArgumentAssign(input); }
                    inline bool traverse(NotLastTemplateArgumentAssign* input) { return static_cast<T*>(this)->traverseNotLastTemplateArgumentAssign(input); }
                    inline bool visitNotLastTemplateArgumentAssign(NotLastTemplateArgumentAssign* input) { return true; }
                    inline bool endNotLastTemplateArgumentAssign(NotLastTemplateArgumentAssign* input) { return true; }
                    bool traverseNotLastTemplateArgumentAssigns(NotLastTemplateArgumentAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseNotLastTemplateArgumentAssign(NotLastTemplateArgumentAssign* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TemplateArgumentAssign) if(!traverse(input->m_TemplateArgumentAssign)) return false;
                        return end(input);
                    }
                    inline bool visit(NotLastTemplateArgumentNoAssigns* input) { return static_cast<T*>(this)->visitNotLastTemplateArgumentNoAssigns(input); }
                    inline bool end(NotLastTemplateArgumentNoAssigns* input) { return static_cast<T*>(this)->endNotLastTemplateArgumentNoAssigns(input); }
                    inline bool traverse(NotLastTemplateArgumentNoAssigns* input) { return static_cast<T*>(this)->traverseNotLastTemplateArgumentNoAssigns(input); }
                    inline bool visitNotLastTemplateArgumentNoAssigns(NotLastTemplateArgumentNoAssigns* input) { return true; }
                    inline bool endNotLastTemplateArgumentNoAssigns(NotLastTemplateArgumentNoAssigns* input) { return true; }
                    inline bool visit(NotLastTemplateArgumentNoAssign* input) { return static_cast<T*>(this)->visitNotLastTemplateArgumentNoAssign(input); }
                    inline bool end(NotLastTemplateArgumentNoAssign* input) { return static_cast<T*>(this)->endNotLastTemplateArgumentNoAssign(input); }
                    inline bool traverse(NotLastTemplateArgumentNoAssign* input) { return static_cast<T*>(this)->traverseNotLastTemplateArgumentNoAssign(input); }
                    inline bool visitNotLastTemplateArgumentNoAssign(NotLastTemplateArgumentNoAssign* input) { return true; }
                    inline bool endNotLastTemplateArgumentNoAssign(NotLastTemplateArgumentNoAssign* input) { return true; }
                    bool traverseNotLastTemplateArgumentNoAssigns(NotLastTemplateArgumentNoAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverseNotLastTemplateArgumentNoAssign(NotLastTemplateArgumentNoAssign* input)
                    {
                        assert(input); 
                        if (!visit(input)) return false;
                        if (input->m_TemplateArgumentNoAssign) if(!traverse(input->m_TemplateArgumentNoAssign)) return false;
                        return end(input);
                    }
                };
                template<class OStream>
                struct Dumper : public Recursive<Dumper<OStream>>
                {
                    Dumper(OStream& stream) : stream(stream), indentation(0) {}
                    void line(phantom::StringView text) { spaces(); stream.write(text.data(), text.size()); stream << '\n'; }
                    void indent() { indentation++; }
                    void unindent() { indentation--; }
                    void spaces() { for(int i = 0; i<indentation; ++i) stream << ' '; }
                    int indentation;
                    OStream& stream;
                    bool traverse(Sources* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Source* input)
                    {
                        assert(input); 
                        line("[Source]");
                        indent();
                        if (input->m_ModuleDeclarations) traverse(input->m_ModuleDeclarations);
                        if (input->m_ImportDeclarations) traverse(input->m_ImportDeclarations);
                        if (input->m_SourceDeclarations) traverse(input->m_SourceDeclarations);
                        unindent();
                        return true;
                    }
                    bool traverse(ModuleDeclarations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ModuleDeclaration* input)
                    {
                        assert(input); 
                        line("[ModuleDeclaration]");
                        indent();
                        if(input->m_Names) for(auto v : *input->m_Names)
                        if(v)
                        line(phantom::lexical_cast<String>(v));
                        unindent();
                        return true;
                    }
                    bool traverse(ImportDeclarations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ImportDeclaration* input)
                    {
                        assert(input); 
                        line("[ImportDeclaration]");
                        indent();
                        switch(input->m_STATIC.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_STATIC.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_STATIC.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_STATIC.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_STATIC.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_STATIC.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_STATIC.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_STATIC.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_STATIC.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_STATIC.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_STATIC.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_STATIC.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_STATIC.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_STATIC.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_STATIC.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_PUBLIC.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_PUBLIC.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_PUBLIC.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Imports) traverse(input->m_Imports);
                        switch(input->m_LT.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_LT.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_LT.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_LT.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_LT.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_LT.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_LT.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_LT.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_LT.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_LT.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_LT.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_LT.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_LT.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_LT.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_LT.as_bool())); break;
                            default: assert(false); break;
                        }
                        if(input->m_Names) for(auto v : *input->m_Names)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_GT.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_GT.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_GT.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_GT.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_GT.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_GT.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_GT.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_GT.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_GT.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_GT.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_GT.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_GT.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_GT.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_GT.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_GT.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_STRING_LIT.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_STRING_LIT.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_STRING_LIT.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(Imports* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Import* input)
                    {
                        assert(input); 
                        line("[Import]");
                        indent();
                        if (input->m_ImportBase) traverse(input->m_ImportBase);
                        if (input->m_ImportBindings) traverse(input->m_ImportBindings);
                        unindent();
                        return true;
                    }
                    bool traverse(ImportBindings* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ImportBinding* input)
                    {
                        assert(input); 
                        line("[ImportBinding]");
                        indent();
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        switch(input->m_ASSIGN.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_ASSIGN.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_ASSIGN.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_IDENTIFIER0.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_IDENTIFIER0.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_IDENTIFIER0.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_IDENTIFIER0.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(ImportBases* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ImportBase* input)
                    {
                        assert(input); 
                        line("[ImportBase]");
                        indent();
                        switch(input->m_Alias.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_Alias.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_Alias.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_Alias.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_Alias.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_Alias.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_Alias.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_Alias.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_Alias.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_Alias.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_Alias.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_Alias.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_Alias.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_Alias.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_Alias.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_ASSIGN.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_ASSIGN.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_ASSIGN.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_bool())); break;
                            default: assert(false); break;
                        }
                        if(input->m_Names) for(auto v : *input->m_Names)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(Metas* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Meta* input)
                    {
                        assert(input); 
                        line("[Meta]");
                        indent();
                        if (input->m_MetaDecls) traverse(input->m_MetaDecls);
                        unindent();
                        return true;
                    }
                    bool traverse(MetaDecls* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(MetaDecl* input)
                    {
                        assert(input); 
                        line("[MetaDecl]");
                        indent();
                        if(input->m_Key)
                        line(phantom::lexical_cast<String>(input->m_Key));
                        switch(input->m_ASSIGN.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_ASSIGN.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_ASSIGN.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Value) traverse(input->m_Value);
                        unindent();
                        return true;
                    }
                    bool traverse(Annotations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Annotation* input)
                    {
                        assert(input); 
                        line("[Annotation]");
                        indent();
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        unindent();
                        return true;
                    }
                    bool traverse(MetaOrAnnotations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(MetaOrAnnotation* input)
                    {
                        assert(input); 
                        line("[MetaOrAnnotation]");
                        indent();
                        if (input->m_Meta) traverse(input->m_Meta);
                        if (input->m_Annotation) traverse(input->m_Annotation);
                        unindent();
                        return true;
                    }
                    bool traverse(SourceDeclarations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(SourceDeclaration* input)
                    {
                        assert(input); 
                        line("[SourceDeclaration]");
                        indent();
                        if (input->m_MetaOrAnnotations) traverse(input->m_MetaOrAnnotations);
                        if (input->m_Declaration) traverse(input->m_Declaration);
                        unindent();
                        return true;
                    }
                    bool traverse(Declarations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Declaration* input)
                    {
                        assert(input); 
                        line("[Declaration]");
                        indent();
                        if (input->m_Class) traverse(input->m_Class);
                        if (input->m_Enum) traverse(input->m_Enum);
                        if (input->m_Alias) traverse(input->m_Alias);
                        if (input->m_Using) traverse(input->m_Using);
                        if (input->m_Typedef) traverse(input->m_Typedef);
                        if (input->m_Template) traverse(input->m_Template);
                        if (input->m_MixinStatementDefinition) traverse(input->m_MixinStatementDefinition);
                        if (input->m_StaticAssert) traverse(input->m_StaticAssert);
                        if (input->m_FunctionOrVariable) traverse(input->m_FunctionOrVariable);
                        unindent();
                        return true;
                    }
                    bool traverse(MixinStatementDefinitions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(MixinStatementDefinition* input)
                    {
                        assert(input); 
                        line("[MixinStatementDefinition]");
                        indent();
                        if(input->m_ATAT)
                        line(phantom::lexical_cast<String>(input->m_ATAT));
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if(input->m_IDENTIFIERs) for(auto v : *input->m_IDENTIFIERs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Statements) traverse(input->m_Statements);
                        unindent();
                        return true;
                    }
                    bool traverse(ForeachSignatures* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ForeachSignature* input)
                    {
                        assert(input); 
                        line("[ForeachSignature]");
                        indent();
                        if (input->m_TypeOrAuto) traverse(input->m_TypeOrAuto);
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(ConversionFunctions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ConversionFunction* input)
                    {
                        assert(input); 
                        line("[ConversionFunction]");
                        indent();
                        if (input->m_TypeNoFunctionType) traverse(input->m_TypeNoFunctionType);
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if(input->m_CONSTs) for(auto v : *input->m_CONSTs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_FunctionBlock) traverse(input->m_FunctionBlock);
                        unindent();
                        return true;
                    }
                    bool traverse(Constructors* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Constructor* input)
                    {
                        assert(input); 
                        line("[Constructor]");
                        indent();
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_TemplateArgumentList) traverse(input->m_TemplateArgumentList);
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Parameters) traverse(input->m_Parameters);
                        switch(input->m_Suffix.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_Suffix.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_Suffix.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_Suffix.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_Suffix.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_Suffix.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_Suffix.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_Suffix.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_Suffix.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_Suffix.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_Suffix.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_Suffix.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_Suffix.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_Suffix.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_Suffix.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_ConstructorInitializers) traverse(input->m_ConstructorInitializers);
                        if (input->m_FunctionBlock) traverse(input->m_FunctionBlock);
                        unindent();
                        return true;
                    }
                    bool traverse(Destructors* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Destructor* input)
                    {
                        assert(input); 
                        line("[Destructor]");
                        indent();
                        switch(input->m_VIRTUAL.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_VIRTUAL.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_VIRTUAL.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_VIRTUAL.as_bool())); break;
                            default: assert(false); break;
                        }
                        if(input->m_COMPL)
                        line(phantom::lexical_cast<String>(input->m_COMPL));
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if(input->m_OVERRIDEs) for(auto v : *input->m_OVERRIDEs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_FunctionBlock) traverse(input->m_FunctionBlock);
                        unindent();
                        return true;
                    }
                    bool traverse(ConstructorInitializers* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ConstructorInitializer* input)
                    {
                        assert(input); 
                        line("[ConstructorInitializer]");
                        indent();
                        if (input->m_BasicType) traverse(input->m_BasicType);
                        if (input->m_CallList) traverse(input->m_CallList);
                        if (input->m_InitializerList) traverse(input->m_InitializerList);
                        unindent();
                        return true;
                    }
                    bool traverse(ArrowReturns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ArrowReturn* input)
                    {
                        assert(input); 
                        line("[ArrowReturn]");
                        indent();
                        if(input->m_ARROW)
                        line(phantom::lexical_cast<String>(input->m_ARROW));
                        if (input->m_Type) traverse(input->m_Type);
                        unindent();
                        return true;
                    }
                    bool traverse(FunctionEnds* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(FunctionEnd* input)
                    {
                        assert(input); 
                        line("[FunctionEnd]");
                        indent();
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Parameters) traverse(input->m_Parameters);
                        if(input->m_CONSTs) for(auto v : *input->m_CONSTs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_ArrowReturn) traverse(input->m_ArrowReturn);
                        if(input->m_OVERRIDEs) for(auto v : *input->m_OVERRIDEs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_FunctionBlock) traverse(input->m_FunctionBlock);
                        switch(input->m_Suffix.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_Suffix.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_Suffix.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_Suffix.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_Suffix.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_Suffix.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_Suffix.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_Suffix.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_Suffix.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_Suffix.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_Suffix.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_Suffix.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_Suffix.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_Suffix.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_Suffix.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(Operators* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Operator* input)
                    {
                        assert(input); 
                        line("[Operator]");
                        indent();
                        if(input->m_OpParen) for(auto v : *input->m_OpParen)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if(input->m_OpBracket) for(auto v : *input->m_OpBracket)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if(input->m_RightShiftAssign) for(auto v : *input->m_RightShiftAssign)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if(input->m_RightShift) for(auto v : *input->m_RightShift)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_GreaterThan.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_GreaterThan.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_GreaterThan.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_GreaterThan.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_ASSIGN.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_ASSIGN.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_ASSIGN.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(OperatorEnds* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(OperatorEnd* input)
                    {
                        assert(input); 
                        line("[OperatorEnd]");
                        indent();
                        if (input->m_Operator) traverse(input->m_Operator);
                        if (input->m_FunctionEnd) traverse(input->m_FunctionEnd);
                        unindent();
                        return true;
                    }
                    bool traverse(FunctionOrVariableEnds* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(FunctionOrVariableEnd* input)
                    {
                        assert(input); 
                        line("[FunctionOrVariableEnd]");
                        indent();
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_InitializerList) traverse(input->m_InitializerList);
                        if (input->m_ArrayExtents) traverse(input->m_ArrayExtents);
                        if (input->m_AssignExpressionOrInitializerList) traverse(input->m_AssignExpressionOrInitializerList);
                        if (input->m_Declarators) traverse(input->m_Declarators);
                        if (input->m_TemplateArgumentList) traverse(input->m_TemplateArgumentList);
                        if (input->m_FunctionEnd) traverse(input->m_FunctionEnd);
                        if (input->m_CallList) traverse(input->m_CallList);
                        unindent();
                        return true;
                    }
                    bool traverse(MethodSpecifiers* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(MethodSpecifier* input)
                    {
                        assert(input); 
                        line("[MethodSpecifier]");
                        indent();
                        if(input->m_STATIC)
                        line(phantom::lexical_cast<String>(input->m_STATIC));
                        unindent();
                        return true;
                    }
                    bool traverse(Specifiers* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Specifier* input)
                    {
                        assert(input); 
                        line("[Specifier]");
                        indent();
                        if (input->m_MethodSpecifier) traverse(input->m_MethodSpecifier);
                        unindent();
                        return true;
                    }
                    bool traverse(FunctionOrVariables* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(FunctionOrVariable* input)
                    {
                        assert(input); 
                        line("[FunctionOrVariable]");
                        indent();
                        if (input->m_Specifiers) traverse(input->m_Specifiers);
                        switch(input->m_PAREN_START.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_PAREN_START.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_PAREN_START.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_AlignAs.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_AlignAs.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_AlignAs.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_AlignAs.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_AlignAs.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_AlignAs.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_AlignAs.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_AlignAs.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_AlignAs.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_AlignAs.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_AlignAs.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_AlignAs.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_AlignAs.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_AlignAs.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_AlignAs.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_TypeOrAuto) traverse(input->m_TypeOrAuto);
                        if (input->m_OperatorEnd) traverse(input->m_OperatorEnd);
                        if (input->m_FunctionOrVariableEnd) traverse(input->m_FunctionOrVariableEnd);
                        unindent();
                        return true;
                    }
                    bool traverse(DeclTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(DeclType* input)
                    {
                        assert(input); 
                        line("[DeclType]");
                        indent();
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(TemplateArguments* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TemplateArgument* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_LastTemplateArgument: return traverse(static_cast<LastTemplateArgument*>(input));
                            case e_RuleType_NotLastTemplateArgument: return traverse(static_cast<NotLastTemplateArgument*>(input));
                            default:
                            {
                                line("[TemplateArgument]");
                                indent();
                            }
                            unindent();
                        }
                        return true;
                    }
                    bool traverse(ForInits* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ForInit* input)
                    {
                        assert(input); 
                        line("[ForInit]");
                        indent();
                        if (input->m_LocalVariableDecl) traverse(input->m_LocalVariableDecl);
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(Statements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Statement* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_BlockDeclaration: return traverse(static_cast<BlockDeclaration*>(input));
                            case e_RuleType_StatementOrCase: return traverse(static_cast<StatementOrCase*>(input));
                            case e_RuleType_IfStatement: return traverse(static_cast<IfStatement*>(input));
                            case e_RuleType_CaseStatement: return traverse(static_cast<CaseStatement*>(input));
                            case e_RuleType_DefaultStatement: return traverse(static_cast<DefaultStatement*>(input));
                            case e_RuleType_LabelStatement: return traverse(static_cast<LabelStatement*>(input));
                            case e_RuleType_SwitchStatement: return traverse(static_cast<SwitchStatement*>(input));
                            case e_RuleType_ForStatement: return traverse(static_cast<ForStatement*>(input));
                            case e_RuleType_WhileStatement: return traverse(static_cast<WhileStatement*>(input));
                            case e_RuleType_BlockStatement: return traverse(static_cast<BlockStatement*>(input));
                            case e_RuleType_DoWhileStatement: return traverse(static_cast<DoWhileStatement*>(input));
                            case e_RuleType_LocalVariableStatement: return traverse(static_cast<LocalVariableStatement*>(input));
                            case e_RuleType_ExpressionStatement: return traverse(static_cast<ExpressionStatement*>(input));
                            case e_RuleType_AssertStatement: return traverse(static_cast<AssertStatement*>(input));
                            case e_RuleType_BreakStatement: return traverse(static_cast<BreakStatement*>(input));
                            case e_RuleType_ContinueStatement: return traverse(static_cast<ContinueStatement*>(input));
                            default:
                            {
                                line("[Statement]");
                                indent();
                                if (input->m_IfStatement) traverse(input->m_IfStatement);
                                if (input->m_ForStatement) traverse(input->m_ForStatement);
                                if (input->m_SwitchStatement) traverse(input->m_SwitchStatement);
                                if (input->m_WhileStatement) traverse(input->m_WhileStatement);
                                if (input->m_AssertStatement) traverse(input->m_AssertStatement);
                                if (input->m_BreakStatement) traverse(input->m_BreakStatement);
                                if (input->m_ContinueStatement) traverse(input->m_ContinueStatement);
                                if (input->m_DoWhileStatement) traverse(input->m_DoWhileStatement);
                                if (input->m_BlockStatement) traverse(input->m_BlockStatement);
                                if (input->m_ReturnStatement) traverse(input->m_ReturnStatement);
                                if (input->m_GotoStatement) traverse(input->m_GotoStatement);
                                if (input->m_BlockDeclaration) traverse(input->m_BlockDeclaration);
                                if (input->m_MixinStatement) traverse(input->m_MixinStatement);
                                if (input->m_LabelStatement) traverse(input->m_LabelStatement);
                                if (input->m_LocalVariableStatement) traverse(input->m_LocalVariableStatement);
                                if (input->m_ExpressionStatement) traverse(input->m_ExpressionStatement);
                            }
                            unindent();
                        }
                        return true;
                    }
                    bool traverse(MixinStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(MixinStatement* input)
                    {
                        assert(input); 
                        line("[MixinStatement]");
                        indent();
                        if(input->m_ATAT)
                        line(phantom::lexical_cast<String>(input->m_ATAT));
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Expressions) traverse(input->m_Expressions);
                        unindent();
                        return true;
                    }
                    bool traverse(ReturnStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ReturnStatement* input)
                    {
                        assert(input); 
                        line("[ReturnStatement]");
                        indent();
                        if (input->m_pExpression) traverse(input->m_pExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(GotoStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(GotoStatement* input)
                    {
                        assert(input); 
                        line("[GotoStatement]");
                        indent();
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        unindent();
                        return true;
                    }
                    bool traverse(Declarators* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Declarator* input)
                    {
                        assert(input); 
                        line("[Declarator]");
                        indent();
                        if (input->m_TypeExtents) traverse(input->m_TypeExtents);
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_ArrayExtents) traverse(input->m_ArrayExtents);
                        if (input->m_AssignExpressionOrInitializerList) traverse(input->m_AssignExpressionOrInitializerList);
                        if (input->m_CallList) traverse(input->m_CallList);
                        if (input->m_InitializerList) traverse(input->m_InitializerList);
                        unindent();
                        return true;
                    }
                    bool traverse(TemplateArgumentListSplits* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TemplateArgumentListSplit* input)
                    {
                        assert(input); 
                        line("[TemplateArgumentListSplit]");
                        indent();
                        if (input->m_NotLastTemplateArguments) traverse(input->m_NotLastTemplateArguments);
                        if (input->m_LastTemplateArgument) traverse(input->m_LastTemplateArgument);
                        unindent();
                        return true;
                    }
                    bool traverse(TemplateArgumentNoAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TemplateArgumentNoAssign* input)
                    {
                        assert(input); 
                        line("[TemplateArgumentNoAssign]");
                        indent();
                        if (input->m_PostFixExpression) traverse(input->m_PostFixExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(TemplateArgumentAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TemplateArgumentAssign* input)
                    {
                        assert(input); 
                        line("[TemplateArgumentAssign]");
                        indent();
                        switch(input->m_INT_VALUE.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_INT_VALUE.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_INT_VALUE.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Type) traverse(input->m_Type);
                        unindent();
                        return true;
                    }
                    bool traverse(TemplateArgumentLists* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TemplateArgumentList* input)
                    {
                        assert(input); 
                        line("[TemplateArgumentList]");
                        indent();
                        switch(input->m_GT.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_GT.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_GT.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_GT.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_GT.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_GT.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_GT.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_GT.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_GT.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_GT.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_GT.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_GT.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_GT.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_GT.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_GT.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_TemplateArgumentListSplit) traverse(input->m_TemplateArgumentListSplit);
                        unindent();
                        return true;
                    }
                    bool traverse(Names* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Name* input)
                    {
                        assert(input); 
                        line("[Name]");
                        indent();
                        if (input->m_Operator) traverse(input->m_Operator);
                        switch(input->m_COMPL.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_COMPL.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_COMPL.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_COMPL.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_COMPL.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_COMPL.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_COMPL.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_COMPL.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_COMPL.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_COMPL.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_COMPL.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_COMPL.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_COMPL.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_COMPL.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_COMPL.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_IDENTIFIER.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_IDENTIFIER.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_IDENTIFIER.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_TemplateArgumentList) traverse(input->m_TemplateArgumentList);
                        unindent();
                        return true;
                    }
                    bool traverse(FundamentalTypeBases* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(FundamentalTypeBase* input)
                    {
                        assert(input); 
                        line("[FundamentalTypeBase]");
                        indent();
                        if(input->m_Typename)
                        line(phantom::lexical_cast<String>(input->m_Typename));
                        unindent();
                        return true;
                    }
                    bool traverse(AutoDeclarators* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(AutoDeclarator* input)
                    {
                        assert(input); 
                        line("[AutoDeclarator]");
                        indent();
                        if (input->m_TypeExtents) traverse(input->m_TypeExtents);
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_CallList) traverse(input->m_CallList);
                        if (input->m_InitializerList) traverse(input->m_InitializerList);
                        if (input->m_ArrayExtents) traverse(input->m_ArrayExtents);
                        if (input->m_AssignExpressionOrInitializerList) traverse(input->m_AssignExpressionOrInitializerList);
                        unindent();
                        return true;
                    }
                    bool traverse(LocalVariableDecls* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LocalVariableDecl* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_ExplicitLocalVariableDecl: return traverse(static_cast<ExplicitLocalVariableDecl*>(input));
                            case e_RuleType_AutoLocalVariableDecl: return traverse(static_cast<AutoLocalVariableDecl*>(input));
                            default:
                            {
                                line("[LocalVariableDecl]");
                                indent();
                                if (input->m_AutoLocalVariableDecl) traverse(input->m_AutoLocalVariableDecl);
                                if (input->m_ExplicitLocalVariableDecl) traverse(input->m_ExplicitLocalVariableDecl);
                            }
                            unindent();
                        }
                        return true;
                    }
                    bool traverse(Expressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Expression* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_IntegralConstant: return traverse(static_cast<IntegralConstant*>(input));
                            case e_RuleType_AssignExpression: return traverse(static_cast<AssignExpression*>(input));
                            case e_RuleType_ConditionalExpression: return traverse(static_cast<ConditionalExpression*>(input));
                            case e_RuleType_LogicalOrExpression: return traverse(static_cast<LogicalOrExpression*>(input));
                            case e_RuleType_LogicalAndExpression: return traverse(static_cast<LogicalAndExpression*>(input));
                            case e_RuleType_BinOrExpression: return traverse(static_cast<BinOrExpression*>(input));
                            case e_RuleType_XOrExpression: return traverse(static_cast<XOrExpression*>(input));
                            case e_RuleType_BinAndExpression: return traverse(static_cast<BinAndExpression*>(input));
                            case e_RuleType_EqualityExpression: return traverse(static_cast<EqualityExpression*>(input));
                            case e_RuleType_RelationalExpression: return traverse(static_cast<RelationalExpression*>(input));
                            case e_RuleType_ShiftExpression: return traverse(static_cast<ShiftExpression*>(input));
                            case e_RuleType_AddExpression: return traverse(static_cast<AddExpression*>(input));
                            case e_RuleType_MulExpression: return traverse(static_cast<MulExpression*>(input));
                            case e_RuleType_UnaryExpression: return traverse(static_cast<UnaryExpression*>(input));
                            case e_RuleType_AssignExpressionOrInitializerList: return traverse(static_cast<AssignExpressionOrInitializerList*>(input));
                            case e_RuleType_ExpressionOrInitializerList: return traverse(static_cast<ExpressionOrInitializerList*>(input));
                            case e_RuleType_InitializerList: return traverse(static_cast<InitializerList*>(input));
                            case e_RuleType_ArrayAccess: return traverse(static_cast<ArrayAccess*>(input));
                            case e_RuleType_KeywordExpression: return traverse(static_cast<KeywordExpression*>(input));
                            case e_RuleType_NullptrExpression: return traverse(static_cast<NullptrExpression*>(input));
                            case e_RuleType_PostFixExpression: return traverse(static_cast<PostFixExpression*>(input));
                            case e_RuleType_CStyleCastExpression: return traverse(static_cast<CStyleCastExpression*>(input));
                            case e_RuleType_TypeTraitBinary: return traverse(static_cast<TypeTraitBinary*>(input));
                            case e_RuleType_SizeofExpression: return traverse(static_cast<SizeofExpression*>(input));
                            case e_RuleType_TypeofExpression: return traverse(static_cast<TypeofExpression*>(input));
                            case e_RuleType_FundamentalTypeFunctionCast: return traverse(static_cast<FundamentalTypeFunctionCast*>(input));
                            case e_RuleType_PrimaryExpression: return traverse(static_cast<PrimaryExpression*>(input));
                            default:
                            {
                                line("[Expression]");
                                indent();
                                if (input->m_Expression_AssignExpression) traverse(input->m_Expression_AssignExpression);
                                if (input->m_Expression_Expression) traverse(input->m_Expression_Expression);
                            }
                            unindent();
                        }
                        return true;
                    }
                    bool traverse(LogicalOrExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LogicalOrExpressionRight* input)
                    {
                        assert(input); 
                        line("[LogicalOrExpressionRight]");
                        indent();
                        if(input->m_Op)
                        line(phantom::lexical_cast<String>(input->m_Op));
                        if (input->m_RightExpression) traverse(input->m_RightExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(LogicalAndExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LogicalAndExpressionRight* input)
                    {
                        assert(input); 
                        line("[LogicalAndExpressionRight]");
                        indent();
                        if(input->m_Op)
                        line(phantom::lexical_cast<String>(input->m_Op));
                        if (input->m_RightExpression) traverse(input->m_RightExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(BinOrExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BinOrExpressionRight* input)
                    {
                        assert(input); 
                        line("[BinOrExpressionRight]");
                        indent();
                        if(input->m_Op)
                        line(phantom::lexical_cast<String>(input->m_Op));
                        if (input->m_RightExpression) traverse(input->m_RightExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(XOrExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(XOrExpressionRight* input)
                    {
                        assert(input); 
                        line("[XOrExpressionRight]");
                        indent();
                        if(input->m_Op)
                        line(phantom::lexical_cast<String>(input->m_Op));
                        if (input->m_RightExpression) traverse(input->m_RightExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(BinAndExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BinAndExpressionRight* input)
                    {
                        assert(input); 
                        line("[BinAndExpressionRight]");
                        indent();
                        if(input->m_Op)
                        line(phantom::lexical_cast<String>(input->m_Op));
                        if (input->m_RightExpression) traverse(input->m_RightExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(EqualityExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(EqualityExpressionRight* input)
                    {
                        assert(input); 
                        line("[EqualityExpressionRight]");
                        indent();
                        if(input->m_Op)
                        line(phantom::lexical_cast<String>(input->m_Op));
                        if (input->m_RightExpression) traverse(input->m_RightExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(RelationalExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(RelationalExpressionRight* input)
                    {
                        assert(input); 
                        line("[RelationalExpressionRight]");
                        indent();
                        if(input->m_OpMul) for(auto v : *input->m_OpMul)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_RightExpression) traverse(input->m_RightExpression);
                        switch(input->m_Op.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_Op.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_Op.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_Op.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_Op.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_Op.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_Op.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_Op.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_Op.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_Op.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_Op.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_Op.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_Op.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_Op.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_Op.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(Types* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Type* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_UnsignedType: return traverse(static_cast<UnsignedType*>(input));
                            case e_RuleType_SignedType: return traverse(static_cast<SignedType*>(input));
                            case e_RuleType_LongType: return traverse(static_cast<LongType*>(input));
                            case e_RuleType_VoidType: return traverse(static_cast<VoidType*>(input));
                            case e_RuleType_FundamentalType: return traverse(static_cast<FundamentalType*>(input));
                            case e_RuleType_QualifiedType: return traverse(static_cast<QualifiedType*>(input));
                            case e_RuleType_ExplicitType: return traverse(static_cast<ExplicitType*>(input));
                            case e_RuleType_TypeNoFunctionType: return traverse(static_cast<TypeNoFunctionType*>(input));
                            case e_RuleType_ExplicitTypeNoFunctionType: return traverse(static_cast<ExplicitTypeNoFunctionType*>(input));
                            case e_RuleType_ExpressionType: return traverse(static_cast<ExpressionType*>(input));
                            case e_RuleType_ExplicitTypeOrAuto: return traverse(static_cast<ExplicitTypeOrAuto*>(input));
                            case e_RuleType_TypeOrAuto: return traverse(static_cast<TypeOrAuto*>(input));
                            case e_RuleType_AutoType: return traverse(static_cast<AutoType*>(input));
                            case e_RuleType_BasicType: return traverse(static_cast<BasicType*>(input));
                            case e_RuleType_Auto: return traverse(static_cast<Auto*>(input));
                            case e_RuleType_BasicTypeOrAuto: return traverse(static_cast<BasicTypeOrAuto*>(input));
                            default:
                            {
                                line("[Type]");
                                indent();
                                if (input->m_ExplicitType) traverse(input->m_ExplicitType);
                                if (input->m_ExpressionType) traverse(input->m_ExpressionType);
                            }
                            unindent();
                        }
                        return true;
                    }
                    bool traverse(ShiftExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ShiftExpressionRight* input)
                    {
                        assert(input); 
                        line("[ShiftExpressionRight]");
                        indent();
                        if(input->m_OpMul) for(auto v : *input->m_OpMul)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_RightExpression) traverse(input->m_RightExpression);
                        switch(input->m_Op.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_Op.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_Op.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_Op.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_Op.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_Op.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_Op.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_Op.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_Op.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_Op.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_Op.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_Op.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_Op.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_Op.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_Op.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(AddExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(AddExpressionRight* input)
                    {
                        assert(input); 
                        line("[AddExpressionRight]");
                        indent();
                        if(input->m_Op)
                        line(phantom::lexical_cast<String>(input->m_Op));
                        if (input->m_RightExpression) traverse(input->m_RightExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(MulExpressionRights* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(MulExpressionRight* input)
                    {
                        assert(input); 
                        line("[MulExpressionRight]");
                        indent();
                        if(input->m_Op)
                        line(phantom::lexical_cast<String>(input->m_Op));
                        if (input->m_RightExpression) traverse(input->m_RightExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(SpecialCastExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(SpecialCastExpression* input)
                    {
                        assert(input); 
                        line("[SpecialCastExpression]");
                        indent();
                        if(input->m_CastKind)
                        line(phantom::lexical_cast<String>(input->m_CastKind));
                        if (input->m_Type) traverse(input->m_Type);
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(FunctionPtrExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(FunctionPtrExpression* input)
                    {
                        assert(input); 
                        line("[FunctionPtrExpression]");
                        indent();
                        if (input->m_Name) traverse(input->m_Name);
                        unindent();
                        return true;
                    }
                    bool traverse(CallLists* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(CallList* input)
                    {
                        assert(input); 
                        line("[CallList]");
                        indent();
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_AssignExpressionOrInitializerLists) traverse(input->m_AssignExpressionOrInitializerLists);
                        unindent();
                        return true;
                    }
                    bool traverse(PostFixEnds* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(PostFixEnd* input)
                    {
                        assert(input); 
                        line("[PostFixEnd]");
                        indent();
                        if (input->m_CallList) traverse(input->m_CallList);
                        if (input->m_ArrayAccess) traverse(input->m_ArrayAccess);
                        if (input->m_InitializerList) traverse(input->m_InitializerList);
                        switch(input->m_DOT.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_DOT.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_DOT.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_DOT.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_DOT.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_DOT.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_DOT.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_DOT.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_DOT.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_DOT.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_DOT.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_DOT.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_DOT.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_DOT.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_DOT.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Name) traverse(input->m_Name);
                        switch(input->m_ELLIPSE.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_ELLIPSE.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_ELLIPSE.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_INC.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_INC.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_INC.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_INC.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_INC.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_INC.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_INC.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_INC.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_INC.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_INC.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_INC.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_INC.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_INC.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_INC.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_INC.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(ArrayExtentExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ArrayExtentExpression* input)
                    {
                        assert(input); 
                        line("[ArrayExtentExpression]");
                        indent();
                        if (input->m_IntegralConstant) traverse(input->m_IntegralConstant);
                        if (input->m_AssignExpression) traverse(input->m_AssignExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(StaticAsserts* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(StaticAssert* input)
                    {
                        assert(input); 
                        line("[StaticAssert]");
                        indent();
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_AssignExpression) traverse(input->m_AssignExpression);
                        switch(input->m_STRING_LIT.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_STRING_LIT.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_STRING_LIT.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(StringLiteralChains* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(StringLiteralChain* input)
                    {
                        assert(input); 
                        line("[StringLiteralChain]");
                        indent();
                        if(input->m_STRING_LIT)
                        line(phantom::lexical_cast<String>(input->m_STRING_LIT));
                        if(input->m_STRING_LITs) for(auto v : *input->m_STRING_LITs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(LambdaCaptures* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LambdaCapture* input)
                    {
                        assert(input); 
                        line("[LambdaCapture]");
                        indent();
                        switch(input->m_IDENTIFIER.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_IDENTIFIER.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_IDENTIFIER.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_ASSIGN.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_ASSIGN.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_ASSIGN.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_BIN_AND.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_BIN_AND.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_BIN_AND.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_BIN_AND.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_THIS.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_THIS.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_THIS.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_THIS.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_THIS.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_THIS.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_THIS.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_THIS.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_THIS.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_THIS.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_THIS.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_THIS.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_THIS.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_THIS.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_THIS.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(LambdaExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LambdaExpression* input)
                    {
                        assert(input); 
                        line("[LambdaExpression]");
                        indent();
                        if (input->m_LambdaCaptures) traverse(input->m_LambdaCaptures);
                        if (input->m_ArrowReturn) traverse(input->m_ArrowReturn);
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Parameters) traverse(input->m_Parameters);
                        if (input->m_FunctionBlock) traverse(input->m_FunctionBlock);
                        unindent();
                        return true;
                    }
                    bool traverse(TypeExtents* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TypeExtent* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_ArrayExtent: return traverse(static_cast<ArrayExtent*>(input));
                            case e_RuleType_FunctionTypeExtent: return traverse(static_cast<FunctionTypeExtent*>(input));
                            case e_RuleType_DerivedExtent: return traverse(static_cast<DerivedExtent*>(input));
                            case e_RuleType_TypeExtentNoFunction: return traverse(static_cast<TypeExtentNoFunction*>(input));
                            default:
                            {
                                line("[TypeExtent]");
                                indent();
                                if (input->m_ArrayExtent) traverse(input->m_ArrayExtent);
                                if (input->m_FunctionTypeExtent) traverse(input->m_FunctionTypeExtent);
                                if (input->m_DerivedExtent) traverse(input->m_DerivedExtent);
                            }
                            unindent();
                        }
                        return true;
                    }
                    bool traverse(Enums* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Enum* input)
                    {
                        assert(input); 
                        line("[Enum]");
                        indent();
                        switch(input->m_CLASS.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_CLASS.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_CLASS.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_CLASS.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_CLASS.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_CLASS.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_CLASS.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_CLASS.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_CLASS.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_CLASS.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_CLASS.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_CLASS.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_CLASS.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_CLASS.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_CLASS.as_bool())); break;
                            default: assert(false); break;
                        }
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_EnumConstants) traverse(input->m_EnumConstants);
                        unindent();
                        return true;
                    }
                    bool traverse(EnumConstants* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(EnumConstant* input)
                    {
                        assert(input); 
                        line("[EnumConstant]");
                        indent();
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_AssignExpression) traverse(input->m_AssignExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(Usings* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Using* input)
                    {
                        assert(input); 
                        line("[Using]");
                        indent();
                        if (input->m_QualifiedName) traverse(input->m_QualifiedName);
                        unindent();
                        return true;
                    }
                    bool traverse(Typedefs* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Typedef* input)
                    {
                        assert(input); 
                        line("[Typedef]");
                        indent();
                        if (input->m_Type) traverse(input->m_Type);
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        unindent();
                        return true;
                    }
                    bool traverse(Aliases* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Alias* input)
                    {
                        assert(input); 
                        line("[Alias]");
                        indent();
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if(input->m_ASSIGN)
                        line(phantom::lexical_cast<String>(input->m_ASSIGN));
                        if (input->m_Type) traverse(input->m_Type);
                        unindent();
                        return true;
                    }
                    bool traverse(TemplateParameterDefaults* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TemplateParameterDefault* input)
                    {
                        assert(input); 
                        line("[TemplateParameterDefault]");
                        indent();
                        if (input->m_TemplateArgumentAssign) traverse(input->m_TemplateArgumentAssign);
                        if (input->m_TemplateArgumentNoAssign) traverse(input->m_TemplateArgumentNoAssign);
                        unindent();
                        return true;
                    }
                    bool traverse(TemplateValueParameters* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TemplateValueParameter* input)
                    {
                        assert(input); 
                        line("[TemplateValueParameter]");
                        indent();
                        if (input->m_QualifiedName) traverse(input->m_QualifiedName);
                        if (input->m_FundamentalType) traverse(input->m_FundamentalType);
                        switch(input->m_ELLIPSE.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_ELLIPSE.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_ELLIPSE.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_bool())); break;
                            default: assert(false); break;
                        }
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        unindent();
                        return true;
                    }
                    bool traverse(TemplateTypeParameters* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TemplateTypeParameter* input)
                    {
                        assert(input); 
                        line("[TemplateTypeParameter]");
                        indent();
                        switch(input->m_ELLIPSE.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_ELLIPSE.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_ELLIPSE.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_bool())); break;
                            default: assert(false); break;
                        }
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        unindent();
                        return true;
                    }
                    bool traverse(TemplateParameters* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TemplateParameter* input)
                    {
                        assert(input); 
                        line("[TemplateParameter]");
                        indent();
                        if (input->m_TemplateValueParameter) traverse(input->m_TemplateValueParameter);
                        if (input->m_TemplateTypeParameter) traverse(input->m_TemplateTypeParameter);
                        if (input->m_TemplateParameterDefault) traverse(input->m_TemplateParameterDefault);
                        unindent();
                        return true;
                    }
                    bool traverse(TemplateSignatures* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TemplateSignature* input)
                    {
                        assert(input); 
                        line("[TemplateSignature]");
                        indent();
                        if (input->m_TemplateParameters) traverse(input->m_TemplateParameters);
                        unindent();
                        return true;
                    }
                    bool traverse(Templates* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Template* input)
                    {
                        assert(input); 
                        line("[Template]");
                        indent();
                        if (input->m_TemplateSignature) traverse(input->m_TemplateSignature);
                        if (input->m_Class) traverse(input->m_Class);
                        if (input->m_Union) traverse(input->m_Union);
                        if (input->m_Alias) traverse(input->m_Alias);
                        if (input->m_TemplateFunction) traverse(input->m_TemplateFunction);
                        if (input->m_Constructor) traverse(input->m_Constructor);
                        unindent();
                        return true;
                    }
                    bool traverse(TemplateFunctions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TemplateFunction* input)
                    {
                        assert(input); 
                        line("[TemplateFunction]");
                        indent();
                        switch(input->m_STATIC.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_STATIC.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_STATIC.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_STATIC.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_STATIC.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_STATIC.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_STATIC.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_STATIC.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_STATIC.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_STATIC.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_STATIC.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_STATIC.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_STATIC.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_STATIC.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_STATIC.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_TypeOrAuto) traverse(input->m_TypeOrAuto);
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_FunctionEnd) traverse(input->m_FunctionEnd);
                        unindent();
                        return true;
                    }
                    bool traverse(FunctionBlocks* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(FunctionBlock* input)
                    {
                        assert(input); 
                        line("[FunctionBlock]");
                        indent();
                        if (input->m_Statements) traverse(input->m_Statements);
                        unindent();
                        return true;
                    }
                    bool traverse(QualifiedNames* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(QualifiedName* input)
                    {
                        assert(input); 
                        line("[QualifiedName]");
                        indent();
                        switch(input->m_TYPENAME.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_TYPENAME.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_TYPENAME.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_TYPENAME.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Scope) traverse(input->m_Scope);
                        if (input->m_Dot) traverse(input->m_Dot);
                        unindent();
                        return true;
                    }
                    bool traverse(QualifiedDotNames* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(QualifiedDotName* input)
                    {
                        assert(input); 
                        line("[QualifiedDotName]");
                        indent();
                        switch(input->m_DOT.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_DOT.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_DOT.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_DOT.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_DOT.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_DOT.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_DOT.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_DOT.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_DOT.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_DOT.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_DOT.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_DOT.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_DOT.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_DOT.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_DOT.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Names) traverse(input->m_Names);
                        unindent();
                        return true;
                    }
                    bool traverse(QualifiedScopedNames* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(QualifiedScopedName* input)
                    {
                        assert(input); 
                        line("[QualifiedScopedName]");
                        indent();
                        if (input->m_Names) traverse(input->m_Names);
                        if (input->m_Name) traverse(input->m_Name);
                        switch(input->m_SCOPE.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_SCOPE.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_SCOPE.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_SCOPE.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_SCOPE.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_SCOPE.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_SCOPE.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_SCOPE.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_SCOPE.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_SCOPE.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_SCOPE.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_SCOPE.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_SCOPE.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_SCOPE.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_SCOPE.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(BaseClasses* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BaseClass* input)
                    {
                        assert(input); 
                        line("[BaseClass]");
                        indent();
                        switch(input->m_PUBLIC.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_PUBLIC.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_PUBLIC.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_PUBLIC.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_BasicType) traverse(input->m_BasicType);
                        unindent();
                        return true;
                    }
                    bool traverse(Classes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Class* input)
                    {
                        assert(input); 
                        line("[Class]");
                        indent();
                        if(input->m_CLASS)
                        line(phantom::lexical_cast<String>(input->m_CLASS));
                        switch(input->m_PAREN_START.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_PAREN_START.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_PAREN_START.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_PAREN_START.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_MetaClassConstructionExpression) traverse(input->m_MetaClassConstructionExpression);
                        switch(input->m_PAREN_START0.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_PAREN_START0.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_PAREN_START0.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_PAREN_START0.as_bool())); break;
                            default: assert(false); break;
                        }
                        switch(input->m_AlignAs.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_AlignAs.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_AlignAs.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_AlignAs.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_AlignAs.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_AlignAs.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_AlignAs.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_AlignAs.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_AlignAs.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_AlignAs.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_AlignAs.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_AlignAs.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_AlignAs.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_AlignAs.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_AlignAs.as_bool())); break;
                            default: assert(false); break;
                        }
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_TemplateArgumentList) traverse(input->m_TemplateArgumentList);
                        if (input->m_BaseClasses) traverse(input->m_BaseClasses);
                        if (input->m_Members) traverse(input->m_Members);
                        unindent();
                        return true;
                    }
                    bool traverse(Unions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Union* input)
                    {
                        assert(input); 
                        line("[Union]");
                        indent();
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_TemplateArgumentList) traverse(input->m_TemplateArgumentList);
                        if (input->m_BasicMembers) traverse(input->m_BasicMembers);
                        unindent();
                        return true;
                    }
                    bool traverse(Methods* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Method* input)
                    {
                        assert(input); 
                        line("[Method]");
                        indent();
                        if (input->m_MethodSpecifiers) traverse(input->m_MethodSpecifiers);
                        if (input->m_TypeOrAuto) traverse(input->m_TypeOrAuto);
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_FunctionEnd) traverse(input->m_FunctionEnd);
                        unindent();
                        return true;
                    }
                    bool traverse(Parameters* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Parameter* input)
                    {
                        assert(input); 
                        line("[Parameter]");
                        indent();
                        if (input->m_Type) traverse(input->m_Type);
                        switch(input->m_IDENTIFIER.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_IDENTIFIER.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_IDENTIFIER.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_IDENTIFIER.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_ArrayExtents) traverse(input->m_ArrayExtents);
                        switch(input->m_ASSIGN.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_ASSIGN.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_ASSIGN.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_ASSIGN.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_AssignExpressionOrInitializerList) traverse(input->m_AssignExpressionOrInitializerList);
                        unindent();
                        return true;
                    }
                    bool traverse(AccessSpecifiers* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(AccessSpecifier* input)
                    {
                        assert(input); 
                        line("[AccessSpecifier]");
                        indent();
                        if(input->m_PUBLIC)
                        line(phantom::lexical_cast<String>(input->m_PUBLIC));
                        unindent();
                        return true;
                    }
                    bool traverse(FieldOverrides* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(FieldOverride* input)
                    {
                        assert(input); 
                        line("[FieldOverride]");
                        indent();
                        if(input->m_DEFAULT)
                        line(phantom::lexical_cast<String>(input->m_DEFAULT));
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if(input->m_ASSIGN)
                        line(phantom::lexical_cast<String>(input->m_ASSIGN));
                        if (input->m_AssignExpression) traverse(input->m_AssignExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(Symbols* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Symbol* input)
                    {
                        assert(input); 
                        line("[Symbol]");
                        indent();
                        if (input->m_ExplicitType) traverse(input->m_ExplicitType);
                        if (input->m_PostFixExpression) traverse(input->m_PostFixExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(Members* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Member* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_MemberWithMetaOrAnnotation: return traverse(static_cast<MemberWithMetaOrAnnotation*>(input));
                            case e_RuleType_BasicMemberWithMetaOrAnnotation: return traverse(static_cast<BasicMemberWithMetaOrAnnotation*>(input));
                            case e_RuleType_Friend: return traverse(static_cast<Friend*>(input));
                            case e_RuleType_BasicMember: return traverse(static_cast<BasicMember*>(input));
                            default:
                            {
                                line("[Member]");
                                indent();
                                if (input->m_MemberWithMetaOrAnnotation) traverse(input->m_MemberWithMetaOrAnnotation);
                                if (input->m_BasicMember) traverse(input->m_BasicMember);
                            }
                            unindent();
                        }
                        return true;
                    }
                    bool traverse(IfOrWhileLocalVariableDecls* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(IfOrWhileLocalVariableDecl* input)
                    {
                        assert(input); 
                        line("[IfOrWhileLocalVariableDecl]");
                        indent();
                        if (input->m_TypeOrAuto) traverse(input->m_TypeOrAuto);
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(IfOrWhileConditions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(IfOrWhileCondition* input)
                    {
                        assert(input); 
                        line("[IfOrWhileCondition]");
                        indent();
                        if (input->m_IfOrWhileLocalVariableDecl) traverse(input->m_IfOrWhileLocalVariableDecl);
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(BlockDeclarations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BlockDeclaration* input)
                    {
                        assert(input); 
                        line("[BlockDeclaration]");
                        indent();
                        if (input->m_Class) traverse(input->m_Class);
                        if (input->m_Enum) traverse(input->m_Enum);
                        if (input->m_Typedef) traverse(input->m_Typedef);
                        if (input->m_Alias) traverse(input->m_Alias);
                        if (input->m_Using) traverse(input->m_Using);
                        if (input->m_StaticAssert) traverse(input->m_StaticAssert);
                        if (input->m_Alias0) traverse(input->m_Alias0);
                        unindent();
                        return true;
                    }
                    bool traverse(FundamentalTypeFunctionCasts* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(FundamentalTypeFunctionCast* input)
                    {
                        assert(input); 
                        line("[FundamentalTypeFunctionCast]");
                        indent();
                        if (input->m_FundamentalType) traverse(input->m_FundamentalType);
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(ExpressionStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ExpressionStatement* input)
                    {
                        assert(input); 
                        line("[ExpressionStatement]");
                        indent();
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(LastTemplateArguments* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LastTemplateArgument* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_LastTemplateArgumentAssign: return traverse(static_cast<LastTemplateArgumentAssign*>(input));
                            case e_RuleType_LastTemplateArgumentNoAssign: return traverse(static_cast<LastTemplateArgumentNoAssign*>(input));
                            default:
                            {
                                line("[LastTemplateArgument]");
                                indent();
                                if (input->m_LastTemplateArgumentAssign) traverse(input->m_LastTemplateArgumentAssign);
                                if (input->m_LastTemplateArgumentNoAssign) traverse(input->m_LastTemplateArgumentNoAssign);
                            }
                            unindent();
                        }
                        return true;
                    }
                    bool traverse(SwitchStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(SwitchStatement* input)
                    {
                        assert(input); 
                        line("[SwitchStatement]");
                        indent();
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Expression) traverse(input->m_Expression);
                        if (input->m_StatementOrCases) traverse(input->m_StatementOrCases);
                        unindent();
                        return true;
                    }
                    bool traverse(PrimaryExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(PrimaryExpression* input)
                    {
                        assert(input); 
                        line("[PrimaryExpression]");
                        indent();
                        if (input->m_KeywordExpression) traverse(input->m_KeywordExpression);
                        if (input->m_SizeofExpression) traverse(input->m_SizeofExpression);
                        if (input->m_TypeofExpression) traverse(input->m_TypeofExpression);
                        if (input->m_NullptrExpression) traverse(input->m_NullptrExpression);
                        switch(input->m_DOT.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_DOT.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_DOT.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_DOT.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_DOT.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_DOT.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_DOT.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_DOT.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_DOT.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_DOT.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_DOT.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_DOT.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_DOT.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_DOT.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_DOT.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Name) traverse(input->m_Name);
                        if (input->m_CStyleCastExpression) traverse(input->m_CStyleCastExpression);
                        if (input->m_TypeTraitBinary) traverse(input->m_TypeTraitBinary);
                        if (input->m_StringLiteralChain) traverse(input->m_StringLiteralChain);
                        if (input->m_SpecialCastExpression) traverse(input->m_SpecialCastExpression);
                        if (input->m_FunctionPtrExpression) traverse(input->m_FunctionPtrExpression);
                        if (input->m_FundamentalTypeFunctionCast) traverse(input->m_FundamentalTypeFunctionCast);
                        if (input->m_LambdaExpression) traverse(input->m_LambdaExpression);
                        switch(input->m_INT_VALUE.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_INT_VALUE.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_INT_VALUE.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(NotLastTemplateArguments* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(NotLastTemplateArgument* input)
                    {
                        assert(input); 
                        switch (input->__type)
                        {
                            case e_RuleType_NotLastTemplateArgumentAssign: return traverse(static_cast<NotLastTemplateArgumentAssign*>(input));
                            case e_RuleType_NotLastTemplateArgumentNoAssign: return traverse(static_cast<NotLastTemplateArgumentNoAssign*>(input));
                            default:
                            {
                                line("[NotLastTemplateArgument]");
                                indent();
                                if (input->m_NotLastTemplateArgumentAssign) traverse(input->m_NotLastTemplateArgumentAssign);
                                if (input->m_NotLastTemplateArgumentNoAssign) traverse(input->m_NotLastTemplateArgumentNoAssign);
                            }
                            unindent();
                        }
                        return true;
                    }
                    bool traverse(UnsignedTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(UnsignedType* input)
                    {
                        assert(input); 
                        line("[UnsignedType]");
                        indent();
                        if(input->m_UNSIGNED)
                        line(phantom::lexical_cast<String>(input->m_UNSIGNED));
                        if (input->m_Type) traverse(input->m_Type);
                        unindent();
                        return true;
                    }
                    bool traverse(SignedTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(SignedType* input)
                    {
                        assert(input); 
                        line("[SignedType]");
                        indent();
                        if(input->m_SIGNED)
                        line(phantom::lexical_cast<String>(input->m_SIGNED));
                        if (input->m_Type) traverse(input->m_Type);
                        unindent();
                        return true;
                    }
                    bool traverse(LongTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LongType* input)
                    {
                        assert(input); 
                        line("[LongType]");
                        indent();
                        if(input->m_LONG)
                        line(phantom::lexical_cast<String>(input->m_LONG));
                        if (input->m_Type) traverse(input->m_Type);
                        unindent();
                        return true;
                    }
                    bool traverse(VoidTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(VoidType* input)
                    {
                        assert(input); 
                        line("[VoidType]");
                        indent();
                        if(input->m_VOID)
                        line(phantom::lexical_cast<String>(input->m_VOID));
                        unindent();
                        return true;
                    }
                    bool traverse(FundamentalTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(FundamentalType* input)
                    {
                        assert(input); 
                        line("[FundamentalType]");
                        indent();
                        if (input->m_UnsignedType) traverse(input->m_UnsignedType);
                        if (input->m_SignedType) traverse(input->m_SignedType);
                        if (input->m_LongType) traverse(input->m_LongType);
                        if (input->m_VoidType) traverse(input->m_VoidType);
                        if (input->m_FundamentalTypeBase) traverse(input->m_FundamentalTypeBase);
                        unindent();
                        return true;
                    }
                    bool traverse(QualifiedTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(QualifiedType* input)
                    {
                        assert(input); 
                        line("[QualifiedType]");
                        indent();
                        if (input->m_QualifiedName) traverse(input->m_QualifiedName);
                        unindent();
                        return true;
                    }
                    bool traverse(ExplicitTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ExplicitType* input)
                    {
                        assert(input); 
                        line("[ExplicitType]");
                        indent();
                        if(input->m_CONSTs) for(auto v : *input->m_CONSTs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_BasicType) traverse(input->m_BasicType);
                        if (input->m_TypeExtents) traverse(input->m_TypeExtents);
                        switch(input->m_ELLIPSE.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_ELLIPSE.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_ELLIPSE.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(TypeNoFunctionTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TypeNoFunctionType* input)
                    {
                        assert(input); 
                        line("[TypeNoFunctionType]");
                        indent();
                        if (input->m_ExplicitTypeNoFunctionType) traverse(input->m_ExplicitTypeNoFunctionType);
                        if (input->m_ExpressionType) traverse(input->m_ExpressionType);
                        unindent();
                        return true;
                    }
                    bool traverse(ExplicitTypeNoFunctionTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ExplicitTypeNoFunctionType* input)
                    {
                        assert(input); 
                        line("[ExplicitTypeNoFunctionType]");
                        indent();
                        if(input->m_CONSTs) for(auto v : *input->m_CONSTs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_BasicType) traverse(input->m_BasicType);
                        if (input->m_TypeExtentNoFunctions) traverse(input->m_TypeExtentNoFunctions);
                        switch(input->m_ELLIPSE.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_ELLIPSE.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_ELLIPSE.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_ELLIPSE.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(ExpressionTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ExpressionType* input)
                    {
                        assert(input); 
                        line("[ExpressionType]");
                        indent();
                        if (input->m_AssignExpression) traverse(input->m_AssignExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(ExplicitTypeOrAutos* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ExplicitTypeOrAuto* input)
                    {
                        assert(input); 
                        line("[ExplicitTypeOrAuto]");
                        indent();
                        if(input->m_CONSTs) for(auto v : *input->m_CONSTs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_BasicTypeOrAuto) traverse(input->m_BasicTypeOrAuto);
                        if (input->m_TypeExtents) traverse(input->m_TypeExtents);
                        unindent();
                        return true;
                    }
                    bool traverse(TypeOrAutos* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TypeOrAuto* input)
                    {
                        assert(input); 
                        line("[TypeOrAuto]");
                        indent();
                        if (input->m_ExplicitTypeOrAuto) traverse(input->m_ExplicitTypeOrAuto);
                        if (input->m_ExpressionType) traverse(input->m_ExpressionType);
                        unindent();
                        return true;
                    }
                    bool traverse(AutoTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(AutoType* input)
                    {
                        assert(input); 
                        line("[AutoType]");
                        indent();
                        if(input->m_CONSTs) for(auto v : *input->m_CONSTs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Auto) traverse(input->m_Auto);
                        if (input->m_TypeExtents) traverse(input->m_TypeExtents);
                        unindent();
                        return true;
                    }
                    bool traverse(BasicTypes* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BasicType* input)
                    {
                        assert(input); 
                        line("[BasicType]");
                        indent();
                        if (input->m_FundamentalType) traverse(input->m_FundamentalType);
                        if (input->m_DeclType) traverse(input->m_DeclType);
                        if (input->m_QualifiedType) traverse(input->m_QualifiedType);
                        unindent();
                        return true;
                    }
                    bool traverse(Autos* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Auto* input)
                    {
                        assert(input); 
                        line("[Auto]");
                        indent();
                        if(input->m_AUTO)
                        line(phantom::lexical_cast<String>(input->m_AUTO));
                        unindent();
                        return true;
                    }
                    bool traverse(BasicTypeOrAutos* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BasicTypeOrAuto* input)
                    {
                        assert(input); 
                        line("[BasicTypeOrAuto]");
                        indent();
                        if (input->m_FundamentalType) traverse(input->m_FundamentalType);
                        if (input->m_Auto) traverse(input->m_Auto);
                        if (input->m_DeclType) traverse(input->m_DeclType);
                        if (input->m_QualifiedType) traverse(input->m_QualifiedType);
                        unindent();
                        return true;
                    }
                    bool traverse(IntegralConstants* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(IntegralConstant* input)
                    {
                        assert(input); 
                        line("[IntegralConstant]");
                        indent();
                        switch(input->m_INT_VALUE.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_INT_VALUE.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_INT_VALUE.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_INT_VALUE.as_bool())); break;
                            default: assert(false); break;
                        }
                        unindent();
                        return true;
                    }
                    bool traverse(ArrayExtents* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ArrayExtent* input)
                    {
                        assert(input); 
                        line("[ArrayExtent]");
                        indent();
                        if (input->m_ArrayExtentExpression) traverse(input->m_ArrayExtentExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(FunctionTypeExtents* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(FunctionTypeExtent* input)
                    {
                        assert(input); 
                        line("[FunctionTypeExtent]");
                        indent();
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Types) traverse(input->m_Types);
                        unindent();
                        return true;
                    }
                    bool traverse(DerivedExtents* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(DerivedExtent* input)
                    {
                        assert(input); 
                        line("[DerivedExtent]");
                        indent();
                        if(input->m_BIN_AND)
                        line(phantom::lexical_cast<String>(input->m_BIN_AND));
                        unindent();
                        return true;
                    }
                    bool traverse(TypeExtentNoFunctions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TypeExtentNoFunction* input)
                    {
                        assert(input); 
                        line("[TypeExtentNoFunction]");
                        indent();
                        if (input->m_ArrayExtent) traverse(input->m_ArrayExtent);
                        if (input->m_DerivedExtent) traverse(input->m_DerivedExtent);
                        unindent();
                        return true;
                    }
                    bool traverse(AssignExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(AssignExpression* input)
                    {
                        assert(input); 
                        line("[AssignExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if(input->m_RightShiftAssign) for(auto v : *input->m_RightShiftAssign)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_RightShiftAssign_RightExpression) traverse(input->m_RightShiftAssign_RightExpression);
                        if(input->m_RightShift) for(auto v : *input->m_RightShift)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_RightShift_RightExpression) traverse(input->m_RightShift_RightExpression);
                        switch(input->m_GT.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_GT.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_GT.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_GT.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_GT.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_GT.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_GT.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_GT.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_GT.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_GT.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_GT.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_GT.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_GT.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_GT.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_GT.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Greater_RightExpression) traverse(input->m_Greater_RightExpression);
                        switch(input->m_Op.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_Op.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_Op.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_Op.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_Op.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_Op.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_Op.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_Op.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_Op.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_Op.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_Op.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_Op.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_Op.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_Op.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_Op.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_RightExpression) traverse(input->m_RightExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(ConditionalExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ConditionalExpression* input)
                    {
                        assert(input); 
                        line("[ConditionalExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if (input->m_Then) traverse(input->m_Then);
                        if (input->m_Else) traverse(input->m_Else);
                        unindent();
                        return true;
                    }
                    bool traverse(Friends* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(Friend* input)
                    {
                        assert(input); 
                        line("[Friend]");
                        indent();
                        switch(input->m_CLASS.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_CLASS.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_CLASS.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_CLASS.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_CLASS.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_CLASS.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_CLASS.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_CLASS.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_CLASS.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_CLASS.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_CLASS.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_CLASS.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_CLASS.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_CLASS.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_CLASS.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_QualifiedName) traverse(input->m_QualifiedName);
                        unindent();
                        return true;
                    }
                    bool traverse(LogicalOrExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LogicalOrExpression* input)
                    {
                        assert(input); 
                        line("[LogicalOrExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if (input->m_LogicalOrExpressionRights) traverse(input->m_LogicalOrExpressionRights);
                        unindent();
                        return true;
                    }
                    bool traverse(BasicMembers* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BasicMember* input)
                    {
                        assert(input); 
                        line("[BasicMember]");
                        indent();
                        if (input->m_AccessSpecifier) traverse(input->m_AccessSpecifier);
                        if (input->m_Destructor) traverse(input->m_Destructor);
                        if (input->m_Method) traverse(input->m_Method);
                        if (input->m_Method0) traverse(input->m_Method0);
                        if (input->m_FunctionOrVariable) traverse(input->m_FunctionOrVariable);
                        if (input->m_ConversionFunction) traverse(input->m_ConversionFunction);
                        if (input->m_Constructor) traverse(input->m_Constructor);
                        if (input->m_Destructor0) traverse(input->m_Destructor0);
                        if (input->m_Friend) traverse(input->m_Friend);
                        if (input->m_BasicMemberWithMetaOrAnnotation) traverse(input->m_BasicMemberWithMetaOrAnnotation);
                        if (input->m_Declaration) traverse(input->m_Declaration);
                        unindent();
                        return true;
                    }
                    bool traverse(LogicalAndExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LogicalAndExpression* input)
                    {
                        assert(input); 
                        line("[LogicalAndExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if (input->m_LogicalAndExpressionRights) traverse(input->m_LogicalAndExpressionRights);
                        unindent();
                        return true;
                    }
                    bool traverse(ForStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ForStatement* input)
                    {
                        assert(input); 
                        line("[ForStatement]");
                        indent();
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_ForeachSignature) traverse(input->m_ForeachSignature);
                        if (input->m_Init) traverse(input->m_Init);
                        if (input->m_Test) traverse(input->m_Test);
                        if (input->m_Update) traverse(input->m_Update);
                        if (input->m_Statement) traverse(input->m_Statement);
                        unindent();
                        return true;
                    }
                    bool traverse(BinOrExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BinOrExpression* input)
                    {
                        assert(input); 
                        line("[BinOrExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if (input->m_BinOrExpressionRights) traverse(input->m_BinOrExpressionRights);
                        unindent();
                        return true;
                    }
                    bool traverse(WhileStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(WhileStatement* input)
                    {
                        assert(input); 
                        line("[WhileStatement]");
                        indent();
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Condition) traverse(input->m_Condition);
                        if (input->m_Statement) traverse(input->m_Statement);
                        unindent();
                        return true;
                    }
                    bool traverse(XOrExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(XOrExpression* input)
                    {
                        assert(input); 
                        line("[XOrExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if (input->m_XOrExpressionRights) traverse(input->m_XOrExpressionRights);
                        unindent();
                        return true;
                    }
                    bool traverse(BlockStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BlockStatement* input)
                    {
                        assert(input); 
                        line("[BlockStatement]");
                        indent();
                        if (input->m_Statements) traverse(input->m_Statements);
                        unindent();
                        return true;
                    }
                    bool traverse(BinAndExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BinAndExpression* input)
                    {
                        assert(input); 
                        line("[BinAndExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if (input->m_BinAndExpressionRights) traverse(input->m_BinAndExpressionRights);
                        unindent();
                        return true;
                    }
                    bool traverse(DoWhileStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(DoWhileStatement* input)
                    {
                        assert(input); 
                        line("[DoWhileStatement]");
                        indent();
                        if (input->m_BlockStatement) traverse(input->m_BlockStatement);
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(EqualityExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(EqualityExpression* input)
                    {
                        assert(input); 
                        line("[EqualityExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if (input->m_EqualityExpressionRights) traverse(input->m_EqualityExpressionRights);
                        unindent();
                        return true;
                    }
                    bool traverse(StatementOrCases* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(StatementOrCase* input)
                    {
                        assert(input); 
                        line("[StatementOrCase]");
                        indent();
                        if (input->m_CaseStatement) traverse(input->m_CaseStatement);
                        if (input->m_DefaultStatement) traverse(input->m_DefaultStatement);
                        if (input->m_Statement) traverse(input->m_Statement);
                        unindent();
                        return true;
                    }
                    bool traverse(RelationalExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(RelationalExpression* input)
                    {
                        assert(input); 
                        line("[RelationalExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if (input->m_RelationalExpressionRights) traverse(input->m_RelationalExpressionRights);
                        unindent();
                        return true;
                    }
                    bool traverse(MemberWithMetaOrAnnotations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(MemberWithMetaOrAnnotation* input)
                    {
                        assert(input); 
                        line("[MemberWithMetaOrAnnotation]");
                        indent();
                        if (input->m_MetaOrAnnotations) traverse(input->m_MetaOrAnnotations);
                        if (input->m_Member) traverse(input->m_Member);
                        unindent();
                        return true;
                    }
                    bool traverse(ShiftExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ShiftExpression* input)
                    {
                        assert(input); 
                        line("[ShiftExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if (input->m_ShiftExpressionRights) traverse(input->m_ShiftExpressionRights);
                        unindent();
                        return true;
                    }
                    bool traverse(BasicMemberWithMetaOrAnnotations* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BasicMemberWithMetaOrAnnotation* input)
                    {
                        assert(input); 
                        line("[BasicMemberWithMetaOrAnnotation]");
                        indent();
                        if (input->m_MetaOrAnnotations) traverse(input->m_MetaOrAnnotations);
                        if (input->m_BasicMember) traverse(input->m_BasicMember);
                        unindent();
                        return true;
                    }
                    bool traverse(AddExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(AddExpression* input)
                    {
                        assert(input); 
                        line("[AddExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if (input->m_AddExpressionRights) traverse(input->m_AddExpressionRights);
                        unindent();
                        return true;
                    }
                    bool traverse(IfStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(IfStatement* input)
                    {
                        assert(input); 
                        line("[IfStatement]");
                        indent();
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Condition) traverse(input->m_Condition);
                        if (input->m_Then) traverse(input->m_Then);
                        if (input->m_Else) traverse(input->m_Else);
                        unindent();
                        return true;
                    }
                    bool traverse(MulExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(MulExpression* input)
                    {
                        assert(input); 
                        line("[MulExpression]");
                        indent();
                        if (input->m_LeftExpression) traverse(input->m_LeftExpression);
                        if (input->m_MulExpressionRights) traverse(input->m_MulExpressionRights);
                        unindent();
                        return true;
                    }
                    bool traverse(CaseStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(CaseStatement* input)
                    {
                        assert(input); 
                        line("[CaseStatement]");
                        indent();
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(ExplicitLocalVariableDecls* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ExplicitLocalVariableDecl* input)
                    {
                        assert(input); 
                        line("[ExplicitLocalVariableDecl]");
                        indent();
                        if(input->m_CONSTs) for(auto v : *input->m_CONSTs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_BasicType) traverse(input->m_BasicType);
                        if (input->m_Declarators) traverse(input->m_Declarators);
                        unindent();
                        return true;
                    }
                    bool traverse(UnaryExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(UnaryExpression* input)
                    {
                        assert(input); 
                        line("[UnaryExpression]");
                        indent();
                        switch(input->m_Op.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_Op.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_Op.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_Op.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_Op.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_Op.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_Op.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_Op.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_Op.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_Op.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_Op.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_Op.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_Op.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_Op.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_Op.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Expression) traverse(input->m_Expression);
                        if (input->m_PostFixExpression) traverse(input->m_PostFixExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(AssignExpressionOrInitializerLists* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(AssignExpressionOrInitializerList* input)
                    {
                        assert(input); 
                        line("[AssignExpressionOrInitializerList]");
                        indent();
                        if (input->m_InitializerList) traverse(input->m_InitializerList);
                        if (input->m_AssignExpression) traverse(input->m_AssignExpression);
                        unindent();
                        return true;
                    }
                    bool traverse(ExpressionOrInitializerLists* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ExpressionOrInitializerList* input)
                    {
                        assert(input); 
                        line("[ExpressionOrInitializerList]");
                        indent();
                        if (input->m_InitializerList) traverse(input->m_InitializerList);
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(InitializerLists* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(InitializerList* input)
                    {
                        assert(input); 
                        line("[InitializerList]");
                        indent();
                        if (input->m_AssignExpressionOrInitializerLists) traverse(input->m_AssignExpressionOrInitializerLists);
                        unindent();
                        return true;
                    }
                    bool traverse(DefaultStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(DefaultStatement* input)
                    {
                        assert(input); 
                        line("[DefaultStatement]");
                        indent();
                        unindent();
                        return true;
                    }
                    bool traverse(ArrayAccesses* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ArrayAccess* input)
                    {
                        assert(input); 
                        line("[ArrayAccess]");
                        indent();
                        if (input->m_Expression) traverse(input->m_Expression);
                        unindent();
                        return true;
                    }
                    bool traverse(KeywordExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(KeywordExpression* input)
                    {
                        assert(input); 
                        line("[KeywordExpression]");
                        indent();
                        if(input->m_Keyword)
                        line(phantom::lexical_cast<String>(input->m_Keyword));
                        unindent();
                        return true;
                    }
                    bool traverse(NullptrExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(NullptrExpression* input)
                    {
                        assert(input); 
                        line("[NullptrExpression]");
                        indent();
                        unindent();
                        return true;
                    }
                    bool traverse(PostFixExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(PostFixExpression* input)
                    {
                        assert(input); 
                        line("[PostFixExpression]");
                        indent();
                        if (input->m_Expression) traverse(input->m_Expression);
                        if (input->m_PostFixEnds) traverse(input->m_PostFixEnds);
                        unindent();
                        return true;
                    }
                    bool traverse(AutoLocalVariableDecls* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(AutoLocalVariableDecl* input)
                    {
                        assert(input); 
                        line("[AutoLocalVariableDecl]");
                        indent();
                        if(input->m_CONSTs) for(auto v : *input->m_CONSTs)
                        switch(v.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(v.as_string()); break;
                            case spell::any::e_char: line(String(&v.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(v.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(v.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(v.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(v.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(v.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(v.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(v.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(v.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(v.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(v.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(v.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(v.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_Auto) traverse(input->m_Auto);
                        if (input->m_AutoDeclarators) traverse(input->m_AutoDeclarators);
                        unindent();
                        return true;
                    }
                    bool traverse(CStyleCastExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(CStyleCastExpression* input)
                    {
                        assert(input); 
                        line("[CStyleCastExpression]");
                        indent();
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Expression) traverse(input->m_Expression);
                        if (input->m_Type) traverse(input->m_Type);
                        if (input->m_Casted) traverse(input->m_Casted);
                        unindent();
                        return true;
                    }
                    bool traverse(TypeTraitBinaries* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TypeTraitBinary* input)
                    {
                        assert(input); 
                        line("[TypeTraitBinary]");
                        indent();
                        if(input->m_IS_SAME)
                        line(phantom::lexical_cast<String>(input->m_IS_SAME));
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Type) traverse(input->m_Type);
                        if (input->m_Type0) traverse(input->m_Type0);
                        unindent();
                        return true;
                    }
                    bool traverse(LabelStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LabelStatement* input)
                    {
                        assert(input); 
                        line("[LabelStatement]");
                        indent();
                        if(input->m_IDENTIFIER)
                        line(phantom::lexical_cast<String>(input->m_IDENTIFIER));
                        unindent();
                        return true;
                    }
                    bool traverse(AssertStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(AssertStatement* input)
                    {
                        assert(input); 
                        line("[AssertStatement]");
                        indent();
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_AssignExpression) traverse(input->m_AssignExpression);
                        switch(input->m_STRING_LIT.type())
                        {
                            case spell::any::e_undefined: break;
                            case spell::any::e_cstring:line(input->m_STRING_LIT.as_string()); break;
                            case spell::any::e_char: line(String(&input->m_STRING_LIT.ref_char(), 1)); break;
                            case spell::any::e_uchar: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_uchar())); break;
                            case spell::any::e_int: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_int())); break;
                            case spell::any::e_uint: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_uint())); break;
                            case spell::any::e_short: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_short())); break;
                            case spell::any::e_ushort: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_ushort())); break;
                            case spell::any::e_long: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_long())); break;
                            case spell::any::e_ulong: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_ulong())); break;
                            case spell::any::e_longlong: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_longlong())); break;
                            case spell::any::e_ulonglong: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_ulonglong())); break;
                            case spell::any::e_double: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_double())); break;
                            case spell::any::e_float: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_float())); break;
                            case spell::any::e_bool: line(phantom::lexical_cast<String>(input->m_STRING_LIT.as_bool())); break;
                            default: assert(false); break;
                        }
                        if (input->m_AssignExpressions) traverse(input->m_AssignExpressions);
                        unindent();
                        return true;
                    }
                    bool traverse(BreakStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(BreakStatement* input)
                    {
                        assert(input); 
                        line("[BreakStatement]");
                        indent();
                        if(input->m_BREAK)
                        line(phantom::lexical_cast<String>(input->m_BREAK));
                        unindent();
                        return true;
                    }
                    bool traverse(ContinueStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(ContinueStatement* input)
                    {
                        assert(input); 
                        line("[ContinueStatement]");
                        indent();
                        if(input->m_CONTINUE)
                        line(phantom::lexical_cast<String>(input->m_CONTINUE));
                        unindent();
                        return true;
                    }
                    bool traverse(SizeofExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(SizeofExpression* input)
                    {
                        assert(input); 
                        line("[SizeofExpression]");
                        indent();
                        if(input->m_SIZEOF)
                        line(phantom::lexical_cast<String>(input->m_SIZEOF));
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Type) traverse(input->m_Type);
                        unindent();
                        return true;
                    }
                    bool traverse(TypeofExpressions* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(TypeofExpression* input)
                    {
                        assert(input); 
                        line("[TypeofExpression]");
                        indent();
                        if(input->m_TYPEOF)
                        line(phantom::lexical_cast<String>(input->m_TYPEOF));
                        if(input->m_PAREN_START)
                        line(phantom::lexical_cast<String>(input->m_PAREN_START));
                        if (input->m_Type) traverse(input->m_Type);
                        unindent();
                        return true;
                    }
                    bool traverse(LocalVariableStatements* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LocalVariableStatement* input)
                    {
                        assert(input); 
                        line("[LocalVariableStatement]");
                        indent();
                        if (input->m_LocalVariableDecl) traverse(input->m_LocalVariableDecl);
                        unindent();
                        return true;
                    }
                    bool traverse(LastTemplateArgumentAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LastTemplateArgumentAssign* input)
                    {
                        assert(input); 
                        line("[LastTemplateArgumentAssign]");
                        indent();
                        if (input->m_TemplateArgumentAssign) traverse(input->m_TemplateArgumentAssign);
                        unindent();
                        return true;
                    }
                    bool traverse(LastTemplateArgumentNoAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(LastTemplateArgumentNoAssign* input)
                    {
                        assert(input); 
                        line("[LastTemplateArgumentNoAssign]");
                        indent();
                        if (input->m_TemplateArgumentNoAssign) traverse(input->m_TemplateArgumentNoAssign);
                        unindent();
                        return true;
                    }
                    bool traverse(NotLastTemplateArgumentAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(NotLastTemplateArgumentAssign* input)
                    {
                        assert(input); 
                        line("[NotLastTemplateArgumentAssign]");
                        indent();
                        if (input->m_TemplateArgumentAssign) traverse(input->m_TemplateArgumentAssign);
                        unindent();
                        return true;
                    }
                    bool traverse(NotLastTemplateArgumentNoAssigns* input)
                    {
                        assert(input);
                        for (auto elem : *input) { if (!traverse(elem)) return false; }
                        return true; 
                    }
                    bool traverse(NotLastTemplateArgumentNoAssign* input)
                    {
                        assert(input); 
                        line("[NotLastTemplateArgumentNoAssign]");
                        indent();
                        if (input->m_TemplateArgumentNoAssign) traverse(input->m_TemplateArgumentNoAssign);
                        unindent();
                        return true;
                    }
                };
                struct StdOutDumper : public Dumper<std::ostream> { StdOutDumper() : Dumper(std::cout) {} };
            }
        } // namespace ast
        class CppLiteGrammar
        {
            public:
            struct Allocator { phantom::Functor<void*(size_t)> allocate = [](size_t s) { return std::malloc(s); }; phantom::Functor<void(void*, size_t)> deallocate = [](void* ptr, size_t){ std::free(ptr); }; }; 
            typedef phantom::Functor<void(spell::Location, ast::EError, const char*)> ErrorDelegate;
            CppLiteGrammar(Allocator a_Allocator = Allocator());
            ~CppLiteGrammar();
            Allocator const& getAllocator() const { return m_Allocator; }
            ast::Source* parseSource(char const* in, size_t size, ErrorDelegate e = ErrorDelegate());
            ast::Symbol* parseSymbol(char const* in, size_t size, ErrorDelegate e = ErrorDelegate());
            ast::Expression* parseExpression(char const* in, size_t size, ErrorDelegate e = ErrorDelegate());
            const char* getTokenPrettyName(ast::EToken tok) const;
            private:
            Allocator m_Allocator;
            ast::CppLiteGrammarPrivate* _private;
        };
    } // namespace phantom
} // namespace lang
