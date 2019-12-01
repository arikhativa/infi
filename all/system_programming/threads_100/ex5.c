

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM					(1000000066)

static const size_t origin = NUM;

size_t SumOfDivisors(size_t start);

size_t SumOfDivisors(size_t start)
{
	size_t run = 0;
	size_t sum = 0;

	#pragma omp parallel for

	for (run = start; run <= origin / 2; ++run)
	{
		if (!(origin % run))
		{
			sum += run;
		}
	}

	sum += origin;

	return sum;
}

int main()
{
	size_t ret = 0;

	ret = SumOfDivisors(1);

	printf("-- sum is %ld\n", ret);

	return 0;
}
