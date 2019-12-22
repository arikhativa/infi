#include <vector>
#include "point.hpp"  // ilrd::Point
#include "glut_utils.h" //COLORS enum
#include "shapes.hpp"

namespace hrd11
{

Group::Group():Shape()
{}

Group::Group(Shape* shape):Shape()
{
    m_shapes.push_back(shape);
    CalcCenterOfGravity();
}

void Group::Add(Shape* shape)
{
    m_shapes.push_back(shape);
    CalcCenterOfGravity();
}


void Group::Remove(Shape* shape)
{
    int size = m_shapes.size();// work with ints in order to be able to return -1 at FindShape
    int index = FindShape(shape, size);

    Swap(size, index);

    m_shapes.pop_back();
    CalcCenterOfGravity();
}

Shape& Group::Draw()
{
    size_t size = m_shapes.size();

    for(size_t i = 0; i < size; ++i)
    {
        m_shapes[i]->Draw();
    }

    return *this;
}

Shape& Group::Move(double dx, double dy)
{
    size_t size = m_shapes.size();

    for(size_t i = 0; i < size; ++i)
    {
        m_shapes[i]->Move(dx, dy);
    }

    return *this;
}

Shape& Group::Rotate(double angle)
{
    size_t size = m_shapes.size();

    for(size_t i = 0; i < size; ++i)
    {
        m_shapes[i]->Revolve(angle, m_center);
    }

    return *this;
}

double Group::Area()
{
    size_t size = m_shapes.size();
    double area = 0;

    for(size_t i = 0; i < size; ++i)
    {
        area += m_shapes[i]->Area();
    }

    return area;
}

void Group::CalcCenterOfGravity()//formula: (Sum of (mi*ri))/(M), where i is index of a shape
{
    double gr_area = Area();
    double sum_area_multiplied_arm_x = 0;
    double sum_area_multiplied_arm_y = 0;
    size_t size = m_shapes.size();

    for(size_t i = 0; i < size; ++i)
    {
        // sum_area_multiplied_arm_x += (m_shapes[i]->Area() * m_shapes[i]->GetCenter().GetY());
        sum_area_multiplied_arm_x += (m_shapes[i]->Area() * m_shapes[i]->m_center.GetY());
        sum_area_multiplied_arm_y += (m_shapes[i]->Area() * m_shapes[i]->GetCenter().GetX());
    }

    m_center = ilrd::Point(sum_area_multiplied_arm_x / gr_area, sum_area_multiplied_arm_y / gr_area);
}

int Group::FindShape(Shape* shape, int size)
{
    for(int i = 0; i < size; ++i)
    {
        if(m_shapes[i] == shape)
        {
            return i;
        }
    }

    return -1;
}

void Group::Swap(int size, int i)
{
    Shape* tmp = m_shapes[i];
    m_shapes[i] = m_shapes[size];
    m_shapes[size] = tmp;
}

}
