
#define _POSIX_C_SOURCE 199309L /* sigaction() */

#include <sys/types.h>          /* fork(), kill(), getppid() ket_t */
#include <unistd.h>             /* execvp(), pause(), fork(), getppid() */
#include <sys/wait.h>           /* wait() */
#include <stdlib.h>             /* exit() */
#include <assert.h>             /* assert() */
#include <signal.h>             /* kill(), sigaction */
#include <pthread.h>            /* pthread_create(), pthread_attr_t() */

#include "write_print.h"        /* Print() */
#include "sem5.h"               /* SemVGetID(), SemVCreate(), SemVDestroy() */
#include "scheduler.h"          /* OperSched.. Create()/Stop()Destroy()/Add() */
#include "watchdog_funcs.h"

typedef enum proc_status
{
    PROC_FAIL = -1,
    DEAD = 0,
    ALIVE
} proc_stt_t;

typedef enum watch_status
{
    WATCH = 0,
    DONT_WATCH
} watch_status_t;

#define KEY         0x3366

#define FAIL        -1
#define SUCCESS     0

#define CHILD_PROC  0
#define NO_WATCHDOG 55      /* '55' is a random number */

typedef struct sched_info
{
    char *file_name;
    char **argv;

} sched_info_t;

/*************************************************************** Global var: */
static proc_stt_t g_proc_stt = DEAD;
static int g_proc_pid;

op_sch_t *g_sched;
sched_info_t g_sched_info = {0};

static pthread_t g_thread = {0};

static struct sigaction g_sig1 = {0};
static struct sigaction g_sig2 = {0};
static struct sigaction g_sig3 = {0};


/********************************************************* Declarations var: */
static int CreateProcess(char *exec_name, char **argv);
static int LookForWatchdog(void);
static int SyncProcesses(void);
static int SendSig2(void *param);
static int CheakProcess(void *param);
static void *Handler(void *data);
static int CreateThread(void);
int SetSignals(void);
void UpdatePID(int new_id);
void UpdateSchedInfo(char *file_name, char **argv);
int BuildSched(void);
void WatchProcess(void);


/************************************************************* Signal funcs: */
void SetProcessAlive(int sig)   /* SIGUSR1 */
{
    (void)sig;

    g_proc_stt = ALIVE;
}

void RespondToSig2(int signum, siginfo_t *siginfo, void *context) /* SIGUSR2 */
{
    (void)context;
    (void)signum;

    kill(siginfo->si_pid, SIGUSR1);
}

void WaitForChild(int sig)     /* SIGCHLD */
{
    (void)sig;

    g_proc_stt = PROC_FAIL;

    wait(NULL);
}


/************************************************************* Static funcs: */
static int LookForWatchdog(void)
{
    int sem_id = SemVGetID(KEY);

    if (-1 == sem_id)
    {
        sem_id = SemVCreate(KEY);
        if (-1 == sem_id)
        {
            return FAIL;
        }

        return NO_WATCHDOG;
    }

    g_proc_pid = getppid();
    kill(getppid(), SIGUSR1);

    return SUCCESS;
}

static int SyncProcesses(void)
{
    while (DEAD == g_proc_stt)
    {
        /* '2' is a random number.
            pause() is no good here because there might be
            a context switch between checking 'g_proc_stt',
            and getting into pause().   */
        sleep(2);
    }

    if (PROC_FAIL == g_proc_stt)
    {
        return FAIL;
    }

    return SUCCESS;
}

static int SendSig2(void *param)
{
    (void)param;

    kill(g_proc_pid, SIGUSR2);

    return SUCCESS;
}

static int CreateProcess(char *exec_name, char **argv)
{
    pid_t child_pid = {0};

    assert(exec_name);
    assert(argv);

    child_pid = fork();
    if (-1 == child_pid)
    {
        return FAIL;
    }

    if (CHILD_PROC == child_pid)
    {
        if (-1 == execvp(exec_name, argv))
        {
            exit(FAIL);
        }
    }

    UpdatePID(child_pid);

    return SUCCESS;
}

static int CheakProcess(void *param)
{
    sched_info_t *info = param;
    int sem_id = SemVGetID(KEY);

    assert(param);

    if (-1 == sem_id)
    {
        OperSchedStop(g_sched);
        return SUCCESS;
    }

    if (DEAD == g_proc_stt)
    {
        if (FAIL == CreateProcess(info->file_name, info->argv))
        {
            Print("\n-- error\n-- failed to run '%s'\n", info->file_name);
            SemVDestroy(SemVGetID(KEY));
            OperSchedStop(g_sched);
            return FAIL;
        }

        if (FAIL == SyncProcesses())
        {
            Print("\n-- error\n-- failed to run '%s'\n", info->file_name);
            SemVDestroy(SemVGetID(KEY));
            OperSchedStop(g_sched);
            return FAIL;
        }
    }

    g_proc_stt = DEAD;

    return SUCCESS;
}


/************************************************************* Thread funcs: */
static void *Handler(void *data)
{
    (void)data;

    WatchProcess();

    return NULL;
}

static int CreateThread(void)
{
    if (pthread_create(&g_thread, NULL, &Handler, NULL))
    {
        return FAIL;
    }

    return SUCCESS;
}


/****************************************************** Sheard Object funcs: */
int SetSignals(void)
{
    int stt = 0;

    g_sig1.sa_handler = SetProcessAlive;
    stt = sigaction(SIGUSR1, &g_sig1, NULL);

    g_sig2.sa_sigaction = RespondToSig2;
	g_sig2.sa_flags = SA_SIGINFO;
    stt += sigaction(SIGUSR2, &g_sig2, NULL);

    g_sig3.sa_handler = WaitForChild;
    stt += sigaction(SIGCHLD, &g_sig3, NULL);

    return stt;
}

void UpdatePID(int new_id)
{
    g_proc_pid = new_id;
}

void UpdateSchedInfo(char *file_name, char **argv)
{
    assert(file_name);
    assert(argv);

    g_sched_info.file_name = file_name;
    g_sched_info.argv = argv;
}

int BuildSched(void)
{
    unique_id_t task_uid = {0};

    g_sched = OperSchedCreate();

    if (NULL == g_sched)
    {
        return FAIL;
    }

    task_uid = OperSchedAdd(g_sched, 4, SendSig2, NULL);
    if (UidIsSame(bad_uid, task_uid))
    {
        OperSchedDestroy(g_sched);
        return FAIL;
    }

    task_uid = OperSchedAdd(g_sched, 5, CheakProcess, &g_sched_info);
    if (UidIsSame(bad_uid, task_uid))
    {
        OperSchedDestroy(g_sched);
        return FAIL;
    }

    return SUCCESS;
}

void WatchProcess(void)
{
    OperSchedRun(g_sched);

    OperSchedDestroy(g_sched);
}


/**************************************************************** API funcs: */
int WDStart(char *argv[])
{
    int stt = 0;

    assert(argv);

    UpdateSchedInfo("./watchdog.out", argv);

    stt = SetSignals();
    if (SUCCESS != stt)
    {
        return FAIL;
    }

    if (BuildSched())
    {
        return FAIL;
    }

    stt = LookForWatchdog();
    if (FAIL == stt)
    {
        OperSchedDestroy(g_sched);
        return FAIL;
    }

    if (NO_WATCHDOG == stt)
    {
        stt = CreateProcess("./watchdog.out", argv);
        if (-1 == stt)
        {
            OperSchedDestroy(g_sched);
            SemVDestroy(SemVGetID(KEY));
            return FAIL;
        }

        stt = SyncProcesses();
        if (FAIL == stt)
        {
            OperSchedDestroy(g_sched);
            SemVDestroy(SemVGetID(KEY));
            return FAIL;
        }
    }

    stt = CreateThread();
    if (FAIL == stt)
    {
        OperSchedDestroy(g_sched);
        SemVDestroy(SemVGetID(KEY));
        kill(g_proc_pid, SIGKILL);
        return FAIL;
    }

    return SUCCESS;
}

void WDStop(void)
{
    SemVDestroy(SemVGetID(KEY));

    OperSchedStop(g_sched);

    pthread_join(g_thread, NULL);
}
