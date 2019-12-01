
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */

static void PrintFloatBit(float number, int digits)
{
	int *ptr = 0;

	ptr = (int*)&number;
	--digits;
	while (digits + 1)
	{
		(*ptr & (1 << (digits))) ? printf("1") : printf("0");
		--digits;
		if (digits == 22 || digits == 30)
		{
			printf("-");
		}
	}
	printf("\n");
}

int main(int ac, char **av)
{
	float number = 3;

	if (2 == ac)
	{
		number = (float)atof(av[1]);
		printf("-- Number is %f - ", number);
		PrintFloatBit(number, 32);
	}
	else
	{
		printf("-- Number is %f - ", number);
		PrintFloatBit(number, 32);
	}

	return 0;
}
