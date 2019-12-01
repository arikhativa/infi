#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <sys/time.h>

#include "ws9.h"
#include "color.h"

static void PrintTime(char *s, struct timeval start, struct timeval stop)
{
	printf("Duration %s %.3f\n", s, (double) (stop.tv_sec - start.tv_sec) * 1000 + (double) (stop.tv_usec - start.tv_usec) / 1000);
}


char *strdup(const char *str);


void StringComper(const char *s1, const char *s2, size_t n ,int print)
{
	static int t = 1;

	if (print)
	{
		printf("-- Sys - %s\n", s1);
		printf("-- Me  - %s\n", s2);
	}
	if (0 == memcmp(s1, s2, n))
		printf("-- Test %d"GREEN" - Pass :)\n"RESET, t);
	else
		printf("-- Test %d"RED" - Fail :(\n"RESET, t);
	++t;
}

void Memsetcomper(const char *str, size_t n, int print)
{
	int c = '0';
	char *s1;
	char *s2;
	char *s3;
	int i = 50;
	struct timeval start;
	struct timeval stop;

	s1 = strdup(str);
	s2 = strdup(str);

	s3 = (s1 + 1);

	if (print)
	{
		gettimeofday(&start, NULL);
		while (0 < i)
		{
			s1 = memset(s1, c, n);
			--i;
		}
		i = 50;
		gettimeofday(&stop, NULL);
		PrintTime("Sys", start, stop);
		gettimeofday(&start, NULL);
		while (0 < i)
		{
			s2 = Memset(s2, c, n);
			--i;
		}
		gettimeofday(&stop, NULL);
		PrintTime("Me ", start, stop);
	}
	else
	{
		s1 = memset(s1, c, n);
		s2 = Memset(s2, c, n);
	}
	StringComper(s1, s2, strlen(s1), print);
	free(s1);
	free(s2);
	s1 = NULL;
	s2 = NULL;
}

void MemcpyComper(const char *str, size_t n, int print)
{
	char *s1;
	char *s2;
	char *dest1;
	char *dest2;

	s1 = strdup(str);
	s2 = strdup(str);
	dest1 = strdup("1234567895555555555555555555555555555555555555555555555");
	dest2 = strdup("1234567895555555555555555555555555555555555555555555555");

	dest1 = memcpy(dest1, s1, n);
	dest2 = Memcpy(dest2, s2, n);

	StringComper(dest1, dest2, strlen(dest1), print);
	free(s1);
	free(s2);
	free(dest1);
	free(dest2);
	s1 = NULL;
	s2 = NULL;
	dest1 = NULL;
	dest2 = NULL;
}

void MemmoveComper(const char *str, size_t n, int front, int print)
{
	char *s1;
	char *s2;
	char *dest1;
	char *dest2;

	s1 = strdup(str);
	s2 = strdup(str);
	if (front == 1)
	{
		dest1 = s1 + 2;
		dest2 = s2 + 2;
		dest1 = memmove(dest1, s1, n);
		dest2 = Memmove(dest2, s2, n);
	}
	else
	{
		dest1 = s1;
		dest2 = s2;
		dest1 = memmove(dest1, s1 + 2, n);
		dest2 = Memmove(dest2, s2 + 2, n);
	}
	StringComper(dest1, dest2, strlen(dest1), print);
}

void AtoiComper(const char *str, int print)
{
	static int t = 1;


	if (print)
	{
		printf("-- Sys - %d\n", atoi(str));
		printf("-- Me  - %d\n", Atoi(str));
	}
	if (Atoi(str) == atoi(str))
		printf("-- Test %d"GREEN" - Pass :)\n"RESET, t);
	else
		printf("-- Test %d"RED" - Fail :(\n"RESET, t);
	++t;
}

void ItoaComper(int number, int print)
{
	char s1[12] = { 0 };
	char s2[12] = { 0 };

	sprintf(s1, "%d", number);
	ItoaBaseTen(number, s2);
	StringComper(s1, s2, 12, print);
}

void ItoaBaseComper(int number, int base, int print, char *str)
{
	char s1[100] = { 0 };

	ItoaAnyBase(number, s1, base);
	StringComper(s1, str, strlen(str), print);
}

void Apper1and2not3(const char *s1, const char *s2, const char *s3, char *res, int print)
{
	if (print)
	{
		printf("-- %s == %s\n", "s1", s1);
		printf("-- %s == %s\n", "s2", s2);
		printf("-- %s == %s\n", "s3", s3);
	}
	printf("-- %s == %s\n", "real res", res);
	printf("-- %s == ", "func res");
	AppearOnlyFirstTwo(s1, s2, s3);
	printf("\n\n");
}

void TestMemset(int print)
{
	printf(CYAN"\n-- Testing Memset:\n\n"RESET);
	Memsetcomper("12345678905555555555555555555555533333333333333333333333333333333333333333333333333333333333333333333333333333333333333333355553333333333333333333333333333333333333333333333333333333333333333333333333333333333333333335555333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333555533333333333333333333333333333333333333333333333333333333333333333333333333333333333333333355553333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333", 500, print);
	Memsetcomper("123456789055555555555555555555555", 10, print);
	Memsetcomper("123456789055555555555555555555555", 0, print);
}

void TestMemcpy(int print)
{
	printf(CYAN"\n-- Testing Memcpy:\n\n"RESET);
	MemcpyComper("0000000000000000000000000000000000000000000000000000000000", 41, print);
	MemcpyComper("0000000000000000000000000000000000000000000000000000000000", 40, print);
	MemcpyComper("0000", 0, print);
	MemcpyComper("0000", 2, print);
}

void TestMemmove(int print)
{
	printf(CYAN"\n-- Testing Memmove:\n\n"RESET);
	MemmoveComper("123456", 3, 1, print);
	MemmoveComper("123456", 3, 0, print);
}

void TestAtoi(int print)
{
	printf(CYAN"\n-- Testing Atoi:\n\n"RESET);
	AtoiComper("2147483648", print);
	AtoiComper("2147483647", print);
	AtoiComper("-2147483648", print);
}

void TestItoa(int print)
{
	printf(CYAN"\n-- Testing Itoa 10:\n\n"RESET);
	ItoaComper(-2147483648, print);
	ItoaComper(2147483647, print);
	ItoaComper(0, print);
	ItoaComper(-13, print);
}

void TestItoaBase(int print)
{
	printf(CYAN"\n-- Testing Itoa Base:\n\n"RESET);
	ItoaBaseComper(22, 10, print, "22");
	ItoaBaseComper(0, 10, print, "0");
	ItoaBaseComper(10, 11, print, "a");
	ItoaBaseComper(54637, 2, print, "1101010101101101");
	ItoaBaseComper(-255, 16, print, "-ff");
	ItoaBaseComper(1000, 16, print, "3e8");
	ItoaBaseComper(55484, 36, print, "16t8");
	ItoaBaseComper(-2147483648, 16, print, "-80000000");
	ItoaBaseComper(-2147483648, 36, print, "-zik0zk");

}

void TestApper(int print)
{
	printf(CYAN"\n-- Testing AppearInFirstTwo:\n\n"RESET);
	Apper1and2not3("123y8oyyo4a65vv4v", "oy8465168vvv4a654v6546", "65465o545a654", "yv", print);
	Apper1and2not3("one", "one", "on", "e", print);
	Apper1and2not3("", "123a", "yyy", "", print);
	Apper1and2not3("123a", "1a23", "123", "a", print);
}

void TestIsLittle()
{
	printf(CYAN"\n-- Testing Is Little Endian:\n\n"RESET);
	if (IsLittleEndian(1))
		printf("-- Is little Endian\n");
	else
		printf("-- Is Big Endian\n");
}

int main(int ac, char **av)
{
	int a = 0;
	int print = 0;

	if (2 <= ac)
	{
		a = atoi(av[1]);
		if (0 == strcmp(av[1], "-p"))
		{
			print = 1;
			if (3 == ac)
				a = atoi(av[2]);
		}
	}
	switch (a)
	{
		case 1:
			TestMemset(print);
			break ;
		case 2:
			TestMemcpy(print);
			break ;
		case 3:
			TestMemmove(print);
			break ;
		case 4:
			TestAtoi(print);
			break ;
		case 5:
			TestItoa(print);
			break ;
		case 6:
			TestItoaBase(print);
			break ;
		case 7:
			TestApper(print);
			break ;
		case 8:
			TestIsLittle();
			break ;
		default:
			TestMemset(print);
			TestMemcpy(print);
			TestMemmove(print);
			TestAtoi(print);
			TestItoa(print);
			TestItoaBase(print);
			TestApper(print);
			TestIsLittle();
	}

	return 0;
}

