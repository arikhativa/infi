
#include <stdio.h>
#include <stdlib.h>


void *alignd_alloc(size_t bytes, size_t alignd)
{
	char *origen = NULL;
	char *run = NULL;

	origen = (char*)malloc(sizeof(char*) + bytes + (alignd - 1));
	if (!origen)
	{
		return NULL;
	}

	run = origen + 8;

	while (0 != (size_t)run % alignd)
	{
		++run;
	}

	*((size_t*)run - 1) = (size_t)origen;

	return run;
}

void alignd_free(void *ptr)
{
	size_t *origen = ptr;


	origen = (size_t*)*(origen - 1);

	free(origen);
}

int main()
{
	char *mem1 = 0;
	size_t blk = 128;

	mem1 = alignd_alloc(100, blk);
	if (0 == (size_t)mem1 % blk)
		printf("-- ok\n");
	else
		printf("-- mmm..\n");
	alignd_free(mem1);

	return 0;

}

