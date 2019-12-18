
#ifndef __HRD11_CIRCLE_HPP__
#define __HRD11_CIRCLE_HPP__

#include <sys/types.h> // ssize_t

#include "point.hpp"  // ilrd::Point
#include "shape.hpp"  // Shape

namespace hrd11
{

class Circle : public Shape
{
public:
    explicit Circle(size_t radius = 50, COLORS color = COLOR_WHITE);

    virtual Shape& Draw();
    virtual double CalculateArea();

    double GetPi();

private:
    Circle(const Circle& other);
    Circle& operator=(const Circle& other);

    size_t m_radius;
};

} // namespace hrd11

#endif //__HRD11_CIRCLE_HPP__
