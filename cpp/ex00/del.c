
#include <iostream>


int &Ref(void)
{
    int ii = 12;
    int &ref = ii;
    std::cout << "-- addres of ii       " << &ii <<"\n";
    std::cout << "-- addres of ref      " << &ref <<"\n";

    return ref;
}

int &Ref2(void)
{
    int ii = 8;
    int &ref = ii;
    std::cout << "-- addres of ii2      " << &ii <<"\n";
    std::cout << "-- addres of ref2     " << &ref <<"\n";

    return ref;
}

int main()
{
    int i = 15;
    int& r = i;

    ++i;


    std::cout << "-- addres if r2       " << &r <<"\n";
    std::cout << "-- value of r2        " << r <<"\n";
    return 0;
}
