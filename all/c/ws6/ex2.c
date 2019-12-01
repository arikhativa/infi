
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */

static unsigned int SubtractOneFromNumber(unsigned int n)
{
        unsigned int carry = 1;

        while (0 == (n & carry))
        {
                n = n | carry;
                carry = carry << 1;
        }
        n = n ^ carry;

        return n;
}

static int IsItPowerOfTwo1(unsigned int n)
{
	if (0 == n)
	{
		return 0;
	}
	while (1 < n)
	{
		if (1 == (n & 1))
		{
			return 0;
		}
		n = n >> 1;
	}

	return 1;
}   

static int IsItPowerOfTwo2(unsigned int n)
{
	int ret = 0;

	if (0 == n)
	{
		return 0;
	}
	ret = (((n & (SubtractOneFromNumber(n))) == 0)) ? 1 : 0;

	return ret;
}   

int main(int ac, char **av)
{
	unsigned int n = 0;

	if (2 == ac)
	{
		n = (unsigned int)atoi(av[1]);
		printf("\n-- is %d a power of 2?\n", n);
		printf("-- Part1 says %d\n", IsItPowerOfTwo1(n));
		printf("-- Part2 says %d\n", IsItPowerOfTwo2(n));
	}
	else
	{
		printf("\n-- This program checks if a number is a power of 2\n");
		printf("-- Run with 1 number\n");
		printf("-- a.out [x]\n\n");
	}

	return 0;
}
