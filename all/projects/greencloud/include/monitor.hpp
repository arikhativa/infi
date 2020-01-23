
#ifndef __HRD11_MONITOR_HPP__
#define __HRD11_MONITOR_HPP__

#include <cstddef>  // size_t

namespace hrd11
{

class Monitor
{
public:
    Monitor() = default;
    virtual ~Monitor() = default;

    virtual void Add(int fd) = 0;
    virtual void Remove(int fd) = 0;
    virtual int Wait() = 0;
    virtual int operator[](size_t index) = 0;

private:

};

} //end namespace hrd11

#endif // __HRD11_MONITOR_HPP__
