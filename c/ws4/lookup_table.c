
#include <stdio.h> /* printf() */
#include <stdlib.h> /* system() */
#include <assert.h> /* assert() */

#define ESCAPE 27

typedef void (*table_t)(int);


static void DoNothing(int i)
{
	i += i;
}

static void PrintKey(int pressed)
{
	if (pressed != 0)
	{
		printf("%c pressed\n", pressed);
	}
}

static void SetArrayToNothing(table_t *lookup_table)
{
	int size = 0;

	assert(lookup_table);

	while (256 > size)
	{
		lookup_table[size] = DoNothing;
		++size;
	}
}

static void SetArrayToFunction(table_t *lookup_table, void (*function_pointer)(int))
{
	assert(lookup_table);
	assert(function_pointer);

	lookup_table['A'] = *function_pointer;
	lookup_table['T'] = *function_pointer;
}

static void RunTime(void (*lookup_table[])(int))
{
	unsigned char pressed = 0;

	assert(lookup_table);

	while (ESCAPE != pressed)
	{
		pressed = (unsigned char)getchar();
		lookup_table[pressed](pressed);
	}
}

int main()
{
	table_t lookup_table[256];

	SetArrayToNothing(lookup_table);
	SetArrayToFunction(lookup_table, &PrintKey);
	if (system("stty -icanon -echo") == -1)
	{
		return -1;
	}
	RunTime(lookup_table);
	if (system("stty icanon echo") == -1)
	{
		return -1;
	}

	return 0;
}
