
#include <sstream>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>  // istream, ostream

#include "shared_pointer.hpp"
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

void Foo(SharedPointer<int> ptr)
{
	*ptr = 6;
}

void TestBasicCycel()
{
	SetTest(1, "Basic Cycel -- use vlg");

	SharedPointer<int> p1(new int(5));
	SharedPointer<int> p2(p1);
	SharedPointer<int> p3;

	p3 = p1;

	Foo(p1);
	Foo(p2);
	Foo(p3);
}

struct X
{
	int m_a;
};

void TestOperators()
{
	SetTest(2, "Operators");

	{
		char dis[] = "SP<>p2(p1)\n-- p2 == p1";
		SharedPointer<int> p1(new int(5));
		SharedPointer<int> p2(p1);

		int tmp = p2 == p1;
		ComperInt(1, tmp, dis, __LINE__);
	}
	{
		char dis[] = "SP<>p1(new int)\n-- !p1";
		char dis2[] = "SP<>p2\n-- !p2";
		SharedPointer<int> p1(new int(5));
		SharedPointer<int> p2;

		int tmp = (!p1);
		ComperInt(0, tmp, dis, __LINE__);
		tmp = !p2;
		ComperInt(1, tmp, dis2, __LINE__);
	}
	{
		char dis[] = "SP<>p1(new int(5))\n-- *p1 = 50\n-- *p1 == ?";
		SharedPointer<int> p1(new int(5));

		*p1 = 50;
		ComperInt(50, *p1, dis, __LINE__);
	}
	{
		char dis[] = "SP<>p1(new X(50))\n-- p1->m_a == ?";
		SharedPointer<X> p1(new X());

		p1->m_a = 50;

		ComperInt(50, p1->m_a, dis, __LINE__);
	}
}

class Base
{
public:
	Base() : m_a(new int(7))
	{}
	virtual ~Base()
	{
		delete m_a;
		// std::cout << "-- Base Dtor \n";
	}
private:
	int* m_a;
};

// class Derive
class Derive : public Base
{
public:
	Derive() : m_b(new int(3))
	{}
	~Derive()
	{
		delete m_b;
		// std::cout << "-- Derive Dtor \n";
	}
private:
	int* m_b;
};

void TestMembers()
{
	SetTest(3, "Members");

	{
		char dis[] = "GetPtr() - check if adderss match.";
		int* a = new int(2);
		SharedPointer<int> p1(a);

		int* b = p1.GetPtr();

		int tmp = (a == b);

		ComperInt(1, tmp, dis, __LINE__);
	}
	{
		SharedPointer<Base> base;
		SharedPointer<Derive> der (new Derive());

		base = der;
	}
	{
		SharedPointer<Derive> der (new Derive());
		SharedPointer<Base> base(der);
	}
	{
		SharedPointer<Derive> bp;
		// SharedPointer<Base> dp (new Base());
		//
		// bp = dp;
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
			TestOperators();
			break ;
		}

		case 3:
		{
			TestMembers();
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
			TestOperators();
			TestMembers();

        }
	}
	PrintSummery();

	return 0;
}
