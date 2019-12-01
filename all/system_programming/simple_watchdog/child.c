
#include <stdio.h>
#include <unistd.h>

int main(int ac, char **av)
{
        while (1)
        {
            sleep(10);
        }
        printf("-- %s\n", av[0]);
}
