
#include <stdio.h>


void Swap(char *a, char *b)
{
	char t = *a;

	*a = *b;
	*b = t;
}

char *RecSwap(char *origin, char *str)
{
	if (!*str)
		return origin;

	origin = RecSwap(origin, str + 1);

	if (str > origin)
		Swap(origin, str);

	return (origin + 1);
}

void StrSwap(char *str)
{
	RecSwap(str, str);
}

int main()
{
	char str[] = "123456";

	StrSwap(str);
	printf("-- %s\n", str);

	return 0;
}
