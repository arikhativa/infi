
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */
#include <stddef.h> /* size_t */

#define ABSOLUTE_VALUE(num) (0 > num) ? -num : num
#define BIT_SIZE 8

static void PrintBit(unsigned int bit)
{
        int n = BIT_SIZE - 1;
        int str[BIT_SIZE];

        while (0 <= n)
        {
                str[n] = (bit % 2);
                str[n] = ABSOLUTE_VALUE(str[n]);
                bit = bit >> 1;
                --n;
        }
        n = 0;
        while (BIT_SIZE > n)
        {
                printf("%d", str[n]);
                ++n;
        }
        printf("\n");
}

static int CheckIfBothOn(unsigned char number)
{
	return (number & (1 << 1)) && (number & (1 << 5));
}

static int CheckIfOneIsOn(unsigned char number)
{
	return (number & (1 << 1)) || (number & (1 << 5));
}

static int Swap3And5(unsigned char number)
{
	int tmp3 = 0;
	int tmp5 = 0;
	unsigned int max = 4294967295;

	tmp3 = number & (1 << 2);
	tmp5 = number & (1 << 4);
	number = number & (max ^ (1 << 2));
	number = number & (max ^ (1 << 4));
	number = number ^ (tmp3 << 2);
	number = number ^ (tmp5 >> 2);

	return (number);
}

int main(int ac, char **av)
{
	unsigned int number = 2147483648;

	if (2 == ac)
	{
		number = (unsigned int)atoi(av[1]);
		printf("-- Number is %u - ", number);
		PrintBit(number);
		//printf("-- Both are %d\n", CheckIfBothOn(number));
		//printf("-- At least one is %d\n", CheckIfOneIsOn(number));
		printf("-- Swap the 5th bit and the 3rd bit\n-- New number   - ");
		//Swap3And5(number);
		PrintBit(Swap3And5(number));
		
	}

	return 0;
}
