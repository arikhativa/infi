#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "vsa.h"
#include "color.h"

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
#ifdef NOT
#endif
static void PrintMem(ssize_t *run, size_t size)
{
	while (size)
	{
		printf("%ld", *run);
		++run;
		--size;
		printf(" _ ");
	}
}
static void TestInit()
{
	vsa_t *mem = 0;
	size_t buff_size = 71;

	SetTest(1, "Init");

	mem = (vsa_t*)malloc(buff_size);

	ReturnComper(0, (size_t)VSAInit(mem, 23));

	memset(mem, 0, buff_size);

	mem = VSAInit(mem, buff_size);

	free(mem);
}

static void TestAlloc()
{
	vsa_t *mem = 0;
	ssize_t *tmp = 0;
	ssize_t *run = 0;
	ssize_t *run1 = 0;
	ssize_t *run2 = 0;
	int i = 0;
	size_t buff_size = (8 * 10) - 5;

	SetTest(2, "Alloc");

	mem = (vsa_t*)malloc(buff_size);

	memset(mem, 0, buff_size);

	mem = VSAInit(mem, buff_size);

	run = VSAAlloc(mem, (2*8)-1);
	for (i = 0; i < 2; ++i)
		*(run + i) = -1;

	tmp = VSAAlloc(mem, (5*8)-7);
	ReturnComper(0, (size_t)tmp);
	tmp = VSAAlloc(mem, 0);
	ReturnComper(0, (size_t)tmp);

	run1 = VSAAlloc(mem, 2*8);
	for (i = 0; i < 2; ++i)
		*(run + i) = -1;
	run2 = VSAAlloc(mem, 1*8);
	for (i = 0; i < 1; ++i)
		*(run + i) = -1;

	VSAFree(run);
	VSAFree(run1);
	VSAFree(run2);

	run = VSAAlloc(mem, 6*8);
	for (i = 0; i < 6; ++i)
		*(run + i) = -1;
	VSAFree(run);
	run = VSAAlloc(mem, 7*8);
	for (i = 0; i < 7; ++i)
		*(run + i) = -1;

	ReturnComper(0, (!((size_t)run)));

	free(mem);
}

static void TestFree()
{
	vsa_t *mem = 0;
	ssize_t *run = 0;
	size_t buff_size = (8 * 10) - 5;

	SetTest(3, "Free");

	mem = (vsa_t*)malloc(buff_size);

	memset(mem, 0, buff_size);

	mem = VSAInit(mem, buff_size);

	run = VSAAlloc(mem, 7*8);

	VSAFree(run);
	VSAFree(run);
	
	run = VSAAlloc(mem, 7*8);
	ReturnComper(0, (!((size_t)run)));

	VSAFree(run);
	run = VSAAlloc(mem, 8);
	VSAFree(run);
	run = VSAAlloc(mem, 8);
	VSAFree(run);
	run = VSAAlloc(mem, 7*8);
	ReturnComper(0, (!((size_t)run)));

	free(mem);
}

static void TestLargest()
{
	vsa_t *mem = 0;
	ssize_t tmp = 0;
	ssize_t *run = 0;
	ssize_t *run1 = 0;
	ssize_t *run2 = 0;
	size_t buff_size = (8 * 21) - 7;

	SetTest(4, "Largest");

	mem = (vsa_t*)malloc(buff_size);

	memset(mem, 0, buff_size);

	mem = VSAInit(mem, buff_size);

	run = VSAAlloc(mem, 2*8);
	run1 = VSAAlloc(mem, 2*8);
	run2 = VSAAlloc(mem, 2*8);
	VSAAlloc(mem, 4*8);

	VSAFree(run);
	VSAFree(run1);
	VSAFree(run2);

	tmp = VSALargestChunkAvailable(mem);
	tmp = VSALargestChunkAvailable(mem);
	tmp = VSALargestChunkAvailable(mem);
	tmp = VSALargestChunkAvailable(mem);

	ReturnComper(8 * 8, tmp);

	free(mem);
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
			TestInit();
			break ;
		case 2:
			TestAlloc();
			break ;
		case 3:
			TestFree();
			break ;
		case 4:
			TestLargest();
			break ;
		case 5:
			break ;
		default:
			TestInit();
			TestAlloc();
			TestFree();
			TestLargest();
	}
	PrintSummery();

	return 0;
}
