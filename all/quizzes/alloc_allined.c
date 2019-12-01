
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>

void PrintMem(char *ptr, size_t size)
{
	while (size)
	{
		printf("%c", *ptr);
		++ptr;
		--size;
	}
	printf("\n");
}

void *alloc_allined(size_t size, size_t allined)
{
	size_t space_to_origen = 0;
	char *ret = NULL;

	if (!size || !allined)
		return NULL;

	ret = (char*)malloc(size + allined + sizeof(size_t));

	if (!ret)
		return NULL;

	if (space_to_origen = ((size_t)ret + sizeof(size_t)) % allined)
	{
		space_to_origen = allined - space_to_origen;
	}

	*(size_t*)(ret + space_to_origen) = (size_t)ret;

	return (ret + space_to_origen + sizeof(size_t));
}

void free_allined(void *ptr)
{
	size_t *tmp = ptr;

	assert(ptr);

	free((size_t*)*(tmp - 1));
}


int main()
{
	void *tmp = 0;
	size_t allined = 3;
	size_t size = 20;

	tmp = alloc_allined(size, allined);

	if ((size_t)tmp % allined)
	{
		printf("-- there is a gap of - %ld\n", ((size_t)tmp % allined));
	}
	memset(tmp, '0', size);


	free_allined(tmp);
}
