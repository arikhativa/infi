
#define _POSIX_C_SOURCE 199309L

#include <sys/types.h>	/*	getpid()	*/
#include <unistd.h>		/*	write()		*/
#include <signal.h>		/*	sigaction()	*/
#include <sys/wait.h>	/*	waitpid()	*/

void Sig1(int a)
{
	char str[] = "Pong\n";

	a = write(1, str, sizeof(str));
	
	(void)a;
}

int main()
{
	pid_t child = 0;
	struct sigaction sig1 = {0};
	char *str = "";
	volatile int i = 1;
	int stt = 0;

	sig1.sa_handler = Sig1;

	sigaction(SIGUSR1, &sig1, NULL);

	child = fork();
	if (!child)
	{
		execvp("./child.out", &str);
	}
	pause();

	while (i < 10 && child)
	{
		kill(child, SIGUSR2);
		waitpid(child, &stt, WUNTRACED);
		++i;
	}

	return 0;
}
