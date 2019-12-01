
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "hash.h"
#include "dlist.h"

/*
struct hash
{
        hash_cmp_t DataCmp;
        hash_func_t ConvDataToIndex;
        size_t size;
        size_t table_size;
        dlist_t *table[1];
};


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

char *strdup(const char *s);

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


static int SpellChecker(hash_t *hash, char *str)
{
	char *str_in_hash = 0;

	assert(hash);

	str_in_hash = HashFind(hash, str);

	if (str_in_hash)
		return 1;

	return 0;
}

int main(int ac, char **av)
{
	hash_t *hash;
	size_t hash_size = 26 * 2 + 1;
	size_t size = 102305;
	size_t i = 0;
	int len = 0;
	char word[25];
	char *tmp;
	char **tab;
	FILE *fd;

	if (ac == 1)
		return 1;

	fd = fopen("/usr/share/dict/words", "r");

	tab = (char**)malloc(sizeof(char*) * 102306);

	hash = HashCreate(HashFunc, hash_size, DataCmp);

	for (i = 0; i < size; ++i)
	{
		fgets(word, 25, fd);

		len = strlen(word);
		word[len - 1] = '\0';

		tmp = strdup(word);

		tab[i] = tmp;
		HashInsert(hash, tmp);
	}

	if (SpellChecker(hash, av[1]))
		printf("-- It IS in the dictionary\n");
	else
		printf("-- It is NOT in the dictionary\n");


	for (i = 0; i < size; ++i)
	{
		free(tab[i]);
	}

	free(tab);

	fclose(fd);

	HashDestroy(hash);

	return 0;
}
