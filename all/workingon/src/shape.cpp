
#include <iostream>

#include "glut_utils.h"     // COLOR
#include "point.hpp"      // ilrd::Point
#include "shape.hpp"    // Shape

namespace hrd11
{
using namespace ilrd;

Shape::Shape(ilrd::Point pos, COLORS color, double angle) :
    m_color(color),
    m_center(pos),
    m_angle(angle)
{}

Shape::~Shape()
{}

Shape& Shape::Move(double x, double y)
{
    m_center += Point(x, y);

    std::cout << m_center << std::endl << std::endl;

    return *this;
}

Shape& Shape::Rotate(double angle)
{
    m_angle = angle;

    return *this;
}

// Shape& Revolve(double angle, const ilrd::Point& pivot)
// {}

void Shape::SetColor(COLORS color)
{
    m_color = color;
}

} // namespace hrd11
