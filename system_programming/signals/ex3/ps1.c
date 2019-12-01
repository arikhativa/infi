
#define _POSIX_C_SOURCE 199309L

#include <sys/types.h>	/*	getpid()	*/
#include <unistd.h>		/*	write()		*/
#include <signal.h>		/*	sigaction()	*/
#include <sys/wait.h>	/*	waitpid()	*/

static void Handlr(int signum, siginfo_t *siginfo, void *context)
{
	char str[] = "Pong\n";

	(void)context;

	kill(siginfo->si_pid, SIGUSR2);

	signum = write(1, str, sizeof(str));
	(void)signum;
}

int main()
{
	struct sigaction sig1 = {0};
	volatile int i = 1;

	sig1.sa_sigaction = Handlr;
	sig1.sa_flags = SA_SIGINFO;

	sigaction(SIGUSR1, &sig1, NULL);

	while (i < 9)
	{
		pause();
		++i;
	}

	return 0;
}
