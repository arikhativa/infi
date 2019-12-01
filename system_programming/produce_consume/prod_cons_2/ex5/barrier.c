#include <pthread.h>	/* pthread_create(), pthread_join() */
#include <semaphore.h>  /* semaphore */

#include "write_print.h"

#define THREAD_NUM          11
#define NUM_OF_CONSUME		(THREAD_NUM - 1)
#define LIMIT               9
#define ON                  1
#define OFF                 0

static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t g_door = PTHREAD_MUTEX_INITIALIZER;
static sem_t g_sem;
static int g_data;
static int g_can_consume;
static int g_sum;

void *Produce(void *param)
{
    size_t i = 0;

    (void)param;

    while (g_data < LIMIT)
    {
        for (i = 0; i < NUM_OF_CONSUME; ++i)
        {
            sem_wait(&g_sem);
        }

        g_can_consume = OFF;
        pthread_mutex_lock(&g_door);

        g_data += 1;

        g_can_consume = ON;
        pthread_cond_broadcast(&g_cond);
        pthread_mutex_unlock(&g_door);
    }

    return NULL;
}

void *Consume(void *param)
{
    (void)param;

    while (g_data < LIMIT)
    {
        pthread_mutex_lock(&g_door);

        sem_post(&g_sem);

        do
        {
            pthread_cond_wait(&g_cond, &g_door);
        } while (OFF == g_can_consume);

        g_sum += g_data;
        pthread_mutex_unlock(&g_door);
    }

    return NULL;
}


int main()
{
    pthread_t thread[THREAD_NUM] = {0};
    size_t i = 0;
    size_t tmp = 0;

    if (sem_init(&g_sem, 0, 0))
    {
        return -1;
    }

    if (pthread_create(&thread[i], NULL, &Produce, NULL))
    {
        return -1;
    }

    for (i = 1; i < THREAD_NUM; ++i)
	{
		if (pthread_create(&thread[i], NULL, &Consume, NULL))
		{
            PutStr("error\n");
			return -1;
		}
	}

    for (i = 0; i < THREAD_NUM ; ++i)
    {
        if(pthread_join(thread[i], (void**)&tmp))
        {
            PutStr("error\n");
            return -1;
        }
    }

    PutNum(g_sum);
    PutChar('\n');

    pthread_mutex_destroy(&g_door);
    sem_destroy(&g_sem);
    pthread_cond_destroy(&g_cond);

    return 0;
}
