
#include <sstream>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>  // istream, ostream

#include "bitarray.hpp"
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

void TestBasicCycel()
{
	SetTest(1, "Basic Cycel and compare");

	BitArray<(8)> b1;

	for (int i = 0; i < 8; ++i)
	{
		b1[i] = true;
	}
	BitArray<(8)> b2(b1);

	b2[2] = false;
	b1[0] = b2[2];

	int tmp = (b1 == b2);
	ComperInt(0, tmp, "b1 == b2", __LINE__);
	b1 = b2;
	tmp = (b1 != b2);
	ComperInt(0, tmp, "b1 != b2", __LINE__);
}

void TestAssign()
{
	SetTest(2, "TestAssign");

	{
		BitArray<(64 * 10)> b1;

		b1[100] = true;
		BitArray<(64 * 10)> b2(b1);
		b2[150] = true;

		b2 ^= b1;
		b1[150] = true;
		b1[100] = false;
		int tmp = (b2 == b1);
		ComperInt(1, tmp, "b1 ^= b2", __LINE__);
	}
	{
		BitArray<(64 * 10)> b1;
		BitArray<(64 * 10)> b2;
		BitArray<(64 * 10)> b3;
		int tmp = 0;

		for (int i = 0; i < 8; ++i)
		{
			b1[i] = true;
			b1[i+8] = true;
			b2[i+8] = true;
			b2[i+16] = true;
			b3[i+8] = true;
		}
		b2 &= b1;
		tmp = (b2 == b3);
		ComperInt(1, tmp, "b1 &= b2", __LINE__);
	}
	{
		BitArray<(64 * 10)> b1;
		BitArray<(64 * 10)> b2;
		BitArray<(64 * 10)> b3;
		int tmp = 0;

		for (int i = 0; i < 8; ++i)
		{
			b1[i] = true;
			b1[i+8] = true;
			b2[i+8] = true;
			b2[i+16] = true;
			b3[i] = true;
			b3[i+8] = true;
			b3[i+16] = true;
		}
		b2 |= b1;
		tmp = (b2 == b3);
		ComperInt(1, tmp, "b1 |= b2", __LINE__);
	}
}

void Test3()
{
	SetTest(3, "Test Count and Shift");

	{
		BitArray<(64 * 10)> b1;

		for(int i =0; i < 10; ++i)
		{
			b1[i + 50] = true;
		}

		int tmp = b1.CountTrueBits();
		ComperInt(10, tmp, "b1.CountTrueBits()", __LINE__);
	}
	{
		BitArray<(64 * 10)> b1;

		for(int i =0; i < 10; ++i)
		{
			b1[i + 50] = true;
		}

		b1[640 - 1] = true;
		b1 <<= 5;
		int tmp = b1.CountTrueBits();
		ComperInt(10, tmp, "b1 <<= 5", __LINE__);
	}
	{
		BitArray<(64 * 10)> b1;
		BitArray<(64 * 10)> b2;
		size_t ttt = 0;

		ttt = ~ttt;

		for(int i = 0; i < 64; ++i)
		{
			b1[i] = true;
			b2[i + 64] = true;
		}

		b1 <<= 64;
		int tmp = (b1 == b2);
		ComperInt(1, tmp, "b1 <<= 64", __LINE__);
	}
}

void TestIO()
{
	SetTest(4, "Test IO");
	{
		BitArray<(64 * 10 - 7)> b1;

		for(int i = 0; i < 64; ++i)
		{
			b1[i] = true;
		}
		b1 <<= 100;
		std::cout << b1;
	}
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
			TestAssign();
			break ;
		}

		case 3:
		{
			Test3();
			break ;
		}
		case 4:
		{
			TestIO();
			break ;
		}

		case 5:
        {
            break ;
        }

		default:
        {
    		TestBasicCycel();
			TestAssign();
			Test3();
        }
	}
	PrintSummery();

	return 0;
}
