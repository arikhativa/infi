
#ifndef __HRD11_SEMAPHORE_HPP__
#define __HRD11_SEMAPHORE_HPP__

#include <semaphore.h>

class Semaphore
{
public:
    explicit Semaphore(sem_t sem);
    ~Semaphore();

    void Post();
    void Wait();
    void TryWait();
    void TimedWait();

private:
    // No cctor, copy=
    Semaphore(const Semaphore& other);
    Semaphore& operator=(const Semaphore& other);

    sem_t m_sem;
};

#endif // __HRD11_SEMAPHORE_HPP__
