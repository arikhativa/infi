
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

//char *strdup(const char *str);

int main(int ac)
{
	int x;

	printf("%d\n", *(&x - 2));


	return 0;
}
