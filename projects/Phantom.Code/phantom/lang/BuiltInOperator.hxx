#pragma once

// haunt {

// clang-format off

#include "BuiltInOperator.h"

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
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/Delegate.hxx>
#include <phantom/utils/SmallSet.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("BuiltInOperator")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<BuiltInOperatorDelegate>("BuiltInOperatorDelegate"); }
        PHANTOM_CLASS(BuiltInOperator)
        {
            using Overload = typedef_<_::Overload>;
            using Overloads = typedef_<_::Overloads>;
            using String = typedef_< phantom::String>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using TypeSet = typedef_<_::TypeSet>;
            this_()
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .typedef_<TypeSet>("TypeSet")
            .staticMethod<::phantom::lang::OperatorKind(Operator)>("OperatorType", &_::OperatorType)({"a_eOperator"})
            .staticMethod<const char*(Operator)>("OperatorName", &_::OperatorName)({"a_eOperator"})
            .struct_<Overload>()
                .constructor<void(), default_>()
                .constructor<void(Type*, Type*, Type*, BuiltInOperatorDelegate)>()({"r","o0","o1","d"})
                .field("result", &_::Overload::result)
                .field("op0", &_::Overload::op0)
                .field("op1", &_::Overload::op1)
                .field("delegate", &_::Overload::delegate)
                .method<void(Language*, Type**, ConversionResults&) const>("newImplicitConversions", &_::Overload::newImplicitConversions)({"a_pLanguage","a_ppTypes","out"})
                .method<String(StringView) const>("toString", &_::Overload::toString)({"a_strName"})
                .method<bool(const Overload&) const>("operator==", &_::Overload::operator==)({"other"})
                .method<bool(const Overload&) const>("operator!=", &_::Overload::operator!=)({"other"})
            .end()
            .typedef_<Overloads>("Overloads")
        
        .public_()
            .constructor<void(Operator)>()({"a_eOperatorId"})
            .method<::phantom::lang::BuiltInOperator *() const, virtual_>("asBuiltInOperator", &_::asBuiltInOperator)
            .method<OperatorKind() const>("getOperatorType", &_::getOperatorType)
            .method<Operator() const>("getId", &_::getId)
            .method<size_t() const>("getOperandCount", &_::getOperandCount)
            .method<void(StringBuffer&) const, virtual_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .method<void(Type**, SmallVector<ConversionResults>&, Overloads&) const>("newImplicitConversions", &_::newImplicitConversions)({"a_ppArgumentTypes","a_Conversions","a_Overloads"})
            .method<void(Expression**, SmallVector<ConversionResults>&, Overloads&) const>("newImplicitConversions", &_::newImplicitConversions)({"a_ppExpressions","a_Conversions","a_Overloads"})
            .method<void(SmallVector<ConversionResults>&) const>("deleteConversions", &_::deleteConversions)({"a_Conversions"})
            .method<::phantom::SmallVector<Overload, 4>::const_iterator() const>("beginOverloads", &_::beginOverloads)
            .method<::phantom::SmallVector<Overload, 4>::const_iterator() const>("endOverloads", &_::endOverloads)
            .method<Overload(size_t) const>("getOverload", &_::getOverload)({"i"})
        
        .protected_()
            .field("m_eOperator", &_::m_eOperator)
            .field("m_Overloads", &_::m_Overloads)
            .field("m_eType", &_::m_eType)
            .field("m_uiOperandCount", &_::m_uiOperandCount)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BuiltInOperator")
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
