#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "dlist.h"
#include "sortlist.h"
#include "color.h"


struct sort_list
{
    dlist_t *list;
    is_before_t IsBefore;
    void *param;
};

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

static int count_test = 1;
static int t_index = 1;
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

void PrintList(dlist_t *list)
{
	dlist_node_t *run = DlistBegin(list);

	while (run != DlistEnd(list))
	{
		printf("-- %d\n", *(int*)run->data);
		run = run->next;
	}
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

int IsBefore(const void *list_data, const void *user_data, void *param)
{
	(void)param;

	if (*(int*)list_data < *(int*)user_data)
		return 1;
	return 0;

}

static void TestCreateAndDestroy()
{
	sort_list_t *list = 0;

	SetTest(1, "Sorted list Create & Destroy (run with vlg)");

	list = SortListCreate(IsBefore, 0);

	SortListDestroy(list);
}

static void TestInsert()
{
	sort_list_t *list = 0;
	sortedl_iter_t iter;

	int arr[] = {2, 3, 5, 4, 1};
	int stud[] = {1, 2, 3, 4, 5};
	int i = 0;

	SetTest(2, "Sorted list Insert");

	list = SortListCreate(IsBefore, 0);

	while (i < 5)
	{
		iter = SortListInsert(list, &arr[i]);
		++i;
	}
	i = 0;

	ReturnComper((size_t)&arr[4], (size_t)DlistGetData(iter.iter));

	iter = SortListBegin(list);

/*	PrintList(list->list); */
	while (i < 5)
	{
		MemoryComper(&stud[i], SortListGetData(iter), sizeof(int));
		iter = SortListNext(iter);
		++i;
	}

	SortListDestroy(list);
}

static void TestBegin()
{
	sort_list_t *list = 0;
	sortedl_iter_t iter;

	SetTest(3, "(white) Sorted list Begin)");

	list = SortListCreate(IsBefore, 0);
	iter = SortListBegin(list);
	ReturnComper((size_t)&list->list->tail,(size_t)iter.iter);

	SortListDestroy(list);
}

static void TestEnd()
{
	sort_list_t *list = 0;
	sortedl_iter_t iter;

	SetTest(4, "(White) Sorted list End)");

	list = SortListCreate(IsBefore, 0);
	iter = SortListEnd(list);
	ReturnComper((size_t)&list->list->tail,(size_t)iter.iter);

	SortListDestroy(list);
}

static void TestIsSame()
{
	sortedl_iter_t iter1;
	sortedl_iter_t iter2;

	iter1.iter = 0;
	iter2.iter = (dlist_node_t*)1;

	SetTest(5, "Sorted list IsSame)");

	ReturnComper(0, SortListIsSameIter(iter1, iter2));
	iter2.iter = (dlist_node_t*)0;
	ReturnComper(1, SortListIsSameIter(iter1, iter2));
}

static void TestNext()
{
	sort_list_t *list = 0;

	sortedl_iter_t iter;
	sortedl_iter_t check;

	int a = 1;
	int b = 2;

	SetTest(6, "Sorted List Next)");

	list = SortListCreate(IsBefore, 0);

	iter = SortListBegin(list);
	iter = SortListNext(iter);

	ReturnComper(1, SortListIsSameIter(iter, SortListEnd(list)));

	check = SortListInsert(list, &a);
	check = SortListInsert(list, &b);
	iter = SortListBegin(list);
	iter = SortListNext(iter);

	ReturnComper(1, SortListIsSameIter(iter, check));

	SortListDestroy(list);
}

static void TestPrev()
{
	sort_list_t *list = 0;

	sortedl_iter_t iter;
	sortedl_iter_t check;

	int a = 1;
	int b = 2;

	SetTest(7, "Sorted List Prev)");

	list = SortListCreate(IsBefore, 0);

	iter = SortListEnd(list);
	iter = SortListPrev(iter);

	ReturnComper(1, SortListIsSameIter(iter, SortListEnd(list)));

	check = SortListInsert(list, &a);
	check = SortListInsert(list, &b);
	iter = SortListEnd(list);
	iter = SortListPrev(iter);

	ReturnComper(1, SortListIsSameIter(iter, check));

	SortListDestroy(list);
}

static void TestGet()
{
	sort_list_t *list = 0;
	sortedl_iter_t iter;

	int a = 99;

	SetTest(8, "Sorted List GetData)");

	list = SortListCreate(IsBefore, 0);

	iter = SortListInsert(list, &a);

	MemoryComper(&a, SortListGetData(iter), 4);

	SortListDestroy(list);
}

static void TestEmpty()
{
	sort_list_t *list = 0;
	int a = 99;

	SetTest(9, "Sort list IsEmpty)");

	list = SortListCreate(IsBefore, 0);

	ReturnComper(1, SortListIsEmpty(list));

	SortListInsert(list, &a);

	ReturnComper(0, SortListIsEmpty(list));

	SortListDestroy(list);
}

static void TestSize()
{
	sort_list_t *list = 0;
	int a = 99;
	int b = 989;
	int c = 9;
	int d = 99;

	SetTest(10, "Sort list Size");

	list = SortListCreate(IsBefore, 0);

	ReturnComper(0, SortListSize(list));

	SortListInsert(list, &a);
	ReturnComper(1, SortListSize(list));

	SortListInsert(list, &c);
	SortListInsert(list, &b);
	SortListInsert(list, &d);
	ReturnComper(4, SortListSize(list));

	SortListDestroy(list);
}

static void TestPopBack()
{
	sort_list_t *list = 0;
	sortedl_iter_t iter;
	int a = 1;
	int b = 2;
	void *tmp = 0;

	SetTest(11, "Sort list PopBack");

	list = SortListCreate(IsBefore, 0);

	SortListInsert(list, &a);
	SortListInsert(list, &b);

	tmp = SortListPopBack(list);
	iter = SortListBegin(list);

	MemoryComper(&b, tmp, 4);
	MemoryComper(&a, SortListGetData(iter), 4);

	ReturnComper(1, SortListSize(list));

	tmp = SortListPopBack(list);
	tmp = SortListPopBack(list);
	tmp = SortListPopBack(list);
	tmp = SortListPopBack(list);

	SortListDestroy(list);
}

static void TestPopFront()
{
	sort_list_t *list = 0;
	sortedl_iter_t iter;
	int a = 1;
	int b = 2;
	void *tmp = 0;

	SetTest(12, "Sort list PopFront");

	list = SortListCreate(IsBefore, 0);

	SortListInsert(list, &a);
	SortListInsert(list, &b);

	tmp = SortListPopFront(list);
	iter = SortListBegin(list);

	MemoryComper(&a, tmp, 4);
	MemoryComper(&b, SortListGetData(iter), 4);

	tmp = SortListPopFront(list);
	tmp = SortListPopFront(list);
	tmp = SortListPopFront(list);
	tmp = SortListPopFront(list);

	SortListDestroy(list);
}

static void TestRemove()
{
	sort_list_t *list = 0;
	sortedl_iter_t iter1;
	sortedl_iter_t iter2;
	int s1 = 1;
	int s2 = 2;

	SetTest(13, "Sort list Remove");

	list = SortListCreate(IsBefore, 0);

	iter1 = SortListEnd(list);
	iter2 = SortListRemove(SortListEnd(list));
	ReturnComper((size_t)iter1.iter, (size_t)iter2.iter);

	iter2 = SortListRemove(SortListBegin(list));
	ReturnComper((size_t)iter1.iter, (size_t)iter2.iter);

	iter1 = SortListInsert(list, &s1);
	iter2 = SortListInsert(list, &s2);

	iter1 = SortListRemove(iter1);

	ReturnComper((size_t)iter1.iter, (size_t)iter2.iter);
	ReturnComper(1, SortListSize(list));
	ReturnComper(1, SortListIsSameIter(iter2, SortListBegin(list)));

	SortListDestroy(list);
}

static int ACT(void *data, void *param)
{
	if ((*(int*)data) == 2)
	{
		*((int*)data) = *((int*)param);
		return 0;
	}

	return 1;
}

static void TestForEach()
{
	sort_list_t *list = 0;
	size_t tmp = 0;
	sortedl_iter_t iter1;
	sortedl_iter_t iter3;
	sortedl_iter_t iter4;
	sortedl_iter_t iter6;

	int a = 2;
	int b = 2;
	int c = 2;
	int d = 3;
	int e = 4;
	int f = 5;

	int ch = 0;

	SetTest(14, "Sort list ForEach");

	list = SortListCreate(IsBefore, 0);

	iter1 = SortListInsert(list, &a);
	SortListInsert(list, &b);
	iter3 = SortListInsert(list, &c);
	iter4 = SortListInsert(list, &d);
	SortListInsert(list, &e);
	iter6 = SortListInsert(list, &f);

	tmp = SortListForEach(iter1, iter4, ACT, &ch);
	ReturnComper(0, (size_t)tmp);

	tmp = SortListForEach(iter4, iter6, ACT, &ch);
	ReturnComper(1, (size_t)tmp);

	a = 2;
	b = 2;
	c = 2;

	tmp = SortListForEach(iter3, iter4, ACT, &ch);
	ReturnComper(0, (size_t)tmp);

	tmp = SortListForEach(iter3, iter4, ACT, &ch);
	ReturnComper(1, (size_t)tmp);

	SortListDestroy(list);
}
static int CMP(const void *data, void *param)
{
	return !(*(int*)data == *(int*)param);
}

static void TestFindIf()
{
        sort_list_t *list = 0;
	sortedl_iter_t tmp;
        sortedl_iter_t iter1;
        sortedl_iter_t iter2;
        sortedl_iter_t iter3;
        sortedl_iter_t iter6;

        int a = 1;
        int b = 2;
        int c = 3;
        int d = 4;
        int e = 5;
        int f = 6;

        int ch = 2;

	SetTest(15, "Sort list FindIf");

        list = SortListCreate(IsBefore, 0);


        iter1 = SortListInsert(list, &a);
        iter2 = SortListInsert(list, &b);
        iter3 = SortListInsert(list, &c);
        SortListInsert(list, &d);
        SortListInsert(list, &e);
        iter6 = SortListInsert(list, &f);

	tmp = SortListFindIf(iter1, iter3, CMP, &ch);
        ReturnComper((size_t)iter2.iter, (size_t)tmp.iter);

	tmp = SortListFindIf(iter1, iter2, CMP, &ch);
        ReturnComper((size_t)iter2.iter, (size_t)tmp.iter);

	tmp = SortListFindIf(iter2, iter6, CMP, &ch);
        ReturnComper((size_t)iter2.iter, (size_t)tmp.iter);

	tmp = SortListFindIf(iter3, iter6, CMP, &ch);
        ReturnComper((size_t)iter6.iter, (size_t)tmp.iter);

        SortListDestroy(list);
}

static void TestFind()
{
        sort_list_t *list = 0;
	sortedl_iter_t tmp;
        sortedl_iter_t iter1;
        sortedl_iter_t iter2;
        sortedl_iter_t iter3;
        sortedl_iter_t iter6;

        int a = 1;
        int b = 2;
        int c = 3;
        int d = 4;
        int e = 5;
        int f = 6;

        int ch = 2;

	SetTest(16, "Sort list Find");

        list = SortListCreate(IsBefore, 0);

        iter1 = SortListInsert(list, &a);
        iter2 = SortListInsert(list, &b);
        iter3 = SortListInsert(list, &c);
        SortListInsert(list, &d);
        SortListInsert(list, &e);
        iter6 = SortListInsert(list, &f);

	tmp = SortListFind(list, iter1, iter3, &ch);
	ReturnComper((size_t)iter2.iter, (size_t)tmp.iter);

	tmp = SortListFind(list, iter1, iter2, &ch);
        ReturnComper((size_t)iter2.iter, (size_t)tmp.iter);

	tmp = SortListFind(list, iter2, iter6, &ch);
	ReturnComper((size_t)iter2.iter, (size_t)tmp.iter);

	tmp = SortListFind(list, iter3, iter6, &ch);
	ReturnComper((size_t)iter6.iter, (size_t)tmp.iter);

        SortListDestroy(list);
}

static void TestMerge()
{
        sort_list_t *list1 = 0;
        sort_list_t *list2 = 0;
        sortedl_iter_t iter;

	int a[] = {0, 1, 5, 10};
	int b[] = {2, 3, 4, 6, 7, 8, 9, 11};

	int arr0[] = {0, 2, 4, 6, 8, 10};
	int arr1[] = {1, 3, 5, 7, 9, 11, 12};


	int i = 0;

	SetTest(17, "Sort list Merge");

        list1 = SortListCreate(IsBefore, 0);
        list2 = SortListCreate(IsBefore, 0);

	for (i = 0; i < 4; ++i)
	{
        	SortListInsert(list1, &a[i]);
	}
	for (i = 0; i < 8; ++i)
	{
        	SortListInsert(list2, &b[i]);
	}
/*
	PrintList(list1->list);
	PrintList(list2->list);*/

	list1 = SortListMerge(list1, list2);
	iter = SortListBegin(list1);
	for (i = 0 ; i < 12; ++i)
	{
		MemoryComper(&i,SortListGetData(iter), 4);
		iter = SortListNext(iter);
	}
	

	SortListDestroy(list1);
	SortListDestroy(list2);

        list1 = SortListCreate(IsBefore, 0);
        list2 = SortListCreate(IsBefore, 0);

	for (i = 0; i < 6; ++i)
	{
        	SortListInsert(list1, &arr0[i]);
	}
	for (i = 0; i < 7; ++i)
	{
        	SortListInsert(list2, &arr1[i]);
	}

	SortListMerge(list2, list1);
	iter = SortListBegin(list2);
	for (i = 0 ; i < 12; ++i)
	{
		MemoryComper(&i,SortListGetData(iter), 4);
		iter = SortListNext(iter);
	}
	SortListDestroy(list2);
	SortListDestroy(list1);	
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
			TestInsert();
			break ;
		case 3:
			TestBegin();
			break ;
		case 4:
			TestEnd();
			break ;
		case 5:
			TestIsSame();
			break ;
		case 6:
			TestNext();
			break ;
		case 7:
			TestPrev();
			break ;
		case 8:
			TestGet();
			break ;
		case 9:
			TestEmpty();
			break ;
		case 10:
			TestSize();
			break ;
		case 11:
			TestPopBack();
			break ;
		case 12:
			TestPopFront();
			break ;
		case 13:
			TestRemove();
			break ;
		case 14:
			TestForEach();
			break ;

		case 15:
			TestFindIf();
			break ;
		case 16:
			TestFind();
			break ;
		case 17:
			TestMerge();
			break ;

		default:
			TestCreateAndDestroy();
			TestInsert();
			TestBegin();
			TestEnd();
			TestIsSame();
			TestNext();
			TestPrev();
			TestGet();
			TestEmpty();
			TestSize();
			TestPopBack();
			TestPopFront();
			TestRemove();
			TestForEach();
			TestFindIf();
			TestFind();
			TestMerge();
	}
	PrintSummery();

	return 0;
}
