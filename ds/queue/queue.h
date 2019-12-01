/*
 * 	var 1.1
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct queue queue_t;

/*
 *	QueueCreate():      O(1)
 *	Create a queue.
 *	
 *	Return:
 *		A pointer to the new queue.
 */ 
	queue_t *QueueCreate(void);

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
	void QueueDestroy(queue_t *queue);

/*
*   QEnqueue():     O(1)
*   Adds node to end of queue.
*
*   Arguments:
*       'queue' - Queue to add to.
*       'data'  - Data to be added to queue.
*
*   Return:
*       Success (0) or failure(1).
*/
    int QEnqueue(queue_t *queue, void *data);

/*
*   QDequeue():     O(1)
*   Remove first node in queue.
*
*   Arguments:
*       'queue' - Queue to be checked.
*
*   Return:
*       None.
*/
    void QDequeue(queue_t *queue);

/*
*   QueuePeek():    O(1)
*   Get data from first node in queue.
*
*   Arguments:
*       'queue' - Queue to be checked.
*
*   Return:
*       Pointer to data of first in queue.
*/
   void *QueuePeek(const queue_t *queue);

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
    int IsQueueEmpty(const queue_t *queue);

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
    size_t QueueSize(const queue_t *queue);

/*
*   QueueAppend():      O(1)
*   Puts src queue as continuens of dest queue.
*
*   Arguments:
*       'dest' - destenation queue.
*       'src' - queue to be add to destenation.
*
*   Return:
*       dest pointer. If fail return NULL. src queue is left empty.
*/
    queue_t *QueueAppend(queue_t *dest, queue_t *src);

#endif /* __QUEUE_H__ */

