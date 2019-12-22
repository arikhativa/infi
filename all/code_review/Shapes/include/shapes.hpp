
#ifndef __HRD11_SHAPES_HPP__
#define __HRD11_SHAPES_HPP__

#include <sys/types.h> // ssize_t
#include <vector>
#include "point.hpp"  // ilrd::Point UDT
#include "glut_utils.h" // enum COLORS

//compile command
//gcpp rectangle.cpp circle.cpp group.cpp shape.cpp test.cpp ../lib/libpoint.a -I ../include -Wl,-rpath=. ../lib/libglut_utils.so

namespace hrd11
{
const static double PI = 3.14159; //used to convert rad to deg

class Shape
{
public:
    explicit Shape(COLORS color = COLOR_WHITE, ilrd::Point position = ilrd::Point(0,0), double angle = 0);
    virtual ~Shape();

    virtual Shape& Draw() = 0;
    virtual Shape& Move(double dx, double dy);
    virtual Shape& Rotate(double angle);// positive number will be clockwise
    virtual Shape& Revolve(double angle, const ilrd::Point& pivot);
    virtual double Area() = 0;

    void SetColor(COLORS color);

    ilrd::Point GetCenter();
protected:
    ilrd::Point m_center;
    double m_angle;
    COLORS m_color;

private:
    // no cctor and no copy=
    Shape(const Shape& other);
    Shape& operator=(const Shape& other);

    inline double DegToRad(double angle);

};

class Rectangle:public Shape
{
public:
    explicit Rectangle(COLORS color, ilrd::Point arr[2], double angle);//point 1 is 'up-left' corner, point 2 is 'down-right'.
    virtual ~Rectangle();

    virtual Shape& Draw();

private:
    // no cctor and no copy=
    Rectangle(const Rectangle& other);
    Rectangle& operator=(const Rectangle& other);

    virtual double Area();
    inline double GetHight();
    inline double GetWidth();

    ilrd::Point m_corners[2];
};

class Circle : public Shape
{
public:
    explicit Circle(COLORS color, ilrd::Point position, double angle, size_t radius);
    virtual ~Circle();

    virtual Shape& Draw();

private:
    Circle(const Circle& other);
    Circle& operator=(const Circle& other);

    virtual double Area();

    double m_radius;
};


class Group : public Shape
{
public:
    Group();
    explicit Group(Shape* shape);
    //virtual ~Group();

    virtual Shape& Draw();
    virtual Shape& Move(double dx, double dy);
    virtual Shape& Rotate(double angle);

    void Add(Shape* shape);
    void Remove(Shape* shape);

private:
    Group(const Group& other);
    Group& operator=(const Group& other);

    virtual double Area();
    void CalcCenterOfGravity();
    int FindShape(Shape* shape, int size);
    void Swap(int size, int i);

    std::vector<Shape*> m_shapes;
};

} // namespace hrd11

#endif //__HRD11_SHAPES_HPP__
