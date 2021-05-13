#pragma once

// haunt {

// clang-format off

#include "CppLiteCompletionHeaderGenerator.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("CppLiteCompletionHeaderGenerator")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(CppLiteCompletionHeaderGenerator)
        {
            using Printer = typedef_<_::Printer>;
            using SourcePrinter = typedef_<_::SourcePrinter>;
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
        .public_()
            .struct_<Printer>()
                .constructor<void()>()
                /// missing symbol(s) reflection (FILE) -> use the 'haunt.bind' to bind symbols with your custom haunt files
                // .constructor<void(FILE*)>()({"_cfile"})
                .constructor<void(String&)>()({"_str"})
                .method<Printer&(StringView)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(const char*)>("operator()", &_::Printer::operator())({"_str"})
                .method<Printer&(bool)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(char)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(int8)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(uint8)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(int16)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(uint16)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(int32)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(uint32)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(int64)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(uint64)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(float)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(double)>("operator()", &_::Printer::operator())({""})
                .method<Printer&(void*)>("operator()", &_::Printer::operator())({""})
                .method<Printer&()>("Ln", &_::Printer::Ln)
                .method<Printer&(int)>("operator++", &_::Printer::operator++)({""})
                .method<Printer&(int)>("operator--", &_::Printer::operator--)({""})
                .method<Printer&()>("operator++", &_::Printer::operator++)
                .method<Printer&()>("operator--", &_::Printer::operator--)
            .end()
            .struct_<SourcePrinter>()
                .staticMethod<String(Source*)>("GetHppIncludePath", &_::SourcePrinter::GetHppIncludePath)({"_source"})
                .field("source", &_::SourcePrinter::source)
                .field("enableModules", &_::SourcePrinter::enableModules)
                .field("includes", &_::SourcePrinter::includes)
                .constructor<void(Source*, Printer&, Printer&, bool)>()({"_source","_hpp","_ufile","_enableModules"})
                .method<String(StringView)>("ProtectQuote", &_::SourcePrinter::ProtectQuote)({"_str"})
                .method<void(Alias*)>("PrintUsing", &_::SourcePrinter::PrintUsing)({"_input"})
                .method<void(Type*)>("PrintUsing", &_::SourcePrinter::PrintUsing)({"_input"})
                .method<void(Symbol*)>("PrintUsing", &_::SourcePrinter::PrintUsing)({"_input"})
                .method<void(Function*)>("PrintUsing", &_::SourcePrinter::PrintUsing)({"_input"})
                .method<void(TemplateSignature*, Printer&)>("PrintTemplateSignature", &_::SourcePrinter::PrintTemplateSignature)({"_input","_printer"})
                .method<void(TemplateSignature*)>("PrintUsingTemplateSignature", &_::SourcePrinter::PrintUsingTemplateSignature)({"_input"})
                .method<void(Template*)>("PrintUsing", &_::SourcePrinter::PrintUsing)({"_input"})
                .method<void(TemplateSpecialization*)>("PrintTemplateDecoration", &_::SourcePrinter::PrintTemplateDecoration)({"_spec"})
                .method<void(LanguageElement*)>("PrintName", &_::SourcePrinter::PrintName)({"_elem"})
                .method<void(LanguageElement*, Printer&)>("PrintName", &_::SourcePrinter::PrintName)({"_elem","_printer"})
                .method<void(Type*, StringView, Printer&)>("PrintVarName", &_::SourcePrinter::PrintVarName)({"_type","_name","_printer"})
                .method<void(Field*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_field"})
                .method<void(Variable*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_variable"})
                .method<void(Constant*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_cst"})
                .method<void(TemplateSpecialization*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_templateSpec"})
                .method<void(Symbol*)>("PrintName", &_::SourcePrinter::PrintName)({"_symbol"})
                .method<void(Symbol*, Printer&)>("PrintName", &_::SourcePrinter::PrintName)({"_symbol","_printer"})
                .method<void(ClassType*, Access)>("PrintMembers", &_::SourcePrinter::PrintMembers)({"_input","_access"})
                .method<void(Namespace*)>("SetCurrentNamespace", &_::SourcePrinter::SetCurrentNamespace)({"_ns"})
                .method<void(Namespace*, Printer&)>("SetCurrentNamespace", &_::SourcePrinter::SetCurrentNamespace)({"_ns","_print"})
                .method<void(Class*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_input"})
                .method<void(Enum*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_input"})
                .method<void(Type*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_type"})
                .method<void(Symbol*)>("PrintHppInclude", &_::SourcePrinter::PrintHppInclude)({"_input"})
                .method<void(Source*)>("PrintHppInclude", &_::SourcePrinter::PrintHppInclude)({"_source"})
                .method<void(Alias*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_input"})
                .method<void(Type*)>("PrintForward", &_::SourcePrinter::PrintForward)({"_type"})
                .method<void(Constructor*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_input"})
                .method<void(Subroutine*)>("PrintNoRet", &_::SourcePrinter::PrintNoRet)({"_input"})
                .method<void(Subroutine*, Printer&, size_t, bool)>("PrintCppParameters", &_::SourcePrinter::PrintCppParameters)({"_input","_printer","_firstIdx","_append"})["0"]["false"]
                .method<void(Subroutine*, Printer&)>("PrintCppSuffix", &_::SourcePrinter::PrintCppSuffix)({"_input","_printer"})
                .method<bool(Subroutine*, Printer&)>("PrintGeneric", &_::SourcePrinter::PrintGeneric)({"_input","_printer"})
                .method<void(Subroutine*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_input"})
                .method<void(TemplateSignature*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_input"})
                .method<void(TemplateSignature*, Printer&)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_input","_printer"})
                .method<void(Template*)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_template"})
                .method<void(Scope*, Access)>("PrintCppSymbols", &_::SourcePrinter::PrintCppSymbols)({"_input","_access"})
                .field("namespace_", &_::SourcePrinter::namespace_)
                .field("currentScope", &_::SourcePrinter::currentScope)
                .field("forwards", &_::SourcePrinter::forwards)
                .field("forwardContent", &_::SourcePrinter::forwardContent)
                .field("aliases", &_::SourcePrinter::aliases)
            .end()
            .method<void(Source*, StringView, bool)>("PrintCppSymbols", &_::PrintCppSymbols)({"_input","_dir","_enableModules"})
            .method<void(Module*, StringView, bool)>("PrintCppSymbols", &_::PrintCppSymbols)({"_input","_dir","_enableModules"})
            .method<void(StringView, StringView, StringView)>("GenerateCppLiteNoExt", &_::GenerateCppLiteNoExt)({"_outDir","_relDir","_inputDir"})
            .method<void(ArrayView<StringView>, bool)>("PrintModules", &_::PrintModules)({"_workspaces","_enableModules"})["false"]
            .method<int(int, char**)>("main", &_::main)({"argc","argv"})
            .field("aliases", &_::aliases)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CppLiteCompletionHeaderGenerator")
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
