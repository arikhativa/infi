
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int MatchPara(char stack, char para)
{

	if (stack == '(')
		return (')' == para);

	if (stack == '[')
		return (']' == para);

	if (stack == '{')
		return ('}' == para);

	return 0;
}

int LookForPara(char *str, size_t *i)
{
	char para = 0;
	int status = 1;

	para = str[*i];
	++*i;
	while (str[*i] && status)
	{
		if (IsParaFront(str[*i]))
		{
			status = LookForPara(str, i);
		}
		else if (IsParaBack(str[*i]))
		{
			return (MatchPara(para, str[*i]));
		}
		++*i;
	}

	return 0;
}

int CheackStrPara(char *str)
{
	int status = 1;
	size_t i = 0;

	while (str[i] && status)
	{
		if (IsParaFront(str[i]))
		{
			status = LookForPara(str, &i);
		}
		else if (IsParaBack(str[i]))
		{
			status = 0;
		}
		++i;
	}

	return status;
}

int main()
{
	char s1[] = "({{{{}}}})";
	char s2[] = "[]";
	char s3[] = "{[()]}";
	char s4[] = "({)}";

	printf("-- %d\n", CheackStrPara(s1));
	printf("-- %d\n", CheackStrPara(s2));
	printf("-- %d\n", CheackStrPara(s3));
	printf("-- %d\n", CheackStrPara(s4));
	return 0;
}
