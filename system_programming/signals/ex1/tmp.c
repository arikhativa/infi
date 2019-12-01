
#define _POSIX_C_SOURCE 199309L

#include <sys/types.h>	/*	getpid()	*/
#include <unistd.h>		/*	write()		*/
#include <signal.h>		/*	sigaction()	*/
#include <sys/wait.h>	/*	waitpid()	*/

#include "write_print.h"

void Sig1(int a)
{
	char str[] = "Pong\n";
	int nothing = 0;

	(void)a;

	PutStr("-- the sig1	- ");
	PutNum((ssize_t)&nothing);
	PutChar('\n');
	nothing = write(1, str, sizeof(str));

	(void)nothing;
}

void Sig2(int a)
{
	int nothing = 0;
	char str[] = "Ping\n";

	PutStr("-- the sig2	- ");
	PutNum((ssize_t)&nothing);
	PutChar('\n');
	(void)a;

	nothing = write(1, str, sizeof(str));

	(void)nothing;
}

static void Child(void)
{
	int i = 0;

	while (i < 5)
	{
		kill(getppid(), SIGUSR1);
		pause();
		/*sleep(1);*/
		++i;
	}
}

static void Parent(pid_t child)
{
	volatile int i = 0;
	int stt = 0;

	while (i < 5)
	{
		kill(child, SIGUSR2);
		waitpid(child, &stt, WUNTRACED);
		/*sleep(1);*/
		++i;
	}
}

int main()
{
	pid_t child = 0;
	struct sigaction sig1 = {0};
	struct sigaction sig2 = {0};

	PutStr("-- The stack	- ");
	PutNum((ssize_t)&child);
	PutChar('\n');
	PutChar('\n');

	sig1.sa_handler = Sig1;
	sig2.sa_handler = Sig2;

	sigaction(SIGUSR1, &sig1, NULL);
	sigaction(SIGUSR2, &sig2, NULL);

	child = fork();

	if (!child)
	{
		Child();
	}
	else
	{
		pause();
		Parent(child);
	}

	return 0;
}
