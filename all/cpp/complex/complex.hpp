/*
    Complex -

    Discription:    api for using complex numbers(a + bi)
    Date:           9.12.19
    File names:     complex.hpp

    Reviewer:       Aviv

    ver 1.1 removed const from complex argument in istream func
    Ver 1.0
*/

#ifndef __HRD11_Complex_HPP__
#define __HRD11_Complex_HPP__

#include <iostream> //istream, ostream

namespace hrd11
{

class Complex;

const Complex operator+(const Complex& lhs, const Complex& rhs);
const Complex operator-(const Complex& lhs, const Complex& rhs);
const Complex operator*(const Complex& lhs, const Complex& rhs);
const Complex operator/(const Complex& lhs, const Complex& rhs);

std::ostream& operator<<(std::ostream& os, const Complex& complex);
std::istream& operator>>(std::istream& os, Complex& complex);


class Complex
{
public:

    //non explicit on purpose
    Complex(double real = 0, double img = 0);
    //using generated cctor , copy= , dtor
    bool operator==(const Complex& other) const;
    Complex& operator+=(const Complex& other);
    Complex& operator-=(const Complex& other);
    Complex& operator*=(const Complex& other);
    Complex& operator/=(const Complex& other);

    void SetReal(double real);
    void SetImg(double img);

private:

    friend std::ostream& operator<<(std::ostream& os, const Complex& complex);
    friend std::istream& operator>>(std::istream& os, Complex& complex);

    // This function (operator<<) is for testing
    friend void operator<<(char* str, Complex& complex);

    double m_real;
    double m_img;
};


// ************************************************************ Special member:
Complex::Complex(double real,double img) : m_real(real), m_img(img)
{}


// ***************************************************** Member Math Operators:
inline bool Complex::operator==(const Complex& other) const
{
    return (m_real == other.m_real && m_img == other.m_img);
}

inline Complex& Complex::operator+=(const Complex& other)
{
    m_real += other.m_real;
    m_img += other.m_img;

    return *this;
}

inline Complex& Complex::operator-=(const Complex& other)
{
    m_real -= other.m_real;
    m_img -= other.m_img;

    return *this;
}

inline Complex& Complex::operator*=(const Complex& other)
{
    double a_real = m_real;
    double a_img = m_img;
    double b_real = other.m_real;
    double b_img = other.m_img;

    m_real = a_real * b_real - a_img * b_img;
    m_img = a_real * b_img + a_img * b_real;

    return *this;
}

inline Complex& Complex::operator/=(const Complex& other)
{
    double a_real = m_real;
    double a_img = m_img;
    double b_real = other.m_real;
    double b_img = other.m_img;
    double devision = (b_real * b_real) + (b_img * b_img);

    m_real = (a_real * b_real + a_img * b_img) / devision;
    m_img = -((a_real * b_img) - (a_img * b_real)) / devision;

    return *this;
}


// ***************************************************** Member Math Operators:
void Complex::SetReal(double real)
{
    m_real = real;
}

void Complex::SetImg(double img)
{
    m_img = img;
}


// ************************************************************ Math Operators:
const Complex operator+(const Complex& lhs, const Complex& rhs)
{
    Complex ret(lhs);

    ret += rhs;

    return ret;
}

const Complex operator-(const Complex& lhs, const Complex& rhs)
{
    Complex ret(lhs);

    ret -= rhs;

    return ret;
}

const Complex operator*(const Complex& lhs, const Complex& rhs)
{
    Complex ret(lhs);

    ret *= rhs;

    return ret;
}

const Complex operator/(const Complex& lhs, const Complex& rhs)
{
    Complex ret(lhs);

    ret /= rhs;

    return ret;
}


// ************************************************************* I/O Operators:
inline std::ostream& operator<<(std::ostream& os, const Complex& complex)
{
    os << complex.m_real << " + ";
    os << complex.m_img << "i\n";

    return os;
}

inline std::istream& operator>>(std::istream& os, Complex& complex)
{
    std::cout << "-- Enter real number:\n";
    os >> complex.m_real;
    std::cout << "-- Enter img number:\n";
    os >> complex.m_img;

    return os;
}
} // namespace hrd11

#endif // __HRD11_Complex_HPP__
