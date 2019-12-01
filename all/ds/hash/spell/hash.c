
#include <assert.h>	/*	assert()		*/
#include <stdlib.h>	/*	free(), malloc()	*/
#include <string.h>	/*	memset()		*/

#include "dlist.h"
#include "hash.h"

#define SIZEOF_HASH_T(table_size) ((sizeof(hash_t) + (sizeof(dlist_t*) * (table_size - 1))))
#define FAIL 1

struct hash
{
	hash_cmp_t DataCmp;
	hash_func_t HashFunc;
	size_t size;
	size_t table_size;
	dlist_t *table[1];
};

static void FreeTable(hash_t *hash, size_t size);

hash_t *HashCreate(hash_func_t HashFunc, size_t hash_size, hash_cmp_t DataCmp)
{
	hash_t *hash = NULL;
	size_t i = 0;

	assert(DataCmp);
	assert(HashFunc);

	hash = (hash_t*)malloc(SIZEOF_HASH_T(hash_size));
	if (!hash)
	{
		return NULL;
	}

	hash->DataCmp = DataCmp;
	hash->HashFunc = HashFunc;
	hash->table_size = hash_size;
	hash->size = 0;

	for (i = 0; i < (hash_size); ++i)
	{
		hash->table[i] = DlistCreate();
		if (!(hash->table[i]))
		{
			FreeTable(hash, i);
			free(hash);
			hash = NULL;
			break ;
		}
	}

	return hash;
}

void HashDestroy(hash_t *hash)
{
	assert(hash);

	FreeTable(hash, (hash->table_size));
	memset(hash, 0 , SIZEOF_HASH_T(hash->table_size));
	free(hash);
	hash = NULL;
}

static void FreeTable(hash_t *hash, size_t size)
{
	size_t i = 0;

	for (i = 0; i < size; ++i)
	{
		DlistDestroy(hash->table[i]);
	}
}

int HashInsert(hash_t *hash, void *data)
{
	int index = 0;
	dlist_iter_t iter = NULL;

	assert(hash);

	index = hash->HashFunc(data);

	iter = DlistPushBack(hash->table[index], data);

	if (DlistIsSameIter(DlistEnd(hash->table[index]), iter))
	{
		return FAIL;
	}

	++hash->size;

	return 0;
}

int HashRemove(hash_t *hash, const void *data)
{
	int index = 0;
	dlist_iter_t iter = NULL;
	dlist_iter_t end = NULL;

	assert(hash);

	index = hash->HashFunc(data);

	iter = DlistBegin(hash->table[index]);
	end = DlistEnd(hash->table[index]);

	iter = DlistFind(iter, end, hash->DataCmp, (void*)data);
	if (DlistIsSameIter(end, iter))
	{
		return FAIL;
	}

	DlistRemove(iter);

	--hash->size;

	return 0;
}

void *HashFind(const hash_t *hash, const void *data)
{
	int index = 0;
	dlist_iter_t iter = NULL;
	dlist_iter_t end = NULL;

	assert(hash);

	index = hash->HashFunc(data);

	iter = DlistBegin(hash->table[index]);
	end = DlistEnd(hash->table[index]);

	iter = DlistFind(iter, end, hash->DataCmp, (void*)data);
	if (DlistIsSameIter(end, iter))
	{
		return NULL;
	}

	return DlistGetData(iter);
}

size_t HashSize(const hash_t *hash)
{
	assert(hash);

	return hash->size;
}

int HashIsEmpty(const hash_t *hash)
{
	assert(hash);

	return (0 == hash->size);
}


int HashForEach(hash_t *hash, act_func_t Act, void *param)
{
	size_t index = 0;
	int ret = 0;
	dlist_iter_t iter = NULL;
	dlist_iter_t end = NULL;

	assert(hash);
	assert(Act);

	for (index = 0; index < hash->table_size; ++index)
	{
		iter = DlistBegin(hash->table[index]);
		end = DlistEnd(hash->table[index]);

		ret = DlistForEach(iter, end, Act, param);
		if (ret)
		{
			break ;
		}
	}

	return ret;
}
