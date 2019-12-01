#include <sys/types.h>	/* getppid */
#include <unistd.h>		/* getppid */
#include <stdio.h>		/* printf */

#define DOG 	(0)
#define FAIL 	(-1)

typedef enum errors
{
	COMM
} errors_t;

void InitArguments(pid_t pid, char *filename, char **args);
int CheckFirstConnection(int is_app);
int RunCommunication(void);
void CleanUp(void);

static void Error(errors_t c);

int main(int argc, char *argv[])
{
	pid_t parent = {0};

	(void) argc;
	parent = getppid();

	InitArguments(parent, argv[0], argv);

	printf("\n-- dog is running\n");

	CheckFirstConnection(DOG);

	sleep(2);
	if(FAIL == RunCommunication())
	{
		Error(COMM);
		return FAIL;
	}

	/* TODO: you might do double destroy to scheduler in cleanip() */
	CleanUp();
	printf("\ndog exit successfully\n");

	return 0;
}

static void Error(errors_t c)
{
	switch (c)
	{
		case(COMM):
		{
			printf("dog failed running communication\n");
			return;
		}
	}
}
