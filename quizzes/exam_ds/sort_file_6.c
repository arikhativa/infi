
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Swap(char *a, char *b)
{
	char tmp = *a;

	*a = *b;
	*b = tmp;
}

void SwapPtr(void **a, void **b)
{
	void *tmp = *a;

	*a = *b;
	*b = tmp;
}

int Partition(char *s, int low, int high)
{
	int a = low;
	int b = low;

	for (; b < high; ++b)
	{
		if (s[b] < s[high])
		{
			Swap(&s[a], &s[b]);
			++a;
		}	
	}

	Swap(&s[a], &s[high]);

	return a;
}

void QuickSort(char *s, int low, int high)
{
	int a = 0;

	if (low < high)
	{
		a = Partition(s, low, high);

		QuickSort(s, low, a - 1);
		QuickSort(s, a + 1, high);
	}
}

int SortCharInFile(const char *file_name, char *res)
{
	FILE *fd = fopen(file_name, "r");

	if (!fd)
		return -1;

	fscanf(fd, "%s", res);

	QuickSort(res, 0, strlen(res) - 1);

	fclose(fd);

	return 0;
}

int main()
{
	char s[4000] = {0};

	SortCharInFile("./for_sort", s);

	printf("-- %s\n", s);

	return 0;
}

