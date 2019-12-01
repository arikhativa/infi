
#include <assert.h>		/* assert() */
#include <stdlib.h> /* malloc() */
#include <string.h> /* memset() */
#include <pthread.h>	/* pthread_create(), pthread_join() */
#include <semaphore.h>

#include "safe_queue2.h"
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
static sem_t g_stock;

queue_t *SafeQueueCreate(void)
{
	queue_t *new_queue = 0;
	slist_node_t *dummy = 0;

	if (sem_init(&g_stock, 0, 0))
	{
		return NULL;
	}

	new_queue = (queue_t*)malloc(sizeof(queue_t));
	if (!(new_queue))
	{
		sem_destroy(&g_stock);
		return NULL;
	}

	dummy = SlistCreateNode(NULL, NULL);
	if (!(dummy))
	{
		sem_destroy(&g_stock);
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

	sem_destroy(&g_stock);

	SlistFreeAll(queue->head);

	memset(queue, 0, sizeof(queue_t));

	free(queue);
	queue = NULL;

	pthread_mutex_unlock(&g_door); /* do i need to unlock? */
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

	sem_post(&g_stock);

	pthread_mutex_unlock(&g_door);

	return SUCCESS;
}

static void *SafeQueuePeek(queue_t *queue)
{
	assert(queue);

	sem_wait(&g_stock);

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
		queue->is_empty = EMPTY; /* maybe dont need this */
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
