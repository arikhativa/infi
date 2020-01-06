
#include <semaphore.h>
#include <stdexcept>

namespace hrd11
{
class Sem
{
public:
    Sem();
    ~Sem();

    void Wait();
    void Post();
    void TryWait();
    void TimedWait(time_t time, long sec);
    void GetVal(int *val);

private:
    // TODO: cctor & copy=
    sem_t m_sem;
};

Sem::Sem()
{
    if(-1 == sem_init(&m_sem, 0, 0))
    {
        throw std::runtime_error("init fail");
    }
}

Sem::~Sem()
{
    sem_destroy(&m_sem); //failure ignored.
}

void Sem::Wait()
{
    if(-1 == sem_wait(&m_sem))
    {
        throw std::runtime_error("wait fail");
    }
}

void Sem::Post()
{
    if(-1 == sem_post(&m_sem))
    {
        throw std::runtime_error("post fail");
    }
}

void Sem::TryWait()
{
    if(-1 == sem_trywait(&m_sem))
    {
        throw std::runtime_error("try wait fail");
    }
}

void Sem::TimedWait(time_t sec, long nanosec)
{
    struct timespec t;
    t.tv_sec = sec;
    t. tv_nsec = nanosec;

    if(-1 == sem_timedwait(&m_sem, &t))
    {
        throw std::runtime_error("timedwait fail");
    }
}

void Sem::GetVal(int *val)
{
    if(-1 == sem_getvalue(&m_sem, val))
    {
        throw std::runtime_error("get val fail");

    }
}

}
