
#ifndef __HRD11_SEMAPHORE_HPP__
#define __HRD11_SEMAPHORE_HPP__

#include <semaphore.h>

namespace hrd11
{

// all functions can throw(runtime_error)
class Semaphore
{
public:
    Semaphore();
    ~Semaphore();

    void Post();
    void Wait();
    void TryWait();
    void TimedWait(size_t time);

private:
    // No cctor, copy=
    Semaphore(const Semaphore& other);
    Semaphore& operator=(const Semaphore& other);

    sem_t m_sem;
};

#endif // __HRD11_SEMAPHORE_HPP__

} // namespace hrd11
