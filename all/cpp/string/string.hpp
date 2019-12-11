/*
    BSD

    String -

    Discription:    api for using strings, wrapper to char*
    Date:           10.12.19
    File names:     string.hpp, string.cpp, string_test.cpp

    Reviewer:       Shlomit

    ver 1.4 removed friend decleration of operators functions from class String
    Ver 1.3 removed ostream friend declaration at private section of class String
    Ver 1.2 removed const from non-member functions with return type bool
    Ver 1.1 changed int to size_t at operator[]
    Ver 1.0
*/

#ifndef __HRD11_String_HPP__
#define __HRD11_String_HPP__

#include <iosfwd> //iostream, ostream
#include <cstddef> //size_t

namespace hrd11
{

class String;

const String operator + (const String& lhs, const String& rhs);
bool operator == (const String& lhs, const String& rhs);
bool operator != (const String& lhs, const String& rhs);
bool operator > (const String& lhs, const String& rhs);
bool operator < (const String& lhs, const String& rhs);

std::ostream& operator << (std::ostream& os, const String& str);
std::istream& operator >> (std::istream& is, String& str);

class String
{
public:
    //non-explicit on purpose
    String(const char* str = "");
    ~String();
    String(const String& other);
    String& operator = (const String& other);

    String& operator += (const String& other);

    const char& operator[] (const size_t index) const;
    char& operator[] (const size_t index);

    const char* ToCstring() const; //for backward compatibility, read-only
    size_t Length() const;

private:
    String(size_t combained_len, const char* s1, const char* s2);
    static char* CreateResource(const char* src);

    char* m_str;
};

}//namespace hrd11

#endif // __HRD11_String_HPP__
