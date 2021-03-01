// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "fwd.h"

#include <phantom/lang/LanguageElementVisitor.h>
#include <phantom/utils/Object.h>
#include <phantom/utils/SmallMap.h>
#include <phantom/utils/String.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Base class for source compilers (designed for multipass compilation).
class PHANTOM_EXPORT_PHANTOM_CODE CodeGenerator : public Object
{
    friend class Compiler;

public:
    enum class Error
    {
        OK,
        UnresolvedSymbol,
        MemoryNotReady,
        OutOfMemory,
    };

    CodeGenerator(int a_uiPassCount);
    ~CodeGenerator(void);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the options of the code generation.
    ///
    /// \return The options.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Options& getOptions() const { return m_Options; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the options of the code generation.
    ///
    /// \return The options.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setOptions(const Options& a_Options) { m_Options = a_Options; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets an option controlling the compilation.
    ///
    /// \param  a_strKey    The option key.
    /// \param  a_strValue  The option value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setOption(StringView a_strKey, StringView a_strValue) { m_Options[a_strKey] = a_strValue; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the pass count.
    ///
    /// \return The pass count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    uint getPassCount() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an option matching the given key.
    ///
    /// \param  a_Key   The key.
    ///
    /// \return The option.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getOption(StringView a_Key) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get one option and its value if it exists.
    ///
    /// \return The true if '_option' exists.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool getOption(StringView _option, StringView& _value) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the source being compiled with multipass.
    ///
    /// \return null if no multipass is currently running, else the source being compiled.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module* getModule() const { return m_pModule; }

    void outdate();

    bool isOutdated() const { return m_bOutdated; }

    bool invoke(Method* a_pMethod, void* a_pObject, void** a_ppArgs, void* a_pRet) const;
    bool call(Subroutine* a_pSubroutine, void** a_ppArgs, void* a_pRet) const;

    virtual ExecutionContext* createExecutionContext() const = 0;
    virtual void              destroyExecutionContext(ExecutionContext* a_pContext) const = 0;

protected:
    virtual DebugInformation* createDebugInformation();
    virtual void              destroyDebugInformation(DebugInformation* a_pDebugInformationHandler);

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Begin the compilation of a source with multipass system. Reimplement to initialize
    /// your compiler.
    ///
    /// \param  pass    The source to parse.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void begin() {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Abort the compilation (end() will not be called)
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void abort() {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  End the compilation of a source with multipass system. Reimplement to cleanup your
    /// compiler
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void end() {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Runs a single compilation pass. Must be reimplemented to perform your own
    /// compilation.
    ///
    /// \param  pass The compilation pass to run.
    ///
    /// \return .
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Error compile(uint a_uiPass) = 0;

    virtual void onOutdated() {}

protected:
    Module* m_pModule;
    Options m_Options;
    uint    m_uiPassCount;
    bool    m_bOutdated = true;
};

} // namespace lang
} // namespace phantom
