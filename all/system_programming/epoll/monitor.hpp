
#ifndef __HRD11_MONITOR_HPP__
#define __HRD11_MONITOR_HPP__

#include <cstddef>  // size_t

namespace hrd11
{

class Monitor
{
public:
    Monitor() = default;
    virtual ~Monitor() = 0;

    virtual void Add(int fd);
    virtual void Remove(int fd);
    virtual int Wait();
    virtual int operator[](size_t index);

private:

};

} //end namespace hrd11

#endif // __HRD11_MONITOR_HPP__
