#ifndef __HRD11_RECTANGLE_HPP__
#define __HRD11_RECTANGLE_HPP__

#include <sys/types.h> // ssize_t

#include "point.hpp"  // ilrd::Point
#include "shape.hpp"  // Shape

namespace hrd11
{

class Rectangle : public Shape
{
public:
    static const s_points = 2;

    explicit Rectangle(ilrd::Point arr[s_points]);
    virtual ~Rectangle();

    virtual Shape& Draw();
    virtual Shape& Move(double x, double y);
    virtual Shape& Rotate(double angle);
    virtual Shape& Revolve(double angle, const ilrd::Point& pivot);

private:
    Rectangle(const Rectangle& other);
    Rectangle& operator=(const Rectangle& other);

    ilrd::Point m_corners[s_points];
};
} // namespace hrd11

#endif //__HRD11_RECTANGLE_HPP__
