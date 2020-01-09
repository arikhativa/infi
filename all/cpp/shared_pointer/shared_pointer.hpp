
/*
Shared Pointer-

Discription:    API for using Shared Pointer
Date:           1.1.2020
Folder name:    Shared Pointer
File names:     shared_pointer.hpp, shared_pointer_test.cpp

Reviewer:

Ver 1.0
*/

#ifndef __HRD11_SHARED_POINTER_HPP__
#define __HRD11_SHARED_POINTER_HPP__

#include <cstddef>      // size_t
#include <algorithm>   // std::swap

namespace hrd11
{


// **************************** Class Declartion ******************************

template <typename T>
class SharedPointer
{
public:
    // create example - SharedPointer<X> x_ptr(new X(...))
    // should avoid - X* p1 = new X(...);
    //                SharedPointer<X> x_ptr(p1);
    explicit SharedPointer(T* ptr = 0);

    template <typename TDerive>
    SharedPointer(const SharedPointer<TDerive>& derive);

    ~SharedPointer();
    SharedPointer(const SharedPointer& other);
    SharedPointer& operator = (const SharedPointer& other);

    bool operator == (const SharedPointer& rhs) const;

    bool operator !() const;
    T& operator *() const;
    T* operator ->() const;

    // template <typename TBase>
    // operator SharedPointer<TBase>();

    T* GetPtr() const; // for backwards compatible

private:
    template <typename>
    friend class SharedPointer;

    void Detach();

    T* m_data;
    size_t* m_count;
};


// *********************************************************** Special Members:

template <typename T>
SharedPointer<T>::SharedPointer(T* ptr) : m_data(ptr)
{
    try
    {
        m_count = new size_t(1);
    }
    catch (std::bad_alloc& e)
    {
        delete ptr;
        throw e;
    }
}

template <typename T>
SharedPointer<T>::~SharedPointer()
{
    Detach();
}

template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer<T>& other) :
    m_data(other.m_data),
    m_count(other.m_count)
{
    ++(*m_count);
}

template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer<T>& other)
{
    SharedPointer<T> tmp(other);

    std::swap(m_data, tmp.m_data);
    std::swap(m_count, tmp.m_count);

    return *this;
}


// ******************************************************************* Members:

template <typename T>
bool SharedPointer<T>::operator==(const SharedPointer<T>& rhs) const
{
    return (m_data == rhs.m_data);
}

template <typename T>
bool SharedPointer<T>::operator!() const
{
    return (!m_data);
}

template <typename T>
T& SharedPointer<T>::operator*() const
{
    return *m_data;
}

template <typename T>
T* SharedPointer<T>::operator->() const
{
    return m_data;
}

template <typename T>
T* SharedPointer<T>::GetPtr() const
{
    return m_data;
}

template <typename T>
void SharedPointer<T>::Detach()
{
    if (0 == --(*m_count))
    {
        delete m_data;
        delete m_count;
    }
}

// template <typename T>
// template <typename TBase>
// SharedPointer<T>::operator SharedPointer<TBase>()
// {
//     TBase* tmp = m_data;
//     (void)tmp;
//
//     return *(reinterpret_cast<SharedPointer<TBase>*>(this));
// }

template <typename T>
template <typename TDerive>
SharedPointer<T>::SharedPointer(const SharedPointer<TDerive>& derive) :
    m_data(derive.m_data),
    m_count(derive.m_count)
{
    ++(*m_count);
}

}   // namespace hrd11

#endif // __HRD11_SHARED_POINTER_HPP__
