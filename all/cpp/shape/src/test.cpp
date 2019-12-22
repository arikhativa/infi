

#include <iostream>
#include <stdio.h>
#include <mcheck.h>

#include "glut_utils.h"

#include "group.hpp"    // Group
#include "shape.hpp"    // Shape
#include "circle.hpp"   // Circle
#include "rectangle.hpp"   // Rectangle

#define SCREEN_X 1000
#define SCREEN_Y 1000

using namespace hrd11;
using namespace ilrd;

static void DrawFunction();
static int KeyboardFunction(unsigned char key, int x, int y);
static int MouseFunction(int button, int state, int x, int y);
static int MotionFunction(int x, int y);
static int TimerFunction();

static Shape* car1;
static Shape* car2;
static Shape* car3;
static Shape* car4;

static Shape* CreateCar(COLORS color_car, COLORS color_wheel)
{
    Group* group = 0;
    Point p1(0, 0);
    Point p2(200, 50);
    Point arr[2];
    Shape* tmp = 0;

    arr[0] = p1;
    arr[1] = p2;

    tmp = new Rectangle(arr, color_car);
    group = new Group(tmp);

    double top_x1 = 0;
    double top_y1 = 0;
    double top_x2 = 140;
    double top_y2 = 40;

    p1 = Point(top_x1, top_y1);
    p2 = Point(top_x2, top_y2);
    arr[0] = p1;
    arr[1] = p2;

    int wheal_rad = 20;
    int wheal_x = 40;
    int wheal_y = 50;

    tmp = new Circle(wheal_rad, color_wheel);
    tmp->Move(wheal_x, wheal_y);
    group->Add(tmp);

    tmp = new Circle(wheal_rad, color_wheel);
    tmp->Move(wheal_x * 4, wheal_y);
    group->Add(tmp);

    tmp = new Rectangle(arr, color_car);
    tmp->Move(30, -40);
    group->Add(tmp);

    return static_cast<Shape*>(group);
}

int main(int argc, char** argv)
{
    car1 = CreateCar(COLOR_BLUE, COLOR_YELLOW);
    car1->Move(-200, 100);
    car2 = CreateCar(COLOR_MAGENTA, COLOR_GREEN);
    car2->Move(-200, 500);
    car3 = CreateCar(COLOR_CYAN, COLOR_RED);
    car3->Move(600, 600);

    Point p1(0, 0);
    Point p2(250, 100);
    Point arr[2];
    arr[0] = p1;
    arr[1] = p2;
    car4 = new Rectangle(arr, COLOR_RED);
    Group* group = new Group(car4);
    car4 = new Circle(50, COLOR_RED);
    group->Add(car4);
    car4 = group;
    car4->Move(100, 100);

    DrawInit(argc, argv, SCREEN_X, SCREEN_Y, DrawFunction);
    DrawSetKeyboardFunc(KeyboardFunction);
    DrawSetMouseFunc(MouseFunction);
    DrawSetMotionFunc(MotionFunction);
    DrawSetTimerFunc(TimerFunction, 3);
    DrawMainLoop();

    return 0;
}

static void DrawFunction()
{
   car1->Draw();
   car2->Draw();
   car3->Draw();
   car4->Draw();
}

static double car1_x = 2;
static double car1_y = 0;
static double car2_x = 3;
static double car2_y = 2;

static void FlipCar(Shape* car)
{
    car->Move(-(SCREEN_X + 300), 100);
}

static void FlipCar2(Shape* car)
{
    car->Move(0, -200);

    car2_x *= -1;
    car2_y *= -1;
}

static int TimerFunction()
{
    static int flag = 1;
    static int bbb = 450;

    static int i = 1;
    car4->Rotate(i);

    car3->Revolve(0.5, Point(bbb, bbb));

    if (Shape::GetCenter(*car1).GetX() > SCREEN_X + 200)
    {
        FlipCar(car1);
        flag = 1;
    }

    if (Shape::GetCenter(*car2).GetX() > SCREEN_X + 200 ||
        Shape::GetCenter(*car2).GetX() < -(SCREEN_X + 200))
    {
        FlipCar2(car2);
    }

    car1->Move(car1_x, car1_y);
    if (flag)
    {
        car2->Move(car2_x, car2_y);
    }

    if ((Shape::GetCenter(*car1).GetX() > SCREEN_X + 200 || Shape::GetCenter(*car1).GetX() < -(SCREEN_X + 200)) &&
        (Shape::GetCenter(*car1).GetY() > SCREEN_Y + 200 || Shape::GetCenter(*car1).GetY() < -(SCREEN_Y + 200)))
        {
            car1->Move(-(Shape::GetCenter(*car1).GetX()), -(Shape::GetCenter(*car1).GetY()));
            car2->Move(-(Shape::GetCenter(*car2).GetX() + 500), -(Shape::GetCenter(*car2).GetY()));
        }


    return 1;  /* draw */
}


static int KeyboardFunction(unsigned char key, int x, int y)
{
    printf("Keyboard: %02x,%d,%d\n", key, x, y);

    (void)x;
    (void)y;

    if (key == 0x1b /* ESC */)
        return -1;

    return 0;
}


static int MouseFunction(int button, int state, int x, int y)
{
    /* printf("Mouse: %d,%d,%d,%d\n", button, state, x, y); */

    (void)x;
    (void)y;
    (void)state;
    (void)button;

    if (state == 1 && button == MOUSE_WHEEL_UP)
    {
        return 1;
    }
    if (state == 1 && button == MOUSE_WHEEL_DOWN)
    {
        return 1;
    }

    if (button == MOUSE_LEFT)
    {
        return 1;
    }

    return 0;
}


static int MotionFunction(int x, int y)
{
    printf("Mouse: %d,%d\n", x, y);

    return 0;
}
