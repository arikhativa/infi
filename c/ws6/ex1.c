
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */

static long pow2(unsigned int x, unsigned int y)
{
	x = x << y;

	return (long)x;
}   

int main(int ac, char **av)
{
	unsigned int x = 0;
	unsigned int y = 0;

	if (3 == ac)
	{
		x = (unsigned int)atoi(av[1]);
		y = (unsigned int)atoi(av[2]);
		printf("%d * (2^%d) == %lu\n", x, y, pow2(x, y));
	}
	else
	{
		printf("\n-- This program does this:\n");
		printf("-- x * (2 ^ y)\n");
		printf("-- Run with 2 numbers\n");
		printf("-- a.out [x] [y]\n\n");
	}

	return 0;
}
