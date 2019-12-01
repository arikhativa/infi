
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */

static unsigned int ClosestDividerTo16(unsigned int number)
{
	return (number & (~15));
}

int main(int ac, char **av)
{
	unsigned int num1 = 85;
	unsigned int num2 = 100;
	unsigned int num3 = 7;

	if (2 == ac)
	{
		num1 = (unsigned int)atoi(av[1]);
		printf("-- Number is %u\n", num1);
		printf("-- Smallest number divided by 16 without a riminder - %u\n", ClosestDividerTo16(num1));
	}
	else
	{
		printf("-- Num1 is %u - \n", num1);
		printf("-- Smallest number divided by 16 without a riminder - %u\n", ClosestDividerTo16(num1));
		printf("-- Num2 is %u - \n", num2);
		printf("-- Smallest number divided by 16 without a riminder - %u\n", ClosestDividerTo16(num2));
		printf("-- Num3 is %u - \n", num3);
		printf("-- Smallest number divided by 16 without a riminder - %u\n", ClosestDividerTo16(num3));
	}

	return 0;
}
