

#include <assert.h>	/* aasert()	*/
#include <stddef.h>	/* size_t	*/
#include <sys/types.h>	/* ssize_t	*/

#include "vsa.h"

#define WORD sizeof(size_t)
#define SIZE_OF_LAST_WORD 1
#define MIN_SIZE ((WORD * 2) + sizeof(blk_header_t))

typedef struct vsa blk_header_t;

struct vsa
{
	ssize_t space_to_next_blk; /* space is in word size */
};

static ssize_t AbsSizeT(ssize_t num)
{
	return ((0 > num) ? -num : num);
}


vsa_t *VSAInit(void *mem, size_t mem_size)
{
	vsa_t *vsa = 0;

	assert(mem);

	if (MIN_SIZE > mem_size)
	{
		return NULL;
	}

	vsa = (vsa_t*)mem;

	mem_size = (mem_size / WORD) - SIZE_OF_LAST_WORD;

	vsa->space_to_next_blk = -mem_size;
	(vsa + mem_size)->space_to_next_blk = 0;

	return vsa;
}

static ssize_t CalcRealSizeToAlloc(size_t mem_size)
{
	size_t words_to_add = 0;

	words_to_add = (mem_size % WORD) ? 2 : 1;

	return (mem_size / WORD) + words_to_add;
}

static int AddFreeTogether(blk_header_t *header)
{
	blk_header_t *last_free = header;
	ssize_t origenal_size = header->space_to_next_blk;

	if (0 > header->space_to_next_blk)
	{
		header += -header->space_to_next_blk;

		while (0 > header->space_to_next_blk)
		{
			last_free->space_to_next_blk += header->space_to_next_blk;
			header += AbsSizeT(header->space_to_next_blk);
		}
	}

	return (last_free->space_to_next_blk != origenal_size);
}

void *VSAAlloc(blk_header_t *vsa, size_t size_to_alloc)
{
	ssize_t real_size = 0;

	assert(vsa);

	if (!size_to_alloc)
	{
		return NULL;
	}

	real_size = CalcRealSizeToAlloc(size_to_alloc);

	while (0 != vsa->space_to_next_blk)
	{
		if (-(vsa->space_to_next_blk) >= real_size)
		{
			if (AbsSizeT(vsa->space_to_next_blk) != real_size)
			{
				(vsa + real_size)->space_to_next_blk = vsa->space_to_next_blk + real_size;
			}

			vsa->space_to_next_blk += real_size + AbsSizeT(vsa->space_to_next_blk);

			/* move 'vsa' to the first valid address for the user */
			return (vsa + (sizeof(blk_header_t) / WORD));		
		}
			

		if (!AddFreeTogether(vsa))
		{
			vsa += AbsSizeT(vsa->space_to_next_blk);
		}
	}

	return NULL;
}

void VSAFree(void *ptr_to_free)
{
	blk_header_t *header = ptr_to_free;

	assert(ptr_to_free);

	--header;
	if (0 < header->space_to_next_blk)
	{
		header->space_to_next_blk = -header->space_to_next_blk;
	}
}

size_t VSALargestChunkAvailable(blk_header_t *mem)
{
	blk_header_t *header = 0;
	ssize_t largest = 0;

	assert(mem);

	header = (blk_header_t*)mem;

	while (0 != header->space_to_next_blk)
	{
		AddFreeTogether(header);

		if (header->space_to_next_blk < largest)
		{
			largest = header->space_to_next_blk;
		}

		header += AbsSizeT(header->space_to_next_blk);
	}

	return ((-largest) * WORD) - sizeof(blk_header_t);
}
