
#include <assert.h>     /* assert() */
#include <stddef.h>     /* size_t */
#include <stdlib.h>     /* calloc(), malloc() */
#include <string.h>     /* memset(), memcpy() */
#include <pthread.h>	/* pthread_create(), pthread_join() */
#include <semaphore.h>  /* sem_init(), sem_wait(), sem_post, sem_destroy */


#include "mt_count_sort.h"
#include "write_print.h" /**** del!!! 14 **/
#include <stdio.h>	/**** del!!! 14 **/
#include <unistd.h>/**** del!!! 14 **/


#define NUM_OF_THREADS  4

enum safety_net_status
{
    LOCK = 0,
    OPEN
};

enum status
{
    SUCCESS = 0,
    FAIL
};

typedef struct thread_fill_handler
{
    size_t *lut;
    char *arr;
    char *sorted;
    size_t start;
    size_t end;
    size_t elem_size;
    get_key_t getkey;
    void *param;

} fill_handel_t;

typedef struct count_handler
{
    pthread_t *thread;
    fill_handel_t *hdlr;
    size_t *lut;
    size_t lut_size;
    char *arr;
    char *sorted;
    size_t arr_size;
    size_t elem_size;
    get_key_t getkey;
    void *param;
} count_handler_t;


/*
    Global var:
*/
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;
static sem_t g_sem;
static int g_safety_net = LOCK;


static void MakeFillHndler(fill_handel_t *dest, count_handler_t *info, size_t index)
{
    dest->lut = info->lut;
    dest->arr = info->arr;
    dest->sorted = info->sorted;
    dest->elem_size = info->elem_size;
    dest->getkey = info->getkey;
    dest->param = info->param;

    dest->start = index * (info->arr_size / NUM_OF_THREADS);

    if (index == NUM_OF_THREADS - 1) /* if last index */
    {
        dest->end = info->arr_size;
    }
    else
    {
        dest->end = ((index + 1) * (info->arr_size / NUM_OF_THREADS));
    }
}

static void ThreadFillLut(fill_handel_t *hdlr)
{
    size_t i = 0;
    size_t index = 0;
    char *arr = 0;

    assert(hdlr);

    arr = hdlr->arr + (hdlr->start * hdlr->elem_size);

    for (i = hdlr->end; i != hdlr->start; --i)
    {
        index = hdlr->getkey(arr, hdlr->param);
        __sync_add_and_fetch(&hdlr->lut[index], 1);
        arr += hdlr->elem_size;
    }
}

static void Barrier(void)
{
    pthread_mutex_lock(&g_mutex);

    sem_post(&g_sem);

    do
    {
        pthread_cond_wait(&g_cond, &g_mutex);
    } while(LOCK == g_safety_net);

    pthread_mutex_unlock(&g_mutex);
}

static void ThreadWriteInResult(fill_handel_t *hdlr)
{
    char *arr = 0;
    char *sorted = 0;

    assert(hdlr);

    arr = hdlr->arr + (hdlr->start * hdlr->elem_size);
    sorted = hdlr->sorted + (hdlr->start * hdlr->elem_size);

    memcpy(arr, sorted, (hdlr->end - hdlr->start) * hdlr->elem_size);
}

static void *ThreadHandler(void *data)
{
    fill_handel_t *hdlr = data;

    assert(data);

    ThreadFillLut(hdlr);

    Barrier();

    ThreadWriteInResult(hdlr);

    return NULL;
}

/*static int FillLut(count_handler_t *info)
{
    char *arr = info->arr;
    size_t *lut = info->lut;
    size_t i = 0;
    size_t index = 0;

    assert(info);

    for (i = info->arr_size; i; --i)
    {
        index = info->getkey(arr , info->param);
        ++(lut[index]);
        arr += info->elem_size;
    }

    ++lut;

    for (i = (info->lut_size - 1); i; --i)
    {
        *lut += *(lut - 1);
        ++lut;
    }
    return SUCCESS;

}*/

static int FillLut(count_handler_t *info)
{
    size_t *lut = info->lut;
    size_t i = 0;

    assert(info);

    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        MakeFillHndler(&(info->hdlr[i]), info, i);
        if(pthread_create(&(info->thread[i]), NULL, ThreadHandler, &(info->hdlr[i])))
        {
            return FAIL;
        }
    }

    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        sem_wait(&g_sem);
    }
    pthread_mutex_lock(&g_mutex);

    ++lut;

    for (i = (info->lut_size - 1); i; --i)
    {
        *lut += *(lut - 1);
        ++lut;
    }

    return SUCCESS;
}

static int FillSortArray(count_handler_t *info)
{
    void *value_in_arr = 0;
    size_t i = 0;
    size_t ret = SUCCESS;
    size_t sorted_index = 0;
    size_t lut_index = 0;

    assert(info);

    value_in_arr = (void*)malloc(info->elem_size);
	if (!value_in_arr)
	{
		return FAIL;
	}

    for (i = info->arr_size; i; --i)
    {
        memcpy(value_in_arr, (info->arr + ((i - 1) * info->elem_size)), info->elem_size);

        lut_index = info->getkey(value_in_arr, info->param);

		sorted_index = --info->lut[lut_index];

        memcpy(info->sorted + (sorted_index * info->elem_size), value_in_arr, info->elem_size);
    }

/*    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        MakeFillHndler(&(info->hdlr[i]), info, i);
        if(pthread_create(&(info->thread[i]), NULL, ThreadHandler, &(info->hdlr[i])))
        {
            return FAIL;
        }
    }*/


    /*memcpy(info->arr, info->sorted, info->arr_size * info->elem_size);*/

    g_safety_net = OPEN;
    pthread_cond_broadcast(&g_cond);
    pthread_mutex_unlock(&g_mutex);

    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        if(pthread_join((info->thread[i]), NULL))
        {
            ret = FAIL;
            break ;
        }
    }

	free(value_in_arr);
	value_in_arr = NULL;

    return ret;
}

int MTCountSort(void *arr, size_t arr_size, size_t elem_size,
                    size_t range, get_key_t getkey, void *param)
{
    pthread_t thread[NUM_OF_THREADS] = {0};
    fill_handel_t hdlr[NUM_OF_THREADS] = {0};
    count_handler_t info = {0};
    int status = 1;

    assert(arr);
    assert(getkey);

    info.thread = thread;
    info.hdlr = hdlr;
    info.arr = (char*)arr;
    info.arr_size = arr_size;
    info.lut_size = range;
    info.elem_size = elem_size;
    info.getkey = getkey;
    info.param = param;

    info.lut = (size_t*)calloc(range, sizeof(size_t));
	if (!(info.lut))
	{
		return FAIL;
	}

    if (sem_init(&g_sem, 0, 0))
    {
        free(info.lut);
        memset(&info, 0, sizeof(count_handler_t));
        return FAIL;
    }

    info.sorted = (char*)calloc(info.arr_size, info.elem_size);
    if (!info.sorted)
    {
        free(info.lut);
        memset(&info, 0, sizeof(count_handler_t));
        sem_destroy(&g_sem);
        return FAIL;
    }

    status = FillLut(&info);
    if (SUCCESS == status)
    {
        status = FillSortArray(&info);
    }

    free(info.sorted);
    free(info.lut);
    sem_destroy(&g_sem);
    memset(&info, 0, sizeof(count_handler_t));

    return status;
}
