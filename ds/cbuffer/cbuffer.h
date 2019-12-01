/*
*   version: 1.4 - changed the order of arguments in Read.
*   version: 1.3 - return value of IsEmpty                   
*/

#ifndef __CBUFFER_H__
#define __CBUFFER_H__

#include <sys/types.h> /* ssize_t */
#include <stddef.h> /* size_t */

typedef struct circular_buffer c_buffer_t;

/*
*	CBufferCreate():
*	Creates an empty circular buffer.
*
*	Arguments:
*		'capacity' - num of bytes in a circle.
*
*	Return:
*		New empty circular buffer.
*/
c_buffer_t *CBufferCreate(size_t capacity);

/*
*	CBufferDestroy():
*	Clean all data from buffer then free buffer.
*
*	Arguments:
*		'c_buffer_t' - buffer to destroy.
*
*	Return:
*		None.
*/
void CBufferDestroy(c_buffer_t *c_buff);

/*
*	CBufferFreeSpace():
*	Check how much space is free in current circle.
*
*	Arguments:
*		'c_buffer_t' - buffer to be checked.
*
*	Return:
*		Number of empty bytes in circle.
*/
size_t CBufferFreeSpace(const c_buffer_t *c_buff);

/*
*	CBufferCapacity():
*	Check size of one circle in buffer.
*
*	Arguments:
*		'c_buffer_t' - buffer to be checked.
*
*	Return:
*		Number of bytes in one circle.
*/
size_t CBufferCapacity(const c_buffer_t *c_buff);

/*
*	CBufferRead():
*	Read up to "nbytes" from "buffer" into the buffer starting
       at "dest".

*
*	Arguments:
*       dest - pointer to the memory that the data is copied to.
*		buffer - pointer to the data copied from.
*       nbytes - number of bytes to be copied 
*
*	Return:
*	On success, the number of bytes read is returned. On fail return -1.
*/	
ssize_t CBufferRead(void *dest, c_buffer_t *c_buff, size_t nbytes);

/*
*	CBufferWrite():
*	Write up to "nbytes" from src into the buffer first empty byte.

*
*	Arguments:
*		buffer -the buffer that the data copied to.
        src - pointer to the data that copied.
        nbytes - The number of bytes to write 
*
*	Return:
*	Returns the number of bytes that were written.
*/	
ssize_t CBufferWrite(c_buffer_t *c_buff, const void *src, size_t nbytes);

/*
*	CBufferIsEmpty():
*	Check if buffer is empty.
*
*	Arguments:
*		'c_buffer_t' - buffer to be checked.
*
*	Return:
*		True (1) if empty, or false (0).
*/
int CBufferIsEmpty(const c_buffer_t *c_buff);

#endif /* __CBUFFER_H__ */
