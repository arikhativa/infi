
#include <stdlib.h> /* malloc() */
#include <assert.h> /* assert() */
#include <string.h> /* memset() */

#include "heap.h"
#include "pque.h"

#define GENERIC_SIZE 20

struct pque
{
	heap_t *heap;
	is_priority_before_t IsBefore;
	void *param;        
};


static int WraperForHeap(const void *usr_data, const void *heap_data, void *param)
{
	int ret1 = 0;
	int ret2 = 0;

	pque_t *que = param;

	ret1 = que->IsBefore(usr_data, heap_data, que->param);

	ret2 = que->IsBefore(heap_data, usr_data, que->param);

	if (ret1 == ret2)
	{
		return 0;
	}

	if (ret1)
	{
		return 1;
	}

	return -1;
}

pque_t *PqueCreate(is_priority_before_t IsBefore, void *param)
{
	pque_t *que = 0;

	assert(IsBefore);

	que = (pque_t*)malloc(sizeof(pque_t));
	if (!que)
	{
		return NULL;
	}


	que->heap = HeapCreate(WraperForHeap, GENERIC_SIZE, que);
	if (!que->heap)
	{
		free(que);
		return NULL;
	}
	que->IsBefore = IsBefore;
	que->param = param;

	return que;
}

void PqueDestroy(pque_t *que)
{
	assert(que);

	HeapDestroy(que->heap);
	memset(que, 0 , sizeof(pque_t));
	free(que);
	que = NULL;
}

void PDequeue(pque_t *que)
{
	assert(que);

	HeapPop(que->heap);
}

void *PquePeek(pque_t *que)
{
	assert(que);

	return HeapPeek(que->heap);
}

int PEnqueue(pque_t *que, void *data)
{
	int ret = 0;

	assert(que);

	ret = HeapPush(que->heap, data);

	return ret;
}

int PqueIsEmpty(pque_t *que)
{
	assert(que);

	return HeapIsEmpty(que->heap);
}

size_t PqueSize(pque_t *que)
{
	assert(que);

	return HeapSize(que->heap);
}

void PqueClear(pque_t *que)
{
	assert(que);

	while (!HeapIsEmpty(que->heap))
	{
		HeapPop(que->heap);
	}
}

void PqueErase(pque_t *que, data_cmp_t DataCmp, void *param)
{
	assert(que);
	assert(DataCmp);

	HeapRemove(que->heap, DataCmp, param);
}
