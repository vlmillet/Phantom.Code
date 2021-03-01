// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <algorithm>
#include <phantom/lang/Source.h>
#include <phantom/lang/lang.h>
#include <phantom/utils/SmallVector.h>

namespace phantom
{
namespace lang
{
/// @cond ADVANCED

struct ConversionVal
{
    ConversionVal() : type(0), category(0) {}
    ConversionVal(Type* a_pType) : type(a_pType), category(0) {}
    ConversionVal(Type* a_pType, int cat) : type(a_pType), category(cat) {}
    Type* type;
    int   category;
    Type* operator->() const { return type; }
          operator Type*() const { return type; }
};

template<typename t_Type>
struct ConversionValT : public ConversionVal
{
    t_Type* operator->() const { return static_cast<t_Type*>(type); }
            operator t_Type*() { static_cast<t_Type*>(type); }
};

/// @endcond

/// \brief  Represents a type conversion.
struct Conversion
{
    Conversion(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o) : m_pOwner(a_pOwner), input(i), output(o) {}
    virtual ~Conversion() { PHANTOM_ASSERT(semantic == nullptr); }
    virtual void        apply(const void* a_pInput, void* a_pOutput) const = 0;
    virtual Conversion* clone(LanguageElement* a_pOwner) const = 0;

    virtual int compare(const Conversion& a_Other) const;

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const = 0;

    bool isIdentity() const;

    virtual bool isCanonical() const;

    void delete_() { m_pOwner->deleteVirtual(this); }

    ConversionVal input;
    ConversionVal output;

    Semantic* semantic = nullptr;

    CastKind kind = CastKind::Implicit;

    LanguageElement* m_pOwner{};
};
/// \brief  Represents a template dependant conversion.
struct TemplateDependantConversion : public Conversion
{
    TemplateDependantConversion(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o);

    virtual void apply(const void*, void*) const
    {
        PHANTOM_ASSERT(false, "template dependant conversion cannot be applied");
    }
    virtual Conversion* clone(LanguageElement* a_pOwner) const override;

    virtual int compare(const Conversion&) const
    {
        PHANTOM_ASSERT_NO_IMPL();
        return 0;
    }

    virtual Expression* convert(Semantic*, Expression*) const
    {
        PHANTOM_ASSERT(false, "template dependant conversion cannot be used");
        return nullptr;
    }

    virtual bool isCanonical() const { return true; }
};

/// \brief  Represent a group of conversions (for arguments list conversions essentially).
class ConversionResults
{
public:
    typedef SmallVector<Conversion*> ConversionList;
    ConversionResults() {}
    ConversionResults(ConversionResults&& rvalue) : content(std::move(rvalue.content)) {}
    ConversionResults(const ConversionResults& rvalue) : content(rvalue.content) {}
    ConversionResults& operator=(ConversionResults&& rvalue)
    {
        content = std::move(rvalue.content);
        return *this;
    }
    ConversionResults& operator=(const ConversionResults& rvalue)
    {
        content = rvalue.content;
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Compares this conversion list with another list.
    ///
    /// \param  other   Another instance to compare.
    ///
    /// \return -1 if 'other' is worth than '', 0 if they are equivalent, or +1 if it is
    ///         better.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    int compare(const ConversionResults& other) const;

    void push_back(Conversion* conv) { content.push_back(conv); }

    bool hasNull() const { return std::find(content.begin(), content.end(), nullptr) != content.end(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Extract best conversion among the list (removes it from the list).
    ///
    /// \param [in,out] a_pIndex    The index buffer (where was found the best conversion).
    ///
    /// \return null if it fails, else.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Conversion* takeBest(size_t* a_pIndex);

    Conversion* const& operator[](size_t index) const { return content[index]; }

    size_t size() const { return content.size(); }

    ConversionList::const_iterator begin() const { return content.begin(); }
    ConversionList::const_iterator end() const { return content.end(); }

    void destroy(Source* a_pSource)
    {
        for (auto* conv : content)
        {
            if (conv && conv->semantic == nullptr)
                a_pSource->deleteVirtual(conv);
        }
    }

protected:
    ConversionList content;
};
} // namespace lang
} // namespace phantom
