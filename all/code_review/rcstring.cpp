#include <cstring>
#include "rcstring.hpp"

namespace hrd11
{

class RCString::RCData
{
public:


    static RCData* CreateResources(const char* str);
    void ReleaseResources();
    void Increase();
    size_t Decrease();


    char* GetStr();
    char SetChar(char c, size_t index);
    bool IsSingle();


private:
    // no cctor or cpy= on purpose.
    RCData(const char *str);
    ~RCData();

    size_t m_count;
    char m_str[1];
};


/*--------------------------------RCString-----------------------------------*/

RCString::RCString(const char* str) : m_rc(RCData::CreateResources(str))
{}

RCString::~RCString()
{
    m_rc->ReleaseResources();
}

RCString::RCString(const RCString& other):m_rc(other.m_rc)
{
    m_rc->Increase();
}

RCString& RCString::operator = (const RCString& other)
{
    if(this != &other)
    {
        m_rc->ReleaseResources();
        m_rc = other.m_rc;

        m_rc->Increase();
    }

    return *this;
}

void RCString::RCData::ReleaseResources()
{
    if(!Decrease())
    {
        delete this;
        // m_rc = 0;
    }
}

RCString& RCString::operator += (const RCString& other)
{
    // strlen many times
    // problem with cpy && cat.
    size_t combined_len = other.Length() + Length() + 1;//+1 for null termination
    char *str = new char[combined_len];

    strcpy(str, m_rc->GetStr());
    strcat(str, other.m_rc->GetStr());

    RCData *tmp = RCString::RCData::CreateResources(str);
    m_rc->ReleaseResources();
    m_rc = tmp;

    delete[] str;

    return *this;
}

const char& RCString::operator[] (size_t index) const
{
    return ToCString()[index];
}

RCString::CharProxy RCString::operator[] (size_t index)
{
    return RCString::CharProxy(*this, index);
    // RCString::CharProxy tmp(*this, index);
    // return tmp;
}


const char* RCString::ToCString() const
{
    return m_rc->GetStr();
}

size_t RCString::Length() const
{
    return strlen(ToCString());
}


/*---------------------------------RCData------------------------------------*/

RCString::RCData* RCString::RCData::CreateResources(const char* str)
{
    // maybe global 'sizeof(size_t)'
    RCData* tmp = static_cast<RCData*>(::operator new (strlen(str) + sizeof(size_t) + 1));
    new (tmp) RCData(str);

    return tmp;
}

RCString::RCData::RCData(const char* str):m_count(1)
{
    strcpy(m_str, str);
}

RCString::RCData::~RCData()
{}

void RCString::RCData::Increase()
{
    ++m_count;
}

size_t RCString::RCData::Decrease()
{
    return --m_count;
}

char* RCString::RCData::GetStr()
{
    return m_str;
}

bool RCString::RCData::IsSingle()
{
    return m_count == 1;
}

char RCString::RCData::SetChar(char c, size_t index)
{
    m_str[index] = c;

    return c;
}

/*---------------------------------CharProxy------------------------------------*/

RCString::CharProxy::CharProxy(RCString& str, size_t index):m_str(str), m_index(index)
{}

char RCString::CharProxy::operator = (char c)
{
    if(!m_str.m_rc->IsSingle())
    {
        RCData *tmp = RCString::RCData::CreateResources(m_str.ToCString());
        m_str.m_rc->ReleaseResources();
        m_str.m_rc = tmp;
    }

    return m_str.m_rc->SetChar(c, m_index);
}

RCString::CharProxy::operator char() const
{
    return m_str.ToCString()[m_index];
}

RCString::CharProxy& RCString::CharProxy::operator = (const CharProxy& other)
{
    // hmm 'm_str[m_index]'
    m_str[m_index] = other; //other converted to char through operator char()

    return *this;
}

/*----------------------------- Non Member Operators ------------------------*/

const RCString operator + (const RCString& lhs, const RCString& rhs)
{
    return RCString(lhs) += rhs;
}

bool operator == (const RCString& lhs, const RCString& rhs)
{
    return !strcmp(lhs.ToCString(), rhs.ToCString());
}

bool operator != (const RCString& lhs, const RCString& rhs)
{
    return !(lhs == rhs);
}

bool operator > (const RCString& lhs, const RCString& rhs)
{
    return strcmp(lhs.ToCString(), rhs.ToCString()) > 0;
}

bool operator < (const RCString& lhs, const RCString& rhs)
{
    return strcmp(lhs.ToCString(), rhs.ToCString()) < 0;
}

//******* non member functions - iostream *******
static const size_t g_buf_size = sizeof(size_t);

std::ostream& operator << (std::ostream& os, const RCString& str)
{
    return os<<str.ToCString();
}

std::istream& operator >> (std::istream& is, RCString& str)
{
    char buffer[g_buf_size] = {0};
    RCString tmp = str;

    while(is.get(buffer,g_buf_size,'\n'))
    {
        tmp += buffer;
    }

    is.clear();
    is.ignore(1,'\n');
    str = tmp;

    return is;
}

} //namespace hrd11
