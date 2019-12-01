/*
 * 	ver 1.1
 */
 
#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h> /* size_t */

typedef struct stack stack_t;

/**
 * allocate dynamic memory (stack frame) in the size of num of ele * ele size
 * arguments:
 *      number of element and sizeof the type.
 * return value: 
 *       pointer to stack 
 */

stack_t *CreateStack(size_t num_of_elements, size_t element_size);

/**
 * free the stack frame
 * arguments:
 *      pointer of struct stack.
 * return value:
 *      none.
 */

void DestroyStack(stack_t *stack);

/**
 * insert new value to the stack
 * arguments:
 *      pointer to stack and data.
 *return value:
 *      success or fail.
 */


int StackPush(stack_t *stack, const void *element);

/**
 * remove the last element
 * arguments:
 *       pointer to stack
 * return value:
 *       none
 */

void StackPop(stack_t *stack);

/**
 * get the current element 
 * 
 * arguments:
 *      pointer to stack
 * return value:
 *      current element.
 *      If the stack is empty, Peek() returns NULL.
 */

void *StackPeek(stack_t *stack);

/**
 * check if stack is empty
 * arguments:
 *      pointer to stack
 * return value:
 *      1 for empty , 0 otherwise
 */

int IsStackEmpty(stack_t *stack);

/**
 * get the number of arguments that inserted to the stack
 * arguments:
 *      pointer to stack
 * return value:
 *      number of arguments
 */

size_t StackSize(stack_t *stack); 


#endif /* __STACK_H_*/
