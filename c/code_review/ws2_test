
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */
#include "ws2.h"
#include "ilrd_string.h"


static void CheckIsPalindrom(const char *str)
{
        static int number = 1;

        printf("-- input %d\n", number);
        printf("-- str == \"%s\"\n", str);	
        printf("-- Is it plaindrom..?\n\n");
        (IsPalindrome(str)) ? printf("yes :)\n\n") : printf("no..\n\n");
        printf("---------------------\n\n");
        ++number;
}

static void CheckSevenBoom(int start, int end)
{
        static int number = 1;

        printf("-- input %d\n", number);
        printf("-- start == %d\n", start);
        printf("-- end   == %d\n\n", end);
	SevenBoom(start, end);
        printf("---------------------\n\n");
        ++number;
}

static void CheckSwapPointers(int *ptr1, int *ptr2)
{
        static int number = 1;

        printf("-- input %d\n", number);
        printf("-- ptr1 == %d\n", *ptr1);
        printf("-- ptr2 == %d\n\n", *ptr2);
        printf("-- Swap\n\n");
	SwapPointers(&ptr1, &ptr2);
        printf("-- ptr1 == %d\n", *ptr1);
        printf("-- ptr2 == %d\n\n", *ptr2);
        printf("---------------------\n\n");
        ++number;
}

static void CheckRmSpace(char *str)
{
	 static int number = 1;

        printf("-- input %d\n", number);
        printf("-- str before == \"%s\"\n\n", str);
	RmSpaces(str);
        printf("-- str after  == \"%s\"\n\n", str);
        printf("---------------------\n\n");
        ++number;
}

static void CheckStringPlusString(char *s1, char *s2)
{
	static int number = 1;
	char *tmp;

        printf("-- input %d\n", number);
        printf("-- s1  == \"%s\"\n", s1);
        printf("-- s2  == \"%s\"\n\n", s2);
	printf("-- New String:\n");
        printf("-- New == \"%s\"\n\n", tmp = StringPlusString(s1,s2));
	free(tmp);
        printf("---------------------\n\n");
        ++number;
}


int main(int ac, char **av)
{
	/* pointers for Swap Pointers */
	int s1[1];
	int s2[1];

	/* strings for Rm Space */
	char str1[] = "1   2  33 ";
	char str2[] = "   1	       2  33";
	char str3[] = "   1 	  2  33 ";

	/* strings for StrPulsStr */
	char str4[] = "9999999999999999999999999999999999999999999999999999999999999999999999";
	char str5[] = "9999999999999999999999999999999999999999999999999999999999999999999999";
	char str6[] = "123";
	char str7[] = "9999";
	char str8[] = "8888";
	char str9[] = "45";

	/* pointers for Swap Pointers */
	s1[0] = 5;
	s2[0] = 7;


	if (ac == 2)
	{
		if (atoi(av[1]) == 1)
		{
			printf("-- testing IsPalindrom --\n\n");
			CheckIsPalindrom("12321");
			CheckIsPalindrom("1");
			CheckIsPalindrom("  ");
			CheckIsPalindrom("123");
		}
		if (atoi(av[1]) == 2)
		{
			printf("-- testing Seven Boom --\n\n");
			CheckSevenBoom(-10, 57);
			CheckSevenBoom(10, -30);
		}

		if (atoi(av[1]) == 3)
		{
			printf("-- testing Swap Pointers --\n\n");
			CheckSwapPointers(s1, s2);
		}
		if (atoi(av[1]) == 4)
		{
			printf("-- testing RM Space --\n\n");

			CheckRmSpace(str1);
			CheckRmSpace(str2);
			CheckRmSpace(str3);
		}
		if (atoi(av[1]) == 5)
		{
			printf("-- testing StringPlusString --\n\n");
			CheckStringPlusString(str4, str5);
			CheckStringPlusString(str6, str7);
			CheckStringPlusString(str8, str9);

		}
	}
	else
	{
		printf("one argument sbaba ahi?\n");
	}

	return 0;
}
