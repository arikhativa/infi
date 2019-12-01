
#include <stdlib.h> /* malloc() free() */
#include <string.h> /* memset() memcpy() */
#include <assert.h> /* assert() */
#include <pthread.h>	/* pthread_create(), pthread_join() */
#include <semaphore.h>

#include "cbuffer.h"


struct circular_buffer
{
	sem_t can_write;
	sem_t can_read;
	pthread_mutex_t door_write;
	pthread_mutex_t door_read;
	size_t read;
	size_t write;
	size_t capacity;
	char buffer[1];
};

#define REAL_CAPACITY (c_buffer->capacity + 1)
#define SIZE_OF_C_BUFF ((sizeof(sem_t) * 2) + (sizeof(pthread_mutex_t) * 2)\
 + (sizeof(size_t) * 3) + 1)


c_buffer_t *CBufferCreate(size_t capacity)
{
	c_buffer_t *buff = 0;

	buff = (c_buffer_t*)malloc(SIZE_OF_C_BUFF + capacity);

	if (!(buff))
	{
		return NULL;
	}

	if (sem_init(&buff->can_write, 0, capacity / sizeof(int)))
	{
		free(buff);
		buff = NULL;
		return NULL;
	}

	if (sem_init(&buff->can_read, 0, 0))
	{
		sem_destroy(&buff->can_write);
		free(buff);
		buff = NULL;
		return NULL;
	}

	if(pthread_mutex_init(&buff->door_write, NULL))
	{
		sem_destroy(&buff->can_write);
		sem_destroy(&buff->can_read);
		free(buff);
		buff = NULL;
		return NULL;
	}

	if(pthread_mutex_init(&buff->door_read, NULL))
	{
		pthread_mutex_destroy(&buff->door_write);
		sem_destroy(&buff->can_write);
		sem_destroy(&buff->can_read);
		free(buff);
		buff = NULL;
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

	sem_destroy(&c_buffer->can_write);
	sem_destroy(&c_buffer->can_read);
	pthread_mutex_destroy(&c_buffer->door_write);
	pthread_mutex_destroy(&c_buffer->door_read);

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

	sem_wait(&c_buffer->can_write);
	pthread_mutex_lock(&c_buffer->door_write);

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

	sem_post(&c_buffer->can_read);
	pthread_mutex_unlock(&c_buffer->door_write);

	return space + space_to_end;
}

ssize_t CBufferRead(void *dest, c_buffer_t *c_buffer, size_t nbytes)
{
	size_t space = 0;
	size_t space_to_end = 0;

	assert(c_buffer);

	sem_wait(&c_buffer->can_read);
	pthread_mutex_lock(&c_buffer->door_read);

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

	sem_post(&c_buffer->can_write);
	pthread_mutex_unlock(&c_buffer->door_read);

	return space + space_to_end;
}

int CBufferIsEmpty(const c_buffer_t *c_buffer)
{
	assert(c_buffer);

	return (c_buffer->write == c_buffer->read);
}
