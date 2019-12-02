
#include <stdio.h>


int main()
{
    const int i = 3;
    int *ip = (int*)&i;

    *ip = 5;


    printf("-- &i\t%p\n-- ip\t%p\n", &i, ip);
    printf("-- i\t%d\n-- *ip\t%d\n", i, *ip);
    printf("-- i2\t%d\n-- *ip2\t%d\n", (int)i, *ip);
    return 0;
}
