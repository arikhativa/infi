
#include <sstream>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>  // istream, ostream

#include <mutex>
#include <pthread.h>

#include "scopelock.hpp"
#include "color.h"

using namespace hrd11;
using namespace std;

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

static void SetTest(int n, const char *name)
{
	sprintf(test_name,"\n\n-- " CYAN "(%d) Test %s:\n\n" RESET, n, name);

	add_test_name = 1;

	t_index = 1;
}

static void Pass()
{
	++count_test;
	++g_pass;
	++t_index;
}

static void PrintFail(const char *dis, int line, char *value, char *bad_value)
{
	if (add_test_name)
	{
		add_test_name = 0;
		sprintf(g_summery[g_fail],"%s", test_name);
		++g_fail;
	}

	sprintf(g_summery[g_fail],"-- " YELLOW "Discription:\n" RESET "-- %s\n\n",dis);
	++g_fail;
	sprintf(g_summery[g_fail],"-- " BBLUE "In Line " BGREEN "%d " RESET , line);
	++g_fail;
	sprintf(g_summery[g_fail], BBLUE "Value should be\t" RESET BGREEN "%s\n" RESET, value);
	++g_fail;
	sprintf(g_summery[g_fail],"-- " BBLUE "It is\t\t\t" RESET RED "%s\n\n" RESET, bad_value);
	++g_fail;

	++count_test;
	++t_index;
}

static void PrintSummery(void)
{
	int i = 0;

	--count_test;
	if (g_pass == count_test)
		printf(GREEN "\n-- Pass :) (%d/%d)\n\n" RESET,g_pass, count_test);
	else
	{
		printf(YELLOW "\n-- Not good enough.. (%d/%d)\n\n" RESET,g_pass, count_test);
		for (i = 0; i < g_fail; ++i)
		{
			printf("%s", g_summery[i]);
		}
	}
}

template <typename T>
static void ReturnComper(T obj1, T obj2, const char *dis, int line)
{
	 char value[20] = {0};
	 char bad_value[20] = {0};

	if (obj1 == obj2)
		Pass();
	else
	{
		value << obj1;
		bad_value << obj2;
		PrintFail(dis, line, value, bad_value);
	}
}


// static void ComperInt(ssize_t num1, ssize_t num2, const char *dis, int line)
// {
// 	char value[20] = {0};
// 	char bad_value[20] = {0};
//
// 	if (num1 == num2)
// 		Pass();
// 	else
// 	{
// 		sprintf(value, "%ld", num1);
// 		sprintf(bad_value, "%ld", num2);
// 		PrintFail(dis, line, value, bad_value);
// 	}
// }

void TestBasicCycel()
{
	SetTest(1, "Basic Cycel -- use vlg");

	// std::recursive_mutex mutex;
	// Scopelock<std::recursive_mutex> lock(mutex);
	// std::mutex mutex;
	// Scopelock<std::mutex> lock(mutex);

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	Scopelock<pthread_mutex_t> lock(mutex);

	// lock.Unlock();
	// lock.Lock();

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
		{
			TestBasicCycel();
			break ;
		}

		case 2:
		{
			break ;
		}

		case 3:
		{
			break ;
		}
		case 4:
		{
			break ;
		}

		case 5:
        {
            break ;
        }

		default:
        {
    		TestBasicCycel();

        }
	}
	PrintSummery();

	return 0;
}
