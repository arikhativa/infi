
#include "ilrd_string.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

char *strdup(const char *s);

static void CheckStrlen(char *str)
{
	static int number = 1;

	printf("-- input %d\n", number);
	printf("-- strlen == %ld\n", strlen(str));
	printf("-- Strlen == %ld\n\n", Strlen(str));
	printf("---------------------\n\n");
	++number;
}

static void CheckStrcmp(char *s1, char *s2)
{
	static int number = 1;

	printf("-- input %d\n", number);
	printf("-- strcmp == %d\n", strcmp(s1, s2));
	printf("-- Strcmp == %d\n\n", Strcmp(s1, s2));
	printf("---------------------\n\n");
	++number;
}

static void CheckStrcpy(char *dest1, char *dest2, char *src)
{
	static int number = 1;

	printf("-- input %d\n", number);
	printf("-- src   == \"%s\"\n", src);
	printf("-- dest1 == \"%s\"\n-- dest2 == \"%s\"\n\n", dest1, dest2);
	printf("-- strcpy == \"%s\"\n", strcpy(dest1, src));
	printf("-- Strcpy == \"%s\"\n", Strcpy(dest2, src));
	printf("-- dest1 == \"%s\"\n", dest1);
	printf("-- dest2 == \"%s\"\n\n", dest2);
			printf("---------------------\n\n");
	++number;
}

static void CheckStrncpy(char *dest1, char *dest2, char *src, size_t n)
{
	static int number = 1;

	printf("-- input %d\n", number);
	printf("-- n     == %ld\n", n);
	printf("-- dest1 == \"%s\"\n-- dest2 == \"%s\"\n", dest1, dest2);
	printf("-- src   == \"%s\"\n\n", src);
	printf("-- strncpy == \"%s\"\n", strncpy(dest1, src, n));
	printf("-- Strncpy == \"%s\"\n", Strncpy(dest2, src, n));
	printf("-- dest1   == \"%s\"\n", dest1);
	printf("-- dest2   == \"%s\"\n\n", dest2);
			printf("---------------------\n\n");
	++number;
}

/*static void CheckStrNcasecmp(char *s1, char *s2, size_t n)
{
	static int number = 1;

	printf("-- input %d\n", number);
	printf("-- s1 == \"%s\"\n", s1);
	printf("-- s2 == \"%s\"\n", s2);
	printf("-- n  == %ld\n\n", n);
	printf("-- strncasecmp == %d\n", strncasecmp(s1, s2, n));
	printf("-- Strncasecmp == %d\n\n", Strncasecmp(s1, s2, n));
			printf("---------------------\n\n");
	++number;

}*/

static void CheckStrChr(const char *s, int c)
{
	static int number = 1;

	printf("-- input %d\n", number);
	printf("-- s == \"%s\"\n", s);
	printf("-- c  == %c\n\n", c);
	printf("-- strchr == \"%s\"\n", strchr(s, c));
	printf("-- Strchr == \"%s\"\n\n", Strchr(s, c));
	printf("---------------------\n\n");
	++number;
}

static void CheckStrDup(const char *s)
{
	static int number = 1;
	char *tmp1;
	char *tmp2;

	printf("-- input %d\n", number);
	printf("-- s == \"%s\"\n", s);
	printf("-- strdup == \"%s\"\n", tmp1 = strdup(s));
	printf("-- Strdup == \"%s\"\n\n", tmp2 = Strdup(s));
	free(tmp1);
	free(tmp2);
	printf("---------------------\n\n");
	++number;
}

static void CheckStrCat(char *dest1, char *dest2, const char *src)
{
	static int number = 1;

	printf("-- input %d\n", number);
	printf("-- dest1 == \"%s\"\n", dest1);
	printf("-- dest2 == \"%s\"\n", dest2);
	printf("-- src == \"%s\"\n\n", src);
	printf("-- strcat == \"%s\"\n", strcat(dest1, src));
	printf("-- Strcat == \"%s\"\n\n", Strcat(dest2, src));
	printf("---------------------\n\n");
	++number;
}

static void CheckStrNCat(char *dest1, char *dest2, const char *src, size_t n)
{
	static int number = 1;

	printf("-- input %d\n", number);
	printf("-- n     == %ld\n", n);
	printf("-- dest1 == \"%s\"\n", dest1);
	printf("-- dest2 == \"%s\"\n", dest2);
	printf("-- src == \"%s\"\n\n", src);
	printf("-- strcat == \"%s\"\n", strncat(dest1, src, n));
	printf("-- Strcat == \"%s\"\n\n", Strncat(dest2, src, n));
	printf("---------------------\n\n");
	++number;
}

static void CheckStrStr(char *haystack, char *needle)
{
	static int number = 1;

	printf("-- input %d\n", number);
	printf("-- haystack == \"%s\"\n", haystack);
	printf("-- needle   == \"%s\"\n\n", needle);
	printf("-- strstr == \"%s\"\n", strstr(haystack, needle));
	printf("-- Strstr == \"%s\"\n\n", Strstr(haystack, needle));
	printf("---------------------\n\n");
	++number;
}

static void CheckStrSpn(const char *s1, const char *s2)
{
	static int number = 1;

	printf("-- input %d\n", number);
	printf("-- s1 == \"%s\"\n", s1);
	printf("-- s2 == \"%s\"\n\n", s2);
	printf("-- strspn == %ld\n", strspn(s1, s2));
	printf("-- Strspn == %ld\n\n", Strspn(s1, s2));
	printf("---------------------\n\n");
	++number;
}

int main(int ac, char **av)
{
	char dest1[] = "12345";
	char dest2[] = "12345";
	char dest3[] = "";
	char dest4[] = "";
	char dest5[] = "ab";
	char dest6[] = "ab";

	if (ac == 2)
	{
		if (atoi(av[1]) == 1)
		{
			printf("-- testing Strlen --\n\n");
			CheckStrlen("12345");
			CheckStrlen("");
		}
		else if (atoi(av[1]) == 2)
		{
			printf("-- testing Strcmp --\n\n");
			CheckStrcmp("123", "123");
			CheckStrcmp("12345", "12");
			CheckStrcmp("12", "12345");
			CheckStrcmp("","123");
			CheckStrcmp("123","");
			CheckStrcmp("","");
		}
		else if (atoi(av[1]) == 3)
		{
			printf("-- testing Strcpy --\n\n");
			CheckStrcpy(dest1, dest2, "123");
			CheckStrcpy(dest3, dest4, "123");
			CheckStrcpy(dest5, dest6, "123");
		}
		else if (atoi(av[1]) == 4)
		{
			printf("-- testing Strncpy --\n\n");
			CheckStrncpy(dest1, dest2, "abcdefgh", 2);
			CheckStrncpy(dest3, dest4, "12345", 6);
		}
		else if (atoi(av[1]) == 5)
		{
		/*	printf("-- testing StrNcasecmp --\n\n");

			CheckStrNcasecmp("abcd", "ABCD", 2);
			CheckStrNcasecmp("@@@", "   ", 10);
			CheckStrNcasecmp("", "123", 10);
			CheckStrNcasecmp("321", "", 10);
			CheckStrNcasecmp("###", "abc", 0);*/
		}
		else if (atoi(av[1]) == 6)
		{
			printf("-- testing Strchr --\n\n");

			CheckStrChr("123a456", 'a');
			CheckStrChr("", '\0');
			CheckStrChr("123a456", 'z');
		}
		else if (atoi(av[1]) == 7)
		{
			printf("-- testing StrDup --\n\n");

			CheckStrDup("string");
			CheckStrDup("");
		}
		else if (atoi(av[1]) == 8)
		{
			char d1[8] = {'1','2','\0','h','\0','\0','\0','\0',};
			char d2[8] = {'1','2','\0','h','\0','\0','\0','\0',};
			char d3[8] = {'\0','2','\0','h','\0','\0','\0','\0',};
			char d4[8] = {'\0','2','\0','h','\0','\0','\0','\0',};

			printf("-- testing Strcat --\n\n");
			CheckStrCat(d1, d2, "34");
			CheckStrCat(d3, d4, "34");
		}
		else if (atoi(av[1]) == 9)
		{
			char dn1[8] = {'1','2','\0','h','\0','\0','\0','\0',};
			char dn2[8] = {'1','2','\0','h','\0','\0','\0','\0',};
			char dn3[8] = {'\0','2','\0','h','\0','\0','\0','\0',};
			char dn4[8] = {'\0','2','\0','h','\0','\0','\0','\0',};

			printf("-- testing StrNcat --\n\n");

			CheckStrNCat(dn1, dn2, "3456", 0);
			CheckStrNCat(dn3, dn4, "3456", 1);
		}
		else if (atoi(av[1]) == 10)
		{
			printf("-- testing StrStr --\n\n");
			CheckStrStr("123abb4556", "abb");
			CheckStrStr("123abb4556", "a55");
			CheckStrStr("yes", "");
			CheckStrStr("", "");
			CheckStrStr("", "123");
		}
		else if (atoi(av[1]) == 11)
		{
			printf("-- testing StrSpn --\n\n");
			CheckStrSpn("12aaa6a", "123a");
			CheckStrSpn("123123321", "123");
			CheckStrSpn("aaaa1a", "aa");
			CheckStrSpn("12aaaa", "");
			CheckStrSpn("", "123");

		}
	}
	else
	{
		printf("one argument sbaba ahi?\n");
	}
	return 0;
}
