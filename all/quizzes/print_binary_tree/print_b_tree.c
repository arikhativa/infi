
#include <stdio.h>

#include "bst.h"
#include "queue.h"
#include "slist.h"

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


void PrintBinaryTree(bst_node_t *node, bst_node_t *end, size_t size)
{
	queue_t *que = NULL;

	que = QueueCreate();

	QEnqueue(que, node);

	while(!IsQueueEmpty(que))
	{
		node = QueuePeek(que);

		if (!BSTIsSameIter(end, node))
		{
			QEnqueue(que, (node->left_child));
			QEnqueue(que, (node->right_child));

			printf("%d, ", *(int*)(node->data));

		}
		QDequeue(que);
	}
	printf("\n");

	QueueDestroy(que);
}



int Cmp(const void *tree, const void *data, void *param)
{
	(void)param;

	if (*(int*)data < *(int*)tree)
		return -1;

	return 1;
}


int main()
{
	bst_t *tree;
	int size = 7;
	int arr[] = {1, 2, 5, 55, 8, 90, 10};
	int i = 0;


	tree = BSTCreate(Cmp, 0);

	for (i = 0; i < size; ++i)
		BSTInsert(tree, &arr[i]);

	//PrintTree(tree, tree->end.right_child, 0);
	PrintBinaryTree(tree->end.right_child, &tree->end, size);


	BSTDestroy(tree);


}
