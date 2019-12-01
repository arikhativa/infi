
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Q_SIZE 6

typedef struct queue
{
	char que[Q_SIZE];
	size_t elem_in_q;
	size_t first_elem;
} que_t;

que_t *Create()
{
	que_t *new = malloc(sizeof(que_t));

	new->elem_in_q = 0;
	new->first_elem = 0;

	return new;
}

int Push(que_t *que, char ch)
{
	if (Q_SIZE == que->elem_in_q)
		return -1;

	que->que[(que->first_elem + que->elem_in_q) % Q_SIZE] = ch;

	++que->elem_in_q;

	return 0;
}

int Pop(que_t *que, char *ret)
{
	if (!que->elem_in_q)
		return -1;

	*ret = que->que[que->first_elem];

	que->first_elem = (que->first_elem + 1) % Q_SIZE;
	--que->elem_in_q;

	return 0;
}

int main()
{
	int i;
	char s[] = "0123456789";
	char a[] = "abcdefgh";
	char ret[20] = {0};
	que_t *que = Create();

	memset(que->que, 0 , Q_SIZE);

	for (i = 0 ; i < 9; ++i)
		Push(que, s[i]);

	printf("-- %s\n", que->que);

	Pop(que, ret);
	printf("-- %s\n", ret);

	for (i = 0 ; i < 7; ++i)
	{
		Push(que, a[i]);
		Pop(que, ret);
	}
		Push(que, a[i]);
		Push(que, a[i]);

	printf("-- %s\n", ret);
	printf("-- %s\n", que->que);
	return 0;
}

