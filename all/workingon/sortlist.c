
#include <stdlib.h> /* malloc() */
#include <string.h> /* memset() */
#include <assert.h> /* assert() */

#include "dlist.h"
#include "sortlist.h"

#define IS_BEFORE(dest, end, param) dest_list->IsBefore(DlistGetData(dest), DlistGetData(end), dest_list->param)

struct sort_list
{
    dlist_t *list;
    is_before_t IsBefore;
    void *param;	
};

sort_list_t *SortListCreate(is_before_t IsBefore, void *param)
{
	sort_list_t *list = 0;

	assert(IsBefore);

	list = (sort_list_t*)malloc(sizeof(sort_list_t));
	if (!list)
	{
		return NULL;
	}

	list->list = DlistCreate();
	if (!list->list)
	{
		free(list);
		return NULL;
	}

	list->IsBefore = IsBefore;
	list->param = param;

	return list;
}

void SortListDestroy(sort_list_t *list)
{
	assert(list);

	DlistDestroy(list->list);
	memset(list, 0, sizeof(sort_list_t));
	free(list);
	list = NULL;
}

sortedl_iter_t SortListInsert(sort_list_t *list, void *data)
{
	sortedl_iter_t iter = { 0 };
	dlist_iter_t end = 0;

	assert(list);

	iter.iter = DlistBegin(list->list);
	end = DlistEnd(list->list);

	while (end != iter.iter)
	{
		if (!list->IsBefore(DlistGetData(iter.iter), data, list->param))
		{
			break ;
		}
		iter.iter = DlistNext(iter.iter);
	}

	iter.iter = DlistInsert(list->list, data, iter.iter);

	return iter;
}

sortedl_iter_t SortListBegin(sort_list_t *list)
{
	sortedl_iter_t iter = { 0 };

	assert(list);

	iter.iter = DlistBegin(list->list);

	return iter;
}

sortedl_iter_t SortListEnd(sort_list_t *list)
{
	sortedl_iter_t iter = { 0 };

	assert(list);

	iter.iter = DlistEnd(list->list);

	return iter;
}

int SortListIsSameIter(const sortedl_iter_t iter1, const sortedl_iter_t iter2)
{
	return DlistIsSameIter(iter1.iter, iter2.iter);
}

sortedl_iter_t SortListNext(sortedl_iter_t iter)
{
	iter.iter = DlistNext(iter.iter);

	return iter;
}

sortedl_iter_t SortListPrev(sortedl_iter_t iter)
{
	iter.iter = DlistPrev(iter.iter);

	return iter;
}

void *SortListGetData(sortedl_iter_t iter)
{
	return DlistGetData(iter.iter);
}

int SortListIsEmpty(const sort_list_t *list)
{
	assert(list);

	return DlistIsEmpty(list->list);
}

size_t SortListSize(const sort_list_t *list)
{
	sort_list_t *tmp = 0;

	assert(list);

	tmp = (sort_list_t*)list;

	return DlistSize(tmp->list);
}

void *SortListPopBack(sort_list_t *list)
{
	assert(list);

	return DlistPopBack(list->list);
}

void *SortListPopFront(sort_list_t *list)
{
	assert(list);

	return DlistPopFront(list->list);
}

sortedl_iter_t SortListRemove(sortedl_iter_t iter)
{
	sortedl_iter_t ret = { 0 };

	ret.iter = DlistRemove(iter.iter);

	return ret;
}

int SortListForEach(sortedl_iter_t start, sortedl_iter_t end, action_func_t ActionFunc, void *param)
{
	assert(ActionFunc);

	return DlistForEach(start.iter, end.iter, ActionFunc, param); 
}

sortedl_iter_t SortListFindIf(sortedl_iter_t start, sortedl_iter_t end, cmp_func_t CmpFunc, void *param)
{
	sortedl_iter_t ret = { 0 };

	assert(CmpFunc);

	ret.iter = DlistFind(start.iter, end.iter, CmpFunc, param); 

	return ret;
}

sortedl_iter_t SortListFind(sort_list_t *list,  sortedl_iter_t start, sortedl_iter_t end, void *data)
{
	assert(list);

	while (start.iter != end.iter && (list->IsBefore(DlistGetData(start.iter), data, list->param)))
	{
		start.iter = DlistNext(start.iter);
	}

	if (!list->IsBefore(data, DlistGetData(start.iter), list->param))
	{
		return start;
	}

	return end;
}

sort_list_t *SortListMerge(sort_list_t *dest_list, sort_list_t *src_list)
{
	sortedl_iter_t start;
	sortedl_iter_t end;
	sortedl_iter_t dest;
	sortedl_iter_t src_stoper;
	sortedl_iter_t dest_stoper;

	assert(dest_list);
	assert(src_list);

	src_stoper.iter = DlistEnd(src_list->list);
	dest_stoper.iter = DlistEnd(dest_list->list);
	dest.iter = DlistBegin(dest_list->list);
	start.iter = DlistBegin(src_list->list);
	end.iter = start.iter;

	if (DlistIsEmpty(src_list->list))
	{
		return dest_list;
	}

	while (start.iter != src_stoper.iter)		/* Stop the loop when src_list is empty */
	{
		while (dest.iter != dest_stoper.iter && (IS_BEFORE(dest.iter, end.iter, param))) /* Loop to find the currect dest for splice */
		{
			dest.iter = DlistNext(dest.iter);
		}
		if (dest.iter == dest_stoper.iter)	/* If dest.iter is deat->tail, then all the rest of src will be added */
		{
			break ;
		}
		while (end.iter != src_stoper.iter && (IS_BEFORE(end.iter, dest.iter, param)))		/* Loop to find the correct 'end' for splice */
		{
			end.iter = DlistNext(end.iter);
		}

		DlistSplice(dest.iter, start.iter, end.iter);

		start.iter = end.iter;
	}

	if (dest.iter == dest_stoper.iter)	/* If dest.iter is deat->tail, then all the rest of src will be added */
	{
		DlistSplice(dest.iter, start.iter, DlistEnd(src_list->list));
	}

	return dest_list;
}
