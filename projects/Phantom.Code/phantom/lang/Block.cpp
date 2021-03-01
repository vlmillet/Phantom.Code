// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "Block.h"

#include "BranchIfNotStatement.h"
#include "BranchIfStatement.h"
#include "CallExpression.h"
#include "ConstantExpression.h"
#include "ConstructorCallExpression.h"
#include "CppLite.h"
#include "ExpressionStatement.h"
#include "Label.h"
#include "LocalVariableExpression.h"
#include "LocalVariableInitializationStatement.h"
#include "OwnerGuard.h"
#include "ReturnStatement.h"
#include "Semantic.h"
#include "TemplateDependantExpression.h"

#include <ostream>
#include <phantom/lang/Function.h>
#include <phantom/lang/Method.h>
#include <phantom/lang/Parameter.h>
#include <phantom/lang/Signature.h>
#include <phantom/lang/TemplateDependantElement.h>
#include <phantom/utils/StringUtil.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
Block::Block(int a_iTag /*= 0*/) : Scope(this, this), m_iTag(a_iTag) {}

void Block::addLocalVariable(LocalVariable* a_pVariable)
{
    PHANTOM_ASSERT(a_pVariable && a_pVariable->getName().empty() ||
                   getLocalVariable(a_pVariable->getName()) == nullptr);
    m_LocalVariables.push_back(a_pVariable);
    if (a_pVariable->getOwner() == nullptr)
        a_pVariable->setOwner(this);
    if (a_pVariable->m_iEvalFrameOffset == -1)
    {
        PHANTOM_ASSERT(getSubroutine());
        int& frameSize = getSubroutine()->m_uiFrameSize;
        a_pVariable->m_iEvalFrameOffset = frameSize;
        frameSize += int(a_pVariable->getValueType()->getSize());
    }
    if (!(a_pVariable->getValueType()->hasAuto()))
    {
        Class* pClass = a_pVariable->getValueType() ? a_pVariable->getValueType()->asClass() : nullptr;
        if (pClass)
        {
            if (pClass->isTemplateDependant())
            {
                addScopedDestruction(New<TemplateDependantExpression>(New<TemplateDependantElement>(
                (New<LocalVariableExpression>(a_pVariable))->address(this), String("~") + pClass->getName(), NullOpt,
                ArrayView<LanguageElement*>())));
            }
            else
            {
                auto pCall = New<CallExpression>(pClass->getDestructor(),
                                                 (New<LocalVariableExpression>(a_pVariable))->address(this));
                pCall->setFinal(); // RAII never uses polymorphism
                addScopedDestruction(pCall);
            }
        }
    }
    else
    {
        PHANTOM_ASSERT(isTemplateElement());
    }
}

LocalVariable* Block::addLocalVariable(Type* a_pType, StringView a_strName, Expression* a_pInit /*= nullptr*/,
                                       Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    LocalVariable* pVar = New<LocalVariable>(a_pType, a_strName, a_Modifiers, a_uiFlags);
    addLocalVariable(pVar);
    if (a_pInit)
    {
        addStatement(New<LocalVariableInitializationStatement>(pVar, a_pInit));
    }
    else if (a_pType->asClassType())
    {
        if (!(a_pType->isDefaultInstanciable()))
            return nullptr;
        addStatement(New<LocalVariableInitializationStatement>(
        pVar, New<ConstructorCallExpression>(static_cast<ClassType*>(a_pType)->getDefaultConstructor())));
    }
    return pVar;
}

LocalVariable* Block::addLocalVariable(StringView a_strCode, Language* a_pLanguage)
{
    if (a_pLanguage == nullptr)
        a_pLanguage = CppLite::Get();
    String decl = a_strCode;
    String init;
    size_t pos = decl.find_first_of("(");
    bool   bExplicit = false;
    if (pos != String::npos)
    {
        // explicit init
        bExplicit = true;
        decl = a_strCode.substr(0, pos);
        size_t end = decl.find_last_of(")");
        if (end == String::npos)
            return nullptr;
        init = a_strCode.substr(pos + 1, end - pos - 1);
    }
    else
    {
        pos = decl.find_first_of("=");
        if (pos != String::npos)
        {
            decl = a_strCode.substr(0, pos);
            init = a_strCode.substr(pos + 1);
            bExplicit = false;
        }
    }
    while (decl.back() == ' ' || decl.back() == '\t')
        decl.pop_back();
    pos = decl.find_last_of(" \t");
    if (pos == String::npos)
        return nullptr;
    String type = decl.substr(0, pos);
    String name = decl.substr(pos + 1);
    if (!CppLite::Get()->isIdentifier(name))
        return nullptr;
    Type* pType = a_pLanguage->findType(type, getOwner() ? this : nullptr);
    if (pType == nullptr)
        return nullptr;
    OwnerGuard<Expression> pInitExp = nullptr;
    if (init.size())
    {
        pInitExp = a_pLanguage->expression(init, this);
        if (pInitExp == nullptr)
        {
            return nullptr;
        }
        pInitExp = CppLite::Get()->getDefaultSemantic()->convert(
        pInitExp, pType, bExplicit ? CastKind::Explicit : CastKind::Implicit,
        bExplicit ? UserDefinedFunctions::All : UserDefinedFunctions::ImplicitsOnly, this);
        if (pInitExp == nullptr)
        {
            return nullptr;
        }
    }
    else if (pType->asClassType())
    {
        Constructor* pDefaultCtor = pType->asClassType()->getDefaultConstructor();
        if (pDefaultCtor == nullptr)
            return nullptr;
        pInitExp = New<ConstructorCallExpression>(pDefaultCtor);
    }
    LocalVariable* pVar = addLocalVariable(pType, name);
    if (pInitExp)
    {
        addStatement(New<LocalVariableInitializationStatement>(pVar, pInitExp));
    }
    return pVar;
}

LocalVariable* Block::getLocalVariableCascade(StringView a_strName) const
{
    if (LocalVariable* pResult = getLocalVariable(a_strName))
        return pResult;
    if (Block* pParent = getParentBlock())
    {
        return pParent->getLocalVariableCascade(a_strName);
    }
    return nullptr;
}

LocalVariable* Block::getLocalVariable(StringView a_strName) const
{
    for (auto it = m_LocalVariables.begin(); it != m_LocalVariables.end(); ++it)
    {
        if ((*it)->getName() == a_strName)
        {
            return *it;
        }
    }
    return nullptr;
}

void Block::getLocalVariablesDeep(LocalVariables& out) const
{
    out.insert(out.end(), m_LocalVariables.begin(), m_LocalVariables.end());
    for (auto it = m_BlockStatements.begin(); it != m_BlockStatements.end(); ++it)
    {
        Block* pChildBlock = (*it)->asBlock();
        if (pChildBlock)
        {
            pChildBlock->getLocalVariablesDeep(out);
        }
    }
}

Subroutine* Block::getSubroutine() const
{
    LanguageElement* pElement = getRootBlock()->getOwner();
    return pElement ? pElement->asSubroutine() : nullptr;
}

void Block::addStatement(Statement* a_pStatement)
{
    m_BlockStatements.push_back(a_pStatement);
    _addStatement(a_pStatement);
}

void Block::eval(ExecutionContext& a_Context) const
{
    if (Statement* pStatement = getFirstNonBlockStatementCascade())
    {
        a_Context.setNextStatement(pStatement);
    }
}

void Block::prependStatement(Statement* a_pStatement)
{
    m_BlockStatements.insert(m_BlockStatements.begin(), a_pStatement);
    _addStatement(a_pStatement);
}

void Block::prependStatement(Expression* a_pExpression)
{
    prependStatement(New<ExpressionStatement>(a_pExpression));
}

void Block::prependStatements(const Statements& a_Statements)
{
    for (auto pSt : a_Statements)
        _addStatement(pSt);
    m_BlockStatements.insert(m_BlockStatements.begin(), a_Statements.begin(), a_Statements.end());
}

Statement* Block::getFirstNonBlockStatementCascade() const
{
    for (Statement* pStatement : m_BlockStatements)
    {
        if (pStatement)
        {
            Block* pBlock = pStatement->asBlock();
            if (pBlock)
            {
                Statement* pFirstStatement = pBlock->getFirstNonBlockStatementCascade();
                if (pFirstStatement)
                    return pFirstStatement;
            }
            else
                return pStatement;
        }
    }
    return nullptr;
}

Statement* Block::getNextNonBlockStatement(Statement* a_pStatement) const
{
    size_t count = m_BlockStatements.size();
    bool   found = false;
    for (size_t i = 0; i < count; ++i)
    {
        Statement* pStatement = m_BlockStatements[i];
        if (found)
        {
            if (Block* pBlock = pStatement->asBlock())
            {
                if (Statement* pFirst = pBlock->getFirstNonBlockStatementCascade())
                {
                    return pFirst;
                }
            }
            else
                return pStatement;
        }
        else
        {
            found = (pStatement == a_pStatement);
        }
    }
    if (Block* pParent = getParentBlock())
    {
        return pParent->getNextNonBlockStatement((Statement*)this);
    }
    return nullptr;
}

ExpressionStatement* Block::addStatement(Expression* a_pExpression)
{
    ExpressionStatement* pStat = New<ExpressionStatement>(a_pExpression);
    addStatement(pStat);
    return pStat;
}

ExpressionStatement* Block::addStatement(StringView a_strExpression, Language* a_pLanguage)
{
    Expression* pExpression = a_pLanguage->expression(a_strExpression, this);
    return pExpression ? addStatement(pExpression) : nullptr;
}

ExpressionStatement* Block::addStatement(StringView a_strExpression)
{
    return addStatement(a_strExpression, CppLite::Get());
}

ReturnStatement* Block::addReturnStatement(StringView a_strExpression)
{
    Expression* pExpression = CppLite::Get()->expression(a_strExpression, this);
    if (pExpression == nullptr)
        return nullptr;
    ReturnStatement* pStat = addReturnStatement(pExpression);
    if (pStat == nullptr)
    {
        Delete(pExpression);
    }
    return pStat;
}

ReturnStatement* Block::addReturnStatement()
{
    if (getSubroutine()->getReturnType() == PHANTOM_TYPEOF(void))
    {
        ReturnStatement* pStat = New<ReturnStatement>();
        addStatement(pStat);
        return pStat;
    }
    return nullptr;
}

ReturnStatement* Block::addReturnStatement(Expression* a_pExpression)
{
    PHANTOM_ASSERT(a_pExpression->getValueType()->isSame(getSubroutine()->getReturnType()));
    ReturnStatement* pStat = New<ReturnStatement>(a_pExpression);
    addStatement(pStat);
    return pStat;
}

void Block::dump(std::ostream& a_Out, int a_Lvl) const
{
    int l = a_Lvl;
    while (l--)
        a_Out << "  ";
    a_Out << '{' << std::endl;
    for (auto s : m_BlockStatements)
    {
        s->dump(a_Out, a_Lvl + 1);
    }
    l = a_Lvl;
    while (l--)
        a_Out << "  ";
    a_Out << '}' << std::endl;
}

void Block::getQualifiedName(StringBuffer& a_Buf) const
{
    if (LanguageElement* pOwner = getOwner())
    {
        getOwner()->getQualifiedDecoratedName(a_Buf);
        a_Buf += "::";
    }
    getDecoratedName(a_Buf);
}

void Block::getDecoratedName(StringBuffer& a_Buf) const
{
    size_t index = 0;
    if (LanguageElement* pOwner = getOwner())
    {
        if (Block* pBlock = pOwner->asBlock())
        {
            index = pBlock->getBlockIndex((Block*)this);
        }
    }
    a_Buf += "{" + StringUtil::ToString(index) + "}";
}

void Block::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    getQualifiedName(a_Buf);
}

void Block::getUniqueName(StringBuffer& a_Buf) const
{
    if (LanguageElement* pOwner = getOwner())
    {
        getOwner()->getUniqueName(a_Buf);
        a_Buf += '.';
    }
    getDecoratedName(a_Buf);
}

Expression* Block::createCall(Semantic* a_pSemantic, Method* a_pMethod, Expression* a_pThisExpression,
                              ExpressionsView a_Arguments)
{
    Expression* pThisExpression = a_pThisExpression;
    if (pThisExpression->getValueType()->asReference())
    {
        if (pThisExpression->getValueType()->removeReference()->asPointer() == nullptr)
        // Call from a reference (*this)
        {
            if (!a_pMethod->acceptsCallerExpressionType(a_pThisExpression->getValueType()))
            {
                PHANTOM_LOG(Error, "method '%s' does not accept 'this' expression of type '%s'",
                            a_pMethod->getQualifiedDecoratedName().c_str(),
                            pThisExpression->getValueType()->getQualifiedDecoratedName().c_str());
                return nullptr;
            }
            pThisExpression = a_pThisExpression->address(this);
        }
        else
        {
            pThisExpression = a_pThisExpression->load(this);
        }
    }
    if (pThisExpression->getValueType()->asPointer())
    {
        // Call from a pointer (this)
        Type* pThisPtrType = a_pThisExpression->getValueType();
        if (!a_pMethod->acceptsCallerExpressionType(pThisPtrType->removePointer()->addLValueReference()))
        {
            PHANTOM_LOG(Error, "createCall : method '%s' does not accept 'this' expression of type '%s'",
                        a_pMethod->getQualifiedDecoratedName().c_str(),
                        pThisPtrType->getQualifiedDecoratedName().c_str());
            if (a_pThisExpression != pThisExpression)
            {
                a_pThisExpression->detach(); // ensure original a_pThisExpression is not destroyed
                Delete(pThisExpression);
            }
            return nullptr;
        }
    }
    else
    {
        PHANTOM_LOG(Error,
                    "createCall : only accepts reference or pointer caller expression (equivalent "
                    "ot '*this' or 'this')");
        if (a_pThisExpression != pThisExpression)
        {
            a_pThisExpression->detach(); // ensure original a_pThisExpression is not destroyed
            Delete(pThisExpression);
        }
        return nullptr;
    }
    // at this point, we have '(*this).method(...)'
    if (a_Arguments.size() < a_pMethod->getSignature()->getRequiredArgumentCount())
    {
        PHANTOM_LOG(Error, "createCall : not enought arguments for call of method '%s', %d required, %d given",
                    a_pMethod->getQualifiedDecoratedName().c_str(),
                    a_pMethod->getSignature()->getRequiredArgumentCount(), a_Arguments.size());
        if (a_pThisExpression != pThisExpression)
        {
            a_pThisExpression->detach(); // ensure original a_pThisExpression is not destroyed
            Delete(pThisExpression);
        }
        return nullptr;
    }
    if (a_Arguments.size() > a_pMethod->getParameters().size())
    {
        PHANTOM_LOG(Error, "createCall : too many arguments for call of method '%s', %d expected, %d given",
                    a_pMethod->getQualifiedDecoratedName().c_str(), a_pMethod->getParameters().size(),
                    a_Arguments.size());
        if (a_pThisExpression != pThisExpression)
        {
            a_pThisExpression->detach(); // ensure original a_pThisExpression is not destroyed
            Delete(pThisExpression);
        }
        return nullptr;
    }
    bool        success = true;
    Expressions args;
    for (size_t i = 0; i < a_Arguments.size(); ++i)
    {
        Expression* pArgument = a_Arguments[i];
        if (!(pArgument->hasEvalConversionTo(a_pMethod->getParameterType(i))))
        {
            Expression* pConverted = a_pSemantic->convert(pArgument, a_pMethod->getParameterType(i), this);
            if (pConverted == nullptr)
            {
                PHANTOM_LOG(Error,
                            "createCall : method '%s' : no implicit conversion from argument %d "
                            "type '%s' to parameter type '%s'",
                            a_pMethod->getQualifiedDecoratedName().c_str(), i,
                            a_Arguments[i]->getValueType()->getQualifiedDecoratedName().c_str(),
                            a_pMethod->getParameterType(i)->getQualifiedDecoratedName().c_str());
                success = false;
                pArgument->detach();
                args.push_back(nullptr);
            }
            else
            {
                args.push_back(pConverted);
            }
        }
        else
        {
            args.push_back(pArgument);
        }
    }
    // if not succeeded, clean up arguments
    if (!success)
    {
        for (size_t i = 0; i < args.size(); ++i)
        {
            if (args[i] && args[i] != a_Arguments[i])
            {
                a_Arguments[i]->detach();
                Delete(args[i]);
            }
        }
        return nullptr;
    }
    args.insert(args.begin(), pThisExpression);
    return New<CallExpression>(a_pMethod, args);
}

Expression* Block::createCall(Semantic* a_pSemantic, Method* a_pMethod, ExpressionsView a_Arguments)
{
    if (getSubroutine()->asMethod() == nullptr)
    {
        PHANTOM_LOG(Error, "createCall : block does not belong to a member function");
        return nullptr;
    }
    Expression* pThisExpr = New<LocalVariableExpression>(getSubroutine()->asMethod()->getThis());
    PHANTOM_ASSERT(pThisExpr);
    Expression* pResult = createCall(a_pSemantic, a_pMethod, pThisExpr, a_Arguments);
    if (pResult == nullptr)
    {
        Delete(pThisExpr);
        return nullptr;
    }
    return pResult;
}

Expression* Block::createCall(Semantic* a_pSemantic, Function* a_pFunction, ExpressionsView a_Arguments)
{
    // at this point, we have '(*this).method(...)'
    if (a_Arguments.size() < a_pFunction->getSignature()->getRequiredArgumentCount())
    {
        PHANTOM_LOG(Error, "createCall : not enought arguments for call of method '%s', %d required, %d given",
                    a_pFunction->getQualifiedDecoratedName().c_str(),
                    a_pFunction->getSignature()->getRequiredArgumentCount(), a_Arguments.size());
        return nullptr;
    }
    if (a_Arguments.size() > a_pFunction->getParameters().size())
    {
        PHANTOM_LOG(Error, "createCall : too many arguments for call of method '%s', %d expected, %d given",
                    a_pFunction->getQualifiedDecoratedName().c_str(), a_pFunction->getParameters().size(),
                    a_Arguments.size());
        return nullptr;
    }
    bool        success = true;
    Expressions args;
    for (size_t i = 0; i < a_Arguments.size(); ++i)
    {
        Expression* pArgument = a_Arguments[i];
        if (!(pArgument->hasEvalConversionTo(a_pFunction->getParameterType(i))))
        {
            Expression* pConverted = a_pSemantic->convert(pArgument, a_pFunction->getParameterType(i), this);
            if (pConverted == nullptr)
            {
                PHANTOM_LOG(Error,
                            "createCall : method '%s' : no implicit conversion from argument %d "
                            "type '%s' to parameter type '%s'",
                            a_pFunction->getQualifiedDecoratedName().c_str(), i,
                            a_Arguments[i]->getValueType()->getQualifiedDecoratedName().c_str(),
                            a_pFunction->getParameterType(i)->getQualifiedDecoratedName().c_str());
                success = false;
                pArgument->detach();
                args.push_back(nullptr);
            }
            else
            {
                args.push_back(pConverted);
            }
        }
        else
        {
            args.push_back(pArgument);
        }
    }
    // if not succeeded, clean up arguments
    if (!success)
    {
        for (size_t i = 0; i < args.size(); ++i)
        {
            if (args[i] && args[i] != a_Arguments[i])
            {
                a_Arguments[i]->detach();
                Delete(args[i]);
            }
        }
        return nullptr;
    }
    return New<CallExpression>(a_pFunction, args);
}

ExpressionStatement* Block::addCall(Semantic* a_pSemantic, Method* a_pMethod, Expression* a_pThisExpression,
                                    ExpressionsView a_Arguments)
{
    Expression* pExp = createCall(a_pSemantic, a_pMethod, a_pThisExpression, a_Arguments);
    return pExp ? addStatement(pExp) : nullptr;
}

ExpressionStatement* Block::addCall(Semantic* a_pSemantic, Method* a_pMethod, ExpressionsView a_Arguments)
{
    Expression* pExp = createCall(a_pSemantic, a_pMethod, a_Arguments);
    return pExp ? addStatement(pExp) : nullptr;
}

ExpressionStatement* Block::addCall(Semantic* a_pSemantic, Function* a_pFunction, ExpressionsView a_Arguments)
{
    Expression* pExp = createCall(a_pSemantic, a_pFunction, a_Arguments);
    return pExp ? addStatement(pExp) : nullptr;
}

Block* Block::getParentBlock() const
{
    return getOwner() ? getOwner()->asBlock() : nullptr;
}

void Block::fetchControlScopedDestructions(Expressions& out, Block* a_pUntilBlock) const
{
    if (a_pUntilBlock == this)
        return;

    auto& destrs = getScopedDestructions();
    for (auto destr : destrs)
    {
        out.push_back(destr);
    }
    if (getParentBlock())
    {
        getParentBlock()->fetchControlScopedDestructions(out, a_pUntilBlock);
    }
}

void Block::_addStatement(Statement* a_pStatement)
{
    a_pStatement->setOwner(this);
    a_pStatement->onAttachedToBlock(this);
}

void Block::insertStatementAfter(Statement* a_pAfterMe, Statement* a_pStatement)
{
    for (auto it = m_BlockStatements.begin(); it != m_BlockStatements.end(); ++it)
    {
        if (*it == a_pAfterMe)
        {
            ++it;
            m_BlockStatements.insert(it, a_pStatement);
            _addStatement(a_pStatement);
            return;
        }
    }
    PHANTOM_ASSERT(false);
}

LocalVariableInitializationStatement* Block::findLocalVariableInitializationStatement(LocalVariable* a_pLocal) const
{
    if (a_pLocal->getBlock() != this)
        return nullptr;
    for (auto it = m_BlockStatements.begin(); it != m_BlockStatements.end(); ++it)
    {
        LocalVariableInitializationStatement* pInit = (*it)->asLocalVariableInitializationStatement();
        if (pInit && pInit->getLocalVariable() == a_pLocal)
        {
            return pInit;
        }
    }
    return nullptr;
}

size_t Block::getBlockIndex(Block* a_pSubBlock) const
{
    size_t index = 0;
    for (auto s : m_BlockStatements)
    {
        if (s == a_pSubBlock)
        {
            break;
        }
        index += (s->rtti.metaClass == PHANTOM_CLASSOF(Block)); // increment only if statement is a block
    }
    return ~0ull;
}

Label* Block::findLabelWithTagCascade(int a_iTag) const
{
    for (auto it = m_BlockStatements.begin(); it != m_BlockStatements.end(); ++it)
    {
        Label* pLabel = (*it)->asLabel();
        if (pLabel && pLabel->getTag() == a_iTag)
            return pLabel;
    }
    if (getParentBlock())
    {
        return getParentBlock()->findLabelWithTagCascade(a_iTag);
    }
    return nullptr;
}

Statement* Block::getNextStatementOf(Statement* a_pStatement) const
{
    size_t count = m_BlockStatements.size();
    if (count > 1)
    {
        Statement* pPrevStatement = m_BlockStatements[count - 1];
        size_t     i = count - 1;
        while (i--)
        {
            Statement* pStatement = m_BlockStatements[i];
            if (pStatement == a_pStatement)
                return pPrevStatement;
            pPrevStatement = pStatement;
        }
    }
    return getNextStatement();
}

} // namespace lang
} // namespace phantom
