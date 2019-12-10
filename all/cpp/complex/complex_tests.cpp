
#include <sstream>


#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "complex.hpp"
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

static void PrintFail(const char *dis, int line, char *value, char *bad_value)
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
namespace hrd11
{
	void operator<<(char* str, Complex& complex)
	{
		sprintf(str, "(%.3f, %.3f)", complex.m_real, complex.m_img);
	}
}

void TestBasicOperators()
{
	SetTest(1, "BasicOperators");

	{
		Complex a(1.7,1.5);
		Complex b(6.3, 1.5);
		Complex correct(8, 3);
		a += b;
		ReturnComper<Complex>(correct, a, "Operator +=", __LINE__);
	}
	{
		Complex a(1.7,1.5);
		Complex b(0.7, 0.5);
		Complex correct(1, 1);
		a -= b;
		ReturnComper<Complex>(correct, a, "Operator -=", __LINE__);
	}
	{
		Complex a(5.5,1.5);
		Complex correct(28, 16.5);
		a *= a;
		ReturnComper<Complex>(correct, a, "Operator *=", __LINE__);
	}
	{
		Complex a(5.5,1.5);
		Complex correct(1, 0);
		a /= a;
		ReturnComper<Complex>(correct, a, "Operator /=", __LINE__);
	}
	{
		Complex a;
		Complex b(5.5, 1.5);
		Complex correct(8.5, 1.5);
		a = b + 3;
		ReturnComper<Complex>(correct, a, "Operator +", __LINE__);
		a = 3 + b;
		ReturnComper<Complex>(correct, a, "Operator +", __LINE__);
	}
	{
		Complex a;
		Complex b(5.5, 1.5);
		Complex correct(2.5, 1.5);
		a = b - 3;
		ReturnComper<Complex>(correct, a, "Operator -", __LINE__);
	}
	{
		Complex a;
		Complex b(5.5, 1.5);
		Complex correct(11, 3);
		a = b * 2;
		ReturnComper<Complex>(correct, a, "Operator -", __LINE__);
	}
	{
		Complex a(2, 4);
		Complex b(5, 5);
		Complex correct(1.5, -0.5);
		a = b / a;
		ReturnComper<Complex>(correct, a, "Operator -", __LINE__);
	}
}

static void TestSet()
{
	SetTest(2, "Set Funcs");

	{
		Complex num(2, 2);
		Complex correct(5, 5);
		num.SetReal(5);
		num.SetImg(5);
		ReturnComper<Complex>(correct, num, "Using SetImg && SetReal", __LINE__);
	}
}

static void TestIO()
{
	SetTest(3, "IO (cheack stdout)");

	{
		Complex num(2, 2);
		std::cin >> num;
		std::cout << num;
	}
}

static void TestAll()
{
	SetTest(4, "Going wild");

	{
		Complex a(2.2, 4.2);
		Complex b(10, 5.5);
		Complex c(7.2, 3.3);
		Complex correct(-16.475, -33);
		a /= a;
		b = a + c * 2 + 5.5 - (b * c);
		b /= (a + a);
		ReturnComper<Complex>(correct, b, "All Operators", __LINE__);
	}
}

static void TestCtor()
{
	SetTest(5, "Ctor");

	{
		Complex a;
		Complex c_a(0, 0);
		ReturnComper<Complex>(c_a, a, "defualt ctor", __LINE__);
	}
	{
		Complex a(10);
		Complex c_a(10, 0);
		ReturnComper<Complex>(c_a, a, "one input ctor", __LINE__);
	}
	{
		Complex c_a(5.5, 4.4);
		Complex a(c_a);
		ReturnComper<Complex>(c_a, a, "cctor", __LINE__);
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
		TestBasicOperators();
		break ;

		case 2:
		TestSet();
		break ;

		case 3:
		TestIO();
		break ;

		case 4:
		TestAll();
		break ;

		case 5:
		TestCtor();
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
		TestBasicOperators();
		TestSet();
		TestIO();
		TestAll();
		TestCtor();
	}
	PrintSummery();

	return 0;
}
