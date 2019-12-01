/*
	ver 1.1
	ver 1.2 changing signature of cmp_func 
	
*/
#ifndef __AVL_H__
#define __AVL_H__

#include <stddef.h> /*size_t*/

#include "dlist.h"


/*
 *	avl_cmp_func_t:	O(1)
 *	Compare between the user data and the data in the tree.
 *
 *	Arguments:
 *		'usr_data' - the input data from the user.
 *		'tree_data' - the data in the tree.
 *
 *	Return:
 *		'1' if the usr_data is 'larger' than  the compared tree data.
 *		'-1' if the usr_data is 'smaller' than  the compared tree data.
 *		'0' - if equal.
*/	
typedef int (*avl_cmp_func_t)(const void *usr_data, const void *tree_data);
/*
 *	cmp_func:	O(1)
 *	Perform a certain actions on the data.
 *
 *	Arguments:
 *		'data' - Pointer to the data we want to modify.
 *		'param' - parameters to the function, if needed.
 *
 *	Return:
 *		'0' - On Success.
 *       else - On failure.
*/
typedef int (*act_func_t)(void *data, void *param);

typedef struct avl avl_t;

/*
 *	AVLCreate:	O(1)
 *	Create a new empty avl_tree.
 *
 *	Arguments:
 *		'CMP' - user compare function to orginize data.
 *		
 *
 *	Return:
 *		New empty avl_tree.
 *		On failure return NULL.
*/
avl_t *AVLCreate(avl_cmp_func_t CMP);
/*
 *	AVLDestroy:	O(n)
 *	Clean and free all nodes in tree, post-order(Left,Right,Root).
 *
 *	Arguments:
 *		'avl' - tree to destroy.
 *
 *	Return:
 *		None.
*/
void AVLDestroy(avl_t *avl);
/*
 *      AVLInsert()     O(log(n))
 *      Inserts 'data' to the avl tree using the compare_func_t.
 *      Duplicate 'data' is undefind behaviour.
 *
 *      Arguments:
 *              'avl' - handle to the tree.
 *              'data' - Pointer to the new data.
 *
 *      Return:
 *              '0' - On Success.
 *              '1' - Allocaion fail.
 */
int AVLInsert(avl_t *avl, const void *data);
/*
 *      AVLRemove()     O(log(n))
 *      Remove matching 'data' from the avl tree using the compare_func_t.
 *
 *      Arguments:
 *              'avl' - handle to the tree.
 *              'data' - Pointer to the data to compare with.
 *
 *      Return:
 *              '0' - On Success.
 *              '1' - If there is no matching data.
 */
int AVLRemove(avl_t *avl, const void *data);
/*
* 	AVLFind() O(Log(n))
* 		Finds and returns data according to given key.
*
* 	Arguments:
*		avl - handle to the tree.
*		data - key to search for.
*
*	return:
*		Data that correspondes to the key.
*		NULL, if not found.
*	
*/
void *AVLFind(avl_t *avl, const void *data);
/*
* 	AVLForEach() O(n)
*	 	Performs a certain action on all the data in the tree
*
* 	Arguments:
*		avl - handle to the tree.
*		ACT - function to execute on each data.
*		param - additional param to the action function
*
*	return:
*		0 - success, 
*		on failure the return value of the failed func.
*/
int AVLForEach(avl_t *avl, act_func_t ACT, void *param);
/*
 * AVLIsEmpty : O(1)
 * 
 * Check if the avl tree is empty.
 *
 * Argument:
 * 		avl - handle to the tree.
 *
 * Return:
 *		'1' - in case its empty.
 *		'0' - in case its doesnt.
 */
int AVLIsEmpty(const avl_t *avl);
/*
 * Height : O(1)
 * 
 * Returns the height of the root.
 *
 * Argument:
 * 		avl - handle to the tree.
 *
 * Return:
 *		The height of the tree.
 */
unsigned short AVLHeight(const avl_t *avl);
/*
* Size: O(n)
* return the size of avl
*
* Argumentd:
*	avl - avl - handle to the tree.
*
* Return:
*	the size of avl.
*/
size_t AVLSize(const avl_t *avl);

void AVLRemoveMulti(avl_t *avl, int (*shuould_remove)(void *data, void *arg), void *arg);
/*
*   uses AVLForEach 
*
*/
void AVLMultiFind(avl_t *avl, int (*is_match)(void *data, void *arg), void *arg, dlist_t *dlist);

#endif /* __NAME_H__ */
