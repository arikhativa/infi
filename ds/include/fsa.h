/*
*      ver. 1.1 - Starting vers.
*/

/*
*       Name of files - fsa.h, fsa.c, fsa_test.c.       GOOD SHABBAT
*/
#ifndef __FSA_H__
#define __FSA_H__

#include <stddef.h> /* size_t */

typedef struct fsa fsa_t;

/* In C File: 
*
struct fsa
{
	size_t block_size;
	size_t mem_size;
	size_t next;
}
*
*/

/*
*	SuggestSize():          O(1)
*	Canculate the size of memory needed by num of blocks wanted and size of block.
*
*	Arguments:
*		'num_of_blocks' - num of members to be able to recieve in memory.
*		'block_size' - size of each member.
*
*	Return:
*		num of bytes needed to create the FSA.
*/
size_t FSASuggestSize(size_t num_of_blocks, size_t block_size);

/*
*	FSAInit():              O(n)
*	Initialize memory for usage.
*
*	Arguments:
*		'mem' - Starting address of memory to init.
*		'buffer_size' - size of memory to init.
*		'block_size' - size of members in buffer.
*
*	Return:
*		Initiated FSA to use.
*/
fsa_t *FSAInit(void *mem, size_t buffer_size, size_t block_size);

/*
*	FSAAlloc():             O(1)
*	Allocate block for usage.
*
*	Arguments:
*		'mem' - FSA in use.
*
*	Return:
*		Success - Address of empty block.
*		Fail - Null. 
*/
void *FSAAlloc(fsa_t *mem);

/*
*	FSAFree():              O(1)
*	Frees a block.
*
*	Arguments:
*		'block' - Address of block to free.
*
*	Return:
*		None.
*/
void FSAFree(void *block);

/*
*	FSACountFree():         O(n)
*	Get number of free blocks in FSA.
*
*	Arguments:
*		'mem' - FSA to check.
*
*	Return:
*		Success - Num of free blocks. 
*		If mem doesn't exist or uninitialized - return 0.
*/
size_t FSACountFree(fsa_t *mem);

#endif /* __FSA_H__ */
