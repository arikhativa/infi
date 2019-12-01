
#include <stdio.h>
#include <stdlib.h>

typedef struct dlist_node node_t;

struct dlist_node
{
	void *data;
	node_t *next;
	node_t *prev;
};

static node_t *Create(int *data, node_t *prev, node_t *next)
{
	node_t *new;

	new = malloc(sizeof(node_t));

	new->data = data;
	new->next = next;
	new->prev = prev;

	return new;
}

void PrintSList(node_t *start, size_t size)
{
	while (size && start != NULL)
	{
		printf("-- %d\n", *(int*)start->data);
	start = start->next;
	--size;
	}

	printf("\n");
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

node_t *DlistInsertBefore(node_t *node, void *data)
{
	node_t *prev = node->prev;
	node_t *new = Create(data, prev, node);

	if (prev != NULL)
		prev->next = new;

	node->prev = new;

	return new;
}

void DlistRemove(node_t *node)
{
	node_t *next = node->next;
	node_t *prev = node->prev;

	if (prev != NULL)
		prev->next = next;
	if (next != NULL)
		next->prev = prev;

	free(node);
}

int main()
{
	node_t *n1;
	node_t *n2;
	node_t *n3;
	node_t *n4;
	node_t *n5;

	node_t *tmp;

	int arr[] = {1, 2, 3, 4, 5};

	n5 = Create(&arr[4], NULL, NULL);
	n4 = DlistInsertBefore(n5, &arr[3]);
	n2 = DlistInsertBefore(n4, &arr[1]);
	n3 = DlistInsertBefore(n4, &arr[2]);
	n1 = DlistInsertBefore(n2, &arr[0]);

	PrintSList(n1, 6);
	DlistRemove(n4);
	PrintSList(n1, 6);

	free(n1);
	free(n2);
	free(n3);
	free(n5);

	return 0;
}

