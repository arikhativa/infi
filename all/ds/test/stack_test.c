#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

#include "stack.h"
#include "color.h"

struct stack
{
        void *end;
        void *current;
        size_t element_size;
        void *start;
};



static int t = 1;

void PrintBit(size_t number, int digits)
{
	size_t one_bit = 1;
	--digits;
	while (digits + 1)
	{
		(number & (one_bit << (digits))) ? printf("1") : printf("0");
		--digits;
		if ((digits + 1) != 0 && (digits+1) % 8 == 0)
		{
			printf("-");
		}
	}
	printf("\n");
}

void PrintPass()
{
	printf("-- Test"GRAY" %d"RESET GREEN" - Pass :)\n"RESET, t++);
}

void PrintFail(char *str)
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

static void TestCreateAndDestroy()
{
	stack_t *stack = 0;

	printf(CYAN"\n-- (1) Test Create And Destroy (run with vlg):\n\n"RESET);

	stack = CreateStack(50, 3);
	DestroyStack(stack);
}

static void TestPush(int print)
{
	stack_t *ds = 0;
	size_t size = 3;
	char elem1[] = "12";
	char elem2[] = "af";
	
	printf(CYAN"\n-- (2) StackPush():\n\n"RESET);

	ds = CreateStack(2, size);

	if (print)
	{
		printf("-- element is:		%s\n", elem1);
		StackPush(ds, elem1);
		printf("-- stack->elem is:	%s\n", (char*)ds->current);
	}
	else
		StackPush(ds, elem1);
	MemoryComper(elem1, ds->current, size);
	if (print)
	{
		printf("-- element is:		%s\n", elem2);
		StackPush(ds, elem2);
		printf("-- stack->elem is:	%s\n", (char*)ds->current);
	}
	else
		StackPush(ds, elem2);
	MemoryComper(elem2, ds->current, size);
	StackPush(ds, elem2);
	MemoryComper(elem2, ds->current, size);

	DestroyStack(ds);
}

static void TestPop(int print)
{
	stack_t *ds = 0;
	size_t size = 3;
	char elem1[] = "12";
	char elem2[] = "af";
	
	printf(CYAN"\n-- (3) StackPop():\n\n"RESET);

	ds = CreateStack(50, size);

	if (print)
	{
		printf("-- element is:		%s\n", elem1);
		StackPush(ds, elem1);
		printf("-- stack->elem is:	%s\n", (char*)ds->current);
	}
	else
		StackPush(ds, elem1);
	if (print)
	{
		printf("-- element is:		%s\n", elem2);
		StackPush(ds, elem2);
		printf("-- stack->elem is:	%s\n", (char*)ds->current);
	}
	else
		StackPush(ds, elem2);
	if (print)
	{
		StackPop(ds);
		printf("-- stack->elem (poped) is:	%s\n", (char*)ds->current);
	}
	else
		StackPop(ds);
	MemoryComper(elem1, ds->current, size);

	DestroyStack(ds);
}

static void TestPeek(int print)
{
	stack_t *ds = 0;
	size_t size = 3;
	char elem1[] = "12";
	char elem2[] = "af";
	
	printf(CYAN"\n-- (4) StackPeek():\n\n"RESET);

	ds = CreateStack(50, size);

	if (print)
	{
		printf("-- element is:		%s\n", elem1);
		StackPush(ds, elem1);
		printf("-- stack->elem is:	%s\n", (char*)ds->current);
	}
	else
		StackPush(ds, elem1);
	MemoryComper(elem1, StackPeek(ds), size);
	if (print)
	{
		printf("-- element is:		%s\n", elem2);
		StackPush(ds, elem2);
		printf("-- stack->elem is:	%s\n", (char*)ds->current);
	}
	else
		StackPush(ds, elem2);
	MemoryComper(elem2, StackPeek(ds), size);

	DestroyStack(ds);
}

static void TestIsEmpty(int print)
{
	stack_t *ds = 0;
	size_t size = 3;
	char elem1[] = "12";
	char elem2[] = "af";
	
	printf(CYAN"\n-- (5) IsStackEmpty():\n\n"RESET);

	ds = CreateStack(50, size);

	if (print)
	{
		StackPush(ds, elem1);
		printf("-- stack->elem is:	%s\n", (char*)ds->current);
	}
	else
		StackPush(ds, elem1);
	if (print)
	{
		StackPush(ds, elem2);
		printf("-- stack->elem is:	%s\n", (char*)ds->current);
	}
	else
		StackPush(ds, elem2);
	if (print)
	{
		StackPop(ds);
		printf("-- stack->elem (poped) is:	%s\n", (char*)ds->current);
	}
	else
		StackPop(ds);
	if (IsStackEmpty(ds))
		PrintFail("-- It is not empty..");
	else
		PrintPass();
	if (print)
	{
		StackPop(ds);
		printf("-- stack->elem (poped) is:	%s\n", (char*)ds->current);
	}
	else
		StackPop(ds);
	if (!(IsStackEmpty(ds)))
		PrintFail("-- It is empty..");
	else
		PrintPass();

	DestroyStack(ds);
}

static void TestSize(int print)
{
	stack_t *ds = 0;
	size_t size = 3;
	int i = 0;
	char elem1[] = "12";
	char elem2[] = "af";
	
	printf(CYAN"\n-- (6) IsStackSize():\n\n"RESET);

	ds = CreateStack(50, size);

	if (print)
	{
		StackPush(ds, elem1);
		printf("-- size is: %d\n", 1);
	}
	else
		StackPush(ds, elem1);
	if (print)
	{
		StackPush(ds, elem2);
		printf("-- size is: %d\n", 2);
	}
	else
		StackPush(ds, elem2);

	if (2 != StackSize(ds))
		PrintFail("-- It not the right size..");
	else
		PrintPass();

	if (print)
	{
		StackPop(ds);
		printf("-- size is: %d\n", 1);
	}
	else
		StackPop(ds);
	
	if (1 != StackSize(ds))
		PrintFail("-- It not the right size..");
	else
		PrintPass();

	if (print)
	{
		StackPop(ds);
		printf("-- size is: %d\n", 0);
	}
	else
		StackPop(ds);

	if (0 != StackSize(ds))
		PrintFail("-- It not the right size..");
	else
		PrintPass();

	for (; 10 > i; ++i)
	{
		StackPush(ds, elem1);
	}
	if (10 != StackSize(ds))
		PrintFail("-- It not the right size..");
	else
		PrintPass();

	DestroyStack(ds);
}
int main(int ac, char **av)
{
	int c = 0;
	int print = 0;

	if (2 <= ac)
	{
		c = atoi(av[1]);
		if (!strcmp(av[1], "-p"))
		{
			print = 1;
			if (3 == ac)
				c = atoi(av[2]);
		}
	}
	switch (c)
	{
		case 1:
			TestCreateAndDestroy();
			break ;
		case 2:
			TestPush(print);
			break ;
		case 3:
			TestPop(print);
			break ;
		case 4:
			TestPeek(print);
			break ;
		case 5:
			TestIsEmpty(print);
			break ;
		case 6:
			TestSize(print);
			break ;
		case 7:
			break ;
		case 8:
			break ;
		case 9:
			break ;
		case 10:
			break ;

		default:
			TestCreateAndDestroy();
			TestPush(print);
			TestPop(print);
			TestPeek(print);
			TestIsEmpty(print);
			TestSize(print);
			break ;
	}
	return 0;
}

