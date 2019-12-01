
#include <stdio.h>
#include <stdlib.h>

typedef struct slist_node node_t;

struct slist_node
{
	void *data;
	node_t *next;
};

static node_t *Create(int *data, node_t *next)
{
	node_t *new;

	new = malloc(sizeof(node_t));

	new->data = data;
	new->next = next;

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

void SlistRemove(node_t *node)
{
	node_t *next_node = node->next;

	SwapPtr((void*)&node->data, (void*)&next_node->data);

	node->next = next_node->next;

	free(next_node);
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

	n5 = Create(&arr[4], NULL);
	n4 = Create(&arr[3], n5);
	n3 = Create(&arr[2], n4);
	n2 = Create(&arr[1], n3);
	n1 = Create(&arr[0], n2);

	PrintSList(n1, 6);
	SlistRemove(n2);
	PrintSList(n1, 6);

	free(n1);
	free(n2);
	free(n4);
	free(n5);

	return 0;
}

