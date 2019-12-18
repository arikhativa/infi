
#include <iostream> //delllllllllll

#include "glut_utils.h" // COLOR
#include "point.hpp"    // ilrd::Point
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

Shape& Shape::Move(double delta_x, double delta_y)
{

    Step(&m_center, delta_x, delta_y);

    // std::cout << "-- in Move" << std::endl;
    // std::cout << m_center << std::endl << std::endl;

    return *this;
}

Shape& Shape::Rotate(double angle)
{
    m_angle = (angle * 3.1415) / 180;

    return *this;
}

Shape& Shape::Revolve(double angle, const ilrd::Point& pivot)
{
    m_center.Revolve(pivot, (angle * 3.1415) / 180);

    return *this;
}

void Shape::SetColor(COLORS color)
{
    m_color = color;
}

ilrd::Point Shape::GetCenter(const Shape& shape)
{
    return shape.m_center;
}


} // namespace hrd11
