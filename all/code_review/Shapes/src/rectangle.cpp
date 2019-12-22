#include "point.hpp"  // ilrd::Point
#include "glut_utils.h" //COLORS enum
#include "shapes.hpp"

namespace hrd11
{
//point 1 is 'up-left' corner, point 2 is 'down-right'.
Rectangle::Rectangle(COLORS color, ilrd::Point arr[2], double angle):Shape(color, ilrd::Point(0,0), angle)
{
    m_corners[0] = arr[0];
    m_corners[1] = arr[1];
    m_center = (m_corners[0] + m_corners[1]) / 2;
}

Rectangle:: ~Rectangle()
{}

Shape& Rectangle::Draw()
{
    int height = GetHight() / 2;
    int width =  GetWidth() / 2;
   
    int x1 = m_center.GetX() - width;
    int y1 = m_center.GetY() + height;
    int x3 = m_center.GetX() + width;
    int y3 = m_center.GetY() - height;

    int x2 = x1;
    int y2 = y3;
    int x4 = x3;
    int y4 = y1;

    ilrd::Point p1(x1, y1);
    ilrd::Point p2(x2, y2);
    ilrd::Point p3(x3, y3);
    ilrd::Point p4(x4, y4);

    p1.Revolve(m_center, m_angle);
    p2.Revolve(m_center, m_angle);
    p3.Revolve(m_center, m_angle);
    p4.Revolve(m_center, m_angle);

    x1 = (int)p1.GetX();
    x2 = (int)p2.GetX();
    x3 = (int)p3.GetX();
    x4 = (int)p4.GetX();

    y1 = (int)p1.GetY();
    y2 = (int)p2.GetY();
    y3 = (int)p3.GetY();
    y4 = (int)p4.GetY();

    DrawPolygon(m_color, 4, x1, y1, x2, y2, x3, y3, x4, y4);

    return *this;
}

double Rectangle::Area()
{
    int height = GetHight();
    int width = GetWidth();
    
    return height * width;
}

inline double Rectangle::GetHight()
{
    return ((int)m_corners[0].GetY() - (int)m_corners[1].GetY());
}

inline double Rectangle::GetWidth()
{
    return ((int)m_corners[1].GetX() - (int)m_corners[0].GetX());
}

}