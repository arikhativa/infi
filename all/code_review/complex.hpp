/*
    Complex -

    Discription:    api for using complex numbers(a + bi)
    Date:           9.12.19
    File names:     complex.hpp

    Reviewer: Yoav

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
std::istream& operator>>(std::istream& os, const Complex& complex);

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
    friend std::istream& operator>>(std::istream& in, Complex& complex);

    // This function (operator<<) is for testing
    friend void operator<<(char* str, Complex& complex);

    double m_real;
    double m_img;

};

/*************************   Contructor   **************************/

Complex::Complex(double real, double img): m_real(real),m_img(img)
{}

/************************* None - member function  ***************/


inline const Complex operator+(const Complex& lhs, const Complex& rhs)
{
  Complex res;
  // TODO: new line.
  res +=lhs;
  res +=rhs;

  return res;
}

inline const Complex operator-(const Complex& lhs, const Complex& rhs)
{
  Complex res;
  res +=lhs;
  res -=rhs;

  return res;
}

inline const Complex operator*(const Complex& lhs, const Complex& rhs)
{
  Complex res(lhs);

  //res = lhs;
  res *=rhs;

  return res;
}

inline const Complex operator/(const Complex& lhs, const Complex& rhs)
{
  Complex res;
  res +=lhs;
  res /=rhs;

  return res;
}

inline std::ostream& operator<<(std::ostream& os, const Complex& complex)
{
    if(complex.m_img >=0)
    {
      os << complex.m_real << " + " << complex.m_img << "i";
    }
    else
    {
      os << complex.m_real << " " << complex.m_img << "i";
    }

    return os;
}
inline std::istream& operator>>(std::istream& in, Complex& complex)
{
    // char i?
    char i = 0;
    in >> complex.m_real;
    in >> complex.m_img;
//    in >> i;

    return in;
}

/************************* Member functions  *********************/


inline void Complex::SetReal(double real)
{
  m_real = real;
}

inline void Complex::SetImg(double img)
{
  m_img = img;
}

inline bool Complex::operator==(const Complex& other) const
{
  return ((m_real == other.m_real) &&(m_img == other.m_img));
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
  Complex copy_this(m_real,m_img);
  Complex copy_other(other.m_real,other.m_img);

  //a1 * a2 - b1 * b2
  m_real =  (copy_this.m_real * copy_other.m_real) - (copy_this.m_img * copy_other.m_img);

  //a1 * b2 + b1 * a2
  m_img = (copy_this.m_real * copy_other.m_img) + (copy_this.m_img * copy_other.m_real);

  return *this;
}
//formula: a1 +b1 / a2 * b2
//        -->(a1 +b1)(a2 * -b2) / (a2 * b2)(a2 * -b2)
inline Complex& Complex::operator/=(const Complex& other)
{
  Complex complement(other.m_real, (other.m_img *-1));
  Complex temp_other(other);
  Complex temp_this(m_real, m_img);

  temp_other *= complement;
  temp_this *= complement;

  // after i cancelled
  m_real = temp_this.m_real / temp_other.m_real;
  m_img = temp_this.m_img / temp_other.m_real;

  return *this;
}

// witch one?
}//end namespace

#endif //  __HRD11_Complex_HPP__
