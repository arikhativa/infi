#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

//char *strdup(const char *str);

int IsRotation(char *s1, char *s2)
{
	int len = 0;
	int n = 0;
	int i = 0;

	len = strlen(s1);

	if (len != strlen(s2))
		return 0;

	for (i = 0; i < len; ++i)
	{
		if (*s1 == s2[i])
			break;
	}

	while (s1[n] == s2[(i + n) % len] && n < len)
		++n;

	return (n == len);
}

int IsRotation1(const char *s1, const char *s2)
{
	const char *run1 = s1;
	const char *run2 = s2;
	const char *start2 = s2;

	if (strlen(s1) != strlen(s2))
	{
		return 0;
	}
	while (*s2)
	{
		while (*run1 == *run2)
		{
			++run1;
			++run2;
			if (!(*run2))
			{
				run2 = start2;
			}
		}
		if (!(*run1))
		{
			return 1;
		}
		run1 = s1;
		++s2;
		run2 = s2;
	}

	return 0;
}

int main()
{
	char s1[] = "2333";
	char s2[] = "3323";

	char s3[] = "11332233";
	char s4[] = "22331133";

	char s5[] = "123123";
	char s6[] = "123321";


	printf("-- %d\n", IsRotation(s1, s2));
	printf("-- %d\n", IsRotation(s3, s4));
	printf("-- %d\n", IsRotation(s5, s6));

	return 0;
}

