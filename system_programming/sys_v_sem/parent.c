
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

static int g_sem_id;

void AtExit(void)
{
    SemVDestroy(g_sem_id);
}

static void Handlr(int signum, siginfo_t *siginfo, void *context)
{
	(void)context;
    (void)signum;

    Print("  Pong!\n");
	kill(siginfo->si_pid, SIGUSR2);
}

int main()
{
    struct sigaction sig1 = {0};
    pid_t child = 0;
    char **dump;

	sig1.sa_sigaction = Handlr;
	sig1.sa_flags = SA_SIGINFO;

	sigaction(SIGUSR1, &sig1, NULL);

    //atexit(AtExit);

    child = fork();
    if (0 == child)
    {
        execvp("./child.out", dump);
    }

    g_sem_id = SemVCreate(KEY, 0660 | IPC_CREAT);
    SemVInit(g_sem_id);

    Print("-- created sem\n\n");

    while (1)
    {
        pause();
    }
    Print("-- i'm here! \n\n");

}
