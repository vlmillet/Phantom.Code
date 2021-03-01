// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

#include "BuiltInOperator.h"

#include <phantom/lang/FundamentalsT.h>
#include <phantom/lang/TypeOf.h>
#include <phantom/traits/SelectType.h>

namespace phantom
{
namespace lang
{
/// disable unsafe integral operations (we generate code for them because of template genericity,
/// but we will never go through them, so it doesn't matter)

#if defined(_MSC_VER)
#    pragma warning(push, 0)
#elif defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wall"
#    pragma clang diagnostic ignored "-Wextra"
#endif

#define operandT0 (*(T0*)a_pArgs[0])

#define operandT1 (*(T1*)a_pArgs[1])

#define resultR (*(R*)a_pOutput)

#define resultRref (*(R**)a_pOutput)

#define resultRrefCast(c) (R*)(c)

#define resultLR PHANTOM_TYPENAME builtin_operation_delegate_LR<T0, T1>::type

template<typename T0, typename T1 = void, typename R = T0>
struct builtin_operation_delegate_Plus
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0; }
};
template<typename T0, typename T1 = void, typename R = T0>
struct builtin_operation_delegate_Minus
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = -operandT0; }
};
template<typename T0, typename T1 = void, typename R = T0>
struct builtin_operation_delegate_PreDecrement
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&--operandT0); }
};
template<typename T1, typename R>
struct builtin_operation_delegate_PreDecrement<bool, T1, R>
{
    static void apply(void** a_pArgs, void* a_pOutput) { PHANTOM_ASSERT_NO_IMPL(); }
};
template<typename T0, typename T1 = void, typename R = T0>
struct builtin_operation_delegate_PreIncrement
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&++operandT0); }
};
template<typename T0, typename T1 = void, typename R = T0>
struct builtin_operation_delegate_Not
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = !operandT0; }
};
template<typename T0, typename T1 = void, typename R = T0>
struct builtin_operation_delegate_Complement
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = ~operandT0; }
};

template<typename T0, typename T1 = void, typename R = T0>
struct builtin_operation_delegate_PostDecrement
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0--; }
};
template<typename T1, typename R>
struct builtin_operation_delegate_PostDecrement<bool, T1, R>
{
    static void apply(void** a_pArgs, void* a_pOutput) { PHANTOM_ASSERT_NO_IMPL(); }
};
template<typename T0, typename T1 = void, typename R = T0>
struct builtin_operation_delegate_PostIncrement
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0++; }
};

template<typename T0, typename T1>
struct builtin_operation_delegate_LR
    : public SelectType<(int(detail::PrimitiveTypeKindH<T0>::value) > int(detail::PrimitiveTypeKindH<T1>::value)), T0,
                        T1>
{
};

template<typename T0, typename T1, typename R = resultLR>
struct builtin_operation_delegate_Add
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = *(T0*)a_pArgs[0] + operandT1; }
};

template<typename T0, typename T1, typename R = resultLR>
struct builtin_operation_delegate_Subtract
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 - operandT1; }
};

template<typename T0, typename T1, typename R = resultLR>
struct builtin_operation_delegate_Multiply
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 * operandT1; }
};

template<typename T0, typename T1, typename R = resultLR>
struct builtin_operation_delegate_Divide
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 / operandT1; }
};

template<typename T0, typename T1, typename R = bool>
struct builtin_operation_delegate_Equal
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 == operandT1; }
};

template<typename T0, typename T1, typename R = bool>
struct builtin_operation_delegate_NotEqual
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 != operandT1; }
};

template<typename T0, typename T1, typename R = bool>
struct builtin_operation_delegate_Greater
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 > operandT1; }
};

template<typename T0, typename T1, typename R = bool>
struct builtin_operation_delegate_Less
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 < operandT1; }
};

template<typename T0, typename T1, typename R = bool>
struct builtin_operation_delegate_GreaterEqual
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 >= operandT1; }
};

template<typename T0, typename T1, typename R = bool>
struct builtin_operation_delegate_LessEqual
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 <= operandT1; }
};

template<typename T0, typename T1, typename R = bool>
struct builtin_operation_delegate_LogicalAnd
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 && operandT1; }
};

template<typename T0, typename T1, typename R = bool>
struct builtin_operation_delegate_LogicalOr
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 || operandT1; }
};

template<typename T0, typename T1, typename R = resultLR>
struct builtin_operation_delegate_XOr
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 ^ operandT1; }
};

template<typename T0, typename T1, typename R = resultLR>
struct builtin_operation_delegate_BitAnd
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 & operandT1; }
};

template<typename T0, typename T1, typename R = resultLR>
struct builtin_operation_delegate_BitOr
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 | operandT1; }
};

template<typename T0, typename T1, typename R = resultLR>
struct builtin_operation_delegate_Modulo
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 % operandT1; }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_ShiftLeft
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 << operandT1; }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_ShiftRight
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultR = operandT0 >> operandT1; }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_Assignment
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&(operandT0 = operandT1)); }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_AssignmentAdd
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&(operandT0 += operandT1)); }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_AssignmentSubtract
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&(operandT0 -= operandT1)); }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_AssignmentMultiply
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&(operandT0 *= operandT1)); }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_AssignmentDivide
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&(operandT0 /= operandT1)); }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_AssignmentBitAnd
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&(operandT0 &= operandT1)); }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_AssignmentBitOr
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&(operandT0 |= operandT1)); }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_AssignmentModulo
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&(operandT0 %= operandT1)); }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_AssignmentShiftLeft
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&(operandT0 <<= operandT1)); }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_AssignmentShiftRight
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&(operandT0 >>= operandT1)); }
};

template<typename T0, typename T1, typename R = T0>
struct builtin_operation_delegate_AssignmentXOr
{
    static void apply(void** a_pArgs, void* a_pOutput) { resultRref = resultRrefCast(&(operandT0 ^= operandT1)); }
};

#undef operandT0 // (*(T0*)a_pArgs[0])

#undef operandT1 // (*(T1*)a_pArgs[1])

#undef resultR // (*(R*)a_pOutput)

#undef resultRref // (*(R**)a_pOutput)

#define builtin_operation_delegate_selector_input_type_id (*(TypeKind*)a_Data.in[1])
#define builtin_operation_delegate_selector_input_op_id (*(Operator*)a_Data.in[0])
#define builtin_operation_delegate_selector_result (*(BuiltInOperatorDelegate*)a_Data.out[0])

#define builtin_operation_delegate_selector_type_case(t, tk, op)                                                       \
    case TypeKind::tk:                                                                                                 \
        builtin_operation_delegate_selector_result =                                                                   \
        BuiltInOperatorDelegate(&builtin_operation_delegate_##op<t_Ty, t>::apply);                                     \
        return

#define builtin_operation_delegate_selector_op_integral_no_bool_cases(op)                                              \
    builtin_operation_delegate_selector_type_case(char, Char, op);                                                     \
    builtin_operation_delegate_selector_type_case(schar, SChar, op);                                                   \
    builtin_operation_delegate_selector_type_case(uchar, UChar, op);                                                   \
    builtin_operation_delegate_selector_type_case(short, Short, op);                                                   \
    builtin_operation_delegate_selector_type_case(ushort, UShort, op);                                                 \
    builtin_operation_delegate_selector_type_case(int, Int, op);                                                       \
    builtin_operation_delegate_selector_type_case(uint, UInt, op);                                                     \
    builtin_operation_delegate_selector_type_case(long, Long, op);                                                     \
    builtin_operation_delegate_selector_type_case(ulong, ULong, op);                                                   \
    builtin_operation_delegate_selector_type_case(longlong, LongLong, op);                                             \
    builtin_operation_delegate_selector_type_case(ulonglong, ULongLong, op);                                           \
    PHANTOM_IF_WCHAR_T(builtin_operation_delegate_selector_type_case(wchar_t, WChar, op);)                             \
    PHANTOM_IF_CHAR16_T(builtin_operation_delegate_selector_type_case(char16_t, Char16, op);)                          \
    PHANTOM_IF_CHAR32_T(builtin_operation_delegate_selector_type_case(char32_t, Char32, op);)

#define builtin_operation_delegate_selector_op_integral_cases(op)                                                      \
    builtin_operation_delegate_selector_type_case(bool, Bool, op);                                                     \
    builtin_operation_delegate_selector_op_integral_no_bool_cases(op);

#define builtin_operation_delegate_selector_op_floating_cases(op)                                                      \
    builtin_operation_delegate_selector_type_case(float, Float, op);                                                   \
    builtin_operation_delegate_selector_type_case(double, Double, op);                                                 \
    builtin_operation_delegate_selector_type_case(longdouble, LongDouble, op);

#define builtin_operation_delegate_selector_op_integral(op)                                                            \
    case Operator::op:                                                                                                 \
    {                                                                                                                  \
        switch (builtin_operation_delegate_selector_input_type_id)                                                     \
        {                                                                                                              \
            builtin_operation_delegate_selector_op_integral_cases(op)                                                  \
        }                                                                                                              \
    }                                                                                                                  \
        return;

#define builtin_operation_delegate_selector_op_unary(op)                                                               \
    case Operator::op:                                                                                                 \
        builtin_operation_delegate_selector_result =                                                                   \
        BuiltInOperatorDelegate(&builtin_operation_delegate_##op<t_Ty>::apply);                                        \
        return;

#define builtin_operation_delegate_selector_op_unary_no_bool(op)                                                       \
    case Operator::op:                                                                                                 \
        if (PHANTOM_TYPEOF(bool) != a_pInput->removeQualifiers())                                                      \
            builtin_operation_delegate_selector_result =                                                               \
            BuiltInOperatorDelegate(&builtin_operation_delegate_##op<t_Ty>::apply);                                    \
        return;

#define builtin_operation_delegate_selector_op_arithmetic(op)                                                          \
    case Operator::op:                                                                                                 \
    {                                                                                                                  \
        switch (builtin_operation_delegate_selector_input_type_id)                                                     \
        {                                                                                                              \
            builtin_operation_delegate_selector_op_integral_cases(op)                                                  \
            builtin_operation_delegate_selector_op_floating_cases(op)                                                  \
        }                                                                                                              \
    }                                                                                                                  \
        return;

#define builtin_operation_delegate_selector_op_arithmetic_no_bool(op)                                                  \
    case Operator::op:                                                                                                 \
    {                                                                                                                  \
        switch (builtin_operation_delegate_selector_input_type_id)                                                     \
        {                                                                                                              \
            builtin_operation_delegate_selector_op_integral_no_bool_cases(op)                                          \
            builtin_operation_delegate_selector_op_floating_cases(op)                                                  \
        }                                                                                                              \
    }                                                                                                                  \
        return;

struct builtin_operation_delegate_selector : public LanguageElementVisitor
{
    BuiltInOperatorDelegate delegate(Operator a_eOperator, Type* T)
    {
        VisitorData             data;
        const void*             in[1] = {&a_eOperator};
        BuiltInOperatorDelegate result;
        void*                   out[1] = {&result};
        data.in = in;
        data.out = out;
        T->visit(this, data);
        PHANTOM_ASSERT(!result.empty());
        return result;
    }

    BuiltInOperatorDelegate delegate(Operator a_eOperator, Type* L, Type* R)
    {
        VisitorData             data;
        TypeKind                RtypeId = R->getTypeKind();
        const void*             in[2] = {&a_eOperator, &RtypeId};
        BuiltInOperatorDelegate result;
        void*                   out[1] = {&result};
        data.in = in;
        data.out = out;
        L->visit(this, data);
        PHANTOM_ASSERT(!result.empty());
        return result;
    }
#undef Modulo
#undef Bool
    template<typename t_Ty>
    void visitIntegral(IntegralTypeT<t_Ty>* a_pInput, VisitorData a_Data)
    {
        this->visitArithmetic(a_pInput, a_Data);
        if (!builtin_operation_delegate_selector_result) /// not found in 'visitArithmetic'
        {
            switch (builtin_operation_delegate_selector_input_op_id)
            {
                builtin_operation_delegate_selector_op_unary(Complement);
                builtin_operation_delegate_selector_op_integral(Modulo);
                builtin_operation_delegate_selector_op_integral(BitAnd);
                builtin_operation_delegate_selector_op_integral(XOr);
                builtin_operation_delegate_selector_op_integral(BitOr);
                builtin_operation_delegate_selector_op_integral(ShiftLeft);
                builtin_operation_delegate_selector_op_integral(ShiftRight);
                builtin_operation_delegate_selector_op_integral(AssignmentModulo);
                builtin_operation_delegate_selector_op_integral(AssignmentShiftLeft);
                builtin_operation_delegate_selector_op_integral(AssignmentShiftRight);
                builtin_operation_delegate_selector_op_integral(AssignmentBitAnd);
                builtin_operation_delegate_selector_op_integral(AssignmentXOr);
                builtin_operation_delegate_selector_op_integral(AssignmentBitOr);
            }
        }
    }
    template<typename t_Ty>
    void visitArithmetic(ArithmeticTypeT<t_Ty>* a_pInput, VisitorData a_Data)
    {
        switch (builtin_operation_delegate_selector_input_op_id)
        {
            builtin_operation_delegate_selector_op_unary(Plus);
            builtin_operation_delegate_selector_op_unary(Minus);
            builtin_operation_delegate_selector_op_arithmetic(Multiply);
            builtin_operation_delegate_selector_op_arithmetic(Divide);
            builtin_operation_delegate_selector_op_arithmetic(Add);
            builtin_operation_delegate_selector_op_arithmetic(Subtract);
            builtin_operation_delegate_selector_op_arithmetic(Less);
            builtin_operation_delegate_selector_op_arithmetic(Greater);
            builtin_operation_delegate_selector_op_arithmetic(LessEqual);
            builtin_operation_delegate_selector_op_arithmetic(GreaterEqual);
            builtin_operation_delegate_selector_op_arithmetic(Equal);
            builtin_operation_delegate_selector_op_arithmetic(NotEqual);
            builtin_operation_delegate_selector_op_unary(PreIncrement);
            builtin_operation_delegate_selector_op_unary(PostIncrement);
            builtin_operation_delegate_selector_op_unary_no_bool(PreDecrement);
            builtin_operation_delegate_selector_op_unary_no_bool(PostDecrement);
            builtin_operation_delegate_selector_op_arithmetic(Assignment);
            builtin_operation_delegate_selector_op_arithmetic(AssignmentAdd);
            builtin_operation_delegate_selector_op_arithmetic(AssignmentSubtract);
            builtin_operation_delegate_selector_op_arithmetic(AssignmentMultiply);
            builtin_operation_delegate_selector_op_arithmetic(AssignmentDivide);
        }
    }
    template<typename t_Ty>
    void visitFloatingPoint(FloatingPointTypeT<t_Ty>* a_pInput, VisitorData a_Data)
    {
        this->visitArithmetic(a_pInput, a_Data);
    }

    virtual void visit(IntegralTypeT<bool>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(IntegralTypeT<char>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(IntegralTypeT<uchar>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(IntegralTypeT<schar>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(IntegralTypeT<short>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(IntegralTypeT<ushort>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(IntegralTypeT<int>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(IntegralTypeT<uint>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(IntegralTypeT<long>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(IntegralTypeT<ulong>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(IntegralTypeT<longlong>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(IntegralTypeT<ulonglong>* a_pInput, VisitorData a_Data) { visitIntegral(a_pInput, a_Data); }
    virtual void visit(FloatingPointTypeT<float>* a_pInput, VisitorData a_Data)
    {
        visitFloatingPoint(a_pInput, a_Data);
    }
    virtual void visit(FloatingPointTypeT<double>* a_pInput, VisitorData a_Data)
    {
        visitFloatingPoint(a_pInput, a_Data);
    }
    virtual void visit(FloatingPointTypeT<longdouble>* a_pInput, VisitorData a_Data)
    {
        visitFloatingPoint(a_pInput, a_Data);
    }

    PHANTOM_IF_WCHAR_T(virtual void visit(IntegralTypeT<wchar_t>* a_pInput, VisitorData a_Data) {
        visitIntegral(a_pInput, a_Data);
    });
    PHANTOM_IF_CHAR16_T(virtual void visit(IntegralTypeT<char16_t>* a_pInput, VisitorData a_Data) {
        visitIntegral(a_pInput, a_Data);
    });
    PHANTOM_IF_CHAR32_T(virtual void visit(IntegralTypeT<char32_t>* a_pInput, VisitorData a_Data) {
        visitIntegral(a_pInput, a_Data);
    });
};

} // namespace lang
} // namespace phantom

#if defined(_MSC_VER)
#    pragma warning(pop)
#elif defined(__clang__)
#    pragma clang diagnostic pop
#endif

/// @endcond
