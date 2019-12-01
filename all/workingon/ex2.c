
#include <stdio.h>

int main()
{
    bool b1 = true;
    bool b2;
    int i = b1;
    printf("-- b2 %d\n", b2);
    printf("-- i %d\n", i);

    i = -8;
    b2 = i;

    printf("-- b1 %d\n", b1);

    return 0;
}
