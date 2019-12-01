
#define _POSIX_C_SOURCE 199309L

#include <sys/types.h>	/*	getpid()	*/
#include <unistd.h>		/*	write()		*/
#include <signal.h>		/*	sigaction()	*/
#include <sys/wait.h>	/*	waitpid()	*/

void Sig1(int a)
{
	char str[] = "Pong\n";
	int nothing = 0;

	(void)a;

	/*nothing = write(1, str, 0);*/
	nothing = write(1, str, sizeof(str));

	(void)nothing;
}

void Sig2(int a)
{
	char str[] = "Ping\n";
	int nothing = 0;

	(void)a;

	nothing = write(1, str, sizeof(str));
	/*nothing = write(1, str, 0);*/

	(void)nothing;
}

static void Child(void)
{
	volatile int i = 1;

	while (i)
	{
		kill(getppid(), SIGUSR1);
		pause();
		sleep(10);
		++i;
	}
}

static void Parent(pid_t child)
{
	volatile int i = 1;
	int stt = 0;

	while (i)
	{
		kill(child, SIGUSR2);
		waitpid(child, &stt, WUNTRACED);
		sleep(10);
		++i;
	}
}

int main()
{
	pid_t child = 0;
	struct sigaction sig1 = {0};
	struct sigaction sig2 = {0};

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
