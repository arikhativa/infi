
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */

static void PrintBit(unsigned int number, unsigned int digits)
{
	digits = 2147483648;

	while (digits)
	{
		(number & digits) ? printf("1") : printf("0");
		digits >>= 1;
	}
	printf("\n");
}

unsigned int AddOneToNumber(unsigned int n)
{
	unsigned int carry = 1;

	while ((n & carry))
	{
		n ^= carry;
		carry <<= 1;
	}
	n ^= carry;

	return n;
}

static unsigned int CountSetBits(unsigned int number)
{
	int count = 0;
	unsigned int digit = 1 << 31;

	while (digit)
	{
		if (number & digit)
		{
			count = AddOneToNumber(count);
		}
		digit >>= 1;
	}

	return count;
}

int main(int ac, char **av)
{
	unsigned int number = 15;

	if (2 == ac)
	{
		number = (unsigned int)atoi(av[1]);
		printf("-- Number is %u - ", number);
		PrintBit((number), 32);
		printf("-- Number of bits %u\n", CountSetBits(number));
	}
	else
	{
		printf("-- Number is %u - ", number);
		PrintBit((number), 32);
		printf("-- Number of bits %u\n", CountSetBits(number));

	}

	return 0;
}
