
#ifndef __HRD11_LOGGER_HPP__
#define __HRD11_LOGGER_HPP__

#include <mutex>        // std::mutex
#include <iostream>     // std::fstream
#include <fstream>      // std::fstream

namespace hrd11
{

enum Loglvl
{
    ERORR = 0,
    INFO,
    DEBUG
};

class Logger
{
public:
    ~Logger() = default;
    void Write(const std::string& msg, const std::string& src_file, int line) const;

private:
    Logger(const std::string& path_to_dir, Loglvl lvl);

    std::mutex m_mutex;
    std::ofstream m_file;
    Loglvl m_lvl;

};

}   // end namespace hrd11

#endif // __HRD11_LOGGER_HPP__
