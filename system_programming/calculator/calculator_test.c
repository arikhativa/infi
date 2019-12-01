#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "color.h"
#include "calculator.h"
#include "stack.h"

static int count_test = 1;
static int t_index = 1;
static int pass = 0;

static int boly = 1;
static int num = 1;
static char *name;



static void SetTest(int n, char *str)
{
	num = n;
	name = str;
	boly = 0;
	t_index = 1;
}

static void Pass()
{
	++count_test;
	++pass;
	++t_index;
}

static void PrintFail(char *str)
{
	if (!boly)
	{
		printf(CYAN"\n-- (%d) Test %s:\n\n"RESET, num, name);
		++boly;
	}
	printf("-- Test"GRAY" %d"RESET RED" - Fail :( "RESET, t_index);
	printf("%s\n", str);
	++count_test;
	++t_index;
}

static void PrintSummery(void)
{
	--count_test;
	if (pass == count_test)
		printf(GREEN"\n-- Pass :) (%d/%d)\n\n"RESET,pass, count_test);
	else
		printf(YELLOW"\n-- Not good enough.. (%d/%d)\n\n"RESET,pass, count_test);
}

static void ReturnComper(size_t num1, size_t num2)
{
	if (num1 == num2)
		Pass();
	else
		PrintFail(" -- Diffrence in return value.");
}

static void MemoryComper(void *mem1, void *mem2, size_t n)
{
	if (0 == memcmp(mem1, mem2, n))
		Pass();
	else
		PrintFail(" -- Diffrence in memory.");
}

static void TestCreateAndDestroy()
{
	double res = 0;
	char str[] = "1.5 - 1.5 + 15 - 32.7 + 70";

	SetTest(1, "Create & Destroy (run with vlg)");

	Calculator(str, &res);
}

static void TestSyntax()
{
	double res = 0;
	double r1 = 1.5;
	double r2 = -15;
	char s1[] = " 1.5 ";
	char s2[] = ".1.5";
	char s3[] = "1.5.5";
	char s4[] = "1 5";
	char s5[] = " + 15";
	char s6[] = "-15";

	SetTest(2, "Syntax");

	ReturnComper(SUCCESS, Calculator(s1, &res));
	MemoryComper(&r1, &res, sizeof(double));
	ReturnComper(SUCCESS, Calculator(s6, &res));
	MemoryComper(&r2, &res, sizeof(double));
	ReturnComper(SYNTAX_ERROR, Calculator(s2, &res));
	ReturnComper(SYNTAX_ERROR, Calculator(s3, &res));
	ReturnComper(SYNTAX_ERROR, Calculator(s4, &res));
	ReturnComper(SYNTAX_ERROR, Calculator(s5, &res));
}

static void TestPlus()
{
	int size = 4;
	int i = 0;
	double res = 0;
	double c[] = {71.5, 71.5, 8.5, 9};
	char *tab[4];
	char s1[] = "1.5 + 70";
	char s2[] = "1.5 + 70 + 0";
	char s3[] = "1.5 + 07";
	char s4[] = "1.5 + 7.5";

	tab[0] = s1;
	tab[1] = s2;
	tab[2] = s3;
	tab[3] = s4;

	SetTest(3, "Plus");

	for (i = 0; i < size; ++i)
	{
		ReturnComper(SUCCESS, Calculator(tab[i], &res));
		MemoryComper(&c[i], &res, sizeof(double));
	}
}

static void TestMinus()
{
	int size = 4;
	int i = 0;
	double res = 0;
	double c[] = {-66.5, -5.5, 34, 2};
	char *tab[4];
	char s1[] = "1.5 + 2 - 70";
	char s2[] = "1.5 - 7 - 0";
	char s3[] = "1 - 7 + 100 - 60";
	char s4[] = "1.5 - 7.5 + 10 - 3 + 0 - 0 + 1";

	tab[0] = s1;
	tab[1] = s2;
	tab[2] = s3;
	tab[3] = s4;

	SetTest(4, "Minus");

	for (i = 0; i < size; ++i)
	{
		ReturnComper(SUCCESS, Calculator(tab[i], &res));
		MemoryComper(&c[i], &res, sizeof(double));
	}
}

static void TestMulitiplay()
{
	int size = 4;
	int i = 0;
	double res = 0;
	double c[] = {2, 5, -729, 1347.5};
	char *tab[4];
	char s1[] = "1 * 2";
	char s2[] = "1 + 2 * 2";
	char s3[] = "1 - 7 * 100 + 60 * 0 - 30 * 1";
	char s4[] = "2 * 1.5 - 7.5 + 10 * 3 * 9 * 5 + 2 + 0 * 0 * 1";

	tab[0] = s1;
	tab[1] = s2;
	tab[2] = s3;
	tab[3] = s4;

	SetTest(5, "Mulitiplay");
/*	Calculator(tab[3], &res);
	printf("-- str %s\n", tab[3]);
	printf("-- res %f\n", res);*/

	for (i = 0; i < size; ++i)
	{
		ReturnComper(SUCCESS, Calculator(tab[i], &res));
		MemoryComper(&c[i], &res, sizeof(double));
	}
}

static void TestDivide()
{
	int size = 4;
	int i = 0;
	double res = 0;
	double c[] = {1, 2, 3, -17};
	char *tab[4];
	char s1[] = "2 / 2";
	char s2[] = "1 + 2 / 2";
	char s3[] = "9 / 1 / 3";
	char s4[] = "2 - 10 / 2 * 10 / 5 - 9 + 5 * 2 * 0 * 0 / 1";
	char s5[] = "10/0";

	tab[0] = s1;
	tab[1] = s2;
	tab[2] = s3;
	tab[3] = s4;

	SetTest(6, "Divide");

	for (i = 0; i < size; ++i)
	{
		ReturnComper(SUCCESS, Calculator(tab[i], &res));
		MemoryComper(&c[i], &res, sizeof(double));
	}
	ReturnComper(DIVIDE_BY_ZERO, Calculator(s5, &res));
}

static void TestPara()
{
	int size = 4;
	int i = 0;
	double res = 0;
	double c[] = {36, 1, -8, 10};
	char *tab[4];
	char s1[] = "3 * (2 * (1 + 5))";
	char s2[] = "10 / ((3 - 1) * (2 + 3))";
	char s3[] = "((((((2) - (10))))))";
	char s4[] = "(2 - 10) / 2 * 10 / ((5 - 9) + 5 * 2 * 0 * 0) / 1";
	char s5[] = ")10";
	char s6[] = "10 + 2)";
	char s7[] = "(10 + 2";

	tab[0] = s1;
	tab[1] = s2;
	tab[2] = s3;
	tab[3] = s4;

	SetTest(7, "Para");

	/*i = 3;
	Calculator(tab[i], &res);
	printf("-- str %s\n", tab[i]);
	printf("-- res %f\n", res);*/

	for (i = 0; i < size; ++i)
	{
		ReturnComper(SUCCESS, Calculator(tab[i], &res));
		MemoryComper(&c[i], &res, sizeof(double));
	}
	ReturnComper(SYNTAX_ERROR, Calculator(s5, &res));
	ReturnComper(SYNTAX_ERROR, Calculator(s6, &res));
	ReturnComper(SYNTAX_ERROR, Calculator(s7, &res));
}

static void TestPower()
{
	int size = 4;
	int i = 0;
	double res = 0;
	double c[] = {0.125, -0.00001, 256, 1, 4};
	char *tab[5];
	char s1[] = "2 ^ -3";
	char s2[] = "-10 ^ -5";
	char s3[] = "2 ^ (2 ^ 3)";
	char s4[] = "10 ^ 0";
/*	char s5[] = "2 ^ (10 * 4 - 38) + 300 ^ (-2 - 1) ";*/

	tab[0] = s1;
	tab[1] = s2;
	tab[2] = s3;
	tab[3] = s4;

	SetTest(8, "Power");

/*	i = 4;
	printf("-- str %s\n", tab[i]);
	Calculator(s5, &res);
	printf("-- res %f\n", res);
	printf("-- c %f\n", c[i]);*/

	for (i = 0; i < size; ++i)
	{
		ReturnComper(SUCCESS, Calculator(tab[i], &res));
		MemoryComper(&c[i], &res, sizeof(double));
	}
}

static void TestNegNum()
{
	int size = 4;
	int i = 0;
	double res = 0;
	double c[] = {-1, 5, -8, 25};
	char *tab[4];
	char s1[] = "2 + -3";
	char s2[] = "2 - -3";
	char s3[] = "-(-((-(((-2) - (-10))))))";
	char s4[] = "-5 * -5";
	char s5[] = "- -10";
	char s6[] = "10 + -(2---2)";

	tab[0] = s1;
	tab[1] = s2;
	tab[2] = s3;
	tab[3] = s4;

	SetTest(9, "Negativ Numbers");

/*	i = 2;
	printf("-- str %s\n", s6);
		Calculator(s6, &res);
	printf("-- res %f\n", res);*/

	for (i = 0; i < size; ++i)
	{
		ReturnComper(SUCCESS, Calculator(tab[i], &res));
		MemoryComper(&c[i], &res, sizeof(double));
	}
	ReturnComper(SYNTAX_ERROR, Calculator(s5, &res));
	ReturnComper(SYNTAX_ERROR, Calculator(s6, &res));
}

static void TestInfi()
{
	int size = 2;
	int i = 0;
	double res = 0;
	double c[] = {0, 48};
	char *tab[4];

	char s1[] = "8+8*3+-2^5";
	char s2[] = "8+8*((3-2)*5)";

	char s3[] = "8+8*3-2^";
	char s4[] = "10/0";
	char s5[] = "3-2)*5";
	char s6[] = "(3-2)*5+ 5*(4+4+4";

	tab[0] = s1;
	tab[1] = s2;

	SetTest(10, "Infi Test");

	for (i = 0; i < size; ++i)
	{
		ReturnComper(SUCCESS, Calculator(tab[i], &res));
		MemoryComper(&c[i], &res, sizeof(double));
	}
	ReturnComper(SYNTAX_ERROR, Calculator(s3, &res));
	ReturnComper(DIVIDE_BY_ZERO, Calculator(s4, &res));
	ReturnComper(SYNTAX_ERROR, Calculator(s5, &res));
	ReturnComper(SYNTAX_ERROR, Calculator(s6, &res));
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
			TestSyntax();
			break ;
		case 3:
			TestPlus();
			break ;
		case 4:
			TestMinus();
			break ;
		case 5:
			TestMulitiplay();
			break ;
		case 6:
			TestDivide();
			break ;
		case 7:
			TestPara();
			break ;
		case 8:
			TestPower();
			break ;
		case 9:
			TestNegNum();
			break ;
		case 10:
			TestInfi();
			break ;

		default:
			TestCreateAndDestroy();
			TestSyntax();
			TestPlus();
			TestMinus();
			TestMulitiplay();
			TestDivide();
			TestPara();
			TestPower();
			TestNegNum();
			TestInfi();
			break ;
	}
	PrintSummery();

	return 0;
}
