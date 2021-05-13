#pragma once

// haunt {

// clang-format off

#include "Lexer.hpp"

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
#include <phantom/union>
#include <phantom/enum>
#include <phantom/variable>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/friend>

namespace std {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Lexer")

        /// missing symbol(s) reflection (std::iterator_traits) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_S((spell::position_iterator), iterator_traits)
        {
            this_()
            // .typedef_<value_type>("value_type")
            /// missing symbol(s) reflection (std::forward_iterator_tag) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .typedef_<iterator_category>("iterator_category")
            ;
        } */
    PHANTOM_END("Lexer")
PHANTOM_END("phantom.lang")
}
namespace std {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Lexer")

        /// missing symbol(s) reflection (std::iterator_traits) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_S((spell::base_iterator_type), iterator_traits)
        {
            this_()
            // .typedef_<value_type>("value_type")
            // .typedef_<difference_type>("difference_type")
            // .typedef_<pointer>("pointer")
            // .typedef_<reference>("reference")
            ;
        } */
    PHANTOM_END("Lexer")
PHANTOM_END("phantom.lang")
}
namespace spell {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Lexer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(any)
        {
            using EType = typedef_<_::EType>;
            this_()
            .enum_<EType>().values({
                {"e_undefined",_::e_undefined},
                {"e_cstring",_::e_cstring},
                {"e_char",_::e_char},
                {"e_uchar",_::e_uchar},
                {"e_int",_::e_int},
                {"e_uint",_::e_uint},
                {"e_short",_::e_short},
                {"e_ushort",_::e_ushort},
                {"e_long",_::e_long},
                {"e_ulong",_::e_ulong},
                {"e_longlong",_::e_longlong},
                {"e_ulonglong",_::e_ulonglong},
                {"e_double",_::e_double},
                {"e_float",_::e_float},
                {"e_bool",_::e_bool}})
            .end()
            .constructor<void(const char*)>()({"v"})
            .constructor<void(char)>()({"v"})
            .constructor<void(unsigned char)>()({"v"})
            .constructor<void(int)>()({"v"})
            .constructor<void(unsigned int)>()({"v"})
            .constructor<void(short)>()({"v"})
            .constructor<void(unsigned short)>()({"v"})
            .constructor<void(long)>()({"v"})
            .constructor<void(unsigned long)>()({"v"})
            .constructor<void(long long)>()({"v"})
            .constructor<void(unsigned long long)>()({"v"})
            .constructor<void(double)>()({"v"})
            .constructor<void(float)>()({"v"})
            .constructor<void(bool)>()({"v"})
            .constructor<void()>()
            .method<any&(const char*)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(char)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(unsigned char)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(int)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(unsigned int)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(short)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(unsigned short)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(long)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(unsigned long)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(long long)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(unsigned long long)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(double)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(float)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<any&(bool)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"v"})
            .method<bool() const>("hasValue", &_::hasValue)
            .method<bool(any&)>("as", &_::as)({"v"})
            .method<bool(const char*&)>("as", &_::as)({"v"})
            .method<bool(char&)>("as", &_::as)({"v"})
            .method<bool(unsigned char&)>("as", &_::as)({"v"})
            .method<bool(int&)>("as", &_::as)({"v"})
            .method<bool(unsigned int&)>("as", &_::as)({"v"})
            .method<bool(short&)>("as", &_::as)({"v"})
            .method<bool(unsigned short&)>("as", &_::as)({"v"})
            .method<bool(long&)>("as", &_::as)({"v"})
            .method<bool(unsigned long&)>("as", &_::as)({"v"})
            .method<bool(long long&)>("as", &_::as)({"v"})
            .method<bool(unsigned long long&)>("as", &_::as)({"v"})
            .method<bool(double&)>("as", &_::as)({"v"})
            .method<bool(float&)>("as", &_::as)({"v"})
            .method<bool(bool&)>("as", &_::as)({"v"})
            .constructor<void(const any&)>()({"other"})
            .method<any&(const any&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"other"})
            .method<const char*&()>("ref_cstring", &_::ref_cstring)
            .method<char&()>("ref_char", &_::ref_char)
            .method<unsigned char&()>("ref_uchar", &_::ref_uchar)
            .method<int&()>("ref_int", &_::ref_int)
            .method<unsigned int&()>("ref_uint", &_::ref_uint)
            .method<short&()>("ref_short", &_::ref_short)
            .method<unsigned short&()>("ref_ushort", &_::ref_ushort)
            .method<long&()>("ref_long", &_::ref_long)
            .method<unsigned long&()>("ref_ulong", &_::ref_ulong)
            .method<long long&()>("ref_longlong", &_::ref_longlong)
            .method<unsigned long long&()>("ref_ulonglong", &_::ref_ulonglong)
            .method<double&()>("ref_double", &_::ref_double)
            .method<float&()>("ref_float", &_::ref_float)
            .method<bool&()>("ref_bool", &_::ref_bool)
            .method<const char*() const>("as_string", &_::as_string)
            .method<char() const>("as_char", &_::as_char)
            .method<unsigned char() const>("as_uchar", &_::as_uchar)
            .method<int() const>("as_int", &_::as_int)
            .method<unsigned int() const>("as_uint", &_::as_uint)
            .method<short() const>("as_short", &_::as_short)
            .method<unsigned short() const>("as_ushort", &_::as_ushort)
            .method<long() const>("as_long", &_::as_long)
            .method<unsigned long() const>("as_ulong", &_::as_ulong)
            .method<long long() const>("as_longlong", &_::as_longlong)
            .method<unsigned long long() const>("as_ulonglong", &_::as_ulonglong)
            .method<double() const>("as_double", &_::as_double)
            .method<float() const>("as_float", &_::as_float)
            .method<bool() const>("as_bool", &_::as_bool)
            .method<EType() const>("type", &_::type)
            .method<void()>("reset", &_::reset)
        
        .protected_()
            .field("_type", &_::_type)
            .union_<>()
                .field("_cstring", &_::_cstring)
                .field("_char", &_::_char)
                .field("_uchar", &_::_uchar)
                .field("_int", &_::_int)
                .field("_uint", &_::_uint)
                .field("_short", &_::_short)
                .field("_ushort", &_::_ushort)
                .field("_long", &_::_long)
                .field("_ulong", &_::_ulong)
                .field("_longlong", &_::_longlong)
                .field("_ulonglong", &_::_ulonglong)
                .field("_double", &_::_double)
                .field("_float", &_::_float)
                .field("_bool", &_::_bool)
            .end()
            ;
        }
        PHANTOM_STRUCT(Position)
        {
            this_()
            .method<bool() const>("isValid", &_::isValid)
            .field("line", &_::line)
            .field("column", &_::column)
            ;
        }
        PHANTOM_STRUCT(Location)
        {
            this_()
            .method<bool() const>("isValid", &_::isValid)
            .field("begin", &_::begin)
            .field("end", &_::end)
            ;
        }
        PHANTOM_STRUCT(Token)
        {
            using EError = typedef_<_::EError>;
            using EWarning = typedef_<_::EWarning>;
            using Errors = typedef_<_::Errors>;
            using Warnings = typedef_<_::Warnings>;
            this_()
            .enum_<EWarning>().values({
                {"e_Warning_OverflowInt",_::e_Warning_OverflowInt},
                {"e_Warning_OverflowUInt",_::e_Warning_OverflowUInt},
                {"e_Warning_OverflowLong",_::e_Warning_OverflowLong},
                {"e_Warning_OverflowULong",_::e_Warning_OverflowULong},
                {"e_Warning_OverflowLongLong",_::e_Warning_OverflowLongLong},
                {"e_Warning_OverflowULongLong",_::e_Warning_OverflowULongLong},
                {"e_Warning_OverflowFloat",_::e_Warning_OverflowFloat}})
            .end()
            .typedef_<Warnings>("Warnings")
            .enum_<EError>().values({
                {"e_Error_InvalidToken",_::e_Error_InvalidToken},
                {"e_Error_OverflowMaximumIntegerRepresentation",_::e_Error_OverflowMaximumIntegerRepresentation},
                {"e_Error_OverflowMaximumFloatingPointRepresentation",_::e_Error_OverflowMaximumFloatingPointRepresentation},
                {"e_Error_InvalidUnicodeCodePointFormatting",_::e_Error_InvalidUnicodeCodePointFormatting}})
            .end()
            .typedef_<Errors>("Errors")
            .field("id", &_::id)
            .field("location", &_::location)
            .field("value", &_::value)
            .field("warnings", &_::warnings)
            .field("errors", &_::errors)
            .field("begin", &_::begin)
            .field("end", &_::end)
            ;
        }
        /// missing symbol(s) reflection (std::vector) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<Tokens>("Tokens"); }
        PHANTOM_CLASS(Lexer)
        {
            using Allocator = typedef_<_::Allocator>;
            using EStatus = typedef_<_::EStatus>;
            using RuleType = typedef_<_::RuleType>;
            using adder = typedef_<_::adder>;
            this_()
        
        .public_()
            .staticMethod<char*()>("as_char", &_::as_char)
            .staticMethod<bool*()>("as_bool", &_::as_bool)
            .staticMethod<unsigned long*()>("as_unsigned_long", &_::as_unsigned_long)
            .staticMethod<unsigned int*()>("as_unsigned_int", &_::as_unsigned_int)
            .staticMethod<unsigned long long*()>("as_unsigned_long_long", &_::as_unsigned_long_long)
            .staticMethod<unsigned long*()>("as_ulong", &_::as_ulong)
            .staticMethod<unsigned int*()>("as_uint", &_::as_uint)
            .staticMethod<unsigned long long*()>("as_ulonglong", &_::as_ulonglong)
            .staticMethod<long*()>("as_long", &_::as_long)
            .staticMethod<int*()>("as_int", &_::as_int)
            .staticMethod<long long*()>("as_longlong", &_::as_longlong)
            .staticMethod<long long*()>("as_long_long", &_::as_long_long)
            .staticMethod<float*()>("as_float", &_::as_float)
            .staticMethod<double*()>("as_double", &_::as_double)
            .staticMethod<const char**()>("as_string", &_::as_string)
            .struct_<Allocator>()
                .field("alloc", &_::Allocator::alloc)
                .field("dealloc", &_::Allocator::dealloc)
            .end()
        
        .protected_()
            .struct_<RuleType>()
                .constructor<void(), default_>()
                .constructor<void(char)>()({"c"})
                .constructor<void(const char*)>()({"str"})
                .method<const char*() const>("operator const char*", &_::RuleType::operator notypedef<const char*>)
            .end()
            .struct_<adder>()
                .constructor<void(Lexer&)>()({"lex"})
                .method<adder&(RuleType, int)>("operator()", &_::adder::operator())({"rule","tokId"})
                .method<adder&(RuleType, int, const char*)>("operator()", &_::adder::operator())({"rule","tokId","nextState"})
                .method<adder&(RuleType, const char*)>("operator()", &_::adder::operator())({"rule","nextState"})
                .method<adder&(const char*, RuleType, const char*)>("operator()", &_::adder::operator())({"currentState","rule","nextState"})
                .method<adder&(const char*, RuleType, int, const char*)>("operator()", &_::adder::operator())({"currentState","rule","tokId","nextState"})
                .method<adder&(const char*, RuleType, int)>("operator()", &_::adder::operator())({"currentState","rule","tokId"})
                .method<adder&(char*, RuleType, int)>("operator()", &_::adder::operator())({"","rule","tokId"})
                .method<adder&(bool*, RuleType, int)>("operator()", &_::adder::operator())({"","rule","tokId"})
                .method<adder&(unsigned long*, RuleType, int)>("operator()", &_::adder::operator())({"","rule","tokId"})
                .method<adder&(unsigned int*, RuleType, int)>("operator()", &_::adder::operator())({"","rule","tokId"})
                .method<adder&(unsigned long long*, RuleType, int)>("operator()", &_::adder::operator())({"","rule","tokId"})
                .method<adder&(long*, RuleType, int)>("operator()", &_::adder::operator())({"","rule","tokId"})
                .method<adder&(int*, RuleType, int)>("operator()", &_::adder::operator())({"","rule","tokId"})
                .method<adder&(long long*, RuleType, int)>("operator()", &_::adder::operator())({"","rule","tokId"})
                .method<adder&(float*, RuleType, int)>("operator()", &_::adder::operator())({"","rule","tokId"})
                .method<adder&(double*, RuleType, int)>("operator()", &_::adder::operator())({"","rule","tokId"})
                .method<adder&(const char**, RuleType, int)>("operator()", &_::adder::operator())({"","rule","tokId"})
            .end()
            .field("m_adder", &_::m_adder)
        
        .public_()
            .constructor<void()>()
            .constructor<void(Allocator)>()({"allocator"})
            .enum_<EStatus>().values({
                {"e_Status_Ready",_::e_Status_Ready},
                {"e_Status_Error",_::e_Status_Error},
                {"e_Status_Finished",_::e_Status_Finished}})
            .end()
            .method<adder&(RuleType, int)>("addRule", &_::addRule)({"rule","tokId"})
            .method<adder&(RuleType, const char*)>("addRule", &_::addRule)({"rule","nextState"})
            .method<adder&(RuleType, int, const char*)>("addRule", &_::addRule)({"rule","tokId","nextState"})
            .method<adder&(const char*, RuleType, const char*)>("addRule", &_::addRule)({"currentState","rule","nextState"})
            .method<adder&(const char*, RuleType, int, const char*)>("addRule", &_::addRule)({"currentState","rule","tokId","nextState"})
            .method<adder&(const char*, RuleType, int)>("addRule", &_::addRule)({"currentState","rule","tokId"})
            .method<adder&(char*, RuleType, int)>("addRule", &_::addRule)({"v","rule","tokId"})
            .method<adder&(bool*, RuleType, int)>("addRule", &_::addRule)({"v","rule","tokId"})
            .method<adder&(unsigned long*, RuleType, int)>("addRule", &_::addRule)({"v","rule","tokId"})
            .method<adder&(unsigned int*, RuleType, int)>("addRule", &_::addRule)({"v","rule","tokId"})
            .method<adder&(unsigned long long*, RuleType, int)>("addRule", &_::addRule)({"v","rule","tokId"})
            .method<adder&(long*, RuleType, int)>("addRule", &_::addRule)({"v","rule","tokId"})
            .method<adder&(int*, RuleType, int)>("addRule", &_::addRule)({"v","rule","tokId"})
            .method<adder&(long long*, RuleType, int)>("addRule", &_::addRule)({"v","rule","tokId"})
            .method<adder&(float*, RuleType, int)>("addRule", &_::addRule)({"v","rule","tokId"})
            .method<adder&(double*, RuleType, int)>("addRule", &_::addRule)({"v","rule","tokId"})
            .method<adder&(const char**, RuleType, int)>("addRule", &_::addRule)({"v","rule","tokId"})
            .method<void(const char*)>("skip", &_::skip)({"regex"})
            .method<void(const char*, const char*)>("addPattern", &_::addPattern)({"name","regex"})
            .method<EStatus(const char*, size_t, const char*)>("begin", &_::begin)({"input","len","initialState"})["nullptr"]
            .method<void()>("end", &_::end)
            .method<EStatus(Token&)>("next", &_::next)({"tok"})
            .method<EStatus() const>("getStatus", &_::getStatus)
        
        .protected_()
            .field("_this", &_::_this)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Lexer")
PHANTOM_END("phantom.lang")
}
namespace spell {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Lexer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<wchar>("wchar"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<uint>("uint"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<ulong>("ulong"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<ulonglong>("ulonglong"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<longlong>("longlong"); }
        PHANTOM_STRUCT(hex)
        {
            this_()
            .constructor<void(unsigned long), explicit_>()({"v"})
            .method<unsigned long long(), explicit_>("operator unsigned long long", &_::operator notypedef<unsigned long long>)
        
        .protected_()
            .field("value", &_::value)
            ;
        }
        PHANTOM_REGISTER(Variables) { this_().variable("null_base_iterator_type", &null_base_iterator_type); }
        PHANTOM_STRUCT(base_iterator_type)
        {
            this_()
            .constructor<void(), default_>()
            .constructor<void(const char*&, const char*)>()({"input","end"})
            .method<char const&() const>("operator*", &_::operator*)
            .method<char const*() const>("operator->", &_::operator->)
            .method<base_iterator_type&()>("operator++", &_::operator++)
            .method<base_iterator_type(int)>("operator++", &_::operator++)({""})
            .method<bool(base_iterator_type const&) const>("operator==", &_::operator==)({"other"})
            .method<bool(base_iterator_type const&) const>("operator!=", &_::operator!=)({"other"})
            .field("end", &_::end)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Lexer")
PHANTOM_END("phantom.lang")
}
namespace spell {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Lexer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(position_iterator)
        {
            using value_type = typedef_<_::value_type>;
            this_()
        .public_()
            .typedef_<value_type>("value_type")
            .constructor<void(), default_>()
            .constructor<void(const char*)>()({"base"})
            .method<position_iterator&()>("operator++", &_::operator++)
            .method<void(int)>("operator++", &_::operator++)({""})
            .method<char const&() const>("operator*", &_::operator*)
            .method<char const*() const>("operator->", &_::operator->)
            .method<bool(position_iterator const&) const>("operator==", &_::operator==)({"other"})
            .method<bool(position_iterator const&) const>("operator!=", &_::operator!=)({"other"})
            .method<bool(position_iterator const&) const>("operator<", &_::operator<)({"other"})
            .method<bool(position_iterator const&) const>("operator>", &_::operator>)({"other"})
            .method<int() const>("get_line", &_::get_line)
            .method<int() const>("get_col", &_::get_col)
            .method<ptrdiff_t(position_iterator) const>("operator-", &_::operator-)({"other"})
            .method<const char*() const>("operator const char*", &_::operator notypedef<const char*>)
        
        .protected_()
            .field("base", &_::base)
            .field("line", &_::line)
            .field("col", &_::col)
            .field("prev", &_::prev)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Lexer")
PHANTOM_END("phantom.lang")
}
namespace spell {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Lexer")

        #if PHANTOM_NOT_TEMPLATE
        /// missing symbol(s) reflection (lexertl::match_results) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<position_match>("position_match"); }
        /// missing symbol(s) reflection (lexertl::state_machine, , lexertl::iterator) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<tok_iterator>("tok_iterator"); }
        PHANTOM_STRUCT(LexerPrivate)
        {
            using Any = typedef_<_::Any>;
            this_()
            /// missing symbol(s) reflection (lexertl::rules) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("rules", &_::rules)
            /// missing symbol(s) reflection (lexertl::state_machine) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("sm", &_::sm)
            /// missing symbol(s) reflection (std::map) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("tokToAction", &_::tokToAction)
            .field("begin_char_it", &_::begin_char_it)
            .field("end_char_it", &_::end_char_it)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("begin_tok_it", &_::begin_tok_it)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("end_tok_it", &_::end_tok_it)
            .field("stream", &_::stream)
            .field("endstream", &_::endstream)
            .field("state", &_::state)
            .field("currentToken", &_::currentToken)
            .field("readFirstToken", &_::readFirstToken)
            .struct_<Any>()
            .end()
            /// missing symbol(s) reflection (std::vector) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("stringPool", &_::stringPool)
            .field("allocator", &_::allocator)
            .constructor<void(::spell::Lexer::Allocator)>()({"allocator"})
            .method<const char*(const char*, size_t)>("addOrGetString", &_::addOrGetString)({"cstr","len"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("defaultAction", &_::defaultAction)({"lexer","begin","end"})
            .method<void(::spell::Token::EError)>("error", &_::error)({"err"})
            .method<void(::spell::Token::EWarning)>("warning", &_::warning)({"warn"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_bool", &_::parse_bool)({"lexer","begin","end"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_hex", &_::parse_hex)({"lexer","begin","end"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_double", &_::parse_double)({"lexer","begin","end"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_float", &_::parse_float)({"lexer","begin","end"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_uint", &_::parse_uint)({"lexer","begin","end"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_ulong", &_::parse_ulong)({"lexer","begin","end"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_ulonglong", &_::parse_ulonglong)({"lexer","begin","end"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_int", &_::parse_int)({"lexer","begin","end"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_long", &_::parse_long)({"lexer","begin","end"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_longlong", &_::parse_longlong)({"lexer","begin","end"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_char", &_::parse_char)({"lexer","begin","end"})
            .staticMethod<void(LexerPrivate&, position_iterator const&, position_iterator const&)>("parse_cstring", &_::parse_cstring)({"lexer","begin","end"})
            ;
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<cstring>("cstring"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Lexer")
PHANTOM_END("phantom.lang")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
