// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "MethodPointerAdjustmentExpression.h"

#include <phantom/lang/MethodPointer.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
MethodPointerAdjustmentExpression::MethodPointerAdjustmentExpression(MethodPointer* a_pOutputPointerType,
                                                                     Expression*    a_pPointerExpression,
                                                                     ptrdiff_t      a_iOffset)
    : Expression(a_pOutputPointerType), m_pPointerExpression(a_pPointerExpression), m_iOffset(a_iOffset)
{
    PHANTOM_ASSERT(m_iOffset != 0, "adjustment is not identity and offset must have non zero value");
    PHANTOM_ASSERT(m_pPointerExpression);
}

MethodPointerAdjustmentExpression::~MethodPointerAdjustmentExpression() {}

void MethodPointerAdjustmentExpression::eval(ExecutionContext& a_Context) const
{
#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO /// /vmg + /vmv must be defined
    struct __MicrosoftUnknownMFP
    {
        void*     m_func_address; // 64 bits for Itanium.
        ptrdiff_t m_delta;
        int       m_vtordisp;
        int       m_vtable_index; // or 0 if no virtual inheritance
    };
    __MicrosoftUnknownMFP input;
    m_pPointerExpression->load(a_Context, &input);
    __MicrosoftUnknownMFP& output = *(__MicrosoftUnknownMFP*)a_Context.resultPointer();
    output.m_func_address = input.m_func_address;
    output.m_delta = input.m_delta + m_iOffset;
    output.m_delta = input.m_vtordisp;
    output.m_delta = input.m_vtable_index;
    PHANTOM_ASSERT(input.m_vtable_index == 0, "phantom does not support virtual inheritance");

    //         if (vindex=0) adjustedthis = this + delta
    //         else adjustedthis = this + delta + vtordisp + *(*(this + vtordisp) + vindex)
    //         CALL funcadr
#elif PHANTOM_COMPILER == PHANTOM_COMPILER_GCC || PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
    // GNU g++ uses a tricky space optimisation, also adopted by IBM's VisualAge and XLC.
    struct __GnuClangMFP
    {
        union {
            void*     m_func_address_even; // always even
            ptrdiff_t m_vtable_index_odd;  //  = vindex*2+1, always odd
        };
        ptrdiff_t m_delta;
    };
    //         adjustedthis = this + delta
    //             if (funcadr & 1) CALL (* ( *delta + (vindex+1)/2) + 4)
    //             else CALL funcadr
    __GnuClangMFP input;
    m_pPointerExpression->load(a_Context, &input);
    __GnuClangMFP& output = *(__GnuClangMFP*)a_Context.resultPointer();
    output.m_vtable_index_odd = input.m_vtable_index_odd;
    output.m_delta = input.m_delta + m_iOffset;
#else
#    error define Member Function Pointer struct for the current compiler
#endif
}

MethodPointerAdjustmentExpression* MethodPointerAdjustmentExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<MethodPointerAdjustmentExpression>(static_cast<MethodPointer*>(getValueType()),
                                                            m_pPointerExpression, m_iOffset);
}

} // namespace lang
} // namespace phantom
