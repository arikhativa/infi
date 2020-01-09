


#ifndef __HRD11_TSFSQ_HPP__
#define __HRD11_TSFSQ_HPP__

#include <cstddef>
#include <mutex>

#include "semaphore.hpp"

namespace hrd11
{

template <size_t Size, typename T>
class Tsfsq
{
public:
    Tsfsq();
    ~Tsfsq();

    void Wite(const T& value);
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

} // namespace hrd11

#endif // __HRD11_TSFSQ_HPP__
