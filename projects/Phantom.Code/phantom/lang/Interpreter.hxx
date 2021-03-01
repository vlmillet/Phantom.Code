#pragma once

// haunt {

// clang-format off

#include "Interpreter.h"

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
#include <phantom/method>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Interpreter")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Interpreter)
        {
            this_()
        .public_()
            .method<bool(ExecutionContext&, Subroutine*, void**, size_t)>("call", &_::call)
            .method<void(Statement*)>("setNextStatement", &_::setNextStatement)
            .method<byte*&()>("stackPointer", &_::stackPointer)
            .method<byte*() const>("getStackPointer", &_::getStackPointer)
            .method<byte*() const>("getBasePointer", &_::getBasePointer)
            .method<void*() const>("getReturnAddress", &_::getReturnAddress)
            .method<void(void*)>("setDefaultThisPointer", &_::setDefaultThisPointer)
            .method<void* const*() const>("getDefaultThisPointerAddress", &_::getDefaultThisPointerAddress)
            .method<void*() const>("getDefaultThisPointer", &_::getDefaultThisPointer)
            ;
        }
        PHANTOM_CLASS(InterpreterContext)
        {
            this_()
            .inherits<::phantom::lang::ExecutionContext>()
        .public_()
            .method<Interpreter*()>("getInterpreter", &_::getInterpreter)
            .method<bool(Subroutine*, void**, size_t), virtual_|override_>("call", &_::call)
            .method<void(Statement*), virtual_|override_>("setNextStatement", &_::setNextStatement)
            .method<byte*() const, virtual_|override_>("getStackPointer", &_::getStackPointer)
            .method<byte*() const, virtual_|override_>("getBasePointer", &_::getBasePointer)
            .method<void*() const, virtual_|override_>("getReturnAddress", &_::getReturnAddress)
            .method<byte*() const, virtual_|override_>("addressOfThis", &_::addressOfThis)
            .method<byte*(LocalVariable*) const, virtual_|override_>("addressOf", &_::addressOf)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Interpreter")
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
