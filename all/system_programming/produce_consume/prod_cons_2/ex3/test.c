
#include <unistd.h>		/* */
#include <assert.h>		/* assert()	*/
#include <stdlib.h>		/* malloc(), free() */
#include <pthread.h>	/* pthread_create(), pthread_join() */

#include "write_print.h"
#include "safe_queue2.h"

#define THREAD_NUM		10
#define IS_EVEN(num)	(0 == (num) % 2)


typedef struct thread_info
{
	queue_t *que;
	int index;
} thread_info_t;

static int g_data1 = 5;

void Produce(queue_t *que, int *num)
{
	if (SQEnqueue(que, num))
	{
		PutStr("-- Produce Error\n");
		return ;
	}

	/*PutStr("-- Produced! ");
	PutNum(*num);
	PutChar('\n');*/

	return ;
}

size_t Consume(queue_t *que)
{
	int *num = NULL;

	if (!(num = SQDequeue(que)))
	{
		PutStr("-- Consume Error\n");
		return 0;
	}

	return *num;
}

void ManegProduce(queue_t *que)
{
	size_t i = 1;

	while (i)
	{
		Produce(que, &g_data1);
		--i;
	}
}

size_t ManegConsume(queue_t *que)
{
	return Consume(que);
}

void *GetJob(void *data)
{
	thread_info_t *info = data;

	assert(data);

	/*if (IS_EVEN(info->index))*/
	if (4 < info->index)
	{
		ManegProduce(info->que);
	}
	else
	{
		return (void*)ManegConsume(info->que);
	}

	return NULL;
}

int main()
{
	pthread_t thread[THREAD_NUM] = {0};
	size_t i = 0;
	size_t tmp = 0;
	size_t ret = 0;
	queue_t* que = NULL;
	thread_info_t info[THREAD_NUM] = {0};

	que = SafeQueueCreate();
	if (!que)
	{
		return -1;
	}

	for (i = 0; i < THREAD_NUM; ++i)
	{
		info[i].que = que;
		info[i].index = i;
	}

	for (i = 0; i < THREAD_NUM; ++i)
	{
		if (pthread_create(&thread[i], NULL, &GetJob, &info[i]))
		{
			SafeQueueDestroy(que);
			que = NULL;
			return -1;
		}
	}

	for (i = 0; i < THREAD_NUM ; ++i)
	{
		if(pthread_join(thread[i], (void**)&tmp))
		{
			SafeQueueDestroy(que);
			que = NULL;
			return -1;
		}
			ret += tmp;
	}

	PutNum(ret);
	PutChar('\n');

	SafeQueueDestroy(que);
	que = NULL;

	return 0;
}
