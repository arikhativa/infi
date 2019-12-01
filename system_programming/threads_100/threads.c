

#include <unistd.h>
#include <pthread.h>

#include "write_print.h"

#define ARR_SIZE	100000
#define TH_NUM		ARR_SIZE

static int arr[ARR_SIZE] = {0};

void *Thread_func(void *a)
{
	int *array = a;

	//pthread_detach(pthread_self());

	*array = (((size_t)array - (size_t)arr) / sizeof(int)) + 1;

	return NULL;
}

int main()
{
	pthread_t thread[TH_NUM];
	int index = 0;
	pthread_attr_t attr = {0};
	int ret = 0;

	ret = pthread_attr_init(&attr);
	ret += pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (ret)
	{
		PutStr("-- fail ret \n");
		return -1;
	}

	for (index = 0; index < ARR_SIZE; ++index)
	{

		if (pthread_create(&thread[index], &attr, &Thread_func, (arr + index)))
		//if (pthread_create(&thread[index],NULL, &Thread_func, (arr + index)))
		{
			PutStr("-- fail create \n");
			return -1;
		}

	}
	sleep(10);

	for (index = 0; index < ARR_SIZE ; ++index)
	{
		if (index + 1 != arr[index])
		{
			PutStr("-- fail end \n");
			return -1;
		}
	}

	pthread_attr_destroy(&attr);
	PutStr("-- SUCCESSSECCS \n");

	return 0;
}












/*
	int j = 500;
	while (--j)

{
	*i += 5;
	*i -= 5;
}*/
