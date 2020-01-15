
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

void Foo()
{
	    sleep(2);
	    
}

int main(int ac, char** av)
{
    int i = 0;

    if (ac != 2)
    {
        printf("gimmi 1 arg\n\n");
        return 1;
    }

    i = access(av[1], F_OK);

    if (-1 == i)
    {
        printf("-- no file\n");
    }
    else
    {
        printf("-- file is alive\n");
        i = access(av[1], R_OK);
        if (-1 == i)
        {
            printf("-- no read\n");
        }
        else
        {
            printf("-- can read\n");

        }
    }

    while(1)
    {
	    Foo();
	    sleep(1);
    }

    printf("\n");

    return 0;
}
