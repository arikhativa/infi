
#include <unistd.h>

#include "watchdog_funcs.h"
#include "write_print.h"

int main(int ac, char **av)
{
    int i = 0;

    if (-1 == WDStart(av))
    {
        Print("-- Fail\n\n");
    }
    else
    {
        Print("-- Success!\n\n");
    }

    while (i < 50)
    {
        sleep(1);
        ++i;
    }

    WDStop();

    return 0;
}
