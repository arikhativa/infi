
#include <stdio.h>

void PrintArray(int *arr, int size);
int step_g = 0;

void Swap(int *a, int *b)
{
	int swap = *a;

	*a = *b;
	*b = swap;
}

int Partition(int *arr, int low, int high)
{
	int a = low;
	int b = 0;

	for (b = low; b < high; ++b)
	{
		if (arr[b] < arr[high])
		{
			Swap(&arr[a], &arr[b]);
			++a;
		}
	}

	Swap(&arr[a], &arr[high]);

	return a;
}

void QuickSort(int *arr, int low, int high)
{
	int pi = 0;

	if (low < high)
	{
		pi = Partition(arr, low, high);

		QuickSort(arr, low, pi - 1);
		QuickSort(arr, pi + 1, high);
	}
}


void PrintArray(int *arr, int size)
{
	int i = 0;

	printf("\n-- Step %d\n ", step_g++);
	for (i = 0; i < size; ++i)
	{
		printf("%d", arr[i]);
		if (i + 1 < size)
			printf(", ");
	}
	printf("\n\n");
}

int main()
{
	int size = 2;
//	int arr[] = {1, 8, 3, 5, 4, 6, 7, 9, 2};
	int arr[] = {8, 4};

	PrintArray(arr, size);
	QuickSort(arr, 0, size -1);
	PrintArray(arr, size);

	return 0;
}
