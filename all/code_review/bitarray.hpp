#ifndef __HRD11_BITARRAY_HPP__
#define __HRD11_BITARRAY_HPP__

#include <vector>   // std::vec
#include <iosfwd>   // std::ostream
#include <cstddef>   // size_t
#include <functional> // binary_function
#include <algorithm>    // std::for_each
#include <iostream> //std::ostream

/*
    BitArray -

    Discription:    API for using BitArray with unlimited number of bits.
    Date:           29.12.19
    Folder name:    BitArray
    File names:     bitarray.hpp, bitarray_test.cpp

    Reviewer:       Yoav

    Ver 1.0
*/

namespace hrd11
{

template <size_t Size>
class BitArray;

class CountBitsOn
{
public:

    // TODO - 'iter'

    void operator() (size_t iter);
    size_t GetCount();
    void ResetCount();

private:
    static size_t m_bits_on;
};

size_t CountBitsOn::m_bits_on = 0;

/*****************************************************************/
class PrintBits
{
public:

    PrintBits(std::ostream& os) : m_os(os)
    {}
    void operator() (size_t iter);
    std::ostream& GetOstream();

private:
    std::ostream& m_os;
};

template <size_t Size>
std::ostream& operator << (std::ostream& os, const BitArray<Size>& arr);

/*********************************************************************/

struct AndBits
{
    size_t operator() (size_t iter1, size_t iter2);
};

struct OrBits
{
    size_t operator() (size_t iter1, size_t iter2);
};

struct XorBits
{
    size_t operator() (size_t iter1, size_t iter2);
};

// struct ShiftBits
// {
//     size_t operator() (size_t iter1, size_t iter2);
// };

/**********************************************************************/

template <size_t Size>
class BitArray
{
private:
    // Design pattern Proxy
    class BitProxy;

public:
    // Generated dtor, cctor, copy=
    BitArray();

    bool operator == (const BitArray& other) const;
    bool operator != (const BitArray& other) const;

    BitArray& operator &= (const BitArray& other);
    BitArray& operator |= (const BitArray& other);
    BitArray& operator ^= (const BitArray& other);
    BitArray& operator <<= (size_t shift);

    bool operator[] (size_t index) const;
    BitProxy operator[] (size_t index);

    size_t CountTrueBits() const;

    template <size_t Size2>
    friend std::ostream& operator << (std::ostream& os, const BitArray<Size2>& arr);

private:
    void SetBit(size_t index, bool value);
    bool GetBit(size_t index) const;

    // TODO - '64'

    static const size_t m_vec_size = Size / 64 + !!(Size % 64);
    std::vector<size_t> m_vec;

    // TODO - comment


    //size_t arr[vec_size];

};


/************************MEMBERS*********************************/
template <size_t Size>
BitArray<Size>::BitArray() : m_vec (m_vec_size, 0)
{}

template <size_t Size>
bool BitArray<Size>::operator == (const BitArray& other) const
{
    return (m_vec == other.m_vec);
}

template <size_t Size>
bool BitArray<Size>::operator != (const BitArray& other) const
{
    return (m_vec != other.m_vec);
}

template <size_t Size>
size_t BitArray<Size>::CountTrueBits() const
{
    CountBitsOn count;

    count.ResetCount();
    for_each(m_vec.begin(), m_vec.end(), count);

    return count.GetCount();
}

template <size_t Size>
BitArray<Size>& BitArray<Size>::operator &= (const BitArray& other)
{
    AndBits tmp;

    transform(m_vec.begin(), m_vec.end(),
    other.m_vec.begin(), m_vec.begin(), tmp);

    return *this;
}

template <size_t Size>
BitArray<Size>& BitArray<Size>::operator |= (const BitArray& other)
{
    OrBits tmp;

    transform(m_vec.begin(), m_vec.end(),
    other.m_vec.begin(), m_vec.begin(), tmp);

    return *this;
}

template <size_t Size>
BitArray<Size>& BitArray<Size>::operator ^= (const BitArray& other)
{
    XorBits tmp;

    transform(m_vec.begin(), m_vec.end(),
    other.m_vec.begin(), m_vec.begin(), tmp);

    return *this;
}

template <size_t Size>
typename BitArray<Size>::BitProxy BitArray<Size>::operator[] (size_t index)
{
    return BitProxy(*this, index);
}

template <size_t Size>
void BitArray<Size>::SetBit(size_t index, bool value)
{
    // TODO - '64'

    size_t tmp = m_vec[index / 64];
    size_t one = 1;

    // TODO - can do with no 'if'

    if (value)
    {
        m_vec[index / 64] = tmp | one << (index % 64);
    }
    else
    {
        m_vec[index / 64] = tmp & ~(one << (index % 64));
    }
}

template <size_t Size>
bool BitArray<Size>::operator[] (size_t index) const
{
    return GetBit(index);
}

template <size_t Size>
bool BitArray<Size>::GetBit(size_t index) const
{

    // TODO - 'maybe obj'

    size_t tmp = m_vec[index / 64];
    size_t one = 1;

    return tmp & (one << (index % 64));
}

// template <size_t Size>
// BitArray<Size>& BitArray<Size>::operator <<= (size_t shift)
// {
//     ShiftElement tmp;

//     transform(m_vec.begin(), m_vec[shift / 64],
//                 m_vec[shift/64], m_vec[shift / 64], tmp);
// }

/**************************NON_MEMBERS**************************/

template <size_t Size>
std::ostream& operator << (std::ostream& os, const BitArray<Size>& arr)
{
    // TODO - comment

    PrintBits tmp_os(os);
    for_each(arr.m_vec.begin(), arr.m_vec.end(), tmp_os);
    // transform (arr.m_vec.begin(), arr.m_vec.end(),
    //                       os, os, PrintBits);
    return os;
}

/**************************BITPROXY****************************/
template <size_t Size>
class BitArray<Size>::BitProxy
{
public:
    // Generated dtor
    // No cctor
    BitProxy(BitArray& arr, size_t index);

    // case --> b1[i] = s2[j]
    BitProxy& operator = (const BitProxy& other);

    // case--> bool value = s[i]
    operator bool() const;

    // case--> s[i] = true
    BitProxy& operator = (bool value);


private:
    BitArray& m_arr;
    size_t m_index;

};

template <size_t Size>
BitArray<Size>::BitProxy::BitProxy(BitArray<Size>& arr, size_t index) : m_arr(arr), m_index(index)
{}

template <size_t Size>
typename BitArray<Size>::BitProxy& BitArray<Size>::BitProxy::operator = (bool value)
{
    // TODO - kefel code

    m_arr.SetBit(m_index, value);

    return *this;
}

template <size_t Size>
typename BitArray<Size>::BitProxy& BitArray<Size>::BitProxy::operator = (const BitProxy& other)
{
    // TODO - kefel code
    m_arr.SetBit(m_index, other);

    return *this;
}

template <size_t Size>
BitArray<Size>::BitProxy::operator bool() const
{
    return m_arr.GetBit(m_index);
}

/*******************************COUNTBITSON***************************/

// CountBitsOn::CountBitsOn() : m_bits_on(0)
// {}

void CountBitsOn::operator() (size_t var)
{
    var = var - ((var >> 1) & 0x5555555555555555);
    var = (var & 0x3333333333333333) + ((var >> 2) & 0x3333333333333333);
    var = (var & 0x0F0F0F0F0F0F0F0F) + ((var >> 4) & 0x0F0F0F0F0F0F0F0F);
    var = (var & 0x00FF00FF00FF00FF) + ((var >> 8) & 0x00FF00FF00FF00FF);
    var = (var & 0x0000FFFF0000FFFF) + ((var >> 16) & 0x0000FFFF0000FFFF);

    m_bits_on += (var & 0x00000000FFFFFFFF) + ((var >> 32) & 0x00000000FFFFFFFF);
}

size_t CountBitsOn::GetCount()
{
    return m_bits_on;
}

void CountBitsOn::ResetCount()
{
    // TODO - why not in ctor?

    m_bits_on = 0;
}

/*******************************PRINTBITS***************************/

void PrintBits::operator() (size_t var)
{
    size_t one = 1;

    // TODO - '64' and '8'

    for (size_t i = 0; i < 64; ++i)
    {
        if (!(i % 8))
        {
            m_os<<'-';
        }
        m_os << !!(var & (one << (63 - i)));
    }
    m_os<<'\n';
}

std::ostream& PrintBits::GetOstream()
{
    return m_os;
}

/*********************************OPBITS**********************************/

size_t AndBits::operator () (size_t iter1, size_t iter2)
{
    return iter1 & iter2;
}

size_t OrBits::operator () (size_t iter1, size_t iter2)
{
    return iter1 | iter2;
}

size_t XorBits::operator () (size_t iter1, size_t iter2)
{
    return iter1 ^ iter2;
}

} // namespace hrd11

#endif // __HRD11_BITARRAY_HPP__
