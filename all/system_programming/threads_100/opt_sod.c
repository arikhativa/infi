

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "write_print.h"

#define NUM					(1000000066)
#define THREAD_NUM			(4)

static const size_t origin = NUM;
static size_t range;

size_t SumOfDivisors(size_t start);

void *Thread_func(void *a)
{
	size_t start = (size_t)a;

	return (void*)SumOfDivisors(start);
}

size_t SumOfDivisors(size_t start)
{
	size_t sum = 0;
	size_t run = start;

	while (run < (start + range))
	{
		if (!(origin % run))
		{
			sum += run;
		}
		++run;
	}

/*	PutStr("-- sum is ");
	PutNum(sum);
	PutChar('\n');*/

	return sum;
}

size_t AddLastRange(size_t range)
{
	size_t end = origin / 2;
	size_t start = end - range;
	size_t sum = 0;

	if (!range)
	{
		return 0;
	}

	for (sum = 0; start <= end ; ++start)
	{
		if (!(origin % start))
		{
			sum += start;
		}
	}

	return sum;
}

size_t CalcRange(void)
{
	size_t mid = origin / 2;

	range = mid / THREAD_NUM;

	return mid % THREAD_NUM;
}

int main()
{
	pthread_t thread[THREAD_NUM] = {0};
	size_t index = 0;
	size_t ret = 0;
	size_t tmp = 0;
	size_t last_range = 0;

	last_range = CalcRange();

	for (index = 0; index < THREAD_NUM; ++index)
	{
		if (pthread_create(&thread[index], NULL, &Thread_func, (void*)(index * range + 1)))
		{
			return -1;
		}
	}

	for (index = 0; index < THREAD_NUM ; ++index)
	{
		if(pthread_join(thread[index], (void**)&tmp))
		{
			return -1;
		}
		ret += tmp;
	}

	ret += AddLastRange(last_range);

	ret += origin;
	printf("-- sum is %ld\n", ret);

	return 0;
}
