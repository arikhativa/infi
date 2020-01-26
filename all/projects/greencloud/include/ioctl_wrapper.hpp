
#ifndef __HRD11_IOCTL_HPP__
#define __HRD11_IOCTL_HPP__

#include <sys/types.h>      // ssize_t
#include <cstddef>	     	// size_t
#include <sys/ioctl.h>		// ioctl()

#include <stdexcept>		// runtime_error
#include <string>           // std::string, to_string

namespace hrd11
{

class IoctlError : public std::runtime_error
{
public:
    static const int NO_FLAGS = -1;

    IoctlError(int fd, size_t macro, ssize_t flags);

    virtual const char* what() const throw () override;

private:

    int m_fd;
    size_t m_macro;
    ssize_t m_flags;
    mutable std::string m_error;
};

//  can throw (IoctlError) inherted from (std::runtime_error)

void Ioctl(int fd, size_t macro, ssize_t flags);

} // end namespace hrd11

#endif // __HRD11_IOCTL_HPP__
