

#include <sys/epoll.h>  // epoll
#include <unistd.h>     // close()
#include <errno.h>      // errno

#include <stdexcept>  // runtime_rerror

#include "epoll.hpp"

namespace hrd11
{

Epoll::Epoll(size_t num_of_events) :
    m_epoll_fd(epoll_create1(0)),
    m_events(num_of_events, {0, 0})
{
    errno = 0;

    if (-1 == m_epoll_fd)
    {
        std::string err = "errno == ";
        err += std::to_string(errno);
        throw std::runtime_error(err);
    }
}

Epoll::~Epoll()
{
    close(m_epoll_fd);
}

void Epoll::Add(int fd)
{
    struct epoll_event event = {0, 0};
    int stt = 0;

    errno = 0;

    event.events = EPOLLIN;
    event.data.fd = fd;

    stt = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event);
    if (-1 == stt)
    {
        std::string err = "errno == ";
        err += std::to_string(errno);
        throw std::runtime_error(err);
    }
}

void Epoll::Remove(int fd)
{
    int stt = 0;

    errno = 0;

    stt = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    if (-1 == stt)
    {
        std::string err = "errno == ";
        err += std::to_string(errno);
        throw std::runtime_error(err);
    }
}

int Epoll::Wait()
{
    int ret = 0;
    
    errno = 0;

    ret = epoll_wait(m_epoll_fd, m_events.data(), 1, -1);

    if (-1 == ret)
    {
        std::string err = "errno == ";
        err += std::to_string(errno);
        throw std::runtime_error(err);
    }

    return m_events[0].data.fd;
}

int Epoll::operator[](size_t index)
{
    return m_events[index].data.fd;
}

} //end namespace hrd11
