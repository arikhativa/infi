
#include <sys/types.h>  /* getppid() */
#include <unistd.h>     /* getppid() */
#include <signal.h>     /* kill() */

#define FAIL        -1
#define SUCCESS     0

/****************************************** Declarations from sherad object: */
int SetSignals(void);
void UpdatePID(int new_id);
void UpdateSchedInfo(char *file_name, char **argv);
int BuildSched(void);
void WatchProcess(void);


/********************************************************************* Main: */
int main(int ac, char **av)
{
    int stt = 0;

    stt = SetSignals();
    if (SUCCESS != stt)
    {
        return FAIL;
    }

    UpdatePID(getppid());
    UpdateSchedInfo(av[0], av);

    if (FAIL == BuildSched())
    {
        return FAIL;
    }

    kill(getppid(), SIGUSR1);

    WatchProcess();

    return 0;
}
