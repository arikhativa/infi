/*
 *	ver. 1.4 - changed the name and type of the struct member.
 *  ver. 1.3 - added <sys/types>.
 *	ver. 1.2 - Changed min size in init. made struct vsa.
 *	ver. 1.1 - Starting vers.
 */

/*
 *       Name of files - vsa.h, vsa.c, vsa_test.c.
 */

#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h>	/* size_t */
#include <sys/types.h>	/* ssize_t */


typedef struct vsa vsa_t;

/*
 * 	IN .C FILE:
 *
 * 	struct vsa
 * 	{
 * 		ssize_t blk_size;
 * 	};
 */

/*
 *	VSAInit():              O(1)
 *	Initialize memory for usage.
 *	The size must be larger then (sizeof(size_t) * 3).
 *
 *	Arguments:
 *		'mem' - Starting address of memory to init.
 *		'mem_size' - size of memory to init.
 *
 *	Return:
 *		On Success  - Pointer to the initialized memory.
 *		On Failure  - Pointer to NULL.
 */
	vsa_t *VSAInit(void *mem, size_t mem_size);

/*
 *	VSAAlloc():             O(n)
 *	Allocate memory for usage.
 *
 *	Arguments:
 *		'vsa' - The memory to use.
 *		'size_to_alloc' - The size to allocate (in bytes).
 *
 *	Return:
 *		On Success  - Pointer to the allocated memory.
 *		On Failure  - Pointer to NULL.
 */
	void *VSAAlloc(vsa_t *vsa, size_t size_to_alloc);

/*
 *	VSAFree():              O(1)
 *	Frees memory.
 *
 *	Arguments:
 *		'ptr_to_free' - The address to free.
 */
	void VSAFree(void *ptr_to_free);

/*
 *	VSALargestChunkAvailable():         O(n)
 *	Finds the lagest free memory space.
 *
 *	Arguments:
 *		'mem' - Memory to search in.
 *
 *	Return:
 *		The size of the memory found (in bytes).
 */
	size_t VSALargestChunkAvailable(vsa_t *mem);

#endif /* __VSA_H__ */

