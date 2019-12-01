/*
 *      ver 1.8 - Added incude for <stddef.h> for size_t (SlistCount function return type)
 *      ver 1.7 - Changed the return value of the compare_func in SlistFind().
 *      ver 1.6 - Changed the return value of ForEach.
 *      ver 1.5 - Changed the only comments in InsertFuncs, Removefuncs and added complexity to all.
 *      ver 1.4 - Changed the return value in 'RemoveNode()' and 'RemoveAfter()'.
 *      ver 1.3 - Added comment to 'RemoveNode()' about the return value.
 *      ver 1.2 - SlistHasLoop changed the return value. Removed all 'const'.
 *      ver 1.1 - Added comments on the return value of remove and insert funcs.
 *      ver 1.0 - Original.
 */


#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h>

typedef struct slist_node slist_node_t;
typedef int (*action_func_t)(void *data, void *param);
typedef int (*cmp_func_t)(const void *data, void *param);

struct slist_node
{
        void *data;
        slist_node_t *next;
};

/*
 *	SlistCreateNode();
 * 	Creats a slist node.        O(1)
 *
 *	Arguments:
 *      	'data' - Pointer to the data.
 *      	'next' - Pointer to the next node. (can be NULL)
 *
 *	Return value: 
 *		Pointer to the new node.
 */
	slist_node_t *SlistCreateNode(void *data, slist_node_t *next);

/*
 *	SlistFreeAll();
 * 	Frees the node and the ones after it.   O(n)
 *
 *	Arguments:
 *      	'node' - Pointer to the node.
 */
	void SlistFreeAll(slist_node_t *node);

/*
 *	SlistInsertAfter();
 * 	Connects 'new_node' after 'node'.       O(1)
 *
 *	Arguments:
 *      	'node' - Pointer to 'node'.
 *      	'new_node' - Pointer to the node to be connected.
 *
 *	Return value: 
 *		Pointer to 'new_node'.
 */
	slist_node_t *SlistInsertAfter(slist_node_t *node, slist_node_t *new_node);

/*
 *	SlistInsertBefore();
 * 	Connects 'new_node' before 'node'.      O(1)
 *
 *	Arguments:
 *      	'node' - Pointer to 'node'.
 *      	'new_node' - The address 'node' will be connected to.
 *
 *	Return value: 
 *		Pointer to 'new_node'.
 */
	slist_node_t *SlistInsertBefore(slist_node_t *node, slist_node_t *new_node);

/*
 *	SlistRemoveNode();
 * 	disconnects 'node' from its slist.      O(1)

 
 *  Last node cannot be removed.
 *
 *	Arguments:
 *      	'node' - Pointer to the 'node' to be removed.
 *
 *	Return value: 
 *		Pointer to the removed 'node'.
 *		If last - NULL.
 */
	slist_node_t *SlistRemoveNode(slist_node_t *node);

/*
 *	SlistRemoveAfter();
 * 	disconnects the node after 'node' from the slist.   O(1)
 *
 *	Arguments:
 *      	'node' - Pointer to 'node'.
 *
 *	Return value: 
 *		Pointer to the removed node.
 */
	slist_node_t *SlistRemoveAfter(slist_node_t *node);

/*
 *	SlistCount();
 *	Counts the number of nodes in the slist.    O(n)
 *
 *	Arguments:
 *      	'node' - Pointer to 'node'.
 *
 *	Return value:
 *		The number of nodes.
 */
	size_t SlistCount(slist_node_t *node);

/*
 *	SlistFind();
 * 	Find node-data by comparing to given param.     O(n)
 *
 *	Arguments:
 *		'node' - Beginning of list search.
 *		'compare_func' - given compare function by user's need.
 *		    return true (1) or false(0).
 *		'param' - user's param.
 *
 *	Return value: 
 *		Pointer to node where data is found or NULL if not found.
 */	
	slist_node_t *SlistFind(slist_node_t *node, cmp_func_t compare_func, void *param);

/*
 *	SlistForEach();
 *		For each node in list - do action.  O(n)
 *
 *	Arguments:
 *		'node' - Beginning list.
 *		'action_func()' - Does an action on node-data using given parameter
 *			Returns an int value representing if the action succeed (0) or not (not 0).
 *		'param' - Parameter given by the user to be used in user func.
 *
 *	Return value:
 *		0 = On seccess.
 *		If fails, returns the value 'action_func' returens.
 */
	int SlistForEach(slist_node_t *node, action_func_t action_func, void *param);

	/*
 *	SlistFlip();
 * 	Reverse the list direction (end points to start).   O(n)
 *
 *	Arguments:
 *		'node' - Beginning list to be flipped.
 *
 *	Return value:
 *		Pointer to first node in new list.
 */
	slist_node_t *SlistFlip(slist_node_t *node);

/*
 *	SlistHasLoop();
 * 	Find if the list has a loop.        O(n)
 *
 *	Arguments:
 *      	'node' - Beginning of search
 *
 *	Return value:
 *		True (1) if list has loop or false (0);
 */
	int SlistHasLoop(slist_node_t *node);

/*
 *	SlistFindIntersection();
 * 	Find an intersection between two lists.     O(n)
 *
 *	Arguments:
 *      	'node1' - Pointer of first list.
 *      	'node2' - Pointer of seconed list.
 *
 *	Return value: 
 *		If found - Pointer to Intersection node.
 *      Otherwise - NULL.
 */
	slist_node_t *SlistFindIntersection(slist_node_t *node1, slist_node_t *node2);

#endif /* __SLIST_H__ */

