#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "cbuffer.h"
#include "color.h"

static int t = 1;

typedef enum status
{
	FAIL = -1,
	SUCCESS = 0
} status_t;

static void PrintPass()
{
	printf("-- Test"GRAY" %d"RESET GREEN" - Pass :)\n"RESET, t++);
}

static void PrintFail(char *str)
{
	printf("-- Test"GRAY" %d"RESET RED" - Fail :( "RESET, t++);
	printf("%s\n", str);
}

static void MemoryComper(void *mem1, void *mem2, size_t n)
{
	if (0 == memcmp(mem1, mem2, n))
		PrintPass();
	else
		PrintFail(" -- Diffrence in memory.");
}

static void SizeTComper(size_t num1, size_t num2)
{
	if (num1 == num2)
		PrintPass();
	else
		PrintFail(" -- Diffrence in size_t.");
}

static void TestCreateAndDestroy()
{
	c_buffer_t *buff = 0;

	printf(CYAN"\n-- (1) Test Cbuff Create And Destroy (run with vlg):\n\n"RESET);

	buff = CBufferCreate(2);

	CBufferDestroy(buff);
}

static void TestCapa()
{
	c_buffer_t *buff = 0;

	printf(CYAN"\n-- (2) Test Capa() :\n\n"RESET);

	buff = CBufferCreate(2);

	SizeTComper(2, CBufferCapacity(buff));

	CBufferDestroy(buff);
}

static void TestFreeSpace()
{
	c_buffer_t *buff = 0;
	char dest[20] = { 0 };

	printf(CYAN"\n-- (3) Test Free Space() :\n\n"RESET);

	buff = CBufferCreate(4);

	SizeTComper(4, CBufferFreeSpace(buff));
	CBufferWrite(buff, "12", 3);
	SizeTComper(1, CBufferFreeSpace(buff));
	CBufferWrite(buff, "12", 3);
	SizeTComper(0, CBufferFreeSpace(buff));
	CBufferRead(dest, buff, 3);
	SizeTComper(3, CBufferFreeSpace(buff));
	CBufferWrite(buff, "12", 2);
	SizeTComper(1, CBufferFreeSpace(buff));

	CBufferDestroy(buff);

	buff = CBufferCreate(5);
	CBufferWrite(buff, "12345", 5);
	CBufferRead(dest, buff, 4);
	CBufferWrite(buff, "12345", 2);
	SizeTComper(2, CBufferFreeSpace(buff));

	CBufferDestroy(buff);

}

static void TestWrite()
{
	c_buffer_t *buff = 0;
	char str[] = "abc";
	ssize_t tmp = 0;

	printf(CYAN"\n-- (4) Test Write() :\n\n"RESET);

	buff = CBufferCreate(7);

	tmp = CBufferWrite(buff, str, 3);
	SizeTComper(3, (size_t)tmp);
	tmp = CBufferWrite(buff, str, 4);
	SizeTComper(4, (size_t)tmp);
	tmp = CBufferWrite(buff, str, 4);
	SizeTComper(0, (size_t)tmp);
	CBufferDestroy(buff);
}

static void TestRead()
{
	c_buffer_t *buff = 0;
	char str[] = "abc";
	char str1[] = "12";
	char dest[20] = { 0 };
	ssize_t tmp = 0;

	printf(CYAN"\n-- (5) Test Read() :\n\n"RESET);

	buff = CBufferCreate(7);

	tmp = CBufferWrite(buff, str, 3);
	tmp = CBufferWrite(buff, str, 4);
	tmp = CBufferRead(dest, buff, 10);
	SizeTComper(7, (size_t)tmp);
	MemoryComper("abcabc", dest, 7);

	tmp = CBufferWrite(buff, str1, 3);
	SizeTComper(3, (size_t)tmp);

	tmp = CBufferRead(dest, buff, 20);
	MemoryComper("12", dest, 3);

	CBufferDestroy(buff);
}

static void TestEmpty()
{
	c_buffer_t *buff = 0;
	char str[] = "abc";
	char dest[20] = { 0 };

	printf(CYAN"\n-- (6) Test C Buff empty() :\n\n"RESET);

	buff = CBufferCreate(7);

	SizeTComper(1, CBufferIsEmpty(buff));
	CBufferWrite(buff, str, 3);
	SizeTComper(0, CBufferIsEmpty(buff));
	CBufferRead(dest, buff, 10);
	SizeTComper(1, CBufferIsEmpty(buff));

	CBufferDestroy(buff);
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
			TestCapa();
			break ;
		case 3:
			TestFreeSpace();
			break ;
		case 4:
			TestWrite();
			break ;
		case 5:
			TestRead();
			break ;
		case 6:
			TestEmpty();
			break ;

		default:
			TestCreateAndDestroy();
			TestCapa();
			TestFreeSpace();
			TestWrite();
			TestRead();
			TestEmpty();
	}

	return 0;
}
