

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "write_print.h"

#define ARR_SIZE	10
#define THREAD_NUM	2
#define ROUNDS	1000

typedef enum is_busy
{
	T1 = 0,
	T2
} is_busy_t;

static int arr[ARR_SIZE] = {0};
static volatile is_busy_t is_busy = T1;

void WaitForT2(void)
{
	while (T1 == is_busy)
	{}
}

void WaitForT1(void)
{
	while (T2 == is_busy)
	{}
}

void SetToT2(void)
{
	__sync_add_and_fetch(&is_busy, 1);
}

void SetToT1(void)
{
	__sync_sub_and_fetch(&is_busy, 1);
}

static void *ThreadFunc1(void *data)
{
	size_t i = 0;
	size_t round = 0;

	(void)data;

	for (round = 0; round < ROUNDS; ++round)
	{
		WaitForT1();
		for (i = 0; i < ARR_SIZE; ++i)
		{
			++arr[i];
		}
		SetToT2();
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
		WaitForT2();
		for (i = 0; i < ARR_SIZE; ++i)
		{
			sum += arr[i];
		}
		PutStr("-- sum is ");
		PutNum(sum);
		PutChar('\n');
		sum = 0;
		SetToT1();
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
