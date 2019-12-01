
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int Exec(char *file_name, char **params)
{
    pid_t cur = 0;

    cur = fork();
    if (!cur)
    {
        execvp(file_name, params);
    }
    wait(0);

    return 0;
}

int System(char *str)
{
    system(str);

    return 0;
}

int main(int ac, char **av)
{
    char *tmp = " ";
    pid_t cur = 0;
    int i = 0;
    char str[200] = {0};
    int flag_s = 0;
    int flag_e = 0;

    if (3 > ac)
    {
        printf("-- this program needs at least 2 params.\n");
        return -1;
    }

    flag_s = strcmp("system", av[1]);
    flag_e = strcmp("execvp", av[1]);

    if (!flag_s)
    {
        for (i = 2; av[i]; ++i)
        {
            strcat(str, av[i]);
            strcat(str, " ");
        }
        System(str);
    }
    else if (!flag_e)
    {
        if (ac == 3)
            Exec(av[2], &tmp);
        else
            Exec(av[2], &av[3]);
    }

    return 0;
}
