
#include <stdlib.h> /* malloc() */
#include <assert.h> /* assert() */
#include <string.h> /* memset() */

#include "dlist.h"
#include "sortlist.h"
#include "pque.h"

struct pque
{
	sort_list_t *list;
	is_before_t IsBefore;
	void *param;        
};


pque_t *PqueCreate(is_before_t IsBefore, void *param)
{
	pque_t *que = 0;

	assert(IsBefore);

	que = (pque_t*)malloc(sizeof(pque_t));
	if (!que)
	{
		return NULL;
	}

	que->list = SortListCreate(IsBefore, param);
	if (!que->list)
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

	SortListDestroy(que->list);
	memset(que, 0 , sizeof(pque_t));
	free(que);
	que = NULL;
}

void PDequeue(pque_t *que)
{
	assert(que);

	SortListPopFront(que->list);
}

void *PquePeek(pque_t *que)
{
	sortedl_iter_t iter = { 0 };

	assert(que);

	iter = SortListBegin(que->list);

	return SortListGetData(iter);
}

int PEnqueue(pque_t *que, void *data)
{
	sortedl_iter_t iter = { 0 };

	assert(que);

	iter = SortListInsert(que->list, data);

	return (SortListIsSameIter(iter, SortListEnd(que->list)));
}

int PqueIsEmpty(pque_t *que)
{
	assert(que);

	return SortListIsEmpty(que->list);
}

size_t PqueSize(pque_t *que)
{
	assert(que);

	return SortListSize(que->list);
}

void PqueClear(pque_t *que)
{
	assert(que);

	while (!SortListIsEmpty(que->list))
	{
		SortListPopFront(que->list);
	}
}

void PqueErase(pque_t *que, data_cmp_t DataCmp, void *param)
{
	sortedl_iter_t iter = { 0 };
	sortedl_iter_t end = { 0 };

	assert(que);
	assert(DataCmp);

	iter = SortListBegin(que->list);
	end = SortListEnd(que->list);
	iter = SortListFindIf(iter, end, DataCmp, param);

	if (!SortListIsSameIter(iter, end))
	{
		SortListRemove(iter);
	}
}
