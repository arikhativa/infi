#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "color.h"
#include "avl.h"


/*
 * 	global 
 */

static char g_summery[400][400] = {0};

static int count_test = 1;
static int t_index = 1;
static int g_pass = 0;
static int g_fail = 0;

/*	var per test	*/

static char test_name[40];

static int add_test_name = 1;

typedef struct avl_node avl_node_t;

typedef enum diraction
{
        LEFT = 0,
        RIGHT
} diraction_t;

struct avl_node
{
        void *data;
        avl_node_t *child[2];
        unsigned short hight;
};

struct avl
{
        avl_node_t *root;
        cmp_func_t Cmp;
};

static void SetTest(int n, char *name)
{
	sprintf(test_name,"\n\n-- "CYAN"(%d) Test %s:\n\n"RESET, n, name);

	add_test_name = 1;

	t_index = 1;
}

static void Pass()
{
	++count_test;
	++g_pass;
	++t_index;
}

static void PrintFail(char *dis, int line, char *value, char *bad_value)
{
	if (add_test_name)
	{
		add_test_name = 0;
		sprintf(g_summery[g_fail],"%s", test_name);
		++g_fail;
	}

	sprintf(g_summery[g_fail],"-- "YELLOW"Discription:\n"RESET"-- %s\n\n",dis);
	++g_fail;
	sprintf(g_summery[g_fail],"-- "BBLUE"In Line "BGREEN "%d "RESET, line);
	++g_fail;
	sprintf(g_summery[g_fail],BBLUE"Value should be "RESET BGREEN"%s\n"RESET, value);
	++g_fail;
	sprintf(g_summery[g_fail],"-- "BBLUE"It is "RESET RED"%s\n\n"RESET, bad_value);
	++g_fail;


	++count_test;
	++t_index;
}

static void PrintSummery(void)
{
	int i = 0;

	--count_test;
	if (g_pass == count_test)
		printf(GREEN"\n-- Pass :) (%d/%d)\n\n"RESET,g_pass, count_test);
	else
	{
		printf(YELLOW"\n-- Not good enough.. (%d/%d)\n\n"RESET,g_pass, count_test);
		for (i = 0; i < g_fail; ++i)
		{
			printf("%s", g_summery[i]);
		}
	}
}

static void ReturnComper(size_t num1, size_t num2, char *dis, int line)
{
	char value[20] = {0};
	char bad_value[20] = {0};

	if (num1 == num2)
		Pass();
	else
	{
		sprintf(value, "%lu", num1);
		sprintf(bad_value, "%lu", num2);
		PrintFail(dis, line, value, bad_value);
	}
}
/*
static void MemoryComper(void *mem1, void *mem2, size_t n)
{
	if (0 == memcmp(mem1, mem2, n))
		Pass();
	else
	{
		PrintFail(dis, line, value, bad_value);

	}
}

void PrintTree(avl_node_t *root, int space)
{
	int i = 5;

	if (root == NULL)
		return ;
	space += 5;

	PrintTree(root->child[RIGHT] ,space);

	printf("\n");
	for (; i < space; ++i)
		printf(" ");

	printf("(%d)", root->hight);
	printf("%d\n", *(int*)root->data);

	PrintTree(root->child[LEFT], space);
}
*/
static int CMP(const void *data, const void *tree)
{
	return (*(int*)data - *(int*)tree);
}

static void TestCreateAndDestroy()
{
	avl_t *tree = 0;

	SetTest(1, "Create & Destroy (run with vlg)");

	tree = AVLCreate(CMP);

	AVLDestroy(tree);
}

static void Normal()
{
	char dis[] = "Adding a few nodes. no balacing needed. the test is checking the hight.";
	int size = 3;
	int i = 0;

	int arr[] = {40, 50, 30};
	int c[] = {1, 2, 2};
	avl_t *avl = 0;

	avl = AVLCreate(CMP);

	for (i = 0; i < size; ++i)
	{
		AVLInsert(avl, &arr[i]);
		ReturnComper(c[i], AVLHeight(avl), dis, __LINE__ + i);
	}

/*	PrintTree(avl->root, 0);*/

	AVLDestroy(avl);
}

static void MiniInsert(char *dis, int size, int *arr, int c)
{
	int i= 0;
	avl_t *avl = 0;

	avl = AVLCreate(CMP);

	for (i = 0; i < size; ++i)
	{
		AVLInsert(avl, &arr[i]);
	}

	ReturnComper(c, AVLHeight(avl), dis, __LINE__);
/*	PrintTree(avl->root, 0);*/

	AVLDestroy(avl);
}

static void TestInsert()
{
	char dis1[] = "Adding nodes so LEFT RIGHT balacing is needed. checking the hight.";
	int arr1[] = {40, 50, 30, 51, 45, 46, 44};

	char dis2[] = "Adding nodes so RIGHT LEFT balacing is needed. checking the hight.";
	int arr2[] = {40, 50, 30, 31, 25, 26};

	SetTest(2, "AVL Insert");

	Normal();
	MiniInsert(dis1, 7,arr1,3);
	MiniInsert(dis2, 6,arr2,3);
}

static void TestFind()
{
	char dis[] = "Using AvlFind and compering the return value";
	int size = 8;
	int i = 0;
	int arr[] = {40, 50, 30, 2, 33, 100, 45, 55};
	int c[] = {100, 2, 45};
	avl_t *avl = 0;
	avl_node_t *ret1;
	avl_node_t *ret2;
	avl_node_t *ret3;

	SetTest(3, "AVL Find");

	avl = AVLCreate(CMP);

	for (i = 0; i < size; ++i)
	{
		AVLInsert(avl, &arr[i]);
	}

/*	PrintTree(avl->root, 0);	*/

	ret1 = AVLFind(avl, &arr[5]);
	ret2 = AVLFind(avl, &arr[3]);
	ret3 = AVLFind(avl, &arr[6]);
	ReturnComper(c[0], *(int*)ret1, dis, __LINE__);
	ReturnComper(c[1], *(int*)ret2, dis, __LINE__);
	ReturnComper(c[2], *(int*)ret3, dis, __LINE__);

	AVLDestroy(avl);
}

static void MiniRemove(char *dis, int size, int *arr, int *data, int c)
{
	int i= 0;
	avl_t *avl = 0;

	avl = AVLCreate(CMP);

	for (i = 0; i < size; ++i)
	{
		AVLInsert(avl, &arr[i]);
	}

	AVLRemove(avl, data);
	ReturnComper(c, AVLHeight(avl), dis, __LINE__);

/*	PrintTree(avl->root, 0);
	printf("\n\n ********************** \n\n");*/

	AVLDestroy(avl);
}

static void MiniRemove7(char *dis, int size, int *arr, int *data, int *c)
{
	int i= 0;
	avl_t *avl = 0;

	avl = AVLCreate(CMP);

	for (i = 0; i < size; ++i)
	{
		AVLInsert(avl, &arr[i]);
	}

/*	PrintTree(avl->root, 0);
	printf("\n\n ********************** \n\n");*/

	for (i = 0; i < size; ++i)
	{
		AVLRemove(avl, &data[i]);
		ReturnComper(c[i], AVLHeight(avl), dis, __LINE__);
	}

	AVLDestroy(avl);
}

static void TestRemove()
{
	char dis1[] = "Remove a Leaf. balancing with right rotation. checking height";
	int arr1[] = {30, 40, 20, 15};
	int size1 = 4;
	int c1 = 2;

	char dis2[] = "Remove a Leaf. balancing with left and right rotation. checking height";
	int arr2[] = {30, 40, 20, 15, 25};
	int size2 = 5;
	int c2 = 3;

	char dis3[] = "Remove root with two children, checking the height";
	int arr3[] = {30, 40, 20, 25, 15};
	int size3 = 5;
	int c3 = 3;

	char dis4[] = "Remove node with left child, cuzing root to rebalance with right left rotation";
	int arr4[] = {30, 40, 20, 35, 45, 15, 55};
	int size4 = 7;
	int c4 = 3;

	char dis5[] = "remove root with two childern. NEXT(root) is a leaf.";
	int arr5[] = {50, 25, 70, 20, 28, 60, 80, 10, 22, 27, 29, 75, 90};
	int size5 = 13;
	int c5 = 4;

	char dis6[] = "remove root with two childern. NEXT(root) has right child.";
	int arr6[] = {50, 25, 70, 20, 28, 60, 80, 10, 22, 27, 29, 65, 75, 90};
	int size6 = 14;
	int c6 = 4;

	char dis7[] = "yoni.";
	int arr7[] = {2, 3, 1, 6, 8, 5, 0, 7, 9, 4};
	int rm7[] = {0, 6, 9, 2, 8, 5, 3, 1, 4, 7};
	int size7 = 10;
	int c7[] = {4, 4, 4, 3, 3, 3, 2, 2, 2, 0};

	int i = 0;
	avl_t *avl;

	SetTest(4, "AVL Remove");

	avl = AVLCreate(CMP);

	for (i = 0; i < size1; ++i)
	{
		AVLInsert(avl, &arr1[i]);
	}

	ReturnComper(1, AVLRemove(avl, &size1), "Checking return value with bad data", __LINE__);

	AVLDestroy(avl);

	MiniRemove(dis1, size1, arr1, &arr1[1], c1);
	MiniRemove(dis2, size2, arr2, &arr2[2], c2);
	MiniRemove(dis3, size3, arr3, &arr3[0], c3);
	MiniRemove(dis4, size4, arr4, &arr4[2], c4);
	MiniRemove(dis5, size5, arr5, &arr5[0], c5);
	MiniRemove(dis6, size6, arr6, &arr6[0], c6);
	MiniRemove7(dis7, size7, arr7, rm7, c7);

}

static void MiniSize(char *dis, int size, int *arr, int c)
{
	int i = 0;
	avl_t *avl = 0;

	avl = AVLCreate(CMP);

	for (i = 0; i < size; ++i)
	{
		AVLInsert(avl, &arr[i]);
	}

	ReturnComper(c, AVLSize(avl), dis, __LINE__);

/*	PrintTree(avl->root, 0);
	printf("\n\n ********************** \n\n");*/

	AVLDestroy(avl);
}

static void TestSize()
{
	char dis1[] = "Made tree with 4 node, compering return value of AVLSize";
	int arr1[] = {30, 40, 20, 15};
	int size1 = 4;
	int c1 = 4;

	char dis2[] = "Made tree with 0 node, compering return value of AVLSize";
	int arr2[] = {1};
	int size2 = 0;
	int c2 = 0;

	char dis3[] = "Made tree with 10 nodes, compering return value of AVLSize";
	int arr3[] = {1, 2, 4, 5, 17, 23, 8, 24, 77 ,45};
	int size3 = 10;
	int c3 = 10;

	SetTest(5, "AVL Size");

	MiniSize(dis1, size1, arr1, c1);
	MiniSize(dis2, size2, arr2, c2);
	MiniSize(dis3, size3, arr3, c3);
}


static void TestForEach()
{
	SetTest(9, "BST ForEach");

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
			TestFind();
			break ;
		case 4:
			TestRemove();
			break ;
		case 5:
			TestSize();
			break ;
		case 6:
			TestForEach();
			break ;
		case 7:
			break ;
		case 8:
			break ;
		case 9:
			break ;
		default:
			TestCreateAndDestroy();
			TestInsert();
			TestFind();
			TestRemove();
			TestSize();
			TestForEach();
	}
	PrintSummery();

	return 0;
}
