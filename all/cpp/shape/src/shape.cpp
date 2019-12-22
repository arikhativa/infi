
#include "glut_utils.h" // COLOR
#include "point.hpp"    // ilrd::Point
#include "shape.hpp"    // Shape

namespace hrd11
{
using namespace ilrd;


// *********************************************************** Spaciel Members:
Shape::Shape(ilrd::Point pos, COLORS color, double angle) :
    m_center(pos),
    m_color(color),
    m_angle(angle)
{}

Shape::~Shape()
{}


// *********************************************************** Virtual members:
Shape& Shape::Move(double delta_x, double delta_y)
{
    m_center += Point(delta_x, delta_y);

    return *this;
}

Shape& Shape::Rotate(double angle)
{
    // Convertion to radians
    m_angle += ((angle * 3.1415) / 180);

    return *this;
}

Shape& Shape::Revolve(double angle, const ilrd::Point& pivot)
{
    // Convertion to radians
    m_center.Revolve(pivot, (angle * 3.1415) / 180);

    return *this;
}


// ******************************************************************* Members:
void Shape::SetColor(COLORS color)
{
    m_color = color;
}

ilrd::Point Shape::GetCenter(const Shape& shape)
{
    return shape.m_center;
}
} // namespace hrd11
