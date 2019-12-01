#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>	/* pthread_create(), pthread_join() */



#include "color.h"
#include "sorting.h"

#define B1(n) n, n, n, n
#define B2(n) B1(n), B1(n), B1(n), B1(n)
#define B3(n) B2(n), B2(n), B2(n), B2(n)
#define BIT B3(0), B3(0), B3(0), B3(0)

#define B1(n) n, n, n, n
#define B2(n) B1(n), B1(n), B1(n), B1(n)
#define B3(n) B2(n), B2(n), B2(n), B2(n)
#define BIT1 B3(1), B3(1), B3(1), B3(1)

#define R1(n) n, n+1, n+2, n+3
#define R2(n) R1(n), R1(n+4), R1(n+8), R1(n+12)
#define R3(n) R2(n), R2(n+16), R2(n+32), R2(n+48)
#define NUM_IN_CHAR R3(0), R3(64), R3(128), R3(192)

#define NUM_OF_THREADS  4

typedef struct info
{
	void *data;
	size_t size;
} info_t;

void SwapPtr(void **a, void **b)
{
	void *tmp = *a;
	*a = *b;
	*b = tmp;
}

int CMP(const void *data1, const void *data2)
{
	char a = **(char**)data1;
	char b = **(char**)data2;

	if (!('a' <= a && 'z' >= a) && !('A' <= a && 'Z' >= a))
	{
		a = 126;
	}
	if (!('a' <= b && 'z' >= b) && !('A' <= b && 'Z' >= b))
	{
		b = 126;
	}

	if (a < b)
	{
		return -1;
	}
	if (a > b)
	{
		return 1;
	}

	return 1;
}

char **CreateDic(void)
{
	char **dic = 0;
	char *dump = 0;
	FILE *fd = 0;
	size_t i = 0;
	size_t dic_size = 102305;

	fd = fopen("/usr/share/dict/american-english", "r");
	dic = (char**)malloc(sizeof(char*) * dic_size);

	for (i = 0; i < dic_size; ++i)
	{
		dic[i] = (char*)malloc(30);
		dump = fgets(dic[i], 30, fd);
		if (!dump)
		{
			exit(1);
		}
	}

	fclose(fd);

	return dic;
}

void DestroyDic(char **dic)
{
	size_t i = 0;
	size_t dic_size = 102305;

	for (i = 0; i < dic_size; ++i)
	{
		free(dic[i]);
	}
	free(dic);
}

void Shuffle(char **dic)
{
	size_t i = 0;
	size_t dic_size = 50000;

	for (i = 0; i < dic_size; i += 2)
	{
		SwapPtr((void**)&dic[i], (void**)&dic[i + 40000]);
	}
}

size_t GetKeyDic(void *data, void *param)
{
	char **str = data;

	(void)param;

	if ('a' <= **str && 'z' >= **str)
	{
		return **str - 'a';
	}
	if ('A' <= **str && 'Z' >= **str)
	{
		return **str - 'A';
	}

	return 26;
}


void BadDictionary()
{
	char **dic = 0;
	size_t dic_size = 102305;

	dic = CreateDic();

	Shuffle(dic);

	qsort(dic, dic_size, sizeof(char*), CMP);

	DestroyDic(dic);
}

int CMP4(const void *data1, const void *data2)
{
	if (*(int*)data1 > *(int*)data2)
	{
		return 1;
	}
	return -1;
}

void QuickSort4(void *arr, size_t size, size_t elem_size, cmp_func_t cmp);

void *Hendler(void *data)
{
	info_t *info = data;
	char **dic = info->data;

	qsort(dic, info->size, sizeof(char*), CMP);

	return NULL;
}

void GoodDictionary()
{
	size_t i = 0;
	char **dic = 0;
	size_t dic_size = 102305;
	size_t small_size = dic_size / 4;
	info_t info[NUM_OF_THREADS] = {0};
	pthread_t thread[NUM_OF_THREADS] = {0};

	dic = CreateDic();

	Shuffle(dic);

	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		info[i].data = (char*)dic + (i * small_size * 8);
		info[i].size = small_size;
	}

	for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        if(pthread_create(&thread[i], NULL, Hendler, &info[i]))
        {
			printf("-- fail\n");
            return ;
        }
    }

	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		if(pthread_join((thread[i]), NULL))
		{
			printf("-- fail\n");
			return ;
		}
	}

	//QuickSort4(dic, dic_size, sizeof(char*), CMP);
	//qsort(dic, dic_size, sizeof(char*), CMP);

	for (i = 0; i < dic_size; ++i)
	{
			printf("%s", dic[i]);
	}

	DestroyDic(dic);
}

int main(int ac, char **av)
{
	int c = 0;

	if (2 <= ac)
	{
		c = atoi(av[1]);
	}
	switch (c)
	{
		case 1:
			GoodDictionary();
			break ;
		case 2:
			BadDictionary();
			break ;
	}

	return 0;
}
