#pragma once

// haunt {

// clang-format off

#include "SemanticPrivate.h"

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
#include <phantom/function>
#include <phantom/variable>
#include <phantom/static_method>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("SemanticPrivate")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Variables) { this_().variable("_canBe", &_canBe); }
        PHANTOM_REGISTER(Variables) { this_().variable("_or", &_or); }
        /// missing symbol(s) reflection (va_list) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Functions) { this_().function<String(const char*, va_list)>("stringf", stringf)({"a_Format","args"});}
        PHANTOM_REGISTER(Functions) { this_().function<StringView(Access)>("getAccessString", getAccessString)({"a_Access"});}
        PHANTOM_CLASS(SemanticPrivate)
        {
            this_()
        .public_()
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SemanticPrivate")
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
