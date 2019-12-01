#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "dlist.h"
#include "sortlist.h"
#include "pque.h"
#include "color.h"

typedef struct id
{
	int num;
	int vip;
}	idnum_t;

struct pque
{
        sort_list_t *list;
        is_before_t IsBefore;
        void *param;
};

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

static int IsBefore(const void *user, const void *list, void *param)
{
	idnum_t *u_id = (idnum_t*)user;
	idnum_t *l_id = (idnum_t*)list;
	(void)param;

	if (u_id->vip < l_id->vip)
		return 1;
	return 0;
}

static void TestCreateAndDestroy()
{
	pque_t *que = 0;

	SetTest(1, "Create & Destroy (run with vlg)");

	que = PqueCreate(IsBefore, 0);

	PqueDestroy(que);
}

static void TestEmpty()
{
	pque_t *que = 0;

	SetTest(2, "IsEmpty()");

	que = PqueCreate(IsBefore, 0);

	ReturnComper(1,PqueIsEmpty(que));

	PqueDestroy(que);
}

static void TestEnque()
{
	pque_t *que = 0;
	idnum_t id1 = {308, 3};
	idnum_t id2 = {307, 3};
	idnum_t id3 = {108, 1};
	idnum_t id4 = {7, 0};

	SetTest(3, "Enque()");

	que = PqueCreate(IsBefore, 0);

	PEnqueue(que, &id1);
	PEnqueue(que, &id2);
	PEnqueue(que, &id3);
	PEnqueue(que, &id4);

/*	PrintList(que->list->list);*/
	ReturnComper(0,PqueIsEmpty(que));

	PqueDestroy(que);
}

static void TestSize()
{
	pque_t *que = 0;
	idnum_t id1 = {308, 3};
	idnum_t id2 = {307, 3};
	idnum_t id3 = {108, 1};
	idnum_t id4 = {7, 0};

	SetTest(4, "Size()");

	que = PqueCreate(IsBefore, 0);

	PEnqueue(que, &id1);
	PEnqueue(que, &id2);
	PEnqueue(que, &id3);
	PEnqueue(que, &id4);

/*	PrintList(que->list->list);*/
	ReturnComper(4,PqueSize(que));

	PqueDestroy(que);
}

static void TestDeque()
{
	pque_t *que = 0;
	int i = 0;
	idnum_t id1 = {308, 3};
	idnum_t id2 = {307, 3};
	idnum_t id3 = {108, 1};
	idnum_t id4 = {7, 0};
	idnum_t arr[4];
	arr[0] = id4;
	arr[1] = id3;
	arr[2] = id2;
	arr[3] = id1;

	SetTest(5, "Deque()");

	que = PqueCreate(IsBefore, 0);

	PEnqueue(que, &id1);
	PEnqueue(que, &id2);
	PEnqueue(que, &id3);
	PEnqueue(que, &id4);

	for (i = 0; i < 4; ++i)
	{
		MemoryComper(PquePeek(que), &arr[i], 8);
		PDequeue(que);
	}

	PqueDestroy(que);
}

static void TestClear()
{
	pque_t *que = 0;
	idnum_t id1 = {308, 3};
	idnum_t id2 = {307, 3};
	idnum_t id3 = {108, 1};
	idnum_t id4 = {7, 0};

	SetTest(6, "Peek()");

	que = PqueCreate(IsBefore, 0);

	PEnqueue(que, &id1);
	PEnqueue(que, &id2);
	PEnqueue(que, &id3);
	PEnqueue(que, &id4);

	PqueClear(que);
	ReturnComper(0,PqueSize(que));

	PqueDestroy(que);
}

static int IsMatch(const void *data, void *param)
{
	idnum_t id1 = *(idnum_t*)data;

	if (id1.num == *(int*)param)
		return 0;
	return 1;
}

static void TestErase()
{
	pque_t *que = 0;
	int i = 0;
	int ppp = 308;
	idnum_t id1 = {308, 3};
	idnum_t id2 = {307, 0};
	idnum_t id3 = {108, 1};
	idnum_t id4 = {7, 10};
	idnum_t arr[3];
	arr[0] = id2;
	arr[1] = id3;
	arr[2] = id4;

	SetTest(7, "Erase()");

	que = PqueCreate(IsBefore, 0);

	PEnqueue(que, &id1);
	PEnqueue(que, &id2);
	PEnqueue(que, &id3);
	PEnqueue(que, &id4);

	PqueErase(que, IsMatch, &ppp);
/*	PrintList(que->list->list);*/
	for (i = 0; i < 3; ++i)
	{
		MemoryComper(PquePeek(que), &arr[i], 8);
		PDequeue(que);
	}

	PqueDestroy(que);
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
			TestEmpty();
			break ;
		case 3:
			TestEnque();
			break ;
		case 4:
			TestSize();
			break ;
		case 5:
			TestDeque();
			break ;
		case 6:
			TestClear();
			break ;
		case 7:
			TestErase();
			break ;
		case 8:
			break ;
		case 9:
			break ;
		case 10:
			break ;
		case 11:
			break ;
		case 12:
			break ;
		case 13:
			break ;
		case 14:
			break ;
		case 15:
			break ;
		case 16:
			break ;
		case 17:
			break ;

		default:
			TestCreateAndDestroy();
			TestEmpty();
			TestEnque();
			TestSize();
			TestDeque();
			TestClear();
			TestErase();
	}
	PrintSummery();

	return 0;
}
