
#include <assert.h>	/*	assert()	*/
#include <stddef.h>	/*	size_t		*/


#include <stdio.h>




#include "search.h"

int IterBinarySearch(void *arr, size_t size, size_t elem_size, search_cmp_func_t cmp, void *req_data)
{
	char *array = arr;
	size_t mid = size / 2;
	size_t last_mid = 0;
	size_t l = 0;
	size_t r = size - 1;
	int stt = 0;

	assert(arr);
	assert(cmp);

	do
	{
		last_mid = mid;

		stt = cmp(array + (mid * elem_size), req_data);
		if (!stt)
		{
			return mid;
		}

		if (1 == stt) /* 'mid' is larger the 'req_data' */
		{
			r = mid;
		}
		else
		{
			l = mid;
		}

		mid = l + ((r - l) / 2);

	}while(last_mid != mid); /* If mid didn't change that mean there is not match */


	return -1;
}

static int RecBS(char *arr, size_t l, size_t r, size_t elem_size, search_cmp_func_t cmp, void *req_data)
{
	int mid = 0;
	int stt = -1;

	if (r >= l)
	{
		mid = l + ((r - l) / 2);

		stt = cmp(arr + (mid * elem_size), req_data);
		if (!stt)
		{
			return mid;
		}

		if (1 == stt) /* 'mid' is larger the 'req_data' */
		{
			r = mid - 1;
		}
		else
		{
			l = mid + 1;
		}

		stt = RecBS(arr, l, r, elem_size, cmp, req_data);
	}

	return stt;
}


int RecBinarySearch(void *arr, size_t size, size_t elem_size, search_cmp_func_t cmp, void *req_data)
{
	assert(arr);
	assert(cmp);

	return RecBS(arr, 0, size - 1, elem_size, cmp, req_data);
}

static size_t Sqrt(size_t size)
{
	size_t ret = 2;

	while ((ret * ret) < size)
	{
		++ret;
	}

	if ((ret * ret) != size)
	{
		--ret;
	}

	return ret;
}

static size_t CalcBlockSize(size_t size)
{
	return Sqrt(size);
}

int FindBlock(char *arr, size_t size, size_t elem_size, search_cmp_func_t cmp, void *req_data)
{
	size_t block_size = 0;
	size_t last_block = 0;
	size_t index = 0;
	int stt = 0;

	assert(arr);
	assert(cmp);

	block_size = CalcBlockSize(size);
	last_block = block_size * block_size;

	index = block_size - 1; /* Set 'index' to the last elem in the block*/

	while (-1 == (stt = cmp(arr + (index * elem_size), req_data)))
	{
		index += block_size; /* jump by block size */

		/* if 'size' dosent have whole sqrt(), there is another small,
		 * block at the end of the array.
		 * 'last_block' is this index.					*/
		if (index >= last_block)
		{
			return last_block;
		}
	}

	/* After we find the correct block, we send the bigining of it. */
	return index - (block_size - 1);
}

int JumpSearch(void *arr, size_t size, size_t elem_size, search_cmp_func_t cmp, void *req_data)
{
	size_t block = 0;
	size_t index = 0;
	int stt = 0;
	char *array = arr;

	assert(arr);
	assert(cmp);

	block = FindBlock(array, size, elem_size, cmp, req_data);

	for (index = block; index < size; ++index)
	{
		stt = cmp(array + (index * elem_size), req_data);
		if (!stt)
		{
			return index;
		}
	}

	return -1;
}
