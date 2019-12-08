
#include <iostream>

using namespace std;

void Foo(const double& i)
{

}

void Bar(double& i)
{
    double &tmp = i;
}

int main()
{
    int i = 0;

    //Foo(i);
    Bar(reinterpret_cast <double&>(i));

    return 0;
}
