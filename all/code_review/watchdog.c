#include <sys/types.h>	/* fork, semget, semctl */
#include <sys/sem.h>	/* sem_t, key_t */
#include <sys/ipc.h>	/* semget, semctl */
#include <unistd.h>		/* getpid, getppid */
#include <stdio.h>		/* printf */
#include <pthread.h>	/* create, join */
#include <assert.h>		/* assert */

#include "wd.h"

/*	TODO:	'APP' no a good name */
#define APP	(1)

/*	TODO:	bad names */
typedef enum errors
{
	SEM,
	DOG,
	THREAD,
	COMM
} errors_t;

typedef enum dog_status
{
	FAIL = -1,
	SUCCESS
} dog_status_t;

/*	TODO:	'g_'! */
static pthread_t g_thread = {0};
static key_t sem_key = 2;
static int g_semid = 0;


void InitArguments(pid_t pid, char *filename, char **args);
void UpdatePid(pid_t pid);
int CheckFirstConnection(int is_app);
int CreateProccess(void);
/*	TODO:	name */
int RunCommunication(void);
void CloseSched(void);
void CleanUp(void);

static void *ThreadCommunication(void *argv);
static void Error(errors_t c);

int WDStart(char *argv[])
{
	pid_t child = {0};

	assert(argv);

	/*	TODO:	why '0' */
	/*	TODO:	return value */
	InitArguments(0, "./watchdog.out", argv);
	g_semid = semget(sem_key, 1, 0);

	if(FAIL == g_semid)
	{
		g_semid = semget(sem_key, 1, IPC_CREAT|0666);

		if(FAIL == g_semid)
		{
			CleanUp();
			Error(SEM);
			return FAIL;
		}

		if(FAIL == CreateProccess())
		{
			CleanUp();
			Error(DOG);
			return FAIL;
		}
	}

	else
	{
		child = getppid();
		UpdatePid(child);
	}

	/*	TODO:	where is the sync func */

	/*	TODO:	this 'check' func donset chack */
	CheckFirstConnection(APP);

	if(pthread_create(&g_thread, NULL, ThreadCommunication, NULL))
	{
		CleanUp();
		Error(THREAD);
		return FAIL;
	}

	return SUCCESS;
}

void WDStop(void)
{
	semctl(g_semid, 0, IPC_RMID);
	CloseSched();
	pthread_join(g_thread, NULL);
	CleanUp();
}

static void *ThreadCommunication(void *nothing)
{
	int status = 0;

	(void) nothing;

	status = RunCommunication();

	if(FAIL == status)
	{
		CleanUp();
		Error(COMM);
	}
	return NULL;
}

static void Error(errors_t c)
{
	switch (c)
	{
		case(SEM):
		{
			printf("fail creating semaphore\n");
			return;
		}

		case(DOG):
		{
			printf("fail creating dog\n");
			return;
		}

		case(THREAD):
		{
			printf("fail thread create\n");
			return;
		}

		case(COMM):
		{
			printf("failed running communication\n");
			return;
		}
	}
}
