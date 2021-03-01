// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/lang/LanguageElement.h>

namespace phantom
{
namespace lang
{
template<class T>
struct OwnerGuard
{
    OwnerGuard(T* a_pElem) : m_pElement(a_pElem) {}
    ~OwnerGuard()
    {
        // TODO : see what to do now with this ...
        //         if (m_pElement && m_pElement->getOwner() == nullptr)
        //             m_pElement->setOwner(m_pElement->getSource());
    }
    T* take()
    {
        T* pElem = m_pElement;
        m_pElement = nullptr;
        return pElem;
    }
    T*& element() { return m_pElement; }
    T*  operator->() { return m_pElement; }
    T*  m_pElement;

    operator T*() const { return m_pElement; }

    OwnerGuard<T>& operator=(T* a_pOther)
    {
        m_pElement = a_pOther;
        return *this;
    }

    template<class U>
    bool operator==(U a_pOther) const
    {
        return m_pElement == a_pOther;
    }
    template<class U>
    bool operator!=(U a_pOther) const
    {
        return m_pElement != a_pOther;
    }
};

template<class Ts>
struct OwnersGuard
{
    OwnersGuard() {}
    OwnersGuard(const Ts& list) : m_Elements(list) {}
    ~OwnersGuard()
    {
        // TODO : see what to do now with this ...
        // 		if (!m_bCancelled)
        //             for (auto pElem : m_Elements)
        //             {
        //                 if (pElem && pElem->getOwner() == nullptr)
        //                     pElem->setOwner(pElem->getSource());
        //             }
    }
    void             cancel() { m_bCancelled = true; }
    void             push_back(typename Ts::value_type const& a_Val) { m_Elements.push_back(a_Val); }
    size_t           size() const { return m_Elements.size(); }
    bool             empty() const { return m_Elements.empty(); }
    PHANTOM_TYPENAME Ts::const_iterator begin() const { return m_Elements.begin(); }
    PHANTOM_TYPENAME Ts::const_iterator end() const { return m_Elements.end(); }
    Ts&                                 elements() { return m_Elements; }
    Ts&                                 operator*() { return m_Elements; }
    Ts*                                 operator->() { return &m_Elements; }
    Ts                                  m_Elements;

    operator typename Ts::ArrayViewType() const { return m_Elements; }
    operator Ts const&() const { return m_Elements; }
    operator Ts&() { return m_Elements; }

private:
    bool m_bCancelled = false;
};
} // namespace lang
} // namespace phantom
