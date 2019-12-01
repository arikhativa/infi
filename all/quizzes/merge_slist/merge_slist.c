
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "slist.h"


typedef int (*is_before_t)(void *data, void *data_maybe_before);

int IsBefore(void *data1, void *data2)
{
	int *d1 = data1;
	int *d2 = data2;

	return (*d1 > *d2);
}

static void SwapPtr(void **data1, void **data2)
{
        void *tmp = 0;

        tmp = *data1;
        *data1 = *data2;
        *data2 = tmp;
}

void PrintList(slist_node_t *list)
{
	while (list)
	{
		printf("[%d]", *(int*)list->data);
		list = list->next;
		if (list)
			printf(" - ");
	}
	printf("\n");
}

slist_node_t *SortSlist(slist_node_t *node, is_before_t IsBefore)
{
	slist_node_t *run = NULL;

	assert(node);

	run = node->next;

	while (node->next)
	{
		while (run)
		{
			if (IsBefore(node->data, run->data))
			{
				SwapPtr(&node->data, &run->data);
			}
			run = run->next;
		}
		node = node->next;
		run = node->next;
	}

	return node;
}

slist_node_t *MargeSlist(slist_node_t *list1, slist_node_t *list2, is_before_t IsBefore)
{
	slist_node_t *list1_end = NULL;

	assert(list1);
	assert(list2);
	assert(IsBefore);

	list1_end = SortSlist(list1, IsBefore);
	list1_end->next = list2;
	SortSlist(list1, IsBefore);

	return list1;
}

int main()
{
	int arr[] = {4, 2, 3, 1};
	int arr2[] = {6, 7, 5, 55};
	slist_node_t *n2;
	slist_node_t *n3;
	slist_node_t *n4;
	slist_node_t *list1;
	slist_node_t *list2;

	n4 = SlistCreateNode(&arr[3], NULL);
	n3 = SlistCreateNode(&arr[2], n4);
	n2 = SlistCreateNode(&arr[1], n3);
	list1 = SlistCreateNode(&arr[0], n2);

	n4 = SlistCreateNode(&arr2[3], NULL);
	n3 = SlistCreateNode(&arr2[2], n4);
	n2 = SlistCreateNode(&arr2[1], n3);
	list2 = SlistCreateNode(&arr2[0], n2);

	printf("-- list1 - ");
	PrintList(list1);
	printf("-- list2 - ");
	PrintList(list2);

	MargeSlist(list1, list2, IsBefore);
	printf("\n-- Both - ");
	PrintList(list1);

	SlistFreeAll(list1);

	return 0;
}



























