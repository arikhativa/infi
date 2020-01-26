
#include <cstddef>		    // size_t
#include <sys/ioctl.h>		// ioctl()

#include <stdexcept>		// runtime_error
#include <string>		// runtime_error

#include "ioctl_wrapper.hpp"

namespace hrd11
{

IoctlError::IoctlError(int fd, size_t macro, ssize_t flags) :
    std::runtime_error("IoctlError"),
    m_fd(fd),
    m_macro(macro),
    m_flags(flags),
    m_error("-- IoctlError:\n")
{}

const char* IoctlError::what() const throw ()
{
    std::string fd(std::to_string(m_fd));
    std::string macro(std::to_string(m_macro));
    std::string flags("NO_FLAGS");

    if (NO_FLAGS != m_flags)
    {
        flags = std::to_string(m_flags);
    }

    m_error += "-- fd\t\t" + fd + "\n-- macro\t" + macro
                    + "\n-- flags\t" + flags + "\n";

    return m_error.c_str();
}




#include <linux/nbd.h>		// NBD macros


void Ioctl(int fd, size_t macro, ssize_t flags)
{
    int stt = 0;

    stt = ioctl(fd, macro, flags);

    // if (macro == NBD_SET_SOCK)
    if (stt)
    {
        throw IoctlError(fd, macro, flags);
    }

}

} // end namespace hrd11
