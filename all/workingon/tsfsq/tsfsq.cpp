

#include <cstddef>

#include "tsfsq.hpp"
#include "scopelock.hpp"
#include "semaphore.hpp"

namespace hrd11
{

template <size_t Size, typename T>
void Tsfsq<Size, T>::Wite(const T& value)
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
