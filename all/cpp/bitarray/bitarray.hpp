
/*
    BitArray -

    Discription:    API for using BitArray with unlimited number of bits.
    Date:           29.12.19
    Folder name:    BitArray
    File names:     BitArray.hpp, bitarray_test.cpp

    Reviewer:       Aviv

    Ver 1.0
*/

#ifndef __HRD11_BITARRAY_HPP__
#define __HRD11_BITARRAY_HPP__

// This macro if for the s_lut[] - needed for CountTrueBits()
#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
#define COUNT_BITS B6(0), B6(1), B6(1), B6(2)

#define YELLOW  "\x1b[33m"
#define GRAY    "\x1b[2m"
#define RESET   "\x1b[0m"

#include <vector>       // std::vec
#include <iostream>     // std::ostream
#include <cstddef>      // size_t
#include <algorithm>    // transform(), for_each(), copy_backwoard(), fill()

namespace hrd11
{


// **************************************************************** Global var:
static const size_t SIZE_OF_LUT = 256;
static const size_t SIZE_T_BITS = 64;
static const unsigned char s_lut[SIZE_OF_LUT] = { COUNT_BITS };


// ********************************************************** Class Declartion:
template <size_t Size>
class BitArray;

template <size_t Size>
std::ostream& operator << (std::ostream& os, const BitArray<Size>& arr);

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

    template <size_t S>
    friend std::ostream& operator << (std::ostream& os, const BitArray<S>& arr);

private:
    void SetBit(size_t index, bool value);
    bool GetBit(size_t index) const;

    static const size_t s_vec_size = (Size / (SIZE_T_BITS + 1)) + 1;
    std::vector<size_t> m_vec;

};

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


// ********************************************************* Struct Declartion:
struct ArrayIndex
{
    ArrayIndex(size_t index) :
    m_arr_index(index / SIZE_T_BITS),
    m_bit_index(index % SIZE_T_BITS)
    {}

    size_t m_arr_index;
    size_t m_bit_index;
};


// ******************************************************** Functor Declartion:
class FunctorCount
{
public:
    FunctorCount();
    void operator()(size_t byte);
    size_t GetCount();

private:
    size_t m_count;

};

class FunctorPrint
{
public:
    FunctorPrint(size_t size, std::ostream& os);
    void operator()(size_t byte);

private:
    size_t m_size;
    std::ostream& m_os;

};

class FunctorShiftBitsLeft
{
public:
    FunctorShiftBitsLeft(size_t shift);
    void operator()(size_t& byte);

private:
    size_t m_shift;
    size_t m_overflow;

};


// ******************************* Declartion *********************************

// ******************************* Static Funcs *******************************
static void PrintWordSize(size_t byte,  std::ostream& os);
static void PrintByBit(size_t byte, size_t* size, std::ostream& os);
static size_t FuncAND(size_t byte1, size_t byte2);
static size_t FuncOR(size_t byte1, size_t byte2);
static size_t FuncXOR(size_t byte1, size_t byte2);


// ******************************* BitArray ***********************************

// *********************************************************** BitArray Public:
template <size_t Size>
BitArray<Size>::BitArray() : m_vec(s_vec_size, 0)
{}

template <size_t Size>
bool BitArray<Size>::operator== (const BitArray& other) const
{
    return m_vec == other.m_vec;
}

template <size_t Size>
bool BitArray<Size>::operator!= (const BitArray& other) const
{
    return m_vec != other.m_vec;
}

template <size_t Size>
BitArray<Size>& BitArray<Size>::operator&= (const BitArray& other)
{
    std::transform(m_vec.begin(), m_vec.end(), other.m_vec.begin(), m_vec.begin(), FuncAND);

    return *this;
}

template <size_t Size>
BitArray<Size>& BitArray<Size>::operator|= (const BitArray& other)
{
    std::transform(m_vec.begin(), m_vec.end(), other.m_vec.begin(), m_vec.begin(), FuncOR);

    return *this;
}

template <size_t Size>
BitArray<Size>& BitArray<Size>::operator^= (const BitArray& other)
{
    std::transform(m_vec.begin(), m_vec.end(), other.m_vec.begin(), m_vec.begin(), FuncXOR);

    return *this;
}

template <size_t Size>
BitArray<Size>& BitArray<Size>::operator<<= (size_t shift)
{
    ArrayIndex tmp(shift);

    if (tmp.m_arr_index)
    {
        std::copy_backward(m_vec.begin(), m_vec.end() - tmp.m_arr_index, m_vec.end());
        std::fill(m_vec.begin(), m_vec.begin() + tmp.m_arr_index, 0);
    }
    if (tmp.m_bit_index)
    {
        FunctorShiftBitsLeft op(tmp.m_bit_index);
        size_t bits_in_end = (Size % __CHAR_BIT__);

        std::for_each(m_vec.begin(), m_vec.end(), op);

        size_t tmp = *(m_vec.end() - 1);    // if vec is bigger then Size. some bits need to be erase

        tmp = (tmp << bits_in_end) >> bits_in_end;
        *(m_vec.end() - 1) = tmp;
    }

    return *this;
}

template <size_t Size>
bool BitArray<Size>::operator[] (size_t index) const
{
    return GetBit(index);
}

template <size_t Size>
typename BitArray<Size>::BitProxy BitArray<Size>::operator[] (size_t index)
{
    return BitArray<Size>::BitProxy(*this, index);
}

template <size_t Size>
size_t BitArray<Size>::CountTrueBits() const
{
    return std::for_each(m_vec.begin(), m_vec.end(), FunctorCount()).GetCount();
}


// ********************************************************** BitArray Private:
template <size_t Size>
void BitArray<Size>::SetBit(size_t index, bool value)
{
    size_t mask = 1;
    ArrayIndex tmp(index);

    m_vec[tmp.m_arr_index] &= ~(mask << tmp.m_bit_index);
    mask = value;
    m_vec[tmp.m_arr_index] |= mask << tmp.m_bit_index;
}

template <size_t Size>
bool BitArray<Size>::GetBit(size_t index) const
{
    ArrayIndex tmp(index);

    return m_vec[tmp.m_arr_index] & (size_t(1) << tmp.m_bit_index);
}


// ******************************* BitProxy ***********************************

// *********************************************************** BitProxy Public:
template <size_t Size>
BitArray<Size>::BitProxy::BitProxy(BitArray<Size>& arr, size_t index) : m_arr(arr), m_index(index)
{}

template <size_t Size>
typename BitArray<Size>::BitProxy& BitArray<Size>::BitProxy::operator= (const BitProxy& other)
{
    *this = static_cast<bool>(other);

    return *this;
}

template <size_t Size>
BitArray<Size>::BitProxy::operator bool() const
{
    return m_arr.GetBit(m_index);
}

template <size_t Size>
typename BitArray<Size>::BitProxy& BitArray<Size>::BitProxy::operator = (bool value)
{
    m_arr.SetBit(m_index, value);

    return *this;
}


// ***************************************************** Overloading operators:
template <size_t Size>
std::ostream& operator<< (std::ostream& os, const BitArray<Size>& arr)
{
    FunctorPrint op(Size, os);

    for_each(arr.m_vec.begin(), arr.m_vec.end(), op);

    return os;
}


// ******************************* Fanctors ***********************************
FunctorCount::FunctorCount() : m_count(0)
{}

void FunctorCount::operator()(size_t byte)
{
	unsigned char tmp = 0;

	while (byte)
	{
		tmp = static_cast<unsigned char>(byte);
		m_count += s_lut[tmp];
		byte >>= __CHAR_BIT__;
	}
}

size_t FunctorCount::GetCount()
{
    return m_count;
}

FunctorPrint::FunctorPrint(size_t size, std::ostream& os) : m_size(size), m_os(os)
{}

void FunctorPrint::operator()(size_t byte)
{
    if (SIZE_T_BITS <= m_size)
    {
        PrintWordSize(byte, m_os);
        m_size -= SIZE_T_BITS;
    }
    else
    {
        PrintByBit(byte, &m_size, m_os);
    }

    m_os << std::endl;
}

FunctorShiftBitsLeft::FunctorShiftBitsLeft(size_t shift) : m_shift(shift), m_overflow(0)
{}

void FunctorShiftBitsLeft::operator()(size_t& byte)
{
    size_t bits_in_danger = byte >> (SIZE_T_BITS - m_shift);

    byte <<= m_shift;
    byte |= m_overflow;

    m_overflow = bits_in_danger;
}


// ******************************* Static Functions ***************************
static size_t FuncAND(size_t byte1, size_t byte2)
{
    return byte1 & byte2;
}

static size_t FuncOR(size_t byte1, size_t byte2)
{
    return byte1 | byte2;
}

static size_t FuncXOR(size_t byte1, size_t byte2)
{
    return byte1 ^ byte2;
}

static void PrintWordSize(size_t byte, std::ostream& os)
{
    size_t mask = 1;

    mask <<= SIZE_T_BITS - 1;

    os << '[';
    os << ' ';
    while (mask)
    {
        os << ((!!(byte & mask)) ? YELLOW : GRAY);
        os << (!!(byte & mask)) << RESET;
        mask >>= 1;
        os << ((!!(byte & mask)) ? YELLOW : GRAY);
        os << (!!(byte & mask)) << RESET;
        mask >>= 1;
        os << ((!!(byte & mask)) ? YELLOW : GRAY);
        os << (!!(byte & mask)) << RESET;
        mask >>= 1;
        os << ((!!(byte & mask)) ? YELLOW : GRAY);
        os << (!!(byte & mask)) << RESET;
        mask >>= 1;
        os << ((!!(byte & mask)) ? YELLOW : GRAY);
        os << (!!(byte & mask)) << RESET;
        mask >>= 1;
        os << ((!!(byte & mask)) ? YELLOW : GRAY);
        os << (!!(byte & mask)) << RESET;
        mask >>= 1;
        os << ((!!(byte & mask)) ? YELLOW : GRAY);
        os << (!!(byte & mask)) << RESET;
        mask >>= 1;
        os << ((!!(byte & mask)) ? YELLOW : GRAY);
        os << (!!(byte & mask)) << RESET;
        mask >>= 1;

        os << ' ';
    }
    os << ']';
}

static void PrintByBit(size_t byte, size_t* size, std::ostream& os)
{
    size_t mask = 1;
    size_t i = SIZE_T_BITS;

    os << '[';
    os << ' ';

    while (i > *size)
    {
        --i;
        os << ' ';
        if (i % __CHAR_BIT__ == 0)
        {
            os << ' ';
        }
    }

    mask <<= (i - 1);

    while (mask)
    {
        os << (!!(byte & mask));
        mask >>= 1;

        *size -= 1;

        if (*size % __CHAR_BIT__ == 0)
        {
            os << ' ';
        }
    }
    os << ']';
}
} // namespace hrd11

#endif // __HRD11_BITARRAY_HPP__
