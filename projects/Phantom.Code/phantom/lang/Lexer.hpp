#pragma once

#include <assert.h>
#include <string>
#include <vector>

#if !defined(HAUNT_OFF) // for __HAUNT__
#    define HAUNT_OFF
#endif

namespace spell
{
struct LexerPrivate;
struct any
{
    enum EType
    {
        e_undefined,
        e_cstring,
        e_char,
        e_uchar,
        e_int,
        e_uint,
        e_short,
        e_ushort,
        e_long,
        e_ulong,
        e_longlong,
        e_ulonglong,
        e_double,
        e_float,
        e_bool,
    };

    any(const char* v) : _cstring(v), _type(e_cstring)
    {
    }
    any(char v) : _char(v), _type(e_char)
    {
    }
    any(unsigned char v) : _uchar(v), _type(e_uchar)
    {
    }
    any(int v) : _int(v), _type(e_int)
    {
    }
    any(unsigned int v) : _uint(v), _type(e_uint)
    {
    }
    any(short v) : _short(v), _type(e_short)
    {
    }
    any(unsigned short v) : _ushort(v), _type(e_ushort)
    {
    }
    any(long v) : _long(v), _type(e_long)
    {
    }
    any(unsigned long v) : _ulong(v), _type(e_ulong)
    {
    }
    any(long long v) : _longlong(v), _type(e_longlong)
    {
    }
    any(unsigned long long v) : _ulonglong(v), _type(e_ulonglong)
    {
    }
    any(double v) : _double(v), _type(e_double)
    {
    }
    any(float v) : _float(v), _type(e_float)
    {
    }
    any(bool v) : _bool(v), _type(e_bool)
    {
    }
    any() : _type(e_undefined)
    {
    }

    any& operator=(const char* v)
    {
        _cstring = (v);
        _type = (e_cstring);
        return *this;
    }
    any& operator=(char v)
    {
        _char = v;
        _type = (e_char);
        return *this;
    }
    any& operator=(unsigned char v)
    {
        _uchar = v;
        _type = (e_uchar);
        return *this;
    }
    any& operator=(int v)
    {
        _int = v;
        _type = (e_int);
        return *this;
    }
    any& operator=(unsigned int v)
    {
        _uint = v;
        _type = (e_uint);
        return *this;
    }
    any& operator=(short v)
    {
        _short = v;
        _type = (e_short);
        return *this;
    }
    any& operator=(unsigned short v)
    {
        _ushort = v;
        _type = (e_ushort);
        return *this;
    }
    any& operator=(long v)
    {
        _long = v;
        _type = (e_long);
        return *this;
    }
    any& operator=(unsigned long v)
    {
        _ulong = v;
        _type = (e_ulong);
        return *this;
    }
    any& operator=(long long v)
    {
        _longlong = v;
        _type = (e_longlong);
        return *this;
    }
    any& operator=(unsigned long long v)
    {
        _ulonglong = v;
        _type = (e_ulonglong);
        return *this;
    }
    any& operator=(double v)
    {
        _double = v;
        _type = (e_double);
        return *this;
    }
    any& operator=(float v)
    {
        _float = v;
        _type = (e_float);
        return *this;
    }
    any& operator=(bool v)
    {
        _bool = v;
        _type = (e_bool);
        return *this;
    }

    bool hasValue() const
    {
        return _type != e_undefined;
    }
    bool as(any& v)
    {
        v = *this;
        return true;
    }
    bool as(const char*& v)
    {
        if (_type == e_cstring)
        {
            v = _cstring;
            return true;
        }
        return false;
    }
    bool as(char& v)
    {
        if (_type == e_char)
        {
            v = _char;
            return true;
        }
        return false;
    }
    bool as(unsigned char& v)
    {
        if (_type == e_uchar)
        {
            v = _uchar;
            return true;
        }
        return false;
    }
    bool as(int& v)
    {
        if (_type == e_int)
        {
            v = _int;
            return true;
        }
        return false;
    }
    bool as(unsigned int& v)
    {
        if (_type == e_uint)
        {
            v = _uint;
            return true;
        }
        return false;
    }
    bool as(short& v)
    {
        if (_type == e_short)
        {
            v = _short;
            return true;
        }
        return false;
    }
    bool as(unsigned short& v)
    {
        if (_type == e_ushort)
        {
            v = _ushort;
            return true;
        }
        return false;
    }
    bool as(long& v)
    {
        if (_type == e_long)
        {
            v = _long;
            return true;
        }
        return false;
    }
    bool as(unsigned long& v)
    {
        if (_type == e_ulong)
        {
            v = _ulong;
            return true;
        }
        return false;
    }
    bool as(long long& v)
    {
        if (_type == e_longlong)
        {
            v = _longlong;
            return true;
        }
        return false;
    }
    bool as(unsigned long long& v)
    {
        if (_type == e_ulonglong)
        {
            v = _ulonglong;
            return true;
        }
        return false;
    }
    bool as(double& v)
    {
        if (_type == e_double)
        {
            v = _double;
            return true;
        }
        return false;
    }
    bool as(float& v)
    {
        if (_type == e_float)
        {
            v = _float;
            return true;
        }
        return false;
    }
    bool as(bool& v)
    {
        if (_type == e_bool)
        {
            v = _bool;
            return true;
        }
        return false;
    }

    any(const any& other)
    {
        memcpy(this, &other, sizeof(any));
    }
    any& operator=(const any& other)
    {
        memcpy(this, &other, sizeof(any));
        return *this;
    }

    const char*& ref_cstring()
    {
        _type = e_cstring;
        return _cstring;
    }
    char& ref_char()
    {
        _type = e_char;
        return _char;
    }
    unsigned char& ref_uchar()
    {
        _type = e_uchar;
        return _uchar;
    }
    int& ref_int()
    {
        _type = e_int;
        return _int;
    }
    unsigned int& ref_uint()
    {
        _type = e_uint;
        return _uint;
    }
    short& ref_short()
    {
        _type = e_short;
        return _short;
    }
    unsigned short& ref_ushort()
    {
        _type = e_ushort;
        return _ushort;
    }
    long& ref_long()
    {
        _type = e_long;
        return _long;
    }
    unsigned long& ref_ulong()
    {
        _type = e_ulong;
        return _ulong;
    }
    long long& ref_longlong()
    {
        _type = e_longlong;
        return _longlong;
    }
    unsigned long long& ref_ulonglong()
    {
        _type = e_ulonglong;
        return _ulonglong;
    }
    double& ref_double()
    {
        _type = e_double;
        return _double;
    }
    float& ref_float()
    {
        _type = e_float;
        return _float;
    }
    bool& ref_bool()
    {
        _type = e_bool;
        return _bool;
    }

    const char* as_string() const
    {
        assert(_type == e_cstring);
        return _cstring;
    }
    char as_char() const
    {
        assert(_type == e_char);
        return _char;
    }
    unsigned char as_uchar() const
    {
        assert(_type == e_uchar);
        return _uchar;
    }
    int as_int() const
    {
        assert(_type == e_int);
        return _int;
    }
    unsigned int as_uint() const
    {
        assert(_type == e_uint);
        return _uint;
    }
    short as_short() const
    {
        assert(_type == e_short);
        return _short;
    }
    unsigned short as_ushort() const
    {
        assert(_type == e_ushort);
        return _ushort;
    }
    long as_long() const
    {
        assert(_type == e_long);
        return _long;
    }
    unsigned long as_ulong() const
    {
        assert(_type == e_ulong);
        return _ulong;
    }
    long long as_longlong() const
    {
        assert(_type == e_longlong);
        return _longlong;
    }
    unsigned long long as_ulonglong() const
    {
        assert(_type == e_ulonglong);
        return _ulonglong;
    }
    double as_double() const
    {
        assert(_type == e_double);
        return _double;
    }
    float as_float() const
    {
        assert(_type == e_float);
        return _float;
    }
    bool as_bool() const
    {
        assert(_type == e_bool);
        return _bool;
    }

    EType type() const
    {
        return _type;
    }
    void reset()
    {
        _type = e_undefined;
    }

protected:
    EType _type;
    union {
        const char*        _cstring;
        char               _char;
        unsigned char      _uchar;
        int                _int;
        unsigned int       _uint;
        short              _short;
        unsigned short     _ushort;
        long               _long;
        unsigned long      _ulong;
        long long          _longlong;
        unsigned long long _ulonglong;
        double             _double;
        float              _float;
        bool               _bool;
    };
};

struct Position
{
    inline bool isValid() const
    {
        return line >= 0 && column >= 0;
    }
    int line;
    int column;
};

struct Location
{
    inline bool isValid() const
    {
        return begin.isValid() && end.isValid();
    }
    Position begin{-1, -1};
    Position end{-1, -1};
};

struct Token
{
    enum EWarning
    {
        e_Warning_OverflowInt = 0x1,
        e_Warning_OverflowUInt = 0x2,
        e_Warning_OverflowLong = 0x4,
        e_Warning_OverflowULong = 0x8,
        e_Warning_OverflowLongLong = 0x10,
        e_Warning_OverflowULongLong = 0x20,
        e_Warning_OverflowFloat = 0x40,
    };
    typedef int Warnings;

    enum EError
    {
        e_Error_InvalidToken = 0x1,
        e_Error_OverflowMaximumIntegerRepresentation = 0x2,
        e_Error_OverflowMaximumFloatingPointRepresentation = 0x4,
        e_Error_InvalidUnicodeCodePointFormatting = 0x8,
    };
    typedef int Errors;

    int         id = -1;
    Location    location;
    any         value;
    Warnings    warnings = 0;
    Errors      errors = 0;
    const char* begin = 0;
    const char* end = 0;
};
typedef std::vector<Token> Tokens;
class Lexer
{
    friend struct LexerPrivate;

public:
    inline static char* as_char()
    {
        return nullptr;
    }
    inline static bool* as_bool()
    {
        return nullptr;
    }
    inline static unsigned long* as_unsigned_long()
    {
        return nullptr;
    }
    inline static unsigned int* as_unsigned_int()
    {
        return nullptr;
    }
    inline static unsigned long long* as_unsigned_long_long()
    {
        return nullptr;
    }
    inline static unsigned long* as_ulong()
    {
        return nullptr;
    }
    inline static unsigned int* as_uint()
    {
        return nullptr;
    }
    inline static unsigned long long* as_ulonglong()
    {
        return nullptr;
    }
    inline static long* as_long()
    {
        return nullptr;
    }
    inline static int* as_int()
    {
        return nullptr;
    }
    inline static long long* as_longlong()
    {
        return nullptr;
    }
    inline static long long* as_long_long()
    {
        return nullptr;
    }
    inline static float* as_float()
    {
        return nullptr;
    }
    inline static double* as_double()
    {
        return nullptr;
    }
    inline static const char** as_string()
    {
        return nullptr;
    }

    struct Allocator
    {
        void* (*alloc)(size_t n) = ::malloc;
        void (*dealloc)(void*) = ::free;
    };

protected:
    struct RuleType
    {
        RuleType() = default;
        RuleType(char c)
        {
            switch (c)
            {
            case '[':
            case ']':
            case '^':
            case '$':
            case '{':
            case '}':
            case '(':
            case ')':
            case '.':
            case '*':
            case '+':
            case '|':
            case '?':
            case '/':
            case '\\':
                cstr[0] = '\\';
                cstr[1] = c;
                break;
            default:
                cstr[0] = c;
                break;
            }
        }
        RuleType(const char* str) : str(str)
        {
        }
        operator const char*() const
        {
            return str;
        }

    private:
        char        cstr[3] = {0, 0, 0};
        char const* str = cstr;
    };

    struct adder
    {
        adder(Lexer& lex) : m_lex(lex)
        {
        }
        adder& operator()(RuleType rule, int tokId);
        adder& operator()(RuleType rule, int tokId, const char* nextState);
        adder& operator()(RuleType rule, const char* nextState);
        adder& operator()(const char* currentState, RuleType rule, const char* nextState);
        adder& operator()(const char* currentState, RuleType rule, int tokId, const char* nextState);
        adder& operator()(const char* currentState, RuleType rule, int tokId);

        template<typename T>
        adder& operator()(RuleType rule, int tokId)
        {
            static_assert((std::is_fundamental<T>::value || std::is_same<std::string, T>::value),
                          "token value type must a fundamental type or std::basic_string");
            return operator()((T*)0, rule, tokId);
        }
        adder& operator()(char*, RuleType rule, int tokId);
        adder& operator()(bool*, RuleType rule, int tokId);
        adder& operator()(unsigned long*, RuleType rule, int tokId);
        adder& operator()(unsigned int*, RuleType rule, int tokId);
        adder& operator()(unsigned long long*, RuleType rule, int tokId);
        adder& operator()(long*, RuleType rule, int tokId);
        adder& operator()(int*, RuleType rule, int tokId);
        adder& operator()(long long*, RuleType rule, int tokId);
        adder& operator()(float*, RuleType rule, int tokId);
        adder& operator()(double*, RuleType rule, int tokId);
        adder& operator()(const char**, RuleType rule, int tokId);

        HAUNT_OFF Lexer& m_lex; ///< @off

    } m_adder;
    friend struct adder;

public:
    Lexer();
    Lexer(Allocator allocator);
    ~Lexer();

    enum EStatus
    {
        e_Status_Ready,
        e_Status_Error,
        e_Status_Finished,
    };

    adder& addRule(RuleType rule, int tokId)
    {
        return m_adder(rule, tokId);
    }
    adder& addRule(RuleType rule, const char* nextState)
    {
        return m_adder(rule, nextState);
    }
    adder& addRule(RuleType rule, int tokId, const char* nextState)
    {
        return m_adder(rule, tokId, nextState);
    }
    adder& addRule(const char* currentState, RuleType rule, const char* nextState)
    {
        return m_adder(currentState, rule, nextState);
    }
    adder& addRule(const char* currentState, RuleType rule, int tokId, const char* nextState)
    {
        return m_adder(currentState, rule, tokId, nextState);
    }
    adder& addRule(const char* currentState, RuleType rule, int tokId)
    {
        return m_adder(currentState, rule, tokId);
    }
    adder& addRule(char* v, RuleType rule, int tokId)
    {
        return m_adder(v, rule, tokId);
    }
    adder& addRule(bool* v, RuleType rule, int tokId)
    {
        return m_adder(v, rule, tokId);
    }
    adder& addRule(unsigned long* v, RuleType rule, int tokId)
    {
        return m_adder(v, rule, tokId);
    }
    adder& addRule(unsigned int* v, RuleType rule, int tokId)
    {
        return m_adder(v, rule, tokId);
    }
    adder& addRule(unsigned long long* v, RuleType rule, int tokId)
    {
        return m_adder(v, rule, tokId);
    }
    adder& addRule(long* v, RuleType rule, int tokId)
    {
        return m_adder(v, rule, tokId);
    }
    adder& addRule(int* v, RuleType rule, int tokId)
    {
        return m_adder(v, rule, tokId);
    }
    adder& addRule(long long* v, RuleType rule, int tokId)
    {
        return m_adder(v, rule, tokId);
    }
    adder& addRule(float* v, RuleType rule, int tokId)
    {
        return m_adder(v, rule, tokId);
    }
    adder& addRule(double* v, RuleType rule, int tokId)
    {
        return m_adder(v, rule, tokId);
    }
    adder& addRule(const char** v, RuleType rule, int tokId)
    {
        return m_adder(v, rule, tokId);
    }

    void skip(const char* regex);

    void addPattern(const char* name, const char* regex);

    EStatus begin(const char* input, size_t len, const char* initialState = nullptr);

    void end();

    EStatus next(Token& tok);

    EStatus getStatus() const;

protected:
    LexerPrivate* _this;
};

} // namespace spell

#include "detail/lexertl-all.hpp"

#include <functional>
#include <stdio.h>
#if PHANTOM_COMPILER != PHANTOM_COMPILER_VISUAL_STUDIO
#    include <inttypes.h>
#else
#    define SCNu64 "llu"
#    define SCNi64 "lli"
#    define SCNu32 "u"
#    define SCNi32 "i"
#endif

#if !defined(LONGLONG_MAX)
#    define LONGLONG_MAX 9223372036854775807i64
#endif

namespace spell
{
typedef wchar_t            wchar;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef unsigned long long ulonglong;
typedef long long          longlong;
struct hex
{
    explicit hex(unsigned long v) : value(v)
    {
    }
    explicit operator unsigned long long()
    {
        return value;
    }

protected:
    unsigned long value;
};

static const char* null_base_iterator_type;

struct base_iterator_type
{
    base_iterator_type() = default;
    base_iterator_type(const char*& input, const char* end) : input(input), end(end)
    {
    }

    char const& operator*() const
    {
        return *input;
    }
    char const* operator->() const
    {
        return input;
    }
    base_iterator_type& operator++()
    {
        input++;
        return *this;
    }
    base_iterator_type operator++(int)
    {
        const char* bkp = input;
        input++;
        return base_iterator_type(bkp, end);
    }

    bool operator==(base_iterator_type const& other) const
    {
        return input == other.input && end == other.end ||
        (&input == &null_base_iterator_type && other.input == other.end) ||
        (&other.input == &null_base_iterator_type && input == end);
    }
    bool operator!=(base_iterator_type const& other) const
    {
        return !operator==(other);
    }

    const char*& input = null_base_iterator_type;
    const char*  end = 0;
};

} // namespace spell

namespace std
{
template<>
struct iterator_traits<spell::base_iterator_type>
{
    typedef char        value_type;
    typedef ptrdiff_t   difference_type;
    typedef const char* pointer;
    typedef const char& reference;
};
} // namespace std

namespace spell
{
class position_iterator
{
public:
    using value_type = char;

    position_iterator() = default;
    position_iterator(const char* base) : base(base)
    {
    }

    position_iterator& operator++()
    {
        _increment();
        return *this;
    }
    void operator++(int)
    {
        _increment();
    }

    char const& operator*() const
    {
        return *base;
    }

    char const* operator->() const
    {
        return base;
    }

    bool operator==(position_iterator const& other) const
    {
        return other.base == base;
    }
    bool operator!=(position_iterator const& other) const
    {
        return !operator==(other);
    }

    bool operator<(position_iterator const& other) const
    {
        return other.base < base;
    }
    bool operator>(position_iterator const& other) const
    {
        return other.base > base;
    }

    int get_line() const
    {
        return line;
    }
    int get_col() const
    {
        return col;
    }

    ptrdiff_t operator-(position_iterator other) const
    {
        return base - other.base;
    }

    operator const char*() const
    {
        return base;
    }

private:
    void _increment()
    {
        auto ref = *base;

        switch (ref)
        {
        case '\r':
            if (prev != '\n')
            {
                ++line;
                col = 1;
            }
            break;
        case '\n':
            if (prev != '\r')
            {
                ++line;
                col = 1;
            }
            break;
        default:
            col++;
            break;
        }

        prev = ref;
        ++base;
    }

protected:
    const char* base;
    int         line = 1;
    int         col = 1;
    char        prev = 0;
};

} // namespace spell

namespace std
{
template<>
struct iterator_traits<spell::position_iterator>
{
    using value_type = char;
    using iterator_category = forward_iterator_tag;
};
} // namespace std

namespace spell
{
using position_match = lexertl::match_results<position_iterator>;

using tok_iterator = lexertl::iterator<position_iterator, lexertl::state_machine, position_match>;

struct LexerPrivate
{
    lexertl::rules                                                                             rules;
    lexertl::state_machine                                                                     sm;
    std::map<int, void (*)(LexerPrivate&, position_iterator const&, position_iterator const&)> tokToAction;

    position_iterator begin_char_it;
    position_iterator end_char_it;
    tok_iterator      begin_tok_it;
    tok_iterator      end_tok_it;
    const char*       stream;
    const char*       endstream;
    Lexer::EStatus    state;
    Token*            currentToken;
    bool              readFirstToken;
    struct Any
    {
        template<typename T>
        inline Any(T& any)
        {
        }
    };

    std::vector<char*> stringPool;
    Lexer::Allocator   allocator;

    LexerPrivate(Lexer::Allocator allocator)
        : allocator(allocator),
          state(Lexer::e_Status_Finished),
          stream(nullptr),
          currentToken(nullptr),
          readFirstToken(false)
    {
    }

    ~LexerPrivate()
    {
        for (auto str : stringPool)
        {
            allocator.dealloc(str);
        }
    }

    const char* addOrGetString(const char* cstr, size_t len)
    {
        char* copy = (char*)allocator.alloc(len + 1);
        memcpy(copy, cstr, len);
        copy[len] = 0;
        stringPool.push_back(copy);
        return copy;
    }

    static void defaultAction(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        assert(lexer.currentToken);
        lexer.currentToken->location.begin.column = begin.get_col();
        lexer.currentToken->location.begin.line = begin.get_line();
        lexer.currentToken->location.end.column = end.get_col();
        lexer.currentToken->location.end.line = end.get_line();
    }

    void error(Token::EError err)
    {
        currentToken->errors |= err;
    }

    void warning(Token::EWarning warn)
    {
        currentToken->warnings |= warn;
    }

    static void parse_bool(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        size_t l = end - begin;
        lexer.currentToken->value.ref_bool() = (l == 4) && strncmp(&*begin, "true", 4) == 0;
    }

    static void parse_hex(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        const char* text = &*begin;
        char        buffer[32];
        size_t      len = std::min(int(end - begin), (int)sizeof(buffer) - 1);
        memcpy(buffer, text, len);
        buffer[len] = 0;
        ulonglong* n = &lexer.currentToken->value.ref_ulonglong();
        sscanf(buffer + 2, "%llx", n);
    }

    static void parse_double(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        const char* text = &*begin;
        char        buffer[32];
        size_t      len = std::min(int(end - begin), (int)sizeof(buffer) - 1);
        memcpy(buffer, text, len);
        buffer[len] = 0;
        double* n = &lexer.currentToken->value.ref_double();
        if (sscanf(buffer, "%lf", n) != 1)
        {
            sscanf(buffer, "%le", n);
        }
    }
    static void parse_float(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        const char* text = &*begin;
        char        buffer[32];
        size_t      len = std::min(int(end - begin), (int)sizeof(buffer) - 1);
        memcpy(buffer, text, len);
        buffer[len] = 0;
        float* n = &lexer.currentToken->value.ref_float();
        if (sscanf(buffer, "%f", n) != 1)
        {
            sscanf(buffer, "%fe", n);
        }
    }
    static void parse_uint(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        const char* text = &*begin;
        char        buffer[32];
        size_t      len = std::min(int(end - begin), (int)sizeof(buffer) - 1);
        memcpy(buffer, text, len);
        buffer[len] = 0;
        if (text[0] == '0' && text[1] == 'x')
            sscanf(buffer + 2, "%x", &lexer.currentToken->value.ref_uint());
        else
            sscanf(buffer, "%u", &lexer.currentToken->value.ref_uint());
    }
    static void parse_ulong(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        const char* text = &*begin;
        char        buffer[32];
        size_t      len = std::min(int(end - begin), (int)sizeof(buffer) - 1);
        memcpy(buffer, text, len);
        buffer[len] = 0;
        if (text[0] == '0' && text[1] == 'x')
            sscanf(buffer + 2, "%x", &lexer.currentToken->value.ref_ulong());
        else
            sscanf(buffer, "%lu", &lexer.currentToken->value.ref_ulong());
    }
    static void parse_ulonglong(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        const char* text = &*begin;
        char        buffer[32];
        size_t      len = std::min(int(end - begin), (int)sizeof(buffer) - 1);
        memcpy(buffer, text, len);
        buffer[len] = 0;
        if (text[0] == '0' && text[1] == 'x')
            sscanf(buffer + 2, "%llx", &lexer.currentToken->value.ref_ulonglong());
        else
            sscanf(buffer, "%" SCNu64, &lexer.currentToken->value.ref_ulonglong());
    }
    static void parse_int(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        const char* text = &*begin;
        char        buffer[32];
        size_t      len = std::min(int(end - begin), (int)sizeof(buffer) - 1);
        memcpy(buffer, text, len);
        buffer[len] = 0;
        if (text[0] == '0' && text[1] == 'x')
            sscanf_s(buffer + 2, "%x", &lexer.currentToken->value.ref_int());
        else
            sscanf_s(buffer, "%i", &lexer.currentToken->value.ref_int());
    }
    static void parse_long(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        const char* text = &*begin;
        char        buffer[32];
        size_t      len = std::min(int(end - begin), (int)sizeof(buffer) - 1);
        memcpy(buffer, text, len);
        buffer[len] = 0;
        if (text[0] == '0' && text[1] == 'x')
            sscanf(buffer + 2, "%x", &lexer.currentToken->value.ref_long());
        else
            sscanf(buffer, "%li", &lexer.currentToken->value.ref_long());
    }
    static void parse_longlong(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        const char* text = &*begin;
        char        buffer[32];
        size_t      len = std::min(int(end - begin), (int)sizeof(buffer) - 1);
        memcpy(buffer, text, len);
        buffer[len] = 0;
        if (text[0] == '0' && text[1] == 'x')
            sscanf(buffer + 2, "%llx", &lexer.currentToken->value.ref_ulonglong());
        else
            sscanf(buffer, "%" SCNi64, &lexer.currentToken->value.ref_longlong());
    }
    static void parse_char(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        lexer.currentToken->value.ref_char() = *begin;
    }
    static void parse_cstring(LexerPrivate& lexer, position_iterator const& begin, position_iterator const& end)
    {
        defaultAction(lexer, begin, end);
        const char* beginc = &*begin;
        lexer.currentToken->value.ref_cstring() = lexer.addOrGetString(beginc, end - begin);
    }
};

inline Lexer::adder& Lexer::adder::operator()(RuleType rule, int tokId)
{
    return operator()("INITIAL", rule, tokId, "");
}

inline Lexer::adder& Lexer::adder::operator()(RuleType rule, int tokId, const char* nextState)
{
    return operator()("INITIAL", rule, tokId, nextState);
}

inline Lexer::adder& Lexer::adder::operator()(RuleType rule, const char* nextState)
{
    return operator()("INITIAL", rule, uint16_t(), nextState);
}

inline Lexer::adder& Lexer::adder::operator()(const char* currentState, RuleType rule, int tokId)
{
    return operator()(currentState, rule, tokId, "");
}

inline Lexer::adder& Lexer::adder::operator()(const char* currentState, RuleType rule, const char* nextState)
{
    return operator()(currentState, rule, uint16_t(), nextState);
}

#define except_if_zero_tokid                                                                                           \
    if (tokId == 0)                                                                                                    \
    {                                                                                                                  \
        assert(false);                                                                                                 \
        printf("token Id must be > 0 : 0 is reserved for internal purposes\n");                                        \
        return *this;                                                                                                  \
    }

inline Lexer::adder& Lexer::adder::operator()(const char* currentState, RuleType rule, int tokId, const char* nextState)
{
    except_if_zero_tokid;
    bool isInitialState = (*currentState == 0) || currentState == "INITIAL";
    bool hasNextState = (*nextState != 0);
    if (*currentState == 0)
        currentState = "INITIAL";

    if (*nextState == 0)
        nextState = ".";
    else
        m_lex._this->rules.push_state(nextState);

    m_lex._this->rules.push(currentState, rule, tokId, nextState);
    return *this;
}

typedef const char* cstring;

#define _defined_op(_type_)                                                                                            \
    inline Lexer::adder& Lexer::adder::operator()(_type_* result, RuleType rule, int tokId)                            \
    {                                                                                                                  \
        except_if_zero_tokid;                                                                                          \
        m_lex._this->rules.push(rule, tokId);                                                                          \
        m_lex._this->tokToAction[tokId] = &LexerPrivate::parse_##_type_;                                               \
        return *this;                                                                                                  \
    }

_defined_op(char);
_defined_op(bool);
_defined_op(ulong);
_defined_op(uint);
_defined_op(ulonglong);
_defined_op(long);
_defined_op(int);
_defined_op(longlong);
_defined_op(float);
_defined_op(double);
_defined_op(cstring);

inline Lexer::Lexer() : Lexer(Allocator())
{
}
inline Lexer::Lexer(Allocator allocator) : m_adder(*this)
{
    _this = new (allocator.alloc(sizeof(LexerPrivate))) LexerPrivate(allocator);
}

inline Lexer::~Lexer()
{
    Allocator a = _this->allocator;
    _this->~LexerPrivate();
    a.dealloc(_this);
}

inline void Lexer::skip(const char* regex)
{
    _this->rules.push(regex, position_match::skip());
}

inline void Lexer::addPattern(const char* name, const char* regex)
{
    _this->rules.insert_macro(name, regex);
}

inline Lexer::EStatus Lexer::begin(const char* input, size_t size, const char* initialState /*= 0*/)
{
    assert(_this->stream == nullptr && _this->state != e_Status_Ready);
    _this->stream = input;
    _this->endstream = input + size;
    _this->state = e_Status_Ready;
    bool result = false;
    try
    {
        base_iterator_type in_begin(_this->stream, _this->stream + size);

        lexertl::generator::build(_this->rules, _this->sm);

        _this->begin_char_it = position_iterator(input);
        _this->end_char_it = position_iterator(input + size);
        _this->begin_tok_it = tok_iterator(_this->begin_char_it, _this->end_char_it, _this->sm);
        _this->end_tok_it = tok_iterator();
    }
    catch (std::exception e)
    {
        printf("%s\n", e.what());
        return _this->state = e_Status_Error;
    }
    _this->readFirstToken = false;
    return (_this->begin_tok_it == _this->end_tok_it) ? (_this->state = e_Status_Finished)
                                                      : (_this->state = e_Status_Ready);
}

inline Lexer::EStatus Lexer::next(Token& tok)
{
    assert(_this->stream && "lexer has not began");
    _this->currentToken = &tok;
    if (_this->begin_tok_it == _this->end_tok_it)
    {
        return _this->state = e_Status_Finished;
    }
    assert(_this->state == e_Status_Ready && "lexer is not reading tokens");

    auto foundAction = _this->tokToAction.find(_this->begin_tok_it->id);
    if (foundAction != _this->tokToAction.end())
    {
        foundAction->second(*_this, _this->begin_tok_it->first, _this->begin_tok_it->second);
    }
    else
    {
        LexerPrivate::defaultAction(*_this, _this->begin_tok_it->first, _this->begin_tok_it->second);
    }
    //     if (_this->readFirstToken)
    //     {
    //         ++_this->begin_tok_it;
    //         if (_this->begin_tok_it == _this->end_tok_it)
    //         {
    //             tok = Token();
    //             _this->currentToken = nullptr;
    //             return _this->state = e_Status_Finished;
    //         }
    //     }
    //     else
    //     {
    //         _this->readFirstToken = true;
    //     }
    if (tok.errors)
    {
        _this->currentToken = nullptr;
        return _this->state = e_Status_Error;
    }

    if (_this->begin_tok_it->id == 0)
    {
        _this->currentToken = nullptr;
        if (_this->stream == _this->endstream) // eof
            return (_this->state = e_Status_Finished);
        tok = Token();
        tok.errors |= Token::e_Error_InvalidToken;
        return (_this->state = e_Status_Error);
    }
    _this->currentToken = nullptr;
    // assert(_this->currentToken->location.isValid());
    tok.id = _this->begin_tok_it->id;
    _this->begin_tok_it++;
    return (_this->state = e_Status_Ready);
}

inline spell::Lexer::EStatus Lexer::getStatus() const
{
    return _this->state;
}

inline void Lexer::end()
{
    assert(_this->stream && "lexer has not began");
    _this->stream = nullptr;
}

} // namespace spell
