#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

//char *strdup(const char *str);

typedef enum chair
{
	Aviv,
	Shlomit,
	Yoav,
	Elyashiv,
	Shahar,
	Max
} chair_t;


int *MakeReviewList(int *arr, size_t seed)
{
	chair_t i = 0;

	if (Max <= seed)
		return NULL;
	for (; i != Max; ++i)
	{
		if (Max <= i + seed)
			arr[i] = i + seed - Max;
		else
			arr[i] = i + seed;
	}

	return arr;
}

void PrintReviewList(int *arr, char **names)
{
	chair_t i = 0;

	printf("\n  %s			%s\n\n", "Dev", "Reviewer");
	for (; i != Max; ++i)
	{
		//printf("  %s	--	%s\n", names[i], names[arr[i]]);
		printf("  %s\n", names[arr[i]]);
	}
}

int main(int ac, char **av)
{
	int *arr = 0;
	char *names[] =
	{
		"Aviv		",
		"Shlomit	",
		"Yoav		",
		"Elyashiv	",
		"Shahar		"
	};

	if (!(arr = (int*)malloc(sizeof(int) * Max)))
		return 0;
	arr = MakeReviewList(arr, 3);
	PrintReviewList(arr, names);


	return 0;
}
