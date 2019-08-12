
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

//char *strdup(const char *str);

void foo(int a[])
{
	
	printf("str size is%lu\n", sizeof(a));
}

int main(int ac, char **av, char **env)
{
	char str[] = "12345";
	char s1;
	int arr[10];

	printf("s1 size is%lu\n", sizeof(s1));
	printf("str size is%lu\n", sizeof(str));
	printf("arr size is%lu\n", sizeof(arr));
	foo(arr);

	return 0;
}
