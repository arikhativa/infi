
#include <sstream>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>  // istream, ostream

// #include "semaphore.hpp"
// #include "scopelock.hpp"
#include "tsfsq.hpp"
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


static void ComperInt(ssize_t num1, ssize_t num2, const char *dis, int line)
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


// static void MemoryComper(void *mem1, void *mem2, size_t n, convert_t conv ,char *dis, int line)
// {
// 	char value[200] = {0};
// 	char bad_value[200] = {0};
//
// 	if (0 == memcmp(mem1, mem2, n))
// 		Pass();
// 	else
// 	{
// 		conv(value, mem1);
// 		conv(bad_value, mem2);
// 		PrintFail(dis, line, value, bad_value);
// 	}
// }



void *ThreadFunc(void *data)
{
	Tsfsq<3, int>* buf = (Tsfsq<3, int>*)data;

	int tmp = buf->Read();
	std::cout << "-- read " << tmp << "\n";
	buf->Write(4);
	std::cout << "-- write 4" << "\n";
	tmp = buf->Read();
	std::cout << "-- read " << tmp << "\n";
	tmp = buf->Read();
	std::cout << "-- read " << tmp << "\n";
	tmp = buf->Read();
	std::cout << "-- read " << tmp << "\n";
	tmp = buf->Read();
	std::cout << "-- read " << tmp << "\n";
	tmp = buf->Read();
	std::cout << "-- read " << tmp << "\n";
	tmp = buf->Read();
	std::cout << "-- read " << tmp << "\n";
	// tmp = buf->Read();
	// std::cout << "-- read " << tmp << "\n";


	return 0;
}


void TestBasicCycel()
{
	SetTest(1, "Basic Cycel and compare");

	pthread_t th;

	int tmp = 0;
	void* ret;

	Tsfsq<3, int> buf;

	pthread_create(&th, 0, ThreadFunc, &buf);

	buf.Write(1);
	buf.Write(2);
	buf.Write(3);
	std::cout << "-- main after 123\n";
	buf.Write(1);
	buf.Write(2);
	buf.Write(3);
	std::cout << "-- main after 123\n";
	// tmp = buf.Read();
	// std::cout << "-- main read " << tmp << "\n";
	// buf.Write(5);
	// buf.Write(6);
	// std::cout << "-- main after 456\n";

	ComperInt(0, tmp, "Reading empty buffer", __LINE__);

	pthread_join(th, &ret);
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
