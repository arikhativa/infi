
#ifndef __HRD11_EPOLL_HPP__
#define __HRD11_EPOLL_HPP__

#include <vector>       // std::vector

#include "monitor.hpp"

namespace hrd11
{

//  can throw(runtime_error) & throw(bad_alloc)

class Epoll : public Monitor
{
public:
    Epoll(size_t num_of_events = 2);
    virtual ~Epoll() override;

    virtual void Add(int fd) override;
    virtual void Remove(int fd) override;
    virtual int Wait() override;
    virtual int operator[](size_t index) override;

private:
    int m_epoll_fd;
    std::vector<struct epoll_event> m_events;

};

} //end namespace hrd11

#endif // __HRD11_EPOLL_HPP__
