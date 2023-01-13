// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "fwd.h"

#include <phantom/utils/SmallMap.h>
#include <phantom/utils/SmallSet.h>

namespace phantom
{
namespace lang
{
class Message;
class BuildSystem;

class PHANTOM_EXPORT_PHANTOM_CODE BuildSource
{
    friend class BuildSystem;

public:
    static BuildSource* Get(Source* a_pSource);

    struct PHANTOM_EXPORT_PHANTOM_CODE Session
    {
        friend class BuildSource;

        Session();

        BuildSource* getBuildSource() const { return m_pBuildSource; }
        Source*      getSource() const { return m_pSource; }
        Package*     getPackage() const { return m_pPackage; }
        Parser*      getParser() const { return m_pParser; }
        Semantic*    getSemantic() const { return m_pSemantic; }
        Language*    getLanguage() const { return m_pLanguage; }
        Message*     getStatusMessage() const { return m_pStatusMessage; }

        bool isNull() const { return m_pParser == nullptr && m_pSource == nullptr; }
        bool isEmpty() const { return m_pSource == nullptr; }

        void undo();
        void redo();

        void addDependency(BuildSource* a_pDep);
        bool hasDependency(BuildSource* a_pDep) const
        {
            for (auto& deps : m_PerPassDependencies)
                if (deps.find(a_pDep) != deps.end())
                    return true;
            return false;
        }
        bool hasError() const;
        bool hasSucceeded() const;

        time_t getTime() const { return m_Time; }

    private:
        bool _hasSucceeded(SmallSet<const Session*>& treated) const;
        void clear();

    private:
        SmallVector<SmallSet<BuildSource*>> m_PerPassDependencies;
        BuildSource*                        m_pBuildSource{};
        Message*                            m_pStatusMessage = nullptr;
        Package*                            m_pPackage = nullptr;
        Source*                             m_pSource = nullptr;
        Parser*                             m_pParser = nullptr;
        Semantic*                           m_pSemantic = nullptr;
        Language*                           m_pLanguage = nullptr;
        time_t                              m_Time{};
    };

    static BuildSource::Session const& EmptyBuild();

    BuildSource(BuildSystem* a_pCompiler, SourceStream* a_pSourceStream);
    ~BuildSource();

    Language* getLanguage() const { return getCurrentBuild().getLanguage(); }

    SourceStream* getSourceStream() const { return m_pSourceStream; }

    Source* getSource() const { return getCurrentBuild().getSource(); }

    bool hasEverSucceeded() const { return getCurrentBuild().getSource(); }

    void ensureStrongDependency(LanguageElement* /*a_pWhere*/, Symbol* /*a_pWho*/)
    {
#if 0
        if (!a_pWho->isTemplateDependant() && !hasDependency(a_pWho->getSource()))
        {
            error(a_pWhere->getCodeRangeLocation(), "missing definition of '%.*s' (add 'import %.*s')",
                  PHANTOM_STRING_AS_PRINTF_ARG(a_pWho->getDecoratedName()),
                  PHANTOM_STRING_AS_PRINTF_ARG(a_pWho->getSource()->getUniqueName()));
        }
#endif
    }

    bool hasBuildDependency(BuildSource* a_pSource) const
    {
        if (this == a_pSource)
            return true;
        return getCurrentBuild().hasDependency(a_pSource);
    }

    void addBuildDependency(BuildSource* a_pSource);
    void addBuildDependency(Source* a_pSource);

    SmallVector<SmallSet<BuildSource*>> const& getBuildDependencies() const
    {
        return getCurrentBuild().m_PerPassDependencies;
    }

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this source is compilable (no invalid element + every dependency
    /// compilable).
    ///
    /// \return true if compilable, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasSucceeded() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Outdates this source (mark as obsolete).
    ///
    /// \return the previous build time.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    time_t outdate();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this source has error (after parsing).
    ///
    /// \return true if error, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasError() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Emit error.
    ///
    /// \param  a_Location  The location of the error.
    /// \param  a_Format    "printf" like formatting.
    ///
    /// \return the emitted message.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Message* error(const char* a_Format, ...);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Emit warning.
    ///
    /// \param  a_Location  The location of the warning.
    /// \param  a_Format    "printf" like formatting.
    ///
    /// \return the emitted message.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Message* warning(const char* a_Format, ...);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Emit error.
    ///
    /// \param  a_Location  The location of the error.
    /// \param  a_Format    "printf" like formatting.
    ///
    /// \return the emitted message.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Message* error(const CodeRangeLocation& a_Location, const char* a_Format, ...);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Emit warning.
    ///
    /// \param  a_Location  The location of the warning.
    /// \param  a_Format    "printf" like formatting.
    ///
    /// \return the emitted message.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Message* warning(const CodeRangeLocation& a_Location, const char* a_Format, ...);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a valid build of this source matching the given index.
    ///
    /// \param  a_uiIndex   The build index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Session& getBuild(size_t a_uiIndex) const { return m_BuildStack[a_uiIndex]; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a the build count of this source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getBuildCount() const { return m_BuildStack.size(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the last valid change time.
    ///
    /// \return The last valid change time.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    time_t getLastValidChangeTime() const { return m_uiLastValidBuildTime; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Forces the last valid change time to a new value.
    ///
    /// \param  a_Time  The new last valid change time.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setLastValidChangeTime(time_t a_Time) { m_uiLastValidBuildTime = a_Time; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the status message of this source, where errors and warnings are set.
    ///
    /// \return The status message.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Message* getStatusMessage() const { return getLastBuild().m_pStatusMessage; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an arbitrary priority used to sort this source relative to other source during
    /// compilation.
    ///
    /// \return the priority.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    int getPriority() const { return m_iPriority; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets an arbitrary priority used to sort this source relative to other source during
    /// compilation.
    ///         The highest, the latest to compile
    ///
    /// \param a_iPriority the priority.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setPriority(int a_iPriority) { m_iPriority = a_iPriority; }

    bool isOutdated() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parser associated with this source, if existing.
    ///
    /// \return null if no parser is associated with this source, else the parser.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Parser* getParser() const { return getCurrentBuild().m_pParser; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the semantic associated with this source, if existing.
    ///
    /// \return null if no semantic is associated with this source (no compilation happened), else
    /// the semantic.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Semantic* getSemantic() const { return getCurrentBuild().m_pSemantic; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the compilation options for this source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setOptions(Options const& a_Options) { m_Options = a_Options; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the compilation options for this source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Options const& getOptions() const { return m_Options; }

    Session const& getCurrentBuild() const
    {
        static Session Null;
        if (m_CurrentBuildIndex > -1)
            return m_BuildStack[m_CurrentBuildIndex];
        return Null;
    }

    Session const& getLastBuild() const
    {
        return m_LastBuildIndex == -1 ? m_InvalidBuild : m_BuildStack[m_LastBuildIndex];
    }
    Session const& getPreviousBuild() const
    {
        static Session Null;
        if (m_PreviousBuildIndex > -1)
            return m_BuildStack[m_PreviousBuildIndex];
        return Null;
    }

    void dumpMessages();

private:
    Session& currentBuild() { return (Session&)const_cast<BuildSource*>(this)->getCurrentBuild(); }
    Session& previousBuild() { return (Session&)const_cast<BuildSource*>(this)->getPreviousBuild(); }

    Session& lastBuild() { return m_LastBuildIndex == -1 ? m_InvalidBuild : m_BuildStack[m_LastBuildIndex]; }
    void     stampTime();
    void     _advanceParse(uint a_Pass);
    bool     _isOutdated(SmallSet<BuildSource const*>& _treated) const;

private:
    // begin new build
    int beginBuild(Session a_Build);

    Session createSession(Package* a_pPck, Source* a_pSource, Language* a_pLanguage, Message& a_Message);
    Session createSession(Package* a_pPck, Source* a_pSource, Message& a_Message);

    void abortBuild();
    // revert top build
    bool endBuild();

    void restore(int a_BuildIdx);

    void cancelCurrentSources()
    {
        Session& curr = currentBuild();
        if (!curr.isEmpty())
            curr.undo();
        Session& prev = previousBuild();
        if (!prev.isEmpty())
            prev.redo();
    }
    void restoreCurrentSources()
    {
        Session& prev = previousBuild();
        if (!prev.isEmpty())
            prev.undo();
        Session& curr = currentBuild();
        if (!curr.isEmpty())
            curr.redo();
    }

private:
    SourceStream*        m_pSourceStream = nullptr;
    BuildSystem*         m_pCompiler = nullptr;
    SmallVector<Session> m_BuildStack;
    int                  m_LastBuildIndex = -1;
    Session              m_InvalidBuild;
    int                  m_PreviousBuildIndex = -1;
    Options              m_Options;
    time_t               m_uiLastValidBuildTime = 0;
    int                  m_ArchiveCounter = 1;
    int                  m_iPriority;
    int                  m_CurrentBuildIndex = -1;
    bool                 m_bBuildInProgress = false;
    bool                 m_bOutdated = false;
};
} // namespace lang
} // namespace phantom
