
/*
    Shape -

    Discreption:
    Shape is an abstruct type. It is intened to use with polymorphism

    date:   17.12.19

    reviwer:   ...

*/

#ifndef __HRD11_SHAPES_HPP__
#define __HRD11_SHAPES_HPP__

#include "glut_utils.h" // COLOR
#include "point.hpp"    // ilrd::Point

namespace hrd11
{

class Shape
{
public:
    explicit Shape(ilrd::Point pos, COLORS color = COLOR_WHITE, double angle = 0);
    virtual ~Shape();

    virtual Shape& Draw() = 0;
    virtual Shape& Move(double x, double y);    // interval from current place
    virtual Shape& Rotate(double angle);        // positive number will be clockwise
    virtual Shape& Revolve(double angle, const ilrd::Point& pivot);
    virtual double CalculateArea() = 0;

    void SetColor(COLORS color);

    static ilrd::Point GetCenter(const Shape& shape);
protected:

    ilrd::Point m_center;
    COLORS m_color;
    double m_angle;

private:
    // no cctor and no copy=
    // can implement 'Clone()'
    Shape(const Shape& other);
    Shape& operator=(const Shape& other);

};

} // namespace hrd11

#endif //__HRD11_SHAPES_HPP__
