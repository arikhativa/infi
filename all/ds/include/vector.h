
/*
 *	ver 1.1
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct dynamic_vector vector_t;

/*
 *	VectorCreate();
 * 	Creats a vector type data structure.
 *
 *	Arguments:
 *      	'element_size' - Size of an element.
 *      	'capacity' - Number of elements.
 *
 *	Return value: 
 *		Pointer to the vector struct. 
 */
	vector_t *VectorCreate(size_t element_size, size_t capacity);

/*
 *	VectorDestroy();
 * 	Frees the vector struct and it's arguments.
 *
 *	Arguments:
 *      	'*vec' - The vector struct to be freed.
 */
	void VectorDestroy(vector_t *vec);

/*
 *	VectorPush().
 * 	Addes an element to the vector.
 * 	When the vector is full, will use realloc x2 of the capacity, and then push.
 *
 *	Arguments:
 *      	'*vec' - The vector struct to add to.
 *      	'*element' - The element's addres to take from.
 *
 *	Return value:
 *		0	- succes.
 *		-1	- when the realloc fails. (the old data won't be lost).
 */
	int VectorPush(vector_t *vec, const void *element);

/*
 *	VectorPop().
 * 	Removes an element from the vector.
 * 	If the number of elements in the vector is lower then 1/4 of the capacity, will use realloc of 1/2 the capacity.
 *
 *	Arguments:
 *      	'*vec' - The vector struct to remove from.
 */
	void VectorPop(vector_t *vec);

/*
 *	VectorGetAddress().
 * 	Returns the address of a spacific index.
 * 	Can use an index that was pushed already.
 *
 *	Arguments:
 *      	'*vec' - The vector struct to add to.
 *      	'*element' - The element's addres to take from.
 *
 *	Return value:
 *		The addres of the element.
 *		NULL - if the index was wrong.
 */
	void *VectorGetAddress(vector_t *vec, size_t index);


/*
 *	Returns the vector's current size.
 */
	size_t VectorSize(vector_t *vec);

/*
 *	Returns the vector's current capacity
 */
	size_t VectorCapacity(vector_t *vec);

/*
 * 	User's use - 	Allocate more memory for vector in advanced.
 *
 * 	Program use -	increase / decrease memory according to users use of memory. 
 *			will not decrease below start memory allocation.
 *
 *	function return success (0) or failor (-1).
 */
	int VectorReserve(vector_t *vec, size_t new_size);

#endif /* __VECTOR_H__ */
                                                                         
