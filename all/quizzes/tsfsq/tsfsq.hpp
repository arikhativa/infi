


#ifndef __HRD11_TSFSQ_HPP__
#define __HRD11_TSFSQ_HPP__

#include <cstddef>
#include <mutex>

#include "semaphore.hpp"
#include "scopelock.hpp"

namespace hrd11
{

template <size_t Size, typename T>
class Tsfsq
{
public:
    Tsfsq();

    void Write(const T& value);
    T Read();

private:
    std::mutex m_mutex_read;
    std::mutex m_mutex_write;
    Semaphore m_sem_read;
    Semaphore m_sem_write;
    size_t m_index_write;
    size_t m_index_read;
    T m_cbuff[Size];

};


template <size_t Size, typename T>
Tsfsq<Size, T>::Tsfsq() : m_index_write(0), m_index_read(0)
{
    for (size_t i = 0; i < Size; ++i)
    {
        m_sem_write.Post();
    }
}

template <size_t Size, typename T>
void Tsfsq<Size, T>::Write(const T& value)
{
    Scopelock<std::mutex> lock(m_mutex_write);

    m_sem_write.Wait();
    m_cbuff[m_index_write % Size] = value;
    ++m_index_write;
    m_sem_read.Post();
}

template <size_t Size, typename T>
T Tsfsq<Size, T>::Read()
{
    Scopelock<std::mutex> lock(m_mutex_read);

    m_sem_read.Wait();
    T ret = m_cbuff[m_index_read % Size];
    ++m_index_read;
    m_sem_write.Post();

    return ret;
}

} // namespace hrd11

#endif // __HRD11_TSFSQ_HPP__
