
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */
#include <stddef.h> /* size_t */

#define ABSOLUTE_VALUE(num) (0 > num) ? -num : num
#define BIT_SIZE 32

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

static unsigned int MirrorBit(unsigned int number)
{
	unsigned int revers_number = 0;
	int index = 0;

	while (BIT_SIZE > index)
	{
		if (number & (1 << index))
		{
			revers_number |= (1 << ((BIT_SIZE - 1)  - index));
		}
		++index;
	}

	return revers_number;
}

int main(int ac, char **av)
{
	unsigned int number = 2147483648;

	if (2 == ac)
	{
		number = (unsigned int)atoi(av[1]);
		printf("-- Soruce - ");
		PrintBit(number);
		printf("-- Mirror - ");
		PrintBit(MirrorBit(number));
	}

	return 0;
}
