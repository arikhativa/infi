#ifndef __SEARCH_H__
#define __SEARCH_H__



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
typedef int (*search_cmp_func_t)(const void *data1, const void *data2);


/*
 *	IterBinarySearch: O(log(n))
 *	    Searchin for the request data in one dimensional sorted array.
 *
 *	Argumentd:
 *		'arr' - array of data to look for.
 *      'size' - size of the array.
 *      'elem_size' - size of each element in the array.
 *      'cmp' - cmp function between two data's.
 *      'req_data' - data to find.
 *
    Return:
 *      returns the index of the requset data.
 *      '-1' if the request data doesn't exist.
 *		
 */
int IterBinarySearch(void *arr, size_t size, size_t elem_size, search_cmp_func_t cmp, void *req_data);

/*
 *	RecBinarySearch: O(log(n))
 *	    Searchin for the request data in one dimensional sorted array.
 *
 *	Argumentd:
 *		'arr' - array of data to look for.
 *      'size' - size of the array.
 *      'elem_size' - size of each element in the array.
 *      'cmp' - cmp function between two data's.
 *      'req_data' - data to find.
 *
    Return:
 *      returns the index of the requset data.
 *      '-1' if the request data doesn't exist.
 *		
 */
int RecBinarySearch(void *arr, size_t size, size_t elem_size, search_cmp_func_t cmp, void *req_data);


/*
 *	JumpSearch: O(sqr(n))
 *	    Searchin for the request data in one dimensional sorted array.
 *
 *	Argumentd:
 *		'arr' - array of data to look for.
 *      'size' - size of the array.
 *      'elem_size' - size of each element in the array.
 *      'cmp' - cmp function between two data's.
 *      'req_data' - data to find.
 *
    Return:
 *      returns the index of the requset data.
 *      '-1' if the request data doesn't exist.
 *		
 */
int JumpSearch(void *arr, size_t size, size_t elem_size, search_cmp_func_t cmp, void *req_data);



#endif /* __SEARCH_H__ */
