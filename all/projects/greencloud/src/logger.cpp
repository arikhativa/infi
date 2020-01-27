
#include <sys/types.h>  // stat()
#include <sys/stat.h>   // stat()
#include <unistd.h>     // stat()

#include <mutex>        // std::mutex
#include <iostream>     // std::fstream
#include <fstream>      // std::fstream

#include "logger.hpp"

using namespace hrd11;


inline bool IsFileExists(const std::string& name)
{
    struct stat buffer;

    return (stat(name.c_str(), &buffer) == 0);
}

Logger::Logger(const std::string& path_to_dir, Loglvl lvl) : m_lvl(lvl)
{
    size_t num = 1;
    std::string file_name = path_to_dir + std::to_string(num);

    while (IsFileExists(file_name))
    {
        ++num;
        file_name = path_to_dir + std::to_string(num);
    }

    m_file.open(file_name);

    if (!m_file.is_open())
    {
        // throw()?
    }
}

void Logger::Write(const std::string& msg,
                    const std::string& src_file, int line) const
{
    (void)msg;
    (void)src_file;
    (void)line;
}
