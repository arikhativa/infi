
#include <stdio.h> /* printf() */
#include <stdlib.h> /* atoi() */
#include <string.h> /* strcmp() */

#define BIT_SIZE 32

static void PrintBit(unsigned int number, int digits)
{
	--digits;
	while (digits + 1)
	{
		(number & (1 << (digits))) ? printf("1") : printf("0");
		--digits;
		if (digits == 7 || digits == 23 || digits == 15)
		{
			printf("-");
		}
	}
	printf("\n");
}

static char **MakeByteToString(unsigned int number)
{
	unsigned int digits = 31;
	char **new = 0;
	char *tmp = 0;
	char *run = 0;
	char **ret = 0;

	new = (char**)malloc(sizeof(char*) * 5);
	tmp = (char*)malloc(sizeof(char) * 9);
	new[4] = NULL;
	tmp[8] = '\0';
	run = tmp;
	ret = new;

	while (digits + 1)
	{
		*run = (number & (1 << digits)) ? '1' : '0';
		--digits;
		++run;
		if (digits == 7 || digits == 23 || digits == 15)
		{
			*new = tmp;
			tmp = (char*)malloc(sizeof(char) * 9);
			tmp[8] = '\0';
			run = tmp;
			++new;
		}
	}
	*new = tmp;

	return ret;
}

static unsigned int MirrorByte(unsigned int number)
{
	unsigned int revers_number = 0;
	unsigned int tmp = 0;

	tmp = number << 24;
	revers_number ^= tmp;
	tmp = number >> 8;
	tmp = tmp << 24;
	tmp = tmp >> 8;
	revers_number ^= tmp;
	tmp = number >> 24;
	revers_number ^= tmp;
	tmp = number << 8;
	tmp = tmp >> 24;
	tmp = tmp << 8;
	revers_number ^= tmp;

	return revers_number;
}

static unsigned int MirrorBit(unsigned int number)
{
	unsigned int revers_number = 0;
	int index = 0;

	while (BIT_SIZE > index)
	{
		if (number & (1 << index))
		{
			revers_number |= (1 << ((BIT_SIZE - 1)  - index));
		}
		++index;
	}

	return revers_number;
}

static int TabCmp(char **tab1, char **tab2)
{
	int n = 3;

	while (*tab2)
	{
		if (0 != (strcmp(*(tab1 + n), *tab2)))
		{
			printf("-- MirrorByte dose not work\n");
			return 0;
		}
		--n;
		++tab2;
	}

	return 1;
}

int main()
{
	unsigned int number1 = 4000;
	char **tab1 = 0;
	char **tab2 = 0;

	printf("-- Test One\n");
	printf("-- Soruce - ");
	PrintBit(number1, BIT_SIZE);
	printf("-- Mirror - ");
	PrintBit(MirrorBit(number1), BIT_SIZE);

	printf("\n-- Test Two\n");
	printf("-- Soruce - ");
	PrintBit(number1, BIT_SIZE);
	tab1 = MakeByteToString(number1);
	printf("-- Mirror - ");
	PrintBit(MirrorByte(number1), BIT_SIZE);
	tab2 = MakeByteToString(MirrorByte(number1));
	if ((TabCmp(tab1, tab2)))
		printf("-- MirrorByte work\n");


	return 0;
}
