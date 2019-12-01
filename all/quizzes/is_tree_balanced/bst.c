
#include <assert.h> /*	assert()		*/
#include <stdlib.h> /*	free()	malloc()	*/
#include <string.h> /*	memset()		*/

#include "bst.h"

typedef enum node_pos
{
	LEFT = 0,
	RIGHT
}node_pos_t;

typedef enum node_fam_stt
{
	LEAF = 0,
	HAS_LEFT_CHILD,
	HAS_RIGHT_CHILD,
	HAS_TWO_CHILDREN
}node_fam_stt_t;

typedef struct bst_node bst_node_t;

struct bst_node
{
	bst_node_t *left_child;
	bst_node_t *right_child;
	bst_node_t *parent;
	void *data;
};

struct bst
{
	bst_node_t end; /* 'end' is a dummy node. it's left child points to it's self. */
	policy_func_t BstCmp;
	void *param;
};

/*
 * 	GetNodePosition()
 * 	This function determens if the node is a left child to his parent or a right child.
 */
	static node_pos_t GetNodePosition(bst_node_t *node);

/*
 *	GetNodeFamilyStatus()
 * 	This function determens the node's 'family status'. (leaf, has right chid...)
 */
	static node_fam_stt_t GetNodeFamilyStatus(bst_node_t *node);

/*
 * 	GetNextLeaf()
 * 	This function is similar to BSTNext as it is locating the next node (with higher value)
 * 	but it will find the next leaf (with higher value) insted.
 */
	static bst_node_t *GetNextLeaf(bst_node_t *node);

static void RemoveLeaf(bst_node_t *node);
static int IsDummy(bst_iter_t *node);
static bst_node_t *NodeCreate(bst_node_t *parent, void *data, bst_node_t *end);
static bst_iter_t *GetBeginOfSubTree(bst_node_t *node);
static bst_iter_t *GetEndOfSubTree(bst_node_t *node);
static void DisconnectLeftChild(bst_node_t *node);
static void DisconnectLeaf(bst_node_t *node);
static void DisconnectRightChild(bst_node_t *node);
static void SwapPtr(void **a, void **b);


/*
 *	BSTCreate()
 */

bst_t *BSTCreate(policy_func_t BstCmp, void *param)
{
	bst_t *bst = 0;

	assert(BstCmp);

	bst = (bst_t*)malloc(sizeof(bst_t));
	if (!bst)
	{
		return NULL;
	}

	bst->BstCmp = BstCmp;
	bst->param = param;

	bst->end.left_child = &bst->end;
	bst->end.right_child = &bst->end;

	/* 'end' right child points to it's self if the BST is empty.
	 * and will point to the root of the BST if exeists. */

	bst->end.data = NULL;
	bst->end.parent = &bst->end;

	return bst;
}

/*
 *	BSTIsEmpty()
 */

int BSTIsEmpty(const bst_t *bst)
{
	assert(bst);

	return (bst->end.right_child == BSTEnd(bst));
}

/*
 *	BSTDestroy()
 */

void BSTDestroy(bst_t *bst)
{
	bst_node_t *leaf = NULL;
	bst_node_t *parent = NULL;
	node_pos_t pos = 0;

	assert(bst);

	parent = bst->end.right_child;

	while (!BSTIsEmpty(bst))
	{
		leaf = GetNextLeaf(parent);
		parent = leaf->parent;

		pos = GetNodePosition(leaf);
		if (LEFT == pos)
		{
			parent->left_child = leaf->left_child; /* it's the dummy node */
		}
		else
		{
			parent->right_child = leaf->left_child; /* it's the dummy node */
		}

		RemoveLeaf(leaf);
	}

	memset(bst, 0, sizeof(bst_t));
	free(bst);
	bst = NULL;
}

static node_pos_t GetNodePosition(bst_node_t *node)
{
	bst_node_t *parent = NULL;

	assert(node);

	parent = node->parent;

	if (BSTIsSameIter(parent->right_child, node))
	{
		return RIGHT;
	}

	return LEFT;
}

static void RemoveLeaf(bst_node_t *node)
{
	assert(node);

	memset(node, 0, sizeof(bst_node_t));
	free(node);
	node = NULL;
}

static int IsDummy(bst_iter_t *node)
{
	assert(node);

	return ((size_t)node->left_child == (size_t)node);
}

static node_fam_stt_t GetNodeFamilyStatus(bst_node_t *node)
{
	node_fam_stt_t status = 0;

	assert(node);

	status += (!IsDummy(node->left_child)) ? HAS_LEFT_CHILD : 0;
	status += (!IsDummy(node->right_child)) ? HAS_RIGHT_CHILD : 0;

	return status;
}

static bst_node_t *GetNextLeaf(bst_node_t *node)
{
	node_fam_stt_t status = 0;

	assert(node);

	status = GetNodeFamilyStatus(node);

	while (LEAF != status)
	{
		node = (!IsDummy(node->left_child)) ? node->left_child : node->right_child;
		status = GetNodeFamilyStatus(node);
	}

	return node;
}

/*
 *	BSTIsSameIter()
 */

int BSTIsSameIter(const bst_iter_t *iter_a, const bst_iter_t *iter_b)
{
	assert(iter_a);
	assert(iter_b);

	return (iter_a == iter_b);
}

/*
 *	BSTEnd()
 */

bst_iter_t *BSTEnd(const bst_t* bst)
{
	assert(bst);

	return (&((bst_t*)bst)->end);	/* The casting is for discarding the 'const' */
}

/*
 *	BSTInsert()
 */

bst_iter_t *BSTInsert(bst_t *bst, void *data)
{
	bst_node_t *end = NULL;
	bst_node_t *current = NULL;
	bst_node_t *current_parent = NULL;
	int status = 0;

	assert(bst);

	end = BSTEnd(bst);
	current = end->right_child;
	current_parent = end->right_child;

	while (current != end)
	{
		status = bst->BstCmp(current->data, data, bst->param);
		current_parent = current;
		current = (0 > status) ? current->left_child : current->right_child;
	}

	current = NodeCreate(current_parent, data, end);
	if (!current)
	{
		return NULL;
	}

	if (0 > status)
	{
		current_parent->left_child = current;
	}
	else
	{
		current_parent->right_child = current;
	}

	return current;
}

static bst_node_t *NodeCreate(bst_node_t *parent, void *data, bst_node_t *end)
{
	bst_node_t *new = NULL;

	assert(parent);
	assert(end);

	new = (bst_node_t*)malloc(sizeof(bst_node_t));
	if (!new)
	{
		return end;
	}

	new->left_child = end;
	new->right_child = end;
	new->parent = parent;
	new->data = data;

	return new;
}

/*
 *	BSTBegin()
 */

bst_iter_t *BSTBegin(const bst_t* bst)
{
	bst_node_t *node = NULL;

	assert(bst);

	node = GetBeginOfSubTree(((bst_t*)bst)->end.right_child);

	return node;
}

static bst_iter_t *GetBeginOfSubTree(bst_node_t *node)
{
	bst_node_t *parent = node;

	assert(node);

	while (!IsDummy(node))
	{
		parent = node;
		node = node->left_child;
	}

	return parent;
}

/*
 *	BSTNext()
 */

bst_iter_t *BSTNext(bst_iter_t *iter)
{
	assert(iter);

	if (!IsDummy(iter->right_child))
	{
		return GetBeginOfSubTree(iter->right_child);
	}
	while (LEFT != GetNodePosition(iter) && !IsDummy(iter->parent))
	{
		iter = iter->parent;
	}

	return iter->parent;
}

/*
 *	BSTRemove()
 */

void BSTRemove(bst_iter_t *iter)
{
	node_fam_stt_t status = 0;
	bst_node_t *next = NULL;

	assert(iter);

	status = GetNodeFamilyStatus(iter);
	next = iter;

	switch (status)
	{
		case LEAF:
			DisconnectLeaf(iter);
			break ;

		case HAS_LEFT_CHILD:
			DisconnectLeftChild(iter);
			break ;

		case HAS_RIGHT_CHILD:
			DisconnectRightChild(iter);
			break ;

		case HAS_TWO_CHILDREN:
			next = BSTNext(iter);
			SwapPtr(&iter->data, &next->data);
			DisconnectRightChild(next);
			break ;
	}

	RemoveLeaf(next);
}

static void SwapPtr(void **a, void **b)
{
	void *tmp = 0;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void DisconnectLeftChild(bst_node_t *node)
{
	bst_node_t *parent = NULL;
	node_pos_t pos = 0;

	assert(node);

	pos = GetNodePosition(node);

	parent = node->parent;
	if (LEFT == pos)
	{
		parent->left_child = node->left_child;
	}
	else
	{
		parent->right_child = node->left_child;

	}

	node->left_child->parent = parent;
}

static void DisconnectLeaf(bst_node_t *node)
{
	bst_node_t *parent = NULL;
	node_pos_t pos = 0;

	assert(node);

	pos = GetNodePosition(node);
	parent = node->parent;

	if (LEFT == pos)
	{
		parent->left_child = node->right_child;
	}
	else
	{
		parent->right_child = node->right_child;

	}
}

static void DisconnectRightChild(bst_node_t *node)
{
	assert(node);

	DisconnectLeaf(node);

	node->right_child->parent = node->parent;
}

/*
 *	BSTPrev()
 */

bst_iter_t *BSTPrev(bst_iter_t* iter)
{
	assert(iter);

	if (!IsDummy(iter->left_child))
	{
		return GetEndOfSubTree(iter->left_child);
	}
	while (RIGHT != GetNodePosition(iter))
	{
		iter = iter->parent;
	}

	return iter->parent;
}

static bst_iter_t *GetEndOfSubTree(bst_node_t *node)
{
	bst_node_t *parent = NULL;

	while (!IsDummy(node))
	{
		parent = node;
		node = node->right_child;
	}

	return parent;
}

/*
 *	BSTGetData()
 */

void *BSTGetData(const bst_iter_t *iter)
{
	return ((bst_node_t*)iter)->data; /* The casting is for discarding the 'const' */
}

/*
 *	BSTSize()
 */

size_t BSTSize(const bst_t *bst)
{
	bst_node_t *begin = NULL;
	bst_node_t *end = NULL;
	size_t size = 0;

	assert(bst);

	begin = BSTBegin(bst);
	end = BSTEnd(bst);

	while (begin != end)
	{
		++size;
		begin = BSTNext(begin);
	}

	return size;
}

/*
 *	BSTSize()
 */

bst_iter_t *BSTFind(bst_t *bst, void *data)
{
	int status = 0;
	bst_node_t *current = NULL;
	bst_node_t *end = NULL;

	assert(bst);

	current = bst->end.right_child;
	end = BSTEnd(bst);

	while (current != end)
	{
		status = bst->BstCmp(current->data, data, bst->param);
		if (!status)
		{
			return current;
		}

		current = (0 > status) ? current->left_child : current->right_child;
	}

	return end;
}

/*
 *	BSTForEach()
 */

int BSTForEach(bst_iter_t *start, bst_iter_t *stop, act_func_t ForEach, void *param)
{
	int status = 0;

	assert(start);
	assert(stop);
	assert(ForEach);

	while (start != stop)
	{
		status = ForEach(start->data, param);
		if (status)
		{
			return status;
		}
		start = BSTNext(start);
	}

	return 0;
}
