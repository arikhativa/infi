
#include <iostream>

#include <dlfcn.h>  /* dl..() */

#include "write_print.h"

typedef void (*fp_t)(char);

void PutChar(char ch);

void Foo(){std::cout << "Foo 1\n";}
void Foo(int i){std::cout << "Foo 2\n";}
void Foo(char){std::cout << "Foo 3\n";}

void Part1()
{
    Foo();
    Foo(5);
    Foo('5');
}

void Part2()
{
    char ch[] = "-- hey part 2\n";
    char tmp = 'a';
    void *handle = dlopen("./libwpp.so", RTLD_LAZY);
    fp_t fp = (fp_t)dlsym(handle, "PutChar");

    std::cout << handle << std::endl;

    fp(tmp);

    dlclose(handle);

}

int main()
{
    //Part1();
    Part2();

    return 0;
}
