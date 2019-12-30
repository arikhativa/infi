/*
    BSD

    RCString -

    Discription:    api for using RCStrings, wrapper to char*
    Date:           24.12.19
    File names:     rcstring.hpp, rcstring.cpp, rcstring_test.cpp

    Reviewer:

    Ver 1.0
*/

#ifndef __HRD11_RCString_HPP__
#define __HRD11_Efficient_RCString_HPP__

#include <iostream> //iostream, ostream
#include <cstddef> //size_t

namespace hrd11
{

class RCString;

const RCString operator + (const RCString& lhs, const RCString& rhs);
bool operator == (const RCString& lhs, const RCString& rhs);
bool operator != (const RCString& lhs, const RCString& rhs);
bool operator > (const RCString& lhs, const RCString& rhs);
bool operator < (const RCString& lhs, const RCString& rhs);

std::ostream& operator << (std::ostream& os, const RCString& str);
std::istream& operator >> (std::istream& is, RCString& str);

class RCString
{
private:
    class CharProxy;
    class RCData;

public:
    //non-explicit on purpose
    RCString(const char* str = "");
    ~RCString();
    RCString(const RCString& other);
    RCString& operator = (const RCString& other);

    RCString& operator += (const RCString& other);

    const char& operator[] (size_t index) const;
    CharProxy operator[] (size_t index);

    const char* ToCString() const; //for backward compatibility, read-only
    size_t Length() const;

private:
    RCData* m_rc;
};

// using Proxy pattern
class RCString::CharProxy
{
public:
    // using generated cctor & dtor

    CharProxy(RCString& str, size_t index);

    // case -->  s1[i] = s2[j]
    CharProxy& operator = (const CharProxy& other);

    // case--> char a = s[i]
    operator char() const;

    //case--> s[i] = 'a'
    char operator = (char c);

private:
    RCString& m_str;
    size_t m_index;
};

}//namespace hrd11

#endif // __HRD11_RCString_HPP__
