/*
 *  ver 1.3 Added signature of cmp func in FindIf and changed name of argument param to data in Find.
 *	ver 1.2 After comment corrections made
 *  ver 1.1 initial release
*/

/*
 *  file names : sortlist.c sortlist.h sortlist_test.c
*/

#ifndef __SORTED_LIST_H__
#define __SORTED_LIST_H__


#include <stddef.h> /* size_t */
#include "dlist.h" /*header file for Dlist function, definition of dlist_iter_t */


typedef struct sort_list sort_list_t;

/* Returns 1  if data1 is before data2, else 0 */
typedef int (*is_before_t)(const void *data1, const void *data2, void *param);

typedef struct sorted_list_iter
{
    dlist_iter_t iter;
}sortedl_iter_t;

/*	*** Write In C flie! **
struct sort_list
{
    dlist_t *dlist;
    is_before_t IsBefore;
    void *param;	
};
*/

/*
 *       SortListCreate():			O(1)
 *       Creates the sorted list
 *       
 *		Arguments:
 *		IsBefore: function that finds if data1 is before data2. 
 *		param - criterion for sorting the list. 
 *
 *       Return:
 *           On success returns pointer to the sorted list. 
 *           On faliure NULL.
 */
sort_list_t *SortListCreate(is_before_t IsBefore, void *param);

/*
 *       SortListDestroy():			O(n)
 *       Frees the sorted list.
 *
 *       Arguments:
 *       	'solist' - the list to destroy.
 *
 */
void SortListDestroy(sort_list_t *solist);

/*
 *       SortListSize():			O(n)			
 *       Get size of list
 *
 *       Arguments:
 *       	'solist' - pointer to list.
 *       
 *       Return:
 *           number of elements in list. If empty or does not exist return 0.
 */
size_t SortListSize(const sort_list_t *solist);

/*
 *       SortListIsEmpty():			O(1)
 *       Checks if the list is empty;
 *
 *       Arguments:
 *       	'solist' - pointer to The list.
 *
 *       Return:
 *          Empty - 1. Otherwise - 0.
 */
int SortListIsEmpty(const sort_list_t *solist);

/*
 *       SortListBegin():			O(1)
 *       Returns iter of the first element in the list.
 *
 *       Arguments:
 *       	'solist' - pointer to the list.
 *
 *       Return:
 *          Returns iter of the first element in the list.
 *			If Empty Returns end iter.
 */
sortedl_iter_t SortListBegin(sort_list_t *solist);

/*
 *       SortListEnd():				O(1)
 *       Returns iter of the last element in the list.
 *
 *       Arguments:
 *       	'solist' - pointer to the list.
 *
 *       Return:
 *          On success returns iter after last element in the list (a.k.a 'end iter').
 */
sortedl_iter_t SortListEnd(sort_list_t *solist);

/*
 *       SortListNext():			O(1)
 *       Returns iter to next element on the list.
 *
 *       Arguments:
 *       	'iter' - struct of element in the list
 *
 *       Return:
 *          On success - iter of the next element in the list.
 *          Failure is when out of range - return end iter.
 */
sortedl_iter_t SortListNext(sortedl_iter_t soiter);

/*
 *       SortListPrev():			O(1)
 *       Returns iter of the element that is previous to the element that was sent by the user
 *
 *       Arguments:
 *       	'iter' - struct of an element in the list.
 *
 *       Return:
 *          On success returns iter of the previous element in the list .
 *          Failure is when out of range - return first element. If empty return end iter.
 */
sortedl_iter_t SortListPrev(sortedl_iter_t soiter);

/*
 *       SortListIsSameIter():		O(1)
 *       Check if to elements in the list are the same.
 *
 *       Arguments:
 *       	'iter1' - the first element in the list to compare.
 *          'iter2' - the second element in the list to compare.
 *
 *       Return:
 *          True returns 1. Otherwise 0.
 */
int SortListIsSameIter(const sortedl_iter_t iter1, const sortedl_iter_t iter2);

/*
 *       SortListGetData():			O(1)
 *       Get data from the iter
 *
 *       Arguments:
 *       	'iter' - the element in the list to get the data from.
 *
 *       Return:
 *			On success returns a void pointer to the element's data. 
 */
void *SortListGetData(sortedl_iter_t iter);

/*
 *       SortListInsert():			O(1)
 *       Insert 'data' to the sorted list at the right place. 
 *
 *       Arguments:
 *       	'solist' - the list that you want to create the element in. using the given data. 
 *       	'data' - pointer to the data.
 *    
 *
 *       Return:
 *          On success - returns iter of the new element
 *          On faliure  - returns iter of the end of the list.
 */
sortedl_iter_t SortListInsert(sort_list_t *solist, void *data);

/*
 *       SortListRemove():			O(1)
 *       Removes iter from the list.
 *
 *       Arguments:
 *       	'iter' - the iter to remove.
 *
 *       Return:
 *          The iter after 'iter'. If last element removed - end iter.
 */
sortedl_iter_t SortListRemove(sortedl_iter_t iter);

/*
 *       SortListPopFront():		O(1)
 *       Removes the first iter from the list.
 *
 *       Arguments:
 *       	'solist' - the list that 'iter' is in.
 *
 *       Return:
 *       	the data in the iter.
 */
void *SortListPopFront(sort_list_t *solist);

/*
 *       SortListPopBack():			O(1)
 *       Removes the last iter from the list.
 *
 *       Arguments:
 *       	'solist' - the list that 'iter' is in.
 *
 *       Return:
 *       	the data in the iter.
 */
void *SortListPopBack(sort_list_t *solist);

/*
 *       DlistForEach():			O(n)
 *       Apply action_func() on all the iters from 'begin'(inclusive) to 'end'(exclusive);
 *
 *       Arguments:
 *       	'begin' - The iter to start from (include it)
 *       	'end' - The iter to stop (does not include it)
 *       	'action_func' - A pointer to the function.
 *       	'param' - parameters for 'action_func'
 *
 *       Return:
 *           On success - 0.
 *           On faliure  - the return value of action_func().
 */
int SortListForEach(sortedl_iter_t start, sortedl_iter_t end, action_func_t ActioFunc, void *param);

/*
 *       SortListMerge():			O(n)
 *    	  Merges two sorted lists that are sorted according to the 'dest_list' key.
 *
 *       Arguments:
 *       	'dest_list' - the list src_list will be merged into.
 *			'src_list' - the list to be merged into dest_list.
 *
 *       Return:
 *           On success returns pointer to dest_list.
 *			 On faliure returns pointer to the src_list. 
 */
sort_list_t *SortListMerge(sort_list_t *dest_list, sort_list_t *src_list);

/*
 *       SortListFind(): 			O(n)
 *       Finds the 'iter' that holds the searched data, using the func IsBefore();
 *
 *       Arguments:
 *			'solist' - pointer to the list.
 *       	'begin' - The iter to start from (inclusive).
 *       	'end' - The iter to stop (exclusive).
 *       	'data' - data to find.
 *
 *       Return:
 *           Found - matching iter.
 *           Not found  - 'end' iter.
 */
sortedl_iter_t SortListFind(sort_list_t *solist, sortedl_iter_t start, sortedl_iter_t end, void *data);

/*
 *       SortListFindIf():			O(n)
 *       Finds the the 'iter' that holds the searched data, using the compare_func();
 *
 *       Arguments:
 *       	'begin' - The iter to start from (inclusive)
 *       	'end' - The iter to stop (exclusive)
 *       	'compare_func' - A pointer to the function. - function signature int(cmp_func)(void *data, void *param).
 *       	'param' - parameters for 'compare_func'
 *
 *       Return:
 *           Found - matching iter.
 *           Not found  - 'end' iter.
 */
sortedl_iter_t SortListFindIf(sortedl_iter_t start, sortedl_iter_t end, cmp_func_t CmpFunc, void *param);

#endif /* __SORTED_LIST_H__ */
