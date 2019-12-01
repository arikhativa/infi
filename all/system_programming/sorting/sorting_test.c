#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "sorting.h"
#include "color.h"

#define R1(n) n, n+1, n+2, n+3
#define R2(n) R1(n), R1(n+4), R1(n+8), R1(n+12)
#define R3(n) R2(n), R2(n+16), R2(n+32), R2(n+48)
#define CORRECT R3(0), R3(64), R3(128), R3(192)

#define B1(n) n, n, n, n
#define B2(n) B1(n), B1(n), B1(n), B1(n)
#define B3(n) B2(n), B2(n), B2(n), B2(n)
#define BIT B3(0), B3(0), B3(1), B3(1)

#define A1(n) n, n, n, n
#define A2(n) A1(n), A1(n), A1(n), A1(n)
#define A3(n) A2(n), A2(n), A2(n), A2(n)
#define RADIX A3(1000), A3(1200), A3(1300), A3(15000)

static int count_test = 1;
static int t_index = 1;
static int pass = 0;

static int boly = 1;
static int num = 1;
static char *name;


static void SetTest(int n, char *str)
{
	num = n;
	name = str;
	boly = 0;
	t_index = 1;
}

static void Pass()
{
	++count_test;
	++pass;
	++t_index;
}

static void PrintFail(char *str)
{
	if (!boly)
	{
		printf(CYAN"\n-- (%d) Test %s:\n\n"RESET, num, name);
		++boly;
	}
	printf("-- Test"GRAY" %d"RESET RED" - Fail :( "RESET, t_index);
	printf("%s\n", str);
	++count_test;
	++t_index;
}

static void PrintSummery(void)
{
	--count_test;
	if (pass == count_test)
		printf(GREEN"\n-- Pass :) (%d/%d)\n\n"RESET,pass, count_test);
	else
		printf(YELLOW"\n-- Not good enough.. (%d/%d)\n\n"RESET,pass, count_test);
}

static void MemoryComper(void *mem1, void *mem2, size_t n)
{
	if (0 == memcmp(mem1, mem2, n))
		Pass();
	else
		PrintFail(" -- Diffrence in memory.");
}

void PrintArray(int *arr, size_t size)
{
	while (size)
	{
		printf("%d", *arr);
		--size;
		++arr;
		if (size)
			printf(", ");
	}
	printf("\n");
}

static void TestInsertion()
{
	size_t size = 20;
	int correct[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	int worst[] = {20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	int random[] = {12, 17, 10, 5, 20, 13, 19, 11, 14, 9, 2, 4, 6, 15, 7, 3, 1, 8, 16, 18};
	int small[] = {12, 1};
	int sc[] = {1, 12};
	clock_t begin = 0;
	clock_t end = 0;
	double time = 0;
	int i = 0;

	SetTest(1, "Insertion Sort");

	MemoryComper(InsertionSort(random, size), correct, size);
	MemoryComper(InsertionSort(worst, size), correct, size);
	MemoryComper(InsertionSort(small, 2), sc, 2);

	begin = clock();
	for (i = 0; i < 50000; ++i)
		InsertionSort(worst, size);
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- Insertion run time - %f\n", time);
}

static void TestSelection()
{
	size_t size = 20;
	int correct[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	int worst[] = {20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	int random[] = {12, 17, 10, 5, 20, 13, 19, 11, 14, 9, 2, 4, 6, 15, 7, 3, 1, 8, 16, 18};
	clock_t begin = 0;
	clock_t end = 0;
	double time = 0;
	int i = 0;

	SetTest(2, "Selection Sort");

	MemoryComper(SelectionSort(random, size), correct, size);
	MemoryComper(SelectionSort(worst, size), correct, size);

	begin = clock();
	for (i = 0; i < 500000; ++i)
		SelectionSort(worst, size);
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- Selection run time - %f\n", time);
}

static void TestBubble()
{
	size_t size = 20;
	int correct[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	int worst[] = {20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	int random[] = {12, 17, 10, 5, 20, 13, 19, 11, 14, 9, 2, 4, 6, 15, 7, 3, 1, 8, 16, 18};
	clock_t begin = 0;
	clock_t end = 0;
	double time = 0;
	int i = 0;

	SetTest(3, "Bubble Sort");

/*	PrintArray(BubbleSort(random, 20), 20);*/
	
	MemoryComper(BubbleSort(random, size), correct, size);
	MemoryComper(BubbleSort(worst, size), correct, size);

	begin = clock();
	for (i = 0; i < 500000; ++i)
		BubbleSort(worst, size);
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- Bubble run time - %f\n", time);
}

static void TestOpBubble()
{
	size_t size = 20;
	int correct[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	int worst[] = {20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	int random[] = {12, 17, 10, 5, 13, 11, 14, 9, 2, 4, 6, 15, 7, 3, 1, 8, 16, 18, 19, 20};
	clock_t begin = 0;
	clock_t end = 0;
	double time = 0;
	int i = 0;

	SetTest(4, "opti Bubble Sort");

/*	PrintArray(OptimizedBubbleSort(random, 20), 20);*/
	
	MemoryComper(OptimizedBubbleSort(random, size), correct, size);
	MemoryComper(OptimizedBubbleSort(worst, size), correct, size);

	begin = clock();
	for (i = 0; i < 500000; ++i)
		OptimizedBubbleSort(random, size);
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- Optimized Bubble run time - %f\n", time);
}


static void Swap(int *a, int *b)
{
        int swap = 0;

        assert(a);
        assert(b);

        swap = *a;
        *a = *b;
        *b = swap;
}

static void MIX(int *arr, int size)
{
	int s = size - 2;
	size = 0;

	while (s > 126)
	{
		Swap(&arr[s], &arr[size]);
		size += 2;
		s -= 2;
	}
}

static int CMP(const void *a, const void *b)
{
	const int *a1 = (const int*)a;
	const int *b1 = (const int*)b;

	return (*a1 - *b1);
}

static void TestCount()
{
	size_t bit_size = 6;
	int bit[] = {0, 1, 1, 1, 1, 0};
	int bit_correct[] = {0, 0, 1, 1, 1, 1};

	size_t size = 20;
	int worst[] = {-2, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	int correct[] = {-2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

	size_t ran_size = 256;
	int random[] = {CORRECT};
	int cor[] = {CORRECT};

	size_t bbb_size = 256;
	int bbb[] = {BIT};
	int ccc[] = {BIT};

	clock_t begin = 0;
	clock_t end = 0;
	double time = 0;
	int i = 0;

	SetTest(5, "Count Sort");


	MIX(random, ran_size);
	MIX(random, ran_size - 100);
	MIX(bbb, bbb_size);
	MIX(bbb, bbb_size - 100);
	CountSort(bit, bit_size, 0, 1);
	CountSort(worst, size, -2, 19);
	CountSort(random, ran_size, 0, 255);
	CountSort(bbb, bbb_size, 0, 1);

	MemoryComper(bit, bit_correct, bit_size);
	MemoryComper(worst, correct, size);
	MemoryComper(random, cor, 256);
	MemoryComper(bbb, ccc, 256);

	begin = clock();
	for (i = 0; i < 50000; ++i)
	{
		MIX(bbb, bbb_size);
		MIX(bbb, bbb_size - 100);
		qsort(bbb, bbb_size, 4, CMP);
	}
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- qsort run time - %f\n", time);

	begin = clock();
	for (i = 0; i < 50000; ++i)
	{
		MIX(bbb, bbb_size);
		MIX(bbb, bbb_size - 100);
		CountSort(bbb, bbb_size, 0, 1);
	}
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- Count run time - %f\n", time);
}

static void TestRadix()
{
	size_t bit_size = 6;
	unsigned int bit[] = {0, 0, 0, 0, 1, 0};
	unsigned int bit_correct[] = {0, 0, 0, 0, 0, 1};

	unsigned int t1[] = {9, 1, 22, 1, 33333, 1234};
	unsigned int t1_correct[] = {1, 1, 9, 22, 1234, 33333};

	size_t bbb_size = 256;
	unsigned int bbb[] = {RADIX};
	unsigned int ccc[] = {RADIX};

	clock_t begin = 0;
	clock_t end = 0;
	double time = 0;
	int i = 0;

	SetTest(6, "Radix Sort");

	RadixSort(bit, bit_size);
	RadixSort(t1, bit_size);
	RadixSort(bbb, bbb_size);


	MemoryComper(bit, bit_correct, bit_size);
	MemoryComper(t1, t1_correct, bit_size);
	MemoryComper(bbb, ccc, bbb_size);

	begin = clock();
	for (i = 0; i < 50000; ++i)
	{
		MIX((int*)bbb, bbb_size);
		MIX((int*)bbb, bbb_size - 100);
		RadixSort(bbb, bbb_size);
	}
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;



	printf("-- radix run time - %f\n", time);

	begin = clock();
	for (i = 0; i < 50000; ++i)
	{
		MIX((int*)bbb, bbb_size);
		MIX((int*)bbb, bbb_size - 100);
		CountSort((int*)bbb, bbb_size, 1000, 20000);
	}
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- count run time - %f\n", time);

}

static int IsBefore(int a, int b)
{
	return (b < a);
}

static void TestMarge()
{
	size_t ran_size = 256;
	int random[] = {CORRECT};
	int cor[] = {CORRECT};

	clock_t begin = 0;
	clock_t end = 0;
	double time = 0;
	int i = 0;

	SetTest(7, "Marge Sort");

	MIX(random, ran_size);
	MIX(random, ran_size - 100);
	MargeSort(random, ran_size, IsBefore);

	MemoryComper(random, cor, ran_size);

	begin = clock();
	for (i = 0; i < 50000; ++i)
	{
		MIX(random, ran_size);
		MIX(random, ran_size - 100);
		qsort(random, ran_size, 4, CMP);
	}
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- qsort run time - %f\n", time);

	begin = clock();
	for (i = 0; i < 50000; ++i)
	{
		MIX(random, ran_size);
		MIX(random, ran_size - 100);
		MargeSort(random, ran_size, IsBefore);
	}
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- merge run time - %f\n", time);
}


int Cmp(const void *data1, const void *data2)
{
	const int *a = data1;
	const int *b = data2;

	if (*a < *b)
	{
		return -1;
	}
	if (*a > *b)
	{
		return 1;
	}

	return 0;
}


static void TestQuick()
{
	size_t size = 256;
	int arr[] = {CORRECT};
	int cor[] = {CORRECT};

/*	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	size_t size = 10;*/

	clock_t begin = 0;
	clock_t end = 0;
	double time = 0;
	int i = 0;

	SetTest(8, "Quick Sort");

/*	printf("-- origin:\n");
	PrintArray(arr, size);
	printf("\n");*/

	MIX(arr, size);
	MIX(arr, size - 100);
	QuickSort(arr, size, sizeof(int), Cmp);

	MemoryComper(arr, cor, size);

	begin = clock();
	for (i = 0; i < 50000; ++i)
	{
		MIX(arr, size);
		MIX(arr, size - 100);
		qsort(arr, size, 4, CMP);
	}
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- qsort run time - %f\n", time);

	begin = clock();
	for (i = 0; i < 50000; ++i)
	{
		MIX(arr, size);
		MIX(arr, size - 100);
		QuickSort(arr, size, sizeof(int), Cmp);
	}
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- my run time - %f\n", time);
}

int main(int ac, char **av)
{
	int c = 0;

	if (2 <= ac)
	{
		c = atoi(av[1]);
	}
	switch (c)
	{
		case 1:
			TestInsertion();
			break ;
		case 2:
			TestSelection();
			break ;
		case 3:
			TestBubble();
			break ;
		case 4:
			TestOpBubble();
			break ;
		case 5:
			TestCount();
			break ;
		case 6:
			TestRadix();
			break ;
		case 7:
			TestMarge();
			break ;
		case 8:
			TestQuick();
			break ;

		default:
			TestInsertion();
			TestSelection();
			TestBubble();
			TestOpBubble();
			TestCount();
			TestRadix();
			TestMarge();
			TestQuick();
	}
	PrintSummery();

	return 0;
}
