
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int ac, char **av)
{
	char str[] = "12345";
	char *rev;

	rev = (str + (strlen(str) - 1));
	printf("%s\n", (rev - 3));


	return 0;
}
