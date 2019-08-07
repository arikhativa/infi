
#ifndef __WS2__H__
#define __WS2__H__

#define SPACE *s == ' ' || *s == '\t'
#define NOSPACE *tmp != ' ' && *tmp != '\t' && *tmp != '\0'

int IsPalindrome(const char *str);

void SevenBoom(int start, int end);

void SwapPointers(int **ptr1, int **ptr2);

void RmSpaces(char *str);

char *StringPlusString(char *s1, char *s2);

#endif /* __WS2__H__ */
