
#include <stddef.h>	/* size_t			*/
#include <assert.h>	/* assert()			*/
#include <string.h>	/* memcpy()			*/
#include <stdlib.h>	/* malloc(), calloc(), free()	*/

#include "sorting.h"

#define NUM_OF_DECIMAL_DIGITS 10
#define BASE 10
#define GET_DIGIT(NUM, DIVIDER) ((NUM / DIVIDER) % BASE)

typedef struct quick_sort
{
	void *arr;
	int size;
	int elem_size;
	cmp_func_t cmp;
} quick_sort_t;


static int FillSortArray(int *arr, size_t size_of_arr, int *lut, int min_value);
static void FillLut(int *lut, size_t size_of_lut, int *arr, size_t size_of_arr, int min_value);
static void Swap(int *a, int *b);
static void Insert(int *start, int *end);
static int StortArrayByLut(unsigned int *dest, unsigned int *src, size_t size, unsigned int *lut, int digit);
static int GetDivider(unsigned int digit);
static int CountRadix(unsigned int *arr, size_t size, size_t max_digits);
static size_t CountDigits(unsigned int num);
static void SwapPtr(unsigned int **a, unsigned int **b);
static unsigned int GetLargestNumFromArray(unsigned int *arr, size_t size);
static void FillLutByDigit(unsigned int *lut, unsigned int *arr, size_t size, unsigned int digit);

int FlexibleSwap(void *a, void *b, size_t size_of_elem);


int *BubbleSort(int *arr, size_t size)
{
	int *run = 0;
	int *end = 0;
	int swaped = 1;

	assert(arr);

	run = arr;
	end = (arr + size);

	while (swaped)
	{
		swaped = 0;

		while ((run + 1) != end)
		{
			if (*run > *(run + 1))
			{
				Swap(run, (run + 1));
				swaped = 1;
			}

			++run;
		}

		run = arr;
	}

	return arr;
}

int *SelectionSort(int *arr, size_t size)
{
	int *lowest = 0;
	int *run = 0;
	int *start = 0;
	int *end = 0;

	assert(arr);

	start = arr;
	lowest = arr;
	run = (arr + 1);
	end = (arr + size);

	while (start != end)
	{
		while (run != end)
		{
			if (*lowest > *run)
			{
				lowest = run;
			}
			++run;
		}
		Swap(start, lowest);
		++start;
		lowest = start;
		run = (start + 1);
	}

	return arr;
}


int *InsertionSort(int *arr, size_t size)
{
	int *begin_to_run = arr;
	int *run = arr;
	int *end = arr + size;

	assert(arr);

	while (run != end)
	{
		while (begin_to_run != run)
		{
			if (*begin_to_run > *run)
			{
				Insert(begin_to_run, run);
				break ;
			}
			++begin_to_run;
		}
		begin_to_run = arr;
		++run;
	}

	return arr;
}

static void Insert(int *start, int *end)
{
	int tmp = 0;

	assert(start);
	assert(end);

	tmp = *end;

	while (start != end)
	{
		*end = *(end - 1);
		--end;
	}

	*end = tmp;
}

int *OptimizedBubbleSort(int *arr, size_t size)
{
	int *run = arr;
	int *tmp_end = arr;
	int *end = arr + size;
	int swaped = 1;

	assert(arr);

	while (swaped)
	{
		swaped = 0;
		while ((run + 1) != end)
		{
			if (*run > *(run + 1))
			{
				Swap(run, (run + 1));
				swaped = 1;
				tmp_end = run;
			}
			++run;
		}
		end = tmp_end + 1;
		run = arr;
	}

	return arr;
}

static void Swap(int *a, int *b)
{
	int swap = 0;

	assert(a);
	assert(b);

	swap = *a;
	*a = *b;
	*b = swap;
}

int CountSort(int *arr, size_t size, int min, int max)
{
	int status = 1;
	int *lut = 0;
	size_t size_of_lut = max - min + 1;

	assert(arr);

	lut = (int*)calloc(size_of_lut, sizeof(int));
	if (!lut)
	{
		return 1;
	}

	FillLut(lut, size_of_lut, arr, size, min);
	status = FillSortArray(arr, size, lut,  min);

	free(lut);

	return status;
}

static void FillLut(int *lut, size_t size_of_lut, int *arr, size_t size_of_arr, int min_value)
{
	int offset = min_value;

	for (size_of_arr += 0; size_of_arr; --size_of_arr)
	{
		++lut[*arr - offset];
		++arr;
	}

	++lut;

	for (--size_of_lut; size_of_lut; --size_of_lut)
	{
		*lut += *(lut - 1);
		++lut;
	}
}

static int FillSortArray(int *arr, size_t size_of_arr, int *lut, int min_value)
{
	int *sorted = 0;
	int value_in_arr = 0;
	int offset = min_value;
	size_t sorted_index = 0;
	size_t index = 0;

	assert(arr);
	assert(lut);

	sorted = (int*)calloc(size_of_arr, sizeof(int));
	if (!sorted)
	{
		return 1;
	}

	for (index = size_of_arr; index; --index)
	{
		value_in_arr = arr[index - 1];
		sorted_index = --lut[value_in_arr - offset];
		sorted[sorted_index] = value_in_arr;
	}

	arr = memcpy(arr, sorted, sizeof(int) * size_of_arr);

	free(sorted);
	sorted = NULL;

	return 0;
}


int RadixSort(unsigned int *arr, size_t size)
{
	int status = 0;
	unsigned int max_num = 0;
	size_t max_digits = 0;

	assert(arr);

	max_num = GetLargestNumFromArray(arr, size);
	max_digits = CountDigits(max_num);

	status = CountRadix(arr, size, max_digits);

	return status;
}

static unsigned int GetLargestNumFromArray(unsigned int *arr, size_t size)
{
	size_t index = 0;
	unsigned int max_num = 0;

	for (index = 0; index < size; ++index)
	{
		if (max_num < arr[index])
		{
			max_num = arr[index];
		}
	}

	return max_num;
}

static size_t CountDigits(unsigned int num)
{
	size_t digits = 0;

	while (num)
	{
		num /= BASE;
		++digits;
	}

	return digits;
}

static int CountRadix(unsigned int *arr, size_t size, size_t max_digits)
{
	unsigned int lut[NUM_OF_DECIMAL_DIGITS] = { 0 };
	unsigned int *dest = 0;
	unsigned int *src = arr;
	size_t digit = 1;

	assert(arr);

	dest = (unsigned int*)calloc(size, sizeof(unsigned int));
	if (!dest)
	{
		return 1;
	}

	for (digit = 1; digit <= max_digits; ++digit)
	{
		FillLutByDigit(lut, src, size, digit);
		StortArrayByLut(dest, src, size, lut, digit);
		memset(lut, 0, sizeof(int) * BASE);
		SwapPtr(&dest, &src);
	}

	/* If 'max_digits' is even, it means then 'dest' is pointing to the sorted array.
	 * We need to copy 'dest' to 'src' and swap to free the allocated memry.	*/

	if (max_digits % 2)
	{
		memcpy(dest, src, sizeof(int) * size);
		SwapPtr(&dest, &src);
	}

	free(dest);
	dest = NULL;

	return 0;
}

static void FillLutByDigit(unsigned int *lut, unsigned int *arr, size_t size, unsigned int digit)
{
	unsigned int index_lut = 0;
	int divider = 1;
	int size_of_lut = NUM_OF_DECIMAL_DIGITS;

	divider = GetDivider(digit);

	for (size += 0; size; --size)
	{
		index_lut = GET_DIGIT(*arr, divider);
		++lut[index_lut];
		++arr;
	}

	++lut;
	for (--size_of_lut; size_of_lut; --size_of_lut)
	{
		*lut += *(lut - 1);
		++lut;
	}
}

static int GetDivider(unsigned int digit)
{
	int divider = 1;

	while (--digit)
	{
		divider *= BASE;
	}

	return divider;
}

static int StortArrayByLut(unsigned int *dest, unsigned int *src, size_t size, unsigned int *lut, int digit)
{
	int value_in_src = 0;
	unsigned int sorted_index = 0;
	size_t index = 0;
	int divider = 1;

	assert(src);
	assert(dest);
	assert(lut);

	divider = GetDivider(digit);

	for (index = size; index; --index)
	{
		value_in_src = src[index - 1];
		sorted_index = --lut[GET_DIGIT(value_in_src, divider)];
		dest[sorted_index] = value_in_src;
	}

	return 0;
}

static void SwapPtr(unsigned int **a, unsigned int **b)
{
	unsigned int *swap = 0;

	assert(a);
	assert(b);

	swap = *a;
	*a = *b;
	*b = swap;
}


void PrintArray(int *arr, size_t size);

/*printf("\n\n******* BEFORE******8 \n");
  PrintArray(res, 4);
  printf("           res\n");
  PrintArray(arr, 4);
  printf("           arr\n");*/

static int Marge(int *res, int start, int mid, int end, is_before_t IsBefore)
{
	int left = 0;
	int right = 0;
	int index = start;
	int size_l = mid - start;
	int size_r = end - mid;
	int *arr_l = NULL;
	int *arr_r = NULL;
	int i = 0;

	arr_l = (int*)malloc(size_l * sizeof(int));
	if (!arr_l)
	{
		return 1;
	}

	arr_r = (int*)malloc(size_r * sizeof(int));
	if (!arr_r)
	{
		free(arr_l);
		return 1;
	}

	for (i = 0; i < size_l; ++i)
	{
		arr_l[i] = res[start + i];
	}
	for (i = 0; i < size_r; ++i)
	{
		arr_r[i] = res[mid + i];
	}

	while (left < size_l && right < size_r)
	{
		if (IsBefore(arr_l[left], arr_r[right]))
		{
			res[index] = arr_r[right];
			++right;
		}
		else
		{
			res[index] = arr_l[left];
			++left;
		}

		++index;
	}

	while (left < size_l)
	{
		res[index] = arr_l[left];
		++left;
		++index;
	}

	while (right < size_r)
	{
		res[index] = arr_r[right];
		++right;
		++index;
	}


	free(arr_l);
	free(arr_r);

	return 0;
}

static int RecMarge(int *res, int start, int end, is_before_t IsBefore)
{
	int mid = 0;
	int stt = 0;

	if (end - start < 2)
		return 0;

	mid = (start + end) / 2;

	stt = RecMarge(res, start, mid, IsBefore);
	if (stt)
		return 1;

	stt = RecMarge(res, mid, end, IsBefore);
	if (stt)
		return 1;

	return Marge(res, start, mid, end, IsBefore);
}

int *MargeSort(int *arr, size_t size, is_before_t IsBefore)
{
	int stt = 0;

	assert(arr);
	assert(IsBefore);

	stt = RecMarge(arr, 0, size, IsBefore);

	if (stt)
		return NULL;

	return arr;
}

static void *CalcPivot(quick_sort_t *handel, int start, int end)
{
	char *arr = NULL;
	int random = 0;

	arr = (char*)handel->arr;

	random = ((end / 2) + 1);

	if (random > end || random < start)
	{
		random = end;
	}

	FlexibleSwap(arr + (random * handel->elem_size),
			arr + (end * handel->elem_size), handel->elem_size);

	return (arr + (end * handel->elem_size));
}

size_t Partition(quick_sort_t *handel, int start, int end)
{
	void *pivot = NULL;
	char *arr = NULL;
	int i = 0;
	int n = start;

	assert(handel);

	pivot = CalcPivot(handel, start, end);

	arr = (char*)handel->arr;

	for (i = start; i < end ; ++i)
	{
		if (-1 == handel->cmp((arr + (i * handel->elem_size)), pivot))
		{
			FlexibleSwap(arr + (n * handel->elem_size),
					arr + (i * handel->elem_size), handel->elem_size);

			++n;
		}
	}

	FlexibleSwap(arr + (n * handel->elem_size), pivot, handel->elem_size);

	return n;
}

void RecQuickSort(quick_sort_t *handel, int start, int end)
{
	int pivot_index = 0;

	assert(handel);

	if (start < end)
	{
		pivot_index = Partition(handel, start, end);

		RecQuickSort(handel, start, pivot_index - 1); /* Recursive to the left */
		RecQuickSort(handel, pivot_index + 1, end); /* Recursive to the right */
	}
}


void QuickSort(void *arr, size_t size, size_t elem_size, cmp_func_t cmp)
{
	quick_sort_t handel = {0};

	assert(arr);

	handel.arr = arr;
	handel.size = size;
	handel.elem_size = elem_size;
	handel.cmp = cmp;

	RecQuickSort(&handel, 0, size - 1);
}

int FlexibleSwap(void *a, void *b, size_t size_of_elem)
{
	void *tmp = NULL;

	assert(a);
	assert(b);

	tmp = malloc(size_of_elem);
	if (!tmp)
	{
		return 1;
	}

	memcpy(tmp, a, size_of_elem);
	memcpy(a, b, size_of_elem);
	memcpy(b, tmp, size_of_elem);

	free(tmp);
	tmp = NULL;

	return 0;
}












