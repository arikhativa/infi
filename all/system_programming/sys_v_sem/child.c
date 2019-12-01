
#define _POSIX_C_SOURCE 199309L

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "sem5.h"
#include "write_print.h"

#define KEY 0x1111

static int g_cond = 0;

static void Handler(int signum)
{
    (void)signum;

    g_cond = 1;
}

int main()
{
    struct sigaction sig2 = {0};
    pid_t child = 0;
    int sem_id = 0;

    sig2.sa_handler = Handler;

	sigaction(SIGUSR2, &sig2, NULL);

    sem_id = SemVCreate(KEY, 0);

    while (1)
    {
        g_cond = 0;
        Print("  Ping!\n");
        kill(getppid(), SIGUSR1);

        sleep(1);
        sleep(1);
        sleep(1);

        if (g_cond == 0)
        {
            SemVDestroy(sem_id);
            return 0;
        }
    }

    return 0;
}
