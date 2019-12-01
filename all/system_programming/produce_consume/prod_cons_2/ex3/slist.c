
#include <assert.h> /* assert() */
#include <stdlib.h> /* malloc() */
#include <string.h> /* memset() */
#include <stdbool.h> /* bool */

#include "slist.h"


enum status
{
	FAIL = -1,
	SUCCESS = 0
};

enum boolean
{
	FALSE = 0,
	TRUE = 1
};

enum flag
{
	OFF = 0,
	ON = 1
};

slist_node_t *SlistCreateNode(void *data, slist_node_t *next)
{
	slist_node_t *new_node = 0;

	new_node = (slist_node_t*)malloc(sizeof(slist_node_t));
	if (!(new_node))
	{
		return NULL;
	}
	new_node->data = data;
	new_node->next = next;

	return new_node;
}

void SlistFreeAll(slist_node_t *node)
{
	slist_node_t *tmp = 0;

	assert(node);

	if (SlistHasLoop(node))
	{
		return ;
	}
	while (node)
	{
		tmp = node->next;
		memset(node, 0, sizeof(slist_node_t));
		free(node);
		node = tmp;
	}
}

static size_t CountNoLoop(slist_node_t *node)
{
	size_t count = 0;

	assert(node);

	while (node)
	{
		node = node->next;
		++count;
	}

	return count;
}

size_t SlistCount(slist_node_t *node)
{
	assert(node);

	if (SlistHasLoop(node))
	{
		return 0;
	}

	return CountNoLoop(node);
}

slist_node_t *SlistInsertAfter(slist_node_t *node, slist_node_t *new_node)
{
	assert(node);
	assert(new_node);

	if (node->next)
	{
		new_node->next = node->next;
	}
	node->next = new_node;

	return new_node;
}

static void SwapDataAddress(void **data1, void **data2)
{
	void *tmp = 0;

	tmp = *data1;
	*data1 = *data2;
	*data2 = tmp;
}

slist_node_t *SlistInsertBefore(slist_node_t *node, slist_node_t *new_node)
{
	assert(node);
	assert(new_node);

	SwapDataAddress(&(node->data), &(new_node->data));
	SlistInsertAfter(node, new_node);

	return node;
}

slist_node_t *SlistRemoveNode(slist_node_t *node)
{
	slist_node_t *removed_node = 0;

	assert(node);

	if (NULL == node->next)
	{
		return NULL;
	}
	SwapDataAddress(&(node->data), &(node->next->data));
	removed_node = node->next;
	node->next = node->next->next;

	return removed_node;
}

slist_node_t *SlistRemoveAfter(slist_node_t *node)
{
	slist_node_t *removed_node = 0;

	assert(node);

	if (NULL == node->next)
	{
		return NULL;
	}
	removed_node = node->next;
	node->next = removed_node->next;

	return removed_node;
}

int SlistHasLoop(slist_node_t *node)
{
	slist_node_t *runner = 0;
	bool flag = OFF;

	assert(node);

	runner = node->next;
	while (runner)
	{
		if (runner == node)
		{
			return TRUE;
		}
		if (ON == flag)
		{
			node = node->next;
			flag = OFF;
		}
		else
		{
			flag = ON;
		}
		runner = runner->next;
	}

	return FALSE;
}

static slist_node_t *MakeSizeEquel(slist_node_t *node, size_t n)
{
	assert(node);

	for (; n; --n)
	{
		node = node->next;
	}

	return node;
}

static slist_node_t *ComperAddress(slist_node_t *node1, slist_node_t *node2)
{
	assert(node1);
	assert(node2);

	while (node1 && node1 != node2)
	{
		node1 = node1->next;
		node2 = node2->next;
	}

	return node1;
}

slist_node_t *SlistFindIntersection(slist_node_t *node1, slist_node_t *node2)
{
	size_t count1 = 0;
	size_t count2 = 0;

	assert(node1);
	assert(node2);

	count1 = SlistCount(node1);
	count2 = SlistCount(node2);
	if (count1 < count2)
	{
		SwapDataAddress((void**)&node1, (void**)&node2);
		count1 = count2 - count1;
	}
	else
	{
		count1 -= count2;
	}
	node1 = MakeSizeEquel(node1, count1);

	return ComperAddress(node1, node2);
}

int SlistForEach(slist_node_t *node, action_func_t action_func, void *param)
{
	int ret = 0;

	assert(node);
	assert(action_func);
	assert(param);

	if (SlistHasLoop(node))
	{
		return FAIL;
	}
	while (node)
	{
		ret = action_func(node->data, param);
		if (ret)
		{
			return ret;
		}
		node = node->next;
	}

	return SUCCESS;
}

slist_node_t *SlistFind(slist_node_t *node, cmp_func_t compare_func, void *param)
{
	bool status = 0;

	assert(node);
	assert(compare_func);
	assert(param);

	if (SlistHasLoop(node))
	{
		return node;
	}
	while (node)
	{
		status = compare_func(node->data, param);
		if (status)
		{
			return node;
		}
		node = node->next;
	}

	return NULL;
}

slist_node_t *SlistFlip(slist_node_t *node)
{
	slist_node_t *first_node = node;
	slist_node_t *tmp = 0;

	assert(node);

	if (SlistHasLoop(node))
	{
		return 0;
	}
	while (node->next)
	{
		tmp = SlistRemoveAfter(node);
		tmp->next = first_node;
		first_node = tmp;
	}

	return first_node;
}

/*
 * 	This part is SlistDisconnectLoop().
 * 	The function finds the 'bad_node' and set it to point to NULL;
 *
 *	Arguments:
 *      	'node' - The address of the 'node'.
 *
 *	Return value: 
 *		The address of the 'bad_node' after the change.
 */ 

static slist_node_t *FindNodeInLoop(slist_node_t *node)
{
	slist_node_t *runner = 0;
	bool flag = OFF;

	assert(node);

	runner = node->next;
	while (runner)
	{
		if (runner == node)
		{
			return runner;
		}
		if (flag)
		{
			node = node->next;
			flag = OFF;
		}
		else
		{
			flag = ON;
		}
		runner = runner->next;
	}

	return NULL;
}

static slist_node_t *DisconnectBadNode(slist_node_t *start, slist_node_t *end)
{
	char count = 0;

	while (start->next != end || 0 == count)
	{
		if (start->next == end)
		{
			count = 1;
		}
		start = start->next;
	}
	start->next = NULL;

	return start;
}

slist_node_t *SlistDisconnectLoop(slist_node_t *node)
{
	slist_node_t *node_in_loop = 0;
	slist_node_t *bad_node = 0;
	slist_node_t *tmp = 0;

	assert(node);

	node_in_loop = FindNodeInLoop(node);
	if (!(node_in_loop))
	{
		return node;
	}
	tmp = node_in_loop->next;
	node_in_loop->next = NULL;	/* Disconnect a node that is inside the loop */
	bad_node = SlistFindIntersection(node, tmp);
	node_in_loop->next = tmp;

	return DisconnectBadNode(node, bad_node);
}

/*
 * 	End of SlistDisconnectLoop().
 */ 


