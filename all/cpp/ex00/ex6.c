
#include <dlfcn.h>			/* dlopen(), dlclose, dlsym */
#include <stdio.h>			/* printf() */

typedef void (*fp_t)(void);


int main(int ac, char **av)
{
    void *handle = 0;
    fp_t func = 0;

    if (ac < 3)
    {
        printf("-- fail\n-- not eanogh args\n");
        return -1;
    }

    handle = dlopen(av[1], RTLD_LAZY);
    if (!handle)
    {
        printf("-- fail\n-- no lib---.so \n");
        return -1;
    }
    func = dlsym(handle, av[2]);
    if (!func)
    {
        printf("-- fail\n-- no func\n");
        return -1;
	}

    func();

    dlclose(handle);

    return 0;
}
