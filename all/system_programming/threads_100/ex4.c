

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define NUM					(25571520)
#define RANGE				(1000)
#define MAX_NUM_OF_THREADS	(30000)

static const size_t origin = NUM;

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

	while (run < (start + RANGE))
	{
		if (!(origin % run))
		{
			sum += run;
		}
		++run;
	}

	return sum;
}

size_t CalcNumOfThreads(size_t *last_range)
{
	size_t ths = origin / 2;

	*last_range = ths & RANGE;

	if (ths / RANGE > MAX_NUM_OF_THREADS)
	{
		return 0;
	}
	return ths / RANGE;
}

size_t AddLastRange(size_t range)
{
	size_t end = origin / 2;
	size_t start = end - range;
	size_t sum = 0;

	for (start = end - range; start <= end ; ++start)
	{
		if (!(origin % start))
		{
			sum += start;
		}
		++start;
	}

	return sum;
}
int main()
{
	pthread_t *thread = NULL;
	size_t index = 0;
	size_t ret = 0;
	size_t num_of_th = 0;
	size_t tmp = 0;
	size_t last_range = 0;

	if (!(num_of_th = CalcNumOfThreads(&last_range)))
	{
		return -1;
	}

	thread = (pthread_t*)malloc(sizeof(pthread_t) * num_of_th);
	if (!thread)
	{
		return -1;
	}

	for (index = 0; index < num_of_th; ++index)
	{
		if (pthread_create(&thread[index], NULL, &Thread_func, (void*)(index * RANGE + 1)))
		{
			return -1;
		}
	}

	for (index = 0; index < num_of_th ; ++index)
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
