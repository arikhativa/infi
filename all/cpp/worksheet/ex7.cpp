
#include <iostream>



//void Foo(int tmp, int i, float f)
void Foo(int tmp, int i = 3, float f = 4)
{
    std::cout << "-- tmp is " << tmp << std::endl;
    std::cout << "-- i is " << i << std::endl;
    std::cout << "-- f is " << f << std::endl << std::endl;
}
int main()
{
    Foo(5);
    Foo(5, 6);
    Foo(5, 6, 7);

    return 0;
}
