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

static int i = 0;

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

static int CheckI(void *param)
{

	if (!i)
	{
		++i;
		return REPEAT;
	}

	return DO_NOT_REPEAT;
}

static int PrintX(void *param)
{
	time_t timer;
	struct tm* tm_info;
	char buffer[26];
	char x = *(char*)param;

	time(&timer);
	tm_info = localtime(&timer);
	strftime(buffer, 26, "%H:%M:%S", tm_info);


	printf("-- %c", x);
	printf("-- %s", buffer);

	return REPEAT;
}

static void TestCreateAndDestroy()
{
	task_t *task = 0;
	char x = 'x';

	SetTest(1, "Create & Destroy (run with vlg)");

	task = TaskCreate(1, &PrintX, &x);

	TaskDestroy(task);
}

static void TestUpdateTime()
{
	task_t *task = 0;
	char x = 'x';
	time_t t = 0;

	SetTest(2, "() Update Time");

	task = TaskCreate(1, &PrintX, &x);

	TaskUpdateTimeToRun(task);

	t = time(NULL);
/*	printf("-- time now -		%s", ctime(&t));
	printf("-- time to run -	%s\n", ctime(&task->time_to_run));*/

	TaskDestroy(task);

}

static void TestRun()
{
	task_t *task = 0;
	run_status_t tmp = 0;
	char x = 'x';

	SetTest(3, "Run Task");

	task = TaskCreate(1, &CheckI, &x);

	tmp = TaskRun(task);
	ReturnComper(0, tmp);
	ReturnComper(1, i);

	tmp = TaskRun(task);
	ReturnComper(1, tmp);
	ReturnComper(1, i);

	TaskDestroy(task);
}

static void TestGetUid()
{
}

static void TestIsMatch()
{
	task_t *task = 0;
	run_status_t tmp = 0;
	unique_id_t uid;
	char x = 'x';

	SetTest(5, "IsMatch");

	task = TaskCreate(1, &CheckI, &x);

	uid = TaskGetUid(task);

	ReturnComper(1, TaskIsMatch(task, uid));
	ReturnComper(0, TaskIsMatch(task, bad_uid));

	TaskDestroy(task);

}
static void TestGetTimeRun()
{
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
			TestUpdateTime();
			break ;
		case 3:
			TestRun();
			break ;
		case 4:
			TestGetUid();
			break ;
		case 5:
			TestIsMatch();
			break ;
		case 6:
			TestGetTimeRun();
			break ;
		case 7:
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
			TestUpdateTime();
			TestRun();
			TestGetUid();
			TestIsMatch();
			TestGetTimeRun();
	}
	PrintSummery();

	return 0;
}
