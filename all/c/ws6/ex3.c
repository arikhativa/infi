
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */

unsigned int AddOneToNumber(unsigned int n)
{
	unsigned int carry = 1;

	/*if (n == 0)
	{
		return 1;
	}*/
	while (0 != (n & carry))
	{
		n = n ^ carry;
		carry = carry << 1;
	}
	n = n ^ carry;

	return n;
}   

int main(int ac, char **av)
{
	unsigned int n = 0;

	if (2 == ac)
	{
		n = atoi(av[1]);
		printf("\n-- Before %d\n", n);
		printf("-- After %d\n", AddOneToNumber(n));
	}
	else
	{
		printf("\n-- This program adds 1 to number\n");
		printf("-- Run with 1 number\n");
		printf("-- a.out [x]\n\n");
	}

	return 0;
}
