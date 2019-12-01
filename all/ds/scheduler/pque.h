/*
 *	
 *   ver 1.2 changing the name and return vlaue of ismatch 
 *   ver 1.1
 *
 * 	file names : pque.c pque.h pque_test.c
 */

#ifndef __PQUE_H__
#define __PQUE_H__

#include <stddef.h> /* size_t */

typedef struct pque pque_t;

/*
 *	(*data_cmp_t)()
 *
 *	Return:
 *		'0' - If same.
 *		'1' - If not same.
 */
	typedef int (*data_cmp_t)(const void *data, void *param);

/*
 *	(*is_before_t)()
 *
 *	Return:
 *		'1' - If 'data1' is before 'data2'.
 *		'0' - If not.
 */
	typedef int (*is_priority_before_t)(const void *data1, const void *data2, void *param);

/****************** .C FILE *******************

struct pque
{
    sort_list_t *list;
    is_before_t IsBefore;
    void *param;        
};

***********************************************/

/*
 *	PqueCreate():                      O(1)
 *	Creates priority queue.
 *
 *	Arguments:
 *		'IsBefore' - Pointer to function that determen the priority.
 *		'param' - Paramenters for 'IsBefore'.
 *
 *	Return:
 *		Pointer to the new queue.
 */
	pque_t *PqueCreate(is_priority_before_t is_before, void *param);

/*
 *	PqueDestroy():                      O(1)
 *	Destroy a priority queue.
 *
 *	Arguments:
 *		'que' - The queue to destroy.
 */
	void PqueDestroy(pque_t *que);

/*
 *	PEnqueue():                      O(1)
 *	Addes element to the queue.
 *
 *	Arguments:
 *		'que' - The queue to add to.
 *		'data' - The data of the new element.
 *
 *	Return:
 *		'0' - On seccess.
 *		'-1' - On failure.
 */
	int PEnqueue(pque_t *que, void *data);

/*
 *	PDequeue():                      O(1)
 *	Removes an element from the queue.
 *
 *	Arguments:
 *		'que' - The queue to remove from.
 *
 */
	void PDequeue(pque_t *que);

/*
 *	PquePeek():                      O(1)
 *	Gets a pointer to the data of the first-in element.
 *
 *	Arguments:
 *		'que' - The queue to peek from.
 *
 *	Return:
 *		A pointer to the data.
 */
	void *PquePeek(pque_t *que);

/*
 *	PqueIsEmpty():                      O(1)
 *	Check if the que is empty.
 *
 *	Arguments:
 *		'que' - The queue to check.
 *
 *	Return:
 *		'1' - if the queue is empty.
 *		'0' - if the queue is NOT empty.
 */
	int PqueIsEmpty(pque_t *que);

/*
 *	PqueSize():                      O(n)
 *	Count the number of elements in the que.
 *
 *	Arguments:
 *		'que' - The queue to count.
 *
 *	Return:
 *		Number of elements.
 */
	size_t PqueSize(pque_t *que);

/*
 *	PqueClear():                      O(n)
 *	Removes all the elements in the queue.
 *	Does not free the queue, only the elements.
 *
 *	Arguments:
 *		'que' - The queue to clear.
 */
	void PqueClear(pque_t *que);

/*
 *	PqueErase():                      O(n)
 *	Find using 'data_cmp' the element to remove. (also frees it).
 *
 *	Arguments:
 *		'que' - The queue to search in.
 *		'data_cmp' - The function to use.
 *		'param' - parameters to use in data_cmp.
 */
	void PqueErase(pque_t *que, data_cmp_t data_cmp, void *param);

#endif /* __PQUE_H__ */
 
