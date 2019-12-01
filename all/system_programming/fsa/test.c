#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "fsa.h"
#include "color.h"

static int count_test = 1;
static int t_index = 1;
static int pass = 0;

static int boly = 1;
static int num = 1;
static char *name;

struct fsa
{
        size_t block_size;
        size_t mem_size;
        size_t next;
};


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

static void MemoryComper(void *mem1, void *mem2, size_t n)
{
        if (0 == memcmp(mem1, mem2, n))
                Pass();
        else
                PrintFail(" -- Diffrence in memory.");
}

static void ReturnComper(size_t num1, size_t num2)
{
	if (num1 == num2)
		Pass();
	else
		PrintFail(" -- Diffrence in return value.");
}
/*
static void PrintMem(size_t *run, size_t size, size_t byte)
{
	int i = 3;

	while (i--)
	{
		printf("%ld", *run);
		printf("-");
		++run;
		--size;
	}
		printf("-");
	i = 0;
	while (size)
	{
		printf("%ld", *run);
		++run;
		--size;
		++i;
		if (!(i % byte))
			printf("---");
		else
			printf("|");
	}
}
*/
static void TestSugSize()
{
	SetTest(1, "Suggsted size");

	ReturnComper(72, FSASuggestSize(2, 13));
	ReturnComper(56, FSASuggestSize(2, 1));
	ReturnComper(56, FSASuggestSize(2, 8));
	ReturnComper(0, FSASuggestSize(2, 0));
	ReturnComper(0, FSASuggestSize(0, 13));
}


static void TestInit()
{
	fsa_t *mem = 0;
	size_t buff_size = 0;
	size_t b_size = 1;

	SetTest(2, "Init");

	buff_size =  FSASuggestSize(3, b_size);
	mem = (fsa_t*)malloc(buff_size);

	memset(mem, 0, buff_size);

	ReturnComper(0, (size_t)FSAInit(mem, 0, b_size));
	ReturnComper(0, (size_t)FSAInit(mem, 5, 0));

	free(mem);
}

static void TestAlloc()
{
	fsa_t *mem = 0;
	size_t *tmp = 0;
	size_t *b1 = 0;
	size_t *b2 = 0;
	size_t *b3 = 0;
	size_t buff_size = 0;
	size_t b_size = 50;
	size_t i = 0;
	size_t real1[] = {0, 1, 2, 3, 4, 5, 6};
	size_t real2[] = {0, 2, 4, 6, 8, 10, 12};
	size_t real3[] = {0, 3, 6, 9, 12, 15, 18};

	SetTest(3, "Alloc");

	buff_size =  FSASuggestSize(3, b_size);
	mem = (fsa_t*)malloc(buff_size);

	memset(mem, 0, buff_size);
	mem = FSAInit(mem, buff_size, b_size);

	b1 = FSAAlloc(mem);
	for (i = 0; i < 7; ++i)
		*(b1 + i) = i;

	b2 = FSAAlloc(mem);
	for (i = 0; i < 7; ++i)
		*(b2 + i) = i * 2;

	b3 = FSAAlloc(mem);
	for (i = 0; i < 7; ++i)
		*(b3 + i) = i * 3;

	tmp = FSAAlloc(mem);
	tmp = FSAAlloc(mem);
	tmp = FSAAlloc(mem);
	tmp = FSAAlloc(mem);
	tmp = FSAAlloc(mem);
	tmp = FSAAlloc(mem);
	tmp = FSAAlloc(mem);
	ReturnComper(0, (size_t)tmp);
	ReturnComper(0, FSACountFree(mem));

	MemoryComper(b1, real1, 7);
	MemoryComper(b2, real2, 7);
	MemoryComper(b3, real3, 7);

	free(mem);
}

static void TestFree()
{
	fsa_t *mem = 0;
	size_t *run1 = 0;
	size_t *run2 = 0;
	size_t *run3 = 0;
	size_t buff_size = 0;
	size_t b_size = 8;
	size_t a[] = {42, 45, 44};

	SetTest(4, "Free");

	buff_size =  FSASuggestSize(3, b_size);
	mem = (fsa_t*)malloc(buff_size);

	memset(mem, 0, buff_size);
	mem = FSAInit(mem, buff_size, b_size);

	run1 = FSAAlloc(mem);
	*run1 = 42;
	run2 = FSAAlloc(mem);
	*run2 = 43;
	run3 = FSAAlloc(mem);
	*run3 = 44;

	FSAFree(run2);

	run2 = FSAAlloc(mem);
	*run2 = 45;

	MemoryComper(run1, &a[0], 8);
	MemoryComper(run2, &a[1], 8);
	MemoryComper(run3, &a[2], 8);
	
	free(mem);
}

static void TestCount()
{
	fsa_t *mem = 0;
	size_t *run = 0;
	size_t *run2 = 0;
	size_t buff_size = 0;
	size_t b_size = 55;

	SetTest(5, "Count Free Space");

	buff_size =  FSASuggestSize(3, b_size);
	mem = (fsa_t*)malloc(buff_size);

	memset(mem, 0, buff_size);
	mem = FSAInit(mem, buff_size, b_size);

	ReturnComper(3, FSACountFree(mem));

	run = FSAAlloc(mem);
	run2 = FSAAlloc(mem);
	run = FSAAlloc(mem);

	ReturnComper(0, FSACountFree(mem));

	FSAFree(run2);
	ReturnComper(1, FSACountFree(mem));
	FSAFree(run);
	ReturnComper(2, FSACountFree(mem));

	run = FSAAlloc(mem);
	ReturnComper(1, FSACountFree(mem));
	run = FSAAlloc(mem);
	ReturnComper(0, FSACountFree(mem));
	
	free(mem);
}

/*static void Test1()
{
	fsa_t *mem = 0;
	size_t *run = 0;
	size_t *run2 = 0;
	size_t buff_size = 0;
	size_t b_size = 55;

	SetTest(5, "Count Free Space");


	buff_size =  FSASuggestSize(3, b_size);
	mem = (fsa_t*)malloc(buff_size);

	memset(mem, 0, buff_size);
	mem = FSAInit(mem, buff_size, b_size);

	ReturnComper(3, FSACountFree(mem));

	run = FSAAlloc(mem);
	run2 = FSAAlloc(mem);
	run = FSAAlloc(mem);

	ReturnComper(0, FSACountFree(mem));

	FSAFree(run2);
	ReturnComper(1, FSACountFree(mem));

	run = FSAAlloc(mem);
	ReturnComper(0, FSACountFree(mem));

	free(mem);
}
*/
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
			TestSugSize();
			break ;
		case 2:
			TestInit();
			break ;
		case 3:
			TestAlloc();
			break ;
		case 4:
			TestFree();
			break ;
		case 5:
			TestCount();
			break ;
		default:
			TestSugSize();
			TestInit();
			TestAlloc();
			TestFree();
			TestCount();
	}
	PrintSummery();

	return 0;
}
