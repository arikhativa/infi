#ifndef __SORTING_H__
#define __SORTING_H__

/*      ver. 1.4 - Added QuickSort. and is_cmp_func_t                                                
 *      ver. 1.3 - Added MargeSort. and is_before_t
 *      ver. 1.2 - Added CountingSort, and size_t.
 *      ver. 1.1 - Updated arguments with int size
 *      ver. 1.0 - Initial release.
 */

/*
 * is_before_t():
 *
 * Pointer to a function that determn the policy of the sorting.
 *
 * Argument:
 *		'data1' - an int in the array
 *		'data2' - the data that would need to be BEFORE 'data1' (if return value is '1')
 *
 * Retrun:
 * 		'1' - if 'data2' needs to be BEFORE 'data1'.
 * 		'0' - if 'data2' needs to be AFTER 'data1'.
*/

typedef int (*is_before_t)(int data1, int data2);

/*
 *	CmpFunc: 
 *	    compare two element's data
 *
 *	Argumentd:
 *		data1 - first data
 *      data2 - second data
 *   
 *
    Return:
 *              '1' if the data1 is 'larger' than  data2;
 *              '-1' if the data1 is 'smaller' than data2.
 *              '0' - if equals.
 *		
 */
typedef int (*cmp_func_t)(const void *data1, const void *data2);



/* Sort array using Bubble sorting method
 * Time complexity O(n^2).
 * Space complexity O(1).
 * Stability: Yes.
 * Argument:
 *		pointer to array
 *		size of array
 * Retrun: pointer to sorted array
 */
	int *BubbleSort(int *array, size_t size);

/* Sort array using Insertion sorting method
 * Time complexity O(n^2).
 * Space complexity O(1).
 * Stability: Yes.
 * Argument:
 *		pointer to array
 *		size of array
 * Retrun: pointer to sorted array
*/
int *InsertionSort(int *array, size_t size);

/* Sort array using Selection sorting method
 * Time complexity O(n^2).
 * Space complexity O(1).
 * Stability: Yes.
 * Argument:
 *		pointer to array
 *		size of array
 * Retrun: pointer to sorted array
*/
int *SelectionSort(int *array, size_t size);

/* Sort array using Bubble sorting method, with optimization
 * Time complexity ???.
 * Space complexity O(1).
 * Stability: Yes.
 * Argument:
 *		pointer to array
 *		size of array
 * Retrun: pointer to sorted array
*/
int *OptimizedBubbleSort(int *array, size_t size);

/*
 * CountSort():
 *
 * Sort array using Count sorting method.
 *
 * Time complexity O(n + k)
 * Space complexity O(k).
 * Stability: Yes.
 *
 * Argument:
 *		'arr' - pointer to array
 *		'size' - size of array
 *		'min' - the smallest input.
 *		'max' - the largest input.
 *
 * Retrun: 
 *      success (0)
 *      fail (1)
*/
int CountSort(int *arr, size_t size, int min, int max);

/*
 * RadixSort():
 *
 * Sort array using Radix sorting method.
 *
 * Time complexity O(d *(n + k))
 * Space complexity O(k).
 * Stability: Yes.
 *
 * Argument:
 *		'arr' - pointer to array
 *		'size' - size of array
 *
 * Retrun: 
 *      success (0)
 *      fail (1)
*/
int RadixSort(unsigned int *arr, size_t size);

/*
 * MargeSort():
 *
 * Sort array recursivly.
 *
 * Time complexity O(n * log(n))
 * Space complexity O(n).
 * Stability: Yes.
 *
 * Argument:
 *		'arr' - pointer to array
 *		'size' - size of array
 *		'IsBefore' - Function to determen the sorting policy
 *
 * Retrun: 
 *      sorted array.
 *      if fail NULL.
*/
int *MargeSort(int *arr, size_t size, is_before_t IsBefore);

/*
 *	QuickSort: O(nlogn)
 *	    Sort the arr by quick sort.
 *
 *	Argumentd:
 *		arr - the array to sort
 *      size - the size of the array
 *      elem_size - size of each element in the array
 *      cmp - compare function
 *
 *	Return:
 *		
 */
void QuickSort(void *arr, size_t size, size_t elem_size, cmp_func_t cmp);

#endif

