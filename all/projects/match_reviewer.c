#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

//char *strdup(const char *str);

typedef enum chair
{
	Elyashiv = 0,
	Michael,
	Yoav,
	Shahar,
	Galit,
	Aviv,
	Gal,
	Shlomit,
	Hadas,
	Yoni,
	Max
} chair_t;


int *MakeReviewList(int *arr, size_t seed)
{
	chair_t i = 0;

	if (2 > seed || 5 == seed || 10 <= seed)
		return NULL;
	for (; i != Max; ++i)
	{
		if (10 <= i + seed)
			arr[i] = i + seed - 10;
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
		"Elyashiv",
		"Michael ",
		"Yoav    ",
		"Shahar  ",
		"Galit   ",
		"Aviv    ",
		"Gal     ",
		"Shlomit ",
		"Hadas   ",
		"Yoni    "
	};

	if (!(arr = (int*)malloc(sizeof(int) * 10)))
		return 0;
	arr = MakeReviewList(arr, 6);
	PrintReviewList(arr, names);


	return 0;
}

