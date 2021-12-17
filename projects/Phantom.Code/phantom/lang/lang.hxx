#pragma once

// haunt {

// clang-format off

#include "lang.h"

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
#include <phantom/struct>
#include <phantom/enum_class>
#include <phantom/static_method>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("lang")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Enums) { this_().enum_<Operator>().values({
            {"Plus",Operator::Plus},
            {"Minus",Operator::Minus},
            {"Add",Operator::Add},
            {"Subtract",Operator::Subtract},
            {"Multiply",Operator::Multiply},
            {"Divide",Operator::Divide},
            {"Address",Operator::Address},
            {"Dereference",Operator::Dereference},
            {"Arrow",Operator::Arrow},
            {"ArrowStar",Operator::ArrowStar},
            {"PreDecrement",Operator::PreDecrement},
            {"PreIncrement",Operator::PreIncrement},
            {"PostDecrement",Operator::PostDecrement},
            {"PostIncrement",Operator::PostIncrement},
            {"Equal",Operator::Equal},
            {"NotEqual",Operator::NotEqual},
            {"Greater",Operator::Greater},
            {"Less",Operator::Less},
            {"GreaterEqual",Operator::GreaterEqual},
            {"LessEqual",Operator::LessEqual},
            {"LogicalAnd",Operator::LogicalAnd},
            {"LogicalOr",Operator::LogicalOr},
            {"XOr",Operator::XOr},
            {"Not",Operator::Not},
            {"BitAnd",Operator::BitAnd},
            {"BitOr",Operator::BitOr},
            {"Complement",Operator::Complement},
            {"Modulo",Operator::Modulo},
            {"Comma",Operator::Comma},
            {"ShiftLeft",Operator::ShiftLeft},
            {"ShiftRight",Operator::ShiftRight},
            {"Assignment",Operator::Assignment},
            {"AssignmentAdd",Operator::AssignmentAdd},
            {"AssignmentSubtract",Operator::AssignmentSubtract},
            {"AssignmentMultiply",Operator::AssignmentMultiply},
            {"AssignmentDivide",Operator::AssignmentDivide},
            {"AssignmentBitAnd",Operator::AssignmentBitAnd},
            {"AssignmentBitOr",Operator::AssignmentBitOr},
            {"AssignmentModulo",Operator::AssignmentModulo},
            {"AssignmentShiftLeft",Operator::AssignmentShiftLeft},
            {"AssignmentShiftRight",Operator::AssignmentShiftRight},
            {"AssignmentXOr",Operator::AssignmentXOr},
            {"Bracket",Operator::Bracket},
            {"Parenthesis",Operator::Parenthesis},
            {"Conversion",Operator::Conversion},
            {"COUNT",Operator::COUNT},
            {"Unknown",Operator::Unknown}});
        }
        PHANTOM_STRUCT(Lang)
        {
            this_()
            .staticMethod<bool(Type*, void*, Type*, void const*)>("CppTypeConverter", &_::CppTypeConverter)({"a_pDestType","a_pDest","a_pSrcType","a_pSrc"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("lang")
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
