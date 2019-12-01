
#include <unistd.h>		/* */
#include <assert.h>		/* assert()	*/
#include <stdlib.h>		/* malloc(), free() */
#include <pthread.h>	/* pthread_create(), pthread_join() */

#include "write_print.h"
#include "cbuffer.h"

#define THREAD_NUM		100
#define ARR_SIZE		4
#define BUFF_SIZE		1
#define IS_EVEN(num)	(0 == (num) % 2)

typedef struct thread_info
{
	c_buffer_t *cbuff;
	int index;
	int i_con;
	int *data;
} thread_info_t;


void Produce(c_buffer_t *cbuff, int *num)
{
	CBufferWrite(cbuff, num, sizeof(int));

	/*PutStr("-- Produced! ");
	PutNum(*num);
	PutChar('\n');*/
}

int Consume(c_buffer_t *cbuff)
{
	int ret = 0;

	CBufferRead(&ret, cbuff, sizeof(int));

/*	PutStr("-- Consumed!");
	PutNum(*num);
	PutChar('\n');*/

	return ret;
}

void ManegProduce(c_buffer_t *cbuff, int *arr)
{
	size_t i = 0;

	for (i = 0; i < ARR_SIZE; ++i)
	{
		Produce(cbuff, &arr[i]);
	}
}

size_t ManegConsume(c_buffer_t *cbuff)
{
	size_t i = 0;
	int ret = 0;

	for (i = 0; i < ARR_SIZE; ++i)
	{
		ret += Consume(cbuff);
	}

	return ret;
}

void *GetJob(void *data)
{
	thread_info_t *info = data;

	assert(data);

	if (!(IS_EVEN(info->index)))
	{
		ManegProduce(info->cbuff, info->data);
	}
	else
	{
		return (void*)ManegConsume(info->cbuff);
	}

	return NULL;
}

int main()
{
	pthread_t thread[THREAD_NUM] = {0};
	size_t i = 0;
	size_t tmp = 0;
	size_t ret = 0;
	c_buffer_t * cbuff = NULL;
	thread_info_t info[THREAD_NUM] = {0};
	int arr[] = {1, 2, 3, 4};

	cbuff = CBufferCreate(BUFF_SIZE * sizeof(int));
	if (!cbuff)
	{
		return -1;
	}

	for (i = 0; i < THREAD_NUM; ++i)
	{
		info[i].cbuff = cbuff;
		info[i].index = i;
		info[i].data = arr;
	}

	for (i = 0; i < THREAD_NUM; ++i)
	{
		if (pthread_create(&thread[i], NULL, &GetJob, &info[i]))
		{
			CBufferDestroy(cbuff);
			cbuff = NULL;
			return -1;
		}
	}

	for (i = 0; i < THREAD_NUM ; ++i)
	{
		if(pthread_join(thread[i], (void**)&tmp))
		{
			PutStr("error\n");
			CBufferDestroy(cbuff);
			cbuff = NULL;
			return -1;
		}
		ret += tmp;
	}

	PutNum(ret);
	PutChar('\n');

	CBufferDestroy(cbuff);
	cbuff = NULL;

	return 0;
}
