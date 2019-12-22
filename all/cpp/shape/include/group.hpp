#ifndef __HRD11_GROUP_HPP__
#define __HRD11_GROUP_HPP__

#include "point.hpp"    // ilrd::Point
#include "shape.hpp"    // Shape

#include <vector>

namespace hrd11
{

class Group : public Shape
{
public:
    Group();
    explicit Group(Shape* shape);
    virtual ~Group();

    virtual Shape& Draw();
    virtual Shape& Move(double x, double y);
    virtual Shape& Rotate(double angle);
    virtual Shape& Revolve(double angle, const ilrd::Point& pivot);
    virtual double CalculateArea();

    void Add(Shape* shape);
    void Remove(Shape* shape);

private:
    // Disabling cctor and copy=
    Group(const Group& other);
    Group& operator=(const Group& other);

    void CalculateCenter();

    std::vector<Shape*> m_shapes;
};
} // namespace hrd11

#endif //__HRD11_GROUP_HPP__
