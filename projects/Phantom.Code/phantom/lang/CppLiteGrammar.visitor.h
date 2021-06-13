namespace phantom
{
    namespace lang
    {
        struct CppLiteGrammarVisitor : ast::visitor::Recursive<CppLiteGrammarVisitor>
        {
            bool visitSource(ast::Source* input);
            bool endSource(ast::Source* input);
            bool visitModuleDeclaration(ast::ModuleDeclaration* input);
            bool endModuleDeclaration(ast::ModuleDeclaration* input);
            bool visitImportDeclaration(ast::ImportDeclaration* input);
            bool endImportDeclaration(ast::ImportDeclaration* input);
            bool visitImport(ast::Import* input);
            bool endImport(ast::Import* input);
            bool visitImportBinding(ast::ImportBinding* input);
            bool endImportBinding(ast::ImportBinding* input);
            bool visitImportBase(ast::ImportBase* input);
            bool endImportBase(ast::ImportBase* input);
            bool visitMeta(ast::Meta* input);
            bool endMeta(ast::Meta* input);
            bool visitMetaDecl(ast::MetaDecl* input);
            bool endMetaDecl(ast::MetaDecl* input);
            bool visitAnnotation(ast::Annotation* input);
            bool endAnnotation(ast::Annotation* input);
            bool visitMetaOrAnnotation(ast::MetaOrAnnotation* input);
            bool endMetaOrAnnotation(ast::MetaOrAnnotation* input);
            bool visitSourceDeclaration(ast::SourceDeclaration* input);
            bool endSourceDeclaration(ast::SourceDeclaration* input);
            bool visitDeclaration(ast::Declaration* input);
            bool endDeclaration(ast::Declaration* input);
            bool visitMixinStatementDefinition(ast::MixinStatementDefinition* input);
            bool endMixinStatementDefinition(ast::MixinStatementDefinition* input);
            bool visitIfOrWhileLocalVariableDecl(ast::IfOrWhileLocalVariableDecl* input);
            bool endIfOrWhileLocalVariableDecl(ast::IfOrWhileLocalVariableDecl* input);
            bool visitConversionFunction(ast::ConversionFunction* input);
            bool endConversionFunction(ast::ConversionFunction* input);
            bool visitConstructor(ast::Constructor* input);
            bool endConstructor(ast::Constructor* input);
            bool visitDestructor(ast::Destructor* input);
            bool endDestructor(ast::Destructor* input);
            bool visitConstructorInitializer(ast::ConstructorInitializer* input);
            bool endConstructorInitializer(ast::ConstructorInitializer* input);
            bool visitFunctionEnd(ast::FunctionEnd* input);
            bool endFunctionEnd(ast::FunctionEnd* input);
            bool visitOperator(ast::Operator* input);
            bool endOperator(ast::Operator* input);
            bool visitOperatorEnd(ast::OperatorEnd* input);
            bool endOperatorEnd(ast::OperatorEnd* input);
            bool visitFunctionOrVariableEnd(ast::FunctionOrVariableEnd* input);
            bool endFunctionOrVariableEnd(ast::FunctionOrVariableEnd* input);
            bool visitMethodSpecifier(ast::MethodSpecifier* input);
            bool endMethodSpecifier(ast::MethodSpecifier* input);
            bool visitSpecifier(ast::Specifier* input);
            bool endSpecifier(ast::Specifier* input);
            bool visitFunctionOrVariable(ast::FunctionOrVariable* input);
            bool endFunctionOrVariable(ast::FunctionOrVariable* input);
            bool visitDeclType(ast::DeclType* input);
            bool endDeclType(ast::DeclType* input);
            bool visitTemplateArgument(ast::TemplateArgument* input);
            bool endTemplateArgument(ast::TemplateArgument* input);
            bool visitIfOrWhileCondition(ast::IfOrWhileCondition* input);
            bool endIfOrWhileCondition(ast::IfOrWhileCondition* input);
            bool visitForeachSignature(ast::ForeachSignature* input);
            bool endForeachSignature(ast::ForeachSignature* input);
            bool visitStatement(ast::Statement* input);
            bool endStatement(ast::Statement* input);
            bool visitMixinStatement(ast::MixinStatement* input);
            bool endMixinStatement(ast::MixinStatement* input);
            bool visitReturnStatement(ast::ReturnStatement* input);
            bool endReturnStatement(ast::ReturnStatement* input);
            bool visitGotoStatement(ast::GotoStatement* input);
            bool endGotoStatement(ast::GotoStatement* input);
            bool visitTemplateArgumentListSplit(ast::TemplateArgumentListSplit* input);
            bool endTemplateArgumentListSplit(ast::TemplateArgumentListSplit* input);
            bool visitTemplateArgumentNoAssign(ast::TemplateArgumentNoAssign* input);
            bool endTemplateArgumentNoAssign(ast::TemplateArgumentNoAssign* input);
            bool visitTemplateArgumentAssign(ast::TemplateArgumentAssign* input);
            bool endTemplateArgumentAssign(ast::TemplateArgumentAssign* input);
            bool visitTemplateArgumentList(ast::TemplateArgumentList* input);
            bool endTemplateArgumentList(ast::TemplateArgumentList* input);
            bool visitName(ast::Name* input);
            bool endName(ast::Name* input);
            bool visitFundamentalTypeBase(ast::FundamentalTypeBase* input);
            bool endFundamentalTypeBase(ast::FundamentalTypeBase* input);
            bool visitDeclarator(ast::Declarator* input);
            bool endDeclarator(ast::Declarator* input);
            bool visitAutoDeclarator(ast::AutoDeclarator* input);
            bool endAutoDeclarator(ast::AutoDeclarator* input);
            bool visitLocalVariableDecl(ast::LocalVariableDecl* input);
            bool endLocalVariableDecl(ast::LocalVariableDecl* input);
            bool visitExpression(ast::Expression* input);
            bool endExpression(ast::Expression* input);
            bool visitLogicalOrExpressionRight(ast::LogicalOrExpressionRight* input);
            bool endLogicalOrExpressionRight(ast::LogicalOrExpressionRight* input);
            bool visitLogicalAndExpressionRight(ast::LogicalAndExpressionRight* input);
            bool endLogicalAndExpressionRight(ast::LogicalAndExpressionRight* input);
            bool visitBinOrExpressionRight(ast::BinOrExpressionRight* input);
            bool endBinOrExpressionRight(ast::BinOrExpressionRight* input);
            bool visitXOrExpressionRight(ast::XOrExpressionRight* input);
            bool endXOrExpressionRight(ast::XOrExpressionRight* input);
            bool visitBinAndExpressionRight(ast::BinAndExpressionRight* input);
            bool endBinAndExpressionRight(ast::BinAndExpressionRight* input);
            bool visitEqualityExpressionRight(ast::EqualityExpressionRight* input);
            bool endEqualityExpressionRight(ast::EqualityExpressionRight* input);
            bool visitType(ast::Type* input);
            bool endType(ast::Type* input);
            bool visitRelationalExpressionRight(ast::RelationalExpressionRight* input);
            bool endRelationalExpressionRight(ast::RelationalExpressionRight* input);
            bool visitShiftExpressionRight(ast::ShiftExpressionRight* input);
            bool endShiftExpressionRight(ast::ShiftExpressionRight* input);
            bool visitAddExpressionRight(ast::AddExpressionRight* input);
            bool endAddExpressionRight(ast::AddExpressionRight* input);
            bool visitMulExpressionRight(ast::MulExpressionRight* input);
            bool endMulExpressionRight(ast::MulExpressionRight* input);
            bool visitSpecialCastExpression(ast::SpecialCastExpression* input);
            bool endSpecialCastExpression(ast::SpecialCastExpression* input);
            bool visitFunctionPtrExpression(ast::FunctionPtrExpression* input);
            bool endFunctionPtrExpression(ast::FunctionPtrExpression* input);
            bool visitCallList(ast::CallList* input);
            bool endCallList(ast::CallList* input);
            bool visitArrayExtentExpression(ast::ArrayExtentExpression* input);
            bool endArrayExtentExpression(ast::ArrayExtentExpression* input);
            bool visitPostFixEnd(ast::PostFixEnd* input);
            bool endPostFixEnd(ast::PostFixEnd* input);
            bool visitStaticAssert(ast::StaticAssert* input);
            bool endStaticAssert(ast::StaticAssert* input);
            bool visitStringLiteralChain(ast::StringLiteralChain* input);
            bool endStringLiteralChain(ast::StringLiteralChain* input);
            bool visitUnion(ast::Union* input);
            bool endUnion(ast::Union* input);
            bool visitTypeExtent(ast::TypeExtent* input);
            bool endTypeExtent(ast::TypeExtent* input);
            bool visitEnum(ast::Enum* input);
            bool endEnum(ast::Enum* input);
            bool visitEnumConstant(ast::EnumConstant* input);
            bool endEnumConstant(ast::EnumConstant* input);
            bool visitUsing(ast::Using* input);
            bool endUsing(ast::Using* input);
            bool visitTypedef(ast::Typedef* input);
            bool endTypedef(ast::Typedef* input);
            bool visitAlias(ast::Alias* input);
            bool endAlias(ast::Alias* input);
            bool visitTemplateParameterDefault(ast::TemplateParameterDefault* input);
            bool endTemplateParameterDefault(ast::TemplateParameterDefault* input);
            bool visitTemplateValueParameter(ast::TemplateValueParameter* input);
            bool endTemplateValueParameter(ast::TemplateValueParameter* input);
            bool visitTemplateTypeParameter(ast::TemplateTypeParameter* input);
            bool endTemplateTypeParameter(ast::TemplateTypeParameter* input);
            bool visitTemplateParameter(ast::TemplateParameter* input);
            bool endTemplateParameter(ast::TemplateParameter* input);
            bool visitTemplateSignature(ast::TemplateSignature* input);
            bool endTemplateSignature(ast::TemplateSignature* input);
            bool visitTemplate(ast::Template* input);
            bool endTemplate(ast::Template* input);
            bool visitTemplateFunction(ast::TemplateFunction* input);
            bool endTemplateFunction(ast::TemplateFunction* input);
            bool visitFunctionBlock(ast::FunctionBlock* input);
            bool endFunctionBlock(ast::FunctionBlock* input);
            bool visitQualifiedName(ast::QualifiedName* input);
            bool endQualifiedName(ast::QualifiedName* input);
            bool visitQualifiedDotName(ast::QualifiedDotName* input);
            bool endQualifiedDotName(ast::QualifiedDotName* input);
            bool visitQualifiedScopedName(ast::QualifiedScopedName* input);
            bool endQualifiedScopedName(ast::QualifiedScopedName* input);
            bool visitBaseClass(ast::BaseClass* input);
            bool endBaseClass(ast::BaseClass* input);
            bool visitClass(ast::Class* input);
            bool endClass(ast::Class* input);
            bool visitForInit(ast::ForInit* input);
            bool endForInit(ast::ForInit* input);
            bool visitMethod(ast::Method* input);
            bool endMethod(ast::Method* input);
            bool visitParameter(ast::Parameter* input);
            bool endParameter(ast::Parameter* input);
            bool visitAccessSpecifier(ast::AccessSpecifier* input);
            bool endAccessSpecifier(ast::AccessSpecifier* input);
            bool visitFieldOverride(ast::FieldOverride* input);
            bool endFieldOverride(ast::FieldOverride* input);
            bool visitSymbol(ast::Symbol* input);
            bool endSymbol(ast::Symbol* input);
            bool visitMember(ast::Member* input);
            bool endMember(ast::Member* input);
            bool visitBlockDeclaration(ast::BlockDeclaration* input);
            bool endBlockDeclaration(ast::BlockDeclaration* input);
            bool visitTypeofExpression(ast::TypeofExpression* input);
            bool endTypeofExpression(ast::TypeofExpression* input);
            bool visitDefaultStatement(ast::DefaultStatement* input);
            bool endDefaultStatement(ast::DefaultStatement* input);
            bool visitLastTemplateArgument(ast::LastTemplateArgument* input);
            bool endLastTemplateArgument(ast::LastTemplateArgument* input);
            bool visitFundamentalTypeFunctionCast(ast::FundamentalTypeFunctionCast* input);
            bool endFundamentalTypeFunctionCast(ast::FundamentalTypeFunctionCast* input);
            bool visitPrimaryExpression(ast::PrimaryExpression* input);
            bool endPrimaryExpression(ast::PrimaryExpression* input);
            bool visitNotLastTemplateArgument(ast::NotLastTemplateArgument* input);
            bool endNotLastTemplateArgument(ast::NotLastTemplateArgument* input);
            bool visitUnsignedType(ast::UnsignedType* input);
            bool endUnsignedType(ast::UnsignedType* input);
            bool visitSignedType(ast::SignedType* input);
            bool endSignedType(ast::SignedType* input);
            bool visitLongType(ast::LongType* input);
            bool endLongType(ast::LongType* input);
            bool visitVoidType(ast::VoidType* input);
            bool endVoidType(ast::VoidType* input);
            bool visitFundamentalType(ast::FundamentalType* input);
            bool endFundamentalType(ast::FundamentalType* input);
            bool visitQualifiedType(ast::QualifiedType* input);
            bool endQualifiedType(ast::QualifiedType* input);
            bool visitExplicitType(ast::ExplicitType* input);
            bool endExplicitType(ast::ExplicitType* input);
            bool visitTypeNoFunctionType(ast::TypeNoFunctionType* input);
            bool endTypeNoFunctionType(ast::TypeNoFunctionType* input);
            bool visitExplicitTypeNoFunctionType(ast::ExplicitTypeNoFunctionType* input);
            bool endExplicitTypeNoFunctionType(ast::ExplicitTypeNoFunctionType* input);
            bool visitExpressionType(ast::ExpressionType* input);
            bool endExpressionType(ast::ExpressionType* input);
            bool visitExplicitTypeOrAuto(ast::ExplicitTypeOrAuto* input);
            bool endExplicitTypeOrAuto(ast::ExplicitTypeOrAuto* input);
            bool visitTypeOrAuto(ast::TypeOrAuto* input);
            bool endTypeOrAuto(ast::TypeOrAuto* input);
            bool visitAutoType(ast::AutoType* input);
            bool endAutoType(ast::AutoType* input);
            bool visitBasicType(ast::BasicType* input);
            bool endBasicType(ast::BasicType* input);
            bool visitAuto(ast::Auto* input);
            bool endAuto(ast::Auto* input);
            bool visitBasicTypeOrAuto(ast::BasicTypeOrAuto* input);
            bool endBasicTypeOrAuto(ast::BasicTypeOrAuto* input);
            bool visitIntegralConstant(ast::IntegralConstant* input);
            bool endIntegralConstant(ast::IntegralConstant* input);
            bool visitArrayExtent(ast::ArrayExtent* input);
            bool endArrayExtent(ast::ArrayExtent* input);
            bool visitFunctionTypeExtent(ast::FunctionTypeExtent* input);
            bool endFunctionTypeExtent(ast::FunctionTypeExtent* input);
            bool visitDerivedExtent(ast::DerivedExtent* input);
            bool endDerivedExtent(ast::DerivedExtent* input);
            bool visitLocalVariableStatement(ast::LocalVariableStatement* input);
            bool endLocalVariableStatement(ast::LocalVariableStatement* input);
            bool visitExpressionStatement(ast::ExpressionStatement* input);
            bool endExpressionStatement(ast::ExpressionStatement* input);
            bool visitLabelStatement(ast::LabelStatement* input);
            bool endLabelStatement(ast::LabelStatement* input);
            bool visitAssignExpression(ast::AssignExpression* input);
            bool endAssignExpression(ast::AssignExpression* input);
            bool visitConditionalExpression(ast::ConditionalExpression* input);
            bool endConditionalExpression(ast::ConditionalExpression* input);
            bool visitSwitchStatement(ast::SwitchStatement* input);
            bool endSwitchStatement(ast::SwitchStatement* input);
            bool visitLogicalOrExpression(ast::LogicalOrExpression* input);
            bool endLogicalOrExpression(ast::LogicalOrExpression* input);
            bool visitFriend(ast::Friend* input);
            bool endFriend(ast::Friend* input);
            bool visitLogicalAndExpression(ast::LogicalAndExpression* input);
            bool endLogicalAndExpression(ast::LogicalAndExpression* input);
            bool visitTypeExtentNoFunction(ast::TypeExtentNoFunction* input);
            bool endTypeExtentNoFunction(ast::TypeExtentNoFunction* input);
            bool visitBinOrExpression(ast::BinOrExpression* input);
            bool endBinOrExpression(ast::BinOrExpression* input);
            bool visitForStatement(ast::ForStatement* input);
            bool endForStatement(ast::ForStatement* input);
            bool visitXOrExpression(ast::XOrExpression* input);
            bool endXOrExpression(ast::XOrExpression* input);
            bool visitWhileStatement(ast::WhileStatement* input);
            bool endWhileStatement(ast::WhileStatement* input);
            bool visitBinAndExpression(ast::BinAndExpression* input);
            bool endBinAndExpression(ast::BinAndExpression* input);
            bool visitBlockStatement(ast::BlockStatement* input);
            bool endBlockStatement(ast::BlockStatement* input);
            bool visitEqualityExpression(ast::EqualityExpression* input);
            bool endEqualityExpression(ast::EqualityExpression* input);
            bool visitDoWhileStatement(ast::DoWhileStatement* input);
            bool endDoWhileStatement(ast::DoWhileStatement* input);
            bool visitRelationalExpression(ast::RelationalExpression* input);
            bool endRelationalExpression(ast::RelationalExpression* input);
            bool visitBasicMember(ast::BasicMember* input);
            bool endBasicMember(ast::BasicMember* input);
            bool visitShiftExpression(ast::ShiftExpression* input);
            bool endShiftExpression(ast::ShiftExpression* input);
            bool visitStatementOrCase(ast::StatementOrCase* input);
            bool endStatementOrCase(ast::StatementOrCase* input);
            bool visitAddExpression(ast::AddExpression* input);
            bool endAddExpression(ast::AddExpression* input);
            bool visitMemberWithMetaOrAnnotation(ast::MemberWithMetaOrAnnotation* input);
            bool endMemberWithMetaOrAnnotation(ast::MemberWithMetaOrAnnotation* input);
            bool visitMulExpression(ast::MulExpression* input);
            bool endMulExpression(ast::MulExpression* input);
            bool visitBasicMemberWithMetaOrAnnotation(ast::BasicMemberWithMetaOrAnnotation* input);
            bool endBasicMemberWithMetaOrAnnotation(ast::BasicMemberWithMetaOrAnnotation* input);
            bool visitIfStatement(ast::IfStatement* input);
            bool endIfStatement(ast::IfStatement* input);
            bool visitUnaryExpression(ast::UnaryExpression* input);
            bool endUnaryExpression(ast::UnaryExpression* input);
            bool visitAssignExpressionOrInitializerList(ast::AssignExpressionOrInitializerList* input);
            bool endAssignExpressionOrInitializerList(ast::AssignExpressionOrInitializerList* input);
            bool visitExpressionOrInitializerList(ast::ExpressionOrInitializerList* input);
            bool endExpressionOrInitializerList(ast::ExpressionOrInitializerList* input);
            bool visitInitializerList(ast::InitializerList* input);
            bool endInitializerList(ast::InitializerList* input);
            bool visitExplicitLocalVariableDecl(ast::ExplicitLocalVariableDecl* input);
            bool endExplicitLocalVariableDecl(ast::ExplicitLocalVariableDecl* input);
            bool visitArrayAccess(ast::ArrayAccess* input);
            bool endArrayAccess(ast::ArrayAccess* input);
            bool visitKeywordExpression(ast::KeywordExpression* input);
            bool endKeywordExpression(ast::KeywordExpression* input);
            bool visitNullptrExpression(ast::NullptrExpression* input);
            bool endNullptrExpression(ast::NullptrExpression* input);
            bool visitPostFixExpression(ast::PostFixExpression* input);
            bool endPostFixExpression(ast::PostFixExpression* input);
            bool visitCaseStatement(ast::CaseStatement* input);
            bool endCaseStatement(ast::CaseStatement* input);
            bool visitCStyleCastExpression(ast::CStyleCastExpression* input);
            bool endCStyleCastExpression(ast::CStyleCastExpression* input);
            bool visitTypeTraitBinary(ast::TypeTraitBinary* input);
            bool endTypeTraitBinary(ast::TypeTraitBinary* input);
            bool visitAutoLocalVariableDecl(ast::AutoLocalVariableDecl* input);
            bool endAutoLocalVariableDecl(ast::AutoLocalVariableDecl* input);
            bool visitAssertStatement(ast::AssertStatement* input);
            bool endAssertStatement(ast::AssertStatement* input);
            bool visitBreakStatement(ast::BreakStatement* input);
            bool endBreakStatement(ast::BreakStatement* input);
            bool visitContinueStatement(ast::ContinueStatement* input);
            bool endContinueStatement(ast::ContinueStatement* input);
            bool visitSizeofExpression(ast::SizeofExpression* input);
            bool endSizeofExpression(ast::SizeofExpression* input);
            bool visitLastTemplateArgumentAssign(ast::LastTemplateArgumentAssign* input);
            bool endLastTemplateArgumentAssign(ast::LastTemplateArgumentAssign* input);
            bool visitLastTemplateArgumentNoAssign(ast::LastTemplateArgumentNoAssign* input);
            bool endLastTemplateArgumentNoAssign(ast::LastTemplateArgumentNoAssign* input);
            bool visitNotLastTemplateArgumentAssign(ast::NotLastTemplateArgumentAssign* input);
            bool endNotLastTemplateArgumentAssign(ast::NotLastTemplateArgumentAssign* input);
            bool visitNotLastTemplateArgumentNoAssign(ast::NotLastTemplateArgumentNoAssign* input);
            bool endNotLastTemplateArgumentNoAssign(ast::NotLastTemplateArgumentNoAssign* input);
        };
        bool CppLiteGrammarVisitor::visitSource(ast::Source* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endSource(ast::Source* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitModuleDeclaration(ast::ModuleDeclaration* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endModuleDeclaration(ast::ModuleDeclaration* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitImportDeclaration(ast::ImportDeclaration* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endImportDeclaration(ast::ImportDeclaration* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitImport(ast::Import* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endImport(ast::Import* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitImportBinding(ast::ImportBinding* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endImportBinding(ast::ImportBinding* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitImportBase(ast::ImportBase* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endImportBase(ast::ImportBase* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitMeta(ast::Meta* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endMeta(ast::Meta* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitMetaDecl(ast::MetaDecl* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endMetaDecl(ast::MetaDecl* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAnnotation(ast::Annotation* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAnnotation(ast::Annotation* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitMetaOrAnnotation(ast::MetaOrAnnotation* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endMetaOrAnnotation(ast::MetaOrAnnotation* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitSourceDeclaration(ast::SourceDeclaration* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endSourceDeclaration(ast::SourceDeclaration* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitDeclaration(ast::Declaration* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endDeclaration(ast::Declaration* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitMixinStatementDefinition(ast::MixinStatementDefinition* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endMixinStatementDefinition(ast::MixinStatementDefinition* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitIfOrWhileLocalVariableDecl(ast::IfOrWhileLocalVariableDecl* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endIfOrWhileLocalVariableDecl(ast::IfOrWhileLocalVariableDecl* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitConversionFunction(ast::ConversionFunction* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endConversionFunction(ast::ConversionFunction* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitConstructor(ast::Constructor* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endConstructor(ast::Constructor* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitDestructor(ast::Destructor* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endDestructor(ast::Destructor* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitConstructorInitializer(ast::ConstructorInitializer* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endConstructorInitializer(ast::ConstructorInitializer* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitFunctionEnd(ast::FunctionEnd* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endFunctionEnd(ast::FunctionEnd* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitOperator(ast::Operator* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endOperator(ast::Operator* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitOperatorEnd(ast::OperatorEnd* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endOperatorEnd(ast::OperatorEnd* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitFunctionOrVariableEnd(ast::FunctionOrVariableEnd* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endFunctionOrVariableEnd(ast::FunctionOrVariableEnd* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitMethodSpecifier(ast::MethodSpecifier* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endMethodSpecifier(ast::MethodSpecifier* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitSpecifier(ast::Specifier* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endSpecifier(ast::Specifier* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitFunctionOrVariable(ast::FunctionOrVariable* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endFunctionOrVariable(ast::FunctionOrVariable* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitDeclType(ast::DeclType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endDeclType(ast::DeclType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplateArgument(ast::TemplateArgument* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplateArgument(ast::TemplateArgument* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitIfOrWhileCondition(ast::IfOrWhileCondition* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endIfOrWhileCondition(ast::IfOrWhileCondition* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitForeachSignature(ast::ForeachSignature* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endForeachSignature(ast::ForeachSignature* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitStatement(ast::Statement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endStatement(ast::Statement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitMixinStatement(ast::MixinStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endMixinStatement(ast::MixinStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitReturnStatement(ast::ReturnStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endReturnStatement(ast::ReturnStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitGotoStatement(ast::GotoStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endGotoStatement(ast::GotoStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplateArgumentListSplit(ast::TemplateArgumentListSplit* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplateArgumentListSplit(ast::TemplateArgumentListSplit* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplateArgumentNoAssign(ast::TemplateArgumentNoAssign* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplateArgumentNoAssign(ast::TemplateArgumentNoAssign* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplateArgumentAssign(ast::TemplateArgumentAssign* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplateArgumentAssign(ast::TemplateArgumentAssign* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplateArgumentList(ast::TemplateArgumentList* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplateArgumentList(ast::TemplateArgumentList* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitName(ast::Name* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endName(ast::Name* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitFundamentalTypeBase(ast::FundamentalTypeBase* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endFundamentalTypeBase(ast::FundamentalTypeBase* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitDeclarator(ast::Declarator* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endDeclarator(ast::Declarator* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAutoDeclarator(ast::AutoDeclarator* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAutoDeclarator(ast::AutoDeclarator* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitLocalVariableDecl(ast::LocalVariableDecl* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endLocalVariableDecl(ast::LocalVariableDecl* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitExpression(ast::Expression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endExpression(ast::Expression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitLogicalOrExpressionRight(ast::LogicalOrExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endLogicalOrExpressionRight(ast::LogicalOrExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitLogicalAndExpressionRight(ast::LogicalAndExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endLogicalAndExpressionRight(ast::LogicalAndExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBinOrExpressionRight(ast::BinOrExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBinOrExpressionRight(ast::BinOrExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitXOrExpressionRight(ast::XOrExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endXOrExpressionRight(ast::XOrExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBinAndExpressionRight(ast::BinAndExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBinAndExpressionRight(ast::BinAndExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitEqualityExpressionRight(ast::EqualityExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endEqualityExpressionRight(ast::EqualityExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitType(ast::Type* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endType(ast::Type* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitRelationalExpressionRight(ast::RelationalExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endRelationalExpressionRight(ast::RelationalExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitShiftExpressionRight(ast::ShiftExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endShiftExpressionRight(ast::ShiftExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAddExpressionRight(ast::AddExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAddExpressionRight(ast::AddExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitMulExpressionRight(ast::MulExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endMulExpressionRight(ast::MulExpressionRight* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitSpecialCastExpression(ast::SpecialCastExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endSpecialCastExpression(ast::SpecialCastExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitFunctionPtrExpression(ast::FunctionPtrExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endFunctionPtrExpression(ast::FunctionPtrExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitCallList(ast::CallList* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endCallList(ast::CallList* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitArrayExtentExpression(ast::ArrayExtentExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endArrayExtentExpression(ast::ArrayExtentExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitPostFixEnd(ast::PostFixEnd* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endPostFixEnd(ast::PostFixEnd* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitStaticAssert(ast::StaticAssert* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endStaticAssert(ast::StaticAssert* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitStringLiteralChain(ast::StringLiteralChain* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endStringLiteralChain(ast::StringLiteralChain* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitUnion(ast::Union* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endUnion(ast::Union* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTypeExtent(ast::TypeExtent* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTypeExtent(ast::TypeExtent* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitEnum(ast::Enum* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endEnum(ast::Enum* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitEnumConstant(ast::EnumConstant* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endEnumConstant(ast::EnumConstant* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitUsing(ast::Using* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endUsing(ast::Using* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTypedef(ast::Typedef* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTypedef(ast::Typedef* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAlias(ast::Alias* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAlias(ast::Alias* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplateParameterDefault(ast::TemplateParameterDefault* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplateParameterDefault(ast::TemplateParameterDefault* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplateValueParameter(ast::TemplateValueParameter* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplateValueParameter(ast::TemplateValueParameter* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplateTypeParameter(ast::TemplateTypeParameter* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplateTypeParameter(ast::TemplateTypeParameter* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplateParameter(ast::TemplateParameter* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplateParameter(ast::TemplateParameter* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplateSignature(ast::TemplateSignature* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplateSignature(ast::TemplateSignature* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplate(ast::Template* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplate(ast::Template* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTemplateFunction(ast::TemplateFunction* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTemplateFunction(ast::TemplateFunction* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitFunctionBlock(ast::FunctionBlock* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endFunctionBlock(ast::FunctionBlock* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitQualifiedName(ast::QualifiedName* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endQualifiedName(ast::QualifiedName* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitQualifiedDotName(ast::QualifiedDotName* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endQualifiedDotName(ast::QualifiedDotName* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitQualifiedScopedName(ast::QualifiedScopedName* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endQualifiedScopedName(ast::QualifiedScopedName* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBaseClass(ast::BaseClass* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBaseClass(ast::BaseClass* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitClass(ast::Class* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endClass(ast::Class* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitForInit(ast::ForInit* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endForInit(ast::ForInit* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitMethod(ast::Method* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endMethod(ast::Method* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitParameter(ast::Parameter* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endParameter(ast::Parameter* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAccessSpecifier(ast::AccessSpecifier* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAccessSpecifier(ast::AccessSpecifier* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitFieldOverride(ast::FieldOverride* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endFieldOverride(ast::FieldOverride* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitSymbol(ast::Symbol* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endSymbol(ast::Symbol* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitMember(ast::Member* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endMember(ast::Member* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBlockDeclaration(ast::BlockDeclaration* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBlockDeclaration(ast::BlockDeclaration* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTypeofExpression(ast::TypeofExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTypeofExpression(ast::TypeofExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitDefaultStatement(ast::DefaultStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endDefaultStatement(ast::DefaultStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitLastTemplateArgument(ast::LastTemplateArgument* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endLastTemplateArgument(ast::LastTemplateArgument* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitFundamentalTypeFunctionCast(ast::FundamentalTypeFunctionCast* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endFundamentalTypeFunctionCast(ast::FundamentalTypeFunctionCast* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitPrimaryExpression(ast::PrimaryExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endPrimaryExpression(ast::PrimaryExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitNotLastTemplateArgument(ast::NotLastTemplateArgument* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endNotLastTemplateArgument(ast::NotLastTemplateArgument* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitUnsignedType(ast::UnsignedType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endUnsignedType(ast::UnsignedType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitSignedType(ast::SignedType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endSignedType(ast::SignedType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitLongType(ast::LongType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endLongType(ast::LongType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitVoidType(ast::VoidType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endVoidType(ast::VoidType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitFundamentalType(ast::FundamentalType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endFundamentalType(ast::FundamentalType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitQualifiedType(ast::QualifiedType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endQualifiedType(ast::QualifiedType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitExplicitType(ast::ExplicitType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endExplicitType(ast::ExplicitType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTypeNoFunctionType(ast::TypeNoFunctionType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTypeNoFunctionType(ast::TypeNoFunctionType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitExplicitTypeNoFunctionType(ast::ExplicitTypeNoFunctionType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endExplicitTypeNoFunctionType(ast::ExplicitTypeNoFunctionType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitExpressionType(ast::ExpressionType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endExpressionType(ast::ExpressionType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitExplicitTypeOrAuto(ast::ExplicitTypeOrAuto* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endExplicitTypeOrAuto(ast::ExplicitTypeOrAuto* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTypeOrAuto(ast::TypeOrAuto* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTypeOrAuto(ast::TypeOrAuto* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAutoType(ast::AutoType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAutoType(ast::AutoType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBasicType(ast::BasicType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBasicType(ast::BasicType* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAuto(ast::Auto* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAuto(ast::Auto* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBasicTypeOrAuto(ast::BasicTypeOrAuto* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBasicTypeOrAuto(ast::BasicTypeOrAuto* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitIntegralConstant(ast::IntegralConstant* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endIntegralConstant(ast::IntegralConstant* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitArrayExtent(ast::ArrayExtent* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endArrayExtent(ast::ArrayExtent* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitFunctionTypeExtent(ast::FunctionTypeExtent* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endFunctionTypeExtent(ast::FunctionTypeExtent* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitDerivedExtent(ast::DerivedExtent* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endDerivedExtent(ast::DerivedExtent* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitLocalVariableStatement(ast::LocalVariableStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endLocalVariableStatement(ast::LocalVariableStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitExpressionStatement(ast::ExpressionStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endExpressionStatement(ast::ExpressionStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitLabelStatement(ast::LabelStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endLabelStatement(ast::LabelStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAssignExpression(ast::AssignExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAssignExpression(ast::AssignExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitConditionalExpression(ast::ConditionalExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endConditionalExpression(ast::ConditionalExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitSwitchStatement(ast::SwitchStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endSwitchStatement(ast::SwitchStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitLogicalOrExpression(ast::LogicalOrExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endLogicalOrExpression(ast::LogicalOrExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitFriend(ast::Friend* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endFriend(ast::Friend* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitLogicalAndExpression(ast::LogicalAndExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endLogicalAndExpression(ast::LogicalAndExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTypeExtentNoFunction(ast::TypeExtentNoFunction* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTypeExtentNoFunction(ast::TypeExtentNoFunction* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBinOrExpression(ast::BinOrExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBinOrExpression(ast::BinOrExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitForStatement(ast::ForStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endForStatement(ast::ForStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitXOrExpression(ast::XOrExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endXOrExpression(ast::XOrExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitWhileStatement(ast::WhileStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endWhileStatement(ast::WhileStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBinAndExpression(ast::BinAndExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBinAndExpression(ast::BinAndExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBlockStatement(ast::BlockStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBlockStatement(ast::BlockStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitEqualityExpression(ast::EqualityExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endEqualityExpression(ast::EqualityExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitDoWhileStatement(ast::DoWhileStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endDoWhileStatement(ast::DoWhileStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitRelationalExpression(ast::RelationalExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endRelationalExpression(ast::RelationalExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBasicMember(ast::BasicMember* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBasicMember(ast::BasicMember* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitShiftExpression(ast::ShiftExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endShiftExpression(ast::ShiftExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitStatementOrCase(ast::StatementOrCase* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endStatementOrCase(ast::StatementOrCase* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAddExpression(ast::AddExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAddExpression(ast::AddExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitMemberWithMetaOrAnnotation(ast::MemberWithMetaOrAnnotation* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endMemberWithMetaOrAnnotation(ast::MemberWithMetaOrAnnotation* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitMulExpression(ast::MulExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endMulExpression(ast::MulExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBasicMemberWithMetaOrAnnotation(ast::BasicMemberWithMetaOrAnnotation* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBasicMemberWithMetaOrAnnotation(ast::BasicMemberWithMetaOrAnnotation* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitIfStatement(ast::IfStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endIfStatement(ast::IfStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitUnaryExpression(ast::UnaryExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endUnaryExpression(ast::UnaryExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAssignExpressionOrInitializerList(ast::AssignExpressionOrInitializerList* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAssignExpressionOrInitializerList(ast::AssignExpressionOrInitializerList* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitExpressionOrInitializerList(ast::ExpressionOrInitializerList* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endExpressionOrInitializerList(ast::ExpressionOrInitializerList* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitInitializerList(ast::InitializerList* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endInitializerList(ast::InitializerList* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitExplicitLocalVariableDecl(ast::ExplicitLocalVariableDecl* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endExplicitLocalVariableDecl(ast::ExplicitLocalVariableDecl* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitArrayAccess(ast::ArrayAccess* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endArrayAccess(ast::ArrayAccess* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitKeywordExpression(ast::KeywordExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endKeywordExpression(ast::KeywordExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitNullptrExpression(ast::NullptrExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endNullptrExpression(ast::NullptrExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitPostFixExpression(ast::PostFixExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endPostFixExpression(ast::PostFixExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitCaseStatement(ast::CaseStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endCaseStatement(ast::CaseStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitCStyleCastExpression(ast::CStyleCastExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endCStyleCastExpression(ast::CStyleCastExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitTypeTraitBinary(ast::TypeTraitBinary* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endTypeTraitBinary(ast::TypeTraitBinary* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAutoLocalVariableDecl(ast::AutoLocalVariableDecl* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAutoLocalVariableDecl(ast::AutoLocalVariableDecl* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitAssertStatement(ast::AssertStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endAssertStatement(ast::AssertStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitBreakStatement(ast::BreakStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endBreakStatement(ast::BreakStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitContinueStatement(ast::ContinueStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endContinueStatement(ast::ContinueStatement* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitSizeofExpression(ast::SizeofExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endSizeofExpression(ast::SizeofExpression* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitLastTemplateArgumentAssign(ast::LastTemplateArgumentAssign* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endLastTemplateArgumentAssign(ast::LastTemplateArgumentAssign* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitLastTemplateArgumentNoAssign(ast::LastTemplateArgumentNoAssign* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endLastTemplateArgumentNoAssign(ast::LastTemplateArgumentNoAssign* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitNotLastTemplateArgumentAssign(ast::NotLastTemplateArgumentAssign* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endNotLastTemplateArgumentAssign(ast::NotLastTemplateArgumentAssign* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::visitNotLastTemplateArgumentNoAssign(ast::NotLastTemplateArgumentNoAssign* input)
        {
            return true;
        }
        bool CppLiteGrammarVisitor::endNotLastTemplateArgumentNoAssign(ast::NotLastTemplateArgumentNoAssign* input)
        {
            return true;
        }
    } // namespace phantom
} // namespace lang
