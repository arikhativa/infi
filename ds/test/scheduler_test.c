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
#include "task.h"
#include "scheduler.h"
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

static int h = 0;
static int i = 0;
static int g = 0;
static int m = 0;


/*
static void PrintUid(unique_id_t uid)
{
        printf("-- PID  == %u\n", uid.pid);
        printf("-- NUM  == %lu\n", uid.counter);
        printf("-- TIME == %s\n", ctime(&uid.time));
}
*/
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

static void ReturnComper(size_t num1, size_t num2)
{
	if (num1 == num2)
		Pass();
	else
		PrintFail(" -- Diffrence in return value.");
}

static int CheckH(void *param)
{
	(void)param;
	if (h < 10)
	{
		++h;
		return REPEAT;
	}

	return DO_NOT_REPEAT;
}

static int CheckI(void *param)
{
	(void)param;
	if (i < 5)
	{
		++i;
		return REPEAT;
	}

	return DO_NOT_REPEAT;
}

static int CheckG(void *param)
{
	(void)param;
	if (g < 2)
	{
		++g;
		return REPEAT;
	}

	return DO_NOT_REPEAT;
}

static int TimesM(void *param)
{
	static int oo = 0;

	(void)param;

	m = m * 10;
	if (oo < 2)
	{
		++oo;
		return REPEAT;
	}

	return DO_NOT_REPEAT;
}

static int PlusM(void *param)
{
	static int oo = 0;

	(void)param;

	++m;
	if (oo < 2)
	{
		++oo;
		return REPEAT;
	}

	return DO_NOT_REPEAT;
}

/*static int PrintX(void *param)
{
	time_t timer;
	struct tm* tm_info;
	char buffer[26];
	static int ppp = 0;
	char x = *(char*)param;

	time(&timer);
	tm_info = localtime(&timer);
	strftime(buffer, 26, "%M:%S", tm_info);
	
	if (ppp == 5)
		return DO_NOT_REPEAT;
	++ppp;


	printf("-- %s\n", buffer);
	printf("-- %c\n", x);

	return REPEAT;
}

static int PrintY(void *param)
{
	static char y = 0;

	if (y == 2)
		return DO_NOT_REPEAT;

	y = *(char*)param;
	printf("-- %c\n", y);
	y = 2;

	return REPEAT;
}

static int PrintZ(void *param)
{
	char z = *(char*)param;

	printf("-- %c\n", z);

	return REPEAT;
}
*/
static int SStop(void *param)
{
	op_sch_t *tmp = 0;

	tmp = param;

	OperSchedStop(tmp);

	return REPEAT;
}

static void TestCreateAndDestroy()
{
	op_sch_t *sch = 0;

	SetTest(1, "Create & Destroy (run with vlg)");

	sch = OperSchedCreate();

	OperSchedDestroy(sch);
}

static void TestAdd()
{
	op_sch_t *sch = 0;

	SetTest(2, "Schler Add");

	sch = OperSchedCreate();

	ReturnComper(1, OperSchedIsEmpty(sch));
	OperSchedAdd(sch, 1, &CheckI, 0);
	ReturnComper(0, OperSchedIsEmpty(sch));
	OperSchedAdd(sch, 1, &CheckI, 0);
	ReturnComper(2, OperSchedSize(sch));

	OperSchedDestroy(sch);
}

static void TestRemove()
{
	op_sch_t *sch = 0;
	int a = 0;
	unique_id_t u1;
	unique_id_t u2;

	SetTest(3, "Schler Remove");

	sch = OperSchedCreate();

	u1 = OperSchedAdd(sch, 1, &CheckI, 0);
	u2 = OperSchedAdd(sch, 1, &CheckH, 0);

	a = OperSchedRemove(sch, u1);
	ReturnComper(1, OperSchedSize(sch));
	ReturnComper(0, (size_t)a);

	a = OperSchedRemove(sch, u1);
	ReturnComper(1, (size_t)a);
	ReturnComper(1, OperSchedSize(sch));

	a = OperSchedRemove(sch, u2);
	ReturnComper(0, (size_t)a);
	ReturnComper(0, OperSchedSize(sch));


	OperSchedDestroy(sch);
}

/*
	u1 = OperSchedAdd(sch, 1, &PrintX, &x);
	u2 = OperSchedAdd(sch, 2, &PrintY, &y);
	u3 = OperSchedAdd(sch, 8, &SStop, sch);
	sleep(5);
*/

static void TestRun()
{
	op_sch_t *sch = 0;
	char x = 'x';
	char y = 'y';
	char z = 'z';
	unique_id_t u3;
	status_t ret = 0;

	SetTest(4, "Sched Run");
	printf("-- This will take about 10 sec.. \n");

	sch = OperSchedCreate();

	OperSchedAdd(sch, 2, &SStop, sch);
	OperSchedAdd(sch, 1, &PlusM, &x);
	OperSchedAdd(sch, 2, &TimesM, &x);
	ret = OperSchedRun(sch);
	ReturnComper(20, m);
	ReturnComper(STOPPED, ret);
	OperSchedClear(sch);

	OperSchedAdd(sch, 1, &CheckH, &x);
	OperSchedAdd(sch, 2, &CheckI, &y);
	OperSchedAdd(sch, 5, &CheckG, &z);
	u3 = OperSchedAdd(sch, 1, &SStop, sch);

	OperSchedRemove(sch, u3);

	ret = OperSchedRun(sch);
	ReturnComper(IS_EMPTY, ret);
	OperSchedRun(sch);

	ReturnComper(10, h);
	ReturnComper(5, i);
	ReturnComper(2, g);
	ReturnComper(0, OperSchedSize(sch));

	OperSchedAdd(sch, 1, &CheckH, 0);
	OperSchedAdd(sch, 2, &SStop, sch);
	u3 = OperSchedAdd(sch, 5, &CheckG, 0);

	h = 0;
	g = 0;
	OperSchedRun(sch);
	ReturnComper(2, h);
	ReturnComper(0, g);
	ReturnComper(3, OperSchedSize(sch));

	OperSchedDestroy(sch);
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
			TestAdd();
			break ;
		case 3:
			TestRemove();
			break ;
		case 4:
			TestRun();
			break ;
		case 5:
			break ;
		case 6:
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
			TestAdd();
			TestRemove();
			TestRun();
	}
	PrintSummery();

	return 0;
}
