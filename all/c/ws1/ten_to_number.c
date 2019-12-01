
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

double TenToNumber(int exponent)
{
	double result = 1;
	double base = 10;

	if (exponent < 0)
	{
		exponent = -exponent;
		base = 0.1;
	}
	while (--exponent > 0)
	{
		result *= base;
	}
	return result;
}

int main(int ac, char **av)
{
	if (ac == 2)
	{
		ac = atoi(av[1]);
		printf("%f\n", TenToNumber(ac));
	}
	return 0;
}
