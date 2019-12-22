#include "point.hpp"  // ilrd::Point
#include "glut_utils.h" //COLORS enum
#include "shapes.hpp" //PI

namespace hrd11
{

Circle::Circle(COLORS color, ilrd::Point position, double angle, size_t radius):Shape(color, position, angle), m_radius(radius)
{}

Circle:: ~Circle()
{}

Shape& Circle::Draw()
{
    DrawCircle(m_color, m_center.GetX(), m_center.GetY(), m_radius);

    return *this;
}

double Circle::Area()
{
    return PI * m_radius * m_radius;
}

}