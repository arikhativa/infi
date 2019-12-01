
#include <assert.h> /* assert() */
#include <string.h> /* memmove() */
#include <stdlib.h> /* malloc(), realloc() */
#include "vector.h"

struct dynamic_vector
{
	void *start;
	size_t current_index;
	size_t capacity;
	size_t element_size;
};

vector_t *VectorCreate(size_t element_size, size_t capacity)
{
	vector_t *vec = 0;

	if (!(vec = (vector_t*)malloc(sizeof(vector_t))))
	{
		return NULL;
	}
	if (!(vec->start = (void*)malloc(element_size * capacity)))
	{
		free(vec);
		return NULL;
	}
	vec->current_index = capacity;
	vec->capacity = capacity;
	vec->element_size = element_size;

	return vec;
}

void VectorDestroy(vector_t *vec)
{
	assert(vec);

	free(vec->start);
	vec->current_index = 0;
	vec->capacity = 0;
	vec->element_size = 0;
	free(vec);
}

int VectorReserve(vector_t *vec, size_t new_size)
{
	void *tmp = 0;

	assert(vec);

	if ((new_size < vec->capacity) || !(tmp = realloc(vec->start, (vec->element_size * new_size))))
	{
		return -1;
	}
	vec->start = tmp;
	vec->capacity = new_size;

	return 0;
}

int VectorPush(vector_t *vec, const void *element)
{
	size_t index = 0;

	assert(element);
	assert(vec);

	if (vec->current_index == vec->capacity)
	{
		if (-1 == (VectorReserve(vec, (2 * vec->capacity))))
		{
			return -1;
		}
	}
	index = (vec->current_index * vec->element_size);
	memmove((void*)((char*)vec->start + index), element, vec->element_size);
	++vec->current_index;

	return 0;
}

static void LowerCapacity(vector_t *vec)
{
	void *tmp = 0;
	size_t new_size  = 0;

	assert(vec);

	new_size = vec->capacity / 2;
	if (!(tmp = realloc(vec->start, (vec->element_size * new_size))))
	{
		return ;
	}
	vec->start = tmp;
	vec->capacity = new_size;
}

void VectorPop(vector_t *vec)
{
	assert(vec);

	if (0 == vec->current_index)
	{
		return ;
	}
	--vec->current_index;
	if (10 <= vec->current_index && vec->current_index * 4 <= vec->capacity)
	{
		LowerCapacity(vec);
	}
}

size_t VectorSize(vector_t *vec)
{
	assert(vec);

	return vec->current_index;
}

void *VectorGetAddress(vector_t *vec, size_t index)
{
	assert(vec);

	if (index >= vec->current_index)
	{
		return NULL;
	}

	return (void*)((char*)vec->start + ((index) * vec->element_size));
}

size_t VectorCapacity(vector_t *vec)
{
	assert(vec);

	return vec->capacity;
}
