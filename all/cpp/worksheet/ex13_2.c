
#include <iostream>


void Foo(char *str)
{
    std::cout << str << "not!\n";
}

void Bar()
{
    char str[] = "bar\n";

    Foo(str);
}
