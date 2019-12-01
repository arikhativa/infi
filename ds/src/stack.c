
#include <stdlib.h> /* malloc() */
#include <string.h> /* memmove() */
#include <assert.h> /* assert() */






#include "stack.h"

struct stack
{             
	void *end;
	void *current;
	size_t element_size;
	void *start;
};

stack_t *CreateStack(size_t num_of_elements, size_t element_size)
{
	stack_t *stack = 0;
	size_t size_of_stack = element_size * num_of_elements;
	
	if (!(stack = (stack_t*)malloc(sizeof(stack_t))))
	{
		return NULL;
	}
	if (!(stack->start = (void*)malloc(size_of_stack)))
	{
		free(stack);
		return NULL;
	}
	stack->element_size = element_size;
	stack->current = NULL;
	stack->end = (void*)((char*)stack->start + size_of_stack - element_size);

	return stack;
}

void DestroyStack(stack_t *stack)
{
	assert(stack);

	free(stack->start);
	stack->start = NULL;
	stack->current = NULL;
	stack->element_size = 0;
	stack->end = NULL;

	free(stack);
	stack = NULL;
}

int StackPush(stack_t *stack, const void *element)
{
	assert(stack);

	if (stack->current == stack->end)
	{
		return -1;
	}
	if (stack->current == NULL)
	{
		stack->current = stack->start;
	}
	else
	{
		stack->current = (void*)((char*)stack->current + stack->element_size);
	}
	memmove(stack->current, element, stack->element_size);

	return 0;
}

void StackPop(stack_t *stack)
{
	assert(stack);

	if (stack->current > stack->start)
	{
		stack->current = (void*)((char*)stack->current - stack->element_size);
	}
	else
	{
		stack->current = NULL;
	}
}

void *StackPeek(stack_t *stack)
{
	assert(stack);

	return stack->current;
}

int IsStackEmpty(stack_t *stack)
{
	assert(stack);

	if (stack->current == NULL)
	{
		return 1;
	}

	return 0;
}

size_t StackSize(stack_t *stack)
{
	size_t size = 0;

	assert(stack);

	if (NULL != stack->current)
	{
		size = ((char*)stack->current - (char*)stack->start) / stack->element_size;
		++size;
	}

	return size;
}

