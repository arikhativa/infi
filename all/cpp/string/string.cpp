
#include "string.hpp"

#include <iostream>  // istream, ostream

#include <cstddef>  // size_t
#include <cstring>  // strcpy(), strlen(), strcat()

namespace hrd11
{
// **************************************************************** Global var:
static const size_t s_buff_size = 10;   // 10 is a random number


// ************************************************************ Special member:
inline char* String::CreateResource(const char* src)
{
    char* tmp = new char[strlen(src) + 1];

    return strcpy(tmp, src);
}

String::String(const char* str) : m_str(CreateResource(str))
{}

String::String(size_t combained_len, const char* s1, const char* s2) : m_str(new char[combained_len])
{
    strcpy(m_str, s1);
    strcat(m_str, s2);
}

String::String(const String& other) : m_str(CreateResource(other.m_str))
{}

String::~String()
{
    delete[] m_str;
    m_str = 0;
}

String& String::operator=(const String& other)
{
    if (this != &other)
    {
        String tmp(other);
        std::swap(tmp.m_str, m_str);
    }

    return *this;
}


// *********************************************************** Member function:
String& String::operator+=(const String& other)
{
    size_t new_len = strlen(m_str) + strlen(other.m_str) + 1;
    String tmp(new_len, m_str, other.m_str);

    std::swap(tmp.m_str, m_str);

    return *this;
}

const char& String::operator[](const size_t index) const
{
    return m_str[index];
}

char& String::operator[](const size_t index)
{
    return m_str[index];
}

const char* String::ToCstring() const
{
    return m_str;
}

size_t String::Length() const
{
    return strlen(m_str);
}


// ***************************************************** Operators overloading:
const String operator+(const String& lhs, const String& rhs)
{
    return String(lhs) += rhs;
}

bool operator==(const String& lhs, const String& rhs)
{
    return !(strcmp(lhs.ToCstring(), rhs.ToCstring()));
}

bool operator!=(const String& lhs, const String& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const String& lhs, const String& rhs)
{
    int tmp = strcmp(lhs.ToCstring(), rhs.ToCstring());

    return (tmp < 0);
}

bool operator>(const String& lhs, const String& rhs)
{
    int tmp = strcmp(lhs.ToCstring(), rhs.ToCstring());

    return (tmp > 0);
}


// ************************************************************* I/O Operators:
std::ostream& operator<<(std::ostream& os, const String& str)
{
    os << str.ToCstring();

    return os;
}

std::istream& operator>>(std::istream& is, String& str)
{
    char buffer[s_buff_size] = {0};
    String tmp;

    while (is.get(buffer, s_buff_size, '\n'))
    {
        tmp += buffer;
    }

    is.clear();
    is.ignore(1,'\n');

    str = tmp;

    return is;
}

}//namespace hrd11
