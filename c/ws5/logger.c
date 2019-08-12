#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

typedef enum	status
{
	success,
	fail,
	exit_program
}		status_t;

typedef struct	all_user_actions
{
	char *str;
	int (*comper_func)(const char *, const char *);
	status_t (*action_func)(const char *, const char *);
}		operation_t;

static int StrCmp(const char *s1, const char *s2)
{
	assert(s1);
	assert(s2);

	while (*s1 && *s1 == *s2)
	{
		++s1;
		++s2;
	}

	return (*s1 - *s2);
}

static status_t ExitProgram(const char *str, const char *file_name)
{
	return exit_program;
}

static status_t WriteStringToFile(const char *str, const char *file_name)
{
	FILE *fd = 0;

	fd = fopen(file_name, "a");
	fputs(str, fd);

	return success;
}

static status_t InitializeProgrem(operation_t *operation, \
					char *user_string, \
					status_t (*function_pointer)(const char*, const char *))
{
	assert(operation);
	assert(user_string);
	assert(function_pointer);

	operation->str = user_string;
	operation->comper_func = StrCmp;
	operation->action_func = *function_pointer;
}

int main(const int ac, const char **av)
{
	operation_t operation[2];
	int index = 0;
	status_t status = 0;
	char user_string[55];

	if (2 == ac)
	{
		scanf("%50s", user_string);
		InitializeProgrem(operation, "-exit", ExitProgram);
		InitializeProgrem((operation + 1), user_string, WriteStringToFile);
		while (exit_program != status)
		{
			while (1 > index)
			{
				if (0 == operation[index].comper_func(user_string, operation[index].str))
				{
					status = operation[index].action_func(user_string, av[1]);
				}
				++index;
			}
			if (status != exit_program)
			{
				operation[index].action_func(user_string, av[1]);
				scanf("%50s", user_string);
				index = 0;
			}
		}
		printf("my str %s\n", user_string);
	}
	else
	{
		printf("-- Error\n-- Logger expects one argument\n");
		printf("-- Run:\n-- Logger [...]\n");
	}

	return 0;
}

