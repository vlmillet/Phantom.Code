#pragma once

// haunt {

// clang-format off

#include "fwd.h"

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
#include <phantom/typedef>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("BuiltInOperator")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<BuiltInOperators>("BuiltInOperators"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BuiltInOperator")
    PHANTOM_SOURCE("ExpressionStatement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<ExpressionStatements>("ExpressionStatements"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ExpressionStatement")
    PHANTOM_SOURCE("Expression")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Expressions>("Expressions"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Expression")
    PHANTOM_SOURCE("Statement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Statements>("Statements"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Statement")
    #if PHANTOM_NOT_TEMPLATE
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<ExpressionsView>("ExpressionsView"); }
    PHANTOM_SOURCE("DebugInformation")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<DebugInformations>("DebugInformations"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("DebugInformation")
    PHANTOM_SOURCE("Evaluable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Evaluables>("Evaluables"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Evaluable")
    PHANTOM_SOURCE("Block")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Blocks>("Blocks"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Block")
    PHANTOM_SOURCE("Label")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Labels>("Labels"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Label")
    #endif // PHANTOM_NOT_TEMPLATE
PHANTOM_END("phantom.lang")
}
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("CompiledSource")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<CompiledSources>("CompiledSources"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CompiledSource")
    PHANTOM_SOURCE("Project")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Projects>("Projects"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Project")
    PHANTOM_SOURCE("Solution")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Solutions>("Solutions"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Solution")
    PHANTOM_SOURCE("Message")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Messages>("Messages"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Message")
    PHANTOM_SOURCE("Language")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Languages>("Languages"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Language")
    #if PHANTOM_NOT_TEMPLATE
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<Options>("Options"); }
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<BuildSessionId>("BuildSessionId"); }
    #endif // PHANTOM_NOT_TEMPLATE
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
