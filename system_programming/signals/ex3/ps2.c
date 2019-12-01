
#define _POSIX_C_SOURCE 199309L

#include <unistd.h>		/*	write()		*/
#include <signal.h>		/*	sigaction()	*/
#include <stdlib.h>		/*	atoi()	*/

void Sig2(int a)
{
	char str[] = "Ping\n";
	int dump = 0;

	(void)a;

	dump = write(1, str, sizeof(str));
	(void)dump;
}

int main(int ac, char **av)
{
	int dump = 0;
	char str[] = "Two Args Plz\n";
	struct sigaction sig2 = {0};
	volatile int i = 1;

	if (2 > ac)
	{
		dump = write(1, str, sizeof(str));
		(void)dump;
		return -1;
	}

	sig2.sa_handler = Sig2;

	sigaction(SIGUSR2, &sig2, NULL);

	while (i < 10)
	{
		kill(atoi(av[1]), SIGUSR1);
		pause();
		++i;
	}

	return 0;
}
