
#include <assert.h> /* assert() */
#include <stdlib.h> /* malloc() */
#include <string.h> /* memset() */
#include <pthread.h>	/* pthread_create(), pthread_join() */

#include "safe_queue.h"
#include "slist.h"

struct queue
{
	slist_node_t *head;
	slist_node_t *tail;
	volatile int is_empty;
};

typedef enum status
{
	FAIL = -1,
	SUCCESS = 0
} status_t;

enum is_empty
{
	EMPTY = 0,
	NOT_EMPTY
} ;

/*
	Global var:
*/
static pthread_mutex_t g_door = PTHREAD_MUTEX_INITIALIZER;

queue_t *SafeQueueCreate(void)
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
	new_queue->is_empty = EMPTY;

	return new_queue;
}

void SafeQueueDestroy(queue_t *queue)
{
	assert(queue);

	pthread_mutex_lock(&g_door);

	SlistFreeAll(queue->head);

	memset(queue, 0, sizeof(queue_t));

	free(queue);
	queue = NULL;

	pthread_mutex_unlock(&g_door);
}

int SQEnqueue(queue_t *queue, void *data)
{
	slist_node_t *node = 0;

	assert(queue);

	node = SlistCreateNode(data, NULL);
	if (!(node))
	{
		return FAIL;
	}

	pthread_mutex_lock(&g_door);

	if (queue->head == queue->tail)
	{
		queue->is_empty = NOT_EMPTY;
	}
	queue->tail = SlistInsertAfter(queue->tail, node);

	pthread_mutex_unlock(&g_door);

	return SUCCESS;
}

static int CheckAndSetEmpty(queue_t *queue)
{
	assert(queue);

	return (__sync_bool_compare_and_swap(&queue->is_empty, NOT_EMPTY, EMPTY));
}

static void *SafeQueuePeek(queue_t *queue)
{
	assert(queue);

	while (!CheckAndSetEmpty(queue))
	{}

	pthread_mutex_lock(&g_door);

	return queue->head->next->data;
}

void *SQDequeue(queue_t *queue)
{
	slist_node_t *removed_node = 0;
	void *data = NULL;

	assert(queue);

	data = SafeQueuePeek(queue);

	removed_node = SlistRemoveAfter(queue->head);

	if (NULL == queue->head->next)
	{
		queue->tail = queue->head;
		queue->is_empty = EMPTY;
	}
	else
	{
		queue->is_empty = NOT_EMPTY;
	}

	pthread_mutex_unlock(&g_door);

	if (removed_node)
	{
		removed_node->next = NULL;
		SlistFreeAll(removed_node);
		removed_node = NULL;
	}

	return data;
}

int IsSafeQueueEmpty(const queue_t *queue)
{
	assert(queue);

	return (queue->head == queue->tail || queue->head == NULL);
}

size_t SafeQueueSize(const queue_t *queue)
{
	size_t count = 0;

	assert(queue);

	if (IsSafeQueueEmpty(queue))
	{
		return 0;
	}

	pthread_mutex_lock(&g_door);

	count = SlistCount(queue->head->next);

	pthread_mutex_unlock(&g_door);

	return count;
}
