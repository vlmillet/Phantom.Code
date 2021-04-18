#include "CppLiteCompletionHeaderGenerator.h"

#include "phantom/lang/Alias.h"
#include "phantom/lang/Application.h"
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
        if (&_printer == &hpp)
            PrintForward(param->getValueType());
        String name = StringView(param->getName());
        if (name.empty())
            name = String("_") + std::to_string(i - _firstIdx).c_str();
        PrintVarName(param->getValueType(), name, _printer);
        if (param->getNativeDefaultArgumentString().size())
        {
            if (_input->getEnclosingTemplateSpecialization())
            {
                // FIXME : do real expression translation here when implemented, for now it's just a placeholder
                // for parsing correctly
                auto pNoRefType = param->getValueType()->removeReference()->removeQualifiers();
                if (pNoRefType->asPointer() || pNoRefType->asArray())
                    _printer(" = nullptr");
                else
                    _printer(" = ")(pNoRefType->getQualifiedDecoratedName())("{}");
            }
            else
            {
                _printer(" = ")(param->getNativeDefaultArgumentString());
            }
        }
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
    bool                gen = false;
    size_t              genParamCount = 0;
    SmallVector<size_t> args;
    size_t              retTypeIdx = -1;
    Type*               retType = _input->getReturnType();
    // handle special Generic cases
    if (retType->getName() == "Return" && retType->getQualifiedName() == "phantom::Generic::Return")
    {
        auto spec = retType->getTemplateSpecialization();
        auto paramParam = spec->getArgument(0)->asType()->removeEverything();
        auto paramParamSpec = paramParam->getTemplateSpecialization();
        auto paramParamSpecCst = paramParamSpec->getArgument(0)->asConstant();
        paramParamSpecCst->getValue(&retTypeIdx);
    }
    for (; genParamCount < _input->getParameters().size(); ++genParamCount)
    {
        if (_input->getParameterType(genParamCount)->getName() != "Param" ||
            _input->getParameterType(genParamCount)->getQualifiedName() != "phantom::Generic::Param")
        {
            break;
        }
    }
    for (; genParamCount < _input->getParameters().size(); ++genParamCount)
    {
        if (_input->getParameterType(genParamCount)->getName() != "Arg" ||
            _input->getParameterType(genParamCount)->getQualifiedName() != "phantom::Generic::Arg")
        {
            break;
        }
        size_t arg = -1;
        auto   spec = _input->getParameterType(genParamCount)->getTemplateSpecialization();
        auto   paramParam = spec->getArgument(0)->asType()->removeEverything();
        auto   paramParamSpec = paramParam->getTemplateSpecialization();
        auto   paramParamSpecCst = paramParamSpec->getArgument(0)->asConstant();
        paramParamSpecCst->getValue(&arg);
        args.push_back(arg);
    }

    if (genParamCount != 0)
    {
        _printer("template<");
        for (size_t i = 0; i < genParamCount - args.size(); ++i)
        {
            if (i)
                _printer(", ");
            _printer("class T")(i);
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
        int c = 0;
        for (auto arg : args)
        {
            if (c++)
                _printer(", ");
            _printer("T")(arg)("*");
        }
        PrintCppParameters(_input, _printer, genParamCount, c != 0);
        _printer(")");
        PrintCppSuffix(_input, _printer);
        _printer(";").Ln();
        return true;
    }
    return false;
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintHppInclude(Source* _source)
{
    if (source == _source || _source->getName() == "default" || _source->getName().empty())
        return;
    String hpp_rel_path = _source->getUniqueName();

    std::for_each(hpp_rel_path.begin(), hpp_rel_path.end(), [](char& c) {
        if (c == '.')
            c = '/';
    });
    includes[hpp_rel_path + ".hpp"];
}

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintHppInclude(Symbol* _input)
{
    Source* includeSource;
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
            else
            {
                includeSource = spec->getTemplate()->getSource();
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
        if (elem->getName() == "vector")
        {
            int i = 0;
        }
        if (elem->testFlags(0x2000)) // implicit
            continue;
        if (elem->getAccess() == _access)
        {
            PrintCppSymbols(elem);
        }
    }
    for (auto elem : _input->getMethods())
    {
        if (elem->testFlags(0x2000)) // implicit
            continue;
        if (elem->getAccess() == _access)
        {
            PrintCppSymbols(elem);
        }
    }
    for (auto elem : _input->getFields())
    {
        if (elem->testFlags(0x2000)) // implicit
            continue;
        if (elem->getAccess() == _access)
        {
            PrintCppSymbols(elem);
        }
    }
    for (auto elem : _input->getFriends())
    {
        if (elem->testFlags(0x2000)) // implicit
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
    for (auto elem : _input->getTypes())
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

void CppLiteCompletionHeaderGenerator::SourcePrinter::PrintCppSymbols(Template* _template)
{
    if (_template->getName() == "vector_set")
    {
        int i = 0;
    }
    for (TemplateSpecialization* spec : _template->getTemplateSpecializations())
    {
        if (spec->getVisibility() == Visibility::Private ||
            (!spec->isNative() && spec->testFlags(0x2000))) // implicit non native
            continue;
        if (auto extended = spec->getExtendedSpecialization())
            spec = extended;

        if (spec->getTemplated())
        {
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
        }
        else if (spec->isEmpty())
        {
            PrintCppSymbols(spec->getTemplateSignature());
            hpp.Ln();
            hpp("struct ")(spec->getName())(";").Ln();
        }
    }
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

    if (PrintGeneric(_input, hpp))
        return;

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
    PrintName(_input->getAliasedSymbol());
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
        if (bc.baseClass->getName() == "numeric_limits")
        {
            int i = 0;
        }
        PrintHppInclude(bc.baseClass);
    }

    TemplateSpecialization* tspec = _input->getTemplateSpecialization();
    if (tspec)
    {
        PrintTemplateSignature(_input->getTemplateSpecialization()->getTemplateSignature(), hpp);
    }
    hpp("class ")(_input->getName());
    if (tspec && !tspec->isEmpty())
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
    std::ofstream cppliteH((_dir + StringView("/cpplite.h")).c_str());
    cppliteH << "#define assert(...)" << std::endl;
    cppliteH << "#define typeof(...) static_cast<phantom::lang::Type*>(nullptr)" << std::endl;
    cppliteH << "#define classof(...) static_cast<phantom::lang::Class*>(nullptr)" << std::endl;
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

int CppLiteCompletionHeaderGenerator::main(int argc, char** argv)
{
    // ensure every known plugin is loaded
    for (auto plugin : Application::Get()->getPlugins())
    {
        plugin->load();
    }
    bool enableModules = false;

    SmallVector<String> workspaces;
    SmallVector<String> includepaths;

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

    // NATIVE MODULES

    // CPPLITE SOURCES
    for (auto workspace : workspaces)
    {
        for (Module* module : Application::Get()->getModules())
        {
            if (module->isNative())
            {
                Path::CreateDirectories(workspace + "/.modules");
                PrintCppSymbols(module, workspace + "/.modules", enableModules);
            }
        }
        for (auto ip : includepaths)
        {
            DirectoryEntries scriptsProjectsDirs;
            Path::ListDirectoryEntries(ip, scriptsProjectsDirs);
            for (auto project : scriptsProjectsDirs)
            {
                if (project.isDirectory())
                    if (project.path().front().front() != '.')
                    {
                        Path::CreateDirectories(workspace + "/.modules/" + project.path().filename());
                        GenerateCppLiteNoExt(workspace + "/.modules/" + project.path().filename(), "",
                                             ip + '/' + project.path().filename());
                    }
            }
        }
    }
    return 0;
}

} // namespace lang
} // namespace phantom
