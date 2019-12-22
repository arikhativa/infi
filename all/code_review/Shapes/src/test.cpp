#include "shapes.hpp"
#include "point.hpp"  // ilrd::Point
#include "glut_utils.h"


using namespace ilrd;
using namespace hrd11;

Group gr;

static void DrawFunction()
{
    
    Point rec_points[2] = {Point(450,450), Point(550,550)};
    Rectangle rec(COLOR_BLUE, rec_points, 45);
    Circle cir(COLOR_RED, Point(500,500), 0, 50);
    cir.Draw();
    rec.Draw();
    gr.Draw();
    for(int i = 0; i < 11; ++i)
    {
        cir.Revolve(30, Point(300,500));
        cir.Draw();
        rec.Revolve(30, Point(300,500));
        rec.Draw();
        gr.Rotate(45);
        gr.Draw();
    }
}


int main(int argc, char** argv)
{
    DrawInit(argc, argv, 1000, 1000, DrawFunction);

    Circle cir1(COLOR_RED, Point(700,700), 0, 50);
    Circle cir2(COLOR_GREEN, Point(800,800), 0, 50);
    Circle cir3(COLOR_BLUE, Point(900,900), 0, 50);
    Circle cir4(COLOR_MAGENTA, Point(600,600), 0, 50);

    gr.Add(&cir1);
    gr.Add(&cir2);
    gr.Add(&cir3);
    gr.Add(&cir4);

    DrawMainLoop();
    return 0;
}