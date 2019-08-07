#include <ctype.h> /*tolower, toupper*/
#include <stdlib.h> /*for malloc, realloc, free*/
#include <string.h> /*str/ncpy, strcmp, strchr, strstr */
#include <assert.h> /*for asserts*/

/* Return the length of the string s. */

size_t Strlen(const char *s)
{
    const char *runner = s;
    
    assert(s);
	
	while (0 != *runner)
	{
		++runner;
	}
	
	return runner - s;
}

/*Compare the strings s1 with s2. */

int Strcmp(const char *s1, const char *s2) 
{
    assert(s1);
    assert(s2);
    
	if(*s1 == '\0' || *s2 == '\0')
	{
	    return *s1 - *s2;
	}
	while(*s1 == *s2 && *s1) 
	{
		++s1;
		++s2;
		
		if(*s1 != *s2)
		{
            return *s1 - *s2;
		}
	}
	
    return 0;
}

/* Copy the string src to dest, returning a pointer to the start of
dest. */

char *Strcpy(char *dest, const char *src)
{
    char *runner = dest;
    
    assert(dest);
    assert(src);
   
	while(*src)
	{
	    *runner = *src; 
	    ++src;
	    ++runner;   
	}
	*runner = '\0';
	
	return dest;
}

/* Copy at most n bytes  from  string  src  to  dest,  returning  a
pointer to the start of dest.*/
char *Strncpy(char *dest, const char *src, size_t n)
{
	char *ptr = dest;
	
	assert(dest);
	assert(src);
	
	while(*src != '\0' && n != 0)
	{
	    --n;
	    *dest = *src;
	    ++src;
	    ++dest;
	}
	if(n == 0)
	{
	    return ptr;
	}
	else
	{
	    *dest = '\0';
	}
	
	return ptr;
}

/* Compare the first n characters of the strings s1 and s2 ignoring
case. */
int Strcasecmp(const char *s1, const char *s2)
{    
    const char *ptr1 = s1, *ptr2 = s2;
    int result;
       
    assert(s1);
    assert(s2);
       
    if (ptr1 == ptr2)
    {
        return 0;
    }
    
    while ((result = tolower(*ptr1) - tolower(*ptr2++)) == 0)
    {
        if (*ptr1++ == '\0')
        {
            break;
        }
    }
    
    return result;
}

/* Return  a  pointer to the first occurrence of the character c in
the string s. */
char *Strchr(const char *s, int c)
{	
	assert(s);
	
	while(*s != c && *s != '\0')
	{
	    ++s;
	}
	if(*s == '\0')
	{
	    return NULL;
	}
	
	return (char *)s;
}

/* Return a duplicate of the string s  in  memory  allocated  using
malloc(3). */
char *Strdup(const char *s)
{
    char *dest = (char *)malloc(sizeof(char) * (Strlen(s) + 1));
    
    assert(s);
    
    if(dest == NULL)
    {
        return NULL;
    }
    Strcpy(dest, s);
    
    return dest;
}

/* Append the string src to the string dest,  returning  a  pointer
dest. */
char *Strcat(char *dest, const char *src)
{
    assert(src);
    assert(dest);
    
    Strcpy(dest + Strlen(dest), src);
    
    return dest;
}

/* Append  at  most  n characters from the string src to the string
dest, returning a pointer to dest. */
char *Strncat(char *dest, const char *src, size_t n)
{  
    char *runner = dest;
    
    assert(dest);
    assert(src);
    
    while(*runner)
    {
        ++runner;
    }
    
    while(*src && 0 < n)
    {
        *runner = *src;
        ++runner;
        ++src;
        --n;
    }
    
    *runner = '\0';
    
    return dest;
}

/* Find  the first occurrence of the substring needle in the string
haystack, returning a pointer to the found substring. */
char *Strstr(const char *haystack, const char *needle)
{
    int i = 0;
    
    assert(haystack);
    assert(needle);
    
    if(*needle == '\0')
    {
        return (char *)haystack;
    }
    while(*haystack != '\0')
    {
        while(haystack[i] == needle[i]) 
        {       
            ++i;
            if(needle[i] == '\0')
                return (char *)haystack;
        }    
        i = 0;
        ++haystack;       
     }
     
     return NULL; 
}

































