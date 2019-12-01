
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

node_t *FindNodeInLoop(node_t *list)
{
	node_t *run = list->next;

	while (run != NULL && list != run)
	{
		run = run->next;

		if (run == NULL)
			return NULL;
		if (list == run)
			return run;
		list = list->next;
		run = run->next;
	}

	return run;
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

size_t SlistSize(node_t *list)
{
	size_t n = 0;

	while (list != NULL)
	{
		++n;
		list = list->next;
	}

	return n;
}

node_t *FindInter(node_t *l1, node_t *l2)
{
	node_t *long_list = l1;
	node_t *short_list = l2;
	size_t long_size = SlistSize(l1);
	size_t short_size = SlistSize(l2);

	if (long_size < short_size)
	{
		Swap(&long_size, &short_size);
		SwapPtr((void**)&long_list, (void**)&short_list);
	}

	while (long_size != short_size)
	{
		--long_size;
		long_list = long_list->next;
	}

	while (long_list != NULL && long_list != short_list)
	{
		long_list = long_list->next;
		short_list = short_list->next;
	}

	return long_list;
}

void DisconnectInter(node_t *list, node_t *bad_list)
{
	node_t *inter = FindInter(list, bad_list);

	while (bad_list->next != inter)
		bad_list = bad_list->next;

	bad_list->next = NULL;
}

void DisconnectLoop(node_t *list)
{
	node_t *node_in_loop;
	node_t *after_node_in_loop;
	node_t *inter;
	node_t *run;

	node_in_loop = FindNodeInLoop(list);
	after_node_in_loop = node_in_loop->next;

	node_in_loop->next = NULL;

	DisconnectInter(list, after_node_in_loop);

	node_in_loop->next = after_node_in_loop;
}

int main()
{
	node_t *n1;
	node_t *n2;
	node_t *n3;
	node_t *n4;
	node_t *n5;

	node_t *a1;
	node_t *a2;
	node_t *a3;

	node_t *nodein;
	node_t *tmp;

	int arr[] = {1, 2, 3, 4, 5};
	int arr2[] = {6, 7, 8};

	a3 = Create(&arr2[2], NULL);
	a2 = Create(&arr2[1], a3);
	a1 = Create(&arr2[0], a2);

	n5 = Create(&arr[4], a1);
	n4 = Create(&arr[3], n5);
	n3 = Create(&arr[2], n4);
	n2 = Create(&arr[1], n3);
	n1 = Create(&arr[0], n2);

	a3->next = n3;

	PrintSList(n1, 10);
	DisconnectLoop(n1);
	PrintSList(n1, 10);

	free(n1);
	free(n2);
	free(n3);
	free(n4);
	free(n5);

	free(a1);
	free(a2);
	free(a3);


	return 0;
}

