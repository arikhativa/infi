
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */
#include <stddef.h> /* size_t */

#define SIZE_OF_ARRAY 5
#define ABSOLUTE_VALUE(num) (0 > num) ? -num : num;

static void PrintBit(int bit)
{
	int n = 7;
	int str[8];

	while (0 <= n)
	{
		str[n] = (bit % 2);
		str[n] = ABSOLUTE_VALUE(str[n]);
		bit = bit >> 1;
		--n;
	}
	n = 0;
	while (8 > n)
	{
		printf("%d", str[n]);
		++n;
	}
	printf("\n");
}

static void PrintNumWithThreeOnes(unsigned int *array, size_t size)
{
	int count = 0;
	unsigned int number = array[0];
	size_t index = 0;

	while (index < size)
	{
		while (4 > count && 0 < number)
		{
			if (1 == (number & 1))
			{
				++count;
			}
			number >>= 1;
		}
		if (3 == count)
		{
			printf("%d\n", array[index]);
		}
		++index;
		count = 0;
		number = array[index];
	}
}   

int main()
{
	unsigned int n[] = {13, 6, 8, 0, 0};
	//unsigned int n[] = {9, 7, 89, 88, 131};
	size_t index = 0;

	printf("\n-- Numbers in array[5]:\n");
	while (index < 5)
	{
		printf("-- array[%ld] is %d\t- ", index, n[index]);
		PrintBit(n[index]);
		++index;
	}
	printf("\n-- But only these ones have 3 '1' in them:\n");
	PrintNumWithThreeOnes(n, SIZE_OF_ARRAY);

	return 0;
}
