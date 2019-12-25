
#include <sstream>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>  // istream, ostream

#include "rcstring.hpp"
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
	void operator<<(char* s, RCString& str)
	{
		sprintf(s, "%s", str.ToCstring());
	}
}

void TestBasicCycel()
{
	SetTest(1, "Basic Cycel");

	RCString s2("123");
	RCString s3 = "123";
    RCString s1(s2);
	RCString s4(s2);
	RCString s5 = s2;

	s1 = s3 = s2;

    s1[0] = 'a';

    // char ch = s1[0];

    // std::cout << "-- ch - " << ch << std::endl;
    // std::cout << "-- s1 - " << s1 << std::endl;
    // std::cout << "-- s4 - " << s4 << std::endl;
}

void TestMembers()
{
	SetTest(2, "Member Funcs");

	{
		RCString s1 = "123";
		RCString s2 = "456";
		char dis[] = "Checking value of (s1 + s2) -> \"123\" + \"456\"";

		s1 += s2;

		ReturnComper<RCString>("123456", s1, dis, __LINE__);
	}
	{
		RCString s1 = "123";
		char dis[] = "RCString s1 = \"123\". What will s1[0] be?";

		ComperInt('1', s1[0], dis, __LINE__);

	}
	{
		const RCString s1 = "123";
		char dis[] = "const RCString s1 = \"123\". What will s1[0] be?";

		ComperInt('1', s1[0], dis, __LINE__);
	}

	{
		RCString s1 = "123";
		const char *str = s1.ToCstring();
		size_t len = s1.Length();
		size_t cor = 3;

		ComperInt(cor, len, "Cheking Return value of RCString.Length()", __LINE__);
		ComperInt(cor, strlen(str), "Cheking the len of RCString.ToCstring()", __LINE__);
	}
}

void TestOperators()
{
	SetTest(3, "Operator overloading");

	{
		RCString s1 = "123";
		RCString s2 = "456";
		int a = (s1 == s2);

		ComperInt(0, a, "Checking value of (s1 == s2)", __LINE__);

		a = (s2 == s2);
		ComperInt(1, a, "Checking value of (s2 == s2)", __LINE__);
	}
	{
		RCString s1 = "123";
		RCString s2 = "456";
		int a = (s1 != s2);

		ComperInt(1, a, "Checking value of (s1 != s2)", __LINE__);

		a = (s2 != s2);

		ComperInt(0, a, "Checking value of (s2 != s2)", __LINE__);
	}
	{
		RCString s1 = "123";
		RCString s2 = "456";
		int a = (s1 < s2);

		ComperInt(1, a, "Checking value of (s1 < s2)", __LINE__);

		a = (s2 < s2);

		ComperInt(0, a, "Checking value of (s2 < s2)", __LINE__);
	}
	{
		RCString s1 = "123";
		RCString s2 = "456";
		int a = (s1 > s2);

		ComperInt(0, a, "Checking value of (s1 > s2)", __LINE__);

		a = (s2 > s2);

		ComperInt(0, a, "Checking value of (s2 > s2)", __LINE__);
	}
	{
		RCString s1 = "123";
		RCString s2 = "456";
		char dis[] = "Checking value of (s1 + s2) -> \"123\" + \"456\"";

		s1 = s1 + s2;

		ReturnComper<RCString>("123456", s1, dis, __LINE__);
	}
}

void TestIO()
{
	RCString s1 = "123";

    SetTest(4, "IO");

	std::cout << "-- s1 before - " << s1 << std::endl;
	std::cin >> s1;
	std::cout << "-- s1 after1 - " << s1 << std::endl;
	std::cin >> s1;
	std::cout << "-- s1 after2 - " << s1 << std::endl;
}

void TestRC()
{
	SetTest(5, "RCString");

	{
		RCString s1 = "123";
		RCString s2(s1);
		RCString s3(s2);
		RCString s4(s1);
        char dis[] = "s1 = '123'\n-- s2(s1)\n-- Checking with X.ToString() if the addres is the same";

		int a = (s1.ToCstring() == s2.ToCstring());
		a += (s1.ToCstring() == s4.ToCstring());
		a += (s1.ToCstring() == s3.ToCstring());

        ComperInt(3, a, dis, __LINE__);

        s2[0] = 't';
        a = (s1.ToCstring() == s2.ToCstring());
		a += (s1.ToCstring() == s4.ToCstring());
		a += (s1.ToCstring() == s3.ToCstring());

        ComperInt(2, a, dis, __LINE__);
	}
	{
		RCString s1 = "123";
		RCString s2(s1);
        s2[0] = 'a';
        char dis[] = "s1 = '123'\n-- s2(s1)\n-- s2[0] = 'a;\n-- Checking with X.ToString() if the addres is diffrent";

		int a = (s1.ToCstring() == s2.ToCstring());

		ComperInt(0, a, dis, __LINE__);
	}
	{
		RCString s1 = "123";
		RCString s2(s1);
        char tmp = s2[0];
        char dis[] = "s1 = '123'\n-- s2(s1)\n-- char tmp = s2[0]\n-- Checking with X.ToString() if the addres is the same";
        char dis2[] = "s1 = '123'\n-- s2(s1)\n-- char tmp = s2[0]\n-- Checking with X.ToString() if tmp == s2[0]";

		int a = (s1.ToCstring() == s2.ToCstring());

		ComperInt(1, a, dis, __LINE__);
        a = (tmp == s2[0]);

		ComperInt(1, a, dis2, __LINE__);
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
			TestMembers();
			break ;
		}

		case 3:
		{
			TestOperators();
			break ;
		}
		case 4:
		{
			TestIO();
			break ;
		}

		case 5:
        {
            TestRC();
            break ;
        }

		default:
        {
    		TestBasicCycel();
    		TestMembers();
    		TestOperators();
    		//TestIO();
            TestRC();
        }
	}
	PrintSummery();

	return 0;
}
