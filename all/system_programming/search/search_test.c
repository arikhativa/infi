#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "color.h"
#include "search.h"

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

static void ReturnComper(ssize_t num1, ssize_t num2, char *dis, int line)
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

void PrintArray(int *arr, size_t size)
{
	size_t i = 0;

	printf("arr - ");
	for (; i < size; ++i)
	{
		printf("%d", arr[i]);
		if (i + 1 < size)
		{
			printf(", ");

		}
	}

	printf("\n\n");
}

int Cmp(const void *data1, const void *data2)
{
	const int *d1 = data1;
	const int *d2 = data2;

/*	printf("****\n-- d1 - [%d]	", *d1);
	printf("d2 - [%d]\n", *d2);*/

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

static void TestIterBS()
{
	char dis1[] = "array of one elem. cheak return value";
	size_t size1 = 1;
	int arr1[] = {6};
	int data1 = 6;

	char dis2[] = "array of three elems. cheak return value";
	size_t size2 = 3;
	int arr2[] = {1, 2, 3};
	int data2 = 2;

	char dis3[] = "array of three elems. cheak return value";
	size_t size3 = 10;
	int arr3[] = {1, 2, 3, 6, 77, 88, 123, 222, 5454, 99999};
	int data3 = 123;

	int stt = 0;

	SetTest(1, "Iter Binary Search");

	stt = IterBinarySearch(arr1, size1, sizeof(int), Cmp, &data1);
	ReturnComper(0, stt, dis1, __LINE__);

	stt = IterBinarySearch(arr2, size2, sizeof(int), Cmp, &data2);
	ReturnComper(1, stt, dis2 , __LINE__);

	stt = IterBinarySearch(arr2, size2, sizeof(int), Cmp, &data1);
	ReturnComper(-1, stt, "searching for data that is not threr" , __LINE__);

	stt = IterBinarySearch(arr3, size3, sizeof(int), Cmp, &data3);
	ReturnComper(6, stt, dis3 , __LINE__);

}

static void TestRecBS()
{
	char dis1[] = "array of one elem. cheak return value";
	size_t size1 = 1;
	int arr1[] = {6};
	int data1 = 6;

	char dis2[] = "array of three elems. cheak return value";
	size_t size2 = 3;
	int arr2[] = {1, 2, 3};
	int data2 = 2;

	char dis3[] = "array of 10 elems. cheak return value";
	size_t size3 = 10;
	int arr3[] = {1, 2, 3, 6, 77, 88, 123, 222, 5454, 99999};
	int data3 = 123;

	char dis4[] = "array of 10 elems. cheak return value";
	int data4 = 2;

	char dis5[] = "array of 8 elems. cheak return value";
	size_t size5 = 8;
	int arr5[] = {-1, 2, 3, 6, 7, 8, 11, 12};
	int data5 = 7;
	int stt = 0;

	SetTest(2, "Rec Binary Search");

	stt = RecBinarySearch(arr1, size1, sizeof(int), Cmp, &data1);
	ReturnComper(0, stt, dis1, __LINE__);

	stt = RecBinarySearch(arr2, size2, sizeof(int), Cmp, &data2);
	ReturnComper(1, stt, dis2 , __LINE__);

	stt = RecBinarySearch(arr2, size2, sizeof(int), Cmp, &data1);
	ReturnComper(-1, stt, "searching for data that is not threr" , __LINE__);

	stt = RecBinarySearch(arr3, size3, sizeof(int), Cmp, &data3);
	ReturnComper(6, stt, dis3 , __LINE__);

	stt = RecBinarySearch(arr3, size3, sizeof(int), Cmp, &data4);
	ReturnComper(1, stt, dis4 , __LINE__);

/*PrintArray(arr5, size5);*/
	stt = RecBinarySearch(arr5, size5, sizeof(int), Cmp, &data5);
	ReturnComper(4, stt, dis5 , __LINE__);
}

static void TestJS()
{
	char dis1[] = "array of one elem. cheak return value";
	size_t size1 = 1;
	int arr1[] = {6};
	int data1 = 6;

	char dis2[] = "array of three elems. cheak return value";
	size_t size2 = 3;
	int arr2[] = {1, 2, 3};
	int data2 = 2;

	char dis3[] = "array of 10 elems. cheak return value";
	size_t size3 = 10;
	int arr3[] = {1, 2, 3, 6, 77, 88, 123, 222, 5454, 99999};
	int data3 = 123;

	char dis4[] = "array of 10 elems. cheak return value";
	int data4 = 99999;

	int stt = 0;

	SetTest(3, "JumpSearch Search");

	stt = JumpSearch(arr1, size1, sizeof(int), Cmp, &data1);
	ReturnComper(0, stt, dis1, __LINE__);

	stt = JumpSearch(arr2, size2, sizeof(int), Cmp, &data2);
	ReturnComper(1, stt, dis2 , __LINE__);

	stt = JumpSearch(arr2, size2, sizeof(int), Cmp, &data1);
	ReturnComper(-1, stt, "searching for data that is not threr" , __LINE__);

	stt = JumpSearch(arr3, size3, sizeof(int), Cmp, &data3);
	ReturnComper(6, stt, dis3 , __LINE__);

	stt = JumpSearch(arr3, size3, sizeof(int), Cmp, &data4);
	ReturnComper(9, stt, dis4 , __LINE__);

	/*PrintArray(arr3, size3);*/

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
			TestIterBS();
			break ;
		case 2:
			TestRecBS();
			break ;
		case 3:
			TestJS();
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
			TestIterBS();
			TestRecBS();
			TestJS();
	}
	PrintSummery();

	return 0;
}
