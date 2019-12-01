
#include <iostream>

enum E
{
    AA,
    BB,
    CC,
    DD
};

int main()
{
    enum E e = 1;

    ++e;

    int i = 2;


    std::cout << "-- i" << i << "\n";
    std::cout << "-- e" << e << "\n";
    /*printf("-- e %d\n", e);
    printf("-- i %d\n", i);*/

    return 0;
}
