#include <stdio.h>
#include <mcheck.h>

#include "glut_utils.h"

#include "shape.hpp"    // Shape
#include "circle.hpp"   // Circle

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

static Shape* all;

int main(int argc, char** argv)
{
    /*--------------------------- mtrace(); */

    DrawInit(argc, argv, 1000, 1000, DrawFunction);

    /* advanced: extra functionality */
    DrawSetKeyboardFunc(KeyboardFunction);
    DrawSetMouseFunc(MouseFunction);
    DrawSetMotionFunc(MotionFunction);
    DrawSetTimerFunc(TimerFunction, 5);

    DrawMainLoop();

    printf("exit\n");

    delete all;

    return 0;
}

static void DrawFunction()
{
    all = new Circle(100, COLOR_RED);

    all->Draw();


    DrawPolygon(COLOR_MAGENTA, 3, (int)150 + xPoly, (int)400, (int)150 + xPoly, (int)650, (int)300 + xPoly, (int)650);

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


static int TimerFunction()
{
    all->Move(55, 55);

    xPoly += 10;

    return 1;  /* draw */
}
