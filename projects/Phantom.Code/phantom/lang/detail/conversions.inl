// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma warning(disable:4312)

Expression* StandardConversionSequence::convert(Semantic* a_pSemantic, Expression* a_pExpression) const
{
    Expression* pInput = a_pExpression;
    if (m_rvalue_promotion)
    {
        a_pExpression = a_pSemantic->New<RValueReferenceExpression>(a_pExpression);
    }
    if (m_lvalue_transformation && m_const_ref  && !m_numeric_conversion)
    {
        if (a_pExpression->getValueType()->getTypeKind() == TypeKind::RValueReference)
        {
            if (m_const_ref->output->getTypeKind() == TypeKind::RValueReference)
            {
                // lvalue transformation (X&& -> X) and rvalue ref (X-> X&&) cancel each others  (at one const of
                return a_pExpression; // identity
            }
            return a_pSemantic->New<RValueToConstLValueExpression>(a_pExpression);
        }
        // lvalue transformation (X& -> X) and const ref (X-> Xconst&) cancel each others  (at one const of difference which is not important for evaluation)
        return a_pExpression;
    }
    if (m_lvalue_transformation)
    {
        a_pExpression = m_lvalue_transformation->convert(a_pSemantic, a_pExpression);
    }
    if (m_numeric_conversion)
    {
        a_pExpression = m_numeric_conversion->convert(a_pSemantic, a_pExpression);
    }
    if (m_const_ref)
    {
        a_pExpression = m_const_ref->convert(a_pSemantic, a_pExpression);
    }
    if (pInput == a_pExpression)
    {
        if (kind != phantom::lang::CastKind::Implicit)
        {
            auto pId = a_pSemantic->New<IdentityExpression>(output.type, a_pExpression);
            pId->setConversionKind(kind);
            return pId;
        }
    }
    return a_pExpression;
}

void DefaultConversionSequence::addUserDefinedByConstruction(Constructor* a_pConstructor)
{
    PHANTOM_ASSERT(semantic == nullptr);
    m_pre_user_defined = m_user_defined;
    m_user_defined = m_pOwner->new_<UserDefinedConversion>(m_pOwner, a_pConstructor);
    output = m_user_defined->output;
}
void DefaultConversionSequence::addUserDefinedByConversionFunction(Method* a_pConversionFunction)
{
	PHANTOM_ASSERT(semantic == nullptr);
	m_user_defined = m_pOwner->new_<UserDefinedConversion>(m_pOwner, a_pConversionFunction);
    input = m_user_defined->input;
}

void MethodPointerConversion::apply(const void* a_pInput, void* a_pOutput) const
{
#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO /// /vmg + /vmv must be defined
    struct __MicrosoftUnknownMFP {
        void* m_func_address; // 64 bits for Itanium.
        ptrdiff_t m_delta;
        int m_vtordisp;
        int m_vtable_index; // or 0 if no virtual inheritance
    };
    __MicrosoftUnknownMFP& _output_ = *(__MicrosoftUnknownMFP*)a_pOutput;
    __MicrosoftUnknownMFP& _input_ = *(__MicrosoftUnknownMFP*)a_pInput;
    _output_.m_delta = _input_.m_delta + offsetToBase;
    PHANTOM_ASSERT(_input_.m_vtable_index == 0 && _output_.m_vtable_index == 0, "phantom does not support virtual inheritance");


    //         if (vindex=0) adjustedthis = this + delta
    //         else adjustedthis = this + delta + vtordisp + *(*(this + vtordisp) + vindex)
    //         CALL funcadr
#elif PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
    // GNU g++ uses a tricky space optimisation, also adopted by IBM's VisualAge and XLC.
    struct __ClangMFP {
        union {
            void* m_func_address_even; // always even
            ptrdiff_t m_vtable_index_odd; //  = vindex*2+1, always odd
        };
        ptrdiff_t m_delta;
    };
    //         adjustedthis = this + delta
    //             if (funcadr & 1) CALL (* ( *delta + (vindex+1)/2) + 4)
    //             else CALL funcadr
    __ClangMFP& output = *(__ClangMFP*)a_pOutput;
    __ClangMFP& input = *(__ClangMFP*)a_pInput;
    output.m_delta = input.m_delta + offsetToBase;
#else
#   error define Member Function Pointer struct for the current compiler
#endif
}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, ConversionVal io, ConversionEllipsis a_bEllipsis /*= false*/)
    : Conversion(a_pOwner, io, a_bEllipsis.bool_ ? nullptr : io)
    , m_standard(a_bEllipsis.bool_ ? 0 : a_pOwner->new_<StandardConversionSequence>(a_pOwner, io)), m_user_defined(0), m_ellipsis(a_bEllipsis)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o, int a_iPlaceholding /*= 0*/)
    : Conversion(a_pOwner, i, o)
    , m_ellipsis(false)
    , m_placeholding(a_iPlaceholding)
    , m_standard(a_pOwner->new_<StandardConversionSequence>(a_pOwner, i, o))
    , m_user_defined(0)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, RefConversion* cr)
    : Conversion(a_pOwner, cr->input, cr->output), m_standard(a_pOwner->new_<StandardConversionSequence>(a_pOwner, cr)), m_user_defined(nullptr), m_ellipsis(false), m_placeholding(0)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lvt)
    : Conversion(a_pOwner, lvt->input, lvt->output), m_standard(a_pOwner->new_<StandardConversionSequence>(a_pOwner, lvt)), m_user_defined(nullptr), m_ellipsis(false), m_placeholding(0)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lvt, QualificationAdjustConversion* qa, RefConversion* cr /*= 0*/)
    : Conversion(a_pOwner, lvt->input, cr ? cr->output : qa->output), m_standard(a_pOwner->new_<StandardConversionSequence>(a_pOwner, lvt, qa, cr)), m_user_defined(nullptr), m_ellipsis(false), m_placeholding(0)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lvt, NumericConversion* nc, RefConversion* cr /*= 0*/)
    : Conversion(a_pOwner, lvt->input, cr ? cr->output : nc->output), m_standard(a_pOwner->new_<StandardConversionSequence>(a_pOwner, lvt, nc, cr)), m_user_defined(nullptr), m_ellipsis(false), m_placeholding(0)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lvt, NumericConversion* nc, QualificationAdjustConversion* qa, RefConversion* cr /*= 0*/)
    : Conversion(a_pOwner, lvt->input, cr ? cr->output : qa->output), m_standard(a_pOwner->new_<StandardConversionSequence>(a_pOwner, lvt, nc, qa, cr)), m_user_defined(nullptr), m_ellipsis(false), m_placeholding(0)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, NumericConversion* nc, QualificationAdjustConversion* qa, RefConversion* cr /*= 0*/)
    : Conversion(a_pOwner, nc->input, cr ? cr->output : qa->output), m_standard(a_pOwner->new_<StandardConversionSequence>(a_pOwner, nc, qa, cr)), m_user_defined(nullptr), m_ellipsis(false), m_placeholding(0)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, NumericConversion* nc, RefConversion* cr /*= 0*/)
    : Conversion(a_pOwner, nc->input, cr ? cr->output : nc->output), m_standard(a_pOwner->new_<StandardConversionSequence>(a_pOwner, nc, cr)), m_user_defined(nullptr), m_ellipsis(false), m_placeholding(0)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, QualificationAdjustConversion* qa, RefConversion* cr /*= 0*/)
    : Conversion(a_pOwner, qa->input, cr ? cr->output : qa->output), m_standard(a_pOwner->new_<StandardConversionSequence>(a_pOwner, qa, cr)), m_user_defined(nullptr), m_ellipsis(false), m_placeholding(0)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, StandardConversionSequence* s1)
    : Conversion(a_pOwner, s1->input, s1->output), m_standard(s1), m_user_defined(nullptr), m_ellipsis(false), m_placeholding(0)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, StandardConversionSequence* s1, UserDefinedConversion* ud)
    : Conversion(a_pOwner, s1->input, ud->output), m_standard(s1), m_user_defined(ud), m_ellipsis(false), m_placeholding(0)
{

}

DefaultConversionSequence::DefaultConversionSequence(LanguageElement* a_pOwner, UserDefinedConversion* ud)
    : Conversion(a_pOwner, ud->input, ud->output), m_standard(0), m_user_defined(ud), m_ellipsis(false), m_placeholding(0)
{

}

LValueTransformationConversion::LValueTransformationConversion(LanguageElement* a_pOwner, ConversionVal ref, Constructor* a_pCtor) 
    : TrivialConversion(a_pOwner, ref, ref->removeReference()), copyConstructor(a_pCtor)
{

}

void LValueTransformationConversion::apply(const void* a_pInput, void* a_pOutput) const
{
    output->copy(*(void**)a_pInput, a_pOutput);
}

Expression* LValueTransformationConversion::convert(Semantic* a_pSemantic, Expression* a_pExpression) const
{
    if (copyConstructor)
    {
        auto pC = a_pSemantic->New<ConstructorCallExpression>(copyConstructor, a_pExpression);
        pC->setImplicit();
        return pC;
    }
    return a_pSemantic->New<LoadExpression>(a_pExpression);
}
RefConversion::RefConversion(LanguageElement* a_pOwner, ConversionVal io, bool rvalue)
    : TrivialConversion(a_pOwner, io, rvalue ? static_cast<Type*>(io->removeReference()->addRValueReference()) : io->makeConst()->makeLValueReference())
{
}

Expression* RefConversion::convert(Semantic* a_pSemantic, Expression* a_pExpression) const
{
    if (output->asRValueReference())
    {
        return a_pSemantic->New<RValueReferenceExpression>(a_pExpression);
    }
    else
    {
        PHANTOM_ASSERT(output->asLValueReference());
        return a_pSemantic->New<RValueToConstLValueExpression>(a_pExpression);
    }
}


template<typename t_Input, typename t_Output, CastKind t_iConversionType>
struct NumericConversionT;

template<typename t_Input, typename t_Output>
struct NumericConversionT<t_Input, t_Output, CastKind::Implicit> : public NumericConversion
{
    NumericConversionT(LanguageElement* a_pOwner, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(t_Input), PHANTOM_TYPEOF(t_Output), promotion) {}
    NumericConversionT(LanguageElement* a_pOwner, Type* a_pInput, Type* a_pOutput, bool promotion = false) : NumericConversion(a_pOwner, a_pInput, a_pOutput, promotion) {}
    virtual void apply(const void* a_pInput, void* a_pOutput) const
    {
        *(t_Output*)a_pOutput = *(const t_Input*)a_pInput;
    }

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const
    {
        return a_pSemantic->New<BuiltInConversionExpression>(a_pExpression, clone(a_pSemantic->getSource()), CastKind::Implicit);
    }

    virtual NumericConversionT<t_Input, t_Output, CastKind::Implicit>* clone(LanguageElement* a_pOwner) const
    { 
        return a_pOwner->new_<NumericConversionT<t_Input, t_Output, CastKind::Implicit>>(a_pOwner, input, output, promotion); 
    }
};

template<typename t_Input, typename t_Output>
struct NumericConversionT<t_Input, t_Output, CastKind::Reinterpret> : public NumericConversion
{
    NumericConversionT(LanguageElement* a_pOwner, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(t_Input), PHANTOM_TYPEOF(t_Output), promotion) {}
    NumericConversionT(LanguageElement* a_pOwner, Type* a_pInput, Type* a_pOutput, bool promotion = false) : NumericConversion(a_pOwner, a_pInput, a_pOutput, promotion) {}
    virtual void apply(const void* a_pInput, void* a_pOutput) const
    {
        *(t_Output*)a_pOutput = reinterpret_cast<t_Output>(*(t_Input*)a_pInput);
    }

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const
    {
        return a_pSemantic->New<BuiltInConversionExpression>(a_pExpression, clone(a_pSemantic->getSource()), CastKind::Reinterpret);
    }

    virtual NumericConversionT<t_Input, t_Output, CastKind::Reinterpret>* clone(LanguageElement* a_pOwner) const { return a_pOwner->new_<NumericConversionT<t_Input, t_Output, CastKind::Reinterpret>>(a_pOwner, input, output, promotion); }
};

template<typename t_InOut>
struct NumericConversionT<t_InOut, t_InOut, CastKind::Reinterpret> : public NumericConversion
{
    NumericConversionT(LanguageElement* a_pOwner, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(t_InOut), PHANTOM_TYPEOF(t_InOut), promotion) {}
    NumericConversionT(LanguageElement* a_pOwner, Type* a_pInput, Type* a_pOutput, bool promotion = false) : NumericConversion(a_pOwner, a_pInput, a_pOutput, promotion) {}
    virtual void apply(const void* a_pInput, void* a_pOutput) const
    {
        *(t_InOut*)a_pOutput = *(t_InOut*)a_pInput;
    }

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const
    {
        return a_pSemantic->New<BuiltInConversionExpression>(a_pExpression, clone(a_pSemantic->getSource()), CastKind::Reinterpret);
    }

    virtual NumericConversionT<t_InOut, t_InOut, CastKind::Reinterpret>* clone(LanguageElement* a_pOwner) const { return a_pOwner->new_<NumericConversionT<t_InOut, t_InOut, CastKind::Reinterpret>>(a_pOwner, input, output, promotion); }
};

template<typename t_Input, typename t_Output>
struct NumericConversionT<t_Input, t_Output, CastKind::Static> : public NumericConversion
{
    NumericConversionT(LanguageElement* a_pOwner, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(t_Input), PHANTOM_TYPEOF(t_Output), promotion) {}
    NumericConversionT(LanguageElement* a_pOwner, Type* a_pInput, Type* a_pOutput, bool promotion = false) : NumericConversion(a_pOwner, a_pInput, a_pOutput, promotion) {}
    virtual void apply(const void* a_pInput, void* a_pOutput) const
    {
        *(t_Output*)a_pOutput = static_cast<t_Output>(*(t_Input*)a_pInput);
    }

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const
    {
        return a_pSemantic->New<BuiltInConversionExpression>(a_pExpression, clone(a_pSemantic->getSource()), CastKind::Static);
    }

    virtual NumericConversionT<t_Input, t_Output, CastKind::Static>* clone(LanguageElement* a_pOwner) const { return a_pOwner->new_ < NumericConversionT<t_Input, t_Output, CastKind::Static>>(a_pOwner, input, output, promotion); }
};


template<typename t_Input, typename t_Output>
struct NumericConversionT<t_Input, t_Output, CastKind::Explicit> : public NumericConversion
{
    NumericConversionT(LanguageElement* a_pOwner, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(t_Input), PHANTOM_TYPEOF(t_Output), promotion) {}
    NumericConversionT(LanguageElement* a_pOwner, Type* a_pInput, Type* a_pOutput, bool promotion = false) : NumericConversion(a_pOwner, a_pInput, a_pOutput, promotion) {}
    virtual void apply(const void* a_pInput, void* a_pOutput) const
    {
        *(t_Output*)a_pOutput = (t_Output)(*(t_Input*)a_pInput);
    }

    virtual Expression* convert(Semantic* a_pSemantic, Expression* a_pExpression) const
    {
        return a_pSemantic->New<BuiltInConversionExpression>(a_pExpression, clone(a_pSemantic->getSource()), CastKind::Explicit);
    }

    virtual NumericConversionT<t_Input, t_Output, CastKind::Explicit>* clone(LanguageElement* a_pOwner) const { return a_pOwner->new_<NumericConversionT<t_Input, t_Output, CastKind::Explicit>>(a_pOwner, input, output, promotion); }
};

template<typename t_Output>
struct NumericConversionT<std::nullptr_t, t_Output, CastKind::Implicit> : public NumericConversion
{
    NumericConversionT(LanguageElement* a_pOwner, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), PHANTOM_TYPEOF(t_Output), promotion) {}
    NumericConversionT(LanguageElement* a_pOwner, Type* a_pOutput, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), a_pOutput, promotion) {}
    virtual void apply(const void* a_pInput, void* a_pOutput) const
    {
        *(t_Output*)a_pOutput = nullptr;
    }

    virtual NumericConversionT<std::nullptr_t, t_Output, CastKind::Implicit>* clone(LanguageElement* a_pOwner) const { return a_pOwner->new_ < NumericConversionT<std::nullptr_t, t_Output, CastKind::Implicit>>(a_pOwner, output, promotion); }
};

template<typename t_Output>
struct NumericConversionT<std::nullptr_t, t_Output, CastKind::Static> : public NumericConversion
{
    NumericConversionT(LanguageElement* a_pOwner, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), PHANTOM_TYPEOF(t_Output), promotion) {}
    NumericConversionT(LanguageElement* a_pOwner, Type* a_pOutput, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), a_pOutput, promotion) {}
    virtual void apply(const void* a_pInput, void* a_pOutput) const
    {
        *(t_Output*)a_pOutput = nullptr;
    }

    virtual NumericConversionT<std::nullptr_t, t_Output, CastKind::Static>* clone(LanguageElement* a_pOwner) const { return a_pOwner->new_ < NumericConversionT<std::nullptr_t, t_Output, CastKind::Static>>(a_pOwner, output, promotion); }
};

template<typename t_Output>
struct NumericConversionT<std::nullptr_t, t_Output, CastKind::Explicit> : public NumericConversion
{
    NumericConversionT(LanguageElement* a_pOwner, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), PHANTOM_TYPEOF(t_Output), promotion) {}
    NumericConversionT(LanguageElement* a_pOwner, Type* a_pOutput, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), a_pOutput, promotion) {}
    virtual void apply(const void* a_pInput, void* a_pOutput) const
    {
        *(t_Output*)a_pOutput = nullptr;
    }

    virtual NumericConversionT<std::nullptr_t, t_Output, CastKind::Explicit>* clone(LanguageElement* a_pOwner) const { return a_pOwner->new_ < NumericConversionT<std::nullptr_t, t_Output, CastKind::Explicit>>(a_pOwner, output, promotion); }
};

template<>
struct NumericConversionT<std::nullptr_t, bool, CastKind::Explicit> : public NumericConversion
{
    NumericConversionT(LanguageElement* a_pOwner, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), PHANTOM_TYPEOF(bool), promotion) {}
    NumericConversionT(LanguageElement* a_pOwner, Type* a_pOutput, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), a_pOutput, promotion) {}
    virtual void apply(const void* , void* a_pOutput) const
    {
        *(bool*)a_pOutput = 0;
    }

    virtual NumericConversionT<std::nullptr_t, bool, CastKind::Explicit>* clone(LanguageElement* a_pOwner) const { return a_pOwner->new_ < NumericConversionT<std::nullptr_t, bool, CastKind::Explicit>>(a_pOwner, output, promotion); }
};

template<typename t_Output>
struct NumericConversionT<std::nullptr_t, t_Output, CastKind::Reinterpret> : public NumericConversion
{
    NumericConversionT(LanguageElement* a_pOwner, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), PHANTOM_TYPEOF(t_Output), promotion) {}
    NumericConversionT(LanguageElement* a_pOwner, Type* a_pOutput, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), a_pOutput, promotion) {}
    virtual void apply(const void* , void* a_pOutput) const
    {
        *(t_Output*)a_pOutput = nullptr;
    }

    virtual NumericConversionT<std::nullptr_t, t_Output, CastKind::Reinterpret>* clone(LanguageElement* a_pOwner) const { return a_pOwner->new_ < NumericConversionT<std::nullptr_t, t_Output, CastKind::Reinterpret>>(a_pOwner, output, promotion); }
};

template<>
struct NumericConversionT<std::nullptr_t, bool, CastKind::Reinterpret> : public NumericConversion
{
    NumericConversionT(LanguageElement* a_pOwner, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), PHANTOM_TYPEOF(bool), promotion) {}
    NumericConversionT(LanguageElement* a_pOwner, Type* a_pOutput, bool promotion = false) : NumericConversion(a_pOwner, PHANTOM_TYPEOF(std::nullptr_t), a_pOutput, promotion) {}
    virtual void apply(const void* , void* a_pOutput) const
    {
        *(bool*)a_pOutput = false;
    }

    virtual NumericConversionT<std::nullptr_t, bool, CastKind::Reinterpret>* clone(LanguageElement* a_pOwner) const { return a_pOwner->new_ < NumericConversionT<std::nullptr_t, bool, CastKind::Reinterpret>>(a_pOwner, output, promotion); }
};

int NumericConversion::compare(const NumericConversion& other) const
{
    if (promotion && !other.promotion) return 1;
    if (!promotion && other.promotion) return -1;

    ///  Conversion that involves pointer-to-bool, pointer-to-member to bool, or std::nullptr_t to bool Conversion is worse than the one that doesn't
    bool ptobool = (output == BuiltInTypes::TYPE_BOOL) &&(input == BuiltInTypes::TYPE_NULLPTR_T || input->getTypeKind() == TypeKind::FieldPointer || input->getTypeKind() == TypeKind::MethodPointer || input->getTypeKind() == TypeKind::Pointer);
    if (other.output == BuiltInTypes::TYPE_BOOL &&(other.input == BuiltInTypes::TYPE_NULLPTR_T || other.input->getTypeKind() == TypeKind::FieldPointer || other.input->getTypeKind() == TypeKind::MethodPointer || other.input->getTypeKind() == TypeKind::Pointer))
    {
        if (!ptobool) return 1;
    }
    else
    {
        if (ptobool) return -1;
    }
    if (pointerToPointerInheritanceLevel != ~size_t(0))
    {
        if (other.pointerToPointerInheritanceLevel != ~size_t(0))
        {
            return (pointerToPointerInheritanceLevel < other.pointerToPointerInheritanceLevel) - (pointerToPointerInheritanceLevel > other.pointerToPointerInheritanceLevel);
        }
        /// Conversion that converts pointer-to-derived to pointer-to-base is better than the Conversion of pointer-to-derived to pointer-to-void, and Conversion of pointer-to-base to void is better than pointer-to-derived to void.
        return 1;
    }
    else
    {
        /// Conversion that converts pointer-to-derived to pointer-to-base is better than the Conversion of pointer-to-derived to pointer-to-void, and Conversion of pointer-to-base to void is better than pointer-to-derived to void.
        if (other.pointerToPointerInheritanceLevel != ~size_t(0))
        {
            return -1;
        }
    }
    return 0;
}

void NumericConversion::apply(const void* a_pInput, void* a_pOutput) const
{
    memcpy(a_pOutput, a_pInput, std::min(input->getSize(), output->getSize()));
}

UserDefinedConversion::UserDefinedConversion(LanguageElement* a_pOwner, StandardConversionSequence* standard)
	: Conversion(a_pOwner, standard->input, standard->output)
, m_member_function(nullptr)
, m_standard(standard)
{

}

UserDefinedConversion::UserDefinedConversion(LanguageElement* a_pOwner, Constructor* a_pConstructor) 
    : Conversion(a_pOwner, a_pConstructor->getParameterType(0), a_pConstructor->getOwnerClassType())
, m_member_function(a_pConstructor)
, m_standard(0)
{

}

UserDefinedConversion::UserDefinedConversion(LanguageElement* a_pOwner, Method* memberFunction) 
    : Conversion(a_pOwner, memberFunction->getOwnerClassType(), memberFunction->getReturnType())
, m_member_function(memberFunction)
, m_standard(0)
{

}

void UserDefinedConversion::apply(const void* a_pInput, void* a_pOutput) const
{
    if (m_member_function->asConstructor())
    {
        void* args[1] = { (void*)a_pInput };
        m_member_function->invoke(a_pOutput, args);
    }
    else
    {
        m_member_function->invoke((void*)a_pInput, nullptr, a_pOutput);
    }
}

int UserDefinedConversion::compare(const UserDefinedConversion& other) const
{
    /// 13.3.3.2 (3.3)
    /// User-defined Conversion sequence U1 is a better Conversion sequence than another user-defined Conversion
    /// sequence U2 if they contain the same user-defined Conversion function or constructor or they
    /// initialize the same class in an aggregate initialization and in either case the second standard Conversion
    /// sequence of U1 is better than the second standard Conversion sequence of U2. [Example:
    if (m_member_function == other.m_member_function)
    {
        if (!m_standard)
        {
            if (!other.m_standard) return 0;
            else return 1;
        }
        else if (!other.m_standard)
        {
            return -1;
        }
        return m_standard->compare(*other.m_standard);
    }
    return 0;
}

Expression* MethodPointerConversion::convert(Semantic* a_pSemantic, Expression* a_pExpression) const
{
    PHANTOM_ASSERT(output->asMethodPointer());
    return a_pSemantic->New<MethodPointerAdjustmentExpression>(output->asMethodPointer(), a_pExpression, offsetToBase);
}

void DefaultConversionSequence::apply(const void* a_pInput, void* a_pOutput) const
{
    if ((m_standard && !m_standard->isIdentity()) || m_user_defined)
    {
        if (m_standard &&(m_standard->m_numeric_conversion || m_standard->m_lvalue_transformation))
        {
            m_standard->apply(a_pInput, a_pOutput);
            a_pInput = a_pOutput;
        }
        if (m_user_defined)
        {
            m_user_defined->apply(a_pInput, a_pOutput);
        }
    }
    else
    {
        // raw copy
        memcpy(a_pOutput, a_pInput, std::min(input->getSize(), output->getSize()));
    }
}

void DefaultConversionSequence::addConstRef()
{
    PHANTOM_ASSERT(semantic == nullptr);
    if (m_user_defined && m_user_defined->output->asReference() == nullptr)
    {
        if (m_user_defined->m_standard == nullptr)
        {
            m_user_defined->m_standard = m_pOwner->new_<StandardConversionSequence>(m_pOwner, m_pOwner->new_<RefConversion>(m_pOwner, output));
        }
        else
        {
            m_user_defined->m_standard->addConstRef();
        }
    }
    else
    {
        if (m_standard == nullptr)
        {
            m_standard = m_pOwner->new_<StandardConversionSequence>(m_pOwner, m_pOwner->new_<RefConversion>(m_pOwner, output));
        }
        else
        {
            m_standard->addConstRef();
        }
    }
    output = output->makeConst()->makeLValueReference();
}

void DefaultConversionSequence::addRValueToLValuePromotion()
{
	PHANTOM_ASSERT(semantic == nullptr);
	PHANTOM_ASSERT(m_standard);
    m_standard->addRValueToLValuePromotion();
}

void DefaultConversionSequence::addRValueRef()
{
	PHANTOM_ASSERT(semantic == nullptr);
	if (m_user_defined && m_user_defined->output->asReference() == nullptr)
    {
        if (m_user_defined->m_standard == nullptr)
        {
            m_user_defined->m_standard = m_pOwner->new_<StandardConversionSequence>(m_pOwner, m_pOwner->new_<RefConversion>(m_pOwner, output, true));
        }
        else
        {
            m_user_defined->m_standard->addRValueRef();
        }
    }
    else
    {
        if (m_standard == nullptr)
        {
            m_standard = m_pOwner->new_<StandardConversionSequence>(m_pOwner, m_pOwner->new_<RefConversion>(m_pOwner, output, true));
        }
        else
        {
            m_standard->addRValueRef();
        }
    }
    output = output->removeReference()->addRValueReference();
}

void DefaultConversionSequence::addRValuePromotion()
{
	PHANTOM_ASSERT(semantic == nullptr);
	if (!m_standard)
	{
        m_standard = m_pOwner->new_<StandardConversionSequence>(m_pOwner, input, m_user_defined->input);
    }
    PHANTOM_ASSERT(input->asRValueReference());
    input = input->removeRValueReference();
    m_standard->addRValuePromotion();
}

bool DefaultConversionSequence::isCanonical() const
{
    return isIdentity() ||(m_standard == nullptr || m_standard->isCanonical()) && m_user_defined == nullptr;
}

void DefaultConversionSequence::addLValueTransform(Constructor* a_pCopyCtor)
{
	PHANTOM_ASSERT(semantic == nullptr);
	if (m_user_defined && m_user_defined->output->asReference())
    {
        PHANTOM_ASSERT(m_user_defined->m_member_function->asConstructor() == nullptr);
        if (m_user_defined->m_standard == nullptr)
        {
            m_user_defined->m_standard = m_pOwner->new_<StandardConversionSequence>(m_pOwner, m_pOwner->new_<LValueTransformationConversion>(m_pOwner, output, a_pCopyCtor));
        }
        else
        {
            m_user_defined->m_standard->addLValueTransform(a_pCopyCtor);
        }
        output = output->removeReference();
    } 
    else
    {
        PHANTOM_ASSERT(input->asReference() == nullptr);
        if (m_standard == nullptr)
        {
            m_standard = m_pOwner->new_<StandardConversionSequence>(m_pOwner, m_pOwner->new_<LValueTransformationConversion>(m_pOwner, input->addLValueReference(), a_pCopyCtor));
        }
        else
        {
            PHANTOM_ASSERT(m_standard->m_lvalue_transformation == nullptr);
            m_standard->m_lvalue_transformation = m_pOwner->new_<LValueTransformationConversion>(m_pOwner, input->addLValueReference(), a_pCopyCtor);
        }
        input = input->addLValueReference();
    }
}

int DefaultConversionSequence::compare(const DefaultConversionSequence& other) const
{
    /// 1) A standard Conversion sequence or user-defined Conversion sequence is always better than an ellipsis Conversion sequence.
    if (m_ellipsis.bool_ && !other.m_ellipsis.bool_) return -1;
    else if (!m_ellipsis.bool_ && other.m_ellipsis.bool_) return 1;
    if (m_pre_user_defined)
    {
        if (!other.m_pre_user_defined)
            return -1;
        else
        {
            int comp = m_pre_user_defined->compare(*other.m_pre_user_defined);
            if (comp)
                return comp;
        }
    }
    else if (other.m_pre_user_defined)
	{
		return 1;
	}
    if (!m_standard)
    {
        if (!m_user_defined)
            // isSame Conversion (canonical copy)
        {
            if (!other.m_standard && !other.m_user_defined)
                return 0;
            return 1;
        }
        else if (other.m_standard)
        {
            if (other.m_user_defined) return 1; /// only u > other.s+other.u
            else if (!other.m_user_defined) return -1; /// only u < other.s 
        }
        else
        {
            if (!other.m_user_defined) return -1; /// other is canonical Conversion
            else return m_user_defined->compare(*other.m_user_defined);
        }
    }
    else // m_standard
    {
        if (!m_user_defined) // s + !u
        {
            if (other.m_standard)
            {
                if (!other.m_user_defined) // o.s + !o.u
                    return m_standard->compare(*other.m_standard);
                else // o.s + o.u 
                    return 1;
            }
            else
            {
                if (!other.m_user_defined) // !o.s + !o.u
                    return -1;
                else // !o.s + o.u 
                    return 1;
            }
        }
        else // s + u
        {
            if (other.m_standard)
            {
                if (!other.m_user_defined) // o.s + !o.u
                    return -1;
                else // o.s + o.u 
                {
                    int scompare = m_standard->compare(*other.m_standard);
                    if (scompare == 0) return m_user_defined->compare(*other.m_user_defined);
                    return scompare;
                }
            }
            else
            {
                return -1;
            }
        }
    }
    /// 2) A standard Conversion sequence is always better than an user-defined Conversion
    if (m_standard && !other.m_standard)
        return 1;
    else if (other.m_standard && !m_standard)
        return -1;
    if (m_standard)
    {
        /// standard Conversion sequences comparison
        PHANTOM_ASSERT(other.m_standard);
        return m_standard->compare(*other.m_standard);
    }
    else
    {
        /// user defined conversions comparison
        PHANTOM_ASSERT(m_user_defined);
        return m_user_defined->compare(*other.m_user_defined);
    }
}

DefaultConversionSequence* DefaultConversionSequence::clone(LanguageElement* a_pOwner) const
{
    DefaultConversionSequence* pClone = a_pOwner->new_<DefaultConversionSequence>(a_pOwner, input, output);
    pClone->m_standard = m_standard ? m_standard->clone(a_pOwner) : nullptr;
    pClone->m_user_defined = m_user_defined ? m_user_defined->clone(a_pOwner) : nullptr;
	pClone->m_pre_user_defined = m_pre_user_defined ? m_pre_user_defined->clone(a_pOwner) : nullptr;
	pClone->m_ellipsis = m_ellipsis;
    return pClone;
}

Expression* DefaultConversionSequence::convert(Semantic* a_pSemantic, Expression* a_pExpression) const
{
    if (a_pExpression->getMetaClass() == PHANTOM_CLASSOF(InitializerListExpression) )
    {
		auto outputNoRefQual = output->removeReference()->removeQualifiers();
		auto pOutputTpl = outputNoRefQual->getTemplate();
		bool outputIsStdInitList = pOutputTpl && pOutputTpl->getQualifiedName() == "std::initializer_list";
		if (input->asInitializerListType())
		{
			InitializerListExpression* pInitListExpression = static_cast<InitializerListExpression*>(a_pExpression);
			if (outputIsStdInitList)
			{
                auto pSource = a_pSemantic->getSource();

				Expressions newArgs;
				Type* pInitType = static_cast<Type*>(outputNoRefQual->getTemplateSpecialization()->getArgument(0));
				for (auto pExp : pInitListExpression->getExpressions())
				{
					newArgs.push_back(a_pSemantic->convert(pExp, pInitType, pSource ? static_cast<LanguageElement*>(pSource) : Namespace::Global()));
					PHANTOM_ASSERT(newArgs.back());
				}
				Expression* pNewExp =
					a_pSemantic->New<InitializerListExpression>(static_cast<Class*>(outputNoRefQual), newArgs);
				if (outputNoRefQual != output.type)
				{
					// const ref
					return a_pSemantic->convert(pNewExp, output.type, pSource ? static_cast<LanguageElement*>(pSource) : Namespace::Global());
				}
				return pNewExp;
			}
		}
        auto inputNoRefQual = input->removeReference()->removeQualifiers();
        if ((output.type == input.type && output.type == outputNoRefQual) || (inputNoRefQual != outputNoRefQual))
        {
            return a_pSemantic->convert(a_pExpression, output.type, a_pSemantic->getSource());
        }
    }
    if (m_standard)
    {
        Expression* pConverted = m_standard->convert(a_pSemantic, a_pExpression);
        if (a_pExpression == pConverted)
        {
            if (!m_standard->output.type->isSame(a_pExpression->getValueType()))
            {
                a_pExpression = a_pSemantic->New<IdentityExpression>(m_standard->output.type, pConverted);
                static_cast<IdentityExpression*>(a_pExpression)->setConversionKind(kind);
            }
        }
        else
        {
            a_pExpression = pConverted;
            if (!m_standard->output.type->isSame(a_pExpression->getValueType()))
            {
				a_pExpression = a_pSemantic->New<IdentityExpression>(m_standard->output.type, a_pExpression);
				static_cast<IdentityExpression*>(a_pExpression)->setConversionKind(kind);
            }
        }
    }
    Conversion* preUserDefinedConv = m_standard;
    if (m_pre_user_defined)
    {
        preUserDefinedConv = m_pre_user_defined;
        Expression* pConverted = m_pre_user_defined->convert(a_pSemantic, a_pExpression);
        if (a_pExpression == pConverted)
        {
            a_pExpression = m_standard->output.type->isSame(a_pExpression->getValueType())
            ? a_pExpression
            : a_pSemantic->New<IdentityExpression>(m_pre_user_defined->output.type, pConverted);
        }
        else
        {
            a_pExpression = pConverted;
        }
    }
    if (m_user_defined)
    {
        Expression* pConverted = m_user_defined->convert(a_pSemantic, a_pExpression);
        if (a_pExpression == pConverted)
        {
            a_pExpression = preUserDefinedConv->output.type->isSame(a_pExpression->getValueType())
                ? a_pExpression
                : a_pSemantic->New<IdentityExpression>(m_user_defined->output.type, pConverted);
        }
        else
        {
            a_pExpression = pConverted;
        }
    }
    return a_pExpression;
}

StandardConversionSequence::StandardConversionSequence(LanguageElement* a_pOwner, ConversionVal io) : Conversion(a_pOwner, io, io), m_lvalue_transformation(0), m_numeric_conversion(0), m_qualification_adjustment(0), m_const_ref(0)
{

}

StandardConversionSequence::StandardConversionSequence(LanguageElement* a_pOwner, ConversionVal i, ConversionVal o) : Conversion(a_pOwner, i, o), m_lvalue_transformation(0), m_numeric_conversion(0), m_qualification_adjustment(0), m_const_ref(0)
{

}
StandardConversionSequence::StandardConversionSequence(LanguageElement* a_pOwner, RefConversion* cr)
    : Conversion(a_pOwner, cr->input, cr->output), m_lvalue_transformation(0), m_numeric_conversion(0), m_qualification_adjustment(0), m_const_ref(cr)
{

}

StandardConversionSequence::StandardConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lt) 
    : Conversion(a_pOwner, lt->input, lt->output), m_lvalue_transformation(lt), m_numeric_conversion(0), m_qualification_adjustment(0), m_const_ref(0)
{

}

StandardConversionSequence::StandardConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lt, QualificationAdjustConversion* qa, RefConversion* cr /*= 0*/) 
    : Conversion(a_pOwner, lt->input, qa->output), m_lvalue_transformation(lt), m_numeric_conversion(0), m_qualification_adjustment(qa), m_const_ref(cr)
{

}

StandardConversionSequence::StandardConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lt, NumericConversion* nc, RefConversion* cr /*= 0*/) 
    : Conversion(a_pOwner, lt->input, cr ? cr->output : nc->output), m_lvalue_transformation(lt), m_numeric_conversion(nc), m_qualification_adjustment(0), m_const_ref(cr)
{

}

StandardConversionSequence::StandardConversionSequence(LanguageElement* a_pOwner, LValueTransformationConversion* lt, NumericConversion* nc, QualificationAdjustConversion* qa, RefConversion* cr /*= 0*/) 
    : Conversion(a_pOwner, lt->input, cr ? cr->output : qa->output), m_lvalue_transformation(lt), m_numeric_conversion(nc), m_qualification_adjustment(qa), m_const_ref(cr)
{

}

StandardConversionSequence::StandardConversionSequence(LanguageElement* a_pOwner, NumericConversion* nc, QualificationAdjustConversion* qa, RefConversion* cr /*= 0*/) 
    : Conversion(a_pOwner, nc->input, cr ? cr->output : qa->output), m_lvalue_transformation(0), m_numeric_conversion(nc), m_qualification_adjustment(qa), m_const_ref(cr)
{

}

StandardConversionSequence::StandardConversionSequence(LanguageElement* a_pOwner, NumericConversion* nc, RefConversion* cr /*= 0*/) 
    : Conversion(a_pOwner, nc->input, cr ? cr->output : nc->output), m_lvalue_transformation(0), m_numeric_conversion(nc), m_qualification_adjustment(0), m_const_ref(cr)
{

}

StandardConversionSequence::StandardConversionSequence(LanguageElement* a_pOwner, QualificationAdjustConversion* qa, RefConversion* cr /*= 0*/) 
    : Conversion(a_pOwner, qa->input, cr ? cr->output : qa->output), m_lvalue_transformation(0), m_numeric_conversion(0), m_qualification_adjustment(qa), m_const_ref(cr)
{

}

void StandardConversionSequence::addRValuePromotion()
{
    PHANTOM_ASSERT(!m_rvalue_promotion);
    PHANTOM_ASSERT(input->asRValueReference());
    m_rvalue_promotion = true;
    input = input->removeRValueReference();
}

void StandardConversionSequence::apply(const void* a_pInput, void* a_pOutput) const
{
    if (m_lvalue_transformation)
    {
        a_pInput = *(void**)a_pInput;
        if (m_numeric_conversion == nullptr)
        {
#pragma message(PHANTOM_TODO "why this assert ? please, me, put comment")
            // PHANTOM_ASSERT(m_const_ref == nullptr);
            memcpy(a_pOutput, a_pInput, output->getSize());
            return;
        }
    }
    if (m_numeric_conversion)
    {
        m_numeric_conversion->apply(a_pInput, a_pOutput);
        a_pInput = a_pOutput;
    }
    /*
    //     if(m_const_ref)
    //     {
    //         m_const_ref->apply(a_pInput, a_pOutput);
    //     }
    */
}

int StandardConversionSequence::compare(const StandardConversionSequence& other) const
{
    if (m_numeric_conversion)
    {
        if (!other.m_numeric_conversion) return -1;
        else return m_numeric_conversion->compare(*other.m_numeric_conversion);
    }
    else
    {
        if (other.m_numeric_conversion) return 1;
        else
        {
            if (m_qualification_adjustment)
            {
                if (!other.m_qualification_adjustment) return -1;
                else return m_qualification_adjustment->compare(*other.m_qualification_adjustment);
            }
            else
            {
                if (other.m_qualification_adjustment) return 1;
                if (m_const_ref && other.m_const_ref)
                {
                    if (m_const_ref->output->asRValueReference() && other.m_const_ref->output->asLValueReference())
                        return 1;
                    if (m_const_ref->output->asLValueReference() && other.m_const_ref->output->asRValueReference())
                        return -1;
                }
            }
        }
    }
    return 0;
}

void StandardConversionSequence::addConstRef()
{
    PHANTOM_ASSERT(input->asReference() == nullptr);
    m_const_ref = m_pOwner->new_<RefConversion>(m_pOwner, output, false);
    output = output->makeConst()->makeLValueReference();
}

void StandardConversionSequence::addRValueToLValuePromotion()
{
    m_qualification_adjustment = m_pOwner->new_<QualificationAdjustConversion>(m_pOwner, input->removeReference()->addRValueReference(), input, 1);
    PHANTOM_ASSERT(input->asLValueReference());
    input = input->addRValueReference();
}

void StandardConversionSequence::addRValueRef()
{
    PHANTOM_ASSERT(input->asReference() == nullptr);
    m_const_ref = m_pOwner->new_<RefConversion>(m_pOwner, output, true);
    output = output->removeReference()->addRValueReference();
    PHANTOM_ASSERT(output);
}

void StandardConversionSequence::addLValueTransform(Constructor* a_pCopyCtor)
{
    m_lvalue_transformation = m_pOwner->new_<LValueTransformationConversion>(m_pOwner, output, a_pCopyCtor);
    output = output->removeReference();
}

bool StandardConversionSequence::isCanonical() const
{
    return ((m_lvalue_transformation == nullptr)
        &&(m_const_ref == nullptr)
        &&(m_numeric_conversion == nullptr
            || m_numeric_conversion->isCanonical()
            ||(m_numeric_conversion->input.type == PHANTOM_TYPEOF(std::nullptr_t) && m_numeric_conversion->output.type->getTypeKind() == TypeKind::Pointer)
            ||(m_numeric_conversion->output.type == PHANTOM_TYPEOF(void*) && m_numeric_conversion->input.type->getTypeKind() == TypeKind::Pointer)));
}

StandardConversionSequence* StandardConversionSequence::clone(LanguageElement* a_pOwner) const
{
    StandardConversionSequence* pClone = a_pOwner->new_<StandardConversionSequence>(a_pOwner, input, output);
    pClone->m_lvalue_transformation = m_lvalue_transformation ? m_lvalue_transformation->clone(a_pOwner) : nullptr;
    pClone->m_numeric_conversion = m_numeric_conversion ? m_numeric_conversion->clone(a_pOwner) : nullptr;
    pClone->m_qualification_adjustment = m_qualification_adjustment ? m_qualification_adjustment->clone(a_pOwner) : nullptr;
    pClone->m_const_ref = m_const_ref ? m_const_ref->clone(a_pOwner) : nullptr;
    return pClone;
}

int QualificationAdjustConversion::compare(const Conversion& a_Other) const
{
    return (((const QualificationAdjustConversion&)a_Other).adjustmentCount > adjustmentCount) - (((const QualificationAdjustConversion&)a_Other).adjustmentCount < adjustmentCount);
}

LValueTransformationConversion* LValueTransformationConversion::clone(LanguageElement* a_pOwner) const
{
    return a_pOwner->new_<LValueTransformationConversion>(a_pOwner, input, copyConstructor);
}

RefConversion* RefConversion::clone(LanguageElement* a_pOwner) const
{
    return a_pOwner->new_<RefConversion>(a_pOwner, input, output->asRValueReference() != nullptr);
}

NumericConversion* NumericConversion::clone(LanguageElement* a_pOwner) const
{
    return a_pOwner->new_<NumericConversion>(a_pOwner, input, output, promotion, pointerToPointerInheritanceLevel);
}

Expression* PointerConversion::convert(Semantic* a_pSemantic, Expression* a_pExpression) const
{
    if (offsetToBase)
    {
        auto pExp = a_pSemantic->New<PointerAdjustmentExpression>(output, a_pExpression, offsetToBase);
        if (kind == CastKind::Implicit)
            pExp->setImplicit();
        return pExp;
    }
    else
    {
        auto pExp = a_pSemantic->New<IdentityExpression>(output.type, a_pExpression);
        pExp->setConversionKind(kind);
        return pExp;
    }
}

Expression* FieldPointerConversion::convert(Semantic* a_pSemantic, Expression* a_pExpression) const
{
    return offsetToBase ? (Expression*)(a_pSemantic->New<FieldPointerAdjustmentExpression>(output, a_pExpression, offsetToBase)) : a_pSemantic->New<IdentityExpression>(output.type, a_pExpression);
}

Expression* UserDefinedConversion::convert(Semantic* a_pSemantic, Expression* a_pExpression) const
{
    if (m_member_function)
    {
        Constructor* pCtor = m_member_function->asConstructor();
        if (pCtor)
        {
            Expressions args{ a_pExpression };
            /// Constructor
            PHANTOM_ASSERT(pCtor->getRequiredArgumentCount() == 1);
            size_t count = 1;
            while (count != pCtor->getParameters().size())
            {
                Expression* pDefaultArg = pCtor->getParameters()[count]->getDefaultArgumentExpression();
				PHANTOM_ASSERT(pDefaultArg);
                pDefaultArg = pDefaultArg->clone(a_pSemantic->getSource());
                if (pDefaultArg->getValueType() != pCtor->getParameters()[count]->getValueType())
                {
                    auto pDefaultArgConv = a_pSemantic->initialize(pDefaultArg, pCtor->getParameters()[count]->getValueType());
                    PHANTOM_ASSERT(pDefaultArgConv);
                    pDefaultArg = pDefaultArgConv;
                }
                args.push_back(pDefaultArg);
                ++count;
            }
            a_pExpression = a_pSemantic->New<ConstructorCallExpression>(pCtor, args);
            if(!pCtor->testModifiers(Modifier::Explicit) && kind == CastKind::Implicit)
                a_pExpression->setImplicit();
        }
        else
        {
            /// Conversion function
            Expressions arguments;
            if (a_pExpression->getValueType()->asReference())
                arguments.push_back(a_pExpression->address(a_pSemantic->getSource())); /// we need to take address of the value to pass it as 'this'
            else
                arguments.push_back((a_pSemantic->New<RValueReferenceExpression>(a_pExpression))->address(a_pSemantic->getSource())); //  any "pure class value" needs to be first converted as RValueReferenceExpresssion to get its address
            a_pExpression = a_pSemantic->New<CallExpression>(m_member_function, arguments);
			if (!m_member_function->testModifiers(Modifier::Explicit) && kind == CastKind::Implicit)
				static_cast<CallExpression*>(a_pExpression)->setImplicit(); /// ensure call is marked as implicit to avoid it to be translated to text when retro generating code
        }
    }
    if (m_standard)
    {
        a_pExpression = m_standard->convert(a_pSemantic, a_pExpression);
    }
    return a_pExpression;
}

void DefaultConversionSequence::setKind(CastKind _kind)
{
    kind = _kind;
    if (m_standard)
    {
        m_standard->kind = _kind;
        if (m_standard->m_numeric_conversion)
            m_standard->m_numeric_conversion->kind = _kind;
    }
    if(!m_standard || m_standard->m_numeric_conversion == nullptr)
	{
		if (m_pre_user_defined)
			m_pre_user_defined->kind = _kind;
		else if (m_user_defined)
			m_user_defined->kind = _kind;
	}
}

#pragma warning(default : 4312)
