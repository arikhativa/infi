
#include <stdio.h>

int Foo(int i);

int main()
{
    int k = 5;

    printf("-- k is %d\n", k);
    k = Foo(k);

    for (int i = 0; i < 2; ++i)
    {
        printf("-- k + i is %d\n", k + i);
    }

    return 0;
}

int Foo(int i)
{
    ++i;

    return i;
}
