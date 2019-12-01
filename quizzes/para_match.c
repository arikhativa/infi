
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *MakeStack(char *str)
{
	char *stack = 0;

	stack = (char*)calloc((strlen(str) + 2), sizeof(char));

	return stack;
}

int IsParaFront(char para)
{
	if ('(' == para)
		return 1;
	if ('[' == para)
		return 1;
	if ('{' == para)
		return 1;
	return 0;
}

int IsParaBack(char para)
{
	if (')' == para)
		return 1;
	if (']' == para)
		return 1;
	if ('}' == para)
		return 1;
	return 0;
}

char *AddToStack(char *stack, char para)
{
	++stack;
	*stack = para;

	return stack;
}

int CheckStack(char *stack, char para)
{

	if (*stack == '(')
		return (')' == para);

	if (*stack == '[')
		return (']' == para);

	if (*stack == '{')
		return ('}' == para);

	return 0;
}

int CheackStrPara(char *str)
{
	int status = 1;
	char *stack = 0;
	char *b = 0;

	stack = MakeStack(str);
	b = stack;

	while (*str && status)
	{
		if (IsParaFront(*str))
		{
			stack = AddToStack(stack, *str);
			printf("-- %s\n", (b + 1));
		}
		else if (IsParaBack(*str))
		{
			status = CheckStack(stack, *str);
			--stack;
		}
		++str;
	}

	free(b);

	return status;
}

int main()
{
	char s1[] = "({{{{}}}})";
	char s2[] = "]";
	char s3[] = "{[()]}";
	char s4[] = "({)}";

	printf("-- %d\n", CheackStrPara(s1));
	printf("-- %d\n", CheackStrPara(s2));
	printf("-- %d\n", CheackStrPara(s3));
	printf("-- %d\n", CheackStrPara(s4));
	return 0;
}
