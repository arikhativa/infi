
#include <assert.h>     /* assert() */
#include <stddef.h>     /* size_t */
#include <stdlib.h>     /* calloc(), malloc() */
#include <string.h>     /* memset(), memcpy() */

#include "mt_count_sort.h"

typedef struct count_handler
{
    size_t *lut;
    size_t lut_size;
    char *arr;
    size_t arr_size;
    size_t elem_size;
    get_key_t getkey;
    void *param;
} count_handler_t;

static void FillLut(count_handler_t *info)
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

    for (i = (info->lut_size - 1); i; --i)
    {
        ++lut;
        *lut += *(lut - 1);
    }
}

static int FillSortArray(count_handler_t *info)
{
    char *sorted = 0;
    void *value_in_arr = 0;
    size_t i = 0;
    size_t sorted_index = 0;
    size_t lut_index = 0;

    assert(info);

    sorted = (char*)calloc(info->arr_size, info->elem_size);
	if (!sorted)
	{
		return 1;
	}

    value_in_arr = (void*)malloc(info->elem_size);
	if (!value_in_arr)
	{
        free(sorted);
		return 1;
	}

    for (i = info->arr_size; i; --i)
    {
        /* (info->arr + ((i - 1) * info->elem_size)):
            (i - 1) to get the correct index. currently i is size.
            (* elem_size) to move by the element size.              */
        memcpy(value_in_arr, (info->arr + ((i - 1) * info->elem_size)), info->elem_size);
        lut_index = info->getkey(value_in_arr, info->param);
		sorted_index = --info->lut[lut_index];
        memcpy(sorted + (sorted_index * info->elem_size), value_in_arr, info->elem_size);
    }

    info->arr = memcpy(info->arr, sorted, info->arr_size * info->elem_size);

	free(sorted);
	sorted = NULL;
	free(value_in_arr);
	value_in_arr = NULL;

    return 0;
}

int MTCountSort(void *arr, size_t arr_size, size_t elem_size,
                    size_t range, get_key_t getkey, void *param)
{
    count_handler_t info = {0};
    int status = 1;

    assert(arr);
    assert(getkey);

    info.lut = (size_t*)calloc(range, sizeof(size_t));
	if (!(info.lut))
	{
		return 1;
	}
    info.lut_size = range;
    info.arr = (char*)arr;
    info.arr_size = arr_size;
    info.elem_size = elem_size;
    info.getkey = getkey;
    info.param = param;

    FillLut(&info);
    status = FillSortArray(&info);

    free(info.lut);
    memset(&info, 0, sizeof(count_handler_t));

    return status;
}
