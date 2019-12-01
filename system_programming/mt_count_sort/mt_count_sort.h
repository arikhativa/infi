
#ifndef __MT_COUNT_SORT_H__
#define __MT_COUNT_SORT_H__

typedef size_t (*get_key_t)(void *data, void *param);

int MTCountSort(void *arr, size_t arr_size, size_t elem_size,
                    size_t range, get_key_t getkey, void *param);

#endif /* __MT_COUNT_SORT_H__ */
