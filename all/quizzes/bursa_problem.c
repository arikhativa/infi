#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>


//char *strdup(const char *str);

/*
 *	Find the biggest profit that you could make.
 *	Return it with the right indexes.
 *
 *	int stock[] will have the stock prices in it.
 *	the first index will represent hour '0'.
 *	int size would be the last hour.
 */

enum return_values {
	LOW_INDEX,
	HIGH_INDEX,
	PROFIT
};

int *BursaProblem(int *stock, size_t size)
{
	int *ret;
	int low_index = 0;
	int index = 1;
	int low_price = stock[0];
	int high_price = 0;

	ret = (int*)malloc(sizeof(int) * 3);
	ret[LOW_INDEX] = 0;
	ret[HIGH_INDEX] = 0;
	ret[PROFIT] = 0;

	while (index < size)
	{
		if (stock[index] > low_price)
		{
			high_price = stock[index];
			if ((high_price - low_price > ret[PROFIT]))
			{
				ret[LOW_INDEX] = low_index;
				ret[HIGH_INDEX] = index;
				ret[PROFIT] = high_price - low_price;
			}
		}
		else
		{
			low_index = index;
			low_price = stock[index];
		}
		++index;
	}

	return ret;
}

int main(int ac, char **av, char **env)
{
	int day1[] = {6, 12, 3, 5, 1, 4, 9, 2};
	int *ret;

	ret = BursaProblem(day1, 8);
	printf("-- low_index is %d\n", ret[LOW_INDEX]);
	printf("-- high_index is %d\n", ret[HIGH_INDEX]);
	printf("-- profit is %d\n", ret[PROFIT]);

	return 0;
}

