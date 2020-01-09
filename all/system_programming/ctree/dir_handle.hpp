
#ifndef __HRD11_DIR_HUNDLE_HPP__
#define __HRD11_DIR_HUNDLE_HPP__

#include <sys/types.h>  // DIR*
#include <dirent.h>     // DIR*, struct dirent*

#include <string>       // std::string

namespace hrd11
{

class DirHandle
{
public:
    explicit DirHandle(const std::string& path);
    ~DirHandle();

    // can throw (std::runtime_error)
    void SafeClose();

    // can fail - return 0;
    const struct dirent* GetEntry() const;

    bool IsOpen() const;

private:
    // no cctor and copy=
    DirHandle(const DirHandle& other);
    DirHandle& operator=(const DirHandle& other);

    DIR* m_handle;
};

}   // namespace hrd11

#endif // __HRD11_DIR_HUNDLE_HPP__
