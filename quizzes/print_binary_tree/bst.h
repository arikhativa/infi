/*
*
* ver 1.2 added comment to remove.
* ver 1.1 added comment to insert.
* ver 1.0 initial release
*
*/

#ifndef __BST_H__
#define __BST_H__

#include <stddef.h> /* size_t */

typedef struct bst_node bst_iter_t;
typedef struct bst bst_t;

/*
*	policy_func_t()        O(1)
*	Decides if the data should go to the left or right child.
*
*	Arguments:
*		'tree_data' - The data on the tree.
*		'user_data' - The data that the user gave.
*		'param' - Extra parameters for the function.
*
*	Return:
*		A negative number - If the data should got to the LEFT child.
*		A positive number - If the data should got to the RIGHT child.
*       '0' - If equal.
*/

typedef int (*policy_func_t)(const void *tree_data, const void *user_data, void *param);
typedef int (*act_func_t)(void *data, void *param);

/*
in c source file

typedef struct bst_node bst_node_t;

struct bst
{
	bst_node_t *root;
    bst_node_t dummy;
	policy_func_t BstCmp;
	void *param;
};


struct bst_node
{
	bst_node_t *left_child;
	bst_node_t *right_child;
	bst_node_t *parent;
	void *data;
};

*/


/*
*	BSTCreate()     O(1)
*	Creates the bst.
*
*	Arguments:
*		'BstCmp' - A function thats desides if the data should go to the left or right child.
*		'param' - Parametres for the function.
*
*	Return:
*		On Seccess - Pointer to the bst.
*		On Fail - NULL;
*/
	bst_t *BSTCreate(policy_func_t BstCmp, void* param); 

/*
*	BSTDestroy()    O(n)
*	Destroys the bst (post order).
*
*	Arguments:
*		'bst' - A pointer to the bst.
*/
	void BSTDestroy(bst_t *bst);

/*	BSTSize():	O(n)
*	Counts num of nodes exist in tree. 
*
*	Arguments:
*		'bst' - tree to count its size.
*
*	Return:
*		Size of tree.
*/
	size_t BSTSize(const bst_t *bst);

/*	BSTIsEmpty():	O(1)
*	Check If bst is empty.
*
*	Arguments:
*		'bst' - tree to check.
*
*	Return:
*		Empty (true) = 1
*		Not empty = 0
*/
	int BSTIsEmpty(const bst_t *bst);
	
/*
*	BSTInsert():   O(log(n))    -   (If the bst is not balanced it could be  O(n))
*	Create new iter with the given data, and insert the iter in the right place.
*   BSTInsert has undefined behavior when inserting the same data.
*
*	Arguments:
*		'bst' - a pointer to the bst. 
*		'data' - the data to insert.
*
*	Return:
*		The inserted iter.
*       If fails return end iter.   
*/
	bst_iter_t* BSTInsert(bst_t *bst, void *data);

/*
*	BSTRemove():  O(1)   
*	Removes the given iter from bst.
*   After using BSTRemove() the bst might change, and the iters are not valid any more. 
*
*	Arguments:
*		'iter' - the iter to remove.
*   
*/
	void BSTRemove(bst_iter_t* iter);
	
/*
*	BSTBegin():   O(log(n))
*	Returns the iter with smallest data.
*
*	Arguments:
*		'usr_bst' - pointer to the tree.
*		
*	Return:
*		pointer to the iter with with the smallest data.
*		end iter - if the tree is empty.
*/
	bst_iter_t *BSTBegin(const bst_t* bst);

	
/*
*	BSTEnd():   O(1)     
*	Returns the iter after the largest iter.
*
*	Arguments:
*		'usr_bst' - pointer to the tree.
*		
*	Return:
*		'end iter'.
*/
	bst_iter_t *BSTEnd(const bst_t* bst);
    
/*
*	BSTNext():   O(1)
*	Returns the parent iter.
*
*	Arguments:
*		'iter' - pointer to the iter.
*	
*		
*	Return:
*		pointer to the parent iter.
*		end iter - if the BTS is empty.
*/
	bst_iter_t* BSTNext(bst_iter_t* iter);

/*
*	BSTPrev(): O(1)
*	Returns iter to the element that is previous in order to the element was sent by the user
*
*	Arguments:
*		'iter' - pointer to an element in the bst
*
*	Return:
*		On success return iter to the previous element in the bst .
*		On failure return end iter.
*/
	bst_iter_t* BSTPrev(bst_iter_t* iter);

/*
*	BSTIsSameIter(): O(1)
*	Check if two elements in the bst are the same.
*
*	Arguments:
*       	'iter1' - the first element in the bst to compare
*          'iter2' - the second element in the bst to compare
*
*       Return:
*               if true retuns 1.
*               if false return 0.
*/
int BSTIsSameIter(const bst_iter_t* iter_a, const bst_iter_t* iter_b);

/*
*	BSTGetData():
*	get pointer to data found in iter
*
*	Arguments:
*	'iter' - the iter to get the data from. 
*
*	Return:
*	pointer to data.
*/
void *BSTGetData(const bst_iter_t* iter);

/*
*       bstFind(): log(n)
*       Finds the the 'iter' using the compare_func();
*
*       Arguments:
*			'bst' - the bst.
*			'data' - key to comparation
*       	'compare_func' - A pointer to the function.
*       	'param' - parameters for 'compare_func'
*
*       Return:
*               if found - The matching iter.
*               On failure return end iter.
*/
bst_iter_t* BSTFind(bst_t* bst, void *data);

/*
*	BSTForEach():   O(n)
*	 Apply act_func_t() on all the iters.
*   If fails, stops and return the function's value.
*
*	Arguments:
*		'start' - A pointer to start of action to be done.
*		'end' - A pointer to end of action to be done.
*		'ForEach' - A pointer to the function.
*		'param' - parameters for 'action_func'
*
*	Return:
*		On success - 0.
*      On faliure  - the return value of ForEach.
*/
int BSTForEach(bst_iter_t* start, bst_iter_t* stop, act_func_t ForEach, void *param);
	
#endif
