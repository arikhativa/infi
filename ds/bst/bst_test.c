#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "color.h"
#include "bst.h"

static int count_test = 1;
static int t_index = 1;
static int pass = 0;

static int boly = 1;
static int num = 1;
static char *name;

/*
	printf("-- %d parent - %d\n", *(int*)iter[2]->data, *(int*)iter[2]->parent->data);
	printf("-- %d left_child - %d\n", *(int*)iter[2]->data, *(int*)iter[2]->left_child->data);
	printf("-- %d right_child - %d\n", *(int*)iter[2]->data, *(int*)iter[2]->right_child->data);

	printf("\n\n\n\n");
	printf("-- %d parent - %d\n", *(int*)iter[6]->data, *(int*)iter[6]->parent->data);
	printf("-- %d left_child - %d\n", *(int*)iter[6]->data, *(int*)iter[6]->left_child->data);
	printf("-- %d right_child - %d\n", *(int*)iter[6]->data, *(int*)iter[6]->right_child->data);

	printf("\n\n\n\n");
	printf("-- %d parent - %d\n", *(int*)iter[0]->data, *(int*)iter[0]->parent->data);
	printf("-- %d left_child - %d\n", *(int*)iter[0]->data, *(int*)iter[0]->left_child->data);
	printf("-- %d right_child - %d\n", *(int*)iter[0]->data, *(int*)iter[0]->right_child->data);

	printf("\n\n\n\n");
	printf("-- %d parent - %d\n", *(int*)iter[0]->parent->data, *(int*)iter[0]->parent->data);
	printf("-- %d left_child - %d\n", *(int*)iter[0]->parent->data, *(int*)iter[0]->parent->left_child->data);
	printf("-- %d right_child - %d\n", *(int*)iter[0]->parent->data, *(int*)iter[0]->parent->right_child->data);
*/




typedef struct bst_node bst_node_t;

struct bst_node
{
	bst_node_t *left_child;
	bst_node_t *right_child;
	bst_node_t *parent;
	void *data;
};

struct bst
{
	bst_node_t end;
	policy_func_t BstCmp;
	void *param;
};




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

void PrintTree(bst_t *bst, bst_iter_t *iter, int space)
{
	int i = 5;

	if (iter == &bst->end)
		return ;
	space += 5;

	PrintTree(bst, iter->right_child ,space);

	printf("\n");
	for (; i < space; ++i)
		printf(" ");

	printf("%d\n", *(int*)iter->data);

	PrintTree(bst, iter->left_child, space);
}

static int CMP(const void *tree, const void *data, void *param)
{
	(void)param;

	return (*(int*)data - *(int*)tree);
}

static int ACT(void *tree, void *param)
{
	(void)param;

	if ((*(int*)tree != 1))
	{	
		++*(int*)tree;
		return (0);
	}

	return (1);
}

static void TestCreateAndDestroy()
{
	bst_t *bst = 0;

	SetTest(1, "Create & Destroy (run with vlg)");

	bst = BSTCreate(CMP, 0);

	BSTDestroy(bst);

}

static void TestInsert()
{
	int size = 7;
	int arr[] = {4, 2, 6, 1,  3, 5, 7};
	int i = 0;
	bst_t *bst = 0;
	bst_iter_t *iter[7];

	SetTest(2, "BST Insert");

	bst = BSTCreate(CMP, 0);

	ReturnComper(1, BSTIsEmpty(bst));

	for (i = 0; i < size; ++i)
		iter[i] = BSTInsert(bst, &arr[i]);

	ReturnComper(0, BSTIsEmpty(bst));

	for (i = 0; i < size; ++i)
		MemoryComper(&arr[i], BSTGetData(iter[i]), sizeof(int));

	/*PrintTree(bst, bst->end.right_child, 0);*/

	BSTDestroy(bst);
}

static void TestNext()
{
	int size = 7;
	int p = 888;
	int arr[] = {4, 2, 6, 1,  3, 5, 7};
	int correct[] = {1, 2, 3, 4, 5, 6, 7};
	int i = 0;
	bst_t *bst = 0;
	bst_iter_t *begin = 0;

	SetTest(3, "BST Next");

	bst = BSTCreate(CMP, &p);

	for (;i < size; ++i)
	{
		BSTInsert(bst, &arr[i]);
	}

	begin = BSTBegin(bst);

/*	PrintTree(bst, bst->end.right_child, 0);*/
	for (i = 0;i < size; ++i)
	{
		MemoryComper(begin->data, &correct[i], sizeof(int));
		begin = BSTNext(begin);
	}

	BSTDestroy(bst);
}

static void TestBegin()
{
	int size = 7;
	int arr[] = {4, 2, 6, 1,  3, 5, 7};
	int correct[] = {4, 2, 2, 1,  1, 1, 1};
	int i = 0;
	bst_t *bst = 0;
	bst_node_t *begin = 0;

	SetTest(4, "BST Begin");

	bst = BSTCreate(CMP, 0);

	for (;i < size; ++i)
	{
		BSTInsert(bst, &arr[i]);
		begin = BSTBegin(bst);
		MemoryComper(begin->data, &correct[i], sizeof(int));
	}

	BSTDestroy(bst);
}

static void TestRemove()
{
	int size = 8;
	int arr[] = {7, 6, 2, 5, 1, 10, 8, 9};
	int i = 0;

	bst_t *bst = 0;
	bst_iter_t *iter[8];

	SetTest(5, "BST Remove");

	bst = BSTCreate(CMP, 0);

	for (i = 0 ;i < size; ++i)
		iter[i] = BSTInsert(bst, &arr[i]);

	BSTRemove(iter[0]);
	MemoryComper(iter[0]->data, &arr[6], sizeof(int));

	BSTRemove(iter[2]);
	MemoryComper(iter[2]->data, &arr[3], sizeof(int));

	BSTDestroy(bst);


	bst = BSTCreate(CMP, 0);

	for (i = 0 ;i < size; ++i)
		iter[i] = BSTInsert(bst, &arr[i]);

	BSTRemove(iter[5]);
 	MemoryComper(iter[7]->data, &arr[7], sizeof(int));

	BSTRemove(iter[1]);


 	MemoryComper(iter[2]->data, &arr[2], sizeof(int));

	BSTDestroy(bst);
/*	PrintTree(bst, bst->end.right_child, 0);*/
}

static void TestPrev()
{
	int size = 7;
	int p = 888;
	int arr[] = {4, 2, 6, 1,  3, 5, 7};
	int correct[] = {7, 6, 5, 4, 3, 2, 1};
	int i = 0;
	bst_t *bst = 0;
	bst_iter_t *begin = 0;

	SetTest(6, "BST Prev");

	bst = BSTCreate(CMP, &p);

	for (;i < size; ++i)
		BSTInsert(bst, &arr[i]);

	begin = BSTBegin(bst);
	for (i = 1; i < size; ++i)
		begin = BSTNext(begin);

/*	PrintTree(bst, bst->end.right_child, 0);*/
	for (i = 0; i < size; ++i)
	{
		MemoryComper(begin->data, &correct[i], sizeof(int));
		begin = BSTPrev(begin);
	}

	BSTDestroy(bst);
}

static void TestSize()
{
	int size = 7;
	int p = 888;
	int arr[] = {4, 2, 6, 1,  3, 5, 7};
	size_t correct[] = {1, 2, 3, 4, 5, 6, 7};
	int i = 0;
	bst_t *bst = 0;
	bst_iter_t *iter[7];

	SetTest(7, "BST Size");

	bst = BSTCreate(CMP, &p);

	ReturnComper(0, BSTSize(bst));
	for (;i < size; ++i)
	{
		iter[i] = BSTInsert(bst, &arr[i]);
		ReturnComper(correct[i], BSTSize(bst));
	}
	BSTRemove(iter[0]);
	ReturnComper(6, BSTSize(bst));

	BSTRemove(iter[1]);
	BSTRemove(iter[2]);

	ReturnComper(4, BSTSize(bst));

	BSTDestroy(bst);
/*	PrintTree(bst, bst->end.right_child, 0);*/
}

static void TestFind()
{
	int size = 7;
	int arr[] = {4, 2, 6, 1,  3, 5, 7};
	int i = 0;
	bst_t *bst = 0;
	bst_iter_t *iter[7];
	bst_iter_t *tmp;

	SetTest(8, "BST Find");

	bst = BSTCreate(CMP, 0);

	for (;i < size; ++i)
		iter[i] = BSTInsert(bst, &arr[i]);

	tmp = BSTFind(bst, &arr[3]);
	ReturnComper(1, BSTIsSameIter(iter[3], tmp));

	BSTRemove(iter[3]);
	tmp = BSTFind(bst, &arr[3]);
	ReturnComper(0, BSTIsSameIter(iter[3], tmp));

/*	PrintTree(bst, bst->end.right_child, 0);*/

	BSTDestroy(bst);
}

static void TestForEach()
{
	int size = 7;
	int arr[] = {4, 2, 6, 1,  3, 5, 7};
	int i = 0;
	bst_t *bst = 0;
	bst_iter_t *iter[7];
	int tmp;

	SetTest(9, "BST ForEach");

	bst = BSTCreate(CMP, 0);

	for (;i < size; ++i)
		iter[i] = BSTInsert(bst, &arr[i]);

	tmp = BSTForEach(BSTBegin(bst), BSTEnd(bst), ACT, 0);
	ReturnComper(1, tmp);
	tmp = BSTForEach(BSTBegin(bst), iter[3], ACT, 0);
	ReturnComper(0, tmp);

	BSTRemove(iter[3]);
	tmp = BSTForEach(BSTBegin(bst), BSTEnd(bst), ACT, 0);
	ReturnComper(0, tmp);

/*	PrintTree(bst, bst->end.right_child, 0);*/

	BSTDestroy(bst);
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
			TestNext();
			break ;
		case 4:
			TestBegin();
			break ;
		case 5:
			TestRemove();
			break ;
		case 6:
			TestPrev();
			break ;
		case 7:
			TestSize();
			break ;
		case 8:
			TestFind();
			break ;
		case 9:
			TestForEach();
			break ;
		default:
			TestCreateAndDestroy();
			TestInsert();
			TestNext();
			TestBegin();
			TestRemove();
			TestPrev();
			TestSize();
			TestFind();
			TestForEach();
	}
	PrintSummery();

	return 0;
}
