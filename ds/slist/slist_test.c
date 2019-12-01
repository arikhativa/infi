#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "slist.h"
#include "color.h"

static int t = 1;

slist_node_t *SlistDisconnectLoop(slist_node_t *node);


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

static int Cmp(const void *data, void *param)
{
        return (*(char*)data == *(char*)param);
}

static int Act(void *data, void *param)
{
	if ('2' != *(char*)data)
	{
		return -1;
	}
	*(char*)data = *(char*)param;

	return 0;
}

static void TestCreateAndDestroy()
{
	slist_node_t *node = 0;
	char str[] = "123";

	printf(CYAN"\n-- (1) Test Slist Create And Destroy (run with vlg):\n\n"RESET);

	node = SlistCreateNode(str, NULL);
	node = SlistCreateNode(str, node);

	SlistFreeAll(node);
}

static void TestSize()
{
	slist_node_t *a1 = 0;
	slist_node_t *a2 = 0;
	slist_node_t *a3 = 0;
	slist_node_t *a4 = 0;
	slist_node_t *a5 = 0;
	slist_node_t *a6 = 0;
	char str[] = "123";

	printf(CYAN"\n-- (2) Slist Size():\n\n"RESET);

	a1 = SlistCreateNode(str, NULL);
	a2 = SlistCreateNode(str, NULL);
	a3 = SlistCreateNode(str, NULL);
	a4 = SlistCreateNode(str, NULL);
	a5 = SlistCreateNode(str, NULL);
	a6 = SlistCreateNode(str, NULL);

	a1->next = a2;
	a2->next = a3;
	a3->next = a4;
	a4->next = a5;
	a5->next = a6;
	a6->next = NULL;

	SizeTComper(6, SlistCount(a1));

	SlistFreeAll(a1);
}

static void TestInsertAfter()
{
	slist_node_t *node1 = 0;
	slist_node_t *node2 = 0;
	slist_node_t *node3 = 0;
	slist_node_t *tmp = 0;
	char str1[] = "123";
	char str2[] = "aa";
	char str3[] = "yyy";

	printf(CYAN"\n-- (3) Slist Insert After():\n\n"RESET);

	node2 = SlistCreateNode(str2, NULL);
	node1 = SlistCreateNode(str1, node2);
	node3 = SlistCreateNode(str3, NULL);

	tmp = SlistInsertAfter(node1, node3);
	SizeTComper(3, SlistCount(node1));
	MemoryComper(str3, tmp->data, 3);
	tmp = node1;
	tmp = tmp->next;
	MemoryComper(str3, tmp->data, 3);

	SlistFreeAll(node1);
}

static void TestInsertBefore()
{
	slist_node_t *node1 = 0;
	slist_node_t *node2 = 0;
	slist_node_t *node3 = 0;
	slist_node_t *node4 = 0;
	slist_node_t *tmp = 0;
	char str1[] = "123";
	char str2[] = "aa";
	char str3[] = "yes";
	char str4[] = "num123";

	printf(CYAN"\n-- (4) Slist Insert Before():\n\n"RESET);

	node4 = SlistCreateNode(str4, NULL);
	node3 = SlistCreateNode(str3, NULL);
	node2 = SlistCreateNode(str2, node3);
	node1 = SlistCreateNode(str1, node2);

	tmp = SlistInsertBefore(node2, node4);
	SizeTComper(4, SlistCount(node1));
	MemoryComper(str4, tmp->data, 6);
	tmp = node1;
	tmp = tmp->next;
	MemoryComper(str4, tmp->data, 6);

	SlistFreeAll(node1);
}

static void TestRemoveNode()
{
	slist_node_t *node1 = 0;
	slist_node_t *node2 = 0;
	slist_node_t *node3 = 0;
	slist_node_t *tmp = 0;
	char str1[] = "123";
	char str2[] = "aa";
	char str3[] = "yes";

	printf(CYAN"\n-- (5) Slis Remmove Node():\n\n"RESET);

	node3 = SlistCreateNode(str3, NULL);
	node2 = SlistCreateNode(str2, node3);
	node1 = SlistCreateNode(str1, node2);

	tmp = SlistRemoveNode(node2);
	SizeTComper(2, SlistCount(node1));
	MemoryComper(str2, tmp->data, 2);
	tmp = node1;
	tmp = tmp->next;
	MemoryComper(str3, tmp->data, 3);

	SlistFreeAll(node1);
	SlistFreeAll(node3);
}

static void TestRemoveAfter()
{
	slist_node_t *node1 = 0;
	slist_node_t *node2 = 0;
	slist_node_t *node3 = 0;
	slist_node_t *tmp = 0;
	char str1[] = "123";
	char str2[] = "aa";
	char str3[] = "yes";

	printf(CYAN"\n-- (6) Slis Remmove After():\n\n"RESET);

	node3 = SlistCreateNode(str3, NULL);
	node2 = SlistCreateNode(str2, node3);
	node1 = SlistCreateNode(str1, node2);

	tmp = SlistRemoveAfter(node1);
	SizeTComper(2, SlistCount(node1));
	MemoryComper(str2, tmp->data, 2);
	tmp = node1;
	tmp = tmp->next;
	MemoryComper(str3, tmp->data, 3);

	node2->next = NULL;
	SlistFreeAll(node1);
	SlistFreeAll(node2);
}

static void TestLoop()
{
	slist_node_t *node1 = 0;
	slist_node_t *node2 = 0;
	slist_node_t *node3 = 0;
	slist_node_t *node4 = 0;
	char str1[] = "123";
	char str2[] = "aa";
	char str3[] = "yes";
	char str4[] = "4444";

	printf(CYAN"\n-- (7) Slis Loop():\n\n"RESET);

	node1 = SlistCreateNode(str1, NULL);
	node2 = SlistCreateNode(str2, NULL);
	node3 = SlistCreateNode(str3, NULL);
	node4 = SlistCreateNode(str4, NULL);

	node1->next = node1;
	SizeTComper(1, SlistHasLoop(node1));

	node1->next = node2;
	node2->next = node1;
	SizeTComper(1, SlistHasLoop(node1));

	node1->next = node2;
	node2->next = node3;
	node3->next = node4;
	node4->next = node3;

	SizeTComper(1, SlistHasLoop(node1));

	node4->next = NULL;

	SlistFreeAll(node1);

}

static void TestIntersection()
{
	slist_node_t *a1 = 0;
	slist_node_t *a2 = 0;
	slist_node_t *b1 = 0;
	slist_node_t *b2 = 0;
	slist_node_t *b3 = 0;
	slist_node_t *b4 = 0;
	char str1[] = "123";

	printf(CYAN"\n-- (8) Slis Intersection():\n\n"RESET);

	a1 = SlistCreateNode(str1, NULL);
	a2 = SlistCreateNode(str1, NULL);
	b1 = SlistCreateNode(str1, NULL);
	b2 = SlistCreateNode(str1, NULL);
	b3 = SlistCreateNode(str1, NULL);
	b4 = SlistCreateNode(str1, NULL);

	a1->next = a2;
	a2->next = b3;

	b1->next = b2;
	b2->next = b3;
	b3->next = b4;

	SizeTComper((size_t)b3, (size_t)SlistFindIntersection(a1, b1));

	a2->next = NULL;
	SlistFreeAll(b1);
	SlistFreeAll(a1);
}

static void TestForEach()
{
	slist_node_t *node1 = 0;
	slist_node_t *node2 = 0;
	slist_node_t *node3 = 0;
	int tmp = 0;
	char str1[] = "2";
	char str2[] = "2";
	char str3[] = "2es";

	printf(CYAN"\n-- (9) Slis ForEach():\n\n"RESET);

	node1 = SlistCreateNode(str1, NULL);
	node2 = SlistCreateNode(str2, NULL);
	node3 = SlistCreateNode(str3, NULL);

	node1->next = node2;
	node2->next = node3;

	tmp = SlistForEach(node1, Act, "a");
	SizeTComper(0, (size_t)tmp);

	tmp = SlistForEach(node1, Act, "a");
	SizeTComper(-1, (size_t)tmp);

	SlistFreeAll(node1);
}

static void TestFind()
{
	slist_node_t *node1 = 0;
	slist_node_t *node2 = 0;
	slist_node_t *node3 = 0;
	slist_node_t *tmp = 0;
	char str1[] = "a23";
	char str2[] = "1a";
	char str3[] = "aes";

	printf(CYAN"\n-- (10) Slis Find():\n\n"RESET);

	node1 = SlistCreateNode(str1, NULL);
	node2 = SlistCreateNode(str2, NULL);
	node3 = SlistCreateNode(str3, NULL);

	node1->next = node2;
	node2->next = node3;

	tmp = SlistFind(node1, Cmp, "a");
	SizeTComper((size_t)node1, (size_t)tmp);

	SlistFreeAll(node1);

	node1 = SlistCreateNode(str1, NULL);
	node2 = SlistCreateNode(str1, NULL);
	node3 = SlistCreateNode(str3, NULL);

	node1->next = node2;
	node2->next = node3;

	tmp = SlistFind(node1, Cmp, "6");
	SizeTComper(0, (size_t)tmp);

	SlistFreeAll(node1);
}

static void TestFlip()
{
	slist_node_t *node1 = 0;
	slist_node_t *node2 = 0;
	slist_node_t *node3 = 0;
	slist_node_t *tmp = 0;
	int i = 0;
	char str1[] = "1";
	char str2[] = "2";
	char str3[] = "3";
	char *ms[4];

	ms[0] = str1;
	ms[1] = str2;
	ms[2] = str3;
	ms[3] = NULL;

	printf(CYAN"\n-- (11) Slis flip():\n\n"RESET);

	node1 = SlistCreateNode(str1, NULL);
	node2 = SlistCreateNode(str2, NULL);
	node3 = SlistCreateNode(str3, NULL);

	node1->next = node2;
	node2->next = node3;

	tmp = SlistFlip(node1);
	for (i = 2; i >= 0; --i)
	{
		MemoryComper(ms[i], tmp->data, 1);
		tmp = tmp->next;
	}

	SlistFreeAll(node3);
}

static void TestDisLoop()
{
	slist_node_t *node1 = 0;
	slist_node_t *node2 = 0;
	slist_node_t *node3 = 0;
	slist_node_t *node4 = 0;
	slist_node_t *node5 = 0;
	slist_node_t *tmp = 0;
	char str1[] = "1";
	char str2[] = "2";
	char str3[] = "3";

	printf(CYAN"\n-- (12) Slis Disconnect Loop():\n\n"RESET);

	node1 = SlistCreateNode(str1, NULL);
	node2 = SlistCreateNode(str2, NULL);
	node3 = SlistCreateNode(str3, NULL);
	node4 = SlistCreateNode(str3, NULL);
	node5 = SlistCreateNode(str3, NULL);

	node1->next = node2;
	node2->next = node3;
	node3->next = node4;
	node4->next = node5;
	node5->next = node1;

	tmp = SlistDisconnectLoop(node1);
	SizeTComper(0, SlistHasLoop(node1));
	SlistFreeAll(tmp);

}

int main(int ac, char **av)
{
	int c = 0;

	if (2 == ac)
	{
		c = atoi(av[1]);
	}
	switch (c)
	{
		case 1:
			TestCreateAndDestroy();
			break ;
		case 2:
			TestSize();
			break ;
		case 3:
			TestInsertAfter();
			break ;
		case 4:
			TestInsertBefore();
			break ;
		case 5:
			TestRemoveNode();
			break ;
		case 6:
			TestRemoveAfter();
			break ;
		case 7:
			TestLoop();
			break ;
		case 8:
			TestIntersection();
			break ;
		case 9:
			TestForEach();
			break ;
		case 10:
			TestFind();
			break ;
		case 11:
			TestFlip();
			break ;
		case 12:
			TestDisLoop();
			break ;


		default:
			TestCreateAndDestroy();
			TestSize();
			TestInsertAfter();
			TestInsertBefore();
			TestRemoveNode();
			TestRemoveAfter();
			TestLoop();
			TestIntersection();
			TestForEach();
			TestFind();
			TestFlip();
			TestDisLoop();
	}
	return 0;
}

