/* 
 *  ver 1.8 Changed the name of a vr in Splice()
 *  ver 1.7 Return value of cmp_func have changed
 *  ver 1.6 One of the vars in IsSame() had a typo.
 *  ver 1.5 Return value of cmp func.
 *  ver 1.4 Change return on failure in Splice
 *  ver 1.3 Remove notes
 *  ver 1.2 Wrote definition of size
 * 	ver 1.1 initial release
 */

#ifndef __DLIST_H__
#define __DLIST_H__

#include <stddef.h> /* size_t */

typedef struct dlist dlist_t;
typedef struct dlist_node* dlist_iter_t;

/*
 *       (*action_func_t)():
 *
 *       Arguments:
 *               'data' - the data that the action would be applied to.
 *               'param' - user's parameters.
 *
 *       Return:
 *               On success retuns 0.
 */
	typedef int (*action_func_t)(void *data, void *param);

/*
 *       (*cmp_func_t)():
 *
 *       Arguments:
 *               'data' - the data to compare.
 *               'param' - user's parameters.
 *
 *       Return:
 *               On success retuns 0. 
 */
	typedef int (*cmp_func_t)(const void *data, void *param);

/*
 *       DlistCreate():
 *       Creates the Dlist
 *
 *       Return:
 *               On success retuns pointer to the Dlist.
 *               On filare NULL.
 */
	dlist_t *DlistCreate(void);

/*
 *       DlistDestroy():
 *       Frees the Dlist
 *
 *       Arguments:
 *       	'list' - the list to destroy.
 *
 */
	void DlistDestroy(dlist_t *list);

/*
 *       DlistSize():
 *       Get size of list
 *
 *       Arguments:
 *       	'list' - the list to destroy.
 *       
 *       Return:
 *          size of list. If empty or not exist return 0.
 */
	size_t DlistSize(const dlist_t *list);

/*
 *       DlistPushBack():
 *       Adds 'data' the the back of the list.
 *
 *       Arguments:
 *       	'list' - the list to add 'data' to.
 *       	'data' - pointer to the data.
 *
 *       Return:
 *               On success - returns an iter to the new element
 *               On filare  - returns DListEnd(list).
 */
	dlist_iter_t DlistPushBack(dlist_t *list, void *data);

/*
 *       DlistPushFront():
 *       Adds 'data' as a new element to the front of the list.
 *
 *       Arguments:
 *       	'list' - the list to add 'data' to.
 *       	'data' - pointer to the data.
 *
 *       Return:
 *               On success - returns iter to the new element.
 *               On filare  - returns iter to the end of the list.
 */
	dlist_iter_t DlistPushFront(dlist_t *list, void *data);

/*
 *       DlistInsert():
 *       Insert 'data' as a new element before 'iter'
 *
 *       Arguments:
 *       	'list' - the list that 'iter' is in.
 *       	'data' - pointer to the data.
 *       	'iter' - the iter that 'data' will be inserted before.
 *
 *       Return:
 *               On success - returns iter to the new element
 *               On filare  - returns iter to the end of the list.
 */
	dlist_iter_t DlistInsert(dlist_t *list, void *data, dlist_iter_t iter);

/*
 *       DlistPopback():
 *       Removes the last iter from the list.
 *
 *       Arguments:
 *       	'list' - the list that 'iter' is in.
 *
 *       Return:
 *       	the data in the iter.
 */
	void *DlistPopBack(dlist_t *list);

/*
 *       DlistPopFront():
 *       Removes the first iter from the list.
 *
 *       Arguments:
 *       	'list' - the list that 'iter' is in.
 *
 *       Return:
 *       	the data in the iter.
 */
	void *DlistPopFront(dlist_t *list);

/*
 *       DlistRemove():
 *       Removes iter from the list.
 *
 *       Arguments:
 *       	'iter' - the iter to remove.
 *
 *       Return:
 *               the iter after 'iter'.
 */
	dlist_iter_t DlistRemove(dlist_iter_t iter); 

/*
 *       DlistFind():
 *       Finds the the 'iter' using the compare_func();
 *
 *       Arguments:
 *       	'begin' - The iter to start from (inclusive)
 *       	'end' - The iter to stop (exclusive)
 *       	'compare_func' - A pointer to the function.
 *       	'param' - parameters for 'compare_func'
 *
 *       Return:
 *               if found - The matching iter.
 *               if not found  - The 'end' iter.
 */
	dlist_iter_t DlistFind(dlist_iter_t begin, dlist_iter_t end, cmp_func_t compare_func, void *param); 

/*
 *       DlistForEach():
 *       Apply action_func() on all the iters from 'begin'(inclusive) to 'end'(exclusive);
 *
 *       Arguments:
 *       	'begin' - The iter to start from (include it)
 *       	'end' - The iter to stop (does not include it)
 *       	'action_func' - A pointer to the function.
 *       	'param' - parameters for 'action_func'
 *
 *       Return:
 *               On success - 0.
 *               On filare  - the return value of action_func().
 */
	int DlistForEach(dlist_iter_t begin, dlist_iter_t end, action_func_t action_func, void *param);

/*
 *       DlistIsEmpty():
 *       Cheacks if the list is empty;
 *
 *       Arguments:
 *       	'list' - The list to cheack
 *
 *       Return:
 *               Empty - 1.
 *               Not empty - 0.
 */
	int DlistIsEmpty(dlist_t *list);

/*
 *       DlistSplice():
 *       Insert part of source list into destination list. The source elementes will be added before the destination element.
 *
 *       Arguments:
 *       	'iter_dest' - pointer to an element in the destination list
 *          'iter_src_start' - iter to the first element in the source list the will be added to the destination list
            'iter_src_end' - iter to the last element in the source list the will be added to the destination list. The end element will not be added

 *       Return:
 *               On success retuns iter to the last element that was added to the destination list .
 */
	dlist_iter_t DlistSplice(dlist_iter_t iter_dest, dlist_iter_t iter_src_start ,dlist_iter_t iter_src_end);

/*
 *       DlistNext():
 *       Returns iter to the element that is next to the element that was sent by the user
 *
 *       Arguments:
 *       	'iter' - pointer to an element in the list
 *
 *       Return:
 *               On success retuns iter to the next element in the list .
 *               On failure return the last iter.
 */
	dlist_iter_t DlistNext(dlist_iter_t iter);

/*
 *       DlistPrev():
 *       Returns iter to the element that is previous to the element that was sent by the user
 *
 *       Arguments:
 *       	'iter' - pointer to an element in the list
 *
 *       Return:
 *               On success retuns iter to the previous element in the list .
 *               On failure return the last iter.
 */
	dlist_iter_t DlistPrev(dlist_iter_t iter);

/*
 *       DlistEnd():
 *       Returns iter to the last element in the list.
 *
 *       Arguments:
 *       	'list' - pointer to the list
 *
 *       Return:
 *               On success retuns iter to the last element in the list .
 */
	dlist_iter_t DlistEnd(dlist_t *list);

/*
 *       DlistBegin():
 *       Returns iter to the first element in the list.
 *
 *       Arguments:
 *       	'list' - pointer to the list
 *
 *       Return:
 *               On success retuns iter to the first element in the list.
 */
	dlist_iter_t DlistBegin(dlist_t *list);

/*
 *       DlistIsSameIter():
 *       Check if to elements in the list are the same.
 *
 *       Arguments:
 *       	'iter1' - the first element in the list to compare
 *          'iter2' - the second element in the list to compare
 *
 *       Return:
 *               if true retuns 1.
 *               if false return 0.
 */
	int DlistIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2);

/*
 *       DlistGetData():
 *       Get data from the iter
 *
 *       Arguments:
 *       	'iter' - the element in the list to get the data from.
 *
 *       Return:ge
 *               On success retuns void pointer to the data.
 */
	void *DlistGetData(dlist_iter_t iter);

#endif /* __DLIST_H__ */

