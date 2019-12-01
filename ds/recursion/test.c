#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "recursion.h"
#include "slist.h"
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

static void MemoryComper(void *mem1, void *mem2, size_t n)
{
	if (0 == memcmp(mem1, mem2, n))
		Pass();
	else
		PrintFail(" -- Diffrence in memory.");
}

static void TestFibIter()
{
	size_t i = 0;
	size_t size = 8;
	size_t fib[] = {0, 1, 1, 2, 3, 5, 8, 13};
	size_t i_30 = 832040;

	SetTest(1, "Iter Fib");

	for (i = 0; i < size; ++i)
	{
		/*		printf("-- ret	%lu\n", IterFibonacci(i));*/
		ReturnComper(fib[i], IterFibonacci(i));
	}

	ReturnComper(i_30, IterFibonacci(30));
}

static void TestFibRec()
{
	size_t i = 0;
	size_t size = 8;
	size_t fib[] = {0, 1, 1, 2, 3, 5, 8, 13};
	size_t i_30 = 832040;

	SetTest(2, "rec Fib");

	for (i = 0; i < size; ++i)
	{
		/*		printf("-- ret	%lu\n", RecursFibonacci(i));*/
		ReturnComper(fib[i], RecursFibonacci(i));
	}

	ReturnComper(i_30, RecursFibonacci(30));
}

static void TestStrlen()
{
	char s1[] = "1";
	char s2[] = "";
	char s3[] = "1234";
	char s4[] = "0123456789";
	char *ss[4];
	size_t c[] = {1, 0, 4, 10};
	int i = 0;
	int size = 4;

	SetTest(3, "strlen");

	ss[0] = s1;
	ss[1] = s2;
	ss[2] = s3;
	ss[3] = s4;

	for (i = 0; i < size; ++i)
	{
		/*		printf("-- str:%s\n", ss[i]);
				printf("-- ret	%lu\n", Strlen(ss[i]));*/
		ReturnComper(c[i], Strlen(ss[i]));
	}
}

static void TestStrcmp()
{
	char s1[] = "1";
	char s2[] = "1";
	char s3[] = "1234";
	char s4[] = "123456789";
	char s5[] = "";
	char s6[] = "a";

	char *ss[10];
	int i = 0;
	int size = 6;

	SetTest(4, "strcmp");

	ss[0] = s1;
	ss[1] = s2;
	ss[2] = s3;
	ss[3] = s4;
	ss[4] = s5;
	ss[5] = s6;

	for (i = 0; i < size; i += 2)
	{
		/*		printf("-- s1:%s\n", ss[i]);
				printf("-- s2:%s\n", ss[i + 1]);
				printf("-- ret	%d\n", Strcmp(ss[i], ss[i + 1]));*/
		ReturnComper(strcmp(ss[i], ss[i + 1]), Strcmp(ss[i], ss[i+ 1]));
	}
}

static void TestStrcpy()
{
	char s[] = "123456";
	char d[100];
	int size = 7;

	SetTest(5, "strcpy");

	Strcpy(d, s);

	/*	printf("-- s:%s\n", s);
		printf("-- d:%s\n", d);*/
	MemoryComper(d,s, size);
}

static void TestStrcat()
{
	char s[] = "123456";
	char c[] = "abcdefghi123456";
	char d[100];
	char i = 0;
	int size = 16;

	for (i = 'a'; i < 'j'; ++i)
		d[i - 'a'] = i;
	d[i - 'a'] = '\0';

	SetTest(6, "strcat");

	Strcat(d, s);

	/*	printf("-- s:%s\n", s);
		printf("-- d:%s\n", d);*/
	MemoryComper(d,c, size);
}

static void TestStrstr()
{
	char n[] = "123";
	char h[] = "abcdefghi123456";
	char *ret;

	SetTest(7, "strstr");

	ret = Strstr(h, n);

	/*	printf("-- h:%s\n", h);
		printf("-- n:%s\n", n);
		printf("-- ret:%s\n", ret);*/

	ReturnComper((size_t)ret, (size_t)(h+9));
}


void PrintSlist(slist_node_t *node)
{
	while (node)
	{
		printf("%d ", *(int*)node->data);
		node = node->next;

	}

	printf("\n");
}

static void TestFlip()
{
	slist_node_t *n1;
	slist_node_t *n2;
	slist_node_t *n3;
	slist_node_t *tmp;
	int i;
	int arr[] = {1, 2, 3};
	int c[] = {3, 2, 1};

	SetTest(8, "Flip");

	n3 = SlistCreateNode(&arr[2], NULL);
	n2 = SlistCreateNode(&arr[1], n3);
	n1 = SlistCreateNode(&arr[0], n2);

	tmp = RecFlip(n1);
	n1 = tmp;

	i = 0;
	while (tmp)
	{
		MemoryComper(tmp->data, &c[i], 4);
		tmp = tmp->next;
		++i;
	}

	SlistFreeAll(n1);
}

static void TestStackSort()
{
	int size = 5;
	int i;
	int arr[] = {5, 7, 3, 30, 1};
	int c[] = {30, 7, 5, 3, 1};
	stack_t *stack = CreateStack(20, 4);

	SetTest(9, "Sort Stack");

	for (i = 0; i < size; ++i)
		StackPush(stack, &arr[i]);

	RecStackSort(stack);
	for (i = 0; i < size; ++i)
	{
		ReturnComper(c[i], *(int*)StackPeek(stack));
		StackPop(stack);
	}

	DestroyStack(stack);

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
			TestFibIter();
			break ;
		case 2:
			TestFibRec();
			break ;
		case 3:
			TestStrlen();
			break ;
		case 4:
			TestStrcmp();
			break ;
		case 5:
			TestStrcpy();
			break ;
		case 6:
			TestStrcat();
			break ;
		case 7:
			TestStrstr();
			break ;
		case 8:
			TestFlip();
			break ;
		case 9:
			TestStackSort();
			break ;
		case 10:
			break ;

		default:
			TestFibIter();
			TestFibRec();
			TestStrlen();
			TestStrcmp();
			TestStrcpy();
			TestStrcat();
			TestStrstr();
			TestFlip();
			TestStackSort();
	}
	PrintSummery();

	return 0;
}
