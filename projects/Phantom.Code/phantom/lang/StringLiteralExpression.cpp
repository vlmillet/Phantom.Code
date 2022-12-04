// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
// #include "phantom/core.h"
#include "StringLiteralExpression.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
size_t StringLiteralExpression_readChar(char* str, const char* cstr, size_t& offset)
{
    if (cstr[offset++] == '\\')
    {
        switch (cstr[offset++])
        {
        case 'x':
        {
            char ch;
            if (sscanf(&cstr[2], "%2x", reinterpret_cast<unsigned int*>(&ch)) != 1)
                return 0;
            offset += 2;
            str[0] = char(ch);
            return 1;
        }

        case 'u':
        {
            char ch[2];
            if (sscanf(&cstr[2], "%4x", reinterpret_cast<unsigned int*>(&ch)) != 1)
                return 0;
            offset += 4;
            str[0] = ch[0];
            str[1] = ch[1];
        }
        break;

        case '0':
            str[0] = '\0';
            break;

        case 'n':
            str[0] = '\n';
            break;

        case 't':
            str[0] = '\t';
            break;

        case 'r':
            str[0] = '\r';
            break;

        case 'b':
            str[0] = '\b';
            break;

        case 'a':
            str[0] = '\a';
            break;

        case 'v':
            str[0] = '\v';
            break;

        case 'f':
            str[0] = '\f';
            break;

        case '\\':
            str[0] = '\\';
            break;

        default:
            str[0] = cstr[1];
            break;
        }
    }
    else
    {
        str[0] = cstr[0];
    }
    return 1;
}

StringLiteralExpression::StringLiteralExpression(StringView a_Literal, StringView a_Value)
    : Expression(PHANTOM_TYPEOF(const char*)), m_Value(a_Value), m_Literal(a_Literal)
{
    m_Buffer = m_Value.c_str();
}

bool StringLiteralExpression::LiteralToString(String& _inOut)
{
    const char* cstr = _inOut.c_str();
    size_t      len = _inOut.size();
    char*       str = (char*)cstr + 1;
    size_t      size = 0;
    for (size_t i = 0; i < (len - 2);)
    {
        size_t offset = 0;
        size_t count = StringLiteralExpression_readChar(str + size, cstr + i + 1, offset);
        if (count == 0)
        {
            PHANTOM_LOG(Error, "invalid String formatting");
            return false;
        }
        i += offset;
        size += count;
    }
    _inOut.erase(_inOut.begin());
    _inOut.resize(size);
    return true;
}

StringLiteralExpression::StringLiteralExpression(StringView a_Literal)
    : Expression(PHANTOM_TYPEOF(const char*)), m_Value(a_Literal), m_Literal(a_Literal)
{
    PHANTOM_ASSERT(a_Literal.size() && a_Literal.front() == '"' && a_Literal.back() == '"');
    LiteralToString(m_Value);
    m_Buffer = m_Value.c_str();
}

void* StringLiteralExpression::evalStorage(ExecutionContext&) const
{
    return (void*)&m_Buffer;
}

void StringLiteralExpression::eval(ExecutionContext& a_Context) const
{
    *((const char**)a_Context.resultPointer()) = m_Buffer;
}

StringLiteralExpression* StringLiteralExpression::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<StringLiteralExpression>(m_Literal, m_Value);
}

} // namespace lang
} // namespace phantom
