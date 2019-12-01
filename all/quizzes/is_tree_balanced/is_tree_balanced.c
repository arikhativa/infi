
#include <stdio.h>
#include "bst.h"

#define BALANCE_FACTOR 1

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
        bst_node_t end; /* 'end' is a dummy node. it's left child points to it's self. */
        policy_func_t BstCmp;
        void *param;
};

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


int Max(int a, int b)
{
	if (b > a)
		a = b;

	return a;
}


int Rec(bst_node_t *node, bst_node_t *dummy)
{
	int ret_r = 0;
	int ret_l = 0;
	int sum;

	if (node == dummy)
		return 0;

	ret_l = Rec(node->left_child, dummy);
	ret_r = Rec(node->right_child, dummy);

	if (-1 == ret_l || -1 == ret_r)
	{
		return -1;
	}

	sum = ret_l - ret_r;
	if (sum < 0)
		sum = -sum;

	if (sum > BALANCE_FACTOR)
	{
		return -1;
	}

	return Max(ret_l, ret_r) + 1;
}

int IsTreeBalanced(bst_t *tree)
{
	if (-1 != Rec(tree->end.right_child, &tree->end))
		return 1;

	return 0;
}



int main()
{
	int arr[] = {4, 2, 6, 3, 5, 7, 11};
	int i = 0;
	int size = 7;
	bst_t *tree = 0;

	tree = BSTCreate(CMP, 0);
	for (i = 0; i < size; ++i)
	{
		BSTInsert(tree, &arr[i]);
	}

	PrintTree(tree, tree->end.right_child,0);
	printf("\n");

	i = IsTreeBalanced(tree);
	if (i)
		printf("-- Tree Is Balanced\n");
	else
		printf("-- Tree Is NOT  Balanced\n");


	BSTDestroy(tree);

	return 0;
}
