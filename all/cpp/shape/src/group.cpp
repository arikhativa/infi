

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

// **************************************************************** Declartion:
static void SwapPtr(Shape** a, Shape** b);


// *********************************************************** Spaciel Members:
Group::Group() : Shape(Point(0,0))
{}

Group::Group(Shape* shape) : Shape(GetCenter(*shape))
{
    m_shapes.push_back(shape);
}

Group::~Group()
{
    m_shapes.clear();
}


// *********************************************************** Virtual members:
Shape& Group::Draw()
{
    for (size_t i = 0; i < m_shapes.size(); ++i)
    {
        m_shapes[i]->Draw();
    }

    return *this;
}

Shape& Group::Move(double x, double y)
{
    m_center += Point(x, y);

    for (size_t i = 0; i < m_shapes.size(); ++i)
    {
        m_shapes[i]->Move(x, y);
    }

    return *this;
}

Shape& Group::Rotate(double angle)
{
    for (size_t i = 0; i < m_shapes.size(); ++i)
    {
        m_shapes[i]->Revolve(angle, m_center);
        m_shapes[i]->Rotate(angle);
    }

    return *this;

}
Shape& Group::Revolve(double angle, const ilrd::Point& pivot)
{
    m_center.Revolve(pivot, angle);

    for (size_t i = 0; i < m_shapes.size(); ++i)
    {
        m_shapes[i]->Revolve(angle, pivot);
        m_shapes[i]->Rotate(angle);
    }

    return *this;
}

double Group::CalculateArea()
{
    double sum = 0;

    for (size_t i = 0; i < m_shapes.size(); ++i)
    {
        sum += m_shapes[i]->CalculateArea();
    }

    return sum;
}


// ******************************************************************* Members:
void Group::Add(Shape* shape)
{
    m_shapes.push_back(shape);

    CalculateCenter();
}

void Group::Remove(Shape* shape)
{
    for (size_t i = 0; i < m_shapes.size(); ++i)
    {
        if(m_shapes[i] == shape)
        {
            SwapPtr(&m_shapes[i], &m_shapes[m_shapes.size() - 1]);
            m_shapes.pop_back();
            break ;
        }
    }

    CalculateCenter();
}

void Group::CalculateCenter()
{
    double sum_area = 0;
    double tmp_area = 0;
    double x = 0;
    double y = 0;
    Point tmp_center(0, 0);

    // Calculating mass cetner of all shapes in the group.
    for (size_t i = 0; i < m_shapes.size(); ++i)
    {
        tmp_area = m_shapes[i]->CalculateArea();
        tmp_center = GetCenter(*m_shapes[i]);

        x += tmp_area * tmp_center.GetX();
        y += tmp_area * tmp_center.GetY();

        sum_area += tmp_area;
    }

    x /= sum_area;
    y /= sum_area;

    m_center = Point(x, y);
}


// ************************************************************** Static Funcs:
static void SwapPtr(Shape** a, Shape** b)
{
    Shape* tmp = *a;

    *a = *b;
    *b = tmp;
}
} // namespace hrd11
