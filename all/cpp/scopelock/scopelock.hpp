
#ifndef __HRD11_SCOPELOCK_HPP__
#define __HRD11_SCOPELOCK_HPP__

#include <mutex>
#include <stdexcept>

namespace hrd11
{

// Scopelock can throw(runtime_error), throw(system_error)
template <typename T>
class Scopelock
{
public:
    explicit Scopelock(T& mutex);
    ~Scopelock();

    void Lock();
    void Unlock();

private:
    // No cctor, copy=
    Scopelock(const Scopelock& other);
    Scopelock& operator=(const Scopelock& other);

    T& m_mutex;
};


// ********************************************************* Template Functoin:

template <typename T>
void Tamplate_Lock(T& mutex)
{
    mutex.lock();
}

template <>
void Tamplate_Lock(pthread_mutex_t& mutex)
{
    if (pthread_mutex_lock(&mutex))
    {
        throw std::runtime_error("fail to lock");
    }
}

template <typename T>
void Tamplate_Unlock(T& mutex)
{
    mutex.unlock();
}

template <>
void Tamplate_Unlock(pthread_mutex_t& mutex)
{
    if (pthread_mutex_unlock(&mutex))
    {
        throw std::runtime_error("fail to unlock");
    }
}

template <typename T>
void Tamplate_Dtor(T& mutex)
{
    (void)mutex;
}

template <>
void Tamplate_Dtor(pthread_mutex_t& mutex)
{
    if (pthread_mutex_destroy(&mutex))
    {
        throw std::runtime_error("fail to destroy");
    }
}


// *********************************************************** Member Functoin:

template <typename T>
Scopelock<T>::Scopelock(T& mutex) : m_mutex(mutex)
{
    Lock();
}

template <typename T>
Scopelock<T>::~Scopelock()
{
    try
    {
        Unlock();
        Tamplate_Dtor(m_mutex);
    }
    catch(std::runtime_error& e)
    {
        // TODO create log file.
    }
}

template <typename T>
void Scopelock<T>::Lock()
{
    Tamplate_Lock(m_mutex);
}

template <typename T>
void Scopelock<T>::Unlock()
{
    Tamplate_Unlock(m_mutex);
}

#endif // __HRD11_SCOPELOCK_HPP__

} // namespace hrd11
