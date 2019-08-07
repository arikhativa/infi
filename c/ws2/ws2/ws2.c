
#include <stdio.h> /* printf for SevenBoom() */ 
#include <string.h> /* strrev for SringPulsString */ 
#include <stdlib.h> /* mallos() SringPulsString */ 
#include <assert.h> /* assert() */
#include "ws2.h" 
#include "ilrd_string.h" 


int IsPalindrome(const char *str)
{
	int len = 0;
	int index = 0;

	assert(str);

	if (*str)
	{
		len = Strlen(str);
		--len;
	}
	while (index < len)
	{
		if (str[index] != str[len])
		{
			return 0;
		}
		++index;
		--len;
	}

	return 1;
}


/* ContainSeven() and DivisibleBySeven() are for SevenBoom */

static int ContainSeven(int number)
{
	while (number)
	{
		if ((number % 10) == 7)
		{
			return 1;
		}
		number /= 10;
	}

	return 0;
}

static int DivisibleBySeven(int number)
{
	if ((number % 7) == 0)
	{
		return 1;
	}

	return 0;
}

void SevenBoom(int start, int end)
{
	assert(start);
	assert(end);

	while (start <= end)
	{
		if (!(ContainSeven(start)) && !(DivisibleBySeven(start)))
		{
			printf("%d\n", start);
		}
		else
		{
			printf("BOOM\n");
		}
		++start;
	}
}

void SwapPointers(int **ptr1, int **ptr2)
{
	int *swap = 0;

	assert(ptr1);
	assert(ptr2);

	swap = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = swap;
}


/* Spacei() is for RmSpaces() */

static char *Space(char *s)
{
	while (SPACE)
	{
		s++;
	}
	return s;
}

void RmSpaces(char *str)
{
	char *tmp = 0;
	char *origin = 0;

	tmp = Strdup(str);
	
	assert(tmp);

	origin = tmp;
	tmp = Space(tmp);
	while (*tmp)
	{
		while (NOSPACE)
		{
			*str = *tmp;
			++str;
			++tmp;
		}
		*str = *tmp;
		++str;
		tmp = Space(tmp);
	}
	if (*(str - 1) == '\t' || *(str - 1) == ' ')
	{
		*(str - 1) = '\0';
	}
	free(origin);
}

/* Strrev() MakeStr() are for StringplusString  */

static char *Strrev(char *str)
{
	int start = 0;
	int end = 0;
	char tmp = 0;

	assert(str);

	end = Strlen(str) - 1;
	while (start < (end - start))
	{
		tmp = str[start];
		str[start] = str[end - start];
		str[end - start] = tmp;
		++start;
	}
	return str;
}

static void MakeStr(char *new_str, char big_str, char small_str, int *reminder)
{
	char c = (big_str - 48) + (small_str - 48) + *reminder;

	*reminder = c / 10;
	c %= 10;
	*new_str = (c + 48);
}

char *StringPlusString(char *s1, char *s2)
{
	int big_size = 0;
	int reminder = 0;
	char *new_str = 0;
	char *big_str = 0;
	char *small_str = 0;

	assert(s1);
	assert(s2);

	if (Strlen(s1) > Strlen(s2))
	{
		big_size = Strlen(s1);
		big_str = s1;
		small_str = s2;
	}
	else
	{	big_size = Strlen(s2);
		big_str = s2;
		small_str = s1;
	}
	new_str = (char*)malloc(sizeof(char) * (big_size + 2));

	assert(new_str);

	big_size = 0;
	Strrev(big_str);
	Strrev(small_str);
	while (*small_str)
	{
		MakeStr(new_str + big_size, *big_str, *small_str, &reminder);
		++big_str;
		++small_str;
		++big_size;
	}
	while (*big_str)
	{
		MakeStr(new_str + big_size, *big_str, '0', &reminder);
		++big_str;
		++big_size;
	}
	if (reminder == 1)
	{
		new_str[big_size++] = (reminder + 48);
	}
	new_str[big_size] = '\0';
	Strrev(new_str);
	return new_str;
}
