// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "fwd.h"

namespace phantom
{
namespace lang
{
/// \brief  C++ compliant operator ids.
enum class Operator
{
    Plus,
    Minus,
    Add,
    Subtract,
    Multiply,
    Divide,
    Address,
    Dereference,
    Arrow,
    ArrowStar,
    PreDecrement,
    PreIncrement,
    PostDecrement,
    PostIncrement,
    Equal,
    NotEqual,
    Greater,
    Less,
    GreaterEqual,
    LessEqual,
    LogicalAnd,
    LogicalOr,
    XOr,
    Not,
    BitAnd,
    BitOr,
    Complement,
    Modulo,
    Comma,
    ShiftLeft,
    ShiftRight,
    Assignment,
    AssignmentAdd,
    AssignmentSubtract,
    AssignmentMultiply,
    AssignmentDivide,
    AssignmentBitAnd,
    AssignmentBitOr,
    AssignmentModulo,
    AssignmentShiftLeft,
    AssignmentShiftRight,
    AssignmentXOr,
    Bracket,
    Parenthesis,

    Conversion,
    COUNT,
    Unknown,
};

struct Lang
{
    static bool CppTypeConverter(Type* a_pDestType, void* a_pDest, Type* a_pSrcType, void const* a_pSrc);
};

} // namespace lang
} // namespace phantom
