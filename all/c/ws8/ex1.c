
#include <stdio.h> /* printf() , sprintf()*/
#include <stdlib.h> /* strdup() */
#include <unistd.h> /* atoi() */
#include <string.h> /* strlen() , strcpy(), strcat() */
#include <assert.h> /* assert() */
#include "ws8.h"

typedef void (*print_func_t)(const element_t *arr);
typedef int (*add_func_t)(element_t *arr, int val);
typedef void (*free_func_t)(element_t  *arr);

typedef struct  element_funcs
{
	print_func_t print;
	add_func_t add;
	free_func_t free;
} element_funcs_t;

struct  element
{
	void* data;
	const element_funcs_t *funcs;
};

char *strdup(const char *s);


/*
 *	Init Array
 */


element_t *InitArray(size_t size)
{
	element_t *arr = 0;
	element_t *runner = 0;

	if (!(arr = (element_t*)malloc(sizeof(element_t) * (size + 1))))
	{
		return NULL;
	}
	runner = arr;
	++size;
	while (size)
	{
		runner->funcs = NULL;
		runner->data = 0;
		++runner;
		--size;
	}

	return arr;
}


/* 
 *	String Functions 
 */


static void PrintString(const element_t *elem)
{
	char *tmp = 0;

	assert(elem);

	tmp = (char*)elem->data;
	printf("%s", tmp);
}

static char *StrJoin(const char *s1, const char *s2)
{
	char *ret = 0;

	assert(s1);
	assert(s2);

	if (!(ret = (char*)malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1))))
	{
		return NULL;
	}
	ret = strcpy(ret, s1);
	ret = strcat(ret, s2);

	return ret;
}

static int CountDigits(int number)
{
	int count = 0;

	while (number)
	{
		number /= 10;
		++count;
	}

	return count;
}

int AddValToString(element_t *elem, int val)
{
	char *tmp = 0;
	int count = 0;
	char *str_num = 0;

	assert(elem);

	tmp = (char*)elem->data;
	count = CountDigits(val);
	if (!(str_num = (char*)malloc(sizeof(char) * (count + 1))))
	{
		return -1;
	}
	str_num[count] = '\0';
	sprintf(str_num, "%d", val);
	if (!(tmp = StrJoin(tmp, str_num)))
	{
		free(str_num);
		str_num = NULL;
		return -1;
	}
	free(elem->data);
	elem->data = (void*)tmp;
	free(str_num);
	tmp = NULL;
	str_num = NULL;


	return 0;
}

void FreeString(element_t *elem)
{
	assert(elem);

	free(elem->data);
	elem->data = NULL;
	elem->funcs = NULL;
}

element_funcs_t funcs_for_str = { PrintString, AddValToString, FreeString };

int InitString(element_t *arr, const char *str, size_t index)
{
	char *copy_of_str = 0;

	assert(arr);
	assert(str);

	if (NULL != arr[index].data)
	{
		free(arr[index].data);
		arr[index].data = NULL;
	}
	if (!(copy_of_str = strdup(str)))
	{
		return -1;
	}
	arr[index].data = copy_of_str;
	arr[index].funcs = &funcs_for_str;

	return 0;
}


/*
 * 	Int Functions
 */


void PrintInt(const element_t *elem)
{
	assert(elem);

	printf("%ld", (long)elem->data);
}

int AddValToInt(element_t *elem, int val)
{
	assert(elem);

	*((int*)&(elem->data)) += val;

	return 0;
}

void FreeInt(element_t *elem)
{
	assert(elem);

	elem->data = NULL;
	elem->funcs = NULL;
}

element_funcs_t funcs_for_int = { PrintInt, AddValToInt, FreeInt };

int InitInt(element_t *arr, int num, size_t index)
{
	assert(arr);

	*((int*)&(arr[index].data)) = num;
	arr[index].funcs = &funcs_for_int;

	return 0;
}


/* 
 *	Functions for Float
 */


void PrintFloat(const element_t *elem)
{
	assert(elem);

	printf("%f", *((float*)&(elem->data)));
}


int AddValToFloat(element_t *elem, int val)
{
	assert (elem);

	*((float*)&(elem->data)) += val;

	return 0;
}

void FreeFloat(element_t *elem)
{
	assert(elem);

	elem->data = NULL;
	elem->funcs = NULL;
}


element_funcs_t funcs_for_float = { PrintFloat, AddValToFloat, FreeFloat };

int InitFloat(element_t *arr, float num, size_t index)
{
	assert(arr);

	*((float*)&(arr[index].data)) = num;
	arr[index].funcs = &funcs_for_float;

	return 0;
}


/* 
 *	Functions for "All"
 */


int AddValToAll(element_t *arr, int val)
{
	assert(arr);

	while (NULL != arr->funcs)
	{
		if (0 != (arr->funcs->add(arr, val)))
		{
			return -1;
		}
		++arr;
	}

	return 0;
}

void PrintAll(const element_t *arr)
{
	assert(arr);

	printf("{");
	while (NULL != arr->funcs)
	{
		arr->funcs->print(arr);
		++arr;
		if (NULL != arr->funcs)
		{
			printf(", ");
		}
	}
	printf("}");
	printf("\n");
}

void FreeAll(element_t *arr)
{
	assert(arr);

	while (NULL != arr->funcs)
	{
		arr->funcs->free(arr);
		++arr;
	}
}
