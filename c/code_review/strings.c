#include "ilrd_string.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

size_t Strlen(const char *str)
{
	int length = 0;
	
	assert(str);
	
	while(*str)
	{
		++str;
		++length;
	}	
	return length;
}


int Strcmp(const char *str1, const char *str2)
{
	assert(str1);
	assert(str2);
	
	while (*str1 || *str2)
	{
		++str1;
		++str2;
	}	
	return (*str1 - *str2);
}

char *Strcpy(char *dest, const char *src)
{
	char *runner = dest;
	
	assert(dest);
	assert(src);
	
	while (*src)
	{
		*runner = *src;
		++runner;
		++src;
	}
	*runner = '\0';
	
	return dest;
}	

char *Strncpy(char *dest, const char *src, size_t num)
{
	size_t i = 0;
	char *runner = dest;
	
	assert(dest);
	assert(src);
	
	for (i = 0 ; i < num && *src != 0; ++i)
	{
		*runner = *src ;
		++runner;
		++src;
	}	
	*runner = '\0';
	
	return dest;
}	

int Strcasecmp(const char *str1, const char *str2)
{
	assert(str1);
	assert(str2);
		
	while(*str1 && (*str1 == *str2 || abs(*str1 - *str2) == 32))		
	{
		++str1;
		++str2;
	}
		
	return (*str1 - *str2);
}

char *Strchr(const char *str, int ch)
{
	
	assert(str);
	
	while(*str != ch)
	{
		++str;
		if(*str == 0)
		{
			return NULL;
		} 
	}	
	return (char *)str;
}
	
char *Strdup(const char *str)
{
	char *dup = NULL; 
	char *runner = NULL; 
	
	assert(str);
	
	dup = (char *)malloc(sizeof(char)*strlen(str) + 1);
	runner = dup;
	while(*str)
	{
		*runner = *str;
		++runner;
		++str;
	}
	*runner = '\0';
			
	return dup;
} 	

char *Strcat(char *dest, const char *src)
{
	char *tmp = dest;
	
	assert(dest);
	assert(src);
	
	while(*tmp)
	{
		++tmp;
	}
	
	while(*src)
	{
		*tmp = *src;
		++tmp;
		++src;
	}
	*tmp = '\0';
	
	return dest;
}
	
char *Strncat(char *dest, const char *src, size_t num)
{
	char *runner = NULL;
	size_t count = 0;
	
	assert(dest);
	assert(src);
	
	runner = dest;	
	while(*runner)
	{
		++runner;
	}	
	for(count = 0 ; count < num && *src != 0; ++count)
	{
		*runner = *src;
		++runner;
		++src;
	}	
	*runner = 0;
	
	return dest;
}

char *Strstr(const char *haystack, const char *needle)
{
	int count = 0;
	char *nfound = "not found";
	
	assert(haystack);
	assert(haystack);
	
	if (*needle == 0)
	{
		return (char *)haystack;
	}
	
	while(*haystack)
	{
		while (*needle == *haystack)
		{
			++needle;
			++haystack;
			++count;
			if (*needle == 0)
			{
				return ((char *)(haystack-count));
			}
		}
		needle =needle - count;
		count = 0;
		++haystack;
	}
	return nfound;
}

size_t Strspn(const char *str, const char *accept)
{
	size_t count = 0;
	
	assert(str);
	assert(accept);
	
	while(*str)
	{
		if (!Strchr(accept, *str)) /*check if the letter exists in the accept string*/
		{	
			return count; 
		}
		++count;
		++str;
	}
	return count;
}
		
	
	
	
