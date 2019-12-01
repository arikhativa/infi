
#include <stdio.h>
#include <stdlib.h>

typedef struct slist_node node_t;

struct slist_node
{
	void *data;
	node_t *next;
};

node_t *flip(node_t *start)
{
	node_t *last = NULL;
	node_t *cur = start;
	node_t *next_node = start->next;

	while (NULL != cur->next)
	{
		cur->next = last;
		last = cur;
		cur = next_node;
		next_node = cur->next;
	}

	cur->next = last;
	last = cur;

	return last;
}

static node_t *Create(int *data, node_t *next)
{
	node_t *new;

	new = malloc(sizeof(node_t));

	new->data = data;
	new->next = next;

	return new;
}

void PrintSList(node_t *start)
{
	while (start != NULL)
	{
		printf("-- %d\n", *(int*)start->data);
	start = start->next;
	}
}

int main()
{
	node_t *n1;
	node_t *n2;
	node_t *n3;
	node_t *n4;
	node_t *tmp;
	int arr[] = {1, 2, 3, 4};

	n4 = Create(&arr[3], NULL);
	n3 = Create(&arr[2], n4);
	n2 = Create(&arr[1], n3);
	n1 = Create(&arr[0], n2);

	PrintSList(n1);
	tmp = flip(n1);
	PrintSList(tmp);

	free(n1);
	free(n2);
	free(n3);
	free(n4);


	return 0;
}

