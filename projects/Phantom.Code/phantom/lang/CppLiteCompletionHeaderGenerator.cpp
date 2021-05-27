#include "CppLiteCompletionHeaderGenerator.h"

#include "phantom/lang/Alias.h"
#include "phantom/lang/Application.h"
#include "phantom/lang/FunctionPointer.h"
#include "phantom/lang/FunctionType.h"
#include "phantom/lang/Module.h"
#include "phantom/lang/Package.h"
#include "phantom/lang/Placeholder.h"
#include "phantom/lang/Plugin.h"
#include "phantom/lang/Template.h"
#include "phantom/lang/TemplateDependantTemplateInstance.h"
#include "phantom/lang/TemplateParameter.h"
#include "phantom/lang/TemplateSignature.h"
#include "phantom/lang/TemplateSpecialization.h"
#include "phantom/lang/Variable.h"
#include "phantom\utils\Path.h"
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    include <windows.h>
#endif
#include <fstream>
#include <sstream>
#include <string>

namespace phantom
{
namespace lang
{
void CppLiteCompletionHeaderGenerator::Printer::_vPrintf(const char* _format, va_list _va)
{
    if (!indentConsumed)
    {
        if (file)
        {
            int tabs = indentLvl;
            while (tabs--)
                fputc('\t', file);
        }
        else
        {
            int tabs = indentLvl;
            while (tabs--)
                *str += '\t';
        }
        indentConsumed = true;
    }
    char buffer[1024];
    vsnprintf(buffer, 1024, _format, _va);
    if (file)
    {
        fprintf(file, buffer);
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
        if (file == stdout)
            OutputDebugStringA(buffer);
#endif
        fflush(file);
    }
    else
    {
        *str += buffer;
    }
    char* pc = buffer;
    while (char c = *pc++)
    {
        if (c == '\n')
        {
            indentConsumed = false;
            break;
        }
    }
}

void CppLiteCompletionHeaderGenerator::Printer::_Printf(const char* _format, ...)
{
    va_list args;
    va_start(args, _format);
    _vPrintf(_format, args);
    va_end(args);
}

CppLiteCompletionHeaderGenerator::Printer::Printer() : file(stdout) {}

CppLiteCompletionHeaderGenerator::Printer::Printer(FILE* _cfile) : file(_cfile) {}

CppLiteCompletionHeaderGenerator::Printer::Printer(String& _str) : str(&_str) {}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(StringView _txt)
{
    _Printf("%.*s", PHANTOM_STRING_AS_PRINTF_ARG(_txt));
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(const char* _str)
{
    _Printf("%s", _str);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(bool _val)
{
    _Printf(_val ? "true" : "false");
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(char _val)
{
    _Printf("%c", _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(int8 _val)
{
    _Printf("%" PRIi8, _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(uint8 _val)
{
    _Printf("%" PRIu8, _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(int16 _val)
{
    _Printf("%" PRIi16, _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(uint16 _val)
{
    _Printf("%" PRIu16, _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(int32 _val)
{
    _Printf("%" PRIi32, _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(uint32 _val)
{
    _Printf("%" PRIu32, _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(int64 _val)
{
    _Printf("%" PRIi64, _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(uint64 _val)
{
    _Printf("%" PRIu64, _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(float _val)
{
    _Printf("%f", _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(double _val)
{
    _Printf("%lf", _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::operator()(void* _val)
{
    _Printf("%p", _val);
    return *this;
}

CppLiteCompletionHeaderGenerator::Printer& CppLiteCompletionHeaderGenerator::Printer::Ln()
{
    _Printf("\n");
    if (file)
        fflush(file);
    return *this;
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppParameter(Parameter* _input, Printer& _printer)
{
    if (&_printer == &hpp)
        if (_input->getValueType()->asClassType() || _input->getValueType()->asEnum())
            PrintHppInclude(_input->getValueType());
        else
            PrintForward(_input->getValueType());
    String name = StringView(_input->getName());
    if (name.empty())
        PrintName(_input->getValueType(), _printer);
    else
        PrintVarName(_input->getValueType(), name, _printer);
    if (_input->getNativeDefaultArgumentString().size())
    {
        if (_input->getEnclosingTemplateSpecialization())
        {
            // FIXME : do real expression translation here when implemented, for now it's just a placeholder
            // for parsing correctly
            auto pNoRefType = _input->getValueType()->removeReference()->removeQualifiers();
            if (pNoRefType->asPointer() || pNoRefType->asArray())
                _printer(" = nullptr");
            else
                _printer(" = ")(pNoRefType->getQualifiedDecoratedName())("{}");
        }
        else
        {
            PrintHppInclude(_input->getValueType()->removeConstReference());
            _printer(" = ")(_input->getNativeDefaultArgumentString());
        }
    }
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppParameters(Subroutine* _input, Printer& _printer,
                                                                         size_t _firstIdx /*= 0*/,
                                                                         bool   _append /*= false*/)
{
    int c = _append;
    for (size_t i = _firstIdx; i < _input->getParameters().size(); ++i)
    {
        Parameter* param = _input->getParameters()[i];
        if (c++)
            _printer(", ");
        PrintCppParameter(param, _printer);
    }
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSuffix(Subroutine* _input, Printer& _printer)
{
    if (_input->isConst())
        _printer(" const");
    if (_input->isVolatile())
        _printer(" volatile");
    if (_input->testModifiers(Modifier::Enum::Override))
        _printer(" override");
    if (_input->testModifiers(Modifier::Enum::Final))
        _printer(" final");
}

bool CppLiteCompletionHeaderGenerator::SourcePrinter::PrintGeneric(Subroutine* _input, Printer& _printer)
{
    enum class ParamType : int
    {
        Normal,
        Param,
        Arg
    };
    struct Param
    {
        Parameter* param{};
        ParamType  type = ParamType::Normal;
    };

    SmallVector<Param> params;

    bool                gen = false;
    SmallVector<size_t> args;
    size_t              retTypeIdx = -1;
    Type*               retType = _input->getReturnType();

    int genParamIndex = 0;
    // handle special Generic cases
    if (retType->getName() == "Return" && retType->getQualifiedName() == "phantom::Generic::Return")
    {
        auto spec = retType->getTemplateSpecialization();
        auto paramParam = spec->getArgument(0)->asType()->removeEverything();
        auto paramParamSpec = paramParam->getTemplateSpecialization();
        auto paramParamSpecCst = paramParamSpec->getArgument(0)->asConstant();
        paramParamSpecCst->getValue(&retTypeIdx);
        gen = true;
    }

    for (auto param : _input->getParameters())
    {
        Type* type = param->getValueType();
        if (type->getName() == "Param" && type->getQualifiedName() == "phantom::Generic::Param")
        {
            params.push_back({param, ParamType::Param});
            gen = true;
        }
        else if (type->getName() == "Arg" && type->getQualifiedName() == "phantom::Generic::Arg")
        {
            params.push_back({param, ParamType::Arg});
            gen = true;
        }
        else
        {
            params.push_back({param, ParamType::Normal});
        }
    }

    if (!gen)
        return false;

    _printer("template<");
    int  genParamCnt = 0;
    bool paramsReadStep = 0;
    for (auto& p : params)
    {
        if (p.type == ParamType::Param)
        {
            if (paramsReadStep == 2)
            {
                PHANTOM_LOG(Error,
                            "invalid generic method or function found while generating completion headers : %.*s",
                            PHANTOM_STRING_AS_PRINTF_ARG(_input->getQualifiedDecoratedName()));
            }
            if (genParamCnt)
                _printer(", ");
            _printer("class T")(genParamCnt);
            ++genParamCnt;
            paramsReadStep = 1;
        }
        else if (paramsReadStep == 1)
        {
            paramsReadStep = 2;
        }
    }
    _printer(">").Ln();
    if (_input->asFunction() && _input->getOwner()->asClassType())
        _printer("static ");
    if (retTypeIdx == -1)
        PrintName(_input->getReturnType(), _printer);
    else
        _printer("T")(retTypeIdx)("*");
    _printer(" ");
    _printer(_input->getName())("(");

    size_t c = 0;
    for (size_t i = genParamCnt; i < params.size(); ++i)
    {
        if (c++)
            _printer(", ");
        if (params[i].type == ParamType::Arg)
        {
            size_t arg = -1;
            auto   spec = params[i].param->getValueType()->getTemplateSpecialization();
            auto   paramParam = spec->getArgument(0)->asType()->removeEverything();
            auto   paramParamSpec = paramParam->getTemplateSpecialization();
            auto   paramParamSpecCst = paramParamSpec->getArgument(0)->asConstant();
            paramParamSpecCst->getValue(&arg);
            StringView name = params[i].param->getName();
            _printer("T")(arg)("*")(" ")(name);
        }
        else
        {
            PrintCppParameter(params[i].param, _printer);
        }
    }
    _printer(")");
    PrintCppSuffix(_input, _printer);
    _printer(";").Ln();
    return true;
}

String CppLiteCompletionHeaderGenerator::SourcePrinter::GetHppIncludePath(Source* _source)
{
    String hpp_rel_path = _source->getUniqueName();

    std::for_each(hpp_rel_path.begin(), hpp_rel_path.end(), [](char& c) {
        if (c == '.')
            c = '/';
    });
    return hpp_rel_path + ".hpp";
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintHppInclude(Source* _source)
{
    if (_source->getOwner() == nullptr || source == _source || _source->getName() == "default" ||
        _source->getName().empty())
        return;

    includes[GetHppIncludePath(_source)];
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintHppInclude(Symbol* _input)
{
    if (auto alias = getAlias(_input))
        PrintHppInclude(alias);

    Source* includeSource{};
    if (PHANTOM_CLASSOF(TemplateDependantTemplateInstance) == _input->getMetaClass())
    {
        TemplateDependantTemplateInstance* tdi = static_cast<TemplateDependantTemplateInstance*>(_input);
        for (auto arg : tdi->getArguments())
        {
            PrintHppInclude(arg->asSymbol());
        }
        includeSource = tdi->getTemplate()->getSource();
    }
    else
    {
        if (TemplateSpecialization* spec = _input->getTemplateSpecialization())
        {
            if (spec->getInstantiationSpecialization())
            {
                includeSource = spec->getInstantiationSpecialization()->getSource();
            }
            else if (spec->testFlags(PHANTOM_R_FLAG_IMPLICIT))
            {
                PrintHppInclude(spec->getTemplate());
            }
            else
            {
                includeSource = spec->getSource();
            }
        }
        else if (Template* pTemplate = _input->asTemplate())
        {
            PrintHppInclude(pTemplate->getSource());
            for (auto spec : pTemplate->getTemplateSpecializations())
            {
                if (spec->isPartial())
                {
                    PrintHppInclude(spec->getSource());
                }
            }
        }
        else
        {
            includeSource = _input->getSource();
        }
    }
    if (includeSource)
        PrintHppInclude(includeSource);
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintUsingTemplateSignature(TemplateSignature* _input)
{
    PrintTemplateSignature(_input, usingfile);
}

CppLiteCompletionHeaderGenerator::SourcePrinter::SourcePrinter(Source* _source, Printer& _hpp, Printer& _ufile,
                                                               bool _enableModules)
    : source(_source), hpp(_hpp), usingfile(_ufile), enableModules(_enableModules)
{
}

phantom::String CppLiteCompletionHeaderGenerator::SourcePrinter::ProtectQuote(StringView _str)
{
    String str;
    for (auto c : _str)
    {
        if (c == '\r')
            str += "\\r";
        else if (c == '\n')
            str += "\\n";
        else if (c == '\t')
            str += "\\t";
        else
            str += c;
    }
    return str;
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintUsing(Template* _input)
{
    PrintUsingTemplateSignature(_input->getTemplateSignature());
    usingfile(" using ")(_input->getName())(" = ")(
    static_cast<LanguageElement*>(_input->getEmptyTemplateSpecialization())->getQualifiedDecoratedName())(";")
    .Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintUsing(Function* _input)
{
    if (_input->getFlags() & (int)0x08000000)
        return;
    if (_input->getName().empty() || _input->getNamingScope() == Namespace::Global())
        return;
    usingfile("using ");
    usingfile(_input->getQualifiedName())(";").Ln();
    // 		if (PrintGeneric(_input, usingfile))
    // 			return;
    // 		if (_input->isStatic() || _input->asFunction() && _input->getOwner()->asClass())
    // 			usingfile("static ");
    // 		usingfile(_input->getReturnType()->getQualifiedDecoratedName());
    // 		usingfile(" ");
    // 		usingfile(_input->getName())("(");
    // 		PrintCppParameters(_input, usingfile);
    // 		usingfile(");").Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintUsing(Symbol* _input)
{
    if (_input->getName().empty() || _input->getNamingScope() == Namespace::Global())
        return;
    if (_input->getOwner() && _input->getOwner()->asTemplateSpecialization())
        return;
    if (_input->asTemplateSpecialization())
        return;
    usingfile("using ")(_input->getQualifiedDecoratedName())(";").Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintUsing(Type* _input)
{
    if (_input->getName().empty() || _input->getNamingScope() == Namespace::Global())
        return;
    usingfile("using ")(_input->getName())(" = ")(_input->getQualifiedDecoratedName())(";").Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintUsing(Alias* _input)
{
    if (_input->getName().empty() || _input->getNamingScope() == Namespace::Global())
        return;
    usingfile("using ")(_input->getName())(" = ")(_input->getQualifiedDecoratedName())(";").Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintTemplateSignature(TemplateSignature* _input,
                                                                             Printer&           _printer)
{
    _printer("template<");
    int c = 0;
    for (TemplateParameter* param : _input->getTemplateParameters())
    {
        if (c++)
            _printer(", ");
        if (param->getPlaceholder()->asSymbol()->asType())
            _printer("class ");
        else if (Constant* cst = param->getPlaceholder()->asSymbol()->asConstant())
        {
            _printer(cst->getValueType()->getQualifiedName());
            _printer(" ");
        }
        _printer(param->getPlaceholder()->asSymbol()->getName());
        if (param->getDefaultArgument())
        {
            PrintHppInclude(param->getDefaultArgument()->asSymbol());
            _printer(" = ");
            _printer(param->getDefaultArgument()->getQualifiedDecoratedName());
        }
    }
    _printer(">").Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintTemplateDecoration(TemplateSpecialization* _spec)
{
    hpp("<");
    int c = 0;
    for (auto arg : _spec->getArguments())
    {
        if (c++)
            hpp(", ");
        PrintName(arg);
    }
    hpp(">");
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintName(Symbol* _symbol, Printer& _printer)
{
    // Aliasing

    if (auto type = _symbol->asType())
    {
        auto typeNaked = type->removeEverything();

        if (m_templatePlaceholderSpec)
        {
            size_t index = 0;
            for (auto elem : m_templatePlaceholderSpec->getArguments())
            {
                if (auto arg = elem->asType())
                {
                    Types replicationTypes;
                    auto  currTypeLvl = type;
                    while (currTypeLvl)
                    {
                        if (arg == currTypeLvl)
                        {
                            Type*  tplArgType = static_cast<Type*>(m_templatePlaceholderSpec->getTemplate()
                                                                  ->getEmptyTemplateSpecialization()
                                                                  ->getTemplateParameters()[index]
                                                                  ->getPlaceholder()
                                                                  ->asSymbol());
                            size_t c = replicationTypes.size();
                            while (c--)
                            {
                                currTypeLvl = replicationTypes[c];
                                if (currTypeLvl->asConstType())
                                {
                                    if (currTypeLvl->asVolatileType())
                                    {
                                        tplArgType = tplArgType->addConstVolatile();
                                    }
                                    else
                                    {
                                        tplArgType = tplArgType->addConst();
                                    }
                                }
                                else if (currTypeLvl->asVolatileType())
                                {
                                    if (currTypeLvl->asConstType())
                                    {
                                        tplArgType = tplArgType->addConstVolatile();
                                    }
                                    else
                                    {
                                        tplArgType = tplArgType->addVolatile();
                                    }
                                }
                                else
                                {
                                    switch (currTypeLvl->getTypeKind())
                                    {
                                    case TypeKind::Array:
                                        tplArgType =
                                        tplArgType->addArray(static_cast<Array*>(currTypeLvl)->getItemCount());
                                        break;
                                    case TypeKind::Pointer:
                                        tplArgType = tplArgType->addPointer();
                                        break;
                                    case TypeKind::LValueReference:
                                        tplArgType = tplArgType->addLValueReference();
                                        break;
                                    case TypeKind::RValueReference:
                                        tplArgType = tplArgType->addRValueReference();
                                        break;
                                    default:
                                        break;
                                    }
                                }
                            }
                            _printer(tplArgType->getDecoratedName());
                            return;
                        }
                        replicationTypes.push_back(currTypeLvl);
                        currTypeLvl = currTypeLvl->getUnderlyingType();
                    }
                }
                ++index;
            }
        }
        else
        {
            if (m_noAlias == 0)
            {
                auto typeNakedAlias = getAlias(typeNaked);

                if (typeNakedAlias)
                {
                    if (auto asType = typeNakedAlias->asType())
                        return PrintName(type->replicate(asType), _printer);

                    if (auto asAlias = typeNakedAlias->asAlias())
                    {
                        if (auto arg = asAlias->getAliasedSymbol()->asType())
                        {
                            Types replicationTypes;
                            auto  currTypeLvl = type;
                            while (currTypeLvl)
                            {
                                if (arg == currTypeLvl)
                                {
                                    StringBuffer aliasedName;
                                    asAlias->getRelativeName(currentScope, aliasedName);

                                    size_t c = replicationTypes.size();
                                    while (c--)
                                    {
                                        currTypeLvl = replicationTypes[c];
                                        if (currTypeLvl->asConstType())
                                        {
                                            if (currTypeLvl->asVolatileType())
                                            {
                                                aliasedName += " const volatile";
                                            }
                                            else
                                            {
                                                aliasedName += " const";
                                            }
                                        }
                                        else if (currTypeLvl->asVolatileType())
                                        {
                                            if (currTypeLvl->asConstType())
                                            {
                                                aliasedName += " const volatile";
                                            }
                                            else
                                            {
                                                aliasedName += " volatile";
                                            }
                                        }
                                        else
                                        {
                                            switch (currTypeLvl->getTypeKind())
                                            {
                                            case TypeKind::Array:
                                                aliasedName += '[';
                                                aliasedName +=
                                                lexical_cast<String>(static_cast<Array*>(currTypeLvl)->getItemCount());
                                                aliasedName += ']';
                                                break;
                                            case TypeKind::Pointer:
                                                aliasedName += '*';
                                                break;
                                            case TypeKind::LValueReference:
                                                aliasedName += '&';
                                                break;
                                            case TypeKind::RValueReference:
                                                aliasedName += "&&";
                                                break;
                                            default:
                                                break;
                                            }
                                        }
                                    }
                                    _printer(aliasedName);
                                    return;
                                }
                                replicationTypes.push_back(currTypeLvl);
                                currTypeLvl = currTypeLvl->getUnderlyingType();
                            }
                        }
                    }
                }
            }
        }
        if (auto typeOwnr = typeNaked->getOwner())
            if (auto pTpl = typeOwnr->asTemplateSpecialization())
            {
                TemplateSpecialization* replacingSpec = nullptr;
                Template*               template_ = pTpl->getTemplate();
                auto                    alias = getAlias(template_);
                if (alias)
                {
                    if (auto aliasTpl = alias->asTemplate())
                    {
                        template_ = aliasTpl;

                        for (auto aliasSpec : aliasTpl->getTemplateSpecializations())
                        {
                            if (aliasSpec->isFull())
                            {
                                if (auto tpled = aliasSpec->getTemplated())
                                    if (auto tpledAsAlias = tpled->asAlias())
                                    {
                                        if (tpledAsAlias->getAliasedSymbol()->isSame(type))
                                        {
                                            replacingSpec = aliasSpec;
                                            break;
                                        }
                                    }
                            }
                        }
                    }
                }

                Symbol* nakedSym = alias;
                if (!nakedSym)
                    nakedSym = typeNaked;

                StringBuffer original;
                type->getRelativeDecoratedName(currentScope, original);

                StringView suffix = original;
                size_t     p = suffix.find_last_of('>');
                suffix = suffix.substr(p + 1);

                StringBuffer relName;
                nakedSym->getRelativeName(currentScope, relName);
                _printer(relName);
                _printer("<");
                if (replacingSpec)
                    pTpl = replacingSpec;
                size_t c = 0;
                for (auto arg : pTpl->getArguments())
                {
                    if ((replacingSpec || !alias) && pTpl->getDefaultArgument(c))
                        break;
                    if (c)
                        _printer(", ");
                    if (auto argType = arg->asType())
                    {
                        PrintName(argType, _printer);
                    }
                    else
                    {
                        _printer(arg->getRelativeDecoratedName(currentScope));
                    }
                    ++c;
                }
                _printer("> ");
                _printer(suffix);
                return;
            }
    }
    _printer(_symbol->getRelativeDecoratedName(currentScope));
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintName(Symbol* _symbol)
{
    PrintName(_symbol, hpp);
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintName(LanguageElement* _elem, Printer& _printer)
{
    if (Symbol* sym = _elem->asSymbol())
    {
        PrintName(sym, _printer);
    }
    else
    {
        assert(false);
    }
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintName(LanguageElement* _elem)
{
    PrintName(_elem, hpp);
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintVarName(Type* _type, StringView _name, Printer& _printer)
{
    if (_type->getMetaClass()->isA(PHANTOM_CLASSOF(Array)))
    {
        PrintVarName(_type->getUnderlyingType(), _name, _printer);
        _printer(_type->getName());
    }
    else if (_type->getMetaClass()->isA(PHANTOM_CLASSOF(FunctionPointer)))
    {
        auto fp = static_cast<FunctionPointer*>(_type);
        PrintName(fp->getFunctionType()->getReturnType());
        _printer("(*");
        _printer(_name);
        _printer(")(");
        for (size_t i = 0; i < fp->getFunctionType()->getParameterTypes().size(); ++i)
        {
            Type* tp = fp->getFunctionType()->getParameterTypes()[i];
            if (&_printer == &hpp)
                PrintForward(tp);
            if (i)
                _printer(", ");
            PrintName(tp);
        }
        _printer(")");
    }
    else
    {
        PrintName(_type, _printer);
        _printer(" ")(_name);
    }
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintMembers(ClassType* _input, Access _access)
{
    PrintCppSymbols(static_cast<Scope*>(_input), _access);
    for (auto elem : _input->getConstructors())
    {
        if (elem->testFlags(PHANTOM_R_FLAG_IMPLICIT)) // implicit
            continue;
        if (elem->getAccess() == _access)
        {
            PrintCppSymbols(elem);
        }
    }
    for (auto elem : _input->getMethods())
    {
        if (elem->testFlags(PHANTOM_R_FLAG_IMPLICIT)) // implicit
            continue;
        if (elem->getAccess() == _access)
        {
            PrintCppSymbols(elem);
        }
    }
    for (auto elem : _input->getFields())
    {
        if (elem->testFlags(PHANTOM_R_FLAG_IMPLICIT)) // implicit
            continue;
        if (elem->getAccess() == _access)
        {
            PrintCppSymbols(elem);
        }
    }
    for (auto elem : _input->getFriends())
    {
        if (elem->testFlags(PHANTOM_R_FLAG_IMPLICIT)) // implicit
            continue;
        if (Access::Private == _access)
        {
            auto spec = elem->getTemplateSpecialization();
            if (spec)
            {
                PrintTemplateSignature(spec->getTemplateSignature(), hpp);
            }
            hpp("friend ");
            if (elem->asClass())
                hpp("class ");
            PrintName(elem);
            hpp(";").Ln();
        }
    }
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::SetCurrentNamespace(Namespace* _ns, Printer& _print)
{
    assert(currentScope->asNamespace());
    if (namespace_ == _ns)
        return;
    while (namespace_->hasOwnerCascade(_ns))
    {
        _print("} // namespace ")(_ns->getName()).Ln();
        namespace_ = namespace_->getParentNamespace();
        currentScope = namespace_;
    }
    if (namespace_ == _ns)
        return;

    if (_ns->hasOwnerCascade(namespace_))
    {
        SmallVector<Namespace*> revert;
        while (_ns && namespace_ != _ns)
        {
            revert.push_back(_ns);
            _ns = _ns->getParentNamespace();
        }
        if (auto i = revert.size())
        {
            namespace_ = revert.front();
            currentScope = namespace_;
            while (i--)
                _print("namespace ")(revert[i]->getName())(" {").Ln();
        }
    }
    else
    {
        SetCurrentNamespace(Namespace::Global(), _print);
        SetCurrentNamespace(_ns, _print);
    }
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::SetCurrentNamespace(Namespace* _ns)
{
    SetCurrentNamespace(_ns, hpp);
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Scope* _input, Access _access)
{
    for (auto elem : _input->getAliases())
    {
        if (elem->getAccess() == _access)
        {
            if (_access == Access::Undefined)
            {
                SetCurrentNamespace(elem->getNamespace());
                if (enableModules)
                    hpp("export ");
                PrintCppSymbols(elem);
                PrintUsing(elem);
            }
            else
            {
                PrintCppSymbols(elem);
            }
        }
    }
    for (auto elem : _input->getConstants())
    {
        if (elem->getAccess() == _access)
        {
            if (_access == Access::Undefined)
            {
                SetCurrentNamespace(elem->getNamespace());
                if (enableModules)
                    hpp("export ");
                PrintCppSymbols(elem);
                PrintUsing(elem);
            }
            else
            {
                PrintCppSymbols(elem);
            }
        }
    }
    for (auto elem : _input->getVariables())
    {
        if (elem->getAccess() == _access)
        {
            if (_access == Access::Undefined)
            {
                SetCurrentNamespace(elem->getNamespace());
                if (enableModules)
                    hpp("export ");
                PrintCppSymbols(elem);
                PrintUsing(elem);
            }
            else
            {
                PrintCppSymbols(elem);
            }
        }
    }

    // enums
    for (auto elem : _input->getTypes())
    {
        if (elem->asEnum() == nullptr)
            continue;
        if (elem->getAccess() == _access)
        {
            if (_access == Access::Undefined)
            {
                SetCurrentNamespace(elem->getNamespace());
                if (enableModules)
                    hpp("export ");
                PrintCppSymbols(elem);
                PrintUsing(elem);
            }
            else
            {
                PrintCppSymbols(elem);
            }
        }
    }
    for (auto elem : _input->getTemplates())
    {
        if (elem->getAccess() == _access)
        {
            if (_access == Access::Undefined)
            {
                SetCurrentNamespace(elem->getNamespace());
                if (enableModules)
                    hpp("export ");
                PrintCppSymbols(elem);
                PrintUsing(elem);
            }
            else
            {
                PrintCppSymbols(elem);
            }
        }
    }

    TemplateSpecializations fullSpecs;
    TemplateSpecializations partialSpecs;

    for (auto elem : _input->getTemplateSpecializations())
    {
        if (elem->testFlags(PHANTOM_R_FLAG_IMPLICIT) || elem->isEmpty())
            continue;
        if (elem->isFull())
        {
            fullSpecs.push_back(elem);
        }
        else
        {
            partialSpecs.push_back(elem);
        }
    }

    for (auto partialSpec : partialSpecs)
    {
        auto PrintLbda = [&]() {
            // deduce template partial specialization
            for (TemplateSpecialization* spec : partialSpec->getTemplate()->getTemplateSpecializations())
            {
                if (spec->getVisibility() == Visibility::Private ||
                    (!spec->isNative() && spec->testFlags(PHANTOM_R_FLAG_IMPLICIT))) // implicit non native
                    continue;
                if (auto extended = spec->getExtendedSpecialization())
                    spec = extended;

                if (spec->getTemplated())
                {
                    if (spec->isFull())
                    {
                        bool everyArgMatch = true;
                        for (size_t i = 0; i < partialSpec->getArguments().size(); ++i)
                        {
                            if (auto pArgType = partialSpec->getArguments()[i]->asType())
                            {
                                if (auto ph = pArgType->removeEverything()->asPlaceholder())
                                {
                                    auto replicated =
                                    spec->getArguments()[i]->asType()->replicate(static_cast<Type*>(ph->asSymbol()));
                                    if (!replicated->isSame(pArgType))
                                    {
                                        everyArgMatch = false;
                                        break;
                                    }
                                }
                                else if (!spec->getArguments()[i]->asType()->isSame(pArgType))
                                {
                                    everyArgMatch = false;
                                    break;
                                }
                            }
                        }
                        if (!everyArgMatch)
                            continue;
                        m_templatePlaceholderSpec = partialSpec;
                        if (Class* class_ = spec->getTemplated()->asClass())
                        {
                            PrintCppSymbols(class_);
                        }
                        else if (Alias* alias = spec->getTemplated()->asAlias())
                        {
                            PrintCppSymbols(spec->getTemplateSignature());
                            hpp.Ln();
                            PrintCppSymbols(alias);
                        }
                        m_templatePlaceholderSpec = nullptr;
                    }
                }
            }
        };

        if (partialSpec->getAccess() == _access)
        {
            if (_access == Access::Undefined)
            {
                SetCurrentNamespace(partialSpec->getTemplate()->getNamespace());
                if (enableModules)
                    hpp("export ");
                PrintLbda();
            }
            else
            {
                PrintLbda();
            }
        }
    }

    std::sort(fullSpecs.begin(), fullSpecs.end(), [](TemplateSpecialization* t0, TemplateSpecialization* t1) {
        if (auto alias0_ = t0->getTemplated()->asAlias())
        {
            if (auto alias1_ = t1->getTemplated()->asAlias())
            {
                return alias0_ < alias1_;
            }
            else
            {
                return true;
            }
        }
        if (auto alias1_ = t1->getTemplated()->asAlias())
        {
            return false;
        }

        Class* c0 = static_cast<Class*>(t0->getTemplated());
        Class* c1 = static_cast<Class*>(t1->getTemplated());

        return c0->getInheritanceLevel(0) < c1->getInheritanceLevel(0);
    });

    // types except enums
    for (auto elem : _input->getTypes())
    {
        if (elem->asEnum() != nullptr)
            continue;
        if (elem->getAccess() == _access)
        {
            if (_access == Access::Undefined)
            {
                SetCurrentNamespace(elem->getNamespace());
                if (enableModules)
                    hpp("export ");
                PrintCppSymbols(elem);
                PrintUsing(elem);
            }
            else
            {
                PrintCppSymbols(elem);
            }
        }
    }

    for (auto fullSpec : fullSpecs)
    {
        if (fullSpec->getAccess() == _access)
        {
            if (_access == Access::Undefined)
            {
                SetCurrentNamespace(fullSpec->getTemplate()->getNamespace());
                if (enableModules)
                    hpp("export ");
                PrintCppSymbols(fullSpec);
                PrintUsing(fullSpec);
            }
            else
            {
                PrintCppSymbols(fullSpec);
            }
        }
    }

    for (auto elem : _input->getFunctions())
    {
        if (elem->getAccess() == _access)
        {
            if (_access == Access::Undefined)
            {
                SetCurrentNamespace(elem->getNamespace());
                if (enableModules)
                    hpp("export ");
                PrintCppSymbols(elem);
                PrintUsing(elem);
            }
            else
            {
                PrintCppSymbols(elem);
            }
        }
    }
    if (_access == Access::Undefined)
    {
        SetCurrentNamespace(Namespace::Global());

        Printer printForwards(forwardContent);
        for (auto& include : includes)
            printForwards("#include <")(include.first)('>').Ln();

        for (auto& pair : forwards)
        {
            if (pair.second)
            {
                SetCurrentNamespace(pair.second, printForwards);
                printForwards(pair.first).Ln();
            }
        }
        SetCurrentNamespace(Namespace::Global(), printForwards);
    }
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(TemplateSpecialization* _templateSpec)
{
    if (_templateSpec->getVisibility() == Visibility::Private ||
        (_templateSpec->testFlags(PHANTOM_R_FLAG_IMPLICIT))) // implicit non native
        return;
    if (auto extended = _templateSpec->getExtendedSpecialization())
        _templateSpec = extended;
    if (_templateSpec->getTemplated())
    {
        if (Class* class_ = _templateSpec->getTemplated()->asClass())
        {
            PrintCppSymbols(class_);
        }
        else if (Alias* alias = _templateSpec->getTemplated()->asAlias())
        {
            PrintCppSymbols(_templateSpec->getTemplateSignature());
            hpp.Ln();
            PrintCppSymbols(alias);
        }
    }
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Template* _template)
{
    if (auto emptySpec = _template->getEmptyTemplateSpecialization())
    {
        if (emptySpec->getTemplated())
        {
            if (Class* class_ = emptySpec->getTemplated()->asClass())
            {
                PrintCppSymbols(class_);
            }
            else if (Alias* alias = emptySpec->getTemplated()->asAlias())
            {
                PrintCppSymbols(emptySpec->getTemplateSignature());
                hpp.Ln();
                PrintCppSymbols(alias);
            }
            return;
        }
    }

    for (TemplateSpecialization* spec : _template->getTemplateSpecializations())
    {
        if (spec->getVisibility() == Visibility::Private ||
            (!spec->isNative() && spec->testFlags(PHANTOM_R_FLAG_IMPLICIT))) // implicit non native
            continue;
        if (auto extended = spec->getExtendedSpecialization())
            spec = extended;

        if (spec->getTemplated())
        {
            if (spec->isFull())
            {
                if (spec->testFlags(PHANTOM_R_FLAG_IMPLICIT)) // use native instance to deduce empty specialization
                {
                    m_templatePlaceholderSpec = spec;
                    if (Class* class_ = spec->getTemplated()->asClass())
                    {
                        PrintCppSymbols(class_);
                    }
                    else if (Alias* alias = spec->getTemplated()->asAlias())
                    {
                        PrintCppSymbols(spec->getTemplateSignature());
                        hpp.Ln();
                        PrintCppSymbols(alias);
                    }
                    m_templatePlaceholderSpec = nullptr;
                    return;
                }
            }
        }
    }
    PrintCppSymbols(_template->getTemplateSignature());
    hpp.Ln();
    hpp("struct ");
    hpp(_template->getName());
    hpp(";");
    hpp.Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(TemplateSignature* _input, Printer& _printer)
{
    _printer("template<");
    int c = 0;
    for (auto param : _input->getTemplateParameters())
    {
        if (c++)
            _printer(", ");
        if (param->getPlaceholder()->asSymbol()->asType())
            _printer("class ");
        else if (Constant* cst = param->getPlaceholder()->asSymbol()->asConstant())
        {
            PrintName(cst->getValueType(), _printer);
            _printer(" ");
        }
        _printer(param->getPlaceholder()->asSymbol()->getName());
        if (auto defaultArg = param->getDefaultArgument())
        {
            _printer(" = ");
            PrintName(defaultArg);
            if (auto defaultArgAsType = defaultArg->asType())
                PrintHppInclude(defaultArgAsType);
        }
    }
    _printer(">");
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(TemplateSignature* _input)
{
    PrintCppSymbols(_input, hpp);
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Subroutine* _input)
{
    if (_input->getFlags() & (int)0x08000000)
        return;
    if (_input->getName().empty())
        return;
    if (_input->getName().front() == '~')
        return PrintNoRet(_input);

    PrintGeneric(_input, hpp);

    bool overr = _input->testModifiers(Modifier::Enum::Override);
    if (_input->isVirtual() && !overr)
        hpp("virtual ");
    PrintForward(_input->getReturnType());
    if (_input->getName().find("operator ") == 0) // conversion function is a special case
    {
        hpp("operator ");
        PrintName(_input->getReturnType());
        hpp("() const;").Ln();
    }
    else
    {
        if (_input->isStatic() || _input->asFunction() && _input->getOwner()->asClass())
            hpp("static ");
        PrintName(_input->getReturnType());
        hpp(" ");
        PrintNoRet(_input);
    }
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Constructor* _input)
{
    PrintNoRet(_input);
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Alias* _input)
{
    if (_input->getFlags() & (int)0x08000000)
        return;
    if (_input->getFlags() & (int)0x04000000)
        return;
    if (_input->getName().empty())
        return;
    if (_input->getAliasedSymbol() == nullptr)
        return;
    if (_input->getAliasedSymbol()->getName().empty())
        return;
    if (Type* atype = _input->getAliasedSymbol()->asType())
        PrintForward(atype);
    PrintHppInclude(_input->getAliasedSymbol());
    hpp("using ");
    if (_input->getAliasedSymbol()->asSubroutine() == nullptr)
        hpp(_input->getName())(" = ");
    m_noAlias++;
    PrintName(_input->getAliasedSymbol());
    m_noAlias--;
    hpp(";").Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Type* _type)
{
    if (Class* class_ = _type->asClass())
    {
        return PrintCppSymbols(class_);
    }
    else if (Enum* enum_ = _type->asEnum())
    {
        return PrintCppSymbols(enum_);
    }
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Enum* _input)
{
    hpp("enum ");
    if (_input->isScoped())
        hpp("class ");
    hpp(_input->getName());

    hpp.Ln();
    hpp("{")++;
    hpp.Ln();
    int c = 0;
    for (auto elem : _input->getConstants())
    {
        if (c++)
            hpp(", ");
        hpp(elem->getName()).Ln();
    }
    hpp--("};").Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Class* _input)
{
    auto prevScope = currentScope;
    currentScope = _input;
    auto& bases = _input->getBaseClasses();
    for (auto& bc : bases)
    {
        PrintHppInclude(bc.baseClass);
    }

    TemplateSpecialization* tspec = _input->getTemplateSpecialization();
    if (tspec)
    {
        if (m_templatePlaceholderSpec == tspec)
            PrintTemplateSignature(m_templatePlaceholderSpec->getTemplate()->getTemplateSignature(), hpp);
        else
            PrintTemplateSignature(tspec->getTemplateSignature(), hpp);
    }
    hpp("class ")(_input->getName());
    if (tspec && !tspec->isEmpty() && m_templatePlaceholderSpec != tspec)
    {
        PrintTemplateDecoration(_input->getTemplateSpecialization());
    }
    if (bases.size())
    {
        hpp(" : ");
        int c = 0;
        for (auto& bc : bases)
        {
            if (c++)
                hpp(", ");
            if (bc.access == Access::Public)
                hpp("public ");
            else if (bc.access == Access::Protected)
                hpp("protected ");
            else if (bc.access == Access::Private)
                hpp("private ");
            PrintForward(bc.baseClass);
            PrintName(bc.baseClass);
        }
    }
    hpp.Ln();
    hpp("{")++;
    hpp.Ln();
    hpp--("public:").Ln();
    hpp++;
    PrintMembers(_input, Access::Public);
    hpp--("protected:").Ln();
    hpp++;
    PrintMembers(_input, Access::Protected);
    hpp--("private:").Ln();
    hpp++;
    PrintMembers(_input, Access::Private);
    hpp--("};").Ln();
    currentScope = prevScope;
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Constant* _cst)
{
    PrintForward(_cst->getValueType());
    if (_cst->getOwner()->asClassType())
        hpp("static ");
    PrintVarName(_cst->getValueType(), "const " + _cst->getName(), hpp);
    if (_cst->getValueType()->removeQualifiers()->asClassType())
    {
        PrintHppInclude(_cst->getValueType());
    }
    hpp(";").Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Variable* _variable)
{
    PrintForward(_variable->getValueType());
    if (_variable->getOwner()->asClassType())
        hpp("static ");
    PrintVarName(_variable->getValueType(), _variable->getName(), hpp);
    if (_variable->getValueType()->removeQualifiers()->asClassType())
    {
        PrintHppInclude(_variable->getValueType());
    }
    hpp(";").Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Field* _field)
{
    PrintForward(_field->getValueType());
    PrintVarName(_field->getValueType(), _field->getName(), hpp);
    if (_field->getValueType()->removeQualifiers()->asClassType())
    {
        PrintHppInclude(_field->getValueType());
    }
    hpp(";").Ln();
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintForward(Type* _type)
{
    if (auto alias = getAlias(_type))
        return PrintHppInclude(alias);
    _type = _type->removeEverything();
    String  forward;
    Printer printForward(forward);
    if (TemplateSpecialization* spec = _type->getTemplateSpecialization())
    {
        for (auto arg : spec->getArguments())
        {
            if (arg->asType() && arg->asPlaceholder() == nullptr)
                PrintForward(arg->asType());
        }
        PrintCppSymbols(_type->getTemplate()->getTemplateSignature(), printForward);
        printForward.Ln();
        printForward("class ");
        printForward(_type->getName());
        printForward(";");
        forwards[forward] = _type->getTemplate()->getNamespace();
    }
    else if (_type->asClassType())
    {
        printForward("class ");
        printForward(_type->getName());
        printForward(";");
        forwards[forward] = _type->getNamespace();
    }
    else if (_type->asEnum())
    {
        printForward("enum ")(_type->getName())(";");
        forwards[forward] = _type->getNamespace();
    }
    else if (auto ft = _type->asFunctionType())
    {
        PrintForward(ft->getReturnType());
        for (auto t : ft->getParameterTypes())
        {
            PrintForward(t);
        }
    }
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintNoRet(Subroutine* _input)
{
    hpp(_input->getName())("(");
    PrintCppParameters(_input, hpp, 0, false);
    hpp(")");
    PrintCppSuffix(_input, hpp);
    hpp(";").Ln();
}

void CppLiteCompletionHeaderGenerator::PrintCppSymbols(Source* _input, StringView _dir, bool _enableModules)
{
    // skip 'default' sources which are internals and not exposed
    if (_input->getName() == "default")
        return;
    // ex : "Wolf.Core.Object.hpp"
    String sourceName = StringView(_input->getUniqueName());

    // prepare printers to collect text
    // .hpp file (c++ skeleton)
    String  hppfile_content;
    Printer hppfile_printer(hppfile_content);

    // noext file (using declarations to import in-place)
    String  usingfile_content;
    Printer usingfile_printer(usingfile_content);

    // print all symbols
    SourcePrinter sprinter(_input, hppfile_printer, usingfile_printer, _enableModules);

    for (auto& src_dst : aliases)
    {
        auto symSrc = phantom::lang::Application::Get()->findCppSymbol(src_dst.first);
        auto symDst = phantom::lang::Application::Get()->findCppSymbol(src_dst.second);
        if (symSrc && symDst)
        {
            sprinter.aliases[symSrc] = symDst;
        }
    }

    if (_enableModules)
        hppfile_printer("export module ")(_input->getUniqueName())(";").Ln().Ln();

    sprinter.PrintCppSymbols(static_cast<Scope*>(_input), Access::Undefined);

    // <package>/<source>.hpp
    String hpp_rel_path = sourceName;

    std::for_each(hpp_rel_path.begin(), hpp_rel_path.end(), [](char& c) {
        if (c == '.')
            c = '/';
    });
    hpp_rel_path += ".hpp";
    {
        String hpp_path = _dir + '/' + hpp_rel_path;
        Path::CreateDirectories(Path(hpp_path).parentPath());
        std::ofstream hpp_stream(hpp_path.c_str(), std::ofstream::trunc);
        hpp_stream << "#pragma once";
        hpp_stream << std::endl;
        hpp_stream << std::endl;
        hpp_stream << sprinter.forwardContent;
        hpp_stream << hppfile_content;
    }

    // ex : "<package>.<source>" (used for import in place)
    String usingfile_path = _dir + '/' + sourceName;
    {
        std::ofstream usingfile_stream(usingfile_path.c_str(), std::ofstream::trunc);
        usingfile_stream << "#include \"" << hpp_rel_path.c_str() << "\"";
        usingfile_stream << std::endl;
        usingfile_stream << usingfile_content;
    }
    if (!_input->isNative())
    {
        // ex : "<package>/<source>" (used for local import in same module)
        String usinglocalfile_relpath = sourceName;

        std::for_each(usinglocalfile_relpath.begin(), usinglocalfile_relpath.end(), [](char& c) {
            if (c == '.')
                c = '/';
        });
        {
            std::ofstream usinglocalfile_stream((_dir + '/' + usinglocalfile_relpath).c_str(), std::ofstream::trunc);
            usinglocalfile_stream << "#include \"" << _input->getName() << ".hpp\"";
            usinglocalfile_stream << std::endl;
            usinglocalfile_stream << usingfile_content;
        }
    }
}

void CppLiteCompletionHeaderGenerator::PrintCppSymbols(Module* _input, StringView _dir, bool _enableModules)
{
    Path::CreateDirectories(_dir);
    for (auto p : _input->getPackages())
    {
        Path::CreateDirectories(_dir + '/' + _input->getName());
        for (auto s : p->getSources())
        {
            if (s->getName().empty())
                continue;

            if (s->getVisibility() == Visibility::Private)
                continue;
            PrintCppSymbols(s, _dir + '/' + _input->getName(), _enableModules);
        }
    }
}

void CppLiteCompletionHeaderGenerator::GenerateCppLiteNoExt(StringView _outDir, StringView _relDir,
                                                            StringView _inputDir)
{
    DirectoryEntries cpplites;
    Path::ListDirectoryEntries(_inputDir, cpplites);

    for (DirectoryEntry& cpplite : cpplites)
    {
        if (!cpplite.isRegularFile() || cpplite.path().extension() != ".cpplite")
            continue;
        String outDir = _outDir + '/' + _relDir;

        String fullCppLiteDir = _inputDir + '/' + cpplite.path().filename();

        // local file
        {
            Path::CreateDirectories(outDir);
            std::ofstream stream((outDir + '/' + cpplite.path().stem()).c_str(), std::ofstream::trunc);
            String        relPath =
            phantom::Path(fullCppLiteDir.c_str()).relative(String(outDir).c_str()).genericString().c_str();
            stream << "#pragma once";
            stream << std::endl;
            stream << "#include \"" << relPath << "\"";
        }

        // global file
        {
            String dotName = _relDir;

            std::for_each(dotName.begin(), dotName.end(), [](char& c) {
                if (c == '/')
                    c = '.';
            });
            if (!dotName.empty())
                dotName += '.';
            dotName += cpplite.path().stem();
            std::ofstream stream((outDir + '/' + dotName).c_str(), std::ofstream::trunc);
            String        relPath =
            phantom::Path(fullCppLiteDir.c_str()).relative(String(_outDir).c_str()).genericString().c_str();
            stream << "#pragma once";
            stream << std::endl;
            stream << "#include \"" << relPath << "\"";
        }
    }

    for (DirectoryEntry& dir : cpplites)
    {
        if (dir.isDirectory())
        {
            if (dir.path() != "." && dir.path() != "..")
            {
                if (_relDir.empty())
                    GenerateCppLiteNoExt(_outDir, dir.path().filename(), _inputDir + '/' + dir.path().filename());
                else
                    GenerateCppLiteNoExt(_outDir, _relDir + '/' + dir.path().filename(),
                                         _inputDir + '/' + dir.path().filename());
            }
        }
    }
}

void CppLiteCompletionHeaderGenerator::PrintModules(ArrayView<StringView> _workspaces, bool _enableModules)
{
    // CPPLITE SOURCES
    for (auto workspace : _workspaces)
    {
        String dir = String(workspace) + "/.modules";
        Path::CreateDirectories(dir);

        std::ofstream allH((dir + StringView("/all.hpp")).c_str());
        for (Module* module : Application::Get()->getModules())
        {
            PrintCppSymbols(module, dir, _enableModules);
            for (Package* pkg : module->getPackages())
            {
                for (Source* src : pkg->getSources())
                {
                    if (src->getName() != "default" && src->getName().size())
                        allH << "#include <" << SourcePrinter::GetHppIncludePath(src) << '>' << std::endl;
                }
            }
        }
        allH << "using size_t = unsigned long long;" << std::endl;
        allH << "#define assert(...)" << std::endl;
        allH << "#define typeof(...) static_cast<phantom::lang::Type*>(nullptr)" << std::endl;
        allH << "#define classof(...) static_cast<phantom::lang::Class*>(nullptr)" << std::endl;
    }
}

int CppLiteCompletionHeaderGenerator::main(int argc, char** argv)
{
    // ensure every known plugin is loaded
    for (auto plugin : Application::Get()->getPlugins())
    {
        plugin->load();
    }
    bool enableModules = false;

    SmallVector<StringView> workspaces;
    SmallVector<String>     includepaths;

    for (int i = 1; i < argc; ++i)
    {
        if (StringView(argv[i]) == "--modules")
        {
            enableModules = true;
        }
        else if (StringView(argv[i]).find("--workspace=") == 0)
        {
            workspaces.push_back(StringView(argv[i]).substr(12));
        }
        else if (StringView(argv[i]).find("--include=") == 0)
        {
            includepaths.push_back(StringView(argv[i]).substr(10));
        }
    }
    if (workspaces.empty())
        workspaces.push_back(".");

    for (auto& ws : workspaces)
    {
        includepaths.push_back(ws);
    }

    PrintModules(workspaces);

    //
    //         for (auto ip : includepaths)
    //         {
    //             DirectoryEntries scriptsProjectsDirs;
    //             Path::ListDirectoryEntries(ip, scriptsProjectsDirs);
    //             for (auto project : scriptsProjectsDirs)
    //             {
    //                 if (project.isDirectory())
    //                     if (project.path().front().front() != '.')
    //                     {
    //                         Path::CreateDirectories(workspace + "/.modules/" + project.path().filename());
    //                         GenerateCppLiteNoExt(workspace + "/.modules/" + project.path().filename(), "",
    //                                              ip + '/' + project.path().filename());
    //                     }
    //             }
    //         }

    return 0;
}

} // namespace lang
} // namespace phantom
