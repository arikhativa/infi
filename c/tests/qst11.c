
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int foo(int a, int b)
{
	return (a + b);
}

int main(int ac, char **av)
{
	int i = 0;
	i = ++i + 1;

	printf("i ==%d\n\n", i);

	int a = 3, b = 2, c;

	c = (a * b) + (++a + 4);
	printf("a ==%d\n", a);
	printf("c ==%d\n", c);


	i = 5;
	printf("\nfoo == %d\n", foo(++i, i));

	float f = 12.54;
	i = 12;
	f = i = f * 2;
	printf("\nf == %f\n", f);


	return 0;
}
