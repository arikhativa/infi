#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "color.h"
#include "hash.h"
#include "dlist.h"


/*
 * 	global 
 */

static char g_summery[400][400] = {0};

static int count_test = 1;
static int t_index = 1;
static int g_pass = 0;
static int g_fail = 0;

/*	var per test	*/

static char test_name[40];

static int add_test_name = 1;



typedef struct avl_node avl_node_t;

typedef enum diraction
{
        LEFT = 0,
        RIGHT
} diraction_t;


struct hash
{
        hash_cmp_t DataCmp;
        hash_func_t ConvDataToIndex;
        size_t size;
        size_t table_size;
        dlist_t *table[1];
};


static void SetTest(int n, char *name)
{
	sprintf(test_name,"\n\n-- "CYAN"(%d) Test %s:\n\n"RESET, n, name);

	add_test_name = 1;

	t_index = 1;
}

static void Pass()
{
	++count_test;
	++g_pass;
	++t_index;
}

static void PrintFail(char *dis, int line, char *value, char *bad_value)
{
	if (add_test_name)
	{
		add_test_name = 0;
		sprintf(g_summery[g_fail],"%s", test_name);
		++g_fail;
	}

	sprintf(g_summery[g_fail],"-- "YELLOW"Discription:\n"RESET"-- %s\n\n",dis);
	++g_fail;
	sprintf(g_summery[g_fail],"-- "BBLUE"In Line "BGREEN "%d "RESET, line);
	++g_fail;
	sprintf(g_summery[g_fail],BBLUE"Value should be "RESET BGREEN"%s\n"RESET, value);
	++g_fail;
	sprintf(g_summery[g_fail],"-- "BBLUE"It is "RESET RED"%s\n\n"RESET, bad_value);
	++g_fail;


	++count_test;
	++t_index;
}

static void PrintSummery(void)
{
	int i = 0;

	--count_test;
	if (g_pass == count_test)
		printf(GREEN"\n-- Pass :) (%d/%d)\n\n"RESET,g_pass, count_test);
	else
	{
		printf(YELLOW"\n-- Not good enough.. (%d/%d)\n\n"RESET,g_pass, count_test);
		for (i = 0; i < g_fail; ++i)
		{
			printf("%s", g_summery[i]);
		}
	}
}

static void ReturnComper(size_t num1, size_t num2, char *dis, int line)
{
	char value[20] = {0};
	char bad_value[20] = {0};

	if (num1 == num2)
		Pass();
	else
	{
		sprintf(value, "%lu", num1);
		sprintf(bad_value, "%lu", num2);
		PrintFail(dis, line, value, bad_value);
	}
}
/*
static void MemoryComper(void *mem1, void *mem2, size_t n, char *dis, int line)
{
	char value[200] = {0};
	char bad_value[200] = {0};

	if (0 == memcmp(mem1, mem2, n))
		Pass();
	else
	{
		PrintFail(dis, line, value, bad_value);

		sprintf(value, "%s", (char*)mem1);
		sprintf(bad_value, "%s", (char*)mem2);
		PrintFail(dis, line, value, bad_value);
	}
}

static void PrintHash(hash_t *hash)
{
	int size = hash->table_size;
	int i = 0;
	dlist_iter_t iter = NULL;
	dlist_iter_t end = NULL;

	iter = DlistBegin(hash->table[i]);
	end = DlistEnd(hash->table[i]);

	for (; i < size; ++i)
	{
		iter = DlistBegin(hash->table[i]);
		end = DlistEnd(hash->table[i]);

		printf("%d)\t", i);
		while (iter != end)
		{
			printf("[ %s ] ", (char*)DlistGetData(iter));

			iter = DlistNext(iter);
		}
		printf("\n");
	}

}
*/

static size_t HashFunc(const void *key)
{
	char *str = (char*)key;
	int index = 0;

	assert(key);

	if (isalpha(*str))
	{
		if (islower(*str))
		{
			index = *str - 'a' + 26;
		}
		else
		{
			index = *str - 'A';
		}
	}
	else
		index = 52;

	return index;
}

static int DataCmp(const void *data, void *user_data)
{
	const char *s1 = data;
	char *s2 = user_data;

	while (*s1 && *s1 == *s2)
	{
		++s1;
		++s2;
	}

	return (*s1 - *s2);
}


/*	while (hash_size)
	{
		printf("%d, ", HashFunc(s));
		++s;
		--hash_size;
	}*/

static void TestCreateAndDestroy()
{
	hash_t *hash;
	size_t hash_size = 26 * 2;

	SetTest(1, "Create & Destroy (run with vlg)");

	hash = HashCreate(HashFunc, hash_size, DataCmp);

	HashDestroy(hash);
}

static void TestInsert()
{
	hash_t *hash;
	size_t hash_size = (26 * 2) + 1;
	size_t i = 0;
	char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char *s = 0;

	char dis1[] = "checking if an empty returns 1";

	s = alpha;

	SetTest(2, "Insert");

	hash = HashCreate(HashFunc, hash_size, DataCmp);

	ReturnComper(1, HashIsEmpty(hash), dis1, __LINE__);

	for (i = 0; i < hash_size; ++i)
	{
		HashInsert(hash, s);
		++s;
	}

	ReturnComper(0, HashIsEmpty(hash), "Checking if not empty", __LINE__);
	ReturnComper(hash_size, HashSize(hash), "Checking size", __LINE__);

	s = alpha;
	for (i = 0; i < hash_size; ++i)
	{
		HashInsert(hash, s);
		++s;
	}

	ReturnComper(2 * hash_size, HashSize(hash), "Checking size", __LINE__);

/*	PrintHash(hash);*/

	HashDestroy(hash);

}

static void TestRemove()
{
	hash_t *hash;
	size_t hash_size = 26 * 2 + 1;
	int i = 0;

	char s1[] = "yoav";
	char s2[] = "shar";
	char s3[] = "Yoni";
	char s4[] = "MOMO";
	char s5[] = "Yes";

	SetTest(3, "Remove");

	hash = HashCreate(HashFunc, hash_size, DataCmp);

	HashInsert(hash, s1);
	HashInsert(hash, s2);
	HashInsert(hash, s3);
	HashInsert(hash, s4);
	HashInsert(hash, s5);

	ReturnComper(1, HashRemove(hash, "11"), "Removeing data that is a bad hash '11'", __LINE__);
	ReturnComper(1, HashRemove(hash, "aa"), "Removeing data that is not there 'aa'", __LINE__);

	HashRemove(hash, s3);

	if (!HashFind(hash, s3))
		i = 1;
	ReturnComper(1, i, "Removed data. checking HashFind() return value", __LINE__);


/*	PrintHash(hash);*/

	HashDestroy(hash);
}

static void TestFind()
{
/*	hash_t *hash;
	size_t hash_size = 26 * 2;
	int i = 0;

	char s1[] = "yoav";
	char s2[] = "shar";
	char s3[] = "Yoni";
	char s4[] = "MOMO";
	char s5[] = "Yes";

	SetTest(4, "Find");

	hash = HashCreate(HashFunc, hash_size, DataCmp);

	HashInsert(hash, s1);
	HashInsert(hash, s2);
	HashInsert(hash, s3);
	HashInsert(hash, s4);


	if (NULL != HashFind(hash, s5))
	{
	}

	PrintHash(hash);

	HashDestroy(hash);*/
}


static void TestSize()
{
/*	hash_t *hash;
	size_t hash_size = 26 * 2;
	int i = 0;

	char s1[] = "yoav";
	char s2[] = "shar";
	char s3[] = "Yoni";
	char s4[] = "MOMO";
	char s5[] = "Yes";

	SetTest(5, "Size");

	hash = HashCreate(HashFunc, hash_size, DataCmp);

	HashInsert(hash, s1);
	HashInsert(hash, s2);
	HashInsert(hash, s3);
	HashInsert(hash, s4);


	printf("%ld\n", HashSize(hash));
	PrintHash(hash);

	HashDestroy(hash);*/
}

static int ACT(void *str, void *param)
{
	(void)param;

	if ((*(char*)str != 'a'))
	{	
		*((char*)str + 1) = '6';
		return 0;
	}

	return 1;
}

static void TestForEach()
{
	hash_t *hash;
	size_t hash_size = 26 * 2 + 1;
	int i = 0;

	char s1[] = "yoav";
	char s2[] = "shar";
	char s3[] = "Yoni";
	char s4[] = "MOMO";
	char s5[] = "aa";

	SetTest(6, "ForeEach");

	hash = HashCreate(HashFunc, hash_size, DataCmp);

	HashInsert(hash, s1);
	HashInsert(hash, s2);
	HashInsert(hash, s3);
	HashInsert(hash, s4);
	HashInsert(hash, s5);

	i = HashForEach(hash, ACT, 0);

	ReturnComper(1, i, "checking if ForEach return value", __LINE__);

	ReturnComper('6', *(s4 + 1), "checking if ForEach changed the char", __LINE__);
	ReturnComper('6', *(s3 + 1), "checking if ForEach changed the char", __LINE__);
	ReturnComper('h', *(s2 + 1), "checking if ForEach didnt changed the char", __LINE__);
	ReturnComper('o', *(s1 + 1), "checking if ForEach didnt changed the char", __LINE__);
/*rintHash(hash);*/

	HashDestroy(hash);

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
			TestCreateAndDestroy();
			break ;
		case 2:
			TestInsert();
			break ;
		case 3:
			TestRemove();
			break ;
		case 4:
			TestFind();
			break ;
		case 5:
			TestSize();
			break ;
		case 6:
			TestForEach();
			break ;
		case 7:
			break ;
		case 8:
			break ;
		case 9:
			break ;
		default:
			TestCreateAndDestroy();
			TestInsert();
			TestFind();
			TestRemove();
			TestSize();
			TestForEach();
	}
	PrintSummery();

	return 0;
}
