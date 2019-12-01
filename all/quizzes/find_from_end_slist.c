
#include <stdio.h>

#include "slist.h"


void PrintList(slist_node_t *start)
{
        while (start != NULL)
        {
                printf("-- %d\n", *(int*)start->data);
        start = start->next;
        }
}
//	printf("-- n %d\n", *(int*)node->data);

static slist_node_t *FindFromEnd(slist_node_t *node, int *n)
{
	slist_node_t *ret;

	if (!node)
		return NULL;

	ret = FindFromEnd(node->next, n);

	if (0 == *n)
	{
		ret = node;
	}
	--*n;

	return ret;
}

int main()
{
	int n = 3;
	int arr[] = {1, 2, 3, 4};

	slist_node_t *n1;
	slist_node_t *n2;
	slist_node_t *n3;
	slist_node_t *n4;
	slist_node_t *tmp;

	n4 = SlistCreateNode(&arr[3], NULL);
	n3 = SlistCreateNode(&arr[2], n4);
	n2 = SlistCreateNode(&arr[1], n3);
	n1 = SlistCreateNode(&arr[0], n2);

//	PrintList(n1);
	tmp = FindFromEnd(n1, &n);

	printf("-- tmp %d\n", *(int*)tmp->data);

	SlistFreeAll(n1);

	return 0;
}
