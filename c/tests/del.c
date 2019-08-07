
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int main(int ac, char **av)
{
	int n = 0;
	char *s = 0;

	while (*s)
	{
		printf("%d, ", n++);
	}

	return 0;
}
