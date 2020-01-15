
#include <sys/types.h>  // DIR*
#include <dirent.h>     // DIR*, struct dirent*

#include <stdexcept>     // std::runtime_error
#include <string>        // std::string

#include "dir_handle.hpp"

namespace hrd11
{

// ************************************ Declartion ****************************
static DIR* InitHandle(const std::string& path);

// ************************************ DirHundel *****************************

DirHandle::DirHandle(const std::string& path) : m_handle(InitHandle(path))
{}

DirHandle::~DirHandle()
{
    if (m_handle)
    {
        closedir(m_handle);
    }
}

const struct dirent* DirHandle::GetEntry() const
{
    if (m_handle)
    {
        return readdir(m_handle);
    }

    return 0;
}

bool DirHandle::IsOpen() const
{
    return !!(m_handle);
}

// ************************************ Static Functions **********************

static DIR* InitHandle(const std::string& path)
{
    DIR* handel = opendir(path.c_str());

    return handel;
}

}   // namespace hrd11
