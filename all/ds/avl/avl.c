
#include <assert.h>	/*	assert()		*/
#include <string.h>	/*	memset()		*/
#include <stdlib.h>	/*	malloc(), free()	*/
#include <stddef.h>	/*	size_t			*/

#include "avl.h"

#define INSERT_HERE	5 /* '5' is a random number	*/
#define NO_MATCH	1
#define REMOVED		55 /* '55' is a random number	*/
#define LEAF_REMOVED	25 /* '25' is a random number	*/
#define BALANCE_FACTOR	2
#define MALLOC_FAIL	1 
#define SUB_LEFT	3 
#define SUB_RIGHT	5 
#define ABS(num)	((0 > num) ? (-num): (num))

typedef struct avl_node avl_node_t;

typedef enum fam_stt
{
        LEAF = 0,
        HAS_LEFT_CHILD,
        HAS_RIGHT_CHILD,
        HAS_TWO_CHILDREN
} famliy_status_t;

typedef enum direction
{
	LEFT = 0,
	RIGHT,
	NUM_OF_CHILDREN
} direction_t;

typedef enum balance_type
{
	LEFT_LEFT = 3,
	RIGHT_LEFT,
	LEFT_RIGHT,
	RIGHT_RIGHT,
	BALANCED
} balance_type_t;

struct avl_node
{
	void *data;
	avl_node_t *child[NUM_OF_CHILDREN];
	unsigned short height;
};

struct avl
{
	avl_node_t *root;
	avl_cmp_func_t Cmp;
};


static void SwapPtr(void **a, void **b);


avl_t *AVLCreate(avl_cmp_func_t Cmp)
{
	avl_t *avl = NULL;

	assert(Cmp);

	avl = (avl_t*)malloc(sizeof(avl_t));
	if (!avl)
	{
		return NULL;
	}

	avl->root = NULL;
	avl->Cmp = Cmp;

	return avl;
}

static void RemoveLeaf(avl_node_t *node)
{
	memset(node, 0, sizeof(avl_node_t));
	free(node);
	node = NULL;
}

static void RecDestroy(avl_node_t *node)
{
	if (!node)
		return ;

	RecDestroy(node->child[LEFT]);

	RecDestroy(node->child[RIGHT]);

	RemoveLeaf(node);
}

void AVLDestroy(avl_t *tree)
{
	assert(tree);

	RecDestroy(tree->root);
	memset(tree, 0, sizeof(avl_t));
	free(tree);
	tree = NULL;
}

int AVLIsEmpty(const avl_t *tree)
{
	assert(tree);

	return (NULL == tree->root);
}

unsigned short AVLHeight(const avl_t *tree)
{
	assert(tree);

	if (!AVLIsEmpty(tree))
	{
		return tree->root->height;
	}

	return 0;
}

static avl_node_t *CreateNode(const void *data)
{
	avl_node_t *new = NULL;

	new = (avl_node_t *)malloc(sizeof(avl_node_t));
	if (!new)
	{
		return NULL;
	}

	memset(new, 0, sizeof(avl_node_t));
	new->data = (void*)data;
	new->height = 1;	/* Leaf's height is '1', and NULL children's height is '0'*/

	return new;
}

static unsigned short GetHight(avl_node_t *node)
{
	if (!node)
	{
		return 0;
	}

	return node->height;
}

static unsigned short GetMaxHight(avl_node_t *left_child, avl_node_t *right_child)
{
	unsigned short left_height = GetHight(left_child);
	unsigned short right_height = GetHight(right_child);

	if (left_height > right_height)
	{
		return left_height;
	}

	return right_height;
}

static void UpdateHight(avl_node_t *node)
{
	unsigned short max_height = 0;

	assert(node);

	max_height = GetMaxHight(node->child[LEFT], node->child[RIGHT]);

	node->height = max_height + 1;
}

/* 	Return:
 * 		'1' - If a balace is needed.
 * 		'0' - If the tree is balanced.	 */
static int CheckBalance(unsigned short left_height, unsigned short right_height)
{
	short sum = 0;

	sum = left_height - right_height;

	return ((ABS(sum)) >= BALANCE_FACTOR);
}

/*
 * 	Return:
 * 		'3' - If a Left-Left balace is needed.
 * 		'4' - If a Right-Left balace is needed.
 * 		'5' - If a Left-Right balace is needed.
 * 		'6' - If a Right-Right balace is needed.
 */
static balance_type_t GetBalanceType(avl_node_t *node)
{
	unsigned short left_height = GetHight(node->child[LEFT]);
	unsigned short right_height = GetHight(node->child[RIGHT]);
	int dir = 0;
	int sub_dir = 0;

	if (!(CheckBalance(left_height, right_height)))
	{
		return BALANCED;
	}

	dir = (left_height > right_height) ? LEFT : RIGHT;

	node = node->child[dir];

	left_height = GetHight(node->child[LEFT]);
	right_height = GetHight(node->child[RIGHT]);

	sub_dir = (left_height > right_height) ? SUB_LEFT : SUB_RIGHT;

	return sub_dir + dir;
}

static void Rotation(avl_node_t *node, direction_t direction)
{
	avl_node_t *child_of_node = NULL;

	assert(node);

	child_of_node = node->child[!direction];

	SwapPtr(&(node->data), &(child_of_node->data));

	node->child[!direction] = child_of_node->child[!direction];

	child_of_node->child[!direction] = child_of_node->child[direction];
	child_of_node->child[direction] = node->child[direction];

	node->child[direction] = child_of_node;

	UpdateHight(node->child[direction]);
	UpdateHight(node);
}

static void BalnceTreeIfNeeded(avl_node_t *node)
{
	balance_type_t balance_type = 0;

	balance_type = GetBalanceType(node);

	switch (balance_type)
	{
		case LEFT_LEFT:
			Rotation(node, RIGHT);
			break ;

		case LEFT_RIGHT:
			Rotation(node->child[LEFT], LEFT);
			Rotation(node, RIGHT);
			break ;

		case RIGHT_LEFT:
			Rotation(node->child[RIGHT], RIGHT);
			Rotation(node, LEFT);
			break ;

		case RIGHT_RIGHT:
			Rotation(node, LEFT);
			break ;

		case BALANCED:
			break ;
	}
}

static int RecInsert(avl_node_t *node, avl_cmp_func_t Cmp, const void *data, avl_node_t *new)
{
	direction_t dir = 0;

	assert(Cmp);

	if (!node)
		return INSERT_HERE;

	dir = (0 < Cmp(data, node->data)) ? RIGHT : LEFT; 

	if (INSERT_HERE == RecInsert(node->child[dir], Cmp, data, new))
	{
		node->child[dir] = new;
	}

	UpdateHight(node);
	BalnceTreeIfNeeded(node);

	return 0;
}

int AVLInsert(avl_t *avl, const void *data)
{
	avl_node_t *new = NULL;

	assert(avl);

	new = CreateNode(data);
	if (!new)
	{
		return MALLOC_FAIL;
	}

	if (AVLIsEmpty(avl))
	{
		avl->root = new;

		return 0;
	}

	RecInsert(avl->root, avl->Cmp, data, new);

	return 0;
}

static void *RecFind(avl_node_t *node, avl_cmp_func_t Cmp, const void *data)
{
	int dir = 0;

	if (!node)
		return NULL;

	dir = Cmp(data, node->data);

	if (!dir)
		return node;

	dir = (0 > dir) ? LEFT : RIGHT;

	return RecFind(node->child[dir], Cmp, data);
}

void *AVLFind(avl_t *avl, const void *data)
{
	avl_node_t *node = NULL;

	assert(avl);

	node = RecFind(avl->root, avl->Cmp, data);

	if (!node)
	{
		return NULL;
	}

	return node->data;
}

static famliy_status_t GetFamliyStatus(avl_node_t *node)
{
        famliy_status_t status = 0;

        assert(node);

        status += (node->child[LEFT]) ? HAS_LEFT_CHILD : 0;
        status += (node->child[RIGHT]) ? HAS_RIGHT_CHILD : 0;

        return status;
}

static void DisconnectAndRemove(avl_node_t *node, avl_node_t *node_to_remove, direction_t dir)
{
	assert(node);

	SwapPtr(&node->data, &(node_to_remove->data));

	node->child[dir] = node_to_remove->child[dir];
	node->child[!dir] = node_to_remove->child[!dir];

	RemoveLeaf(node_to_remove);
}

static void RemoveNodeWithTwoChildren(avl_node_t *node, avl_node_t *node_to_remove)
{
	avl_node_t *tmp = node_to_remove;

	assert(node);

	if (node_to_remove->child[RIGHT])
	{
		SwapPtr(&(node_to_remove->child[RIGHT]->data), &(node_to_remove->data));
		tmp = node_to_remove->child[RIGHT];
		node_to_remove->child[RIGHT] = NULL;
	}

	SwapPtr(&node->data, &(tmp->data));

	RemoveLeaf(tmp);
}

static avl_node_t *GetBeginOfSubTree(avl_node_t *node)
{
	avl_node_t *ret = NULL;

	if (!node->child[LEFT]) /*	Stops one node before NEXT(node)*/
		return NULL;

	ret = GetBeginOfSubTree(node->child[LEFT]);
	if (!ret)
	{
		ret = node->child[LEFT];

		/*	Removing a leaf makes it hard to find the parent.
		 *	This if stytment is checking if it is a leaf.
		 *	if it is, then we disconnect and balance.	*/
		if (!ret->child[RIGHT])
		{
			node->child[LEFT] = NULL;
			UpdateHight(node);
			BalnceTreeIfNeeded(node);
		}

		return ret;
	}

        return ret;
}

static famliy_status_t RemoveNode(avl_node_t *node)
{
	famliy_status_t status = 0;
	avl_node_t *next = NULL;

	assert(node);

	status = GetFamliyStatus(node);

	switch (status)
	{
		case LEAF:
			RemoveLeaf(node);
			return LEAF_REMOVED;

		case HAS_LEFT_CHILD:
			DisconnectAndRemove(node, node->child[LEFT], LEFT);
			break ;

		case HAS_RIGHT_CHILD:
			DisconnectAndRemove(node, node->child[RIGHT], RIGHT);
			break ;

		case HAS_TWO_CHILDREN:

			next = GetBeginOfSubTree(node->child[RIGHT]);

			/*	If node->child[RIGHT] is the same as NEXT(node).
			 *	next would be NULL.				*/
			if (!next)
			{
				next = node->child[RIGHT];
				node->child[RIGHT] = next->child[RIGHT];
				SwapPtr(node->data, next->data);
				RemoveLeaf(next);
			}
			else
			{
				RemoveNodeWithTwoChildren(node, next);
			}

			break ;
	}

	UpdateHight(node);
	BalnceTreeIfNeeded(node);

	return REMOVED;
}

static int RecRemove(avl_node_t *node, avl_cmp_func_t Cmp, const void *data)
{
	int dir = 0;
	int ret = 0;

	if (!node)
		return NO_MATCH;

	dir = Cmp(data, node->data);

	if (!dir)
	{
		return RemoveNode(node);
	}

	dir = (0 > dir) ? LEFT : RIGHT;

	ret = RecRemove(node->child[dir], Cmp, data);

	if (NO_MATCH == ret)
	{
		return 1;
	}

	if (LEAF_REMOVED == ret)
	{
		node->child[dir] = NULL;
	}
	else
	{
		UpdateHight(node->child[dir]);
	}

	UpdateHight(node);
	BalnceTreeIfNeeded(node);

	return 0;
}

static int RemoveRoot(avl_node_t *root, avl_cmp_func_t Cmp, const void *data)
{
	int ret = 0;

	ret = Cmp(data, root->data);

	if (!ret)
	{
		RemoveLeaf(root);
	}

	return ret;
}

int AVLRemove(avl_t *avl, const void *data)
{
	int ret = 0;

	assert(avl);

	if (1 == AVLSize(avl))
	{
		ret = RemoveRoot(avl->root, avl->Cmp, data);
		if (!ret)
		{
			avl->root = NULL;
		}
	}
	else
	{
		ret = RecRemove(avl->root, avl->Cmp, data);
	}


	return ret;
}

static size_t RecSize(avl_node_t *node)
{
	if (!node)
		return 0;

	return (1 + RecSize(node->child[LEFT]) + RecSize(node->child[RIGHT]));
}

size_t AVLSize(const avl_t *avl)
{
	return RecSize(avl->root);
}

static int RecForEach(avl_node_t *node, act_func_t ACT, void *param)
{
	int stt = 0;

	if (!node)
		return 0;

	stt = RecForEach(node->child[LEFT], ACT, param);
	if (stt)
		return stt;

	stt = ACT(node->data, param);
	if (stt)
		return stt;

	stt = RecForEach(node->child[RIGHT], ACT, param);

	return stt;
}

int AVLForEach(avl_t *avl, act_func_t ACT, void *param)
{
	assert(avl);
	assert(ACT);

	if (AVLIsEmpty(avl))
	{
		return -1;
	}

	return RecForEach(avl->root, ACT, param);
}

static void SwapPtr(void **a, void **b)
{
	void *tmp = *a;

	*a = *b;
	*b = tmp;
}
