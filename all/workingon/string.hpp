/*
    BSD

    String -

    Discription:    api for using strings, wrapper to char*
    Date:           10.12.19
    File names:     string.hpp, string.cpp, string_test.cpp

    Reviewer:       Shlomit

    Ver 1.0
*/

#ifndef __HRD11_String_HPP__
#define __HRD11_String_HPP__

#include <iostream> //istream, ostream
#include <cstddef>  //size_t

#include <cstring>  // strcpy(), strlen()


namespace hrd11
{

class String;

const String operator+(const String& lhs, const String& rhs);
bool operator==(const String& lhs, const String& rhs);
bool operator!=(const String& lhs, const String& rhs);
bool operator>(const String& lhs, const String& rhs);
bool operator<(const String& lhs, const String& rhs);

std::ostream& operator<<(std::ostream& os, const String& str);
std::istream& operator>>(std::istream& is, String& str);

class String
{
public:
    //non-explicit on purpose
    String(const char* str = "");
    ~String();
    String(const String& str);
    String& operator=(const String& other);

    String& operator+=(const String& other);

    const char& operator[](const size_t index) const;
    char& operator[](const size_t index);

    const char* ToCstring() const; //for backward compatibility, read-only
    size_t Length() const;

private:

    friend bool operator==(const String& lhs, const String& rhs);
    friend bool operator<(const String& lhs, const String& rhs);

    friend std::istream& operator>>(std::istream& is, String& str);

    char* m_str;
};


// ************************************************************ Special member:
String::String(const char* str) : m_str(new char[strlen(str) + 1])
{
    strcpy(m_str, str);
}

String::~String()
{
    delete[] m_str;
    m_str = 0;
}

String::String(const String& str)
{
    m_str = new char[strlen(str.m_str) + 1];
    strcpy(m_str, str.m_str);
}

String& String::operator = (const String& other)
{
    delete[] m_str;

    m_str = new char[strlen(other.m_str) + 1];
    strcpy(m_str, other.m_str);

    return *this;
}


// *********************************************************** Member function:
String& String::operator+=(const String& other)
{
    size_t new_len = strlen(m_str) + strlen(other.m_str) + 1;
    char *tmp = m_str;

    m_str = new char[new_len];

    strcpy(m_str, tmp);
    strcat(m_str, other.m_str);

    delete[] tmp;
    tmp = 0;

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
    char buffer[100] = {0};

    is >> buffer;

    str = buffer;

    std::cout << "\n\nnew str is\t";
    std::cout << str.m_str;


    return is;
}

}//namespace hrd11

#endif // __HRD11_String_HPP__
