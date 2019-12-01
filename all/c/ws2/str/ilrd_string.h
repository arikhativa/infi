
#ifndef __ILRD_STRING_H__
#define __ILRD_STRING_H__

#include <stddef.h>

/* Return the length of the string s. */
size_t Strlen(const char *s);

/*Compare the strings s1 with s2. */
int Strcmp(const char *s1, const char *s2);

/* Copy the string src to dest, returning a pointer to the start of
dest. */
char *Strcpy(char *dest, const char *src);


/* Copy at most n bytes  from  string  src  to  dest,  returning  a
pointer to the start of dest.*/
char *Strncpy(char *dest, const char *src, size_t n);

/* Compare the first n characters of the strings s1 and s2 ignoring
case. */
int Strncasecmp(const char *s1, const char *s2, size_t n);

/* Return  a  pointer to the first occurrence of the character c in
the string s. */
char *Strchr(const char *s, int c);

/* Return a duplicate of the string s  in  memory  allocated  using
malloc(3). */
char *Strdup(const char *s);

/* Append the string src to the string dest,  returning  a  pointer
dest. */
char *Strcat(char *dest, const char *src);

/* Append  at  most  n characters from the string src to the string
dest, returning a pointer to dest. */
char *Strncat(char *dest, const char *src, size_t n);

/* Find  the first occurrence of the substring needle in the string
haystack, returning a pointer to the found substring. */
char *Strstr(const char *haystack, const char *needle);

/* Calculate the length of the starting segment  in  the  string  s
that consists entirely of bytes in accept. */
size_t Strspn(const char *s, const char *accept);

/* Extract tokens from the string s that are delimited  by  one  of
the bytes in delim. */
char *Strtok(char *s, const char *delim);


#endif /*__ILRD_STRING_H__*/

