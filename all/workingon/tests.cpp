
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "slist.hpp"
#include "color.h"

using namespace hrd11;

typedef void (*convert_t)(char *dest, unsigned char src[4]);

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
	sprintf(g_summery[g_fail],BBLUE"Value should be\t"RESET BGREEN"%s\n"RESET, value);
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
static void MemoryComper(void *mem1, void *mem2, size_t n, convert_t conv ,char *dis, int line)
{
	char value[200] = {0};
	char bad_value[200] = {0};

	if (0 == memcmp(mem1, mem2, n))
		Pass();
	else
	{
		conv(value, mem1);
		conv(bad_value, mem2);
		PrintFail(dis, line, value, bad_value);
	}
}
*/



void TestCreateAndDestroy()
{
    SList list;
}

void TestPush()
{
    SList list;
    int ret = 0;
    char dis[] = "-- Pushing '1' and compering using the ret value of Head().";


    list.PushFront(1);
    ret = list.Head();

    ReturnComper(1, ret, dis, __LINE__);

}

void TestPop()
{
    SList list;
    bool ret = 0;
    char dis[] = "-- Pushing and Poping cheaking with IsEmpty()";


    list.PushFront(1);
    list.PopFront();
    ret = list.IsEmpty();

    ReturnComper(1, ret, dis, __LINE__);
}

void TestSize()
{
    SList list;
    size_t ret = 0;
    char dis[] = "-- Pushing 6 times and poping once. comparing ret of Size()";

    for (int i = 0; i < 5; ++i)
    {
        list.PushFront(i);
    }
    list.PopFront();
    list.PushFront(66);

    ret = list.Size();

    ReturnComper(5, ret, dis, __LINE__);
}

int main(int ac, char **av)
{
    int test = 0;

    if (2 == ac)
    {
        test = atoi(av[1]);
    }
    switch (test)
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
            TestSize();
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
        TestSize();



	}
	PrintSummery();

}
