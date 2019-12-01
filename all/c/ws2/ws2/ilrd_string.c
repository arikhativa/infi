
#include "ilrd_string.h"
#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc() */
#include <ctype.h> /* isupper() */
#include <assert.h> /* assert() */



#include <stdio.h>


size_t Strlen(const char *str)
{
	const char *runner = str;

	assert(str);

	while (*runner)
	{
		++runner;
	}

	return runner - str;
}

int Strcmp(const char *s1, const char *s2)
{
	assert(s1);
	assert(s2);

	while (*s1 && *s1 == *s2)
	{
		++s1;
		++s2;
	}

	return (*s1 - *s2);
}

char *Strcpy(char *dest, const char *src)
{
	char *pointer = dest;

	assert(src);
	while (*src)
	{
		*dest = *src;
		++dest;
		++src;
	}
	*dest = '\0';

	return pointer;
}

char *Strncpy(char *dest, const char *src, size_t n)
{
	char *pointer = dest;

	assert(src);

	while (*src && n > 0)
	{
		*dest = *src;
		++dest;
		++src;
		--n;
	}
	if (n != 0)
	{
		*dest = '\0';
	}

	return pointer;
}

int Strncasecmp(const char *s1, const char *s2, size_t n)
{
	char a = *s1;
	char b = *s2;

	assert(s1);
	assert(s2);

	if (n == 0)
	{
		return 0;
	}
	while (*s1 && *s2 && 0 != n)
	{
		a = (isupper(*s1)) ? *s1 + 32 : *s1;
		b = (isupper(*s2)) ? *s2 + 32 : *s2;
		if (a != b)
		{
			return (a - b);
		}
		++s1;
		++s2;
		--n;
	}

	return (a - b);
}

char *Strchr(const char *s, int c)
{
	assert(s);

	if (*s == c)
	{
		return (char*)s;
	}
	while (*s)
	{
		if (*s == c)
		{
			return (char*)s;
		}
		s++;
	}

	return (NULL);
}

char *Strdup(const char *s)
{
	char *new_string;

	assert(s);

	new_string = (char*)malloc((sizeof(char) * (Strlen(s) + 1)));

	assert(new_string);

	return (Strcpy(new_string, s));
}

char *Strcat(char *dest, const char *src)
{
	char *pointer = dest;

	assert(dest);
	assert(src);

	while (*dest)
	{
		++dest;
	}
	while (*src)
	{
		*dest = *src;
		++src;
		++dest;
	}
	*dest = '\0';

	return pointer;
}

char *Strncat(char *dest, const char *src, size_t n)
{
	char *pointer = dest;

	assert(dest);
	assert(src);

	while (*dest)
	{
		++dest;
	}
	while (*src && 0 != n)
	{
		*dest = *src;
		++src;
		++dest;
		--n;
	}
	*dest = '\0';

	return pointer;
}

char *Strstr(const char *haystack, const char *needle)
{
	size_t index = 0;


	assert(haystack);
	assert(needle);

	if (*needle == '\0')
	{
		return (char*)haystack;
	}
	while (*haystack)
	{
		while (*(haystack + index) == needle[index])
		{
			++index;
			if (needle[index] == '\0')
			{
				return (char*)haystack;
			}
		}
		index = 0;
		++haystack;
	}

	return NULL;
}

size_t Strspn(const char *s, const char *accept)
{
	size_t count = 0;
	size_t index = 0;

	assert(s);
	assert(accept);

	while (*s && accept[index])
	{
		if (*s == accept[index])
		{
			++count;
			++s;
			index = 0;
		}
		else
		{
			++index;
		}
	}

	return count;
}

char *Strtok(char *s, const char *delim)
{
	char *result;
	static char *old_str;
	const char *runner = delim;

	assert(s);
	assert(delim);

	if (s != NULL)
	{
		old_str = s;
	}
	if (old_str == NULL)
	{
		return NULL;
	}
	while (*old_str && *runner)
	{
		if (*old_str == *runner)
		{
			++old_str;
			runner = delim;
		}
		else
		{
			++runner;
		}
	}
	if (*old_str == '\0')
	{
		return NULL;
	}
	runner = delim;
	result = old_str;
	while (*old_str)
	{
		while (*runner)
		{
			if (*old_str == *runner)
			{
				*old_str = '\0';
				++old_str;
				return result;
			}
			++runner;
		}
		runner = delim;
		++old_str;
	}
	old_str = NULL;

	return result;
	
}
