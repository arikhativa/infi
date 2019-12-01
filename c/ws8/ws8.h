
#ifndef __WS8__H__
#define __WS8__H__

#include <stddef.h> /* size_t */

typedef struct element element_t;


/* This Function return an array that is terminated with one extra empty struct element.
 *  This array needs to be freed using FreeAll() */
element_t *InitArray(size_t size);


/* This Function adds a String to the array using 'index' for the spasific place in that array.
 * It returns -1 if the allocation fails */
int InitString(element_t *arr, const char *str, size_t index);


/* This Function adds an Int to the array using 'index' for the spasific place in that array.
 * It allways returns 0 */
int InitInt(element_t *arr, int num, size_t index);


/* This Function adds a Float to the array using 'index' for the spasific place in that array.
 * It allways returns 0 */
int InitFloat(element_t *arr, float num, size_t index);


/* This Function adds an Int to all element in the array.
 * It returns -1 if the allocation fails */
int AddValToAll(element_t *arr, int val);


/* This Function prints all the elements in the array */
void PrintAll(const element_t *arr);


/* This Function frees all the elements in the array */
void FreeAll(element_t *arr);


#endif /* __WS8__H__ */
