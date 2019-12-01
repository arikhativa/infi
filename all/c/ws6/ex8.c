
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */

static void SwapNumbers(unsigned int *num1, unsigned int *num2)
{
	*num1 = *num1 ^ *num2;
	*num2 = *num1 ^ *num2;
	*num1 = *num1 ^ *num2;
}

int main(int ac, char **av)
{
	unsigned int num1 = 55;
	unsigned int num2 = 7;

	if (3 == ac)
	{
		num1 = (unsigned int)atoi(av[1]);
		num2 = (unsigned int)atoi(av[2]);
		printf("-- Number1 is %u - \n", num1);
		printf("-- Number2 is %u - \n", num2);
		printf("-- Swap\n");
		SwapNumbers(&num1, &num2);
		printf("-- Number1 is %u - \n", num1);
		printf("-- Number2 is %u - \n", num2);
	}
	else
	{
		printf("-- Number1 is %u - \n", num1);
		printf("-- Number2 is %u - \n", num2);
		printf("-- Swap\n");
		SwapNumbers(&num1, &num2);
		printf("-- Number1 is %u - \n", num1);
		printf("-- Number2 is %u - \n", num2);
	}

	return 0;
}
