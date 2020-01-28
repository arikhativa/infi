
#ifndef __HRD11_LOGGER_HPP__
#define __HRD11_LOGGER_HPP__

#include <mutex>        // std::mutex
#include <iostream>     // std::fstream
#include <fstream>      // std::fstream

#include "handleton.hpp"

namespace hrd11
{

enum Loglvl
{
    LOG_ERROR = 0,
    LOG_INFO,
    LOG_DEBUG
};

class Logger
{
public:
    ~Logger();
    void Write(Loglvl lvl, const std::string& msg, const std::string& src_file, int line);

private:
    template<typename T>
    friend class hrd11::Handleton;

    Logger(const std::string& path_and_filename, Loglvl lvl);

    std::mutex m_mutex;
    std::ofstream m_file;
    Loglvl m_lvl;

};

}   // end namespace hrd11

#endif // __HRD11_LOGGER_HPP__
