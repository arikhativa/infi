
#include <iostream>

#include "point.hpp"  // ilrd::Point
#include "shape.hpp"  // Shape
#include "rectangle.hpp"  // Rectangle

static const int s_corners_in_rectangle = 4;

namespace hrd11
{

static inline ilrd::Point GetCenterForRectangle(ilrd::Point corner1, ilrd::Point corner2)
{
    int x1 = corner1.GetX();
    int y1 = corner1.GetY();
    int x2 = corner2.GetX();
    int y2 = corner2.GetY();

    x1 = (x1 + x2) / 2;
    y1 = (y1 + y2) / 2;

    return ilrd::Point(x1, y1);
}

Rectangle::Rectangle(ilrd::Point arr[s_points], COLORS color) :
                Shape(GetCenterForRectangle(arr[0], arr[1]), color)
{
    m_corners[0] = arr[0];
    m_corners[1] = arr[1];
}

Shape& Rectangle::Draw()
{
    using namespace ilrd;

    Point tmp1(m_corners[0]);
    Point tmp3(m_corners[1]);
    Point tmp_center(GetCenterForRectangle(tmp1, tmp3));

    Point tmp2(m_corners[0].GetX(), m_corners[1].GetY());
    Point tmp4(m_corners[1].GetX(), m_corners[0].GetY());

    tmp1 += m_center - tmp_center;
    tmp2 += m_center - tmp_center;
    tmp3 += m_center - tmp_center;
    tmp4 += m_center - tmp_center;

    tmp1.Revolve(tmp_center, m_angle);
    tmp2.Revolve(tmp_center, m_angle);
    tmp3.Revolve(tmp_center, m_angle);
    tmp4.Revolve(tmp_center, m_angle);

    DrawPolygon(m_color, s_corners_in_rectangle,
                (int)tmp1.GetX(), (int)tmp1.GetY(),
                (int)tmp2.GetX(), (int)tmp2.GetY(),
                (int)tmp3.GetX(), (int)tmp3.GetY(),
                (int)tmp4.GetX(), (int)tmp4.GetY());


    return *this;
}

double Rectangle::CalculateArea()
{
    double x1 = m_corners[0].GetX();
    double y1 = m_corners[0].GetY();
    double x2 = m_corners[1].GetX();
    double y2 = m_corners[1].GetY();

    x1 = x2 - x1;
    y1 = y2 - y1;

    return (x1 * y1);
}

} // namespace hrd11
