// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "Semantic.h"
#include "phantom/lang/TemplateParameter.h"

#include <phantom/lang/Message.h>
#include <phantom/lang/Placeholder.h>
#include <phantom/lang/TemplateSpecialization.h>
#include <stdarg.h>

#define PHANTOM_SEMANTIC_ASSERT PHANTOM_ASSERT_DEBUG

namespace phantom
{
namespace lang
{
static const char* _canBe = "can be";
static const char* _or = "or";

inline String stringf(const char* a_Format, va_list args)
{
    size_t size = strlen(a_Format) + 512;
    char*  buffer = (char*)alloca(size);
    buffer[size - 1] = '\0';
    int r = vsnprintf(buffer, size, a_Format, args);
    if (r == -1)
        return String();
    return buffer;
}

inline String stringf(const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    String str = stringf(a_Format, args);
    va_end(args);
    return str;
}

inline StringView getAccessString(Access a_Access)
{
    switch (a_Access)
    {
    case Access::Public:
        return "public";
    case Access::Protected:
        return "protected";
    case Access::Private:
        return "private";
    default:
        PHANTOM_SEMANTIC_ASSERT(false);
        return "unknown";
    }
}

#define FormatCStr(s) FormatElement(s).data()
#define o_release_arguments(...)
#define CxxCustomSemanticError(sem, ...)                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        if (sem->getMessage())                                                                                         \
            sem->getMessage()->errorD(sem->currentCodeRangeLocation(), __VA_ARGS__);                                   \
    } while (0)

#define CxxSemanticError(...)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        if (m_pMessage)                                                                                                \
            m_pMessage->errorD(currentCodeRangeLocation(), __VA_ARGS__);                                               \
    } while (0)
#define CxxSemanticSubError(...)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        if (m_pMessage)                                                                                                \
            m_pMessage->getChildren().back()->informationD(currentCodeRangeLocation(), __VA_ARGS__);                   \
    } while (0)
#define CxxSemanticSubSubError(...)                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        if (m_pMessage)                                                                                                \
            m_pMessage->getChildren().back()->getChildren().back()->informationD(currentCodeRangeLocation(),           \
                                                                                 __VA_ARGS__);                         \
    } while (0)
#define CxxSemanticErrorLoc(loc, code, ...)                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        const void* args[] = {__VA_ARGS__, 0};                                                                         \
        if (!m_ErrorDelegate.empty())                                                                                  \
        {                                                                                                              \
            m_ErrorDelegate(loc, code, in_pContextScope, args);                                                        \
        }                                                                                                              \
    } while (false)
#define CxxSemanticCheckSymbolName(name, scope)                                                                        \
    CxxSemanticErrorReturnIf((scope)->hasSymbol(name), "'%s' : symbol with same name already exists in '%s'",          \
                             name.data(), FormatCStr(scope))
#define CxxSemanticErrorReturnIf(cond, ...)                                                                            \
    if (cond)                                                                                                          \
    {                                                                                                                  \
        CxxSemanticError(__VA_ARGS__);                                                                                 \
        return;                                                                                                        \
    }
#define CxxSemanticErrorReturnValIf(code, cond, ...)                                                                   \
    if (cond)                                                                                                          \
    {                                                                                                                  \
        CxxSemanticError(__VA_ARGS__);                                                                                 \
        return code;                                                                                                   \
    }

#define CxxSemanticErrorConversion(fromExp, toType) ConversionError(fromExp, toType)

#define CxxSemanticErrorInacesssibleArgs(elem, scope)                                                                  \
    "'%.*s' : %.*s in '%.*s' and inaccessible from '%.*s'", PHANTOM_STRING_AS_PRINTF_ARG(FormatElement(elem)),         \
    PHANTOM_STRING_AS_PRINTF_ARG(getAccessString(elem->getAccess())),                                                  \
    PHANTOM_STRING_AS_PRINTF_ARG(FormatElement(elem->getEnclosingClassType())),                                        \
    PHANTOM_STRING_AS_PRINTF_ARG(FormatElement(scope))

#define CxxSemanticErrorIfInaccessible(elem, scope, ...)                                                               \
    CxxSemanticErrorReturnValIf(__VA_ARGS__, !(isSymbolAccessible(elem, scope)),                                       \
                                CxxSemanticErrorInacesssibleArgs(elem, scope))

#define CxxSemanticTemplateResolutionError(elem)                                                                       \
    CxxSemanticError("'%.*s' : template instantiation failure", PHANTOM_STRING_AS_PRINTF_ARG(elem->getName()));

#define CxxSemanticConversion(exp, type)                                                                               \
    convert(exp, type, CastKind::Implicit, UserDefinedFunctions::ImplicitsOnly, in_pContextScope);
#define CxxSemanticConversionNE(exp, type) convertIfNotEqual(exp, type, in_pContextScope);

#define in_Pass (*(EClassBuildState*)a_Data.in[1])

#define PHANTOM_SEMANTIC_ASSERT_SEMANTIC_HAS_ERROR_REPORT(elem)                                                        \
    PHANTOM_SEMANTIC_ASSERT(elem || this->getMessage() == nullptr ||                                                   \
                            this->getMessage()->getMostValuableMessageType() == MessageType::Error)

/// TEMPLATE INSTANCIATION

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define o_instantiateT(...)                                                                                        \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(o_instantiateT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))
#    define o_instantiate_asT(...)                                                                                     \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(o_instantiate_asT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))
#    define o_resolveT(...) PHANTOM_PP_CAT(PHANTOM_PP_CAT(o_resolveT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))
#    define o_resolve_asT(...)                                                                                         \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(o_resolve_asT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))
//#   define o_findT(...) PHANTOM_PP_CAT(PHANTOM_PP_CAT(o_findT_,
// PHANTOM_PP_ARGCOUNT(__VA_ARGS__)),(__VA_ARGS__))
#else
#    define o_instantiateT(...) PHANTOM_PP_CAT(o_instantiateT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#    define o_instantiate_asT(...) PHANTOM_PP_CAT(o_instantiate_asT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#    define o_resolveT(...) PHANTOM_PP_CAT(o_resolveT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#    define o_resolve_asT(...) PHANTOM_PP_CAT(o_resolve_asT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
//#   define o_findT(...) PHANTOM_PP_CAT(o_findT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#endif

#define o_findT(type, primary) static_cast<type*>(_findInstantiation(in_TemplateSubstitution, primary))

#define o_mapT(primary, instance)                                                                                      \
    templateInstantiations()[in_TemplateSubstitution.getInstantiation()][primary] = instance

#define o_instantiateT_2(type, primary) o_instantiateT_3(type, primary, *(LanguageElement**)a_Data.in[2])

#define o_instantiateT_3(type, primary, scope)                                                                         \
    SemanticPrivate::SemanticInstantiateTemplateElement<type>(                                                         \
    this, (primary), *(TemplateSubstitution const*)a_Data.in[0], *(EClassBuildState*)a_Data.in[1], scope,              \
    a_Data.flags, &LanguageElement::as##type)

#define o_instantiate_asT_2(type, primary) o_instantiate_asT_3(type, primary, *(LanguageElement**)a_Data.in[2])

#define o_instantiate_asT_3(type, primary, scope)                                                                      \
    SemanticPrivate::SemanticInstantiateTemplateElement<type>(this, (primary),                                         \
                                                              *(TemplateSubstitution const*)a_Data.in[0],              \
                                                              *(EClassBuildState*)a_Data.in[1], scope, a_Data.flags)

#define o_resolveT_2(type, primary) o_resolveT_3(type, primary, *(LanguageElement**)a_Data.in[2])

#define o_resolve_asT_2(type, primary) o_resolve_asT_3(type, primary, *(LanguageElement**)a_Data.in[2])

class SemanticPrivate
{
public:
    template<class T>
    static T* SemanticResolveTemplateDependency(Semantic* a_pSemantic, LanguageElement* a_pElement,
                                                const class TemplateSubstitution& a_TemplateSubstitution,
                                                Semantic::EClassBuildState a_uiPass, LanguageElement* a_pScope,
                                                int a_Flags, T* (LanguageElement::*a_Func)() const)
    {
        PHANTOM_SEMANTIC_ASSERT(a_pElement);
        LanguageElement* pResult =
        a_pSemantic->resolveTemplateDependency(a_pElement, a_TemplateSubstitution, a_uiPass, a_pScope, a_Flags);
        return pResult ? (pResult->*a_Func)() : nullptr;
    }
    template<class T>
    static T* SemanticInstantiateTemplateElement(Semantic* a_pSemantic, LanguageElement* a_pElement,
                                                 const class TemplateSubstitution& a_TemplateSubstitution,
                                                 Semantic::EClassBuildState a_uiPass, LanguageElement* a_pScope,
                                                 int a_Flags, T* (LanguageElement::*a_Func)() const)
    {
        if (a_pElement == nullptr)
            return nullptr;
        LanguageElement* pResult =
        a_pSemantic->instantiateTemplateElement(a_pElement, a_TemplateSubstitution, a_uiPass, a_pScope, a_Flags);
        if (!pResult)
            return nullptr;
        T* pResT = (pResult->*a_Func)();
        if (!pResT)
        {
            CxxCustomSemanticError(a_pSemantic, "'%s' : unexpected symbol", a_pSemantic->FormatCStr(pResult));
        }
        return pResT;
    }

    /// @off
    inline static Expression*
    SemanticInstantiateTemplateElement(Semantic* a_pSemantic, Expression* a_pElement,
                                       const class TemplateSubstitution& a_TemplateSubstitution,
                                       Semantic::EClassBuildState a_uiPass, LanguageElement* a_pScope, int a_Flags,
                                       Expression* (LanguageElement::*a_Func)() const)
    {
        if (a_pElement == nullptr)
            return nullptr;
        if (!a_pElement->isTemplateDependant())
            return a_pElement->clone(a_pSemantic->getSource());
        LanguageElement* pResult =
        a_pSemantic->instantiateTemplateElement(a_pElement, a_TemplateSubstitution, a_uiPass, a_pScope, a_Flags);
        PHANTOM_SEMANTIC_ASSERT(pResult == nullptr || !(pResult->isTemplateDependant()));
        if (!pResult)
            return nullptr;
        Expression* pResT = (pResult->*a_Func)();
        if (!pResT)
        {
            CxxCustomSemanticError(a_pSemantic, "'%s' : unexpected symbol", a_pSemantic->FormatCStr(pResult));
        }
        return pResT;
    }
    template<class T>
    static T* SemanticResolveTemplateDependency(Semantic* a_pSemantic, LanguageElement* a_pElement,
                                                const class TemplateSubstitution& a_TemplateSubstitution,
                                                Semantic::EClassBuildState a_uiPass, LanguageElement* a_pScope,
                                                int a_Flags)
    {
        PHANTOM_SEMANTIC_ASSERT(a_pElement);
        if (!a_pElement->isTemplateDependant())
            return static_cast<T*>(a_pElement);
        LanguageElement* pResult =
        a_pSemantic->resolveTemplateDependency(a_pElement, a_TemplateSubstitution, a_uiPass, a_pScope, a_Flags);
        PHANTOM_SEMANTIC_ASSERT(pResult == nullptr || !(pResult->isTemplateDependant()));
        return pResult ? phantom::Object::Cast<T>(pResult) : nullptr;
    }
    template<class T>
    static T* SemanticInstantiateTemplateElement(Semantic* a_pSemantic, LanguageElement* a_pElement,
                                                 const class TemplateSubstitution& a_TemplateSubstitution,
                                                 Semantic::EClassBuildState a_uiPass, LanguageElement* a_pScope,
                                                 int a_Flags)
    {
        if (a_pElement == nullptr)
            return nullptr;
        LanguageElement* pResult =
        a_pSemantic->instantiateTemplateElement(a_pElement, a_TemplateSubstitution, a_uiPass, a_pScope, a_Flags);
        PHANTOM_SEMANTIC_ASSERT(pResult == nullptr || !(pResult->isTemplateDependant()));
        return pResult ? phantom::Object::Cast<T>(pResult) : nullptr;
    }
};

#define o_resolveT_3(type, primary, scope)                                                                             \
    SemanticPrivate::SemanticResolveTemplateDependency<type>(                                                          \
    this, (primary), *(TemplateSubstitution const*)a_Data.in[0], *(EClassBuildState*)a_Data.in[1], scope,              \
    a_Data.flags, &LanguageElement::as##type)

#define o_resolve_asT_3(type, primary, scope)                                                                          \
    SemanticPrivate::SemanticResolveTemplateDependency<type>(                                                          \
    this, (primary), *(TemplateSubstitution const*)a_Data.in[0], *(EClassBuildState*)a_Data.in[1], scope,              \
    a_Data.flags) //
                  // #define o_findT_2(type, element)\
                                                //     (((element) == nullptr) ? nullptr : (*(ppTempElement =
                  //     &m_Templated[&arguments][element])) ? (*ppTempElement)->as##type() : nullptr)\
//
// #define o_findT_3(type, element, scope)\
//     (((element) == nullptr) ? nullptr : (*(ppTempElement = &m_Templated[&arguments][element])) ? (*ppTempElement)->as##type() : nullptr)\


#if PHANTOM_HAS_BUILT_IN_WCHAR_T
#    define case_e_wchar_t case TypeKind::WChar:
#else
#    define case_e_wchar_t
#endif

#if PHANTOM_HAS_BUILT_IN_CHAR16_T
#    define case_e_char16_t case TypeKind::Char16:
#else
#    define case_e_char16_t
#endif

#if PHANTOM_HAS_BUILT_IN_CHAR32_T
#    define case_e_char32_t case TypeKind::Char32:
#else
#    define case_e_char32_t
#endif

#define o_instantiate_template_clone                                                                                   \
    case e_VisitorFunction_InstanciateTemplateElement:                                                                 \
    {                                                                                                                  \
        LanguageElement*& pInstanciated = *(LanguageElement**)a_Data.out[0];                                           \
        pInstanciated = a_pInput->clone(getSource());                                                                  \
    }                                                                                                                  \
        return;

} // namespace lang
} // namespace phantom
