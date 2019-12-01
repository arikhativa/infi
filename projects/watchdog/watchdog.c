
#include <sys/types.h>  /* getppid() */
#include <unistd.h>     /* getppid() */
#include <signal.h>     /* kill() */

#define FAIL        -1
#define SUCCESS     0


/****************************************** Declarations from sherad object: */
int SetSignals(void);
void UpdateGlobaPID(int new_id);
void SetSchedInfo(char *file_name, char **argv);
int InitSched(void);
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

    UpdateGlobaPID(getppid());
    SetSchedInfo(av[0], av);

    if (FAIL == InitSched())
    {
        return FAIL;
    }

    if (-1 == kill(getppid(), SIGUSR1))
    {
        return FAIL;
    }

    WatchProcess();

    return 0;
}
