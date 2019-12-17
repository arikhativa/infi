
#include "glut_utils.h"     // DrawCircle()
#include "point.hpp"          // ilrd::Point
#include "shape.hpp"        // Shape
#include "circle.hpp"       // Circle

namespace hrd11
{

using namespace ilrd;

Circle::Circle(size_t radius, COLORS color) :
    Shape(Point(0, 0), color),
    m_radius(radius)
{}

Shape& Circle::Draw()
{
    DrawCircle(m_color, static_cast<int>(m_center.GetX()),
                static_cast<int>(m_center.GetY()), static_cast<int>(m_radius));

    return *this;
}

Shape& Circle::Revolve(double angle, const Point& pivot)
{
    (void)angle;
    (void)pivot;

    return *this;
}

} // namespace hrd11
