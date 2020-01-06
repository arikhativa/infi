
#include <stdexcept>
#include <semaphore.h>
#include <cerrno>

#include "semaphore.hpp"

extern int errno;

namespace hrd11
{

Semaphore::Semaphore()
{
    // sem start at value '0'
    if (sem_init(&m_sem, 0, 0))
    {
        throw std::runtime_error("sem_init fail");
    }
}

Semaphore::~Semaphore()
{
    sem_destroy(&m_sem);
}

void Semaphore::Post()
{
    if (sem_post(&m_sem))
    {
        throw std::runtime_error("sem_post fail");
    }
}

void Semaphore::Wait()
{
    if (sem_wait(&m_sem))
    {
        throw std::runtime_error("sem_wait fail");
    }
}

void Semaphore::TryWait()
{
    int ret = sem_trywait(&m_sem);

    if (ret && EAGAIN != errno)
    {
        throw std::runtime_error("sem_trywait fail");
    }
}

void Semaphore::TimedWait(size_t time)
{
    struct timespec restrict;

    if (-1 == clock_gettime(CLOCK_REALTIME, &restrict))
    {
        throw std::runtime_error("sem_timedwait fail - clock_gettime() fail");
    }

    restrict.tv_sec += time;

    int ret = sem_timedwait(&m_sem, &restrict);

    if (ret && ETIMEDOUT != errno)
    {
        throw std::runtime_error("sem_timedwait fail");
    }
}

} // namespace hrd11
