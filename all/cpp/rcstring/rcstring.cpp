
#include "rcstring.hpp"

#include <iostream>  // istream, ostream

#include <cstddef>   // size_t
#include <cstring>   // strcpy(), strlen(), strcat()

namespace hrd11
{

class RCString::RCData
{
public:
    static RCData* CreateResources(const char* str);
    static RCData* CreateResources(const char* s1, const char* s2);
    void Increase();
    size_t Decrease();
    void Detach();      // This func will delete resources if it can.
    char* GetString();
    void SetChar(size_t index, char ch);
    bool IsOwner();

private:
    // non ctor, cctor or copy= on purpose.
    RCData(const char *str);
    RCData(const char *s1, const char *s2);
    ~RCData();

    size_t m_count;
    char m_str[1];
};

// **************************************************************** Global var:
static const size_t RC_METADATA = sizeof(size_t);
static const size_t s_buff_size = 10;   // 10 is a random number


// ********************************** CharProxy *******************************

// ************************************************************* Public member:
RCString::CharProxy::CharProxy(RCString& str, size_t index) :
    m_str(str),
    m_index(index)
{}

RCString::CharProxy& RCString::CharProxy::operator = (const CharProxy& other)
{
    *this = static_cast<char>(other);

    return *this;
}

RCString::CharProxy::operator char() const
{
    return m_str.GetChar(m_index);
}

RCString::CharProxy& RCString::CharProxy::operator = (char c)
{
    m_str.SetChar(m_index, c);

    return *this;
}


// ************************************ RCData ********************************

// ************************************************************* Public Member:
RCString::RCData* RCString::RCData::CreateResources(const char* str)
{
    size_t len = strlen(str) + RC_METADATA + sizeof('\0');
    RCData* ret = static_cast<RCData*>(::operator new (len));

    new (ret) RCData(str);

    return ret;
}

RCString::RCData* RCString::RCData::CreateResources(const char* s1, const char* s2)
{
    RCData* ret;
    size_t len = strlen(s1) + strlen(s2) + RC_METADATA + sizeof('\0');

    ret = static_cast<RCData*>(::operator new (len));

    new (ret) RCData(s1, s2);

    return ret;
}

void RCString::RCData::Increase()
{
    ++m_count;
}

size_t RCString::RCData::Decrease()
{
    return --m_count;
}

char* RCString::RCData::GetString()
{
    return m_str;
}

void RCString::RCData::Detach()  // This func will delete resources if it can.
{
    if (!Decrease())
    {
        delete this;
    }
}

void RCString::RCData::SetChar(size_t index, char ch)
{
    GetString()[index] = ch;
}

bool RCString::RCData::IsOwner()
{
    return (m_count == 1);
}


// ************************************************************ Private Member:
RCString::RCData::RCData(const char* str) : m_count(1)
{
    char *tmp = reinterpret_cast<char*>(&m_str);

    strcpy(tmp, str);
}

RCString::RCData::RCData(const char* s1, const char* s2) : m_count(1)
{
    char *tmp = reinterpret_cast<char*>(&m_str);

    strcpy(tmp, s1);
    strcat(tmp, s2);
}

RCString::RCData::~RCData()
{}


// ****************************************************************************
// ********************************** RCString ********************************
// ****************************************************************************

// ************************************************************* Public Member:
RCString::RCString(const char* str) : m_rc(RCData::CreateResources(str))
{}

RCString::RCString(const RCString& other) : m_rc(other.m_rc)
{
    m_rc->Increase();
}

RCString::~RCString()
{
    m_rc->Detach();  // This func will delete resources if it can.
    m_rc = 0;
}

RCString& RCString::operator=(const RCString& other)
{
    if (this != &other)
    {
        // RCString tmp(other);
        // std::swap(*m_rc, tmp.);

        m_rc->Detach();  // This func will delete resources if it can.
        m_rc = other.m_rc;
        m_rc->Increase();
    }

    return *this;
}

RCString& RCString::operator+=(const RCString& other)
{
    RCData* tmp = RCData::CreateResources(m_rc->GetString(), other.m_rc->GetString());

    m_rc->Detach();  // This func will delete resources if it can.

    m_rc = tmp;

    return *this;
}

const char& RCString::operator[](const size_t index) const
{
    return ToCstring()[index];
}

RCString::CharProxy RCString::operator[](const size_t index)
{
    return CharProxy(*this, index);
}

const char* RCString::RCString::ToCstring() const
{
    return m_rc->GetString();
}

size_t RCString::Length() const
{
    return strlen(ToCstring());
}


// ************************************************************ Private Member:
void RCString::SetChar(size_t index, char ch)
{
    if (!m_rc->IsOwner())
    {
        m_rc->Decrease();

        RCData* new_data = RCData::CreateResources(m_rc->GetString());
        m_rc = new_data;
    }

    m_rc->SetChar(index, ch);
}

char RCString::GetChar(size_t index)
{
    return m_rc->GetString()[index];
}


// ***************************************************** Operators overloading:
const RCString operator+(const RCString& lhs, const RCString& rhs)
{
    return RCString(lhs) += rhs;
}

bool operator==(const RCString& lhs, const RCString& rhs)
{
    return !(strcmp(lhs.ToCstring(), rhs.ToCstring()));
}

bool operator!=(const RCString& lhs, const RCString& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const RCString& lhs, const RCString& rhs)
{
    int tmp = strcmp(lhs.ToCstring(), rhs.ToCstring());

    return (tmp < 0);
}

bool operator>(const RCString& lhs, const RCString& rhs)
{
    int tmp = strcmp(lhs.ToCstring(), rhs.ToCstring());

    return (tmp > 0);
}


// ************************************************************* I/O Operators:
std::ostream& operator<<(std::ostream& os, const RCString& str)
{
    os << str.ToCstring();

    return os;
}

std::istream& operator>>(std::istream& is, RCString& str)
{
    char buffer[s_buff_size] = {0};
    RCString tmp;

    while (is.get(buffer, s_buff_size, '\n'))
    {
        tmp += buffer;
    }

    is.clear();
    is.ignore(1,'\n');

    str = tmp;

    return is;
}

} //namespace hrd11
