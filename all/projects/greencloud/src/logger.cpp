
#include <sys/types.h>  // stat()
#include <sys/stat.h>   // stat()
#include <unistd.h>     // stat()
#include <string.h>     // strlen()

#include <mutex>        // std::mutex
#include <iostream>     // std::fstream
#include <fstream>      // std::fstream
#include <chrono>       // std::chrono::system_clock::now()
#include <ctime>        // std::ctime()

#include "logger.hpp"

namespace hrd11
{

inline bool IsFileExists(const std::string& name)
{
    struct stat buffer;

    return (stat(name.c_str(), &buffer) == 0);
}


Logger::Logger(const std::string& path_and_filename, Loglvl lvl) : m_lvl(lvl)
{
    size_t num = 1;
    std::string file_name = path_and_filename + std::to_string(num);

    // while (IsFileExists(file_name))
    // {
    //     ++num;
    //     file_name = path_and_filename + std::to_string(num);
    // }

    m_file.open(file_name);

    if (!m_file.is_open())
    {
        printf("-- cant open file \n");
        exit(1);
    }

    m_file << "Data & Time" << "\t\t\t";
    m_file << "ID" << "\t";
    m_file << "Source File" << "\t\t";
    m_file << "Line" << "\t";
    m_file << "Type" << "\t";
    m_file << "Massage" << "\n";
}

Logger::~Logger()
{
    m_file.close();
}


void Logger::Write( Loglvl lvl,
                    const std::string& msg,
                    const std::string& src_file,
                    int line)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (lvl <= m_lvl)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t full_data = std::chrono::system_clock::to_time_t(now);
        char *tmp = std::ctime(&full_data);
        size_t len = strlen(tmp);
        tmp[len - 1] = '\0';

        m_file << tmp << "\t";
        m_file << getpid() << "\t";
        m_file << src_file << "\t\t";
        m_file << line << "\t";
        switch (lvl)
        {
            case LOG_ERROR:
                m_file << "Error" << "\t";
                break ;
            case LOG_INFO:
                m_file << "Info" << "\t";
                break ;
            case LOG_DEBUG:
                m_file << "Debug" << "\t";
                break ;
        }
        m_file << msg << "\n";
    }
}

}   // end namespace hrd11
