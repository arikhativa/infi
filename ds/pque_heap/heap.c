

#include <assert.h>	/*	assert()		*/
#include <string.h>	/*	memset()		*/
#include <stdlib.h>	/*	free(), malloc()	*/

#include "vector.h"
#include "heap.h"


#define PARENT_INDEX(index) (((index) - 1) / 2)

/* This Macro should only get the child_t valus: LEFT or RIGHT	*/
#define GET_DIR(index, dir) (((index * 2) + (dir) + 1))
#define SET_DIR(dir) (!!((dir) + 1))

#define LARGER 1
#define MALLOC_FAIL_INT 1
#define MALLOC_FAIL_PTR NULL


struct heap
{
	vector_t *vec;
	heap_cmp_func_t fp_cmp;
	void *param;
};

typedef enum children
{
	LEFT = 0,
	RIGHT,
	NUM_OF_CHILDREN
} child_t;

typedef enum famliy_status
{
	LEAF = 0,
	LEFT_CHILD = 1,
	TWO_CHILDREN = 2
} fam_stt_t;


static void HeapifyUp(heap_t *heap, size_t last_index);
static void HeapifyDown(heap_t *heap, size_t index, size_t size);
static int GetFamStt(size_t left, size_t right, size_t size);
static void SwapData(void **a, void **b);


heap_t *HeapCreate(heap_cmp_func_t fp_cmp, size_t suggested_size, void *param)
{
	heap_t *heap = NULL;

	assert(fp_cmp);

	heap = (heap_t*)malloc(sizeof(heap_t));
	if(!heap)
	{
		return MALLOC_FAIL_PTR;
	}

	heap->vec = VectorCreate(sizeof(void*), 1);
	if (!heap->vec)
	{
		free(heap);
		return MALLOC_FAIL_PTR;
	}

	/* When Creating a Vector, it starts full.
	 * We empty it, and then adding the suggested size	*/
	VectorPop(heap->vec);
	VectorReserve(heap->vec, suggested_size);

	heap->fp_cmp = fp_cmp;
	heap->param = param;

	return heap;
}

void HeapDestroy(heap_t *heap)
{
	assert(heap);

	VectorDestroy(heap->vec);

	memset(heap, 0, sizeof(heap_t));
	free(heap);
	heap = NULL;
}

int HeapIsEmpty(const heap_t *heap)
{
	assert(heap);

	return !(VectorSize(heap->vec));
}

void *HeapPeek(heap_t *heap)
{
	void **data = NULL;

	assert(heap);

	data = VectorGetAddress(heap->vec, 0);

	return *data;
}

size_t HeapSize(const heap_t *heap)
{
	assert(heap);

	return VectorSize(heap->vec);
}

int HeapPush(heap_t *heap, const void *data)
{
	size_t last_index = 0;

	assert(heap);

	if (VectorPush(heap->vec, &data))
	{
		return MALLOC_FAIL_INT;
	}

	last_index = VectorSize(heap->vec) - 1; /* Last index is size - 1 */

	HeapifyUp(heap, last_index);

	return 0;
}

void *HeapPop(heap_t *heap)
{
	void **last_node_data = NULL;
	void **root = NULL;
	int last_node_index = 0;

	assert(heap);

	last_node_index = HeapSize(heap) - 1;

	if (0 > last_node_index)
	{
		return NULL;
	}

	root = VectorGetAddress(heap->vec, 0);
	last_node_data = VectorGetAddress(heap->vec, last_node_index);

	SwapData(last_node_data, root);

	VectorPop(heap->vec);

	HeapifyDown(heap, 0, last_node_index - 1); /* '-1' for the updated last index (after Pop()) */

	return *last_node_data;
}

void HeapRemove(heap_t *heap, heap_rm_func_t fp_rm, void *param)
{
	void **root = NULL;
	int index = 0;
	int size = 0;
	int stt = 0;

	assert(heap);
	assert(fp_rm);

	root = VectorGetAddress(heap->vec, 0);
	size = HeapSize(heap);

	for (index = 0; index < size; ++index)
	{
		stt = fp_rm(root[index], param);
		if (!stt)
		{
			SwapData(&(root[size - 1]), &(root[index]));
			VectorPop(heap->vec);
			HeapifyDown(heap, index, size - 2); /* '-2' becuse of Pop() */
			HeapifyUp(heap, index);
			return ;
		}
	}
}

static void HeapifyUp(heap_t *heap, size_t last_index)
{
	void **node_data = NULL;
	void **parent_data = NULL;
	int stt = 0;

	assert(heap);

	while (0 != last_index)
	{
		node_data = VectorGetAddress(heap->vec, last_index);

		last_index = PARENT_INDEX(last_index);

		parent_data = VectorGetAddress(heap->vec, last_index);

		stt = heap->fp_cmp(*node_data, *parent_data, heap->param);
		if (LARGER == stt)
		{
			SwapData(node_data, parent_data);
		}
	}
}

static void HeapifyDown(heap_t *heap, size_t index, size_t size)
{
	void **node = NULL;
	void **child[NUM_OF_CHILDREN];
	fam_stt_t fam_stt = 0;
	int diraction = 0;

	while ((index) < size)
	{
		node = VectorGetAddress(heap->vec, index);

		child[LEFT] = VectorGetAddress(heap->vec, GET_DIR(index, LEFT));
		child[RIGHT] = VectorGetAddress(heap->vec, GET_DIR(index, RIGHT));

		fam_stt = GetFamStt(GET_DIR(index, LEFT), GET_DIR(index, RIGHT), size  +1);

		if (LEAF == fam_stt)
			break ;

		switch (fam_stt)
		{
			case LEAF:
				break ;

			case LEFT_CHILD:
				diraction = LEFT;
				break ;

			case TWO_CHILDREN:
				diraction = heap->fp_cmp(*(child[RIGHT]), *(child[LEFT]), heap->param);
				diraction = SET_DIR(diraction);
				break ;
		}

		/* fp_cmp will return '-1' if there's need to swap node with child */

		if (-1 == heap->fp_cmp(*node, *(child[diraction]), heap->param))
		{
			SwapData(node, child[diraction]);
		}

		index = GET_DIR(index, diraction);
	}
}

static int GetFamStt(size_t left, size_t right, size_t size)
{
	int ret_l = 0;
	int ret_r = 0;

	ret_l = size - left;

	if (0 >= ret_l)
	{
		return LEAF;
	}

	ret_r = size - right;
	if (0 == ret_r)
	{
		return LEFT_CHILD;
	}

	return TWO_CHILDREN;
}

static void SwapData(void **a, void **b)
{
	void *tmp = *a;

	*a = *b;
	*b = tmp;
}
