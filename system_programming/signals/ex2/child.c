
#define _POSIX_C_SOURCE 199309L

#include <sys/types.h>	/*	getpid()	*/
#include <unistd.h>		/*	write()		*/
#include <signal.h>		/*	sigaction()	*/
#include <sys/wait.h>	/*	waitpid()	*/

void Sig2(int a)
{
	char str[] = "Ping\n";

	a = write(1, str, sizeof(str));
	
	(void)a;
}

int main()
{
	pid_t child = 0;
	struct sigaction sig2 = {0};
	volatile int i = 1;

	sig2.sa_handler = Sig2;

	sigaction(SIGUSR2, &sig2, NULL);

	while (i < 10 && !child)
	{
		kill(getppid(), SIGUSR1);
		pause();
		++i;
	}

	return 0;
}
