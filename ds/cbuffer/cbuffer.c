
#include <stdlib.h> /* malloc() free() */
#include <string.h> /* memset() memcpy() */
#include <assert.h> /* assert() */

#include "cbuffer.h"

#define SIZE_OF_C_BUFF ((sizeof(size_t) * 3) + 1)
#define REAL_CAPACITY (c_buffer->capacity + 1)

struct circular_buffer
{
	size_t read;
	size_t write;
	size_t capacity;
	char buffer[1];
};

c_buffer_t *CBufferCreate(size_t capacity)
{
	c_buffer_t *buff = 0;

	buff = (c_buffer_t*)malloc(SIZE_OF_C_BUFF + capacity);

	if (!(buff))
	{
		return NULL;
	}

	buff->capacity = capacity;
	buff->read = 0;
	buff->write = 0;

	return buff;
}

void CBufferDestroy(c_buffer_t *c_buffer)
{
	assert(c_buffer);

	memset(c_buffer, 0, (SIZE_OF_C_BUFF + c_buffer->capacity));

	free(c_buffer);
	c_buffer = NULL;
}

size_t CBufferCapacity(const c_buffer_t *c_buffer)
{
	assert(c_buffer);

	return c_buffer->capacity;
}

size_t CBufferFreeSpace(const c_buffer_t *c_buffer)
{
	assert(c_buffer);

	return ((c_buffer->capacity - c_buffer->write + c_buffer->read) % REAL_CAPACITY);
}

ssize_t CBufferWrite(c_buffer_t *c_buffer, const void *src, size_t nbytes)
{
	size_t space = 0;
	size_t space_to_end = 0;

	assert(c_buffer);

	space_to_end = (REAL_CAPACITY) - c_buffer->write;
	space = CBufferFreeSpace(c_buffer);

	if (nbytes < space)
	{
		space = nbytes;
	}
	if (space > space_to_end)
	{
		space -= space_to_end;
	}
	else
	{
		space_to_end = space;
		space = 0;
	}

	memcpy((c_buffer->buffer + c_buffer->write), src, space_to_end);

	src = (char*)src + space_to_end;
	c_buffer->write += space_to_end; 

	memcpy(c_buffer->buffer, src, space);

	c_buffer->write += space;
	c_buffer->write %= (REAL_CAPACITY);

	return space + space_to_end;
}

ssize_t CBufferRead(void *dest, c_buffer_t *c_buffer, size_t nbytes)
{
	size_t space = 0;
	size_t space_to_end = 0;

	assert(c_buffer);

	space_to_end = REAL_CAPACITY - c_buffer->read;
	space = c_buffer->capacity - CBufferFreeSpace(c_buffer);

	if (nbytes < space)
	{
		space = nbytes;
	}
	if (space > space_to_end)
	{
		space -= space_to_end;
	}
	else
	{
		space_to_end = space;
		space = 0;
	}

	memcpy(dest, (c_buffer->buffer + c_buffer->read), space_to_end);

	dest = (char*)dest + space_to_end;
	c_buffer->read += space_to_end; 

	memcpy(dest, c_buffer->buffer, space);

	c_buffer->read += space;
	c_buffer->read %= (REAL_CAPACITY);

	return space + space_to_end;
}


int CBufferIsEmpty(const c_buffer_t *c_buffer)
{
	assert(c_buffer);

	return (c_buffer->write == c_buffer->read);
}
