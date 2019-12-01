#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include "ws8.h"
#include "ex2.h"
#include "ex3.h"
#include "ex4.h"

int TestEx1()
{
	element_t *arr;

	if (!(arr = InitArray(3)))
	{
		return 0;
	}
	InitFloat(arr, 2.5, 0);
	if (0 != InitString(arr, "bad!", 1))
	{
		FreeAll(arr);
		free(arr);
		arr = NULL;

		return 0;
	}
	InitInt(arr, 2, 2);
	if (0 != InitString(arr, "good!", 1))
	{
		FreeAll(arr);
		free(arr);
		arr = NULL;

		return 0;
	}
	InitInt(arr, 15, 2);

	if (0 == AddValToAll(arr, 10))
	{
		PrintAll(arr);
	}
	if (0 == AddValToAll(arr, 10))
	{
		PrintAll(arr);
	}

	FreeAll(arr);
	free(arr);
	arr = NULL;

	return 0;
}

void TestEx2()
{
	int a = 2;
	int b = 3;
	int c = 4;

	if (MAX3(a, b, c) == 4)
	{
		printf("\n-- Test 2\n-- Pass :)\n");
		return ;
	}
	printf("-- Test 2\n-- Fail :(\n");
}

void TestEx3()
{
	int a = 2;
	long b = 3;
	char c = 4;

	if (SIZEOF_VAR(a) == sizeof(a) \
			&& SIZEOF_VAR(b) == sizeof(b) \
			&& SIZEOF_VAR(c) == sizeof(c))
	{
		printf("\n-- Test 3\n-- Pass :)\n");
		return ;
	}
	printf("-- Test 3\n-- Fail :(\n");
}

void TestEx4()
{
	if (SIZEOF_TYPE(int) == sizeof(int) \
			&& SIZEOF_TYPE(long) == sizeof(long) \
			&& SIZEOF_TYPE(char*) == sizeof(char*))
	{
		printf("\n-- Test 4\n-- Pass :)\n");
		return ;
	}
	printf("-- Test 4\n-- Fail :(\n");
}

int main(int ac, char **av)
{
	int n = 0;

	if (2 == ac)
	{
		n = atoi(av[1]);
		switch (n)
		{
			case 1:
				TestEx1();
				break ;
			case 2:
				TestEx2();
				break ;
			case 3:
				TestEx3();
				break ;
			case 4:
				TestEx4();
				break ;
		}
	}
	else
	{
		TestEx1();
		TestEx2();
		TestEx3();
		TestEx4();
	}

	return 0;
}
