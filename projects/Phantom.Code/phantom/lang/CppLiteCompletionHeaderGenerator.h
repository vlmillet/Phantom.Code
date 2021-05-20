#pragma once

#include "fwd.h"
#include "plugin.h"

#include <ostream>
#include <phantom/lang/Namespace.h>
#include <phantom/utils/SmallMap.h>

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM_CODE CppLiteCompletionHeaderGenerator
{
public:
    struct PHANTOM_EXPORT_PHANTOM_CODE Printer
    {
        Printer();
        Printer(FILE* _cfile);
        Printer(String& _str);

        Printer& operator()(StringView);
        Printer& operator()(const char* _str);
        Printer& operator()(bool);
        Printer& operator()(char);
        Printer& operator()(int8);
        Printer& operator()(uint8);
        Printer& operator()(int16);
        Printer& operator()(uint16);
        Printer& operator()(int32);
        Printer& operator()(uint32);
        Printer& operator()(int64);
        Printer& operator()(uint64);
        Printer& operator()(float);
        Printer& operator()(double);
        Printer& operator()(void*);
        Printer& Ln();

        Printer& operator++(int)
        {
            indentLvl++;
            return *this;
        }
        Printer& operator--(int)
        {
            PHANTOM_ASSERT(indentLvl > 0);
            indentLvl--;
            return *this;
        }
        Printer& operator++()
        {
            indentLvl++;
            return *this;
        }
        Printer& operator--()
        {
            PHANTOM_ASSERT(indentLvl > 0);
            indentLvl--;
            return *this;
        }

    private:
        void    _vPrintf(const char* _format, va_list _va);
        void    _Printf(const char* _format, ...);
        int     indentLvl = 0;
        int     indentConsumed = false;
        String* str = nullptr;
        FILE*   file = nullptr;
    };

    struct PHANTOM_EXPORT_PHANTOM_CODE SourcePrinter
    {
        static String GetHppIncludePath(Source* _source);

        Source*                source;
        Printer&               hpp;
        Printer&               usingfile;
        bool                   enableModules;
        SmallMap<String, bool> includes;

        SourcePrinter(Source* _source, Printer& _hpp, Printer& _ufile, bool _enableModules);

        String ProtectQuote(StringView _str);

        void PrintUsing(Alias* _input);

        void PrintUsing(Type* _input);

        void PrintUsing(Symbol* _input);

        void PrintUsing(Function* _input);
        void PrintTemplateSignature(TemplateSignature* _input, Printer& _printer);
        void PrintUsingTemplateSignature(TemplateSignature* _input);

        void PrintUsing(Template* _input);

        void PrintTemplateDecoration(TemplateSpecialization* _spec);

        void PrintName(LanguageElement* _elem);

        void PrintName(LanguageElement* _elem, Printer& _printer);

        void PrintVarName(Type* _type, StringView _name, Printer& _printer);

        void PrintCppSymbols(Field* _field);

        void PrintCppSymbols(Variable* _variable);

        void PrintCppSymbols(Constant* _cst);

        void PrintCppSymbols(TemplateSpecialization* _templateSpec);

        void PrintName(Symbol* _symbol);

        void PrintName(Symbol* _symbol, Printer& _printer);

        void PrintMembers(ClassType* _input, Access _access);

        void SetCurrentNamespace(Namespace* _ns);

        void SetCurrentNamespace(Namespace* _ns, Printer& _print);

        void PrintCppSymbols(Class* _input);

        void PrintCppSymbols(Enum* _input);

        void PrintCppSymbols(Type* _type);

        void PrintHppInclude(Symbol* _input);

        void PrintHppInclude(Source* _source);

        void PrintCppSymbols(Alias* _input);

        void PrintForward(Type* _type);

        void PrintCppSymbols(Constructor* _input);

        void PrintNoRet(Subroutine* _input);

        void PrintCppParameter(Parameter* _input, Printer& _printer);
        void PrintCppParameters(Subroutine* _input, Printer& _printer, size_t _firstIdx = 0, bool _append = false);
        void PrintCppSuffix(Subroutine* _input, Printer& _printer);

        bool PrintGeneric(Subroutine* _input, Printer& _printer);

        void PrintCppSymbols(Subroutine* _input);

        void PrintCppSymbols(TemplateSignature* _input);

        void PrintCppSymbols(TemplateSignature* _input, Printer& _printer);

        void PrintCppSymbols(Template* _template);

        void                         PrintCppSymbols(Scope* _input, Access _access);
        Namespace*                   namespace_ = Namespace::Global();
        LanguageElement*             currentScope = Namespace::Global();
        SmallMap<String, Namespace*> forwards;
        String                       forwardContent;
        SmallMap<Symbol*, Symbol*>   aliases;

    private:
        Symbol* getAlias(Symbol* _symbol)
        {
            auto found = aliases.find(_symbol);
            if (found != aliases.end())
                return found->second;
            return nullptr;
        }

        int m_noAlias = 0;

    private:
        TemplateSpecialization* m_templatePlaceholderSpec{};
    };

    void PrintCppSymbols(Source* _input, StringView _dir, bool _enableModules);

    void PrintCppSymbols(Module* _input, StringView _dir, bool _enableModules);

    void GenerateCppLiteNoExt(StringView _outDir, StringView _relDir, StringView _inputDir);

    void PrintModules(ArrayView<StringView> _workspaces, bool _enableModules = false);

    int main(int argc, char** argv);

    SmallMap<String, String> aliases;
};

} // namespace lang
} // namespace phantom
