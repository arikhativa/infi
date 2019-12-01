
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct subarray
{
	int low;
	int high;
	int sum;
} sub_t;

void FindBestSubArray(int *arr, int size, sub_t *ret)
{
	int low = 0;
	int high = 0;
	int sum = 0;

	while (arr[low] < 0)
		++low;

	ret->low = low;
	ret->high = low;
	ret->sum = arr[low];

	while (low < size - 1)
	{
		high = low + 1;

		while (high < size)
		{
			sum += arr[high];

			if (sum > ret->sum)
			{
				ret->low = low;
				ret->high = high;
				ret->sum = sum;
			}
			++high;
		}

		++low;
		sum = arr[low];
	}
}

int main()
{
	int size = 10;
	int i;
	int arr[] = {5, 7, -4, 30, -1, 70, -336, 44, 2, 0};
	sub_t ret;

	FindBestSubArray(arr, size, &ret);

	printf("-- ret:\n");
	printf("-- low %d\n", ret.low);
	printf("-- h %d\n", ret.high);
	printf("-- sum %d\n", ret.sum);

	return 0;
}

