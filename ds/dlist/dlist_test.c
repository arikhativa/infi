#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "dlist.h"
#include "color.h"




typedef struct dlist_node dlist_node_t;

struct dlist_node
{
	void *data;
	dlist_node_t *next;
	dlist_node_t *prev;
};

struct dlist
{
	dlist_node_t head;
	dlist_node_t tail;
};
	


static int t = 1;
static int pass = 0;

static int boly = 1;
static int num = 1;
static char *name;


typedef enum status
{
	FAIL = -1,
	SUCCESS = 0
} status_t;

static void SetTest(int n, char *str)
{
	num = n;
	name = str;
	boly = 1;
}

static void Pass()
{
	++t;
	++pass;
}

static void PrintFail(char *str)
{
	if (boly)
		printf(CYAN"\n-- (%d) Test %s:\n\n"RESET, num, name);
	printf("-- Test"GRAY" %d"RESET RED" - Fail :( "RESET, t++);
	printf("%s\n", str);
	boly = 0;
}

static void PrintSummery(void)
{
	--t;
	if (pass == t)
		printf(GREEN"\n-- Pass :) (%d/%d)\n\n"RESET,pass, t);
	else
		printf(YELLOW"\n-- Not good enough.. (%d/%d)\n\n"RESET,pass, t);
}

/*
static void PrintList(dlist_t *list)
{
	dlist_node_t *run = DlistBegin(list);

	while (run != DlistEnd(list))
	{
		printf("-- %s\n", (char*)run->data);
		run = run->next;
	}
}*/

static void MemoryComper(void *mem1, void *mem2, size_t n)
{
	if (0 == memcmp(mem1, mem2, n))
		Pass();
	else
		PrintFail(" -- Diffrence in memory.");
}

static void SizeTComper(size_t num1, size_t num2)
{
	if (num1 == num2)
		Pass();
	else
		PrintFail(" -- Diffrence in size_t.");
}

static void TestCreateAndDestroy()
{
	dlist_t *list = 0;

	SetTest(1, "Dlist Create & Destroy (run with vlg)");

	list = DlistCreate();

	DlistDestroy(list);
}

static void TestIsSame()
{
	dlist_iter_t iter1 = 0;
	dlist_iter_t iter2 = (dlist_node_t*)1;


	SetTest(2, "Dlist IsSame)");

	SizeTComper(0, DlistIsSameIter(iter1, iter2));
	iter2 = 0;
	SizeTComper(1, DlistIsSameIter(iter1, iter2));
}

static void TestEnd()
{
	dlist_t *list = 0;

	SetTest(3, "(White) Dlist End)");

	list = DlistCreate();

	MemoryComper(&list->tail, DlistEnd(list), sizeof(dlist_node_t));

	DlistDestroy(list);
}

static void TestBegin()
{
	dlist_t *list = 0;

	SetTest(4, "(White) Dlist Begin)");

	list = DlistCreate();
	MemoryComper(list->head.next, DlistBegin(list), sizeof(dlist_node_t));

	DlistDestroy(list);
}

static void TestNext()
{
	dlist_t *list = 0;
	dlist_iter_t iter = 0;
	dlist_iter_t check = 0;
	char str[] = "tzuzaman";
	char str1[] = "tzuzaman1";

	SetTest(5, "Dlist Next)");

	list = DlistCreate();

	iter = DlistBegin(list);
	iter = DlistNext(iter);

	SizeTComper(1, DlistIsSameIter(iter, DlistEnd(list)));

	check = DlistPushBack(list, str);
	check = DlistPushBack(list, str1);
	iter = DlistBegin(list);
	iter = DlistNext(iter);

	SizeTComper(1, DlistIsSameIter(iter, check));

	DlistDestroy(list);
}
static void TestPrev()
{
	dlist_t *list = 0;
	dlist_iter_t iter = 0;
	dlist_iter_t check = 0;
	char str[] = "tzuzaman";

	SetTest(6, "Dlist Prev)");

	list = DlistCreate();

	iter = DlistEnd(list);
	iter = DlistPrev(iter);
	iter = DlistPrev(iter);

	SizeTComper(1, DlistIsSameIter(iter, DlistEnd(list)));
	check = DlistPushBack(list, str);
	iter = DlistEnd(list);
	iter = DlistPrev(iter);

	SizeTComper(1, DlistIsSameIter(iter, check));

	DlistDestroy(list);
}

static void TestGet()
{
	dlist_t *list = 0;
	dlist_iter_t iter = 0;
	char str[] = "tzuzaman";

	SetTest(7, "Dlist GetData)");

	list = DlistCreate();
	iter = DlistPushBack(list, str);
	MemoryComper(str, DlistGetData(iter), 9);

	DlistDestroy(list);
}

static void TestPushBack()
{
	dlist_t *list = 0;
	dlist_iter_t iter = 0;
	char str[] = "tzuzaman";

	SetTest(8, "Dlist PushBack)");

	list = DlistCreate();

	iter = DlistPushBack(list, str);
	MemoryComper(str, DlistGetData(iter), 9);

	DlistDestroy(list);
}

static void TestPushFront()
{
	dlist_t *list = 0;
	dlist_iter_t iter = 0;
	char str[] = "tzuzaman";

	SetTest(9, "Dlist PushFront)");

	list = DlistCreate();

	iter = DlistPushFront(list, str);
	MemoryComper(str, DlistGetData(iter), 9);

	DlistDestroy(list);
}

static void TestEmpty()
{
	dlist_t *list = 0;
	char str[] = "tzuzaman";

	SetTest(10, "Dlist IsEmpty)");

	list = DlistCreate();
	SizeTComper(1, DlistIsEmpty(list));

	DlistPushFront(list, str);
	SizeTComper(0, DlistIsEmpty(list));


	DlistDestroy(list);
}

static void TestSize()
{
	dlist_t *list = 0;
	char str[] = "tzuzaman";

	SetTest(11, "Dlist Size");

	list = DlistCreate();
	SizeTComper(0, DlistSize(list));

	DlistPushFront(list, str);
	SizeTComper(1, DlistSize(list));
	DlistPushBack(list, str);
	DlistPushBack(list, str);
	DlistPushBack(list, str);
	SizeTComper(4, DlistSize(list));

	DlistDestroy(list);
}

static void TestInsert()
{
	dlist_t *list = 0;
	dlist_iter_t iter = 0;
	int i = 0;
	char s1[] = "1";
	char s2[] = "2";
	char s3[] = "3";
	char s4[] = "4";
	char s5[] = "5";
	char *ms[6];
	ms[0] = s1;
	ms[1] = s2;
	ms[2] = s3;
	ms[3] = s4;
	ms[4] = s5;
	ms[5] = NULL;

	SetTest(12, "Dlist Insert");

	list = DlistCreate();


	DlistPushFront(list, s5);
	iter = DlistPushFront(list, s4);
	DlistPushFront(list, s2);
	DlistPushFront(list, s1);

	DlistInsert(list, s3, iter);
	iter = DlistBegin(list);

	while (ms[i])
	{
		MemoryComper(ms[i], DlistGetData(iter), 2);
	/*	printf("%s\n", (char*)DlistGetData(iter));*/
		iter = DlistNext(iter);
		++i;
	}

	DlistDestroy(list);
}

static void TestPopBack()
{
	dlist_t *list = 0;
	char s1[] = "1";
	char s2[] = "2";

	SetTest(13, "Dlist PopBack");

	list = DlistCreate();

	SizeTComper(0, (size_t)DlistPopBack(list));
	DlistPushFront(list, s2);
	DlistPushFront(list, s1);

	MemoryComper(s2, DlistPopBack(list), 2);
	MemoryComper(s1, DlistPopBack(list), 2);

	DlistDestroy(list);
}

static void TestPopFront()
{
	dlist_t *list = 0;
	char s1[] = "1";
	char s2[] = "2";

	SetTest(14, "Dlist PopFront");

	list = DlistCreate();

	SizeTComper(0, (size_t)DlistPopFront(list));
	DlistPushFront(list, s2);
	DlistPushFront(list, s1);

	MemoryComper(s1, DlistPopFront(list), 2);
	MemoryComper(s2, DlistPopFront(list), 2);

	DlistDestroy(list);
}

static void TestRemove()
{
	dlist_t *list = 0;
	dlist_iter_t iter1 = 0;
	dlist_iter_t iter2 = 0;
	char s1[] = "1";
	char s2[] = "2";

	SetTest(15, "Dlist Remove");

	list = DlistCreate();

	SizeTComper((size_t)DlistEnd(list), (size_t)DlistRemove(DlistEnd(list)));
	SizeTComper((size_t)DlistEnd(list), (size_t)DlistRemove(DlistBegin(list)));
	iter2 = DlistPushFront(list, s2);
	iter1 = DlistPushFront(list, s1);

	iter1 = DlistRemove(iter1);

	SizeTComper((size_t)iter1, (size_t)iter2);
	SizeTComper(1, DlistSize(list));

	DlistDestroy(list);
}

static int CMP(const void *data, void *param)
{
	return !(*(char*)data == *(char*)param);
}

static void TestFind()
{
	dlist_t *list = 0;
	dlist_iter_t tmp = 0;
	dlist_iter_t iter1 = 0;
	dlist_iter_t iter2 = 0;
	dlist_iter_t iter3 = 0;
	dlist_iter_t iter6 = 0;
	char s1[] = "1";
	char s2[] = "a";
	char s3[] = "2";
	char s4[] = "2";
	char s5[] = "5";
	char s6[] = "2";
	char ch = 'a';

	SetTest(16, "Dlist Find");

	list = DlistCreate();

	iter6 = DlistPushFront(list, s6);
	DlistPushFront(list, s5);
	DlistPushFront(list, s4);
	iter3 = DlistPushFront(list, s3);
	iter2 = DlistPushFront(list, s2);
	iter1 = DlistPushFront(list, s1);

	tmp = DlistFind(iter1, iter3, CMP, &ch);
	SizeTComper((size_t)tmp, (size_t)iter2);

	tmp = DlistFind(iter1, iter2, CMP, &ch);
	SizeTComper((size_t)tmp, (size_t)iter2);

	tmp = DlistFind(iter2, iter6, CMP, &ch);
	SizeTComper((size_t)tmp, (size_t)iter2);

	tmp = DlistFind(iter3, iter6, CMP, &ch);
	SizeTComper((size_t)tmp, (size_t)iter6);

	DlistDestroy(list);
}

static int ACT(void *data, void *param)
{
	if ((*(char*)data) == '2')
	{
		*((char*)data) = *((char*)param);
		return 0;
	}

	return 1;
}

static void TestForEach()
{
	dlist_t *list = 0;
	size_t tmp = 0;
	dlist_iter_t iter1 = 0;
	dlist_iter_t iter3 = 0;
	dlist_iter_t iter4 = 0;
	dlist_iter_t iter6 = 0;
	char s1[] = "2";
	char s2[] = "2";
	char s3[] = "2";
	char s4[] = "3";
	char s5[] = "4";
	char s6[] = "5";
	char ch = 'a';

	SetTest(17, "Dlist ForEach");

	list = DlistCreate();

	iter6 = DlistPushFront(list, s6);
	DlistPushFront(list, s5);
	iter4 = DlistPushFront(list, s4);
	iter3 = DlistPushFront(list, s3);
	DlistPushFront(list, s2);
	iter1 = DlistPushFront(list, s1);

	tmp = DlistForEach(iter1, iter4, ACT, &ch);
	SizeTComper(0, (size_t)tmp);

	tmp = DlistForEach(iter4, iter6, ACT, &ch);
	SizeTComper(1, (size_t)tmp);

	*s1 = '2';
	*s2 = '2';
	*s3 = '2';

	tmp = DlistForEach(iter3, iter4, ACT, &ch);
	SizeTComper(0, (size_t)tmp);

	tmp = DlistForEach(iter3, iter4, ACT, &ch);
	SizeTComper(1, (size_t)tmp);

	DlistDestroy(list);
}

static void TestSplice()
{
	dlist_t *list = 0;
	dlist_iter_t tmp = 0;
	dlist_iter_t iter1 = 0;
	dlist_iter_t iter2 = 0;
	dlist_iter_t iter4 = 0;
	dlist_iter_t iter6 = 0;

	int i = 0;
	char s1[] = "1";
	char s2[] = "4";
	char s3[] = "5";

	char a1[] = "2";
	char a2[] = "3";

	char bla[] = "6";
	char *ms[6];

	ms[0] = s1;
	ms[1] = a1;
	ms[2] = a2;
	ms[3] = s2;
	ms[4] = s3;
	ms[5] = NULL;

	SetTest(18, "Dlist Splice");

	list = DlistCreate();

	iter6 = DlistPushFront(list, bla);
	DlistPushFront(list, a2);
	iter4 = DlistPushFront(list, a1);
	DlistPushFront(list, s3);
	iter2 = DlistPushFront(list, s2);
	iter1 = DlistPushFront(list, s1);

	tmp = DlistSplice(iter2, iter4, iter6);

/*	PrintList(list);*/
	MemoryComper(a2, tmp->data, 2);
	tmp = iter1;

	while (ms[i])
	{
		MemoryComper(ms[i], DlistGetData(tmp), 2);
		tmp = DlistNext(tmp);
		++i;
	}

	DlistDestroy(list);
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
			TestIsSame();
			break ;
		case 3:
			TestEnd();
			break ;
		case 4:
			TestBegin();
			break ;
		case 5:
			TestNext();
			break ;
		case 6:
			TestPrev();
			break ;
		case 7:
			TestGet();
			break ;
		case 8:
			TestPushBack();
			break ;
		case 9:
			TestPushFront();
			break ;
		case 10:
			TestEmpty();
			break ;
		case 11:
			TestSize();
			break ;
		case 12:
			TestInsert();
			break ;
		case 13:
			TestPopBack();
			break ;
		case 14:
			TestPopFront();
			break ;
		case 15:
			TestRemove();
			break ;
		case 16:
			TestFind();
			break ;
		case 17:
			TestForEach();
			break ;
		case 18:
			TestSplice();
			break ;

		default:
			TestCreateAndDestroy();
			TestIsSame();
			TestEnd();
			TestBegin();
			TestNext();
			TestPrev();
			TestGet();
			TestPushBack();
			TestPushFront();
			TestEmpty();
			TestSize();
			TestInsert();
			TestPopBack();
			TestPopFront();
			TestRemove();
			TestFind();
			TestForEach();
			TestSplice();
	}
	PrintSummery();

	return 0;
}
