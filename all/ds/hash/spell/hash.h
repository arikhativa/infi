/*
 *	ver 1.4 - changed return value of hash-func from int to size_t.
 *	ver 1.3 - changed return value of hash-func from size_t to int.
 *  ver 1.2 - changed signature of cmp_func_t, and Create signature.
 *  ver 1.1
*/

/* Complexities: n - num of entries in table. k - num of members in each entry. */

#include <stddef.h> /* size_t */

typedef struct hash hash_t;

/* *********IN C FILE********* 
struct hash
{
	hash_cmp_t DataCmp;
    hash_func_t ConvDataToIndex;
    size_t table_size;
    size_t size;
    dlist_t *table[1];
};
*/

/* 
*	HashFunc():
*	Converts key to index in hash-table. 
*	
*	Arguments:
*		'key' -  data that table holds.
*		
*	Return:
*		index in table where key belongs.
*/
typedef size_t (*hash_func_t)(const void *key);

/* 
*	ActFunc():
*	Does an action on data. Notice that ActFunc must not change key value.
*	
*	Arguments:
*		'data' -  data that table holds.
*		'param' - addittion for Act function. 
*		
*	Return:
*		'0' - success.
*		otherwise - fail.
*/
typedef int (*act_func_t)(void *data, void *param);

/* 
*	CmpFunc():
*	Compares user_data to data from table. 
*	
*	Arguments:
*		'data' -  hash table .
*		'usr_data' - user data. 
*		
*	Return:
*		'1' - True.
*		'0' - False.
*/
typedef int (*hash_cmp_t)(const void *data, void *usr_data);

/*
*   HashCreate():	O(1)
*   Create Handle for the data structure.
*
*	Arguments:
*		'HashFunc'   - function that converts data to index.
*		'hash_size'  - number of indexes in the hash table.
*		'DataCmp' - default compare function.
*
*	Return:
*		On success - handle to the data-structure
*		On failure - NULL
*/
hash_t *HashCreate(hash_func_t HashFunc, size_t hash_size, hash_cmp_t DataCmp);

/*
*   HashDestroy():	O(n + k)
*
*   Arguments:
*		'hash' - table to destroy.
*
*	Return:
*		None.
*/
void HashDestroy(hash_t *hash);

/*
*	HashInsert:	O(k)
*	Insert 'data' to hash table 
*	
*	Arguments:
*		'hash' - handle to hash table.
*		'data' - pointer to new data.
*
*	Return:
*		'0' - On success.
*		'1' - Fail.		
*/
int HashInsert(hash_t *hash, void *data);

/*
 *	HashRemove():	O(k)
 *	remove the data from the list.	
 *
 *	Arguments:
 *		pointer to the hash table.
 *		data to removal.
 *
 *	Return:
 *		'0' - success. the data has found and removed.
 *		'1' - fail. the data has not found.
 */
int HashRemove(hash_t *hash, const void *data);

/*
*	HashSize():	O(1)
*	Return the num of inserted data in table.
*
*	Argumentd:
*		'hash' - pointer to table.
*
*	Return:
*		The size of hash.
*/
size_t HashSize(const hash_t *hash);

/*
 *	HashIsEmpty():	O(1)
 *	Checks if the hash table is empty.
 *
 *	Arguments:
 *		'hash' - pointer to table.
 *
 *	Return:
 *		'1' - if empty.
 *		'0' - else.	
*/
int HashIsEmpty(const hash_t *hash);

/*
*	HashFind():	O(k)
*	Find if 'data' exist in table.
*
*	Arguments:
*		'hash' - table to search.
*		'data' - data to find. data must be same type as data in table.
*
*	Return:
*       If found - return data from table.
*		not found - return NULL.
*/
void *HashFind(const hash_t *hash, const void *data);

/*
*	HashForEach():	O(n + k)
*	preform Act function on all datas in table.
*
*	Arguments:
*		'hash' - table to act on.
*		'Act' - comparing between section inside data to param.
*		'param' - parameter to use in Act func.
*
*	Return:
*       If Act fails - return Act failure result.
*		If all succeed - 0.
*/
int HashForEach(hash_t *hash, act_func_t Act, void *param);
