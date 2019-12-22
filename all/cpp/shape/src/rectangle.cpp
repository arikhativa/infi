
#include "point.hpp"  // Point
#include "shape.hpp"  // Shape
#include "rectangle.hpp"  // Rectangle

static const int s_corners_in_rectangle = 4;

namespace hrd11
{

using namespace ilrd;


// **************************************************************** Declartion:
static inline Point GetCenterForRectangle(Point corner1, Point corner2);


// *********************************************************** Spaciel Members:
Rectangle::Rectangle(Point arr[s_points], COLORS color) :
                Shape(GetCenterForRectangle(arr[0], arr[1]), color)
{
    m_corners[0] = arr[0];
    m_corners[1] = arr[1];
}


// *********************************************************** Virtual members:
Shape& Rectangle::Draw()
{
    Point tmp1(m_corners[0]);
    Point tmp3(m_corners[1]);
    Point tmp_center(GetCenterForRectangle(tmp1, tmp3));

    Point tmp2(m_corners[0].GetX(), m_corners[1].GetY());
    Point tmp4(m_corners[1].GetX(), m_corners[0].GetY());

    tmp1 += m_center - tmp_center;
    tmp2 += m_center - tmp_center;
    tmp3 += m_center - tmp_center;
    tmp4 += m_center - tmp_center;

    tmp1.Revolve(m_center, m_angle);
    tmp2.Revolve(m_center, m_angle);
    tmp3.Revolve(m_center, m_angle);
    tmp4.Revolve(m_center, m_angle);

    DrawPolygon(m_color, s_corners_in_rectangle,
                static_cast<int>(tmp1.GetX()), static_cast<int>(tmp1.GetY()),
                static_cast<int>(tmp2.GetX()), static_cast<int>(tmp2.GetY()),
                static_cast<int>(tmp3.GetX()), static_cast<int>(tmp3.GetY()),
                static_cast<int>(tmp4.GetX()), static_cast<int>(tmp4.GetY()));

    return *this;
}

double Rectangle::CalculateArea()
{
    double x1 = m_corners[0].GetX();
    double y1 = m_corners[0].GetY();
    double x2 = m_corners[1].GetX();
    double y2 = m_corners[1].GetY();

    // x1 = x2 - x1;
    // y1 = y2 - y1;
    // return (x1 * y1);

    return ((x2 - x1) * (y2 - y1));
}


// ************************************************************** Static Funcs:
static inline Point GetCenterForRectangle(Point corner1, Point corner2)
{
    int x1 = corner1.GetX();
    int y1 = corner1.GetY();
    int x2 = corner2.GetX();
    int y2 = corner2.GetY();

    x1 = (x1 + x2) / 2;
    y1 = (y1 + y2) / 2;

    return Point(x1, y1);
}
} // namespace hrd11
