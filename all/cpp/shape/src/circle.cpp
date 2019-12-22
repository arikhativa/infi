
#include "glut_utils.h"     // DrawCircle()
#include "point.hpp"        // ilrd::Point
#include "shape.hpp"        // Shape
#include "circle.hpp"       // Circle

namespace hrd11
{

using namespace ilrd;


// **************************************************************** Declartion:
static double GetPi();


// *********************************************************** Spaciel Members:
Circle::Circle(size_t radius, COLORS color) :
    Shape(Point(0, 0), color),
    m_radius(radius)
{}


// *********************************************************** Virtual members:
Shape& Circle::Draw()
{
    int x = static_cast<int>(m_center.GetX());
    int y = static_cast<int>(m_center.GetY());

    DrawCircle(m_color, x, y, static_cast<int>(m_radius));

    return *this;
}

double Circle::CalculateArea()
{
    return (m_radius * m_radius) * GetPi();
}


// ************************************************************** Static Funcs:
double GetPi()
{
    static const double s_pi = (double)3.14159265359;

    return s_pi;
}
} // namespace hrd11
