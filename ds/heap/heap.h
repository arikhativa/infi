/*
 *  ver 1.5 add const to HeapPeek.
 *  ver 1.4 add void *param to hea_cmp_compare_t.
 *  ver 1.3 changed the signature of HeapRemove (returns void)
 *  ver 1.2 changed the signature of HeapPop (removed 'const void *data') and added description for 'suggested size' in HeapCreate.
 * 	ver 1.1
 */

#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h> /*size_t*/

typedef struct heap heap_t;

/*
 *      heap_cmp_func_t:	O(1)
 *      Compare between the user data and the data in the tree.
 *
 *      Arguments:
 *              'usr_data' - the input data from the user.
 *              'heap_data' - the data in the tree.
 *              'param' - extra parametrs.
 *
 *      Return:
 *              '1' if the usr_data is 'larger' than  the compared heap data.
 *              '-1' if the usr_data is 'smaller' than  the compared heap data.
 *              '0' - if equal.
 */
typedef int (*heap_cmp_func_t)(const void *usr_data, const void *heap_data, void *param);

/*
 *      heap_rm_func_t:	O(1)
 *      Compare between the user data and the data in the heap.
 *      if match. will remove.
 *
 *      Arguments:
 *              'heap_data' - the data in the heap.
 *              'param' - extra parametrs.
 *
 *      Return:
 *              '0' - if match.
 *              '1' if not.
 */
typedef int (*heap_rm_func_t)(const void *heap_data, void *param);

/*
   in c file
#include "vector.h"

struct heap
{
heap_cmp_func_t fp_cmp;
void *param;
vector_t *vector;
};
*/

/*
 *	HeapCreate:	O(1)
 *	Create a new empty heap_tree.
 *
 *	Arguments:
 *		'cmp' - user compare function to orginize data.
 *      'suggested_size' - suggested size of heap.
 *      'param' - extra parametrs.
 *
 *	Return:
 *		New empty heap.
 *		On failure return NULL.
 */
heap_t *HeapCreate(heap_cmp_func_t cmp, size_t suggested_size, void *param);

/*
 *	HeapDestroy:	O(n)
 *	Clean the Heap.
 *
 *	Arguments:
 *		'heap' - heap to destroy.
 */
void HeapDestroy(heap_t *heap);

/*
 *      HeapPush()     O(log(n))
 *      Push 'data' to the heap.
 *
 *      Arguments:
 *              'heap' - handle to the heap.
 *              'data' - Pointer to the new data.
 *
 *      Return:
 *              '0' - On Success.
 *              '1' - Allocaion fail.
 */
int HeapPush(heap_t *heap, const void *data);

/*
 *      HeapPop()     O(log(n))
 *      Remove the root (top of the heap).
 *
 *      Arguments:
 *              'heap' - handle to the heap.
 *
 *      Return:
 *      	the data that was removed.
 *      	if the heap empty returns NULL.
 */
void *HeapPop(heap_t *heap);

/*
 *      HeapRemove()     O(n)
 *      Remove a spacific matching data.
 *      Compare with cmp.
 *
 *      Arguments:
 *              'heap' - handle to the heap.
 *              'RmFunc' - Pointer to function.
 *              'param' - Parameters to use in RmFunc.
 *
 *      Return:
 *              void	
 */
void HeapRemove(heap_t *heap, heap_rm_func_t RmFunc, void *param);

/*
 * 	HeapPeek()	O(1)
 * 		Peeks and returns pointer to data at the top of the heap (root).
 *
 * 	Arguments:
 *		'heap' - handle to the heap.
 *
 *	return:
 *		Data of root.
 *		NULL, if heap empty.
 *	
 */
void *HeapPeek(const heap_t *heap);

/*
 *	HeapIsEmpty	O(1)
 * 
 *	Check if the heap is empty.
 *
 *	Argument:
 * 		heap - handle to the heap.
 *
 *	Return:
 *		'1' - in case its empty.
 *		'0' - in case its doesnt.
 */
int HeapIsEmpty(const heap_t *heap);

/*
 *	Size: O(1)
 *		return the size of heap
 *
 *	Argumentd:
 *		heap - handle to the heap.
 *
 *	Return:
 *		the size of heap.
 */
size_t HeapSize(const heap_t *heap);

#endif /* __HEAP_H__ */

