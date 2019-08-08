
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

//char *strdup(const char *str);

int main(int ac, char **av, char **env)
{

	while (*env != NULL)
	{
		printf("%s\n", *env);
		++env;
	}

	return 0;
}
