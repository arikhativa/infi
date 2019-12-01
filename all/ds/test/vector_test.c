#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

#include "vector.h"
#include "color.h"

struct dynamic_vector
{
        void *start;
        size_t current_index;
        size_t capacity;
        size_t element_size;
};

static int t = 1;

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
	vector_t *vec = 0;

	printf(CYAN"\n-- (1) Test Vector Create And Destroy (run with vlg):\n\n"RESET);

	vec = VectorCreate(50, 3);

	VectorDestroy(vec);
}

static void TestReserve()
{
	vector_t *vec = 0;

	printf(CYAN"\n-- (2) Test Vector Reserve (run with vlg):\n\n"RESET);

	vec = VectorCreate(3, 3);
	VectorReserve(vec, 5);
	SizeTComper(5, vec->capacity);
	VectorReserve(vec, 1);
	SizeTComper(5, vec->capacity);

	VectorDestroy(vec);
}


static void TestPush(int print)
{
	vector_t *vec = 0;
	size_t size = 3;
	char elem1[] = "12";
	
	printf(CYAN"\n-- (2) Vec Push():\n\n"RESET);

	vec = VectorCreate(size, 1);

	if (print)
	{
		printf("-- element is:		%s\n", elem1);
		VectorPush(vec, elem1);
		printf("-- vec->elem is:	%s\n", ((char*)vec->start + ((vec->current_index - 1) * vec->element_size)));
	}
	else
		VectorPush(vec, elem1);
	MemoryComper(elem1, ((char*)vec->start + ((vec->current_index - 1)* vec->element_size)), size);
	
	VectorPush(vec, elem1);
	SizeTComper(4, vec->capacity);
	
	VectorDestroy(vec);
}

static void TestPop(int print)
{
	vector_t *vec = 0;
	int i = 90;
	size_t size = 3;
	char elem1[] = "12";
	char elem2[] = "55";

	printf(CYAN"\n-- (3) Vec Pop():\n\n"RESET);

	vec = VectorCreate(size, 120);

	for (; i; --i)
		VectorPop(vec);
	SizeTComper(60, vec->capacity);
	i = 200;
	VectorPush(vec, elem1);
	VectorPush(vec, elem2);
	if (print)
	{
		printf("-- element is:		%s\n", elem1);
		printf("-- vec->elem is:	%s\n", ((char*)vec->start + ((vec->current_index - 2)* vec->element_size)));
	}
	VectorPop(vec);
	MemoryComper(elem1, ((char*)vec->start + ((vec->current_index - 1)* vec->element_size)), size);
	
	for (; i; --i)
		VectorPop(vec);
	VectorDestroy(vec);
}

static void TestGet(int print)
{
	vector_t *vec = 0;
	size_t size = 3;
	char elem1[] = "12";
	char elem2[] = "55";

	printf(CYAN"\n-- (4) Vec Get():\n\n"RESET);

	vec = VectorCreate(size, 1);

	VectorPop(vec);
	VectorPush(vec, elem1);
	VectorPush(vec, elem2);

	if (print)
	{
		printf("-- element is:		%s\n", elem2);
		printf("-- vec->elem is:	%s\n", ((char*)vec->start + ((vec->current_index - 1)* vec->element_size)));
	}

	MemoryComper(elem2, (char*)VectorGetAddress(vec, 1), size);
	
	VectorDestroy(vec);
}

static void TestCapacity()
{
	vector_t *vec = 0;
	size_t size = 3;
	char elem1[] = "12";

	printf(CYAN"\n-- (5) Vec Capacity():\n\n"RESET);

	vec = VectorCreate(size, 1);

	SizeTComper(1, VectorCapacity(vec));
	VectorPush(vec, elem1);
	VectorPush(vec, elem1);
	VectorPush(vec, elem1);
	VectorPush(vec, elem1);
	VectorPush(vec, elem1);
	VectorPush(vec, elem1);
	SizeTComper(8, VectorCapacity(vec));
	
	VectorDestroy(vec);
}

static void TestSize(int print)
{
        vector_t *vec = 0;
        size_t size = 3;

        printf(CYAN"\n-- (6) Vec Get():\n\n"RESET);

        vec = VectorCreate(size, 55);


        if (print)
        {
                printf("-- vec->size :	%ld\n", (vec->current_index));
        }

	SizeTComper(55, VectorSize(vec));

        VectorDestroy(vec);
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
			TestReserve();
			break ;

		case 2:
			TestPush(print);
			break ;
		case 3:
			TestPop(print);
			break ;
		case 4:
			TestGet(print);
			break ;
		case 5:
			TestCapacity();
			break ;
		case 6:
			TestSize(print);
			break ;

		default:
			TestCreateAndDestroy();
			TestReserve();
			TestPush(print);
			TestPop(print);
			TestGet(print);
			TestCapacity();
			TestSize(print);
	}
	return 0;
}

