#pragma once

// haunt {

// clang-format off

#include "Project.h"

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
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Project")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(ProjectData)
        {
            using Options = typedef_< phantom::lang::Options>;
            using Strings = typedef_< phantom::Strings>;
            this_()
            .field("files", &_::files)
            .field("dependencies", &_::dependencies)
            .field("options", &_::options)
            ;
        }
        PHANTOM_CLASS(Project)
        {
            using CompiledSources = typedef_< phantom::lang::CompiledSources>;
            using Modules = typedef_< phantom::lang::Modules>;
            using Options = typedef_< phantom::lang::Options>;
            using Projects = typedef_< phantom::lang::Projects>;
            using SourceFiles = typedef_< phantom::lang::SourceFiles>;
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
        
        .public_()
            .constructor<void(Solution*, StringView, Module*)>()({"a_pSolution","a_Path","a_pModule"})
            .method<bool(ProjectData)>("setData", &_::setData)({"_data"})
            .method<ProjectData() const>("getData", &_::getData)
            .method<Solution*() const>("getSolution", &_::getSolution)
            .method<int() const>("getDependencyLevel", &_::getDependencyLevel)
            .method<StringView() const>("getSolutionRelativePath", &_::getSolutionRelativePath)
            .method<String() const>("getPath", &_::getPath)
            .method<StringView() const>("getName", &_::getName)
            .method<void(StringView)>("setName", &_::setName)({"a_Name"})
            .method<StringView() const>("getOutputDir", &_::getOutputDir)
            .method<Options const&() const>("getOptions", &_::getOptions)
            .method<void(Options const&)>("setOptions", &_::setOptions)({"_options"})
            .method<SourceFiles const&() const>("getSourceFiles", &_::getSourceFiles)
            .method<Modules const&() const>("getDependencies", &_::getDependencies)
            .method<Projects() const>("getDependenciesProjects", &_::getDependenciesProjects)
            .method<Module*() const>("getModule", &_::getModule)
            .method<String() const>("getSourcePath", &_::getSourcePath)
            .method<Package*(SourceStream*) const>("getPackageForSourceStream", &_::getPackageForSourceStream)({"a_pStream"})
            .method<SourceFile*(StringView) const>("getSourceFileByPath", &_::getSourceFileByPath)({"a_Path"})
            .method<bool(StringView)>("addDependency", &_::addDependency)({"a_ModuleName"})
            .method<void(Module*)>("addDependency", &_::addDependency)({"a_pModule"})
            .method<void(Module*)>("removeDependency", &_::removeDependency)({"a_pModule"})
            .method<bool(Module*) const>("hasDependency", &_::hasDependency)({"a_pModule"})
            .method<SourceFile*(StringView, StringView)>("addSourceFile", &_::addSourceFile)({"a_RelativePath","a_Code"})["\"\""]
            .method<bool(SourceFile*)>("addSourceFile", &_::addSourceFile)({"a_pSourceFile"})
            .method<void(SourceFile*)>("removeSourceFile", &_::removeSourceFile)({"a_pSourceFile"})
            .method<bool() const>("isPathExisting", &_::isPathExisting)
            .method<bool(Project*) const>("hasProjectDependencyCascade", &_::hasProjectDependencyCascade)({"a_pOther"})
            .method<CompiledSources() const>("getCompiledSources", &_::getCompiledSources)
            .method<void(CompiledSources&) const>("getCompiledSources", &_::getCompiledSources)({"_out"})
        
        .private_()
            .method<void()>("removeFromDisk", &_::removeFromDisk)
        
        .private_()
            .field("m_pSolution", &_::m_pSolution)
            .field("m_Name", &_::m_Name)
            .field("m_OutputDir", &_::m_OutputDir)
            .field("m_Path", &_::m_Path)
            .field("m_Options", &_::m_Options)
            .field("m_SourceFiles", &_::m_SourceFiles)
            .field("m_Dependencies", &_::m_Dependencies)
            .field("m_pModule", &_::m_pModule)
            .field("m_pMessage", &_::m_pMessage)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Project")
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
