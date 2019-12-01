#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>
#include <time.h>


#include "color.h"
#include "mt_count_sort.h"

#define B1(n) n, n, n, n
#define B2(n) B1(n), B1(n), B1(n), B1(n)
#define B3(n) B2(n), B2(n), B2(n), B2(n)
#define BIT B3(0), B3(0), B3(0), B3(0)

#define B1(n) n, n, n, n
#define B2(n) B1(n), B1(n), B1(n), B1(n)
#define B3(n) B2(n), B2(n), B2(n), B2(n)
#define BIT1 B3(1), B3(1), B3(1), B3(1)

#define R1(n) n, n+1, n+2, n+3
#define R2(n) R1(n), R1(n+4), R1(n+8), R1(n+12)
#define R3(n) R2(n), R2(n+16), R2(n+32), R2(n+48)
#define NUM_IN_CHAR R3(0), R3(64), R3(128), R3(192)

typedef void (*convert_t)(char *dest, int *arr, size_t size);

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
	sprintf(g_summery[g_fail],BBLUE"Value should be\t"RESET GREEN"%s\n"RESET, value);
	++g_fail;
	sprintf(g_summery[g_fail],"-- "BBLUE"It is\t\t\t"RESET RED"%s\n\n"RESET, bad_value);
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

/*static void ReturnComper(ssize_t num1, ssize_t num2, char *dis, int line)
{
	char value[20] = {0};
	char bad_value[20] = {0};

	if (num1 == num2)
		Pass();
	else
	{
		sprintf(value, "%ld", num1);
		sprintf(bad_value, "%ld", num2);
		PrintFail(dis, line, value, bad_value);
	}
}*/

static void MemoryComper(void *mem1, void *mem2, size_t n, convert_t conv ,char *dis, int line)
{
	char value[200] = {0};
	char bad_value[200] = {0};

	if (0 == memcmp(mem1, mem2, n))
		Pass();
	else
	{
		conv(value, mem1, n);
		conv(bad_value, mem2, n);
		PrintFail(dis, line, value, bad_value);
	}
}

void Swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void SwapPtr(void **a, void **b)
{
	void *tmp = *a;
	*a = *b;
	*b = tmp;
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

int CMP(const void *data1, const void *data2)
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

void Convert(char *dest, int *arr, size_t size)
{
	size_t i = 0;

	if (size > 100)
	{
		sprintf(dest, "%s", " 0-255");
		return ;
	}
	size = size / 4;

	for (i = 0; i < size; ++i)
	{
		sprintf(dest, "%d", arr[i]);
		do
		{
			arr[i] /= 10;
			++dest;
		} while (arr[i]);
		if (i + 1 < size)
		{
			sprintf(dest, "%c%c", ',',' ');
			++dest;
		}
		++dest;
	}
	*dest = '\0';
}

size_t GetKey(void *data, void *param)
{
	(void)param;

	return *(int*)data;
}

void MiniCount(int *arr, int *cor, size_t size, size_t range, char *dis)
{
	clock_t begin = 0;
	clock_t end = 0;
	double time = 0;
	int i = 0;

	MTCountSort(arr, size, sizeof(int), range, GetKey, 0);
	MemoryComper(cor, arr, size * sizeof(int), Convert, dis, __LINE__);

	begin = clock();
	for (i = 0; i < 1; ++i)
	{
		MIX(arr, size);
		MIX(arr, size - 100);
		qsort(arr, size, 4, CMP);
	}
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- qsort run time -\t%f\n", time);

	begin = clock();
	for (i = 0; i < 1; ++i)
	{
		MIX(arr, size);
		MIX(arr, size - 100);
		MTCountSort(arr, size, sizeof(int), range, GetKey, 0);
	}
	end = clock();
	time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("-- my run time -\t%f\n\n", time);

}

void TestMTCount(void)
{
	int arr1[] = {1, 0, 1, 1, 0, 0, 1, 0, 1, 0};
	size_t size1 = 10;
	size_t range1 = 2;
	int cor1[] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
	char dis1[] = "-- sorting binary array. size 6";

	int arr2[] = {5, 9, 4, 8, 7, 6, 2, 1, 3, 0};
	size_t size2 = 10;
	size_t range2 = 10;
	int cor2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	char dis2[] = "-- sorting 0-9 array.";

	int arr3[] = {NUM_IN_CHAR};
	size_t size3 = 256;
	size_t range3 = 256;
	int cor3[] = {NUM_IN_CHAR};
	char dis3[] = "-- sorting 0-255 array.";

	int arr4[] = {
		BIT, BIT, BIT, BIT1, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT, BIT, BIT, BIT1, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT, BIT, BIT, BIT1, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT, BIT, BIT, BIT1, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT, BIT, BIT, BIT};
	size_t size4 = 256 * 64;
	size_t range4 = 2;
	int cor4[] = {
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT,
		BIT1, BIT1, BIT1, BIT1};
	char dis4[] = "-- sorting 0-255 array.";

	SetTest(4, "Generic Count");

	MiniCount(arr1, cor1, size1, range1, dis1);
	MiniCount(arr2, cor2, size2, range2, dis2);
	MIX(arr3, size3);
	MIX(arr3, size3 - 100);
	MiniCount(arr3, cor3, size3, range3, dis3);
	MIX(arr4, size4);
	MIX(arr4, size4 - 100);
	MiniCount(arr4, cor4, size4, range4, dis4);
}

char **CreateDic(void)
{
	char **dic = 0;
	char *dump = 0;
	FILE *fd = 0;
	size_t i = 0;
	size_t dic_size = 102305;

	fd = fopen("/usr/share/dict/american-english", "r");
	dic = (char**)malloc(sizeof(char*) * dic_size);

	for (i = 0; i < dic_size; ++i)
	{
		dic[i] = (char*)malloc(30);
		dump = fgets(dic[i], 30, fd);
		if (!dump)
			exit(1);
	}



	fclose(fd);

	return dic;
}

void DestroyDic(char **dic)
{
	size_t i = 0;
	size_t dic_size = 102305;

	for (i = 0; i < dic_size; ++i)
	{
		free(dic[i]);
	}
	free(dic);
}

void Shuffle(char **dic)
{
	size_t i = 0;
	size_t dic_size = 50000;

	for (i = 0; i < dic_size; i += 2)
	{
		SwapPtr((void**)&dic[i], (void**)&dic[i + 40000]);
	}
}

size_t GetKeyDic(void *data, void *param)
{
	char **str = data;

	(void)param;

	if ('a' <= **str && 'z' >= **str)
	{
		return **str - 'a';
	}
	if ('A' <= **str && 'Z' >= **str)
	{
		return **str - 'A';
	}

	return 26;
}


void Dictionary()
{
	char **dic = 0;
	size_t i = 0;
	size_t dic_size = 102305;

	dic = CreateDic();

	Shuffle(dic);

	MTCountSort(dic, dic_size, sizeof(char*), 27, GetKeyDic, 0);

	for (i += 0; i < dic_size; ++i)
	{
		/*	printf("%s", dic[i]);*/
	}

	DestroyDic(dic);
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
			TestMTCount();
			break ;
		case 2:
			Dictionary();
			break ;
		case 3:
			break ;
		case 4:
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
		TestMTCount();


	}
	PrintSummery();

	return 0;
}
