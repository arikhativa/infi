
#include <stdio.h>
#include <stdlib.h>

typedef int (*cmp_func_t)(const void *tree, const void *new, void *param);
typedef struct node node_t;
typedef struct bst bst_t;

typedef enum child
{
	LEFT = 0,
	RIGHT
} child_t;

struct node
{
	void *data;
	node_t* parent;
	node_t *child[2];
};

struct bst
{
	node_t dummy;
	cmp_func_t Cmp;
	void *param;
};


void PrintTree(bst_t *bst, node_t *iter, int space)
{
	int i = 5;

	if (iter == &bst->dummy)
		return ;
	space += 5;

	PrintTree(bst, iter->child[RIGHT] ,space);

	printf("\n");
	for (; i < space; ++i)
		printf(" ");

	printf("%d\n", *(int*)iter->data);

	PrintTree(bst, iter->child[LEFT], space);
}


static int CMP(const void *tree, const void *data, void *param)
{
	(void)param;


	if (*(int*)data < *(int*)tree)
		return LEFT;
	return RIGHT;
}


static node_t *Create(int *data)
{
	node_t *new;

	new = malloc(sizeof(node_t));

	new->data = data;

	return new;
}

void Swap(size_t *a, size_t *b)
{
	size_t tmp = *a;

	*a = *b;
	*b = tmp;
}

void SwapPtr(void **a, void **b)
{
	void *tmp = *a;

	*a = *b;
	*b = tmp;
}

bst_t * BSTCreate(cmp_func_t BstCmp, void *param)
{
	bst_t *bst = 0;

	bst = (bst_t*)malloc(sizeof(bst_t));
	if (!bst)
	{
		return NULL;
	}

	bst->Cmp = BstCmp;
	bst->param = param;

	bst->dummy.parent = &bst->dummy;
	bst->dummy.data = param;
	bst->dummy.child[LEFT] = &bst->dummy;
	bst->dummy.child[RIGHT] = &bst->dummy;

	return bst;
}

node_t *BSTInsert(bst_t *bst, void *data)
{
	node_t *new = Create(data);
	node_t *parent = bst->dummy.child[LEFT];
	node_t *cur = bst->dummy.child[LEFT];
	node_t *end = &bst->dummy;
	child_t dir = LEFT;

	new->child[LEFT] = &bst->dummy;
	new->child[RIGHT] = &bst->dummy;

	while (cur != end)
	{
		parent = cur;
		dir = bst->Cmp(cur->data, data, bst->param);
		cur = cur->child[dir];
	}

	parent->child[dir] = new;
	new->parent = parent;

	return new;
}


int main()
{
	int size = 7;
	int arr[] = {4, 2, 6, 1,  3, 5, 7};
	int i = 0;
	bst_t *bst = 0;
	node_t *iter[7];

	bst = BSTCreate(CMP, &size);

	for (i = 0; i < size; ++i)
		iter[i] = BSTInsert(bst, &arr[i]);

	for (i = 0; i < 0; ++i)
	{
	printf("-- node%d\n", *(int*)(iter[i]->data));
	printf("-- parent %d\n", *(int*)(iter[i]->parent->data));
	printf("-- left %d\n", *(int*)(iter[i]->child[LEFT]->data));
	printf("-- right %d\n", *(int*)(iter[i]->child[RIGHT]->data));
	printf("\n");
	}

	PrintTree(bst, bst->dummy.child[LEFT], 0 );
	return 0;
}

