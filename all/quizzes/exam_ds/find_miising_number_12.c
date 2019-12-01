
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KB 1024

void Fill(int *arr)
{
	int i = 0;

	while (i < (KB -1))
	{
		arr[i] = i;
		++i;
	}
}

int FindMissingNumber(int *arr)
{
	int full_sum;
	int arr_sum = 0;
	int i = 0;

	for (; i< KB; ++i)
		full_sum += i;

	for(i = 0; i < (KB - 1); ++i)
		arr_sum += arr[i];

	return full_sum - arr_sum;
}

int main()
{
	int arr[KB - 1] = {0};
	int num;
	int i = 0;

	Fill(arr);
	arr[1022] = 1023;
	num = FindMissingNumber(arr);

	printf("-- %d\n", num);

	return 0;
}

