

#include <assert.h> /* assert() */
#include <stdlib.h> /* malloc() */
#include <string.h> /* memset() */


#include "queue.h"
#include "slist.h"

struct queue
{
	slist_node_t *head;
	slist_node_t *tail;
};

typedef enum status
{
	FAIL = -1,
	SUCCESS = 0
} status_t;

typedef enum boolean
{
	FALSE = 0,
	TRUE = 1
} bool_t;

queue_t *QueueCreate(void)
{
	queue_t *new_queue = 0;
	slist_node_t *dummy = 0;

	new_queue = (queue_t*)malloc(sizeof(queue_t));
	if (!(new_queue))
	{
		return NULL;
	}

	dummy = SlistCreateNode(NULL, NULL);
	if (!(dummy))
	{
		free(new_queue);
		return NULL;
	}

	new_queue->head = dummy;
	new_queue->tail = dummy;

	return new_queue;
}

void QueueDestroy(queue_t *queue)
{
	assert(queue);

	SlistFreeAll(queue->head);

	memset(queue, 0, sizeof(queue_t));

	free(queue);
	queue = NULL;
}

int QEnqueue(queue_t *queue, void *data)
{
	slist_node_t *node = 0;

	assert(queue);

	node = SlistCreateNode(data, NULL);
	if (!(node))
	{
		return FAIL;
	}

	queue->tail = SlistInsertAfter(queue->tail, node);

	return SUCCESS;
}

void QDequeue(queue_t *queue)
{
	slist_node_t *removed_node = 0;

	assert(queue);

	removed_node = SlistRemoveAfter(queue->head);

	if (NULL == queue->head->next)
	{
		queue->tail = queue->head;
	}
	if (removed_node)
	{
		removed_node->next = NULL;
		SlistFreeAll(removed_node);
		removed_node = NULL;
	}
}

void *QueuePeek(const queue_t *queue)
{
	assert(queue);

	if (IsQueueEmpty(queue))
	{
		return NULL;
	}

	return queue->head->next->data;
}

int IsQueueEmpty(const queue_t *queue)
{
	assert(queue);

	return (queue->head == queue->tail);
}

size_t QueueSize(const queue_t *queue)
{
	assert(queue);

	if (IsQueueEmpty(queue))
	{
		return 0;
	}

	return (SlistCount(queue->head->next));
}

queue_t *QueueAppend(queue_t *dest, queue_t *src)
{
	assert(dest);
	assert(src);

	if (IsQueueEmpty(dest) || IsQueueEmpty(src))
	{
		return NULL;
	}

	dest->tail->next = src->head->next;
	dest->tail = src->tail;

	src->head->next = NULL;
	QueueDestroy(src);
	src = NULL;

	return dest;
}
