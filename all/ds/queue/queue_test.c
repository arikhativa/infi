#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "queue.h"
#include "slist.h"
#include "color.h"

static int t = 1;

struct queue
{
        slist_node_t *head;
        slist_node_t *tail;
};

typedef enum status
{
        FAIL = -1,
        SUCCESS = 0
} status_t;

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
	queue_t *que = 0;

	printf(CYAN"\n-- (1) Test Queue Create And Destroy (run with vlg):\n\n"RESET);

	que = QueueCreate();

	QueueDestroy(que);
}

static void TestQenqueue()
{
	queue_t *que = 0;
	char str[] = "123";
	char str1[] = "888";

	printf(CYAN"\n-- (2) Test Qenqueue() :\n\n"RESET);

	que = QueueCreate();

	QEnqueue(que, str);
	MemoryComper(str,(char*)que->head->next->data, 3);

	QEnqueue(que, str1);
	MemoryComper(str,(char*)que->head->next->data, 3);

	QueueDestroy(que);
}

static void TestQDequeue()
{
	queue_t *que = 0;
	char str[] = "123";
	char str1[] = "888";

	printf(CYAN"\n-- (3) Test QDequeue() :\n\n"RESET);

	que = QueueCreate();
	QEnqueue(que, str);
	QEnqueue(que, str1);

	QDequeue(que);
	MemoryComper(str1, que->tail->data, 3);

	QDequeue(que);
	MemoryComper(que->head, que->tail, 8);

	QueueDestroy(que);
}

static void TestIsEmpty()
{
	queue_t *que = 0;
	char str[] = "123";

	printf(CYAN"\n-- (4) Test IsEmpty() :\n\n"RESET);

	que = QueueCreate();
	
	SizeTComper(1, IsQueueEmpty(que));
	QEnqueue(que, str);
	QEnqueue(que, str);
	SizeTComper(0, IsQueueEmpty(que));

	QueueDestroy(que);
}

static void TestPeek()
{
	queue_t *que = 0;
	char str[] = "123";
	char str1[] = "123";

	printf(CYAN"\n-- (5) Test Peek() :\n\n"RESET);

	que = QueueCreate();
	
	QEnqueue(que, str);
	MemoryComper(str, QueuePeek(que), 3);
	QEnqueue(que, str1);
	MemoryComper(str, QueuePeek(que), 3);

	QueueDestroy(que);
}

static void TestSize()
{
	queue_t *que = 0;
	char str[] = "123";
	char str1[] = "123";

	printf(CYAN"\n-- (6) Test Size() :\n\n"RESET);

	que = QueueCreate();
	
	SizeTComper(0, QueueSize(que));
	QEnqueue(que, str);
	SizeTComper(1, QueueSize(que));
	QEnqueue(que, str1);
	SizeTComper(2, QueueSize(que));

	QueueDestroy(que);
}

static void TestAppend()
{
	int i = 0;
	queue_t *que1 = 0;
	queue_t *que2 = 0;
	queue_t *tmp = 0;
	char s1[] = "s1";
	char s2[] = "s2";
	char s3[] = "s3";
	char a1[] = "a1";
	char a2[] = "a2";
	char *ms[6];

	ms[0] = s1;
	ms[1] = s2;
	ms[2] = s3;
	ms[3] = a1;
	ms[4] = a2;
	ms[5] = NULL;

	printf(CYAN"\n-- (7) Test Que Append() :\n\n"RESET);

	que1 = QueueCreate();
	que2 = QueueCreate();
	
	tmp = QueueAppend(que1, que2);
	SizeTComper(0, (size_t)tmp);

	QEnqueue(que1, s1);
	QEnqueue(que1, s2);
	QEnqueue(que1, s3);

	tmp = QueueAppend(que1, que2);
	SizeTComper(0, (size_t)tmp);

	QEnqueue(que2, a1);
	QEnqueue(que2, a2);

	tmp = QueueAppend(que1, que2);
	MemoryComper(que1, tmp, 8);
	while (ms[i])
	{
		MemoryComper(ms[i], QueuePeek(tmp), 3);
		QDequeue(tmp);
		++i;
	}

	QueueDestroy(tmp);
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
			TestQenqueue();
			break ;
		case 3:
			TestQDequeue();
			break ;
		case 4:
			TestIsEmpty();
			break ;
		case 5:
			TestPeek();
			break ;
		case 6:
			TestSize();
			break ;
		case 7:
			TestAppend();
			break ;

		default:
			TestCreateAndDestroy();
			TestQenqueue();
			TestQDequeue();
			TestIsEmpty();
			TestPeek();
			TestSize();
			TestAppend();
	}

	return 0;
}
