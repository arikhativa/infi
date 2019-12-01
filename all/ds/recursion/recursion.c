

#include <stddef.h>	/*	size_t	*/
#include <assert.h>	/*	assert()	*/

#include "recursion.h"
#include "slist.h"

size_t IterFibonacci(size_t n)
{
	size_t ret = 1;
	size_t num = 1;
	size_t num_before = 0;

	if (!n)
	{
		return 0;
	}

	--n;
	for (; n; --n)
	{
		ret = num + num_before;
		num_before = num;
		num = ret;
	}

	return ret;
}

size_t RecursFibonacci(size_t n)
{
	if (2 > n)
		return n;

	return (RecursFibonacci(n-1) + RecursFibonacci(n-2));
}

size_t Strlen(const char *s)
{
	assert(s);

	if ('\0' == *s)
		return 0;

	return (1 + Strlen(s + 1));
}

int Strcmp(const char *s1, const char *s2)
{
	assert(s1);
	assert(s2);

	if (!*s1 || *s1 != *s2)
		return (*s1 - *s2);

	return (Strcmp(s1 + 1, s2 + 1));
}

char *Strcpy(char *dest, const char *src)
{
	assert(dest);
	assert(src);

	if (!*src)
	{
		*dest = *src;
		return NULL;
	}

	*dest = *src;
	Strcpy(dest + 1, src + 1);

	return dest;


}

char *Strcat(char *dest, const char *src)
{
	assert(dest);
	assert(src);

	if (!*dest)
	{
		Strcpy(dest, src);
		return 0;
	}

	Strcat(dest+1, src);

	return dest;
}

int IsMatch(const char *hay, const char *need)
{
	if (!*need)
		return 1;

	if (*need != *hay)
		return 0;

	return IsMatch(hay + 1, need +1);
}

char *Strstr(const char *haystack, const char *needle)
{
	if (!*haystack)
		return NULL;

	if (*haystack == *needle && IsMatch(haystack +1, needle +1))
	{
			return (char*)haystack;
	}
	
	return Strstr(haystack+1, needle);
}


static slist_node_t *AsistFlip(slist_node_t *node_before, slist_node_t *node)
{
	slist_node_t *tmp = NULL;

	if (!node)
		return node_before;

	tmp = AsistFlip(node, node->next);

	node->next = node_before;

	return tmp;
}


slist_node_t *RecFlip(slist_node_t *head)
{
	assert(head);

	return AsistFlip(NULL, head);
}

static void StackSortedInsert(stack_t *stack, int elem)
{
        int top_elem = 0;

	assert(stack);

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
                        StackSortedInsert(stack, elem);
                        StackPush(stack, &top_elem);
                }
        }
}

void RecStackSort(stack_t *stack)
{
        int tmp = 0;

	assert(stack);

        if (!IsStackEmpty(stack))
        {
                tmp = *(int*)StackPeek(stack);
                StackPop(stack);
                RecStackSort(stack);
        	StackSortedInsert(stack, tmp);
        }

        return ;
}









