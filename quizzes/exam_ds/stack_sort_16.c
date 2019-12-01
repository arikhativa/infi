
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

void StackInsert(stack_t *stack, int elem)
{
	int top_elem = 0;

	if (IsStackEmpty(stack))
		StackPush(stack, &elem);
	else
	{
		top_elem = *(int*)StackPeek(stack);

		if (elem > top_elem)
			StackPush(stack, &elem);
		else
		{
			StackPop(stack);
			StackInsert(stack, elem);
			StackPush(stack, &top_elem);
		}
	}
}

void StackSort(stack_t *stack)
{
	int tmp = 0;

	if (!IsStackEmpty(stack))
	{
		tmp = *(int*)StackPeek(stack);
		StackPop(stack);
	}

	while (!IsStackEmpty(tmp))
	{
		StackInsert(stack, *(int*)StackPeek(tmp));
		StackPop(tmp);
	}

	DestroyStack(tmp);

	return 0;
}

int main()
{
	int size = 5;
	int i;
	int arr[] = {5, 7, 3, 30, 1};
	stack_t *stack = CreateStack(20, 4);

	for (i = 0; i < size; ++i)
		StackPush(stack, &arr[i]);

	StackSort(stack, 20, 4);
	for (i = 0; i < size; ++i)
	{
			printf("-- %d\n", *(int*)StackPeek(stack));
			StackPop(stack);
	}

	return 0;
}

