// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "lang.h"

#include "BuildSystem.h"
#include "CppLite.h"
#include "Interpreter.h"
#include "Semantic.h"
#include "conversions.h"
#include "phantom/thread/SpinMutex.h"

#include <map>
#include <phantom/plugin>
#include <phantom/utils/Placement.h>

namespace phantom
{
namespace lang
{
StaticGlobal<CppLite>     g_PHNTM_LNG_CppLite(false);
StaticGlobal<BuildSystem> g_PHNTM_LNG_Compiler(false);

struct LangInit
{
    LangInit()
    {
        g_PHNTM_LNG_CppLite.construct();
        g_PHNTM_LNG_Compiler.construct();
        Variant::SetTypeConverter(&Lang::CppTypeConverter);
        BuildSystem::Get()->addLanguage(g_PHNTM_LNG_CppLite);
    }

    ~LangInit()
    {
        BuildSystem::Get()->removeLanguage(g_PHNTM_LNG_CppLite);
        Variant::SetTypeConverter(nullptr);
    }
};
StaticGlobal<LangInit> g_PHNTM_LNG_Init(false);

extern PHANTOM_EXPORT_PHANTOM ExecutionContext* (*GetDefaultExecutionContext)();

SpinMutex ts_InterpreterContextMutex;

StaticGlobal<std::map<std::thread::id, InterpreterContext>> tls_InterpreterContext(false);

ExecutionContext* GetTLSInterpreterContext()
{
    auto lock = ts_InterpreterContextMutex.autoLock();
    return &(*tls_InterpreterContext)[std::this_thread::get_id()];
}

void StaticGlobalsInit()
{
    g_PHNTM_LNG_Init.construct();
    tls_InterpreterContext.construct();
    GetDefaultExecutionContext = GetTLSInterpreterContext;
}

void StaticGlobalsRelease() {}

bool Lang::CppTypeConverter(Type* a_pDestType, void* a_pDest, Type* a_pSrcType, void const* a_pSrc)
{
    lang::Conversion* conv = CppLite::Get()->getDefaultSemantic()->newConversion(
    a_pSrcType, a_pDestType, (CastKind)CastKind::Implicit, (UserDefinedFunctions)UserDefinedFunctions::ImplicitsOnly,
    nullptr);
    if (conv == nullptr)
        return false;
    conv->apply(a_pSrc, a_pDest);
    PHANTOM_ASSERT(conv->semantic);
    return true;
}

} // namespace lang
} // namespace phantom

PHANTOM_PLUGIN("Phantom.Code", PHANTOM_PLUGIN_REGISTER_CLASS_MEMBERS_ON_ACCESS, phantom::lang::StaticGlobalsInit,
               phantom::lang::StaticGlobalsRelease);
