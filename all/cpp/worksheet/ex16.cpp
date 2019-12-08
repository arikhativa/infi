
#include <iostream>

int &Foo(int &i)
{
    return i;
}

int main()
{
    int i = 8;
    int b = 0;
    int c = 10;

    b = Foo(i);
    Foo(i) = c;
    std::cout << "this is i " << i << std::endl;


    return 0;
}

//ref = 9;
