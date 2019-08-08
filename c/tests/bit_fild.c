
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct bit_fild
{
	unsigned char bit : 1;
} 		fild_t;

int main(int ac, char **av)
{
	fild_t *s;
	fild_t *runner;

	s = (fild_t*)malloc(sizeof(fild_t) * 5);
	runner = s;
	while (runner < (s + 5))
	{
		runner->bit = 0;
		++runner;
	}
	while (s != runner)
	{
		printf("%d", s->bit);
		++s;
	}

	return 0;
}
