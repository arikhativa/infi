#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "color.h"
#include "heap.h"
#include "vector.h"

/*
 * 	global 
 */

static char g_summery[400][400] = {0};

static int count_test = 1;
static int t_index = 1;
static int g_pass = 0;
static int g_fail = 0;

/*	var per test	*/

static char test_name[40];

static int add_test_name = 1;


struct dynamic_vector
{
        void *start;
        size_t current_index;
        size_t capacity;
        size_t element_size;
};

struct heap
{
        vector_t *vec;
        heap_cmp_func_t fp_cmp;
        void *param;
};


static void SetTest(int n, char *name)
{
	sprintf(test_name,"\n\n-- "CYAN"(%d) Test %s:\n\n"RESET, n, name);

	add_test_name = 1;

	t_index = 1;
}

static void Pass()
{
	++count_test;
	++g_pass;
	++t_index;
}

static void PrintFail(char *dis, int line, char *value, char *bad_value)
{
	if (add_test_name)
	{
		add_test_name = 0;
		sprintf(g_summery[g_fail],"%s", test_name);
		++g_fail;
	}

	sprintf(g_summery[g_fail],"-- "YELLOW"Discription:\n"RESET"-- %s\n\n",dis);
	++g_fail;
	sprintf(g_summery[g_fail],"-- "BBLUE"In Line "BGREEN "%d "RESET, line);
	++g_fail;
	sprintf(g_summery[g_fail],BBLUE"Value should be "RESET BGREEN"%s\n"RESET, value);
	++g_fail;
	sprintf(g_summery[g_fail],"-- "BBLUE"It is "RESET RED"%s\n\n"RESET, bad_value);
	++g_fail;


	++count_test;
	++t_index;
}

static void PrintSummery(void)
{
	int i = 0;

	--count_test;
	if (g_pass == count_test)
		printf(GREEN"\n-- Pass :) (%d/%d)\n\n"RESET,g_pass, count_test);
	else
	{
		printf(YELLOW"\n-- Not good enough.. (%d/%d)\n\n"RESET,g_pass, count_test);
		for (i = 0; i < g_fail; ++i)
		{
			printf("%s", g_summery[i]);
		}
	}
}

static void ReturnComper(size_t num1, size_t num2, char *dis, int line)
{
	char value[20] = {0};
	char bad_value[20] = {0};

	if (num1 == num2)
		Pass();
	else
	{
		sprintf(value, "%lu", num1);
		sprintf(bad_value, "%lu", num2);
		PrintFail(dis, line, value, bad_value);
	}
}
/*
static void MemoryComper(void *mem1, void *mem2, size_t n, char *dis, int line)
{
	char value[200] = {0};
	char bad_value[200] = {0};

	if (0 == memcmp(mem1, mem2, n))
		Pass();
	else
	{
		PrintFail(dis, line, value, bad_value);

		sprintf(value, "%s", (char*)mem1);
		sprintf(bad_value, "%s", (char*)mem2);
		PrintFail(dis, line, value, bad_value);
	}
}

*/

int IfOpen(int num)
{
	int arr[] = {0, 1, 3, 7};
	int i = 0;
	int size = 4;


	while (i < size)
	{
		if (arr[i] == num)
			return 1;
		++i;
	}

	return 0;
}

int IfClose(int num)
{
	int arr[] = {0, 2, 6, 14};
	int i = 0;
	int size = 4;


	while (i < size)
	{
		if (arr[i] == num)
			return 1;
		++i;
	}

	return 0;
}

void PrintHeap(heap_t *heap)
{
	size_t **arr;
	size_t size;
	size_t i;
	int t = 4;
	int j = 0;

        arr = heap->vec->start;
	size = HeapSize(heap);

	printf("\n-- Heap\n");
	for (i = 0; i < size; ++i)
	{
		if (IfOpen(i))
		{
			for (j = 0; j < t ; ++j)
			{
				printf("   ");
			}
				--t;

			printf("[");
		}
		printf("%d", **(int**)(arr + i));
		if (IfClose(i))
		{
			printf("]\n");
		}
		else if (i +1 < size)
			printf(" - ");


	}
	printf("\n\n");
}

int Cmp(const void *data, const void *user_data, void *param)
{
	const int *d1 = data;
	const int *d2 = user_data;

	(void)param;

	if (*d2 > *d1)
	{
		return -1;
	}
	if (*d2 < *d1)
	{
		return 1;
	}

	return 0;
}

static void TestCreateAndDestroy()
{
	heap_t *heap;
	size_t size = 50;

	SetTest(1, "Create & Destroy (run with vlg)");

	heap = HeapCreate(Cmp, size, 0);

	HeapDestroy(heap);
}

static void TestPush()
{
	heap_t *heap;
	size_t i = 0;
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
	size_t size = 8;

	SetTest(2, "Push");

	heap = HeapCreate(Cmp, size * 10, 0);

	ReturnComper(1, HeapIsEmpty(heap), "checking is empty, with empty heap.", __LINE__);

	for (i = 0; i < size; ++i)
		HeapPush(heap, &arr[i]);

	ReturnComper(0, HeapIsEmpty(heap), "checking is empty, with full heap.", __LINE__);
	ReturnComper(8, HeapSize(heap), "cheking HeapSize. after pushed 8 time", __LINE__);

/*	PrintHeap(heap);*/

	HeapDestroy(heap);

}

static void TestPop()
{
	char dis1[] = "cheking return value of HeapPop.\n-- also, making sure the data in the vector\n-- is not a copy but the original.";
	heap_t *heap;
	size_t i = 0;
	size_t size = 10;
	int arr[] = {5, 6, 6, 6, 8, 9, 7 ,10, 3, 2};
	void *tmp;

	SetTest(3, "Pop");

	heap = HeapCreate(Cmp, size * 10, 0);


	for (i = 0; i < size; ++i)
		HeapPush(heap, &arr[i]);

	arr[5] = 22;
	for (i = 0; i < 2; ++i)
	{
		tmp = HeapPop(heap);
	}

	ReturnComper(8, HeapSize(heap), "cheking HeapPop. using HeapSize", __LINE__);
	ReturnComper(22, *(int*)tmp, dis1, __LINE__);

	for (i = 0; i < size; ++i)
	{
		tmp = HeapPop(heap);
	}

/*	PrintHeap(heap);*/

	HeapDestroy(heap);
}




static int RmFunc(const void *data, void *param)
{
	if (*(int*)data == *(int*)param)
	{	
		return 0;
	}

	return 1;
}

static void TestRemove()
{
	char dis1[] = "checking if removed the correct node.\n-- using the return value from HeapPop. can fail if HeapRemove dose not do heapifyUp and Down correctly";
	heap_t *heap;
	size_t i = 0;
	size_t size = 9;
	size_t size2 = 10;
	int arr[] = {0, 5, 4, 3, 2, 1, 6, 9, 7};
	int cor[] = {9, 6, 5, 4, 3, 2, 1, 0};

	int arr2[] = {100, 80, 80, 70, 75, 25, 50, 20, 40, 72};
	int cor2[] = {100, 80, 80, 75, 70, 50, 40, 25};

	void *tmp;

	SetTest(4, "Remove");

	heap = HeapCreate(Cmp, size * 10, 0);

	for (i = 0; i < size; ++i)
		HeapPush(heap, &arr[i]);

	HeapRemove(heap, RmFunc, &arr[8]);

	ReturnComper(8, HeapSize(heap), "cheking HeapRemove. using HeapSize", __LINE__);

	for (i = 0; i < size -1; ++i)
	{
		tmp = HeapPop(heap);
		ReturnComper(cor[i], *(int*)tmp, dis1, __LINE__);
	}
	HeapDestroy(heap);

	heap = HeapCreate(Cmp, size * 10, 0);

	for (i = 0; i < size2; ++i)
		HeapPush(heap, &arr2[i]);

	HeapRemove(heap, RmFunc, &arr2[9]);
	HeapRemove(heap, RmFunc, &arr2[7]);
	for (i = 0; i < size2 -2; ++i)
	{
		tmp = HeapPop(heap);
		ReturnComper(cor2[i], *(int*)tmp, dis1, __LINE__);
	}

/*	PrintHeap(heap);*/

	HeapDestroy(heap);


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
			TestCreateAndDestroy();
			break ;
		case 2:
			TestPush();
			break ;
		case 3:
			TestPop();
			break ;
		case 4:
			TestRemove();
			break ;
		case 5:
			break ;
		case 6:
			break ;
		case 7:
			break ;
		case 8:
			break ;
		case 9:
			break ;
		default:
			TestCreateAndDestroy();
			TestPush();
			TestPop();
			TestRemove();
	}
	PrintSummery();

	return 0;
}
