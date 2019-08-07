
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int FlipInt(int a)
{
	int result = 0;

	while (a != 0)
	{
		result = (result * 10) + (a % 10);
		a /= 10;
	}
	return result;
}


int main(int ac, char **av)
{
	ac = atoi(av[1]);
	printf("%d\n", FlipInt(ac));
	return 0;
}

