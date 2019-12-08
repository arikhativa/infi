
#include <iostream>

#include "del.hpp"

template<>
void Foo(char ch)
{
    std::cout << "int Spec\n\n";
}
