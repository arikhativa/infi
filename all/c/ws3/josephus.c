
#include <stdio.h> /* printf() */
#include <stdlib.h> /* malloc() */

char *SetStrToOne(char *str, size_t size)
{
	char *runner = str;

	while(runner < str + size)
	{
		*runner = '1';
		++runner;
	}

	return str;
}

static size_t LookForAlive(char *str, size_t size)
{
	size_t round = size;
	size_t alive = 1;
	char *runner = (str + 1);
	int kill = 1;

	while (0 < round)
	{
		if (*runner == '1')
		{
			if (kill == 1)
			{
				*runner = '2';
				kill = 0;
			}
			else
			{
				round = size;
				alive = runner - str + 1;
				kill = 1;
			}
		}
		++runner;
		if (*runner == '\0')
		{
			runner = str;
		}
		--round;
	}
	free(str);
	str = NULL;

	return alive;
}

size_t Josephus(size_t size)
{
	char *str = 0;

	if (size == 0)
	{
		return 0;
	}
	if (size == 1)
	{
		return 1;
	}
	if (!(str = (char*)malloc(sizeof(char) * (size + 1))))
	{
		return 0;
	}
	str[size] = '\0';
	str = SetStrToOne(str, size);

	return LookForAlive(str, size);
}

int main(int ac, char **av)
{
	int n = 0;
	int res[100] = {0, 1, 1, 3, 1, 3, 5, 7, 1, 3, 5, 7, 9, 11, 13, 15};

	if (ac == 2)
	{
		printf("\n\n-- Result for %s is == %ld\n\n\n", av[1], Josephus(atoi(av[1])));
	}
	else
	{
		while (15 >= n)
		{
			printf("-- Input %d\n",n);
			printf("-- Result is == %ld\n-- Should be == %d\n\n", Josephus(n), res[n]);
			++n;
		}
	}

	return 0;
}
