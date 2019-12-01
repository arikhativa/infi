/*
 * 	var 1.1
 */

#ifndef __SafeQueue_H__
#define __SafeQueue_H__

typedef struct queue queue_t;

/*
 *	SafeQueueCreate():      O(1)
 *	Create a queue.
 *
 *	Return:
 *		A pointer to the new queue.
 */
	queue_t *SafeQueueCreate(void);

/*
 *	QueueDestroy():     O(n)
 *	Frees the queue.
 *
 *	Arguments:
 *		'queue' - a pointer to the queue.
 *
 *	Return:
 *		None.
 */
	void SafeQueueDestroy(queue_t *queue);

/*
*   SQEnqueue():     O(1)
*   Adds node to end of queue.
*
*   Arguments:
*       'queue' - Queue to add to.
*       'data'  - Data to be added to queue.
*
*   Return:
*       Success (0) or failure(1).
*/
    int SQEnqueue(queue_t *queue, void *data);

/*
*   SQDequeue():     O(1)
*   Remove first node in queue.
*
*   Arguments:
*       'queue' - Queue to be checked.
*
*   Return:
*       None.
*/
    void *SQDequeue(queue_t *queue);

/*
*   IsQueueEmpty():     O(1)
*   Find if queue is empty.
*
*   Arguments:
*       'queue' - Queue to be checked.
*
*   Return:
*       True (1) if empty, or false (0).
*/
    int IsSafeQueueEmpty(const queue_t *queue);

/*
*   QueueSize():      O(n)
*   Find the size of a queue.
*
*   Arguments:
*       'queue' - queue to be checked.
*
*   Return:
*       num of nodes in queue.
*/
    size_t SafeQueueSize(const queue_t *queue);

#endif /* __SafeQueue_H__ */
