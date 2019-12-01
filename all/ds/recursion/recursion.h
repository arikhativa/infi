
/*
*   var 2.1 added slist.h, stack.h and the new funcs
*   var 1.1
*/

#ifndef __RECURSION_EX_H__
#define __RECURSION_EX_H__

#include <stddef.h>

#include "slist.h"
#include "stack.h"

/*
    FILE NAME:
    recursion.c recursion.h recursion_test.c
    
*/

/*Return the value of the number in the n place fibonacci sequence. itreative implementation*/
size_t IterFibonacci(size_t n);

/*Return the value of the number in the n place fibonacci sequence. recursive implementation*/
size_t RecursFibonacci(size_t n);

/* Return the length of the string s. */
size_t Strlen(const char *s); 

/*Compare the strings s1 with s2. */
int Strcmp(const char *s1, const char *s2);

/* Copy the string src to dest, returning a pointer to the start of
dest. */
char *Strcpy(char *dest, const char *src);

/* Append the string src to the string dest,  returning  a  pointer
dest. */
char *Strcat(char *dest, const char *src);

/* Find  the first occurrence of the substring needle in the string
haystack, returning a pointer to the found substring. */
char *Strstr(const char *haystack, const char *needle);

/*  Flip a slist.
    return the new head.    */
slist_node_t *RecFlip(slist_node_t *head);

/*  Sort a stack of ints    */
void RecStackSort(stack_t *stack);

#endif /*__RECURSION_EX_H__*/

