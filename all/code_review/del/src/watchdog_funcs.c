#define _POSIX_C_SOURCE 199309L

#include <signal.h>		/* kill, sigaction */
#include <stdlib.h>		/* exit */
#include <sys/types.h>	/* fork, semget */
#include <sys/wait.h>	/* wait */
#include <sys/sem.h>	/* semget, sem_t */
#include <sys/ipc.h>	/* semget */
#include <unistd.h>		/* fork, execvp */
#include <string.h>		/* strlen, memcpy */
#include <stdio.h>		/* printf */
#include <assert.h>		/* assert */

#include "scheduler.h"
#include "wd.h"

#define KEY 2
#define RUN_TIME 2

typedef enum dog_status
{
	FAILED = -1,
	SUCCESS
} dog_status_t;

static op_sch_t *g_sch = 0;
static char g_filename[25] = {0};
static char **g_args = 0;
static pid_t g_pid = 0;
static int g_alive = 0;

void InitArguments(pid_t pid, char *filename, char **args);
void UpdatePid(pid_t pid);
int CreateProccess(void);
int CheckFirstConnection(int is_app);
int RunCommunication(void);
void CloseSched(void);

static int Task1(void *param);
static int Task2(void *param);
static void BringToLife(int signum);
static void CallOfLife(int signum);
static void Wait(int signum);

void InitArguments(pid_t pid, char *filename, char **args)
{
	struct sigaction handler[3] = {0};

	assert(filename && args);
	
	handler[0].sa_handler = BringToLife;
	handler[0].sa_flags = SA_SIGINFO;
	handler[1].sa_handler = CallOfLife;
	handler[1].sa_flags = SA_SIGINFO;
	handler[2].sa_handler = Wait;
	handler[2].sa_flags = SA_SIGINFO;
	
	if(FAILED == sigaction(SIGUSR1, &handler[0], NULL))
	{
		return;
	}
	
	if(FAILED == sigaction(SIGUSR2, &handler[1], NULL))
	{
		return;
	}
	
	if(FAILED == sigaction(SIGCHLD, &handler[2], NULL))
		return;

	g_sch = OperSchedCreate();
	
	if(!g_sch)
	{
		return;
	}
	
	OperSchedAdd(g_sch, RUN_TIME, Task1, NULL);
	OperSchedAdd(g_sch, RUN_TIME * 2, Task2, NULL);
	
	memcpy(g_filename, filename, strlen(filename));
	g_args = args;
	g_pid = pid;
}

void UpdatePid(pid_t pid)
{
	g_pid = pid;
}

int CreateProccess(void)
{
	pid_t child = {0};
	
	if(FAILED == (child = fork()))
	{
		return FAIL;
	}
	
	if(!child)
	{
		if(FAILED == execvp(g_filename, g_args))
		{
			exit(-1);
		}
	}
	
	g_pid = child;
	
	return SUCCESS;
}

int CheckFirstConnection(int is_app)
{
	if(is_app)
		printf("app is here");
		
	else
		printf("dog is here ");
	
	kill(g_pid, SIGUSR1);
	
	return SUCCESS;
}

int RunCommunication(void)
{
	int status = 0;
	
	status = OperSchedRun(g_sch);
	
	if(status != 1) /* STOPPED by sched_status */
	{
		return FAILED;
	}
	
	return SUCCESS;
}

void CleanUp(void)
{
	int sem_id = 0;
	
	OperSchedDestroy(g_sch);
	sem_id = semget(KEY, 1, 0);
	
	if(FAILED != sem_id)
		semctl(sem_id, 0, IPC_RMID);
}

int Task1(void *param)
{
	(void) param;
	 
	kill(g_pid, SIGUSR2);
	return SUCCESS;
}

int Task2(void *param)
{
	(void) param;
	
	if(FAILED == semget(KEY, 1, 0))
	{
		CloseSched();
		return SUCCESS;
	}
	
	if(!g_alive)
	{
		if(FAIL == CreateProccess())
		{
			semctl(KEY, 0, IPC_RMID);
			CloseSched();
		}		
	}
	
	g_alive = 0;
	
	return SUCCESS;
}

void CallOfLife(int signum)
{
	(void) signum;
	
	kill(g_pid, SIGUSR1);
}

void BringToLife(int signum)
{
	(void) signum;
	
	++g_alive;
}

void CloseSched(void)
{
	OperSchedStop(g_sch);
}

void Wait(int signum)
{
	(void) signum;
	
	wait(NULL);
}
