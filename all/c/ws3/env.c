
#include <stdio.h> /* printf() */
#include <stdlib.h> /* malloc() */
#include <string.h> /* strlen() */
#include <ctype.h> /* tolower() */
#include <assert.h> /* assert */

static size_t Tablen(char **tab)
{
	size_t count = 0;

	while (*tab)
	{
		++tab;
		++count;
	}

	return count;
}

static char *CopyLowerStr(const char *str)
{
	char *new_str = 0;
	char *runner = 0;

	if (!(new_str = (char*)malloc(sizeof(char) * strlen(str) + 1)))
	{
		return NULL;
	}
	runner = new_str;
	while (*str)
	{
		*runner = tolower(*str);
		++runner;
		++str;
	}
	*runner = '\0';

	return new_str;
}

static void FreeRunner(char **runner)
{
	while (*runner)
	{
		free(*runner);
		*runner = NULL;
		++runner;
	}
}

int main(int ac, char **av, char **env)
{
	char **buffer = 0;
	char **runner = 0;

	assert(env);

	ac += ac;
	ac = atoi(av[0]);

	if (!(buffer = (char**)malloc(sizeof(char*) * (Tablen(env) + 1))))
	{
		return 0;
	}
	runner = buffer;
	while (*env)
	{
		if (!(*runner = CopyLowerStr(*env)))
		{
			break ;
		}
		else
		{
			printf("%s\n", *runner);
			++runner;
			++env;
		}
	}
	*runner = NULL;
	runner = buffer;
	FreeRunner(runner);
	free(buffer);
	buffer = NULL;

	return 0;
}
