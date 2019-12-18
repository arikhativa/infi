

#include <iostream>
#include <stdio.h>
#include <mcheck.h>

#include "glut_utils.h"

#include "group.hpp"    // Group
#include "shape.hpp"    // Shape
#include "circle.hpp"   // Circle
#include "rectangle.hpp"   // Rectangle

using namespace hrd11;
using namespace ilrd;

int drawCircle = 1;
int xCircle = 250;
int yCircle = 100;
int xPoly = 0;
int yPoly = 0;
double rCircle = 100;

static void DrawFunction();
static int KeyboardFunction(unsigned char key, int x, int y);
static int MouseFunction(int button, int state, int x, int y);
static int MotionFunction(int x, int y);
static int TimerFunction();

static Shape* s1;
static Shape* s2;
static Shape* s3;
static Shape* s4;
static Shape* s5;
static Shape* s6;

int main(int argc, char** argv)
{
    Point p1(0, 0);
    Point p2(200, 200);
    Point arr[2];
    arr[0] = p1;
    arr[1] = p2;

    size_t rad = 20;
    size_t bbb = 200;

    Group* tmp = 0;

    //s1 = new Rectangle(arr, COLOR_BLUE);
    s2 = new Circle(rad, COLOR_BLUE);

    // std::cout << s1->CalculateArea();
    // std::cout << std::endl;

    s3 = new Circle(rad, COLOR_GREEN);
    s3->Move(bbb/2, bbb);
    s4 = new Circle(rad, COLOR_RED);
    s4->Move(bbb, bbb);
    s5 = new Circle(rad, COLOR_WHITE);
    s5->Move(bbb/2, 0);
    s6 = new Circle(rad / 5, COLOR_CYAN);
    s6->Move(bbb / 2, bbb / 2);

    tmp = new Group(s2);
    tmp->Add(s3);
    tmp->Add(s4);
    tmp->Add(s5);
    tmp->Add(s6);
    s1 = tmp;

    s1->Move(100, 100);
    // tmp->CalculateCenter();

    DrawInit(argc, argv, 1000, 1000, DrawFunction);
    DrawSetKeyboardFunc(KeyboardFunction);
    DrawSetMouseFunc(MouseFunction);
    DrawSetMotionFunc(MotionFunction);
    DrawSetTimerFunc(TimerFunction, 10);
    DrawMainLoop();

    return 0;
}

static void DrawFunction()
{
    s1->Draw();
}

static int TimerFunction()
{
    static double i = 30;

    s1->Rotate(i);

    //s1->Revolve(i, Point(400, 400));


    // xPoly += 10;

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
        rCircle *= 1.1;
        return 1;
    }
    if (state == 1 && button == MOUSE_WHEEL_DOWN)
    {
        rCircle *= 0.9;
        return 1;
    }

    if (button == MOUSE_LEFT)
    {
        drawCircle = state;
        return 1;
    }

    return 0;
}


static int MotionFunction(int x, int y)
{
    printf("Mouse: %d,%d\n", x, y);

    return 0;
}
