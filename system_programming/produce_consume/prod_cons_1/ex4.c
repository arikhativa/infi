

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "write_print.h"

#define ARR_SIZE	10000
#define THREAD_NUM	2
#define ROUNDS	10000

typedef enum bool
{
	OFF = 0,
	ON
} bool_t;

static int g_arr[ARR_SIZE] = {0};
static volatile bool_t g_to_produce = ON;
static volatile bool_t g_to_consume = OFF;

void WaitForProduce(void)
{
	while (OFF == g_to_produce)
	{}
}

void WaitForConsume(void)
{
	while (OFF == g_to_consume)
	{}
}

void SetProduce(bool_t flag)
{

	__sync_lock_test_and_set (&g_to_produce, flag);
}

void SetConsume(bool_t flag)
{
	__sync_lock_test_and_set (&g_to_consume, flag);
}

static void *ThreadFunc1(void *data)
{
	size_t i = 0;
	size_t round = 0;

	(void)data;

	for (round = 0; round < ROUNDS; ++round)
	{
		WaitForProduce();
		SetProduce(OFF);

		for (i = 0; i < ARR_SIZE; ++i)
		{
			++g_arr[i];
		}

		SetConsume(ON);
	}

	return (void*)0;
}

static void *ThreadFunc2(void *data)
{
	size_t i = 0;
	size_t round = 0;
	size_t sum = 0;

	(void)data;

	for (round = 0; round < ROUNDS; ++round)
	{
		WaitForConsume();
		SetConsume(OFF);

		for (i = 0; i < ARR_SIZE; ++i)
		{
			sum += g_arr[i];
		}

		PutStr("-- sum is ");
		PutNum(sum);
		PutChar('\n');
		sum = 0;

		SetProduce(ON);
	}

	return (void*)sum;
}

int main()
{
	pthread_t thread[THREAD_NUM] = {0};
	size_t i = 0;
	size_t tmp = 0;
	size_t ret = 0;

	if (pthread_create(&thread[0], NULL, &ThreadFunc1, NULL))
	{
		return -1;
	}

	if (pthread_create(&thread[1], NULL, &ThreadFunc2, NULL))
	{
		return -1;
	}

	for (i = 0; i < THREAD_NUM ; ++i)
	{
		if(pthread_join(thread[i], (void**)&tmp))
		{
			return -1;
		}
		ret += tmp;
	}

	return 0;
}
