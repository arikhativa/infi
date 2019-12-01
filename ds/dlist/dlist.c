
#include <stdlib.h> /* malloc() */
#include <string.h> /* memset() */
#include <assert.h> /* assert() */

#include "dlist.h"

#define START iter_src_start
#define END iter_src_end
#define DEST iter_dest

typedef struct dlist_node dlist_node_t;

struct dlist_node
{
	void *data;
	dlist_node_t *next;
	dlist_node_t *prev;
};

struct dlist
{
	dlist_node_t head;
	dlist_node_t tail;
};


dlist_t *DlistCreate(void)
{
	dlist_t *list = 0;

	list = (dlist_t*)calloc(1, sizeof(dlist_t));
	if (!list)
	{
		return NULL;
	}

	list->head.next = &list->tail;
	list->tail.prev = &list->head;

	return list;
}

void DlistDestroy(dlist_t *list)
{
	dlist_node_t *node = 0;
	dlist_node_t *stoper = 0;

	assert(list);

	node = list->head.next;
	stoper = &list->tail;

	while (node != stoper)
	{
		node = node->next;
		memset(node->prev, 0, sizeof(dlist_node_t));
		free(node->prev);
	}
	node = NULL;
	stoper = NULL;

	memset(list, 0, sizeof(dlist_t));
	free(list);
	list = NULL;
}

int DlistIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2)
{
	return (iter1 == iter2);
}

dlist_iter_t DlistEnd(dlist_t *list)
{
	assert(list);

	return &list->tail;
}

dlist_iter_t DlistBegin(dlist_t *list)
{
	assert(list);

	return list->head.next;
}

dlist_iter_t DlistNext(dlist_iter_t iter)
{
	if (iter->next == NULL)
	{
		return iter;
	}

	return iter->next;
}

dlist_iter_t DlistPrev(dlist_iter_t iter)
{
	if (iter->prev->prev == NULL)
	{
		return iter;
	}

	return iter->prev;
}

void *DlistGetData(dlist_iter_t iter)
{
	return iter->data;
}

static dlist_node_t *DlistCreateNode(void *data)
{
	dlist_node_t *new_node = 0;

	new_node = (dlist_node_t*)malloc(sizeof(dlist_node_t));
	if (!new_node)
	{
		return NULL;
	}

	new_node->data = data;

	return new_node;
}

dlist_iter_t DlistPushBack(dlist_t *list, void *data)
{
	assert(list);

	return DlistInsert(list, data, &list->tail);
}

dlist_iter_t DlistPushFront(dlist_t *list, void *data)
{
	assert(list);

	return DlistInsert(list, data, list->head.next);
}

int DlistIsEmpty(dlist_t *list)
{
	assert(list);

	return (list->head.next == &list->tail);
}

static int Count(void *data, void *param)
{
	++*(size_t*)param;
	(void)(data);

	return 0;
}

size_t DlistSize(const dlist_t *list)
{
	size_t count = 0;
	dlist_t *tmp = 0;

	assert(list);

	tmp = (dlist_t*)list;

	DlistForEach(list->head.next, &tmp->tail, Count, &count);

	return count;
}

static void DlistInsertBefore(dlist_node_t *node, dlist_node_t *new_node)
{
	new_node->next = node;
	new_node->prev = node->prev;
	node->prev->next = new_node;
	node->prev = new_node;
}

dlist_iter_t DlistInsert(dlist_t *list, void *data, dlist_iter_t iter)
{
	dlist_node_t *node = 0;

	assert(list);

	node = DlistCreateNode(data);
	if (!node)
	{
		return DlistEnd(list);
	}

	DlistInsertBefore(iter, node);

	return node;
}

static void DlistRemoveNode(dlist_node_t *node)
{
	assert(node);

	node->prev->next = node->next;
	node->next->prev = node->prev;

	memset(node, 0, sizeof(dlist_node_t));
	free(node);
	node = NULL;
}

void *DlistPopBack(dlist_t *list)
{
	void *data = 0;

	assert(list);

	if (DlistIsEmpty(list))
	{
		return 0;
	}

	data = DlistGetData(list->tail.prev);
	DlistRemoveNode(list->tail.prev);

	return data;
}

void *DlistPopFront(dlist_t *list)
{
	void *data = 0;

	assert(list);

	if (DlistIsEmpty(list))
	{
		return 0;
	}

	data = DlistGetData(list->head.next);
	DlistRemoveNode(list->head.next);

	return data;
}

dlist_iter_t DlistRemove(dlist_iter_t iter)
{
	dlist_node_t *after_iter = 0;

	if (!iter->next)
	{
		return iter;
	}

	after_iter = iter->next;

	DlistRemoveNode(iter);

	return after_iter;
}

dlist_iter_t DlistFind(dlist_iter_t begin, dlist_iter_t end, cmp_func_t compare_func, void *param)
{
        int status = 0;

        assert(compare_func);

        while (begin != end)
        {
                status = compare_func(begin->data, param);
                if (!status)
                {
                        return begin;
                }
                begin = begin->next;
        }

        return end;
}

int DlistForEach(dlist_iter_t begin, dlist_iter_t end, action_func_t action_func, void *param)
{
        int status = 0;

        assert(action_func);

        while (begin != end)
        {
                status = action_func(begin->data, param);
                if (status)
                {
                        return status;
                }
                begin = begin->next;
        }

        return 0;
}

dlist_iter_t DlistSplice(dlist_iter_t iter_dest, dlist_iter_t iter_src_start ,dlist_iter_t iter_src_end)
{
	dlist_node_t *ret = 0;

	ret = END->prev; /* Save the last node in the sublist */

	START->prev->next = END; /* Disconnect the sublist from its origenal list */
	END->prev = START->prev;

	START->prev = DEST->prev; /* Connect the bigining of the sublist to the iter before 'dest' */
	DEST->prev->next = START;

	DEST->prev = ret; /* Connect the end of the sublist to 'dest' */
	ret->next = DEST;

	return ret;
}
