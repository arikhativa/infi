

#include <assert.h>	/* aasert() */

#include "fsa.h"

#define WORD 8
#define INDEX_OF_FIRST_META_DATA 2

struct fsa
{
        size_t block_size;
        size_t mem_size;
        size_t next;
};


static size_t CalcRealBlockSize(size_t user_block)
{
	size_t add = WORD;

	if (!(user_block % WORD) && user_block)
	{
		add = 0;
	}

	user_block = ((user_block / WORD) + 1 ) * WORD;

	return user_block + add; 
}

size_t FSASuggestSize(size_t num_of_blocks, size_t block_size)
{
	if (!(num_of_blocks) || !(block_size))
	{
		return 0;
	}

	block_size = CalcRealBlockSize(block_size);

	return (sizeof(fsa_t) + (block_size * num_of_blocks));
}

static void FillMetaData(size_t *run, size_t num_of_blocks, size_t block_size)
{
	size_t index = (block_size / WORD);
	size_t counter = INDEX_OF_FIRST_META_DATA;

	assert(run);

	while (num_of_blocks)
	{
		counter += (block_size / WORD);
		*run = counter;
		run += index; 
		--num_of_blocks;
	}
}

fsa_t *FSAInit(void *mem, size_t buffer_size, size_t block_size)
{
	fsa_t *fsa = 0;
	size_t num_of_blocks = 0;
	size_t *run = 0;

	assert(mem);

	if (sizeof(fsa_t) > buffer_size || !block_size)
	{
		return NULL;
	}

	fsa = mem;
	run = (size_t*)fsa;
	run += (offsetof(fsa_t, next) / WORD) + 1;

	fsa->block_size = CalcRealBlockSize(block_size);
	fsa->mem_size = buffer_size;
	fsa->next = INDEX_OF_FIRST_META_DATA;

	num_of_blocks = (buffer_size - sizeof(fsa_t)) / fsa->block_size;

	FillMetaData(run, num_of_blocks, fsa->block_size);

	return fsa;
}

static void SwapSizeT(size_t *s1, size_t *s2)
{
	size_t swap = 0;

	assert(s1);
	assert(s2);

	swap = *s1;
	*s1 = *s2;
	*s2 = swap;
}

void *FSAAlloc(fsa_t *mem)
{
	size_t *ret = 0;

	assert(mem);

	if ((mem->next + 1 ) * WORD == mem->mem_size)
	{
		return NULL;
	}

	ret = (size_t*)mem;
	ret += (offsetof(fsa_t, next) / WORD) + mem->next;

	SwapSizeT((ret - 1), &mem->next);

	return ret;
}

void FSAFree(void *block)
{
	size_t bytes_to_next = 0;
	size_t *ptr = block;

	assert(block);

	bytes_to_next = *(ptr - 1);
	SwapSizeT((ptr - 1), (ptr - bytes_to_next));	
}

size_t FSACountFree(fsa_t *mem)
{
	size_t *run = 0;
	size_t count = 0;
	size_t index = INDEX_OF_FIRST_META_DATA;
	size_t num_of_blocks = 0;

	assert(mem);

	run = (size_t*)mem;
	run += (offsetof(fsa_t, mem_size) / WORD);

	num_of_blocks = (mem->mem_size - sizeof(fsa_t)) / mem->block_size;

	while (num_of_blocks)
	{
		if (*(run + index) != index)
		{
			++count;
		}
		index += (mem->block_size / WORD);
		--num_of_blocks;
	}

	return count;
}
