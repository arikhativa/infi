
/*
    Shape -

    Discreption:
    Shape is an abstruct type. It is intened to use with polymorphism

    date:   17.12.19

    reviwer:   ...

*/

#ifndef __HRD11_SHAPES_HPP__
#define __HRD11_SHAPES_HPP__

#include "point.hpp"  // ilrd::Point

namespace hrd11
{

class Shape
{
public:
    explicit Shape(ilrd::Point pos, COLORS color = COLOR_WHITE, double angle = 0);
    // define ~Shape()
    virtual ~Shape();

    virtual Shape& Draw() = 0;
    // interval from current place
    virtual Shape& Move(double x, double y);
    // positive number will be clockwise
    virtual Shape& Rotate(double angle);
    virtual Shape& Revolve(double angle, const ilrd::Point& pivot) = 0;

    void SetColor(COLORS color);

protected:
    COLORS m_color;
    ilrd::Point m_center;
    double m_angle;

private:
    // no cctor and no copy=
    // can implement 'Clone()'
    Shape(const Shape& other);
    Shape& operator=(const Shape& other);

};

} // namespace hrd11

#endif //__HRD11_SHAPES_HPP__
