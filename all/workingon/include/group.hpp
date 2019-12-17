#ifndef __HRD11_GROUP_HPP__
#define __HRD11_GROUP_HPP__

#include <sys/types.h> // ssize_t

#include "point.hpp"  // ilrd::Point
#include "shape.hpp"  // Shape

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

    void Add(Shape* shape);
    void Remove(Shape* shape);

private:
    Group(const Group& other);
    Group& operator=(const Group& other);

    std::vector<Shape*> m_shapes;
    size_t m_size;
};
} // namespace hrd11

#endif //__HRD11_GROUP_HPP__
