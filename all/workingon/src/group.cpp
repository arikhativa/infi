
#include <iostream> //delllllllllll


#include "glut_utils.h"     // DrawCircle()
#include "point.hpp"        // ilrd::Point
#include "shape.hpp"        // Shape
#include "group.hpp"       // Circle


#include "circle.hpp"       // Circle
#include "rectangle.hpp"   // Rectangle

#include <vector>

namespace hrd11
{

using namespace ilrd;

// ********************************************************* Static declartion:
static void SwapPtr(Shape** a, Shape** b);

// *********************************************************** Spaciel Members:
Group::Group() : Shape(Point(0,0)), m_size(0)
{}

Group::Group(Shape* shape) : Shape(GetCenter(*shape)), m_size(1)
{
    m_shapes.push_back(shape);
}

Group::~Group()
{
    m_shapes.clear();
    m_size = 0;
}

// ******************************************************************* Members:
Shape& Group::Draw()
{
    for (size_t i = 0; i < m_size; ++i)
    {
        m_shapes[i]->Draw();
    }

    return *this;
}

Shape& Group::Move(double x, double y)
{
    m_center += Point(x, y);
    
    for (size_t i = 0; i < m_size; ++i)
    {
        m_shapes[i]->Move(x, y);
    }


    return *this;
}

Shape& Group::Rotate(double angle)
{
    for (size_t i = 0; i < m_size; ++i)
    {
        m_shapes[i]->Revolve(angle, m_center);
    }

    return *this;

}
Shape& Group::Revolve(double angle, const ilrd::Point& pivot)
{
    (void)angle;
    (void)pivot;

    return *this;
}

double Group::CalculateArea()
{
    return 0;
}


void Group::Add(Shape* shape)
{
    m_shapes.push_back(shape);
    ++m_size;

    CalculateCenter();
}


void Group::Remove(Shape* shape)
{
    for (size_t i = 0; i < m_size; ++i)
    {
        if(m_shapes[i] == shape)
        {
            SwapPtr(&m_shapes[i], &m_shapes[m_size - 1]);
            m_shapes.pop_back();
            --m_size;
            break ;
        }
    }

    CalculateCenter();
}

// ************************************************************** Static Funcs:
void Group::CalculateCenter()
{
    double sum = 0;
    double tmp = 0;
    double mini_x = 0;
    double mini_y = 0;
    Point tmp_center(0, 0);
    int x = 0;
    int y = 0;

    for (size_t i = 0; i < m_size; ++i)
    {
        tmp = m_shapes[i]->CalculateArea();
        tmp_center = GetCenter(*m_shapes[i]);

        mini_x += tmp * tmp_center.GetX();
        mini_y += tmp * tmp_center.GetY();

        sum += tmp;
    }
    x = static_cast<int>(mini_x / sum);
    y = static_cast<int>(mini_y / sum);

    m_center = Point(x, y);
    std::cout << m_center << std::endl;
}

static void SwapPtr(Shape** a, Shape** b)
{
    Shape* tmp = *a;

    *a = *b;
    *b = tmp;
}

} // namespace hrd11
