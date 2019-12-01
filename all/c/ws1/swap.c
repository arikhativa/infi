
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void Swap(int *a, int *b)
{
	int swap = *a;

	*a = *b;
	*b = swap;
}

int main(int ac, char **av)
{
	int a;
	int b;

	a = ac;
	a = atoi(av[1]);
	b = atoi(av[2]);
	printf("a is %d\nb is %d\n", a, b);
	printf("swap\n");
	Swap(&a, &b);
	printf("a is %d\nb is %d\n", a, b);
	return 0;
}
