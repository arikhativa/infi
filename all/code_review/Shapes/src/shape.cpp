#include "point.hpp"  // ilrd::Point
#include "glut_utils.h" //COLORS enum
#include "shapes.hpp" //PI



namespace hrd11
{
Shape::Shape(COLORS color, ilrd::Point position, double angle): m_center(position), m_angle(angle), m_color(color)
{}

Shape::~Shape()
{}

Shape& Shape::Move(double dx, double dy)
{
    ilrd::Step(&m_center, dx, dy);

    return *this;
}

void Shape::SetColor(COLORS color)
{
    m_color = color;
}

Shape& Shape::Revolve(double angle, const ilrd::Point& pivot)
{
    m_center.Revolve(pivot, DegToRad(angle));

    return *this;
}

Shape& Shape::Rotate(double angle)
{
    m_angle = DegToRad(angle);

    return *this;
}

ilrd::Point Shape::GetCenter()
{
    return m_center;
}

inline double Shape::DegToRad(double angle)
{
    return angle * PI / 180;
}

}