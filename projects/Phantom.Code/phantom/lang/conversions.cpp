// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "conversions.h"

namespace phantom
{
namespace lang
{
Conversion* ConversionResults::takeBest(size_t* a_pIndex)
{
    if (content.size() == 1)
    {
        Conversion* conv = content.back();
        content.clear();
        return conv;
    }
    for (size_t i = 0; i < content.size(); ++i)
    {
        int    bestResult = -1;
        size_t j = 0;
        for (; j < content.size(); ++j)
        {
            if (i == j)
                continue;
            int result = content[i]->compare(*content[j]);
            PHANTOM_ASSERT(content[j]->compare(*content[i]) == -result);
            if (result >= 0)
            {
                bestResult = std::max(result, bestResult);
            }
            else
                break;
        }
        if (j == content.size())
        {
            if (bestResult == 1)
            {
                Conversion* conv = content[i];
                if (a_pIndex)
                    *a_pIndex = i;
                content.erase(content.begin() + i);
                return conv;
            }
        }
    }
    return nullptr;
}

TemplateDependantConversion::TemplateDependantConversion(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o)
    : Conversion(a_pOwner, i, o)
{
}

Conversion* TemplateDependantConversion::clone(LanguageElement* a_pOwner) const
{
    return a_pOwner->new_<TemplateDependantConversion>(a_pOwner, input, output);
}

int Conversion::compare(const Conversion&) const
{
    PHANTOM_ASSERT_NO_IMPL();
    return 0;
}

} // namespace lang
} // namespace phantom
