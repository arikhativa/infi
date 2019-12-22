#ifndef __HRD11_RECTANGLE_HPP__
#define __HRD11_RECTANGLE_HPP__

#include "point.hpp"  // ilrd::Point
#include "shape.hpp"  // Shape

namespace hrd11
{

class Rectangle : public Shape
{
public:
    static const size_t s_points = 2;

    // arr[0] should have the top-left corner and arr[1] the bottom-right.
    explicit Rectangle(ilrd::Point arr[s_points], COLORS color = COLOR_WHITE);

    virtual Shape& Draw();
    virtual double CalculateArea();

private:
    Rectangle(const Rectangle& other);
    Rectangle& operator=(const Rectangle& other);

    ilrd::Point m_corners[s_points];
};
} // namespace hrd11

#endif //__HRD11_RECTANGLE_HPP__
