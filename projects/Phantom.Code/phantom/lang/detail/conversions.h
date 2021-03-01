// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/// CONVERSION

#include <phantom/lang/conversions.h>

namespace phantom
{
namespace lang
{
struct RefConversion;
struct LValueTransformationConversion;
struct QualificationAdjustConversion;
struct StandardConversion;
struct StandardConversionSequence;
struct NumericConversion;
struct UserDefinedConversion;

struct ConversionEllipsis
{
    explicit ConversionEllipsis(bool bool_) : bool_(bool_) {}
    bool bool_;
};

struct DefaultConversionSequence : public Conversion
{
    /// STANDARD CONVERSION ONLY
    DefaultConversionSequence(LanguageElement* a_pOwner, ConversionVal io,
                              ConversionEllipsis a_bEllipsis = ConversionEllipsis(false)); /// identity or ellipsis

    DefaultConversionSequence(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o,
                              int a_iPlaceholding = 0); /// reinterpret_cast or placeholding

    DefaultConversionSequence(LanguageElement* a_pOwner, RefConversion* cr);

    DefaultConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lvt);

    DefaultConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lvt,
                              QualificationAdjustConversion* qa, RefConversion* cr = nullptr);

    DefaultConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lvt, NumericConversion* nc,
                              RefConversion* cr = nullptr);

    DefaultConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lvt, NumericConversion* nc,
                              QualificationAdjustConversion* qa, RefConversion* cr = nullptr);

    DefaultConversionSequence(LanguageElement* a_pOwner, NumericConversion* nc, QualificationAdjustConversion* qa,
                              RefConversion* cr = nullptr);

    DefaultConversionSequence(LanguageElement* a_pOwner, NumericConversion* nc, RefConversion* cr = nullptr);

    DefaultConversionSequence(LanguageElement* a_pOwner, QualificationAdjustConversion* qa,
                              RefConversion* cr = nullptr);

    DefaultConversionSequence(LanguageElement* a_pOwner, StandardConversionSequence* s1);

    /// + opt[USER DEFINED] + opt[STANDARD CONVERSION]
    DefaultConversionSequence(LanguageElement* a_pOwner, StandardConversionSequence* s1, UserDefinedConversion* ud);
    DefaultConversionSequence(LanguageElement* a_pOwner, UserDefinedConversion* ud);

    ~DefaultConversionSequence();

    StandardConversionSequence* m_standard = nullptr;
    UserDefinedConversion*      m_pre_user_defined = nullptr;
    UserDefinedConversion*      m_user_defined = nullptr;
    ConversionEllipsis          m_ellipsis = ConversionEllipsis(false);
    int                         m_placeholding = false;

    virtual void apply(const void* a_pInput, void* a_pOutput) const;

    virtual bool isCanonical() const;

    void addConstRef();
    void addRValueToLValuePromotion();
    void addRValueRef();
    void addRValuePromotion();
    void addLValueTransform(Constructor* a_pCopyCtor);
    void addUserDefinedByConstruction(Constructor* a_pConstructor);
    void addUserDefinedByConversionFunction(Method* a_pConversionFunction);

    int compare(const DefaultConversionSequence& other) const;

    virtual int compare(const Conversion& other) const { return compare((const DefaultConversionSequence&)other); }

    virtual DefaultConversionSequence* clone(LanguageElement* a_pOwner) const override;

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const override;

    void setKind(CastKind _kind);
};

struct StandardConversionSequence : public Conversion
{
    /// int -> int const ?
    StandardConversionSequence(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o);

    /// int -> int
    StandardConversionSequence(LanguageElement* a_pOwner, ConversionVal io);

    /// int -> int const&
    StandardConversionSequence(LanguageElement* a_pOwner, RefConversion* cr);

    /// int& -> int
    StandardConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lt);

    /// int*& -> int* -> int const* (-> int const* const&)
    StandardConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lt,
                               QualificationAdjustConversion* qa, RefConversion* cr = nullptr);

    /// int& -> int -> float (-> float const&) || Derived*& -> Derived* -> Base* (-> Base* const&)
    StandardConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lt, NumericConversion* nc,
                               RefConversion* cr = nullptr);

    /// Derived*& -> Derived* -> Base* -> Base const* (-> Base const* const&)
    StandardConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lt, NumericConversion* nc,
                               QualificationAdjustConversion* qa, RefConversion* cr = nullptr);

    /// Derived* -> Base* -> Base const* (-> Base const* const&)
    StandardConversionSequence(LanguageElement* a_pOwner, NumericConversion* nc, QualificationAdjustConversion* qa,
                               RefConversion* cr = nullptr);

    /// int -> float -> const float& || Derived* -> Base* -> Base* const&
    StandardConversionSequence(LanguageElement* a_pOwner, NumericConversion* nc, RefConversion* cr = nullptr);

    /// Class* -> Class const* -> Class const* const&
    StandardConversionSequence(LanguageElement* a_pOwner, QualificationAdjustConversion* qa,
                               RefConversion* cr = nullptr);

    ~StandardConversionSequence();

    bool                            m_rvalue_promotion = false;
    LValueTransformationConversion* m_lvalue_transformation = nullptr;
    NumericConversion*              m_numeric_conversion = nullptr;
    QualificationAdjustConversion*  m_qualification_adjustment = nullptr;
    RefConversion*                  m_const_ref = nullptr;

    void addLValueTransform(Constructor* a_pCopyCtor);

    void addConstRef();

    void addRValueToLValuePromotion();

    void addRValueRef();

    void addRValuePromotion();

    virtual bool isCanonical() const;

    void apply(const void* a_pInput, void* a_pOutput) const;

    int compare(const StandardConversionSequence& other) const;

    virtual StandardConversionSequence* clone(LanguageElement* a_pOwner) const override;

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const;
};

struct StandardConversion : public Conversion
{
    enum EType
    {
        e_trivial_conversion,
        e_numeric_conversion,
        e_qualification_adjustment,
    };

    StandardConversion(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o, EType t)
        : Conversion(a_pOwner, i, o), type(t)
    {
    }
    EType type;
};

struct TrivialConversion : public StandardConversion
{
    TrivialConversion(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o)
        : StandardConversion(a_pOwner, i, o, e_trivial_conversion)
    {
    }
};

struct LValueTransformationConversion : public TrivialConversion
{
    LValueTransformationConversion(LanguageElement* a_pOwner, ConversionVal ref, Constructor* a_pCopyCtor);

    virtual LValueTransformationConversion* clone(LanguageElement* a_pOwner) const override;

    virtual void        apply(const void* a_pInput, void* a_pOutput) const;
    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const;

    Constructor* copyConstructor;
};

struct RefConversion : public TrivialConversion
{
    RefConversion(LanguageElement* a_pOwner, ConversionVal io, bool rvalue = false);

    virtual void apply(const void* a_pInput, void* a_pOutput) const { *(void**)a_pOutput = (void*)a_pInput; }

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const;

    virtual RefConversion* clone(LanguageElement* a_pOwner) const override;
};

struct NumericConversion : public StandardConversion
{
    NumericConversion(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o, bool prom = false,
                      size_t ptrToPtrInheritLevel = 0)
        : StandardConversion(a_pOwner, i, o, e_numeric_conversion),
          promotion(prom),
          pointerToPointerInheritanceLevel(ptrToPtrInheritLevel)
    {
    }

    int compare(const NumericConversion& other) const;

    virtual NumericConversion* clone(LanguageElement* a_pOwner) const override;

    void apply(const void* a_pInput, void* a_pOutput) const;

    virtual Expression* convert(Semantic*, Expression* a_pExpression) const { return a_pExpression; }

    bool   promotion;
    size_t pointerToPointerInheritanceLevel;
};

struct PointerConversion : public NumericConversion
{
    PointerConversion(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o, size_t inheritanceLevel = ~size_t(0),
                      ptrdiff_t offset = 0)
        : NumericConversion(a_pOwner, i, o, false, inheritanceLevel), offsetToBase(offset)
    {
    }
    virtual void apply(const void* a_pInput, void* a_pOutput) const
    {
        byte* inputValue = *(byte* const*)a_pInput;
        if (inputValue == 0)
            *(byte**)a_pOutput = 0;
        *(byte**)a_pOutput = *(byte* const*)a_pInput + offsetToBase;
    }

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const;

    virtual bool isCanonical() const { return offsetToBase == 0; }

    virtual PointerConversion* clone(LanguageElement* a_pOwner) const override
    {
        return a_pOwner->new_<PointerConversion>(a_pOwner, input, output, pointerToPointerInheritanceLevel,
                                                 offsetToBase);
    }

    ptrdiff_t offsetToBase;
};

struct FieldPointerConversion : public NumericConversion
{
    FieldPointerConversion(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o,
                           size_t inheritanceLevel = ~size_t(0), ptrdiff_t offset = 0)
        : NumericConversion(a_pOwner, i, o, false, inheritanceLevel), offsetToBase(offset)
    {
    }

    virtual void apply(const void* a_pInput, void* a_pOutput) const
    {
        size_t inputValue = *(const size_t*)a_pInput;
        if (inputValue == ~size_t(0))
            *(size_t*)a_pOutput = ~size_t(0);
        *(size_t*)a_pOutput = *(const size_t*)a_pInput + offsetToBase;
    }

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const;

    virtual FieldPointerConversion* clone(LanguageElement* a_pOwner) const override
    {
        return a_pOwner->new_<FieldPointerConversion>(a_pOwner, input, output, pointerToPointerInheritanceLevel,
                                                      offsetToBase);
    }

    ptrdiff_t offsetToBase;
};

struct MethodPointerConversion : public NumericConversion
{
    MethodPointerConversion(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o,
                            size_t inheritanceLevel = ~size_t(0), ptrdiff_t offset = 0)
        : NumericConversion(a_pOwner, i, o, false, inheritanceLevel), offsetToBase(offset)
    {
    }
    virtual void apply(const void* a_pInput, void* a_pOutput) const;
    ptrdiff_t    offsetToBase;

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const;

    virtual MethodPointerConversion* clone(LanguageElement* a_pOwner) const override
    {
        return a_pOwner->new_<MethodPointerConversion>(a_pOwner, input, output, pointerToPointerInheritanceLevel,
                                                       offsetToBase);
    }
};

struct QualificationAdjustConversion : public StandardConversion
{
    QualificationAdjustConversion(LanguageElement* a_pOwner, ConversionVal src, ConversionVal dest, int count)
        : StandardConversion(a_pOwner, src, dest, e_qualification_adjustment), adjustmentCount(count)
    {
    }
    int                                    adjustmentCount;
    virtual int                            compare(const Conversion& a_Other) const;
    virtual void                           apply(const void*, void*) const {}
    virtual QualificationAdjustConversion* clone(LanguageElement* a_pOwner) const override
    {
        return a_pOwner->new_<QualificationAdjustConversion>(a_pOwner, input, output, adjustmentCount);
    }
    virtual Expression* convert(Semantic*, Expression* a_pExpression) const { return a_pExpression; }
};

struct UserDefinedConversion : public Conversion
{
    UserDefinedConversion(LanguageElement* a_pOwner, StandardConversionSequence* standard);
    UserDefinedConversion(LanguageElement* a_pOwner, Constructor* a_pConstructor);
    UserDefinedConversion(LanguageElement* a_pOwner, Method* memberFunction);
    UserDefinedConversion(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o, Method* memberFunction,
                          StandardConversionSequence* a_standard)
        : Conversion(a_pOwner, i, o), m_member_function(memberFunction), m_standard(a_standard)
    {
    }
    ~UserDefinedConversion()
    {
        if (m_standard && m_standard->semantic == nullptr)
            m_standard->delete_();
    }
    Method*                     m_member_function = nullptr;
    StandardConversionSequence* m_standard = nullptr;

    void apply(const void* a_pInput, void* a_pOutput) const override;

    Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const override;

    int                            compare(const UserDefinedConversion& other) const;
    virtual UserDefinedConversion* clone(LanguageElement* a_pOwner) const override
    {
        return a_pOwner->new_<UserDefinedConversion>(a_pOwner, input, output, m_member_function, m_standard);
    }
};

inline StandardConversionSequence::~StandardConversionSequence()
{
    if (m_lvalue_transformation && m_lvalue_transformation->semantic == nullptr)
        m_lvalue_transformation->delete_();
    if (m_numeric_conversion && m_numeric_conversion->semantic == nullptr)
        m_numeric_conversion->delete_();
    if (m_qualification_adjustment && m_qualification_adjustment->semantic == nullptr)
        m_qualification_adjustment->delete_();
    if (m_const_ref && m_const_ref->semantic == nullptr)
        m_const_ref->delete_();
}

inline DefaultConversionSequence::~DefaultConversionSequence()
{
    if (m_standard && m_standard->semantic == nullptr)
        m_standard->delete_();
    if (m_user_defined && m_user_defined->semantic == nullptr)
        m_user_defined->delete_();
}

} // namespace lang
} // namespace phantom
